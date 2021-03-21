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

#include <iostream>
#include <filesystem>
#include <unordered_map>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <switch.h>

#include "fspp.hpp"
#include "fs_cfg.hpp"


namespace fs = std::filesystem;
namespace bpt = boost::property_tree;


namespace
{

auto name_value_pt = [](auto name, auto val) -> auto
{
	bpt::ptree param_pt;
	param_pt.add("<xmlattr>.name", name);
	param_pt.add("<xmlattr>.value", val);
	return param_pt;
};

auto fs_xml_header = [](auto& pt) -> auto&
{
	auto& header = pt.put("document", "");
	header.put("<xmlattr>.type", "freeswitch/xml");

	return header;
};

auto conf_section_xml = [](auto& pt) -> auto&
{
	auto& document = fs_xml_header(pt);

	bpt::ptree configuration_pt;
	configuration_pt.add("<xmlattr>.name", "configuration");
	auto& conf_section = document.add_child("section", configuration_pt);

	return conf_section;
};

} // anon namespace


namespace fspp
{

fs_cfg::fs_cfg(fspp::config cfg) : cfg_{cfg}
{
}

inline std::string to_string(const bpt::ptree& cfg, bool human_readable = false)
{
	std::ostringstream oss;

	if (human_readable)
	{
		bpt::xml_parser::write_xml(oss, cfg, bpt::xml_writer_make_settings<std::string>(' ', 2));
	}
	else
	{
		bpt::xml_parser::write_xml(oss, cfg);
	}

	return oss.str();
}

bpt::ptree fs_cfg::modules_conf(const fs_cfg&)
{
	bpt::ptree pt;

	auto& conf_section = conf_section_xml(pt);

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

		bpt::ptree mod_dialplan_xml_pt;
		mod_dialplan_xml_pt.add("<xmlattr>.module", "mod_dialplan_xml");
		modules.add_child("load", mod_dialplan_xml_pt);

		bpt::ptree mod_commands_pt;
		mod_commands_pt.add("<xmlattr>.module", "mod_commands");
		modules.add_child("load", mod_commands_pt);

		bpt::ptree mod_sofia_pt;
		mod_sofia_pt.add("<xmlattr>.module", "mod_sofia");
		modules.add_child("load", mod_sofia_pt);

		return pt_;
	}(conf_section);

	return pt;
}

bpt::ptree fs_cfg::console_conf(const fs_cfg&)
{
	bpt::ptree pt;

	auto& conf_section = conf_section_xml(pt);

	//--- configuration -> console.conf
	auto console_ = [&](auto& parent_pt)
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

	return pt;
}

bpt::ptree fs_cfg::logfile_conf(const fs_cfg&)
{
	bpt::ptree pt;

	auto& conf_section = conf_section_xml(pt);

	//--- configuration -> logfile.conf
	// TODO
	auto logfile_ = [&](auto& parent_pt)
	{
		bpt::ptree pt_;
		pt_.add("<xmlattr>.name", "logfile.conf");
		auto& conf_ = parent_pt.add_child("configuration", pt_);

		auto& settings = conf_.put("settings", "");
		settings.add_child("param", name_value_pt("rotate-on-hup", "true"));

		return pt_;
	}(conf_section);

	return pt;
}

bpt::ptree fs_cfg::switch_conf(const fs_cfg&)
{
	bpt::ptree pt;

	auto& conf_section = conf_section_xml(pt);

	//--- configuration -> switch.conf
	auto switch_ = [&](auto& parent_pt)
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

	return pt;
}

bpt::ptree fs_cfg::event_socket_conf(const fs_cfg&)
{
	bpt::ptree pt;

	auto& conf_section = conf_section_xml(pt);

	//--- configuration -> event_socket.conf
	auto event_socket_ = [&](auto& parent_pt)
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

	return pt;
}

bpt::ptree fs_cfg::sofia_conf(const fs_cfg&)
{
	bpt::ptree pt;

	auto& conf_section = conf_section_xml(pt);

	[&](auto& parent_pt)
	{
		bpt::ptree pt_;
		pt_.add("<xmlattr>.name", "sofia.conf");
		auto& conf_ = parent_pt.add_child("configuration", pt_);

		auto& profiles = conf_.put("profiles", "");
		auto& profile = profiles.put("profile", "");
		profile.add("<xmlattr>.name", "internal");
		auto& settings = profile.put("settings", "");

		settings.add_child("param", name_value_pt("sip-ip", switch_core_get_variable("local_ip_v4")));
		settings.add_child("param", name_value_pt("rtp-ip", switch_core_get_variable("local_ip_v4")));
		settings.add_child("param", name_value_pt("sip-port", "5060"));
		settings.add_child("param", name_value_pt("ext-rtp-ip", "stun:stun.freeswitch.org"));
		settings.add_child("param", name_value_pt("ext-sip-ip", "stun:stun.freeswitch.org"));		

		settings.add_child("param", name_value_pt("inbound-codec-prefs", "PCMA,PCMU"));
		settings.add_child("param", name_value_pt("outbound-codec-prefs", "PCMA,PCMU"));

		settings.add_child("param", name_value_pt("context", "public"));

		settings.add_child("param", name_value_pt("apply-nat-acl", "nat.auto"));
		settings.add_child("param", name_value_pt("local-network-acl", "localnet.auto"));

		settings.add_child("param", name_value_pt("disable-register", "true"));
		settings.add_child("param", name_value_pt("disable-transfer", "true"));

		settings.add_child("param", name_value_pt("auth-calls", "false"));		

		return pt_;
	}(conf_section);

	return pt;
}


#if 0
bpt::ptree fs_cfg::none_existing_conf()
{
	bpt::ptree pt;
	auto& document = fs_xml_header(pt);

	bpt::ptree result_pt;
	result_pt.add("<xmlattr>.name", "result");
	document.add_child("section", result_pt);

	//--- configuration -> event_socket.conf
	auto not_found_ = [&](auto& parent_pt)
	{
		bpt::ptree pt_;
		pt_.add("<xmlattr>.status", "not found");		
		parent_pt.add_child("result", pt_);
		return pt_;
	}(document);

	return pt;
}
#endif

#if 0
	//
	//--- dialplan
	//
	bpt::ptree dialplan_pt;
	dialplan_pt.add("<xmlattr>.name", "dialplan");
	document.add_child("section", dialplan_pt);


	return document;
}
#endif


std::optional<std::string> fs_cfg::operator()(const conf_tuple& tpl) const 
{
	// std::cerr
	// 	<< std::string{tpl.section} << ":"
	// 	<< std::string{tpl.tag} << ":"
	// 	<< std::string{tpl.key} << ":"
	// 	<< std::string{tpl.value} << ":"
	// 	<< '\n';

	if (tpl.section == std::string{"configuration"} and tpl.key == std::string{"name"})
	{
		if (auto it = config_funcs_.find(std::string{tpl.value}); it != config_funcs_.end())
		{
			return to_string(it->second(*this));
		}
	}
	else if (tpl.section == std::string{"dialplan"})
	{
	}

	//TODO
	//return to_string(none_existing_conf()); 
	return std::nullopt;
}

} // namespace fspp
