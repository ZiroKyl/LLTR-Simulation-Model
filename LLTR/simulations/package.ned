package lltr;

import inet.networklayer.configurator.ipv4.IPv4NetworkConfigurator;

import inet.node.inet.StandardHost;
import inet.node.ethernet.EtherSwitch;

import inet.node.ethernet.Eth100M;


network Network
{
    @display("bgb=693,416,grey99");
    submodules:
        configurator: IPv4NetworkConfigurator {
            parameters:
                @display("p=62,31");
        }
        switch0: EtherSwitch { @display("p=240,203"); }
        switch1: EtherSwitch { @display("p=408,203"); }
        host0: StandardHost  { @display("p=149,269"); }
        host1: StandardHost  { @display("p=149,141"); }
        host2: StandardHost  { @display("p=500,269"); }
        host3: StandardHost  { @display("p=500,141"); }
    connections:
        switch0.ethg++ <--> Eth100M <--> host0.ethg++;
        switch0.ethg++ <--> Eth100M <--> host1.ethg++;
        switch1.ethg++ <--> Eth100M <--> host2.ethg++;
        switch1.ethg++ <--> Eth100M <--> host3.ethg++;
        switch0.ethg++ <--> Eth100M <--> switch1.ethg++;
}
