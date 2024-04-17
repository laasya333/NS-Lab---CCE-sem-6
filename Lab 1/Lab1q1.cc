#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/netanim-module.h"

// Default Network Topology
//
//       172.16.1.0       172.16.2.0
//    0             1   0            1
// n0 -------------- n1 -------------- n2 -->server
//         p2p               p2p
//n0 is the client

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("SecondScriptExample");

int 
main ()
{
  NodeContainer p2pNodes;
  p2pNodes.Create (3);
  
  NodeContainer n0n1 = NodeContainer (p2pNodes.Get (0), p2pNodes.Get(1));
  NodeContainer n1n2 = NodeContainer (p2pNodes.Get (1), p2pNodes.Get(2));
  
  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
  NetDeviceContainer d0d1 = pointToPoint.Install (n0n1);
  NetDeviceContainer d1d2 = pointToPoint.Install (n1n2);
  
  InternetStackHelper stack;
  stack.Install (p2pNodes);
  // Later, we add IP addresses.
  NS_LOG_INFO ("Assign IP Addresses.");
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i0i1 = ipv4.Assign (d0d1);
  //i0i1 

  ipv4.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer i1i2 = ipv4.Assign (d1d2);
  
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  UdpEchoServerHelper echoServer (9);
  //node 3 is the server
  ApplicationContainer serverApps = echoServer.Install (p2pNodes.Get (2));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  UdpEchoClientHelper echoClient (i1i2.GetAddress (1), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (6));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps = echoClient.Install (p2pNodes.Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));

  AnimationInterface anim ("L1q1.xml");
  anim.SetConstantPosition (p2pNodes.Get(0), 9.0, 13.0);
  anim.SetConstantPosition (p2pNodes.Get(1), 70.0, 32.30);
  anim.SetConstantPosition (p2pNodes.Get(2), 60.5, 3.3);
  
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}

