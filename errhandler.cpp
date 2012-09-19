

#include "errhandler.h"

#include <iostream>

void extRtCLCHandler(
	int err,
	const char *msg)
{
    
	fff::String errname = fff::_Intern::RtCLC::rtclcDebugErrorByName(err);
	std::cout << "Error(" << err << "): " << errname << ", message: " << msg << std::endl;
	cin.get();
	::exit(err);
}