#ifndef __fspp_hpp__
#define __fspp_hpp__

#include <filesystem>
#include <boost/dll.hpp>
#include <boost/process.hpp>

#include "fs_cfg.hpp"


namespace fspp
{
class lib final
{
public:

	lib();
	~lib();
	lib(const lib&) = delete;
	lib& operator=(const lib&) = delete;
	lib(lib&&) = default;
	lib& operator=(lib&&) = default;

	void operator()();

private:
	std::string m_run_dir {boost::dll::program_location().parent_path().string()};
	std::string m_cur_dir {std::filesystem::current_path().string()};
	fs_cfg m_fs_cfg;

private:
	void init_SWITCH_GLOBAL_dirs();
};

} // namespace fspp


#endif //__fspp_hpp__