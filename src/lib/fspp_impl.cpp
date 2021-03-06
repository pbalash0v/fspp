/*
	C++ FreeSWITCH wrapper library
	Copyright (C) 2021 Pavel Balashov

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <chrono>
#include <thread>
#include <random>

#include <boost/asio.hpp>
#include <boost/throw_exception.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <switch.h>

#include "fspp_impl.hpp"

namespace net = boost::asio;

namespace
{

const auto on_xml_search = [](const char* section, const char* tag_name, const char* key_name, const char* key_value
	, switch_event_t* , void* user_data) -> switch_xml_t
{
	auto* self = static_cast<fspp::lib_impl*>(user_data);

	// std::cerr << "--on_xml_search--" << '\n';
	// std::cerr << "section: " << section << '\n';
	// std::cerr << "tag_name: " << tag_name << '\n';
	// std::cerr << "key_name: " << key_name << '\n';
	// std::cerr << "key_value: " << key_value << '\n';
	// std::cerr << "----" << '\n';

	if (auto xml_str = self->cfg()({section, tag_name, key_name, key_value}); xml_str)
	{
		return switch_xml_parse_str_dup(xml_str->data());
	}
	else
	{
		return nullptr;
	}
};

const auto get_random_port = []()
{
	static std::random_device rd;     // only used once to initialise (seed) engine
	static std::mt19937 rng {rd()};    // random-number engine used (Mersenne-Twister in this case)
	static std::uniform_int_distribution<uint16_t> uni {49152u, std::numeric_limits<std::uint16_t>::max()}; // guaranteed unbiased

	return uni(rng);
};

[[nodiscard]] fspp::config modify_cfg(fspp::config cfg)
{
	constexpr auto* fspp_dir_ {"fspp"};
	static fs::path fspp_def_base {fs::temp_directory_path() / fspp_dir_}; // usually results in "/tmp/fspp"
	static boost::uuids::random_generator m_rand_gen;

	cfg.id = boost::uuids::to_string(m_rand_gen());
	if (cfg.base_path.empty())
	{
		// gives smth like "/tmp/fspp/cdb26bab-3265-4fb0-93ef-b46907c3bddd"
		cfg.base_path = (fspp_def_base / cfg.id).string();
	}

	return cfg;
}

[[nodiscard]] std::pair<net::ip::tcp::socket, net::ip::tcp::socket> adjust_esl_port(fspp::config& cfg)
{
	// Randomly assign ESL port
	if (cfg.esl_port == 0)
	{
		cfg.esl_port = get_random_port();
	}

	static net::io_context m_io_ctx;

	net::ip::tcp::socket esl_socket_v4 {m_io_ctx, net::ip::tcp::v4()};
	esl_socket_v4.set_option(net::ip::tcp::socket::reuse_address(true));
	esl_socket_v4.bind(net::ip::tcp::endpoint {net::ip::tcp::v4(), cfg.esl_port});

	net::ip::tcp::socket esl_socket_v6 {m_io_ctx, boost::asio::ip::tcp::v6()};
	esl_socket_v6.set_option(net::ip::tcp::socket::reuse_address(true));
	esl_socket_v6.bind(net::ip::tcp::endpoint {net::ip::tcp::v6(), cfg.esl_port});

	return std::pair{std::move(esl_socket_v4), std::move(esl_socket_v6)};
}

}


namespace fspp
{

lib_impl::lib_impl(fspp::config cfg)
	: cfg_{modify_cfg(cfg)}
	, dirs_{cfg_}
	, fs_modules_{cfg_}
	, fs_cfg_{cfg_}
{
	auto _ = adjust_esl_port(cfg_);

	// loads internal SWITCH_GLOBAL_dirs struct of char* with autoconf generated values (?)
	::switch_core_set_globals();

	// std::cerr << "Initial SWITCH_GLOBAL_dirs\n";
	// print_SWITCH_GLOBAL_dirs();
	init_SWITCH_GLOBAL_dirs();
	//std::cerr << "fspp initialized SWITCH_GLOBAL_dirs\n";
	//print_SWITCH_GLOBAL_dirs();

	//
	//switch_core_flag_t flags {SCF_USE_SQL};
	switch_core_flag_t flags{};
	const char* err {nullptr};

	if (auto res = ::switch_core_init(flags, cfg.console ? SWITCH_TRUE : SWITCH_FALSE, &err); res != SWITCH_STATUS_SUCCESS)
	{
		BOOST_THROW_EXCEPTION(std::runtime_error{static_cast<const char*>(err)});
	}

	::switch_xml_bind_search_function(on_xml_search, SWITCH_XML_SECTION_CONFIG | SWITCH_XML_SECTION_DIALPLAN, this);

	if (auto res = ::switch_core_init_and_modload(flags, cfg.console ? SWITCH_TRUE : SWITCH_FALSE, &err); res != SWITCH_STATUS_SUCCESS)
	{
		BOOST_THROW_EXCEPTION(std::runtime_error{static_cast<const char*>(err)});
	}

	// allow all module threads to load and start their runtimes, in particular prevent race in CORE_SOFTTIMER_MODULE shutdown
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

lib_impl::~lib_impl()
{
	if (auto destroy_status = ::switch_core_destroy(); destroy_status != SWITCH_STATUS_SUCCESS)
	{
		std::cerr << "switch_core_destroy failed" << '\n';
		BOOST_ASSERT(destroy_status == SWITCH_STATUS_SUCCESS);
	}
}

// blocks here in console or stdout
void lib_impl::operator()()
{
	::switch_core_runtime_loop(not cfg_.console);
}

void lib_impl::init_SWITCH_GLOBAL_dirs()
{
	const auto dup_c_str = [](const auto& str, auto*& dest)
	{
		switch_safe_free(dest);

		char* res = strdup(str.c_str());
		if (not res)
		{
			BOOST_THROW_EXCEPTION(std::runtime_error{"strdup failed"});
		}

		dest = res;
	};

	dup_c_str(cfg_.base_path, SWITCH_GLOBAL_dirs.base_dir);
	dup_c_str(cfg_.base_path, SWITCH_GLOBAL_dirs.mod_dir);
	dup_c_str(cfg_.base_path, SWITCH_GLOBAL_dirs.conf_dir);
	dup_c_str(cfg_.base_path, SWITCH_GLOBAL_dirs.log_dir);
	dup_c_str(cfg_.base_path, SWITCH_GLOBAL_dirs.run_dir);
	dup_c_str(cfg_.base_path, SWITCH_GLOBAL_dirs.lib_dir);
	dup_c_str(cfg_.base_path, SWITCH_GLOBAL_dirs.temp_dir);
	dup_c_str(cfg_.base_path, SWITCH_GLOBAL_dirs.db_dir);	
}

void lib_impl::print_SWITCH_GLOBAL_dirs()
{
	// struct switch_directories {
	// 	char *base_dir;
	// 	char *mod_dir;
	// 	char *conf_dir;
	// 	char *log_dir;
	// 	char *run_dir;
	// 	char *db_dir;
	// 	char *script_dir;
	// 	char *temp_dir;
	// 	char *htdocs_dir;
	// 	char *grammar_dir;
	// 	char *storage_dir;
	// 	char *cache_dir;
	// 	char *recordings_dir;
	// 	char *sounds_dir;
	// 	char *lib_dir;
	// 	char *certs_dir;
	// 	char *fonts_dir;
	// 	char *images_dir;
	// 	char *data_dir;
	// 	char *localstate_dir;
	// };
	// typedef struct switch_directories switch_directories;
	// SWITCH_DECLARE_DATA extern switch_directories SWITCH_GLOBAL_dirs;

	std::cerr << "SWITCH_GLOBAL_dirs.base_dir: " << SWITCH_GLOBAL_dirs.base_dir << "\n";
	std::cerr << "SWITCH_GLOBAL_dirs.mod_dir: " << SWITCH_GLOBAL_dirs.mod_dir << "\n";
	std::cerr << "SWITCH_GLOBAL_dirs.conf_dir: " << SWITCH_GLOBAL_dirs.conf_dir << "\n";
	std::cerr << "SWITCH_GLOBAL_dirs.log_dir: " << SWITCH_GLOBAL_dirs.log_dir << "\n";
	std::cerr << "SWITCH_GLOBAL_dirs.run_dir: " << SWITCH_GLOBAL_dirs.run_dir << "\n";
	std::cerr << "SWITCH_GLOBAL_dirs.lib_dir: " << SWITCH_GLOBAL_dirs.lib_dir << "\n";	
	std::cerr << "SWITCH_GLOBAL_dirs.temp_dir: " << SWITCH_GLOBAL_dirs.temp_dir << "\n";
	std::cerr << "----\n";
	std::cerr << "SWITCH_GLOBAL_filenames.conf_name: " << SWITCH_GLOBAL_filenames.conf_name << "\n";
	std::cerr << "----\n";
}

} // namespace fspp
