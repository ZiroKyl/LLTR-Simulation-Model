package inet.applications.lltrapp;

import inet.applications.contract.ITcpUdpApp;


simple LLTRSuperApp like ITcpUdpApp
{
    parameters:
        string interfaceTableModule;   // The path to the InterfaceTable module
        int port;
        
        @display("i=block/app");
    gates:
        input  udpIn  @labels(UDPControlInfo/up);
        output udpOut @labels(UDPControlInfo/down);
        input  tcpIn  @labels(TCPCommand/up);
        output tcpOut @labels(TCPCommand/down);
}
