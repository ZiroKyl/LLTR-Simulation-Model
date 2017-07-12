package lltr;

import inet.networklayer.configurator.ipv4.IPv4NetworkConfigurator;

import inet.node.inet.SimpleUdpTcpHost;
import inet.node.ethernet.EtherSwitch;

import inet.node.ethernet.Eth100M;


network Network_serial_len_test_simple
{
    @display("bgb=693,416,grey99");
    submodules:
        configurator: IPv4NetworkConfigurator {
            @display("p=62,31");
            config = xmldoc("config.xml");
        }
        switch0: EtherSwitch    { @display("p=240,203"); }
        switch1: EtherSwitch    { @display("p=408,203"); }
        switch2: EtherSwitch    { @display("p=296,203"); }
        switch3: EtherSwitch    { @display("p=352,203"); }
        hostS: SimpleUdpTcpHost { @display("p=149,269"); }
        host0: SimpleUdpTcpHost { @display("p=149,141"); }
        host1: SimpleUdpTcpHost { @display("p=500,269"); }
        host2: SimpleUdpTcpHost { @display("p=500,141"); }
    connections:
        switch0.ethg++ <--> Eth100M <--> hostS.ethg++;
        switch0.ethg++ <--> Eth100M <--> host0.ethg++;
        switch1.ethg++ <--> Eth100M <--> host1.ethg++;
        switch1.ethg++ <--> Eth100M <--> host2.ethg++;
        
        switch0.ethg++ <--> Eth100M <--> switch2.ethg++;
        switch2.ethg++ <--> Eth100M <--> switch3.ethg++;
        switch3.ethg++ <--> Eth100M <--> switch1.ethg++;
}

