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

struct fs_cfg final
{
	fs_cfg() = default;

	fs_cfg(const fs_cfg&) = delete;
	fs_cfg&	operator=(const fs_cfg&) = delete;
	fs_cfg(fs_cfg&&) = default;
	fs_cfg&	operator=(fs_cfg&&) = default;

	struct conf_tuple
	{
		std::string_view section;
		std::string_view tag;
		std::string_view key;
		std::string_view value;
	};
	std::optional<std::string> operator()(const conf_tuple&) const;

	static bpt::ptree modules_conf();
	static bpt::ptree switch_conf();
	static bpt::ptree event_socket_conf();
	static bpt::ptree console_conf();
	static bpt::ptree logfile_conf();
	static bpt::ptree sofia_conf();

	//
	fspp::freeswitch_xml freeswitch_xml_;
	std::unordered_map<std::string, std::function<bpt::ptree()>> config_funcs_ {
		{"modules.conf", fs_cfg::modules_conf},
		{"switch.conf", fs_cfg::switch_conf},
		{"event_socket.conf", fs_cfg::event_socket_conf},
		{"console.conf", fs_cfg::console_conf},
		{"logfile.conf", fs_cfg::logfile_conf},
		{"sofia.conf", fs_cfg::sofia_conf},
	};
};


} // namespace fspp

#endif //__fs_cfg_hpp__