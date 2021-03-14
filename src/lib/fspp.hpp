#ifndef __fspp_hpp__
#define __fspp_hpp__

#include <memory>


namespace fspp
{
class lib_impl;

class lib final
{
public:
	lib();
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