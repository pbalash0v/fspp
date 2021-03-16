#ifndef __fs_modules_hpp__
#define __fs_modules_hpp__

#include <boost/throw_exception.hpp>
#include <switch.h>

namespace
{

extern "C"
{
extern switch_loadable_module_function_table_t CORE_SOFTTIMER_MODULE_module_interface;
extern switch_loadable_module_function_table_t CORE_PCM_MODULE_module_interface;
extern switch_loadable_module_function_table_t CORE_SPEEX_MODULE_module_interface;
extern switch_loadable_module_function_table_t mod_sofia_module_interface;
extern switch_loadable_module_function_table_t mod_event_socket_module_interface;
extern switch_loadable_module_function_table_t mod_commands_module_interface;
extern switch_loadable_module_function_table_t mod_logfile_module_interface;
extern switch_loadable_module_function_table_t mod_console_module_interface;
extern switch_loadable_module_function_table_t mod_dialplan_xml_module_interface;
}

} //anon namespace

namespace fspp
{

class fs_modules final
{
public:
	fs_modules()
	{
		if (not CORE_PCM_MODULE_module_interface_
			or not CORE_SPEEX_MODULE_module_interface_
			or not CORE_SOFTTIMER_MODULE_module_interface_
			or not mod_sofia_module_interface_
			or not mod_event_socket_module_interface_
			or not mod_commands_module_interface_
			or not mod_logfile_module_interface_
			or not mod_console_module_interface_
			or not mod_dialplan_xml_module_interface_
			)
		{
			BOOST_THROW_EXCEPTION(std::runtime_error{"Builtin FreeSWITCH modules was not proprely linked"});
		}
	}

private:
	const void* CORE_SOFTTIMER_MODULE_module_interface_ = static_cast<void*>(&CORE_SOFTTIMER_MODULE_module_interface);
	const void* CORE_PCM_MODULE_module_interface_ = static_cast<void*>(&CORE_PCM_MODULE_module_interface);
	const void* CORE_SPEEX_MODULE_module_interface_ = static_cast<void*>(&CORE_SPEEX_MODULE_module_interface);
	const void* mod_sofia_module_interface_ = static_cast<void*>(&mod_sofia_module_interface);
	const void* mod_event_socket_module_interface_ = static_cast<void*>(&mod_event_socket_module_interface);
	const void* mod_commands_module_interface_ = static_cast<void*>(&mod_commands_module_interface);
	const void* mod_logfile_module_interface_ = static_cast<void*>(&mod_logfile_module_interface);
	const void* mod_console_module_interface_ = static_cast<void*>(&mod_console_module_interface);
	const void* mod_dialplan_xml_module_interface_ = static_cast<void*>(&mod_dialplan_xml_module_interface);
};

} // namespace fspp

#endif