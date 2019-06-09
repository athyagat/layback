// Network Descriptor File

simple src
{
    parameters:
        int    numSrc @prompt(" Number of Src")                    = default(3);
        double meanCallDuration @prompt(" Mean Call Duration")     = default(10);
        double meanCallRate @prompt(" Mean Call Rate")             = default(10);
        int    numBlock @prompt(" Number of Funcational Block")    = default(3);
        int    callLoad @prompt(" Call load type")                 = default(0);
        int    sharingMode @prompt(" Sharing vs. no-sharing")      = default(0);
    gates:
        inout  src_to_fb;
}

simple fb
{
    parameters:
        int    numSrc @prompt(" Number of Src")                 = default(3);
        double fb_to_fb_link @prompt("fb to fb link speed")     = default(10);
        double fb_to_sink_link @prompt("fb to sink link speed") = default(10);
        int    numBlock @prompt(" Number of functional blocks") = default(3);
    gates:
        inout src_to_fb;
        inout fb_to_sink;
        inout fb_to_other;
        inout other_to_fb;
        inout fb_to_SDN;
}

simple sink
{
    parameters:
        int numSrc @prompt(" Number of Src") = default(3);
    gates:
        inout fb_to_sink[numSrc];
        inout SDN_to_sink;
}

simple SDN
{
    parameters:
        int numSrc       @prompt(" Number of Src")               = default(3);
        int numBlock     @prompt(" Number of functional blocks") = default(3);
        int sharingMode  @prompt(" Sharing vs. no-sharing")      = default(0);
    gates:
        inout SDN_to_fb[numSrc * numBlock];
        inout SDN_to_sink;
}

network Network
{
    parameters:
        int numSrc @prompt(" Number of Src")                 = default(3);
        int numBlock @prompt(" Number of functional blocks") = default(3);

        @display("bgb=915,520");

    types:
        channel C extends ned.DatarateChannel
        {
            //  delay = 0.000005 s;
            delay    = 0.000005 s;
            datarate = 1 Gbps;
        }

    submodules:
        src[numSrc]: src {
            @display("p=130.9575,234.345,c,100");
        }
        fb[numSrc * numBlock]: fb {
            @display("p=302.12125,234.345,m,4,100,100");
        }
        sink: sink {
            @display("p=690.39874,234.345");
        }
        sdn: SDN {
            @display("p=499.70627,99.94125");
        }
    connections allowunconnected:

        // Source to functional block connections
        for j=0..(numSrc-1) {
            src[j].src_to_fb <--> fb[j*numBlock].src_to_fb;
        }

        // Functional block to functional block (horizontal) connections
        for i=0..(numSrc-1), for j=0..(numBlock-2) {
            fb[j+(i*numBlock)].fb_to_sink <--> fb[j+1+(i*numBlock)].src_to_fb;
        }

        // Functional block to sink connections
        for j=1..(numSrc) {
            fb[(j*numBlock-1)].fb_to_sink <--> sink.fb_to_sink++;
        }

        // Functional block to sink (horizontal) connections
        for j=0..(numBlock-1), for i=0..(numSrc-2) {
            fb[j+(i*numBlock)].fb_to_other <--> fb[j+(i*numBlock)+numBlock].other_to_fb;
        }
        // Functional block to SDN connections
        for j=0..((numBlock*numSrc)-1) {
            fb[j].fb_to_SDN <--> sdn.SDN_to_fb++;
        }

        sdn.SDN_to_sink <--> sink.SDN_to_sink;
}

