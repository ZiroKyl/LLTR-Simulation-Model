package lltr;

import inet.networklayer.configurator.ipv4.IPv4NetworkConfigurator;

import inet.node.inet.SimpleUdpTcpHost;
import inet.node.ethernet.EtherSwitch;

import inet.node.ethernet.Eth100M;


network Network_tree
{
    @display("bgb=616,370,grey99");
    submodules:
        configurator: IPv4NetworkConfigurator {
            @display("p=62,31");
            config = xmldoc("config.xml");
        }
        switch0: EtherSwitch    { @display("p=300, 50"); }
        switch1: EtherSwitch    { @display("p=100,200"); }
        switch2: EtherSwitch    { @display("p=300,200"); }
        switch3: EtherSwitch    { @display("p=500,200"); }
        hostS: SimpleUdpTcpHost { @display("p= 50,300"); }
        host0: SimpleUdpTcpHost { @display("p=150,300"); }
        host1: SimpleUdpTcpHost { @display("p=250,300"); }
        host2: SimpleUdpTcpHost { @display("p=350,300"); }
        host3: SimpleUdpTcpHost { @display("p=450,300"); }
        host4: SimpleUdpTcpHost { @display("p=550,300"); }
    connections:
        switch1.ethg++ <--> Eth100M <--> hostS.ethg++;
        switch1.ethg++ <--> Eth100M <--> host0.ethg++;
        switch2.ethg++ <--> Eth100M <--> host1.ethg++;
        switch2.ethg++ <--> Eth100M <--> host2.ethg++;
        switch3.ethg++ <--> Eth100M <--> host3.ethg++;
        switch3.ethg++ <--> Eth100M <--> host4.ethg++;

        switch0.ethg++ <--> Eth100M <--> switch1.ethg++;
        switch0.ethg++ <--> Eth100M <--> switch2.ethg++;
        switch0.ethg++ <--> Eth100M <--> switch3.ethg++;
}

