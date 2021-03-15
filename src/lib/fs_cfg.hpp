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

inline std::string to_string(const bpt::ptree& cfg)
{
	std::ostringstream oss;
	bpt::xml_parser::write_xml(oss, cfg);
	return oss.str();
}


struct fs_cfg final
{
	fs_cfg() = default;

	fs_cfg(const fs_cfg&) = delete;
	fs_cfg&	operator=(const fs_cfg&) = delete;
	fs_cfg(fs_cfg&&) = default;
	fs_cfg&	operator=(fs_cfg&&) = default;

	struct conf_tuple
	{
		std::string section;
		std::string key;
		std::string value;
	};
	std::optional<std::string> operator()(const conf_tuple&) const;

	static bpt::ptree modules_conf();
	static bpt::ptree switch_conf();
	static bpt::ptree event_socket_conf();
	static bpt::ptree console_conf();
	static bpt::ptree logfile_conf();

	//
	fspp::freeswitch_xml freeswitch_xml_;
	std::unordered_map<std::string, std::function<bpt::ptree()>> config_funcs_ {
		{"modules.conf", fs_cfg::modules_conf},
		{"switch.conf", fs_cfg::switch_conf},
		{"event_socket.conf", fs_cfg::event_socket_conf},
		{"console.conf", fs_cfg::console_conf},
		{"logfile.conf", fs_cfg::logfile_conf},		
	};
};


} // namespace fspp

#endif //__fs_cfg_hpp__