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
	auto name_value_pt = [](auto name, auto val)
	{
		bpt::ptree param_pt;
		param_pt.add("<xmlattr>.name", name);
		param_pt.add("<xmlattr>.value", val);
		return param_pt;
	};

	bpt::ptree pt;
	auto& document = pt.put("document", "");
	pt.put("document.<xmlattr>.type", "freeswitch/xml");

	//--- configuration
	bpt::ptree configuration_pt;
	configuration_pt.add("<xmlattr>.name", "configuration");
	auto& conf_section = document.add_child("section", configuration_pt);

	//--- configuration -> modules
	auto modules = [](auto& parent_pt)
	{
		bpt::ptree modules_pt;
		modules_pt.add("<xmlattr>.name", "modules.conf");
		auto& modules_conf = parent_pt.add_child("configuration", modules_pt);

		auto& modules = modules_conf.put("modules", "");
		bpt::ptree mod_console_pt;
		mod_console_pt.add("<xmlattr>.module", "mod_console");
		modules.add_child("load", mod_console_pt);
		bpt::ptree mod_logfile_pt;
		mod_logfile_pt.add("<xmlattr>.module", "mod_logfile");
		modules.add_child("load", mod_logfile_pt);

		return modules_pt;
	}(conf_section);

	//--- configuration -> console
	auto console = [&name_value_pt](auto& parent_pt)
	{
		bpt::ptree console_pt;
		console_pt.add("<xmlattr>.name", "console.conf");
		auto& console_conf = parent_pt.add_child("configuration", console_pt);

		auto& mappings = console_conf.put("mappings", "");
		mappings.add_child("map", name_value_pt("all", "console,debug,info,notice,warning,err,crit,alert"));

		auto& settings = console_conf.put("settings", "");
		settings.add_child("param", name_value_pt("colorize", "true"));
		settings.add_child("param", name_value_pt("loglevel", "debug"));

		return console_pt;
	}(conf_section);

	//--- configuration -> logfile
	auto logfile = [&name_value_pt](auto& parent_pt)
	{
		bpt::ptree logfile_pt;
		logfile_pt.add("<xmlattr>.name", "logfile.conf");
		auto& logfile_conf = parent_pt.add_child("configuration", logfile_pt);

		auto& settings = logfile_conf.put("settings", "");
		settings.add_child("param", name_value_pt("rotate-on-hup", "true"));

		return logfile_pt;
	}(conf_section);


	//--- dialplan
	bpt::ptree dialplan_pt;
	dialplan_pt.add("<xmlattr>.name", "dialplan");
	document.add_child("section", dialplan_pt);

	//--- write to file
	bpt::xml_parser::write_xml(cfg_file_path.string(), pt, std::locale(), bpt::xml_writer_make_settings<std::string>(' ', 2));
}


class fs_cfg
{
public:	
	fs_cfg()
	{
		fs::create_directory(fspp_conf_path);
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