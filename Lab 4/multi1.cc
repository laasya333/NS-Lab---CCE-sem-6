/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
// Network topology
//
//                     Lan1
//                 ===========
//                 |    |    |
//       n0 -  n1 -  n2   n3   n4
//               |
//       ===========
//           Lan0
//
// - Multicast source is at node n0;
// - Multicast forwarded by node n2 onto LAN1;
// - Nodes n0, n1, n2, n3, and n4 receive the multicast frame.
// - Node n4 listens for the data
#include <iostream>
#include <fstream>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/csma-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"
using namespace ns3;
NS_LOG_COMPONENT_DEFINE ("CsmaMulticastExample");
int main (int argc, char *argv[])
{
  //
  // Users may find it convenient to turn on explicit debugging
  // for selected modules; the below lines suggest how to do this
  //
  // LogComponentEnable ("CsmaMulticastExample", LOG_LEVEL_INFO);
  //
  // Set up default values for the simulation.
  //
  // Select DIX/Ethernet II-style encapsulation (no LLC/Snap header)
  Config::SetDefault ("ns3::CsmaNetDevice::EncapsulationMode", StringValue ("Dix"));
  // Allow the user to override any of the defaults at
  // run-time, via command-line arguments
  CommandLine cmd (__FILE__);
  cmd.Parse (argc, argv);
  NS_LOG_INFO ("Create nodes.");
  // Create nodes
  NodeContainer allNodes;
  allNodes.Create (5);
  // Create point-to-point and CSMA node containers
  NodeContainer p2pNodes = NodeContainer (allNodes.Get (0), allNodes.Get (1));
  NodeContainer p2pNodes1 = NodeContainer (allNodes.Get (1), allNodes.Get (2));
  NodeContainer csmaNodes = NodeContainer (allNodes.Get (2), allNodes.Get (3), allNodes.Get (4));
  // Setup point-to-point links
  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
  NetDeviceContainer p2pDevices = pointToPoint.Install (p2pNodes);
  NetDeviceContainer p2pDevices1 = pointToPoint.Install (p2pNodes1);
  // Setup CSMA links
  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", DataRateValue (DataRate (5000000)));
  csma.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (2)));
  NetDeviceContainer csmaDevices = csma.Install (csmaNodes);
  // Install the internet stack on all nodes
  InternetStackHelper stack;
  stack.Install (allNodes);
  // Assign IP addresses to point-to-point devices
  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces = address.Assign (p2pDevices);
  address.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces1 = address.Assign (p2pDevices1);
 
  // Assign IP addresses to CSMA devices
  address.SetBase ("10.1.3.0", "255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces = address.Assign (csmaDevices);
  NS_LOG_INFO ("Configure multicasting.");
  //
  // Now we can configure multicasting.  As described above, the multicast
  // source is at node zero, which we assigned the IP address of 10.1.1.1
  // earlier.  We need to define a multicast group to send packets to.  This
  // can be any multicast address from 224.0.0.0 through 239.255.255.255
  // (avoiding the reserved routing protocol addresses).
  Ipv4Address multicastSource ("10.1.1.1");
  Ipv4Address multicastGroup ("225.1.2.4");
  // Configure a static multicast route on node n2 (the multicast router)
  Ptr<Node> multicastRouter = allNodes.Get (2);
  Ptr<NetDevice> inputIf = p2pDevices1.Get (1); // Interface of n2 on p2p network
  NetDeviceContainer outputDevices;
  outputDevices.Add (csmaDevices.Get (0)); // Interface of n2 on CSMA network
  // Now, we will set up multicast routing.  We need to do three things:
  // 1) Configure a (static) multicast route on node n2
  // 2) Set up a default multicast route on the sender n0
  // 3) Have node n4 join the multicast group
  // We have a helper that can help us with static multicast
  Ipv4StaticRoutingHelper multicast;
  multicast.AddMulticastRoute (multicastRouter, multicastSource, multicastGroup, inputIf, outputDevices);
  // Set up a default multicast route on the sender n0
  Ptr<Node> sender = allNodes.Get (0);
  Ptr<NetDevice> senderIf = p2pDevices.Get (0);
  multicast.SetDefaultMulticastRoute (sender, senderIf);
  UdpEchoServerHelper echoServer (9);
  ApplicationContainer serverApps = echoServer.Install (allNodes.Get (4));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));
  UdpEchoClientHelper echoClient (csmaInterfaces.GetAddress (2), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));
  ApplicationContainer clientApps = echoClient.Install (allNodes.Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));
  // Create a multicast sender application on n0
  /*uint16_t multicastPort = 9; // Discard port (RFC 863)
  OnOffHelper onoff ("ns3::UdpSocketFactory", Address (InetSocketAddress (multicastGroup, multicastPort)));
  onoff.SetConstantRate (DataRate ("255b/s"));
  onoff.SetAttribute ("PacketSize", UintegerValue (128));
  ApplicationContainer srcC = onoff.Install (allNodes.Get (0));
  srcC.Start (Seconds (1.0));
  srcC.Stop (Seconds (10.0));

  // Create a multicast receiver application on n4
  PacketSinkHelper sink ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), multicastPort));
  ApplicationContainer sinkC = sink.Install (allNodes.Get (2));
  sinkC.Start (Seconds (1.0));
  sinkC.Stop (Seconds (10.0));*/
  // Set up tracing if needed
  AsciiTraceHelper ascii;
  pointToPoint.EnableAsciiAll (ascii.CreateFileStream ("p2p-multicast.tr"));
  csma.EnableAsciiAll (ascii.CreateFileStream ("csma-multicast.tr"));
  AnimationInterface anim ("multicastmodified.xml");
  anim.SetConstantPosition(allNodes.Get(0),10.0,10.0);
  anim.SetConstantPosition(allNodes.Get(1),20.0,20.0);
  anim.SetConstantPosition(allNodes.Get(2),30.0,30.0);
  anim.SetConstantPosition(allNodes.Get(3),40.0,40.0);
  anim.SetConstantPosition(allNodes.Get(4),50.0,50.0);
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
  // Run the simulation
  Simulator::Run ();
  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");
}