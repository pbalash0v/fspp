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

#ifndef __fspp_hpp__
#define __fspp_hpp__

#include <memory>


namespace fspp
{
class lib_impl;

struct config
{
	bool python {false};
	bool console {false};
	std::string id{};
	std::string base_path{};
};

class lib final
{
public:
	lib(config = {});
	~lib();

	lib(const lib&) = delete;
	lib& operator=(const lib&) = delete;
	lib(lib&&) = default;
	lib& operator=(lib&&) = default;

	void operator()();

private:
	std::unique_ptr<lib_impl> p_impl_;
};

} // namespace fspp


#endif //__fspp_hpp__