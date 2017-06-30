// LLTRUtils.h: Declares/Defines LLTR Utilities.
//
#ifndef __LLTR_UTILITIES_H
#define __LLTR_UTILITIES_H

#include <array>

#include "inet/common/INETDefs.h"
#include "inet/common/ModuleAccess.h"

#include "inet/transportlayer/contract/tcp/TCPSocket.h"


namespace LLTR {
using namespace inet;

const uint32 getHostsCount(cComponentType const *const hostType);

//Hosts path. Default path - "hostS"
//Define for fast copy/paste in the future.
class HostPath_defSuperHost_
{public:
	//char path[15] = "hostS\0\0\0\0\0\0\0\0\0"; //not support                                                 //default "hostS"
	//char path[15] = {'h','o','s','t','S','\0','\0','\0','\0','\0','\0','\0','\0','\0', '\0'}; //bad code gen //default "hostS"
	std::array<char,15> path{"hostS\0\0\0\0\0\0\0\0\0"};                                                       //default "hostS"

	//default path - "hostS"
	HostPath_defSuperHost_() = default;

	HostPath_defSuperHost_(const uint32 hostId)
	{
		SetHost(hostId);
	}

	HostPath_defSuperHost_& SetSuperHost()
	{
		path[4]='S';
		path[5]='\0';

		return *this;
	}

	HostPath_defSuperHost_& SetHost(const uint32 hostId)
	{
		itoa10(hostId, &path[4]);

		return *this;
	}

	const L3Address GetIp() const
	{
		IInterfaceTable *inet_ift = dynamic_cast<IInterfaceTable*>(getSimulation()->getModuleByPath(path.data())->getSubmodule("interfaceTable"));

		return inet_ift->getInterfaceById( inet_ift->getBiggestInterfaceId() )->getNetworkAddress();
	}

	const L3Address operator() (const uint32 hostId)
	{
		SetHost(hostId);

		return GetIp();
	}

	// http://www.jb.man.ac.uk/~slowe/cpp/itoa.html
	char* itoa10(uint32 value, char* result) const
	{
		char* ptr = result, *ptr1 = result, tmp_char;
		uint32 tmp_value;

		do{
			tmp_value = value;
			value /= 10;
			*ptr++ = '0' + (tmp_value - value*10);
		}while(value);

		*ptr-- = '\0';
		while(ptr1 < ptr) {
			tmp_char = *ptr;
			*ptr--   = *ptr1;
			*ptr1++  = tmp_char;
		}
		return result;
	}
};

}
#endif // ifndef __LLTR_UTILITIES_H
