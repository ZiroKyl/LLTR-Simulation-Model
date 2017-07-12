package lltr;

import inet.networklayer.configurator.ipv4.IPv4NetworkConfigurator;

import inet.node.inet.SimpleUdpTcpHost;
import inet.node.ethernet.EtherSwitch;

import inet.node.ethernet.Eth100M;


network Network_serial_len_test
{
    @display("bgb=566,370,grey99");
    submodules:
        configurator: IPv4NetworkConfigurator {
            @display("p=62,31");
            config = xmldoc("config.xml");
        }
        switch0: EtherSwitch    { @display("p=375,130"); }
        switch1: EtherSwitch    { @display("p=275,150"); }
        switch2: EtherSwitch    { @display("p=175,150"); }
        switch3: EtherSwitch    { @display("p= 75,150"); }
        switch4: EtherSwitch    { @display("p=475,150"); }
        hostS: SimpleUdpTcpHost { @display("p=400,270"); }
        host0: SimpleUdpTcpHost { @display("p=350,270"); }
        host1: SimpleUdpTcpHost { @display("p=100,250"); }
        host2: SimpleUdpTcpHost { @display("p= 50,250"); }
        host3: SimpleUdpTcpHost { @display("p=450,250"); }
        host4: SimpleUdpTcpHost { @display("p=500,250"); }
    connections:
        switch0.ethg++ <--> Eth100M <--> hostS.ethg++;
        switch0.ethg++ <--> Eth100M <--> host0.ethg++;
        switch3.ethg++ <--> Eth100M <--> host1.ethg++;
        switch3.ethg++ <--> Eth100M <--> host2.ethg++;
        switch4.ethg++ <--> Eth100M <--> host3.ethg++;
        switch4.ethg++ <--> Eth100M <--> host4.ethg++;

        switch0.ethg++ <--> Eth100M <--> switch1.ethg++;
        switch1.ethg++ <--> Eth100M <--> switch2.ethg++;
        switch2.ethg++ <--> Eth100M <--> switch3.ethg++;
        switch0.ethg++ <--> Eth100M <--> switch4.ethg++;

}

