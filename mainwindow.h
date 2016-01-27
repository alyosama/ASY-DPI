#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "settingsdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    friend void packetRecieved(RawPacket* rawPacket, PcapLiveDevice* pDevice, void* userCookie);
    static void addPacketToTable(RawPacket *RawPacket,Ui::MainWindow *ui);
private slots:
    void on_actionStart_Capture_triggered();
    void on_actionSave_triggered();
    void on_actionOpen_triggered();
    void on_actionSettings_triggered();
    void on_actionStop_Capture_triggered();

private:
    Ui::MainWindow *ui;
    SettingsDialog *settingsDialog;
    QStringList m_TableHeader;
    void readPackets(QString filename);
    void savePackets(PcapLiveDevice *pDevice,QString filename,int time);
    void readPacketsFromFile(QString filename,RawPacketVector& packets, char* errorString);
    void savePacketsToFile(const char* fileName,RawPacketVector& packets, char* errString);


};

#endif // MAINWINDOW_H
