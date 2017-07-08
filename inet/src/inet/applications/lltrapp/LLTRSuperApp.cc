// LLTRSuperApp.cc : Defines LLTR Super Application.
//

#include <vector>


#include "inet/common/INETDefs.h"
#include "inet/common/ModuleAccess.h"

#include "inet/transportlayer/contract/udp/UDPSocket.h"
#include "inet/transportlayer/contract/tcp/TCPSocket.h"


#include "inet/applications/lltrapp/FillPayload_m.h"
#include "inet/applications/lltrapp/StatPayload_m.h"

#include "inet/applications/lltrapp/LLTRUtils.h"

#define QTENV_FINISH_LOG_BUG


namespace inet {
using namespace LLTR;

class INET_API LLTRSuperApp: public cSimpleModule, public TCPSocket::CallbackInterface
{
	/* save state in "global" variables is practical to slow down && bugging your code */

	cMessage *evFill = nullptr;
	cMessage *evStat = nullptr;

	uint32    iN = 0; //iteration Number
	int     pktN = 0; //packet    Number (sub      iteration Number)
	uint32 hostN = 0; //host      Number (parallel iteration Number)

	int  port = -1;
	long pktLen = 0;

	UDPSocket::SendOptions sendUdpOpt; //socketFill
	int gateTcpId;                     //socketStat

	TCPSocket socketStat;
	UDPSocket socketFill;

	std::vector<int> countFill; //[numHosts][combHosts]
	uint32  numHosts;  //without "hostS"
	uint32 combHosts;  //(numHosts-1)*(numHosts)

	/*=================================================================================*/

	int numInitStages() const
	{
		return NUM_INIT_STAGES;
	}

	void initialize(int stage)
	{
		cSimpleModule::initialize(stage);

		switch(stage){
		case INITSTAGE_LOCAL:{
			port   = par("port");
			pktLen = par("packetLength").longValue();

			gateTcpId = gateBaseId("tcpIn");

			evFill = new cMessage("Fill");
			evStat = new cMessage("Stat");

			numHosts = getHostsCount(getParentModule()->getComponentType()) - 1;	//"host*" without "hostS" (-1)
			combHosts = (numHosts-1)*(numHosts);

			countFill.resize(numHosts*combHosts);

		}break;
		case INITSTAGE_APPLICATION_LAYER:{
			socketFill.setOutputGate(gate("udpOut"));
			socketFill.setTimeToLive(1);

			UDPSocket socketDISCARD;
			socketDISCARD.setOutputGate(gate("udpOut"));
			socketDISCARD.bind(9);

			{
				//IInterfaceTable *inet_ift = L3AddressResolver().findInterfaceTableOf(getParentModule());
				//IInterfaceTable *inet_ift = check_and_cast<IInterfaceTable*>(getParentModule()->getModuleByPath(".interfaceTable"));
				IInterfaceTable *inet_ift = getModuleFromPar<IInterfaceTable>(par("interfaceTableModule"), this);

				sendUdpOpt.outInterfaceId = inet_ift->getBiggestInterfaceId();
				sendUdpOpt.srcAddr        = inet_ift->getInterfaceById(sendUdpOpt.outInterfaceId)->getNetworkAddress();
			}

			socketStat.setOutputGate(gate("tcpOut"));
			socketStat.setDataTransferMode(TCP_TRANSFER_OBJECT);
			socketStat.setCallbackObject(this);

		}break;
		case INITSTAGE_LAST:{
			setTimeout(evFill, INIT, SIMTIME_ZERO);

		}break;
		}
	}

	void handleMessage(cMessage *msg)
	{
		if(msg->isSelfMessage()){
			if     (msg == evFill) handleFill((LLTRStep)msg->getKind());
			else if(msg == evStat) handleStat();
			else delete msg;
		}else{
			if(msg->arrivedOn(gateTcpId) && socketStat.belongsToSocket(msg)) socketStat.processMessage(msg);
			else delete msg;
		}
	}

