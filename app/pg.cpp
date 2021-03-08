#include <iostream>
#include <fs_cfg.hpp>

namespace fs = std::filesystem;

int main()
{
	auto temp_dir = fs::temp_directory_path();
	//fspp::read_fs_config(temp_dir / "f.xml");
	fspp::generate_fs_config(temp_dir / "fcfg.xml");
}