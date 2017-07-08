// LLTRApp.cc : Defines LLTR Application.
//

#include <vector>


#include "inet/common/INETDefs.h"

#include "inet/transportlayer/contract/udp/UDPSocket.h"
#include "inet/transportlayer/contract/tcp/TCPSocket.h"


#include "inet/applications/lltrapp/FillPayload_m.h"
#include "inet/applications/lltrapp/StatPayload_m.h"

#include "inet/applications/lltrapp/LLTRUtils.h"

namespace inet {
using namespace LLTR;

class INET_API LLTRApp: public cSimpleModule, public TCPSocket::CallbackInterface
{
	/* save state in "global" variables is practical to slow down && bugging your code */

	cMessage *evTrickle = nullptr; //as evFill in LLTRSuperApp

	LLTRStep step = INIT;
	uint32 iN = 0; //iteration Number
	int  pktN = 0; //packet    Number (sub iteration Number)

	int  port = -1;
	long pktLen = 0;

	int thisHostId = 0;

	L3Address destHost;

	int gateUdpId; //socketTrickle
	int gateTcpId; //socketStat

	UDPSocket socketTrickle; //as socketFill in LLTRSuperApp;  incoming - Fill, outgoing - Trickle
	TCPSocket socketStat;

	std::vector<int> countFill; //[combHosts]
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

			thisHostId = atoi(&getParentModule()->getName()[4]);
			gateUdpId = gateBaseId("udpIn");
			gateTcpId = gateBaseId("tcpIn");

			numHosts = getHostsCount(getParentModule()->getComponentType()) - 1;	//"host*" without "hostS" (-1)
			combHosts = (numHosts-1)*(numHosts);

			countFill.resize(1 + combHosts);

			WATCH(step);
			WATCH(iN);
			WATCH_VECTOR(countFill);

			evTrickle = new cMessage("Trickle");

		}break;
		case INITSTAGE_APPLICATION_LAYER:{
			socketTrickle.setOutputGate(gate("udpOut"));
			socketTrickle.setTimeToLive(1);   //to transmit Trickle
			socketTrickle.setBroadcast(true); //to receive  Fill
			socketTrickle.bind(port);         //to receive  Fill

			socketStat.setOutputGate(gate("tcpOut"));
			socketStat.setDataTransferMode(TCP_TRANSFER_OBJECT);
			socketStat.setCallbackObject(this);
			socketStat.bind(port);
			socketStat.listenOnce();

			UDPSocket socketDISCARD;
			socketDISCARD.setOutputGate(gate("udpOut"));
			socketDISCARD.bind(9);

			iN = 0;

		}break;
		case INITSTAGE_LAST:{
			//послать ARP-запросы всем родным хостам, т.к. дальше пакеты пройдут с трудом
			HostPath_defSuperHost_ path;
			socketTrickle.sendTo(new cPacket("To ARP"), path.GetIp(), 9);
			for(int i=0; i<numHosts; i++) socketTrickle.sendTo(new cPacket("To ARP"), path(i), 9);
		}break;
		}
	}

	void handleMessage(cMessage *msg)
	{
		if(msg->isSelfMessage()){
			trickleSend();
		}else{
			//for multiple gates use std::map<int, (f)()> AND msg->getArrivalGateId()
			if     (msg->arrivedOn(gateUdpId) && socketTrickle.belongsToSocket(msg)) handleUdp(msg);
			else if(msg->arrivedOn(gateTcpId)) socketStat.processMessage(msg);
			else delete msg;
		}
	}

	inline void handleUdp(cMessage *msg)
	{
		switch(msg->getKind()){
		case UDP_I_DATA:{
			LLTRStep step;
			uint32_t  iteration;
			{
				FillPayload* fill = check_and_cast<FillPayload*>(msg);
				step       = fill->getStep();
				iteration  = fill->getIterationNumber();
			}
			delete msg;

			if(iteration >= combHosts) iteration=0;	//anti buffer overflow

			if(step < this->step) break;
			if(step > this->step) this->step = step;

			switch(step){
			case PROBING:{
				/*if(iteration+1 == iN)*/countFill[iN]++;	//calc only LLTRSuperApp fill (LLTRApp trickle always have step==0)
				if(iteration+1 <= iN) break;
				if(iteration+1 >  iN) iN = iteration+1;

				cancelEvent(evTrickle);	//stop transfer


				//https://omnetpp.org/doc/omnetpp/manual/#sec:graphics:submodule-status-icon
				cDisplayString& parentDispStr = getParentModule()->getDisplayString();

				int uSrcHostId = iteration/(numHosts-1);
				int uDstHostId = (uSrcHostId + (iteration%(numHosts-1)+1))%numHosts;

				if(uSrcHostId == thisHostId) {
					destHost = HostPath_defSuperHost_(uDstHostId).GetIp();

					EV << "!!!!!! Iteration: " << iteration << " | " << thisHostId << " ~ " << iteration%(numHosts-1) << endl;
					EV << destHost << endl;

					parentDispStr.setTagArg("i2", 0, "status/up");

					pktN = 0;
					trickleSend();
				}else if(uDstHostId == thisHostId){
					parentDispStr.setTagArg("i2", 0, "status/down");
				}else parentDispStr.removeTag("i2");
			}break;
			case COLLECT:{
				cancelEvent(evTrickle);	//stop transfer
			}break;
			}
		}break;
		case UDP_I_ERROR:{
			EV_WARN << "Ignoring UDP error report" << endl;
			delete msg;

		}break;
		default: throw cRuntimeError("Unrecognized message (%s)%s", msg->getClassName(), msg->getName());
		}
	}

	void trickleSend()
	{
		FillPayload *zeroPkt = new FillPayload();
		zeroPkt->setStep(INIT);
		zeroPkt->setIterationNumber(0);
		zeroPkt->setByteLength(pktLen);

		socketTrickle.sendTo(zeroPkt, destHost, port);

		if(++pktN < 300) setTimeout(0, SimTime(136,SIMTIME_US)); //128us before
	}

	void socketEstablished(int, void*)
	{
		StatPayload *stat = new StatPayload();
		Array6 &pcount = stat->getFillCount();
		pcount.resize(combHosts);

		for(int i=0;i<combHosts;i++) pcount[i]=countFill[1+i]; //std::copy() ...
		stat->setByteLength(sizeof(pcount[0])*pcount.size());

		socketStat.send(stat);
	}

	void socketDataArrived(int, void*, cPacket*, bool)
	{

	}

	void socketPeerClosed(int, void*) {
		socketStat.close();
		socketStat.renewSocket();
		socketStat.bind(port);
	}

	inline void setTimeout(short kind, simtime_t delay)
	{
		evTrickle->setKind(kind);
		scheduleAt(simTime() + delay, evTrickle);
	}

	void finish()
	{
		cancelEvent(evTrickle);
		//socketTrickle.close(); //"CLOSE" msg can't be delivered to UDP module:
				                 //the event loop already terminated

		cSimpleModule::finish();
	}


	~LLTRApp()
	{
		cancelAndDelete(evTrickle);
	}
};

Define_Module(LLTRApp);
}