	inline void handleFill(const LLTRStep step)
	{
		switch(step){
		case INIT:{
			iN   = 0;
			pktN = 0;

			setTimeout(evFill, PROBING, SimTime(136*10,SIMTIME_US));
		}break;
		case PROBING:{
			FillPayload *fill; {
				char msgName[64], time[64]; //see min buf size in ".str()" doc
				snprintf(msgName, sizeof msgName, ">>> %d/%d time:%s", iN, 300-1-pktN, simTime().str(time));
				msgName[sizeof(msgName)-1] = '\0'; //http://demin.ws/blog/english/2013/01/28/use-snprintf-on-different-platforms/
				fill = new FillPayload(msgName);
			}
			fill->setStep(step);
			fill->setIterationNumber(iN);
			fill->setByteLength(pktLen);

			socketFill.sendTo(fill, IPv4Address::ALLONES_ADDRESS, port, &sendUdpOpt);

			if(++pktN < 300){
				setTimeout(evFill, step, SimTime(136,SIMTIME_US));
				break;
			}

			if(++iN < combHosts){
				pktN = 0;
				setTimeout(evFill, step, SimTime(136*100,SIMTIME_US));	//before x10 (136*10)
			}else{
				pktN  = 0;
				hostN = 0;

				setTimeout(evFill, COLLECT, SimTime(136*110,SIMTIME_US));
				setTimeout(evStat, 0,       SimTime(136*120,SIMTIME_US));
			}
		}break;
		case COLLECT:{
			FillPayload *fill; {
				char msgName[64], time[64]; //see min buf size in ".str()" doc
				snprintf(msgName, sizeof msgName, ">>> %d/%d time:%s", iN, 30-1-pktN, simTime().str(time));
				msgName[sizeof(msgName)-1] = '\0'; //http://demin.ws/blog/english/2013/01/28/use-snprintf-on-different-platforms/
				fill = new FillPayload(msgName);
			}
			fill->setStep(step);
			fill->setIterationNumber(iN);
			fill->setByteLength(5);

			socketFill.sendTo(fill, IPv4Address::ALLONES_ADDRESS, port, &sendUdpOpt);

			if(++pktN < 30){
				setTimeout(evFill, step, SimTime(136*5,SIMTIME_US));
				break;
			}
		}break;
		default: throw cRuntimeError("Invalid Fill step: %d", step);
		}
	}

	inline void handleStat()
	{
		socketStat.connect(HostPath_defSuperHost_(hostN).GetIp(), port);
	}

	void socketDataArrived(int, void*, cPacket *msg, bool)
	{
		socketStat.close();

		Array6 &pcount = check_and_cast<StatPayload*>(msg)->getFillCount();
		for(int i=0;i<combHosts;i++) countFill[hostN*combHosts + i] = pcount[i];

		delete msg;
		if(++hostN < numHosts){
			socketStat.renewSocket();
			handleStat();
		}
	#ifdef QTENV_FINISH_LOG_BUG
		else{
			EV << "=== anti Qtenv \"finish()\" bug ===" << endl;
			//dublicate finish() func
			for(int j=0;j<combHosts;j++){
				EV << "{" << countFill[0*combHosts + j];

				for(int i=1;i<numHosts;i++) EV << "," << countFill[i*combHosts + j];

				EV << "}," << endl;
			}
		}
	#endif
	}

	void socketPeerClosed(int, void*)
	{
		socketStat.close();
	}

	inline void setTimeout(cMessage *event, short kind, simtime_t delay)
	{
		event->setKind(kind);
		scheduleAt(simTime() + delay, event);
	}

	void finish()
	{
		cancelEvent(evFill);
		cancelEvent(evStat);
		//socketFill.close(); //"CLOSE" msg can't be delivered to UDP module:
		                      //the event loop already terminated

	#ifdef QTENV_FINISH_LOG_BUG
		std::ostringstream ssLog;
	#else
		#define ssLog EV
	#endif
		for(int j=0;j<combHosts;j++){
			ssLog << "{" << countFill[0*combHosts + j];

			for(int i=1;i<numHosts;i++) ssLog << "," << countFill[i*combHosts + j];

			ssLog << "}," << endl;
		}
	#ifdef QTENV_FINISH_LOG_BUG
		auto strLog = ssLog.str();
		EV << strLog;

		//https://omnetpp.org/doc/omnetpp/manual/#sec:graphics:submdule-text-and-tooltip
		//https://omnetpp.org/doc/omnetpp/manual/#sec:graphics:changing-displaystrings-at-runtime
		//https://omnetpp.org/doc/omnetpp/manual/#cha:display-strings
		cDisplayString& parentDispStr = getParentModule()->getDisplayString();
		parentDispStr.setTagArg("t", 0, strLog.c_str());
		parentDispStr.setTagArg("t", 1, "l");

		//https://omnetpp.org/doc/omnetpp/manual/#sec:sim-lib:result-recording
		//https://omnetpp.org/doc/omnetpp/manual/#sec:ana-sim:scavetool
		//scavetool vector -F csv -O count.csv *-0.vec
		for(int i=0;i<numHosts;i++){
			cOutVector outVec("Fill count");
			for(int j=0;j<combHosts;j++) outVec.record(countFill[i*combHosts + j]);
		}
	#endif

		cSimpleModule::finish();
	}

	~LLTRSuperApp()
	{
		cancelAndDelete(evFill);
		cancelAndDelete(evStat);
	}
};

Define_Module(LLTRSuperApp);
}
