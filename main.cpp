#include <iostream>

#ifdef OS_LINUX
#include "linux_platform.h"
#endif

using namespace std;

int main()
{
	return start_platform_engine();
}