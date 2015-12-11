#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H
#include <QDialog>
#include <PcapLiveDeviceList.h>
#include <PcapLiveDevice.h>
#include <QListWidgetItem>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:

    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();
    PcapLiveDevice* getSelectedDevice();
    int getCaptureTime();

private slots:


    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::SettingsDialog *ui;
    void SetUpListDevices();
    std::vector<PcapLiveDevice*> devices;
    PcapLiveDevice* selectedDevice;
    int captureTime;

};

#endif // SETTINGSDIALOG_H
