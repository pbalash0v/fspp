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


namespace dll = boost::dll;
namespace fs = std::filesystem;

#define EXTERN_FS_C_OBJ(symbol) extern switch_loadable_module_function_table_t symbol
#define PRIVATE_OBJ(symbol) const void* symbol ## _ = static_cast<void*>(&symbol)

namespace
{

extern "C"
{
EXTERN_FS_C_OBJ(CORE_SOFTTIMER_MODULE_module_interface);
EXTERN_FS_C_OBJ(CORE_PCM_MODULE_module_interface);
EXTERN_FS_C_OBJ(CORE_SPEEX_MODULE_module_interface);
EXTERN_FS_C_OBJ(mod_sofia_module_interface);
EXTERN_FS_C_OBJ(mod_event_socket_module_interface);
EXTERN_FS_C_OBJ(mod_commands_module_interface);
EXTERN_FS_C_OBJ(mod_logfile_module_interface);
EXTERN_FS_C_OBJ(mod_console_module_interface);
EXTERN_FS_C_OBJ(mod_dialplan_xml_module_interface);
}

const auto path_to_py_so = [](const auto& symbol)
{
	return dll::symbol_location(symbol).is_absolute() ?
		dll::symbol_location(symbol) : fs::current_path() / dll::symbol_location(symbol);
};

} //anon namespace


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
			fs::create_symlink(path_to_py_so(CORE_SOFTTIMER_MODULE_module_interface), fspp_conf_path/"CORE_SOFTTIMER_MODULE.so");
			fs::create_symlink(path_to_py_so(CORE_PCM_MODULE_module_interface), fspp_conf_path/"CORE_PCM_MODULE.so");
			fs::create_symlink(path_to_py_so(CORE_SPEEX_MODULE_module_interface), fspp_conf_path/"CORE_SPEEX_MODULE.so");
			// common modules
			fs::create_symlink(path_to_py_so(mod_console_module_interface), fspp_conf_path/"mod_console.so");
			fs::create_symlink(path_to_py_so(mod_sofia_module_interface), fspp_conf_path/"mod_sofia.so");
			fs::create_symlink(path_to_py_so(mod_event_socket_module_interface), fspp_conf_path/"mod_event_socket.so");
			fs::create_symlink(path_to_py_so(mod_commands_module_interface), fspp_conf_path/"mod_commands.so");
			fs::create_symlink(path_to_py_so(mod_logfile_module_interface), fspp_conf_path/"mod_logfile.so");
			fs::create_symlink(path_to_py_so(mod_dialplan_xml_module_interface), fspp_conf_path/"mod_dialplan_xml.so");
		}
	}

private:
	PRIVATE_OBJ(CORE_SOFTTIMER_MODULE_module_interface);
	PRIVATE_OBJ(CORE_PCM_MODULE_module_interface);
	PRIVATE_OBJ(CORE_SPEEX_MODULE_module_interface);
	PRIVATE_OBJ(mod_sofia_module_interface);
	PRIVATE_OBJ(mod_event_socket_module_interface);
	PRIVATE_OBJ(mod_commands_module_interface);
	PRIVATE_OBJ(mod_logfile_module_interface);
	PRIVATE_OBJ(mod_console_module_interface);
	PRIVATE_OBJ(mod_dialplan_xml_module_interface);
};

} // namespace fspp

#endif