#ifndef __freeswitch_xml_hpp__
#define __freeswitch_xml_hpp__

#include <filesystem>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

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

	operator std::string() const
	{
	    return fspp_conf_path.string();
	}

	freeswitch_xml(const freeswitch_xml&) = delete;
	freeswitch_xml&	operator=(const freeswitch_xml&) = delete;
	freeswitch_xml(freeswitch_xml&&) = default;
	freeswitch_xml&	operator=(freeswitch_xml&&) = default;

private:
	inline static constexpr auto* freeswitch_xml_ {"freeswitch.xml"};
	inline static constexpr auto* fspp_dir_ {"fspp"};

	fs::path fspp_conf_path {fs::temp_directory_path() / fspp_dir_};
	fs::path freeswitch_xml_path {fspp_conf_path / freeswitch_xml_};
};

} // namespace fspp

#endif //__freeswitch_xml_hpp__