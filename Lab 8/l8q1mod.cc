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
#include "ns3/mobility-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/wifi-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("WirelessNetworkExample");

int main(int argc, char* argv[]) {
    bool verbose = true;
    uint32_t nWifi = 5; // Number of wifi STA devices
    bool tracing = false;

    CommandLine cmd(__FILE__);
    cmd.AddValue("nWifi", "Number of wifi STA devices", nWifi);
    cmd.AddValue("verbose", "Tell echo applications to log if true", verbose);
    cmd.AddValue("tracing", "Enable pcap tracing", tracing);
    cmd.Parse(argc, argv);

    if (verbose) {
        LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
        LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);
    }

    NodeContainer wifiStaNodes;
    wifiStaNodes.Create(nWifi);
    
    YansWifiChannelHelper channel = YansWifiChannelHelper::Default();
    YansWifiPhyHelper phy ;
    phy.SetErrorRateModel ("ns3::NistErrorRateModel");
    phy.SetChannel (channel.Create ());

    WifiHelper wifi;
    wifi.SetRemoteStationManager("ns3::AarfWifiManager");

    WifiMacHelper mac;
    Ssid ssid = Ssid("ns-3-ssid");
    mac.SetType("ns3::StaWifiMac", "Ssid", SsidValue(ssid), "ActiveProbing", BooleanValue(false));

    NetDeviceContainer staDevices;
    staDevices = wifi.Install(phy, mac, wifiStaNodes);

    // Set up the AP node
    NodeContainer wifiApNode;
    wifiApNode.Create(1);
    mac.SetType("ns3::ApWifiMac", "Ssid", SsidValue(ssid));

    NetDeviceContainer apDevices;
    apDevices = wifi.Install(phy, mac, wifiApNode);

    // Set up mobility models
    
    MobilityHelper mobility;
 
 mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
 "MinX", DoubleValue (4.0),
 "MinY", DoubleValue (4.0),
 "DeltaX", DoubleValue (20.0),
 "DeltaY", DoubleValue (40.0),
 "GridWidth", UintegerValue (2),
 "LayoutType", StringValue ("RowFirst"));
 
 mobility.SetMobilityModel("ns3::ConstantVelocityMobilityModel");
 mobility.Install(wifiStaNodes);
 
 Ptr<ConstantVelocityMobilityModel> mob1 = wifiStaNodes.Get(0)->GetObject<ConstantVelocityMobilityModel>();
 mob1->SetVelocity(Vector(10,44,0));
 
 Ptr<ConstantVelocityMobilityModel> mob2 = wifiStaNodes.Get(1)->GetObject<ConstantVelocityMobilityModel>();
 mob2->SetVelocity(Vector(10,44,0));
 
 Ptr<ConstantVelocityMobilityModel> mob3 = wifiStaNodes.Get(2)->GetObject<ConstantVelocityMobilityModel>();
 mob3->SetVelocity(Vector(12,44,0));

 Ptr<ConstantVelocityMobilityModel> mob4 = wifiStaNodes.Get(3)->GetObject<ConstantVelocityMobilityModel>();
 mob4->SetVelocity(Vector(11,40,0));
 
 mobility.SetMobilityModel ("ns3::ConstantVelocityMobilityModel");
 mobility.Install (wifiApNode);

 Ptr<ConstantVelocityMobilityModel> mob = wifiStaNodes.Get(4)->GetObject<ConstantVelocityMobilityModel>();
 mob->SetVelocity(Vector(10,40,0));
 
    // Install Internet stack
    InternetStackHelper stack;
    stack.Install(wifiApNode);
    stack.Install(wifiStaNodes);

    // Assign IP addresses
    Ipv4AddressHelper address;
    address.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer staInterfaces;
    staInterfaces = address.Assign(staDevices);
    address.Assign(apDevices);

    // Set up echo server
    UdpEchoServerHelper echoServer(9);
    ApplicationContainer serverApps = echoServer.Install(wifiApNode.Get(0));
    serverApps.Start(Seconds(1.0));
    serverApps.Stop(Seconds(10.0));

    // Set up echo client
    UdpEchoClientHelper echoClient(staInterfaces.GetAddress(nWifi - 1), 9);
    echoClient.SetAttribute("MaxPackets", UintegerValue(1));
    echoClient.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient.SetAttribute("PacketSize", UintegerValue(1024));

    ApplicationContainer clientApps = echoClient.Install(wifiStaNodes.Get(nWifi - 1));
    clientApps.Start(Seconds(2.0));
    clientApps.Stop(Seconds(10.0));

    // Enable tracing if requested
    if (tracing) {
        phy.EnablePcapAll("l8q1mod");
    }

    // Run simulation
    Simulator::Stop(Seconds(10.0));
    AnimationInterface anim("l8q1mod.xml");
    Simulator::Run();
    Simulator::Destroy();

    return 0;
}

