#ifndef COMMON
#define COMMON

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
#include <PlatformSpecificUtils.h>
#ifdef WIN32
#include <winsock2.h>
#else
#include <in.h>
#endif

#include <cstring>
#include <vector>


#endif // COMMON

