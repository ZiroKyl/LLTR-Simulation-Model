// See StandardHost.ned AND NodeBase.ned

package inet.node.inet;

import inet.common.packet.PcapRecorder;

import inet.linklayer.contract.IWiredNic;
import inet.linklayer.loopback.LoopbackInterface;
import inet.networklayer.contract.INetworkLayer;
import inet.networklayer.contract.IRoutingTable;
import inet.networklayer.common.InterfaceTable;

import inet.transportlayer.contract.ITCP;
import inet.transportlayer.contract.IUDP;
import inet.applications.contract.ITcpUdpApp;

//
// IPv4 host with TCP, UDP layers and applications.
// IP forwarding is disabled by default (see forwarding).
// - Can be connected via ethernet interface to other nodes using
//   the ethg gate. By default full-duplex connections are supported
//   only (twisted pair). Set **.eth[*].typename="EthernetInterface" for
//   a full/half-duplex CSMA/CD implementation (coaxial cable)
//
module SimpleUdpTcpHost
{
    parameters:
        @display("i=device/pc2");
        @networkNode;
        @labels(node,ethernet-node);
        int numApps = default(0);  // N of TCP apps. Specify the app types in INI file with app[0..1].typename="LLTRApp" syntax
        bool hasApp = default(numApps > 0);
        string tcpType = default(firstAvailableOrEmpty("TCP", "TCP_lwIP", "TCP_NSC"));  // tcp implementation (e.g. ~TCP, ~TCP_lwIP, ~TCP_NSC) or ~TCPSpoof
        string udpType = default(firstAvailableOrEmpty("UDP"));

        int numPcapRecorders = default(0); // no of PcapRecorders.
        string networkLayerType = default("IPv4NetworkLayer");
        string routingTableType = default("IPv4RoutingTable");
        networkLayer.proxyARP = default(false);
        bool forwarding = default(false);  // disable routing by default
        bool multicastForwarding = default(false);
        routingTable.forwarding = forwarding;
        routingTable.multicastForwarding = multicastForwarding;   // for IPv4, IPv6, Generic
        *.interfaceTableModule = default(absPath(".interfaceTable"));
        *.routingTableModule = default(routingTableType != "" ? absPath(".routingTable") : "");
    gates:
        inout ethg[] @labels(EtherFrame-conn);
    submodules:
        app[numApps]: <> like ITcpUdpApp {
            parameters:
                @display("p=329,54,row,60");
        }
        tcp: <tcpType> like ITCP if hasApp {
            parameters:
                @display("p=204,150");
        }
        udp: <udpType> like IUDP if hasApp {
            parameters:
                @display("p=455,150");
        }
        
        networkLayer: <networkLayerType> like INetworkLayer {
            parameters:
                @display("p=329,287;q=queue");
        }
        routingTable: <routingTableType> like IRoutingTable if routingTableType != "" {
            parameters:
                @display("p=53,250;is=s");
        }
        interfaceTable: InterfaceTable {
            parameters:
                @display("p=53,300;is=s");
        }
        pcapRecorder[numPcapRecorders]: PcapRecorder {
            @display("p=53,350,r,10");
        }
        lo0: LoopbackInterface {
            @display("p=212,287");
        }
        eth[sizeof(ethg)]: <default("EthernetInterface")> like IWiredNic {
            parameters:
                @display("p=329,406,row,60;q=txQueue");
        }
    connections allowunconnected:
        for i=0..numApps-1 {
            app[i].tcpOut --> tcp.appIn++;
            app[i].tcpIn <-- tcp.appOut++;
        }
        for i=0..numApps-1 {
            app[i].udpOut --> udp.appIn++;
            app[i].udpIn <-- udp.appOut++;
        }

        tcp.ipOut --> networkLayer.transportIn++ if hasApp;
        tcp.ipIn <-- networkLayer.transportOut++ if hasApp;

        udp.ipOut --> networkLayer.transportIn++ if hasApp;
        udp.ipIn <-- networkLayer.transportOut++ if hasApp;


        networkLayer.ifOut++ --> lo0.upperLayerIn;
        lo0.upperLayerOut --> networkLayer.ifIn++;

        for i=0..sizeof(ethg)-1 {
            ethg[i] <--> { @display("m=s"); } <--> eth[i].phys;
            eth[i].upperLayerOut --> networkLayer.ifIn++;
            eth[i].upperLayerIn <-- networkLayer.ifOut++;
        }
}
