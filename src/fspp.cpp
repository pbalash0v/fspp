#include <switch.h>
#include "fspp.hpp"

namespace fspp
{

void lib_main()
{
    switch_core_flag_t flags {SCF_USE_SQL};
    bool console {true};
    const char* err {nullptr};

    switch_core_set_globals();  
    switch_core_init_and_modload(flags, console ? SWITCH_TRUE : SWITCH_FALSE, &err);
    switch_core_runtime_loop(!console);

}


} // namespace fspp
