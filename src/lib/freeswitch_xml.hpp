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


namespace fs = std::filesystem;
namespace bpt = boost::property_tree;

namespace
{
constexpr auto* freeswitch_xml_filename {"freeswitch.xml"};
}

namespace fspp
{

class freeswitch_dot_xml
{
public:	
	freeswitch_dot_xml(const fspp::config& cfg) : freeswitch_xml_{fs::path{cfg.base_path} / freeswitch_xml_filename}
	{
		auto freeswitch_xml_contents = []()
		{
			bpt::ptree document;
			document.put("document", "");
			document.put("<xmlattr>.type", "freeswitch/xml");
			return document;
		}();

		bpt::xml_parser::write_xml(freeswitch_xml_.string(), freeswitch_xml_contents, std::locale(), bpt::xml_writer_make_settings<std::string>(' ', 2));
	}

	freeswitch_dot_xml(const freeswitch_dot_xml&) = delete;
	freeswitch_dot_xml&	operator=(const freeswitch_dot_xml&) = delete;
	freeswitch_dot_xml(freeswitch_dot_xml&&) = default;
	freeswitch_dot_xml&	operator=(freeswitch_dot_xml&&) = default;

	~freeswitch_dot_xml()
	{
		std::error_code ec;

		fs::remove_all(freeswitch_xml_, ec);
		if (ec)
		{
			std::cerr << ec << '\n';
		}
	}

	fs::path freeswitch_xml_;
};

} // namespace fspp

#endif //__freeswitch_xml_hpp__