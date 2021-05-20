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

#ifndef __fspp_impl_hpp__
#define __fspp_impl_hpp__

#include <filesystem>
#include <boost/dll.hpp>
#include <boost/process.hpp>

#include "fspp.hpp"
#include "fspp_dirs.hpp"
#include "fs_cfg.hpp"
#include "fs_modules.hpp"


namespace fspp
{

class lib_impl final
{
public:
	lib_impl(fspp::config);
	~lib_impl();

	lib_impl(const lib_impl&) = delete;
	lib_impl& operator=(const lib_impl&) = delete;
	lib_impl(lib_impl&&) = delete;
	lib_impl& operator=(lib_impl&&) = delete;

	//! Blocks in runtime loop
	void operator()();

	const fs_cfg& cfg() const noexcept { return fs_cfg_; };

private:
	//! User-provided fspp lib config
	fspp::config cfg_;
	//! RAII create/delete this instance's specific directories
	fspp::dirs dirs_;
	//! Make sure all C symbols from stat lib are present
	fs_modules fs_modules_;
	fs_cfg fs_cfg_;

private:
	void init_SWITCH_GLOBAL_dirs();
	void print_SWITCH_GLOBAL_dirs();
};

} // namespace fspp


#endif //__fspp_impl_hpp__