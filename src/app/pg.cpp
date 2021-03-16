#include <iostream>
#include <fs_cfg.hpp>


namespace
{

std::string to_string(const bpt::ptree& cfg, bool human_readable = false)
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

}

int main()
{
	std::cerr << to_string(fspp::fs_cfg::sofia_conf(), true) << '\n';
}