package inet.applications.lltrapp;

import inet.applications.contract.ITcpUdpApp;


simple LLTRApp like ITcpUdpApp
{
    parameters:
        string interfaceTableModule; // The path to the InterfaceTable module
        int port;
        int packetLength @unit(B);   // length of messages to generate, in bytes
        
        @display("i=block/app");
    gates:
        input  udpIn  @labels(UDPControlInfo/up);
        output udpOut @labels(UDPControlInfo/down);
        input  tcpIn  @labels(TCPCommand/up);
        output tcpOut @labels(TCPCommand/down);
}
