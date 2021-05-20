#include <pybind11/pybind11.h>
#include <pybind11/functional.h>

#include <fspp.hpp>


namespace py = pybind11;

PYBIND11_MODULE(pyfspp, m)
{
    py::class_<fspp::config, std::shared_ptr<fspp::config>>(m, "config")
    	.def(py::init([]() { return std::make_shared<fspp::config>(); }))
    	.def_readonly("id", &fspp::config::id)
		.def_readonly("base_path", &fspp::config::base_path)
		.def_readwrite("esl_port", &fspp::config::esl_port)
		.def_readwrite("sip", &fspp::config::sip);
		

    py::class_<fspp::lib, std::shared_ptr<fspp::lib>>(m, "FSPP")
    	.def(py::init([](std::shared_ptr<fspp::config> cfg)
			{
				cfg->python = true;
				return std::make_shared<fspp::lib>(*cfg);
			}
		))
		.def("__call__", &fspp::lib::operator());
}
