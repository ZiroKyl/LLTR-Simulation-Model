package lltr;

import inet.networklayer.configurator.ipv4.IPv4NetworkConfigurator;

import inet.node.inet.SimpleUdpTcpHost;
import inet.node.ethernet.EtherSwitch;

import inet.node.ethernet.Eth100M;


network Network
{
    parameters:
        @display("bgb=693,416,grey99");
        host[0].p_x=149; host[0].p_y=269;
        host[1].p_x=149; host[1].p_y=141;
        host[2].p_x=500; host[2].p_y=269;
        host[3].p_x=500; host[3].p_y=141;
    types:
        module Host extends SimpleUdpTcpHost {
            @display("p=$p_x,$p_y");
            int p_x = default(0);
            int p_y = default(0);
        }
    submodules:
        configurator: IPv4NetworkConfigurator {
            @display("p=62,31");
            config = xmldoc("config.xml");
        }
        switch0: EtherSwitch    { @display("p=240,203"); }
        switch1: EtherSwitch    { @display("p=408,203"); }
        host[4]: Host;
    connections:
        switch0.ethg++ <--> Eth100M <--> host[0].ethg++;
        switch0.ethg++ <--> Eth100M <--> host[1].ethg++;
        switch1.ethg++ <--> Eth100M <--> host[2].ethg++;
        switch1.ethg++ <--> Eth100M <--> host[3].ethg++;
        switch0.ethg++ <--> Eth100M <--> switch1.ethg++;
}
