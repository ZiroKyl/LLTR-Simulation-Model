package inet.applications.lltrapp;

import inet.applications.contract.IUDPApp;
import inet.applications.contract.ITCPApp;


simple LLTRSuperApp like IUDPApp, ITCPApp
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
