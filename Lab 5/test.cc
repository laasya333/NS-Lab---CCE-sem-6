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
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"
#include "ns3/ipv4-global-routing-helper.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("TEST");

int main (int argc, char *argv[]) {
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

  NodeContainer nodes;
  nodes.Create (5);

  PointToPointHelper pointToPoint1, pointToPoint2, pointToPoint3, pointToPoint4, pointToPoint5, pointToPoint6;
  pointToPoint1.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint1.SetChannelAttribute ("Delay", StringValue ("10ms"));
  pointToPoint2.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint2.SetChannelAttribute ("Delay", StringValue ("7ms"));
  pointToPoint3.SetDeviceAttribute ("DataRate", StringValue ("2Mbps"));
  pointToPoint3.SetChannelAttribute ("Delay", StringValue ("15ms"));
  pointToPoint4.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
  pointToPoint4.SetChannelAttribute ("Delay", StringValue ("20ms"));
  pointToPoint5.SetDeviceAttribute ("DataRate", StringValue ("15Mbps"));
  pointToPoint5.SetChannelAttribute ("Delay", StringValue ("10ms"));
  pointToPoint6.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
  pointToPoint6.SetChannelAttribute ("Delay", StringValue ("1ms"));          

  NetDeviceContainer p2p1, p2p2, p2p3, p2p4, p2p5, p2p6;
  p2p1 = pointToPoint1.Install (nodes.Get (0), nodes.Get (1));
  p2p2 = pointToPoint2.Install (nodes.Get (1), nodes.Get (2));
  p2p3 = pointToPoint3.Install (nodes.Get (0), nodes.Get (2));
  p2p4 = pointToPoint4.Install (nodes.Get (2), nodes.Get (3));
  p2p5 = pointToPoint5.Install (nodes.Get (1), nodes.Get (3));
  p2p6 = pointToPoint6.Install (nodes.Get (3), nodes.Get (4));

  InternetStackHelper stack;
  stack.Install (nodes);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i1 = address.Assign (p2p1);
  address.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer i2 = address.Assign (p2p2);    
  address.SetBase ("10.1.3.0", "255.255.255.0");
  Ipv4InterfaceContainer i3 = address.Assign (p2p3);
  address.SetBase ("10.1.4.0", "255.255.255.0");
  Ipv4InterfaceContainer i4 = address.Assign (p2p4);
  address.SetBase ("10.1.5.0", "255.255.255.0");
  Ipv4InterfaceContainer i5 = address.Assign (p2p5);
  address.SetBase ("10.1.6.0", "255.255.255.0");
  Ipv4InterfaceContainer i6 = address.Assign (p2p6);      
  
  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install (nodes.Get (4));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  UdpEchoClientHelper echoClient (i6.GetAddress (1), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  AnimationInterface anim ("test.xml");

  anim.SetConstantPosition(nodes.Get (0), 20, 20, 0);
  anim.SetConstantPosition(nodes.Get (1), 30, 10, 0);
  anim.SetConstantPosition(nodes.Get (2), 30, 30, 0);
  anim.SetConstantPosition(nodes.Get (3), 40, 20, 0);
  anim.SetConstantPosition(nodes.Get (4), 50, 20, 0);
  
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
