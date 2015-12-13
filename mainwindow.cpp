#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QHeaderView>

#include <QFileDialog>
#include <RawPacket.h>
#include <Layer.h>
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
#include <Logger.h>
#include <debug_new.h>
#include <PlatformSpecificUtils.h>
#include <PcapFileDevice.h>
#include <cstdlib>
#ifdef WIN32
#include <winsock2.h>
#else
#include <in.h>
#endif
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  settingsDialog=NULL;

  ui->tableWidget->setColumnCount(8);
  m_TableHeader<<"#"<<"Type"<<"Mac Source"<<"Mac Dist"<<"IP Source"<<"IP Dist"<<"TCP/UDP Source"<<"TCP/UDP Dist";
  ui->tableWidget->setHorizontalHeaderLabels(m_TableHeader);
  ui->tableWidget->verticalHeader()->setVisible(false);
  ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
  ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
  ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
  ui->tableWidget->setStyleSheet("QTableView {selection-background-color: red;}");
}

MainWindow::~MainWindow() {
    delete ui;
    if(settingsDialog)
        delete settingsDialog;
}
int count=0;
void printPacket(RawPacket* rawPacket){

    Packet packet(rawPacket);
    QString type;
    // Print packet types
    printf("Packet is of type: ");
    if (packet.isPacketOfType(Ethernet))
    {
        //type<<"Ethernet";
        EthLayer* ethlayer= packet.getLayerOfType<EthLayer>();
        printf("Src MAC: %s\n", ethlayer->getSourceMac().toString().c_str());
        printf("Dst MAC: %s\n", ethlayer->getDestMac().toString().c_str());
    }

    if (packet.isPacketOfType(IP))
    {
        if (packet.isPacketOfType(IPv4))
        {
            //type<<"- IPv4 ";
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
        //type<<"- TCP ";
        TcpLayer* tcpLayer = packet.getLayerOfType<TcpLayer>();
        printf("Port Dst: %d\n", ntohs(tcpLayer->getTcpHeader()->portDst));
        printf("Port Src: %d\n", ntohs(tcpLayer->getTcpHeader()->portSrc));
    }
    if (packet.isPacketOfType(UDP))
    {
        //type<<"- UDP ";
        UdpLayer* udpLayer = packet.getLayerOfType<UdpLayer>();
        printf("Port Dst: %d\n", ntohs(udpLayer->getUdpHeader()->portDst));
        printf("Port Src: %d\n", ntohs(udpLayer->getUdpHeader()->portSrc));

    }
    printf("\n");
}


//friend function, we will use userCookie in as the MainWindow object
void packetRecieved(RawPacket* rawPacket, PcapLiveDevice* pDevice,void* userCookie)
{
    printf("packet received %d\n",++count);
    printPacket(rawPacket);
    MainWindow* win = (MainWindow*)userCookie;
    int rowc = win->ui->tableWidget->rowCount();
    win->ui->tableWidget->insertRow(rowc - 1);
    QTableWidgetItem* qtwi;
//    qtwi->setText("blabla1"); //this line shuts the window down after the first packet
//    win->ui->tableWidget->setItem(rowc,1,qtwi);

    // TODO: add the new packet data to the table or call some member function to do it.
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

void readPacketsFromFile(QString filename,RawPacketVector& packets, char* errorString){

    PcapFileReaderDevice readerDevice(filename.toStdString().c_str());
    if (!readerDevice.open())
    {
        printf("Error opening the device: %s", errorString);
        return;
    }
    RawPacket rawPacket;
    while (readerDevice.getNextPacket(rawPacket))
    {
        packets.pushBack(new RawPacket(rawPacket));
    }
    readerDevice.close();

}

void readPackets(QString filename){

    printf("Start Reading Packets From %s\n",filename.toStdString().c_str());
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

void savePackets(PcapLiveDevice *pDevice,QString filename,int time){
    printf("Start Saving Packets at %s\n",filename.toStdString().c_str());
    char errorString[1000];
    LoggerPP::getInstance().setErrorString(errorString, 1000);
    RawPacketVector capturedPackets;
    pDevice->startCapture(capturedPackets);
    PCAP_SLEEP(time);
    pDevice->stopCapture();
    savePacketsToFile(filename.toStdString().c_str(),capturedPackets,errorString);
    printf("End Saving %d Packets\n",capturedPackets.size());
}

void MainWindow::on_actionStart_Capture_triggered()
{
    QMessageBox Msgbox;
    PcapLiveDevice* pIfaceDevice=settingsDialog->getSelectedDevice();
    if(!pIfaceDevice){
         Msgbox.setText("Cannot find interface. \n"
                        "Please Select Device...");
         Msgbox.exec();
         return;
    }
    printf("\ndevice name: %s \n",pIfaceDevice->getName());
    //Opening interface device
    if (!pIfaceDevice->open())
    {
         Msgbox.setText("Cannot open interface. Exiting...\n");
         Msgbox.exec();
         return;
    }

    int time=settingsDialog->getCaptureTime();
    //user cookie (this) is passed to the packetRecieved as an object *void, read startcapture code documentation for more info
    pIfaceDevice->startCapture(packetRecieved, this);
}

void MainWindow::on_actionSave_triggered()
{
    QMessageBox Msgbox;
    PcapLiveDevice* pIfaceDevice=settingsDialog->getSelectedDevice();
    if(!pIfaceDevice){
         Msgbox.setText("Cannot find interface. \n"
                        "Please Select Device...");
         Msgbox.exec();
         return;
    }

    //Opening interface device
    if (!pIfaceDevice->open())
    {
         Msgbox.setText("Cannot open interface. Exiting...\n");
         Msgbox.exec();
         return;
    }
    int time=settingsDialog->getCaptureTime();
    pIfaceDevice->stopCapture();

    QString filename = QFileDialog::getSaveFileName(this, "Save file", "", ".pcap");
    if (!filename.isEmpty())
        savePackets(pIfaceDevice,filename,time);
    count=0;
}

void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File..."),
                                                 QString(), tr("Raw Packets (*.pcap);;All Files (*)"));
    if (!filename.isEmpty())
     readPackets(filename);
}

void MainWindow::on_actionSettings_triggered()
{
    settingsDialog = new SettingsDialog();
    settingsDialog->show();

}

void MainWindow::on_actionStop_Capture_triggered()
{
    QMessageBox Msgbox;
    PcapLiveDevice* pIfaceDevice=settingsDialog->getSelectedDevice();
    if(!pIfaceDevice){
         Msgbox.setText("Cannot find interface. \n"
                        "Please Select Device...");
         Msgbox.exec();
         return;
    }
    pIfaceDevice->stopCapture();
    count=0;
}
