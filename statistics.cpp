#include "statistics.h"

statistics::statistics()
{

    ipConverter["213.158.163.117"]="Google";
     ipConverter["93.184.220.127"]="soundcloud";
     ipConverter["96.17.211.172"]="cambridge";
     map2["Google"]=0;
     map2["soundcloud"]=0;
     map2["cambridge"]=0;
}

void statistics::check(RawPacket * r){

     Packet packet(r);
     if (packet.isPacketOfType(IPv4))
         {
             IPv4Layer* ipv4layer= packet.getLayerOfType<IPv4Layer>();

             total++;
             if( ipConverter.find(ipv4layer->getDstIpAddress().toString()) != ipConverter.end())
             {
                 this->mycount(ipConverter[ipv4layer->getDstIpAddress().toString()]);
             }

}
}


int statistics::mycount(string y)
{
    return ++map2[y];
}

void statistics::print()
{
    map<string, int>::iterator it = map2.begin();
    map<string, int>::iterator it1 = map2.end();

    for(;it!=it1;it++)
        cout<< it->first << " "<< it->second <<endl;

    cout<<"total: "<< total<<endl;
}

void statistics::openStatisticsDialog()
{
    statDialog = new statisticsDialog();
    map<string, int>::iterator it = map2.begin();
    map<string, int>::iterator it1 = map2.end();

    for(;it!=it1;it++)
        statDialog->addItem(it->first, it->second, (1.0*it->second/total) *100);
    statDialog->show();
}

