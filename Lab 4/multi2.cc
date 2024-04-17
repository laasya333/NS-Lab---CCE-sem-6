
// Network topology

//

//                         Lan1                Lan2

//                    ======          =====        

//                      |       |     |          |        |

//       n0   n1   n2   n3   n4------n5     n6

//       |       |      |

//       =======

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

#include "ns3/csma-module.h"

#include "ns3/applications-module.h"

#include "ns3/internet-module.h"

#include "ns3/netanim-module.h"

#include "ns3/point-to-point-module.h"

using namespace ns3;
NS_LOG_COMPONENT_DEFINE ("CsmaMulticastExample");
int

main (int argc, char *argv[])

{​​​​​​
 
  NS_LOG_INFO ("Create 7 nodes.");

  NodeContainer c;

  c.Create (7);

  NS_LOG_INFO ("Create 3 Lans with devices n0,n1,n2| n2,n3,n4 | n5,n6");

  NodeContainer c0 = NodeContainer (c.Get (0), c.Get (1), c.Get (2));

  NodeContainer c1 = NodeContainer (c.Get (2), c.Get (3), c.Get (4));

  NodeContainer c2 = NodeContainer (c.Get (5), c.Get (6));

  NodeContainer p1 = NodeContainer (c.Get (4), c.Get (5));
  NS_LOG_INFO ("Build Topology.");
  CsmaHelper csma;

  csma.SetChannelAttribute ("DataRate", DataRateValue (DataRate (5000000)));

  csma.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (2)));
  PointToPointHelper p2p;

  p2p.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));

  p2p.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer nd0 = csma.Install (c0);  // First LAN

  NetDeviceContainer nd1 = csma.Install (c1);  // Second LAN

  NetDeviceContainer nd2 = csma.Install (c2);  // Third LAN

  NetDeviceContainer nd3 = p2p.Install (p1);   //P2P from second lan to third lan
 
  NS_LOG_INFO ("Assign IP Addresses.");

  InternetStackHelper internet;

  internet.Install (c);

  Ipv4AddressHelper ipv4Addr;

  ipv4Addr.SetBase ("10.1.1.0", "255.255.255.0");

  ipv4Addr.Assign (nd0);

  ipv4Addr.SetBase ("10.1.2.0", "255.255.255.0");

  ipv4Addr.Assign (nd1);

  ipv4Addr.SetBase ("10.1.3.0", "255.255.255.0");

  ipv4Addr.Assign (nd2);

  ipv4Addr.SetBase ("10.1.4.0", "255.255.255.0");

  ipv4Addr.Assign (nd3);

  NS_LOG_INFO ("Configure multicasting.");
  Ipv4Address multicastSource ("10.1.1.1");

  Ipv4Address multicastGroup ("225.1.2.4");

  Ipv4StaticRoutingHelper multicast;

//MULTICAST ROYUTER INPUT

  Ptr<Node> multicastRouter = c.Get (2);  // SET Node2 as Multicast Router

  Ptr<NetDevice> inputIf = nd0.Get (2);  // SET Node2 Interface from nd0 as input Interface for Multicast Router
//MULTICAST ROUTER OUTPUT

  NetDeviceContainer outputDevices;

  outputDevices.Add (nd1.Get (0));  // Set Node2 Interface from nd1 as output interface

  // outputDevices.Add (nd3.Get(0));
 
