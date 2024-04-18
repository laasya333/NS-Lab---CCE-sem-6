BEGIN {
    packet_count = 0;
}

{
    action = $1;
    time = $2;
    event = $3;
    protocol = $8;
    src_ip = $13;
    dst_ip = $14;
    src_port = $15;
    dst_port = $16;
    seq_num = $18;
    ack_num = $19;
    flags = $20;
    payload_info = $21;

    if (event == "ns3::TcpHeader" || event == "ns3::UdpHeader") {
        packet_count++;

        printf("Packet %d:\n", packet_count);
        printf("  Time: %s\n", time);
        printf("  Action: %s\n", action);
        printf("  Protocol: %s\n", protocol);
        printf("  Source IP: %s\n", src_ip);
        printf("  Destination IP: %s\n", dst_ip);
        printf("  Source Port: %s\n", src_port);
        printf("  Destination Port: %s\n", dst_port);
        if (event == "ns3::TcpHeader") {
            printf("  Sequence Number: %s\n", seq_num);
            printf("  Acknowledgement Number: %s\n", ack_num);
            printf("  Flags: %s\n", flags);
        }
        printf("  Payload Information: %s\n", payload_info);
        printf("\n");
    }
}

END {
    print "Total number of packets processed:", packet_count;
}
