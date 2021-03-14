#include "fspp.hpp"
#include "fspp_impl.hpp"


namespace fspp
{

lib::lib() : p_impl_{std::make_unique<lib_impl>()}
{
}

lib::~lib()
{
}

void lib::operator()()
{
	p_impl_->operator()();
}

} // namespace fspp
