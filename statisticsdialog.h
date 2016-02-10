#ifndef STATISTICSDIALOG_H
#define STATISTICSDIALOG_H

#include <QDialog>
#include <string>
using namespace std;

namespace Ui {
class statisticsDialog;
}

class statisticsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit statisticsDialog(QWidget *parent = 0);
    ~statisticsDialog();
    void addItem(string domainName, int n, float perc);

private slots:
    void on_pushButton_clicked();

private:
    Ui::statisticsDialog *ui;
};

#endif // STATISTICSDIALOG_H
