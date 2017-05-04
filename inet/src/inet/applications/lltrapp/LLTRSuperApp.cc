// LLTRSuperApp.cc : Defines LLTR Super Application.
//

#include "inet/common/INETDefs.h"
#include "inet/transportlayer/contract/udp/UDPSocket.h"

namespace inet {

class INET_API LLTRSuperApp: public cSimpleModule
{
	int numInitStages() const
	{
		return NUM_INIT_STAGES;
	}

	void initialize(int stage)
	{
		cSimpleModule::initialize(stage);

		//TODO: write here
	}

	void handleMessage(cMessage *msg)
	{
		//TODO: write here

		delete msg;
	}

	void finish()
	{
		//TODO: write here

		cSimpleModule::finish();
	}

	~LLTRSuperApp()
	{
		//TODO: write here
	}
};

Define_Module(LLTRSuperApp);
}
