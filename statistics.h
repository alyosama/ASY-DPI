#ifndef STATISTICS_H
#define STATISTICS_H
#include<map>
#include <Packet.h>
#include <RawPacket.h>
#include <IPv4Layer.h>
#include <iostream>
#include "statisticsdialog.h"

using namespace std;
class statistics
{
public:
    statistics();
    map<string,string> ipConverter;
    map<string,int> map2;
    int total=0;
    statisticsDialog *statDialog;

    void check(RawPacket * r);
    int mycount(string x);
    void openStatisticsDialog();


    void print();

};

#endif // STATISTICS_H
