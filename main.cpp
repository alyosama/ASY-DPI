#include <PcapLiveDeviceList.h>
#include <PcapLiveDevice.h>
#include <PcapFileDevice.h>
#include <PlatformSpecificUtils.h>
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
#include <vector>

using namespace std;

void printPacket(RawPacket* rawPacket){

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


int count=0;
void packetRecieved(RawPacket* rawPacket, PcapLiveDevice* pDevice,void* userCookie)
{
    printf("packet received %d\n",++count);
    printPacket(rawPacket);


}
void savePacketsToFile(const char* fileName,RawPacketVector& packets, char* errString)
{
    PcapFileWriterDevice writerDevice(fileName);
    if (!writerDevice.open())
    {
        printf("Error opening writer device for %s: %s", fileName, errString);
        return;
    }

    for (RawPacketVector::VectorIterator packetIter = packets.begin(); packetIter != packets.end(); packetIter++)
    {
        writerDevice.writePacket(*(*packetIter));
    }

    writerDevice.close();
}

void readPacketsFromFile(string filename,RawPacketVector& packets, char* errorString){

    PcapFileReaderDevice readerDevice(filename.c_str());
    if (!readerDevice.open())
    {
        printf("Error opening the device: %s", errorString);
        return;
    }
    RawPacket rawPacket;
    while (readerDevice.getNextPacket(rawPacket))
    {
        packets.pushBack(&rawPacket);
    }
    readerDevice.close();

}

void readPackets(string filename){

    printf("Start Reading Packets From %s\n",filename.c_str());
    char errorString[1000];
    LoggerPP::getInstance().setErrorString(errorString, 1000);
    RawPacketVector packets;
    readPacketsFromFile(filename,packets,errorString);

    printf("Number of Packets found: %d\n", packets.size());
    int i = 0;
    for (RawPacketVector::VectorIterator packetIter = packets.begin(); packetIter != packets.end(); packetIter++)
    {
        printf("packet read %d\n",++i);
        printPacket(*packetIter);
    }


}

void savePackets(PcapLiveDevice *pDevice,string filename,int time){
    printf("Start Saving Packets at %s\n",filename.c_str());
    char errorString[1000];
    LoggerPP::getInstance().setErrorString(errorString, 1000);
    RawPacketVector capturedPackets;
    pDevice->startCapture(capturedPackets);
    PCAP_SLEEP(time);
    pDevice->stopCapture();
    savePacketsToFile(filename.c_str(),capturedPackets,errorString);
    printf("End Saving %d Packets\n",capturedPackets.size());
}

void livePackets(PcapLiveDevice *pDevice,int time){
    pDevice->startCapture(packetRecieved, NULL);
    PCAP_SLEEP(time);
    pDevice->stopCapture();
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


    // Options
    char mode='R';
    int time=10;
    string filename="p.pcap";

    switch(mode){
    case 'L':
        livePackets(pIfaceDevice,time);
        break;
    case 'S':
        savePackets(pIfaceDevice,filename,time);
        break;
    case 'R':
        readPackets(filename);
    default:
        break;
    }

    pIfaceDevice->close();

}
