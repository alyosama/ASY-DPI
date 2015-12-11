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

private slots:
    void on_actionStart_Capture_triggered();
    void on_actionSave_triggered();
    void on_actionOpen_triggered();
    void on_actionSettings_triggered();

private:
    Ui::MainWindow *ui;
    SettingsDialog *settingsDialog;
    QStringList m_TableHeader;

};

#endif // MAINWINDOW_H
