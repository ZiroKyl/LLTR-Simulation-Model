// LLTRSuperApp.cc : Defines LLTR Application.
//

#include "inet/common/INETDefs.h"
#include "inet/transportlayer/contract/udp/UDPSocket.h"
#include "inet/transportlayer/contract/tcp/TCPSocket.h"

namespace inet {

class INET_API LLTRApp: public cSimpleModule, public TCPSocket::CallbackInterface
{
	int port = -1;

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
			socket.setBroadcast(true);
			socket.bind(port);

			socketTcp.setOutputGate(gate("tcpOut"));
			socketTcp.setDataTransferMode(TCP_TRANSFER_BYTECOUNT);
			socketTcp.setCallbackObject(this);
			socketTcp.bind(port+1);
			socketTcp.listenOnce();

			break;
		}
	}

	void handleMessage(cMessage *msg)
	{
		if(msg->arrivedOn(gateTcpId)){
			socketTcp.processMessage(msg);
			return;
		}

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

	void socketEstablished(int, void*)
	{
		cPacket *packet = new cPacket("=TCP Packet=");
		packet->setByteLength(1);

		socketTcp.send(packet);
	}

	void socketDataArrived(int, void*, cPacket*, bool)
	{

	}

	void socketPeerClosed(int connId, void*) {
		socketTcp.close();
		socketTcp.renewSocket();
		socketTcp.bind(port+1);
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
