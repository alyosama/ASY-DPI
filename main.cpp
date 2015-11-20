#include <PcapLiveDeviceList.h>
#include <PcapLiveDevice.h>
#include <RawPacket.h>
#include <Packet.h>
#include <IpAddress.h>
#include <EthLayer.h>
#include <VlanLayer.h>
#include <IPv4Layer.h>
#include <IPv6Layer.h>
#include <ArpLayer.h>
#include <UdpLayer.h>
#include <TcpLayer.h>
#include <UdpLayer.h>
#include <HttpLayer.h>
#include <PPPoELayer.h>
#include <DnsLayer.h>
#include <MplsLayer.h>
#include <PayloadLayer.h>
#include <Logger.h>
#include <debug_new.h>
#ifdef WIN32
#include <winsock2.h>
#else
#include <in.h>
#endif



#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <cstring>

using namespace std;


int count=0;

void packetRecieved(RawPacket* rawPacket, PcapLiveDevice* pDevice,void* userCookie)
{
    printf("packet received %d\n",count++);

    ////parsing
    Packet packet(rawPacket);
    // Print packet types
    printf("Packet is of type: ");
    if (packet.isPacketOfType(Ethernet))
    {
        printf("Ethernet ");
        EthLayer* ethlayer= packet.getLayerOfType<EthLayer>();
        printf("Src MAC: %s\n", ethlayer->getSourceMac().toString().c_str());
        printf("Dst MAC: %s\n", ethlayer->getDestMac().toString().c_str());
    }

    if (packet.isPacketOfType(IP))
    {
        if (packet.isPacketOfType(IPv4))
        {
            printf("IPv4 ");
            IPv4Layer* ipv4layer= packet.getLayerOfType<IPv4Layer>();
            printf("Dst IP: %s\n",ipv4layer->getDstIpAddress().toString().c_str());
            printf("Src IP: %s\n",ipv4layer->getSrcIpAddress().toString().c_str());
        }
        else if (packet.isPacketOfType(IPv6))
        {
            IPv6Layer* ipv6layer= packet.getLayerOfType<IPv6Layer>();
            printf("Dst IP: %s\n",ipv6layer->getDstIpAddress().toString().c_str());
            printf("Src IP: %s\n",ipv6layer->getSrcIpAddress().toString().c_str());
        }
    }
    if (packet.isPacketOfType(TCP))
    {
        printf("TCP ");
        TcpLayer* tcpLayer = packet.getLayerOfType<TcpLayer>();
        printf("Port Dst: %d\n", ntohs(tcpLayer->getTcpHeader()->portDst));
        printf("Port Src: %d\n", ntohs(tcpLayer->getTcpHeader()->portSrc));
    }
    if (packet.isPacketOfType(UDP))
    {
        printf("UDP ");
        UdpLayer* udpLayer = packet.getLayerOfType<UdpLayer>();
        printf("Port Dst: %d\n", ntohs(udpLayer->getUdpHeader()->portDst));
        printf("Port Src: %d\n", ntohs(udpLayer->getUdpHeader()->portSrc));

    }
    printf("\n");


}
int main(int argc, char* argv[])
{

    PcapLiveDevice* pIfaceDevice = PcapLiveDeviceList::getInstance().getPcapLiveDeviceByName("wlan0"); //change to any interface later
    //Verifying interface is valid
    if (pIfaceDevice == NULL)
    {
        printf("Cannot find interface. Exiting...\n");
        exit(-1);
    }
    //Opening interface device
    if (!pIfaceDevice->open())
    {
        printf("Cannot open interface. Exiting...\n");
        exit(-1);
    }

    pIfaceDevice->startCapture(packetRecieved, NULL);
    while(true){}
    pIfaceDevice->close();

}