//SET Packet sending node as Node0 Interface from nd0

  multicast.AddMulticastRoute (multicastRouter, multicastSource, multicastGroup, inputIf, outputDevices);

  Ptr<Node> sender = c.Get (0);

  Ptr<NetDevice> senderIf = nd0.Get (0);

  multicast.SetDefaultMulticastRoute (sender, senderIf);  // Takes sent packets from Node0, sets a route through Node2

  NS_LOG_INFO ("Create Applications: Sender on node0 and receiver on Node5");
  int port = 9;   // Discard port (RFC 863)
 

  OnOffHelper onoff ("ns3::UdpSocketFactory", Address (InetSocketAddress (multicastGroup, port)));

  onoff.SetConstantRate (DataRate ("255b/s"));

  onoff.SetAttribute ("PacketSize", UintegerValue (128));
  ApplicationContainer srcC = onoff.Install (c0.Get (0));

  srcC.Start (Seconds (1.));

  srcC.Stop (Seconds (10.));
  // Create an optional packet sink to receive these packets

  PacketSinkHelper sink ("ns3::UdpSocketFactory",

                         InetSocketAddress (Ipv4Address::GetAny (), port));
  ApplicationContainer sinkC = sink.Install (c2.Get (0)); // Node n4

  // Start the sink

  sinkC.Start (Seconds (1.0));

  sinkC.Stop (Seconds (10.0));
  NS_LOG_INFO ("Configure Tracing.");
  AsciiTraceHelper ascii;

  csma.EnableAsciiAll (ascii.CreateFileStream ("l4q3.tr"));

  NS_LOG_INFO ("Run Simulation.");
  AnimationInterface anim ("csmamnew.xml");

  Simulator::Run ();

  Simulator::Destroy ();

  NS_LOG_INFO ("Done.");

}​​​​​​
[2/8 9:17 AM] Nisha P Shetty [MAHE-MIT]
TCP Program
https://www.youtube.com/watch?v=I8jn4vKm5QA
https://www.youtube.com/watch?v=9rkN3FtOkaQ&t=412s
Comparison of TCP Protocols u…
6K views · 3 years ago
[2/19 9:18 AM] Nisha P Shetty [MAHE-MIT]
RIP
https://www.youtube.com/watch?v=bCXI1GoCIj4
[3/4 3:01 PM] KADALI PRAHARSHINI - 210953242
third.cc 
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
 
#include "ns3/core-module.h"

#include "ns3/point-to-point-module.h"

#include "ns3/network-module.h"

#include "ns3/applications-module.h"

#include "ns3/mobility-module.h"

#include "ns3/csma-module.h"

#include "ns3/internet-module.h"

#include "ns3/yans-wifi-helper.h"

#include "ns3/ssid.h"

#include "ns3/netanim-module.h"
 
// Default Network Topology

//

//   Wifi 10.1.3.0

//                 AP

//  *    *    *    *

//  |    |    |    |    10.1.1.0

// n5   n6   n7   n0 -------------- n1   n2   n3   n4

//                   point-to-point  |    |    |    |

//                                   ================

//                                     LAN 10.1.2.0
 
using namespace ns3;
 
NS_LOG_COMPONENT_DEFINE ("ThirdScriptExample");
 
int 

main (int argc, char *argv[])

