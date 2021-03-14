#ifndef __fspp_impl_hpp__
#define __fspp_impl_hpp__

#include <filesystem>
#include <boost/dll.hpp>
#include <boost/process.hpp>

#include "fs_cfg.hpp"
#include "fs_modules.hpp"


namespace fspp
{

class lib_impl final
{
public:
	lib_impl();
	~lib_impl();

	lib_impl(const lib_impl&) = delete;
	lib_impl& operator=(const lib_impl&) = delete;
	lib_impl(lib_impl&&) = default;
	lib_impl& operator=(lib_impl&&) = default;

	void operator()();

private:
	fs_modules _;
	std::string m_run_dir {boost::dll::program_location().parent_path().string()};
	std::string m_cur_dir {std::filesystem::current_path().string()};
	fs_cfg m_fs_cfg;

private:
	void init_SWITCH_GLOBAL_dirs();
	void print_SWITCH_GLOBAL_dirs();
};

} // namespace fspp


#endif //__fspp_impl_hpp__