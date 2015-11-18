#include <stdlib.h>
#include <fstream>
#include <RawPacket.h>
#include <Packet.h>
#include <IpAddress.h>
#include <EthLayer.h>
#include <IPv6Layer.h>
#include <UdpLayer.h>

#include <Logger.h>
#include <Packet.h>
#include <EthLayer.h>
#include <VlanLayer.h>
#include <PayloadLayer.h>
#include <IPv4Layer.h>
#include <IPv6Layer.h>
#include <ArpLayer.h>
#include <UdpLayer.h>
#include <TcpLayer.h>
#include <HttpLayer.h>
#include <PPPoELayer.h>
#include <DnsLayer.h>
#include <MplsLayer.h>
#include <IpAddress.h>
#include <fstream>
#include <stdlib.h>
#include <debug_new.h>
#include <iostream>
#include <sstream>
#include <string.h>
#ifdef WIN32
#include <winsock2.h>
#else
#include <in.h>
#endif


#include <PcapLiveDeviceList.h>
#include <PcapLiveDevice.h>


using namespace std;


int count=0; void packetRecieved(RawPacket* rawPacket, PcapLiveDevice* pDevice,
                                 void* userCookie)
{
    printf("packet received %d\n",count);


    ////parsing
    Packet packet(rawPacket);
    // Print packet types
    printf("Packet is of type: ");
    if (packet.isPacketOfType(Ethernet))
    { printf("Ethernet ");
        printf("Src MAC: %s\n", ((EthLayer*)packet.getFirstLayer())->getSourceMac().toString().c_str());}

    if (packet.isPacketOfType(IP))
    { printf("IP ");
    }
    if (packet.isPacketOfType(IPv4))
    {printf("IPv4 ");

        printf("Dst IP: %s\n", packet.getLayerOfType<IPv4Layer>()->getDstIpAddress().toString().c_str());}
    if (packet.isPacketOfType(IPv6))
    {printf("IPv6 ");
        printf("Dst IP: %s\n", packet.getLayerOfType<IPv6Layer>()->getDstIpAddress().toString().c_str());}
    if (packet.isPacketOfType(TCP))
    {printf("TCP ");
    }
    if (packet.isPacketOfType(UDP))
    { printf("UDP ");
        UdpLayer* udpLayer = packet.getLayerOfType<UdpLayer>();
        printf("Port Dst: %d\n", ntohs(udpLayer->getUdpHeader()->portDst));}
    printf("\n");


    /*
// Access L2 fields
    printf("Src MAC: %s\n", ((EthLayer*)packet.getFirstLayer())->getSourceMac().toString().c_str());

    // Access L3 fields
    printf("Dst IP: %s\n", packet.getLayerOfType<IPv6Layer>()->getDstIpAddress().toString().c_str());

    // Access L4 fields
    UdpLayer* udpLayer = packet.getLayerOfType<UdpLayer>();
    printf("Port Dst: %d\n", ntohs(udpLayer->getUdpHeader()->portDst));

    // Access packet properties
    printf("Packet length: %d\n", packet.getRawPacket()->getRawDataLen());

    Layer* payloadLayer=udpLayer->getNextLayer();

    printf("Packet payload offset: %s\n", payloadLayer->toString().c_str()); */


    count++;
}
int main(int argc, char* argv[])
{


    printf("hello ankdjljd \n");
    PcapLiveDevice* pIfaceDevice = PcapLiveDeviceList::getInstance().getPcapLiveDeviceByName("wlan0"); //change to any interface later


    //Verifying interface is valid
    if (pIfaceDevice == NULL)
    {
        printf("Cannot find interface. Exiting...\n");
        exit(-1);
    }

    //Opening interface device
    int c=0;
    if (!pIfaceDevice->open())
    {
        printf("Cannot open interface. Exiting...\n");
        exit(-1);
    }
    pIfaceDevice->startCapture(packetRecieved, NULL);

    while(true){};
    pIfaceDevice->close();



}
