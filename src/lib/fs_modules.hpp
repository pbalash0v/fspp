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

#ifndef __fs_modules_hpp__
#define __fs_modules_hpp__

#include <filesystem>
#include <boost/throw_exception.hpp>
#include <boost/dll.hpp>
#include "fspp_config.hpp"

#include <switch.h>

namespace
{

extern "C"
{
extern switch_loadable_module_function_table_t CORE_SOFTTIMER_MODULE_module_interface;
extern switch_loadable_module_function_table_t CORE_PCM_MODULE_module_interface;
extern switch_loadable_module_function_table_t CORE_SPEEX_MODULE_module_interface;
extern switch_loadable_module_function_table_t mod_sofia_module_interface;
extern switch_loadable_module_function_table_t mod_event_socket_module_interface;
extern switch_loadable_module_function_table_t mod_commands_module_interface;
extern switch_loadable_module_function_table_t mod_logfile_module_interface;
extern switch_loadable_module_function_table_t mod_console_module_interface;
extern switch_loadable_module_function_table_t mod_dialplan_xml_module_interface;
}

} //anon namespace

namespace dll = boost::dll;

namespace fspp
{

class fs_modules final
{
public:
	fs_modules(const fspp::config& cfg)
	{
		if (not CORE_PCM_MODULE_module_interface_
			or not CORE_SPEEX_MODULE_module_interface_
			or not CORE_SOFTTIMER_MODULE_module_interface_
			or not mod_sofia_module_interface_
			or not mod_event_socket_module_interface_
			or not mod_commands_module_interface_
			or not mod_logfile_module_interface_
			or not mod_console_module_interface_
			or not mod_dialplan_xml_module_interface_
			)
		{
			BOOST_THROW_EXCEPTION(std::runtime_error{"Builtin FreeSWITCH modules was not proprely linked"});
		}

		fs::remove_all(fspp_conf_path);
		fs::create_directory(fspp_conf_path);

		// fake symlinks for python module
		if (cfg.python)
		{
			// core modules
			std::filesystem::create_symlink(dll::symbol_location_ptr(&CORE_SOFTTIMER_MODULE_module_interface), fspp_conf_path/"CORE_SOFTTIMER_MODULE.so");
			std::filesystem::create_symlink(dll::symbol_location_ptr(&CORE_PCM_MODULE_module_interface), fspp_conf_path/"CORE_PCM_MODULE.so");
			std::filesystem::create_symlink(dll::symbol_location_ptr(&CORE_SPEEX_MODULE_module_interface), fspp_conf_path/"CORE_SPEEX_MODULE.so");
			// common modules
			std::filesystem::create_symlink(dll::symbol_location_ptr(&mod_console_module_interface), fspp_conf_path/"mod_console.so");
			std::filesystem::create_symlink(dll::symbol_location_ptr(&mod_sofia_module_interface), fspp_conf_path/"mod_sofia.so");
			std::filesystem::create_symlink(dll::symbol_location_ptr(&mod_event_socket_module_interface), fspp_conf_path/"mod_event_socket.so");
			std::filesystem::create_symlink(dll::symbol_location_ptr(&mod_commands_module_interface), fspp_conf_path/"mod_commands.so");
			std::filesystem::create_symlink(dll::symbol_location_ptr(&mod_logfile_module_interface), fspp_conf_path/"mod_logfile.so");
			std::filesystem::create_symlink(dll::symbol_location_ptr(&mod_dialplan_xml_module_interface), fspp_conf_path/"mod_dialplan_xml.so");
		}
	}

private:
	const void* CORE_SOFTTIMER_MODULE_module_interface_ = static_cast<void*>(&CORE_SOFTTIMER_MODULE_module_interface);
	const void* CORE_PCM_MODULE_module_interface_ = static_cast<void*>(&CORE_PCM_MODULE_module_interface);
	const void* CORE_SPEEX_MODULE_module_interface_ = static_cast<void*>(&CORE_SPEEX_MODULE_module_interface);
	const void* mod_sofia_module_interface_ = static_cast<void*>(&mod_sofia_module_interface);
	const void* mod_event_socket_module_interface_ = static_cast<void*>(&mod_event_socket_module_interface);
	const void* mod_commands_module_interface_ = static_cast<void*>(&mod_commands_module_interface);
	const void* mod_logfile_module_interface_ = static_cast<void*>(&mod_logfile_module_interface);
	const void* mod_console_module_interface_ = static_cast<void*>(&mod_console_module_interface);
	const void* mod_dialplan_xml_module_interface_ = static_cast<void*>(&mod_dialplan_xml_module_interface);
};

} // namespace fspp

#endif