{​​​​​

  bool verbose = true;

  uint32_t nCsma = 3;

  uint32_t nWifi = 3;

  bool tracing = false;
 
  CommandLine cmd (__FILE__);

  cmd.AddValue ("nCsma", "Number of \"extra\" CSMA nodes/devices", nCsma);

  cmd.AddValue ("nWifi", "Number of wifi STA devices", nWifi);

  cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);

  cmd.AddValue ("tracing", "Enable pcap tracing", tracing);
 
  cmd.Parse (argc,argv);
 
  // The underlying restriction of 18 is due to the grid position

  // allocator's configuration; the grid layout will exceed the

  // bounding box if more than 18 nodes are provided.

  if (nWifi > 18)

    {​​​​​

      std::cout << "nWifi should be 18 or less; otherwise grid layout exceeds the bounding box" << std::endl;

      return 1;

    }​​​​​
 
  if (verbose)

    {​​​​​

      LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);

      LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

    }​​​​​
 
  NodeContainer p2pNodes;

  p2pNodes.Create (2);
 
  PointToPointHelper pointToPoint;

  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));

  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
 
  NetDeviceContainer p2pDevices;

  p2pDevices = pointToPoint.Install (p2pNodes);
 
  NodeContainer csmaNodes;

  csmaNodes.Add (p2pNodes.Get (1));

  csmaNodes.Create (nCsma);
 
  CsmaHelper csma;

  csma.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));

  csma.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (6560)));
 
  NetDeviceContainer csmaDevices;

  csmaDevices = csma.Install (csmaNodes);
 
  NodeContainer wifiStaNodes;

  wifiStaNodes.Create (nWifi);

  NodeContainer wifiApNode = p2pNodes.Get (0);
 
  YansWifiChannelHelper channel = YansWifiChannelHelper::Default();

  YansWifiPhyHelper phy ;

   phy.SetErrorRateModel ("ns3::NistErrorRateModel");

  phy.SetChannel (channel.Create ());
 
  WifiHelper wifi;

  wifi.SetRemoteStationManager ("ns3::AarfWifiManager");
 
  WifiMacHelper mac;

  Ssid ssid = Ssid ("ns-3-ssid");

  mac.SetType ("ns3::StaWifiMac",

               "Ssid", SsidValue (ssid),

               "ActiveProbing", BooleanValue (false));
 
  NetDeviceContainer staDevices;

  staDevices = wifi.Install (phy, mac, wifiStaNodes);
 
  mac.SetType ("ns3::ApWifiMac",

               "Ssid", SsidValue (ssid));
 
  NetDeviceContainer apDevices;

  apDevices = wifi.Install (phy, mac, wifiApNode);
 
  MobilityHelper mobility;
 
  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",

                                 "MinX", DoubleValue (0.0),

                                 "MinY", DoubleValue (0.0),

                                 "DeltaX", DoubleValue (5.0),

                                 "DeltaY", DoubleValue (10.0),

                                 "GridWidth", UintegerValue (3),

                                 "LayoutType", StringValue ("RowFirst"));
 
  mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",

                             "Bounds", RectangleValue (Rectangle (-50, 50, -50, 50)));

  mobility.Install (wifiStaNodes);

  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");

  mobility.Install (wifiApNode);
 
  InternetStackHelper stack;

  stack.Install (csmaNodes);

  stack.Install (wifiApNode);

  stack.Install (wifiStaNodes);
 
  Ipv4AddressHelper address;
 
  address.SetBase ("10.1.1.0", "255.255.255.0");

  Ipv4InterfaceContainer p2pInterfaces;

  p2pInterfaces = address.Assign (p2pDevices);
 
  address.SetBase ("10.1.2.0", "255.255.255.0");

  Ipv4InterfaceContainer csmaInterfaces;

  csmaInterfaces = address.Assign (csmaDevices);
 
  address.SetBase ("10.1.3.0", "255.255.255.0");

  address.Assign (staDevices);

  address.Assign (apDevices);
 
  UdpEchoServerHelper echoServer (9);
 
  ApplicationContainer serverApps = echoServer.Install (csmaNodes.Get (nCsma));

  serverApps.Start (Seconds (1.0));

  serverApps.Stop (Seconds (10.0));
 
  UdpEchoClientHelper echoClient (csmaInterfaces.GetAddress (nCsma), 9);

  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));

  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));

  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));
 
  ApplicationContainer clientApps = 

    echoClient.Install (wifiStaNodes.Get (nWifi - 1));

  clientApps.Start (Seconds (2.0));

  clientApps.Stop (Seconds (10.0));
 
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  AnimationInterface anim("wifi1.xml");
 
  Simulator::Stop (Seconds (10.0));
 
  if (tracing == true)

    {​​​​​

      pointToPoint.EnablePcapAll ("third");

      phy.EnablePcap ("third", apDevices.Get (0));

      csma.EnablePcap ("third", csmaDevices.Get (0), true);

    }​​​​​
 
  Simulator::Run ();

  Simulator::Destroy ();

  return 0;

}​​​​​