#ifndef __utils_hpp__
#define __utils_hpp__

#include <boost/dll.hpp>
#include <boost/process.hpp>

namespace fspp
{
	auto get_working_directory()
	{
		std::cerr << boost::dll::program_location().parent_path() << "\n";
		std::cerr << boost::dll::program_location().filename() << "\n";
		std::cerr << boost::dll::program_location().root_name() << "\n";
	}

} //namespace fspp


#endif //__utils_hpp__