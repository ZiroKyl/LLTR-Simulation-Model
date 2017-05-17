package inet.applications.lltrapp;

import inet.applications.contract.IUDPApp;


simple LLTRSuperApp like IUDPApp
{
    parameters:
        string interfaceTableModule;   // The path to the InterfaceTable module
        int port;
        
        @display("i=block/app");
    gates:
        input  udpIn  @labels(UDPControlInfo/up);
        output udpOut @labels(UDPControlInfo/down);
}
