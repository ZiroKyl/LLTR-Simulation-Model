// LLTRSuperApp.cc : Defines LLTR Super Application.
//

#include "inet/common/INETDefs.h"
#include "inet/common/ModuleAccess.h"
#include "inet/transportlayer/contract/udp/UDPSocket.h"
#include "inet/transportlayer/contract/tcp/TCPSocket.h"

namespace inet {

class INET_API LLTRSuperApp: public cSimpleModule, public TCPSocket::CallbackInterface
{
	int port = -1;

	UDPSocket::SendOptions udpSendOpt;
	UDPSocket socket;

	int gateTcpId;
	TCPSocket socketTcp;

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

			gateTcpId = gateBaseId("tcpIn");

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

			socketTcp.setOutputGate(gate("tcpOut"));
			socketTcp.setDataTransferMode(TCP_TRANSFER_BYTECOUNT);
			socketTcp.setCallbackObject(this);

			break;
		case INITSTAGE_LAST:
			socket.sendTo(new cPacket("=Broadcast Packet="), IPv4Address::ALLONES_ADDRESS, port, &udpSendOpt);
			socketTcp.connect(IPv4Address(10,0,1,0), port+1);

			break;
		}
	}

	void handleMessage(cMessage *msg)
	{
		if(msg->arrivedOn(gateTcpId) && socketTcp.belongsToSocket(msg)) socketTcp.processMessage(msg);
		else delete msg;
	}

	void socketDataArrived(int, void*, cPacket *msg, bool)
	{
		socketTcp.close();

		EV << "Arrived (TCP): " << msg->getName() << endl;

		delete msg;
	}

	void socketPeerClosed(int, void*) {
		socketTcp.close();
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
