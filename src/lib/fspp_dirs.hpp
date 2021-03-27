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

#ifndef __fspp_dirs_hpp__
#define __fspp_dirs_hpp__

#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;


namespace fspp
{

struct dirs
{
	dirs(const fspp::config& cfg) : p_{cfg.base_path}
	{
		fs::create_directories(p_);
	}

	dirs(const dirs&) = delete;
	dirs& operator=(const dirs&) = delete;
	dirs(dirs&&) = default;
	dirs& operator=(dirs&&) = default;

	~dirs()
	{
		std::error_code ec;

		fs::remove_all(p_, ec);
		if (ec)
		{
			std::cerr << ec << '\n';
		}
	}

	fs::path p_;
};

} // namespace fspp

#endif //__fspp_dirs_hpp__