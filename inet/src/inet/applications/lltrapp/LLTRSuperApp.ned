package inet.applications.lltrapp;

import inet.applications.contract.IUDPApp;


simple LLTRSuperApp like IUDPApp
{
    parameters:
        @display("i=block/app");
    gates:
        input  udpIn  @labels(UDPControlInfo/up);
        output udpOut @labels(UDPControlInfo/down);
}
