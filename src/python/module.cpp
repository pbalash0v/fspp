#include <pybind11/pybind11.h>
#include <pybind11/functional.h>

#include <fspp.hpp>


namespace py = pybind11;

PYBIND11_MODULE(pyfspp, m)
{
    py::class_<fspp::lib, std::shared_ptr<fspp::lib>>(m, "FSPP")
    	.def(py::init([]() { return std::make_shared<fspp::lib>(fspp::config{true}); }))
		.def("__call__", &fspp::lib::operator());
}
