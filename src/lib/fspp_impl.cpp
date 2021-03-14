#include <iostream>
#include <boost/exception/diagnostic_information.hpp>

#include <switch.h>

#include "utils.hpp"
#include "fspp_impl.hpp"
#include "fs_cfg.hpp"


namespace fspp
{

lib_impl::lib_impl()
{
	// loads internal SWITCH_GLOBAL_dirs struct of char* with autoconf generated values (?)
    switch_core_set_globals();

	// std::cerr << "Initial SWITCH_GLOBAL_dirs\n";
	// print_SWITCH_GLOBAL_dirs();
	init_SWITCH_GLOBAL_dirs();
	// std::cerr << "fspp initialized SWITCH_GLOBAL_dirs\n";
	// print_SWITCH_GLOBAL_dirs();

    //
    switch_core_flag_t flags {SCF_USE_SQL};
	bool console {true};
    const char* err {nullptr};
	   
   	if (auto res = switch_core_init_and_modload(flags, console ? SWITCH_TRUE : SWITCH_FALSE, &err); res != SWITCH_STATUS_SUCCESS)
   	{
   		BOOST_THROW_EXCEPTION(std::runtime_error{err});
   	}
}

lib_impl::~lib_impl()
{
	[[maybe_unused]] auto destroy_status = switch_core_destroy();
	BOOST_ASSERT(destroy_status == SWITCH_STATUS_SUCCESS);
}

void lib_impl::operator()()
{
    print_SWITCH_GLOBAL_dirs();

    // blocks here in console or stdout
	switch_core_runtime_loop(false);
    //print_SWITCH_GLOBAL_dirs();
}


void lib_impl::init_SWITCH_GLOBAL_dirs()
{
	SWITCH_GLOBAL_dirs.base_dir	= m_fs_cfg.base_dir.data();
	SWITCH_GLOBAL_dirs.mod_dir = m_fs_cfg.mod_dir.data();
	//SWITCH_GLOBAL_dirs.mod_dir = nullptr;

	SWITCH_GLOBAL_dirs.conf_dir = m_fs_cfg.conf_dir.data();
	SWITCH_GLOBAL_dirs.log_dir = m_fs_cfg.log_dir.data();
	SWITCH_GLOBAL_dirs.run_dir = m_fs_cfg.run_dir.data();
	SWITCH_GLOBAL_dirs.lib_dir = m_fs_cfg.lib_dir.data();
	SWITCH_GLOBAL_dirs.temp_dir = m_fs_cfg.temp_dir.data();
	SWITCH_GLOBAL_dirs.db_dir = m_fs_cfg.temp_dir.data();
}

void lib_impl::print_SWITCH_GLOBAL_dirs()
{
	// struct switch_directories {
	// 	char *base_dir;
	// 	char *mod_dir;
	// 	char *conf_dir;
	// 	char *log_dir;
	// 	char *run_dir;
	// 	char *db_dir;
	// 	char *script_dir;
	// 	char *temp_dir;
	// 	char *htdocs_dir;
	// 	char *grammar_dir;
	// 	char *storage_dir;
	// 	char *cache_dir;
	// 	char *recordings_dir;
	// 	char *sounds_dir;
	// 	char *lib_dir;
	// 	char *certs_dir;
	// 	char *fonts_dir;
	// 	char *images_dir;
	// 	char *data_dir;
	// 	char *localstate_dir;
	// };
	// typedef struct switch_directories switch_directories;
	// SWITCH_DECLARE_DATA extern switch_directories SWITCH_GLOBAL_dirs;

	std::cerr << "SWITCH_GLOBAL_dirs.base_dir: " << SWITCH_GLOBAL_dirs.base_dir << "\n";
	std::cerr << "SWITCH_GLOBAL_dirs.mod_dir: " << SWITCH_GLOBAL_dirs.mod_dir << "\n";
	std::cerr << "SWITCH_GLOBAL_dirs.conf_dir: " << SWITCH_GLOBAL_dirs.conf_dir << "\n";
	std::cerr << "SWITCH_GLOBAL_dirs.log_dir: " << SWITCH_GLOBAL_dirs.log_dir << "\n";
	std::cerr << "SWITCH_GLOBAL_dirs.run_dir: " << SWITCH_GLOBAL_dirs.run_dir << "\n";
	std::cerr << "SWITCH_GLOBAL_dirs.lib_dir: " << SWITCH_GLOBAL_dirs.lib_dir << "\n";	
	std::cerr << "SWITCH_GLOBAL_dirs.temp_dir: " << SWITCH_GLOBAL_dirs.temp_dir << "\n";
}

} // namespace fspp
