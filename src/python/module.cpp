#include <pybind11/pybind11.h>
#include <pybind11/functional.h>

#include <fspp.hpp>


namespace py = pybind11;

PYBIND11_MODULE(pyfspp, m)
{
    py::class_<fspp::lib, std::shared_ptr<fspp::lib>>(m, "FSPP")
    	.def(py::init([]()
			{
				fspp::config cfg;
				cfg.python = true;
				return std::make_shared<fspp::lib>(cfg);
			}
		))
		.def("__call__", &fspp::lib::operator());
}
