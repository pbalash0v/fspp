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

#ifndef __freeswitch_xml_hpp__
#define __freeswitch_xml_hpp__

#include <filesystem>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "fspp_config.hpp"

namespace fs = std::filesystem;
namespace bpt = boost::property_tree;


namespace fspp
{

class freeswitch_xml
{
public:	
	freeswitch_xml()
	{
		fs::create_directory(fspp_conf_path);

		auto freeswitch_dot_xml = []()
		{
			bpt::ptree document;
			document.put("document", "");
			document.put("<xmlattr>.type", "freeswitch/xml");
			return document;
		}();

		bpt::xml_parser::write_xml(freeswitch_xml_path.string(), freeswitch_dot_xml, std::locale(), bpt::xml_writer_make_settings<std::string>(' ', 2));
	}
	
	~freeswitch_xml()
	{
		std::error_code ec;

		fs::remove_all(fspp_conf_path, ec);
		if (ec)
		{
			std::cerr << ec << '\n';
		}
	}

	freeswitch_xml(const freeswitch_xml&) = delete;
	freeswitch_xml&	operator=(const freeswitch_xml&) = delete;
	freeswitch_xml(freeswitch_xml&&) = default;
	freeswitch_xml&	operator=(freeswitch_xml&&) = default;
};

} // namespace fspp

#endif //__freeswitch_xml_hpp__