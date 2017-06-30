// LLTRUtils.cc : Defines LLTR Utilities.
//

#include "inet/applications/lltrapp/LLTRUtils.h"


namespace LLTR {
using namespace inet;

const uint32 getHostsCount(cComponentType const *const hostType)
{
	uint32 counter = 0;

	for(cModule::SubmoduleIterator i(getSimulation()->getSystemModule()); !i.end(); i++){
		if((*i)->getComponentType() == hostType) counter++;
	}

	return counter;
}

}
