#include <iostream>
#include <boost/throw_exception.hpp>

#include <switch.h>

#include "utils.hpp"
#include "fspp_impl.hpp"
#include "fs_cfg.hpp"



namespace
{

auto on_xml_search = [](const char* section, const char* /*tag_name*/, const char* key_name, const char* key_value
	, switch_event_t* , void* user_data) -> switch_xml_t
{
	auto* self = static_cast<fspp::lib_impl*>(user_data);

	// std::cerr << "--on_xml_search--" << '\n';
	// std::cerr << "section: " << section << '\n';
	// std::cerr << "tag_name: " << tag_name << '\n';
	// std::cerr << "key_name: " << key_name << '\n';
	// std::cerr << "key_value: " << key_value << '\n';
	// std::cerr << "----" << '\n';

	if (auto xml_str = self->cfg()({section, key_name, key_value}); xml_str)
	{
		return switch_xml_parse_str_dup(xml_str->data());
	}
	else
	{
		return nullptr;
	}
};

}


namespace fspp
{

lib_impl::lib_impl()
{
	// loads internal SWITCH_GLOBAL_dirs struct of char* with autoconf generated values (?)
    ::switch_core_set_globals();

	// std::cerr << "Initial SWITCH_GLOBAL_dirs\n";
	// print_SWITCH_GLOBAL_dirs();
	init_SWITCH_GLOBAL_dirs();
	//std::cerr << "fspp initialized SWITCH_GLOBAL_dirs\n";
	//print_SWITCH_GLOBAL_dirs();

    //
    switch_core_flag_t flags {SCF_USE_SQL};
	bool console {true};
    const char* err {nullptr};

	if (auto res = ::switch_core_init(flags, console ? SWITCH_TRUE : SWITCH_FALSE, &err); res != SWITCH_STATUS_SUCCESS)
	{
		BOOST_THROW_EXCEPTION(std::runtime_error{static_cast<const char*>(err)});
	}

	::switch_xml_bind_search_function(on_xml_search, SWITCH_XML_SECTION_CONFIG, this);

   	if (auto res = ::switch_core_init_and_modload(flags, console ? SWITCH_TRUE : SWITCH_FALSE, &err); res != SWITCH_STATUS_SUCCESS)
   	{
   		BOOST_THROW_EXCEPTION(std::runtime_error{static_cast<const char*>(err)});
   	}
}

lib_impl::~lib_impl()
{
	[[maybe_unused]] auto destroy_status = ::switch_core_destroy();
	BOOST_ASSERT(destroy_status == SWITCH_STATUS_SUCCESS);
}

void lib_impl::operator()()
{
    //print_SWITCH_GLOBAL_dirs();

    // blocks here in console or stdout
	::switch_core_runtime_loop(false);
    //print_SWITCH_GLOBAL_dirs();
}


void lib_impl::init_SWITCH_GLOBAL_dirs()
{
	auto dup_c_str = [](const std::string& str, auto*& dest)
	{
		switch_safe_free(dest);

		char* res = strdup(str.c_str());
		if (not res)
		{
			BOOST_THROW_EXCEPTION(std::runtime_error{"strdup failed"});
		}

		dest = res;
	};

	dup_c_str(m_fs_cfg.freeswitch_xml_, SWITCH_GLOBAL_dirs.base_dir);
	dup_c_str(m_fs_cfg.freeswitch_xml_, SWITCH_GLOBAL_dirs.mod_dir);
	dup_c_str(m_fs_cfg.freeswitch_xml_, SWITCH_GLOBAL_dirs.conf_dir);
	dup_c_str(m_fs_cfg.freeswitch_xml_, SWITCH_GLOBAL_dirs.log_dir);
	dup_c_str(m_fs_cfg.freeswitch_xml_, SWITCH_GLOBAL_dirs.run_dir);
	dup_c_str(m_fs_cfg.freeswitch_xml_, SWITCH_GLOBAL_dirs.lib_dir);
	dup_c_str(m_fs_cfg.freeswitch_xml_, SWITCH_GLOBAL_dirs.temp_dir);
	dup_c_str(m_fs_cfg.freeswitch_xml_, SWITCH_GLOBAL_dirs.db_dir);	
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
