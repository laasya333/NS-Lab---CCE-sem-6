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
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/netanim-module.h"

// Default Network Topology (star)
//                      
//                 n4
//           n3    /   n5
//      n2   |    /   /
//        \  |   /   /
//         \ |  /   /
//n1----------n0----------n6
//        /  / |\  \
//       n7 /  | \  n11
//         /   |  \ 
//        n8   |   n10
//             n9

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("threenodeScriptExample");

int 
main ()
{
  NodeContainer p2pNodes;
  p2pNodes.Create (12);
  
  NodeContainer n0n1 = NodeContainer (p2pNodes.Get (0), p2pNodes.Get(1));
  NodeContainer n0n2 = NodeContainer (p2pNodes.Get (0), p2pNodes.Get(2));
  NodeContainer n0n3 = NodeContainer (p2pNodes.Get (0), p2pNodes.Get(3));
  NodeContainer n0n4 = NodeContainer (p2pNodes.Get (0), p2pNodes.Get(4));
  NodeContainer n0n5 = NodeContainer (p2pNodes.Get (0), p2pNodes.Get(5));
  NodeContainer n0n6 = NodeContainer (p2pNodes.Get (0), p2pNodes.Get(6));
  NodeContainer n0n7 = NodeContainer (p2pNodes.Get (0), p2pNodes.Get(7));
  NodeContainer n0n8 = NodeContainer (p2pNodes.Get (0), p2pNodes.Get(8));
  NodeContainer n0n9 = NodeContainer (p2pNodes.Get (0), p2pNodes.Get(9));
  NodeContainer n0n10 = NodeContainer (p2pNodes.Get (0), p2pNodes.Get(10));
  NodeContainer n0n11 = NodeContainer (p2pNodes.Get (0), p2pNodes.Get(11));
 
  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
  NetDeviceContainer d0d1 = pointToPoint.Install (n0n1);
  NetDeviceContainer d0d2 = pointToPoint.Install (n0n2);
  NetDeviceContainer d0d3 = pointToPoint.Install (n0n3);
  NetDeviceContainer d0d4 = pointToPoint.Install (n0n4);
  NetDeviceContainer d0d5 = pointToPoint.Install (n0n5);
  NetDeviceContainer d0d6 = pointToPoint.Install (n0n6);
  NetDeviceContainer d0d7 = pointToPoint.Install (n0n7);
  NetDeviceContainer d0d8 = pointToPoint.Install (n0n8);
  NetDeviceContainer d0d9 = pointToPoint.Install (n0n9);
  NetDeviceContainer d0d10 = pointToPoint.Install (n0n10);
  NetDeviceContainer d0d11 = pointToPoint.Install (n0n11);
  
  InternetStackHelper stack;
  stack.Install (p2pNodes);
  // Later, we add IP addresses.
  NS_LOG_INFO ("Assign IP Addresses.");
  Ipv4AddressHelper ipv4;
  //i0i1
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i0i1 = ipv4.Assign (d0d1);
  //i0i2 
  ipv4.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer i0i2 = ipv4.Assign (d0d2);
  //i0i3
  ipv4.SetBase ("10.1.3.0", "255.255.255.0");
  Ipv4InterfaceContainer i0i3 = ipv4.Assign (d0d3);
  //i0i4 
  ipv4.SetBase ("10.1.4.0", "255.255.255.0");
  Ipv4InterfaceContainer i0i4 = ipv4.Assign (d0d4);
  //i0i5
  ipv4.SetBase ("10.1.5.0", "255.255.255.0");
  Ipv4InterfaceContainer i0i5 = ipv4.Assign (d0d5);
  //i0i6 
  ipv4.SetBase ("10.1.6.0", "255.255.255.0");
  Ipv4InterfaceContainer i0i6 = ipv4.Assign (d0d6);
  //i0i7
  ipv4.SetBase ("10.1.7.0", "255.255.255.0");
  Ipv4InterfaceContainer i0i7 = ipv4.Assign (d0d7);
  //i0i8 
  ipv4.SetBase ("10.1.8.0", "255.255.255.0");
  Ipv4InterfaceContainer i0i8 = ipv4.Assign (d0d8);
  //i0i9
  ipv4.SetBase ("10.1.9.0", "255.255.255.0");
  Ipv4InterfaceContainer i0i9 = ipv4.Assign (d0d9);
  //i0i10 
  ipv4.SetBase ("10.1.10.0", "255.255.255.0");
  Ipv4InterfaceContainer i0i10 = ipv4.Assign (d0d10); 
  //i0i11
  ipv4.SetBase ("10.1.11.0", "255.255.255.0");
  Ipv4InterfaceContainer i0i11 = ipv4.Assign (d0d11);
  
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  UdpEchoServerHelper echoServer (9);
  //node 0 is the server
  ApplicationContainer serverApps = echoServer.Install (p2pNodes.Get (0));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  UdpEchoClientHelper echoClient (i0i1.GetAddress (1), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (6));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps = echoClient.Install (p2pNodes.Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));

  AnimationInterface anim ("L2q2.xml");
  
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
