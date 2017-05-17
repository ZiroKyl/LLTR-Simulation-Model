// LLTRSuperApp.cc : Defines LLTR Super Application.
//

#include "inet/common/INETDefs.h"
#include "inet/common/ModuleAccess.h"
#include "inet/transportlayer/contract/udp/UDPSocket.h"

namespace inet {

class INET_API LLTRSuperApp: public cSimpleModule
{
	int port = -1;

	UDPSocket::SendOptions udpSendOpt;
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
			socket.setTimeToLive(1);

			{
				//IInterfaceTable *inet_ift = L3AddressResolver().findInterfaceTableOf(getParentModule());
				//IInterfaceTable *inet_ift = check_and_cast<IInterfaceTable*>(getParentModule()->getModuleByPath(".interfaceTable"));
				IInterfaceTable *inet_ift = getModuleFromPar<IInterfaceTable>(par("interfaceTableModule"), this);

				udpSendOpt.outInterfaceId = inet_ift->getBiggestInterfaceId();
				udpSendOpt.srcAddr = inet_ift->getInterfaceById(udpSendOpt.outInterfaceId)->getNetworkAddress();
			}

			break;
		case INITSTAGE_LAST:
			socket.sendTo(new cPacket("=Broadcast Packet="), IPv4Address::ALLONES_ADDRESS, port, &udpSendOpt);

			break;
		}
	}

	void handleMessage(cMessage *msg)
	{
		//TODO: write here

		delete msg;
	}

	void finish()
	{
		//socket.close(); //"CLOSE" msg can't be delivered to UDP module:
		                  //the event loop already terminated

		cSimpleModule::finish();
	}

	~LLTRSuperApp()
	{
		//TODO: write here
	}
};

Define_Module(LLTRSuperApp);
}
