#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QDebug>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    devices=PcapLiveDeviceList::getInstance().getPcapLiveDevicesList();

    if(devices.size())
        selectedDevice=devices[0];
    else
        selectedDevice=NULL;

    captureTime=10;
    ui->setupUi(this);
    SetUpListDevices();

}
SettingsDialog::~SettingsDialog()
{
    delete ui;
}

PcapLiveDevice* SettingsDialog::getSelectedDevice(){
    return selectedDevice;
}

int SettingsDialog::getCaptureTime(){
    return captureTime;
}

void SettingsDialog::SetUpListDevices()
{

    for(int i=0;i<devices.size();i++){
        //qDebug()<<QString::fromUtf8(devices[i]->getName());
        QString label=QString::fromUtf8(devices[i]->getName());
        ui->listWidget->addItem(label);
    }

}



void SettingsDialog::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    for(int i=0;i<devices.size();i++){
        if(QString::fromUtf8(devices[i]->getName())==item->text()){
            selectedDevice=devices[i];
            break;
        }
    }
    this->hide();
}
