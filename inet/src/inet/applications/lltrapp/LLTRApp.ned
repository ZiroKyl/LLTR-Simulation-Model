package inet.applications.lltrapp;


simple LLTRApp
{
    parameters:
        @display("i=block/app");
    gates:
        input  udpIn  @labels(UDPControlInfo/up);
        output udpOut @labels(UDPControlInfo/down);
}
