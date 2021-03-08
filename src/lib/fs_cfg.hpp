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

	//--- configuration -> modules.conf
	auto modules_ = [](auto& parent_pt)
	{
		bpt::ptree pt_;
		pt_.add("<xmlattr>.name", "modules.conf");
		auto& conf_ = parent_pt.add_child("configuration", pt_);

		auto& modules = conf_.put("modules", "");

		bpt::ptree mod_console_pt;
		mod_console_pt.add("<xmlattr>.module", "mod_console");
		modules.add_child("load", mod_console_pt);

		bpt::ptree mod_logfile_pt;
		mod_logfile_pt.add("<xmlattr>.module", "mod_logfile");
		modules.add_child("load", mod_logfile_pt);

		bpt::ptree mod_event_socket_pt;
		mod_event_socket_pt.add("<xmlattr>.module", "mod_event_socket");
		modules.add_child("load", mod_event_socket_pt);

		bpt::ptree mod_commands_pt;
		mod_commands_pt.add("<xmlattr>.module", "mod_commands");
		modules.add_child("load", mod_commands_pt);

		return pt_;
	}(conf_section);

	//--- configuration -> console.conf
	auto console_ = [&name_value_pt](auto& parent_pt)
	{
		bpt::ptree pt_;
		pt_.add("<xmlattr>.name", "console.conf");
		auto& conf_ = parent_pt.add_child("configuration", pt_);

		auto& mappings = conf_.put("mappings", "");
		mappings.add_child("map", name_value_pt("all", "console,debug,info,notice,warning,err,crit,alert"));

		auto& settings = conf_.put("settings", "");
		settings.add_child("param", name_value_pt("colorize", "true"));
		settings.add_child("param", name_value_pt("loglevel", "debug"));

		return pt_;
	}(conf_section);

	//--- configuration -> logfile.conf
	// TODO
	auto logfile_ = [&name_value_pt](auto& parent_pt)
	{
		bpt::ptree pt_;
		pt_.add("<xmlattr>.name", "logfile.conf");
		auto& conf_ = parent_pt.add_child("configuration", pt_);

		auto& settings = conf_.put("settings", "");
		settings.add_child("param", name_value_pt("rotate-on-hup", "true"));

		return pt_;
	}(conf_section);

	//--- configuration -> switch.conf
	auto switch_ = [&name_value_pt](auto& parent_pt)
	{
		bpt::ptree pt_;
		pt_.add("<xmlattr>.name", "switch.conf");
		auto& conf_ = parent_pt.add_child("configuration", pt_);

		auto& settings = conf_.put("settings", "");
		settings.add_child("param", name_value_pt("colorize-console", "true"));
		settings.add_child("param", name_value_pt("dialplan-timestamps", "false"));
		settings.add_child("param", name_value_pt("max-db-handles", "50"));
		settings.add_child("param", name_value_pt("db-handle-timeout", "10"));
		settings.add_child("param", name_value_pt("max-sessions", "1000"));
		settings.add_child("param", name_value_pt("sessions-per-second", "30"));
		settings.add_child("param", name_value_pt("loglevel", "debug"));
		settings.add_child("param", name_value_pt("dump-cores", "yes"));

		return pt_;
	}(conf_section);

	//--- configuration -> event_socket.conf
	auto event_socket_ = [&name_value_pt](auto& parent_pt)
	{
		bpt::ptree pt_;
		pt_.add("<xmlattr>.name", "event_socket.conf");
		auto& conf_ = parent_pt.add_child("configuration", pt_);

		auto& settings = conf_.put("settings", "");
		settings.add_child("param", name_value_pt("nat-map", "false"));
		settings.add_child("param", name_value_pt("listen-ip", "::"));
		settings.add_child("param", name_value_pt("listen-port", "8021"));
		settings.add_child("param", name_value_pt("password", "ClueCon"));

		return pt_;
	}(conf_section);


	//
	//--- dialplan
	//
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