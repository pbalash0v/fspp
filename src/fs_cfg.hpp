#ifndef __fs_cfg_hpp__
#define __fs_cfg_hpp__

#include <iostream>
#include <filesystem>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/lexical_cast.hpp>

namespace fs = std::filesystem;
namespace bpt = boost::property_tree;


namespace
{

constexpr auto* freeswitch_xml {"freeswitch.xml"};
constexpr auto* fspp_dir {"fspp"};

} // anon namespace


namespace fspp
{
inline void generate_fs_config(const std::filesystem::path& cfg_file_path)
{
	bpt::ptree pt;
	auto& document = pt.put("document", "");
	pt.put("document.<xmlattr>.type", "freeswitch/xml");

	//--- configuration
	bpt::ptree configuration_pt;
	configuration_pt.add("<xmlattr>.name", "configuration");
	auto& conf_section = document.add_child("section", configuration_pt);

	//--- configuration -> modules
	bpt::ptree modules_pt;
	modules_pt.add("<xmlattr>.name", "modules.conf");
	auto& modules_conf = conf_section.add_child("configuration", modules_pt);

	auto& modules = modules_conf.put("modules", "");
	bpt::ptree mod_console_pt;
	mod_console_pt.add("<xmlattr>.module", "mod_console");
	modules.add_child("load", mod_console_pt);
	bpt::ptree mod_logfile_pt;
	mod_logfile_pt.add("<xmlattr>.module", "mod_logfile");
	modules.add_child("load", mod_logfile_pt);



	//--- dialplan
	bpt::ptree dialplan_pt;
	dialplan_pt.add("<xmlattr>.name", "dialplan");
	document.add_child("section", dialplan_pt);


	bpt::xml_parser::write_xml(cfg_file_path.string(), pt, std::locale(), bpt::xml_writer_make_settings<std::string>(' ', 2));
}

inline void read_fs_config(const std::filesystem::path& cfg_file_path)
{
	bpt::ptree pt;
	bpt::xml_parser::read_xml(cfg_file_path.string(), pt);
	auto val = pt.get<std::string>("document.<xmlattr>.type");
	std::cerr << val << '\n';	
	// if (auto it = pt.find("document"); it.to_iterator() != pt.end())
	// {
	// 	auto val = boost::lexical_cast<std::string>(it->second.data());
	// 	std::cerr << val << '\n';
	// }
}


class fs_cfg
{
public:	
	fs_cfg()
	{
		fs::create_directory(fspp_conf_path);
		//auto temp_dir = fs::temp_directory_path();
		//auto freeswitch_xml_path = temp_dir / freeswitch_xml;
		generate_fs_config(freeswitch_xml_path);
	}
	fs_cfg(const fs_cfg&) = delete;
	fs_cfg&	operator=(const fs_cfg&) = delete;
	fs_cfg(fs_cfg&&) = default;
	fs_cfg&	operator=(fs_cfg&&) = default;

private:
	fs::path fspp_conf_path {fs::temp_directory_path() / fspp_dir};
	fs::path freeswitch_xml_path {fspp_conf_path / freeswitch_xml};

public:
	std::string base_dir {fspp_conf_path.string()};
	std::string mod_dir {fspp_conf_path.string()};
	std::string log_dir {fspp_conf_path.string()};
	std::string conf_dir {fspp_conf_path.string()};
	std::string run_dir {fspp_conf_path.string()};
	std::string lib_dir {fspp_conf_path.string()};
	std::string temp_dir {fspp_conf_path.string()};
	std::string db_dir {fspp_conf_path.string()};	
};

} // namespace fspp

#endif //__fs_cfg_hpp__