#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"

using namespace ns3;

int main(int argc, char *argv[]) {
    // Set the resolution for time calculations
    Time::SetResolution (Time::NS);
    // Enable logging for the UdpEchoClientApplication and UdpEchoServerApplication
    LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

    // Create nodes
    NodeContainer nodes;
    nodes.Create(10);

    // Create mesh topology
    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));

    NetDeviceContainer devices;
    devices = pointToPoint.Install(nodes);

    InternetStackHelper stack;
    stack.Install(nodes);

    Ipv4AddressHelper address;
    address.SetBase ("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer interfaces = address.Assign(devices);

    // Install UDP echo server on each node
    UdpEchoServerHelper echoServer(9);

    ApplicationContainer serverApps;
    for (uint32_t i = 0; i < nodes.GetN(); ++i) {
        serverApps.Add(echoServer.Install(nodes.Get(i)));
    }
    serverApps.Start(Seconds(1.0));
    serverApps.Stop(Seconds(10.0));

    // Install UDP echo client on each node
    UdpEchoClientHelper echoClient(interfaces.GetAddress(9), 9);
    echoClient.SetAttribute("MaxPackets", UintegerValue(3));
    echoClient.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient.SetAttribute("PacketSize", UintegerValue(1024));

    ApplicationContainer clientApps;
    for (uint32_t i = 0; i < nodes.GetN(); ++i) {
        clientApps.Add(echoClient.Install(nodes.Get(i)));
    }
    clientApps.Start(Seconds(2.0));
    clientApps.Stop(Seconds(10.0));

    // Create animation trace
    AnimationInterface anim("mesh_udp.xml");
    anim.SetConstantPosition(nodes.Get(0), 0.0, 0.0);
    anim.SetConstantPosition(nodes.Get(1), 1.0, 1.0);
    anim.SetConstantPosition(nodes.Get(2), 2.0, 0.0);
    anim.SetConstantPosition(nodes.Get(3), 3.0, 1.0);
    anim.SetConstantPosition(nodes.Get(4), 4.0, 0.0);
    anim.SetConstantPosition(nodes.Get(5), 5.0, 1.0);
    anim.SetConstantPosition(nodes.Get(6), 6.0, 0.0);
    anim.SetConstantPosition(nodes.Get(7), 7.0, 1.0);
    anim.SetConstantPosition(nodes.Get(8), 8.0, 0.0);
    anim.SetConstantPosition(nodes.Get(9), 9.0, 1.0);

    // Enable ASCII trace
    AsciiTraceHelper ascii;
    pointToPoint.EnableAsciiAll(ascii.CreateFileStream("mesh_udp.tr"));

    // Run the simulation
    Simulator::Run ();
    Simulator::Destroy ();

    return 0;
}
