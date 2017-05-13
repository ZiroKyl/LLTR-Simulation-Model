// LLTRSuperApp.cc : Defines LLTR Application.
//

#include "inet/common/INETDefs.h"
#include "inet/transportlayer/contract/udp/UDPSocket.h"

namespace inet {

class INET_API LLTRApp: public cSimpleModule
{
	int port = -1;

	UDPSocket socket;

	/*=================================================================================*/

	int numInitStages() const
	{
		return NUM_INIT_STAGES;
	}

	void initialize(int stage)
	{
		cSimpleModule::initialize(stage);

		switch(stage){
		case INITSTAGE_LOCAL:
			port = par("port");

			break;
		case INITSTAGE_APPLICATION_LAYER:
			socket.setOutputGate(gate("udpOut"));
			socket.bind(port);

			break;
		}
	}

	void handleMessage(cMessage *msg)
	{
		switch(msg->getKind()){
		case UDP_I_DATA:{
			EV << "Arrived: " << msg->getName() << endl;
			delete msg;

		}break;
		case UDP_I_ERROR:{
			EV_WARN << "Ignoring UDP error report" << endl;
			delete msg;

		}break;
		default: throw cRuntimeError("Unrecognized message (%s)%s", msg->getClassName(), msg->getName());
		}
	}

	void finish()
	{
		//socket.close(); //"CLOSE" msg can't be delivered to UDP module:
				          //the event loop already terminated

		cSimpleModule::finish();
	}

	~LLTRApp()
	{
		//TODO: write here
	}
};

Define_Module(LLTRApp);
}
