package inet.applications.lltrapp;

import inet.applications.contract.ITcpUdpApp;


simple LLTRApp like ITcpUdpApp
{
    parameters:
        int port;
        
        @display("i=block/app");
    gates:
        input  udpIn  @labels(UDPControlInfo/up);
        output udpOut @labels(UDPControlInfo/down);
        input  tcpIn  @labels(TCPCommand/up);
        output tcpOut @labels(TCPCommand/down);
}
