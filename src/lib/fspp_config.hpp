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

#ifndef __fspp_config_hpp__
#define __fspp_config_hpp__

#include <filesystem>

namespace fs = std::filesystem;

namespace fspp
{

constexpr auto* freeswitch_xml_ {"freeswitch.xml"};
constexpr auto* fspp_dir_ {"fspp"};

inline fs::path fspp_conf_path {fs::temp_directory_path() / fspp_dir_};
inline fs::path freeswitch_xml_path {fspp_conf_path / freeswitch_xml_};

} //namespace fspp

#endif //__fspp_config_hpp__