package lltr;

import inet.node.inet.StandardHost;
import inet.node.ethernet.EtherSwitch;

import inet.node.ethernet.Eth100M;


network Network
{
    @display("bgb=693,416,grey99");
    submodules:
        switch0: EtherSwitch { @display("p=240,203"); eth[*].mac.duplexMode = true; csmacdSupport = false; }
        switch1: EtherSwitch { @display("p=408,203"); eth[*].mac.duplexMode = true; csmacdSupport = false; }
        host0: StandardHost  { @display("p=149,269"); eth[*].mac.duplexMode = true; }
        host1: StandardHost  { @display("p=149,141"); eth[*].mac.duplexMode = true; }
        host2: StandardHost  { @display("p=500,269"); eth[*].mac.duplexMode = true; }
        host3: StandardHost  { @display("p=500,141"); eth[*].mac.duplexMode = true; }
    connections:
        switch0.ethg++ <--> Eth100M <--> host0.ethg++;
        switch0.ethg++ <--> Eth100M <--> host1.ethg++;
        switch1.ethg++ <--> Eth100M <--> host2.ethg++;
        switch1.ethg++ <--> Eth100M <--> host3.ethg++;
        switch0.ethg++ <--> Eth100M <--> switch1.ethg++;
}
