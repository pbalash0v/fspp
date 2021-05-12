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

#ifndef __fs_cfg_hpp__
#define __fs_cfg_hpp__

#include <iostream>
#include <filesystem>
#include <unordered_map>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "freeswitch_xml.hpp"

namespace fs = std::filesystem;
namespace bpt = boost::property_tree;


namespace fspp
{
struct config;

//! Provides config for FreeSWITCH via programmatical xml generation
struct fs_cfg final
{
	fs_cfg(fspp::config);

	//! Filled in switch_xml_bind_search_function provided callback
	struct conf_tuple
	{
		std::string_view section;
		std::string_view tag;
		std::string_view key;
		std::string_view value;
	};
	//! Returns generated xml string if requested config section was found
	std::optional<std::string> operator()(const conf_tuple&) const;

	static bpt::ptree modules_conf(const fs_cfg&);
	static bpt::ptree switch_conf(const fs_cfg&);
	static bpt::ptree event_socket_conf(const fs_cfg&);
	static bpt::ptree console_conf(const fs_cfg&);
	static bpt::ptree logfile_conf(const fs_cfg&);
	static bpt::ptree sofia_conf(const fs_cfg&);

	//! User-provided fspp lib config
	fspp::config cfg_;
	//! RAII create and fill w/ minimal content "freeswitch.xml" file in fspp base_dir
	freeswitch_dot_xml freeswitch_dot_xml_;
	//! Functions to be called on corresponding FS xml engine callbacks
	std::unordered_map<std::string, std::function<bpt::ptree(const fs_cfg&)>> config_funcs_
	{
		{"modules.conf", &fs_cfg::modules_conf},
		{"switch.conf", &fs_cfg::switch_conf},
		{"event_socket.conf", &fs_cfg::event_socket_conf},
		{"console.conf", &fs_cfg::console_conf},
		{"logfile.conf", &fs_cfg::logfile_conf},
		{"sofia.conf", &fs_cfg::sofia_conf}
	};
};


} // namespace fspp

#endif //__fs_cfg_hpp__