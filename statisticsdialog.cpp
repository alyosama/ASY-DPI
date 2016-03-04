#include "statisticsdialog.h"
#include "ui_statisticsdialog.h"

statisticsDialog::statisticsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::statisticsDialog)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(3);
    QStringList m_TableHeader;
    m_TableHeader<<"Domain Name"<<"Number of Packets"<<"%";
    ui->tableWidget->setHorizontalHeaderLabels(m_TableHeader);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
}

statisticsDialog::~statisticsDialog()
{
    delete ui;
}

void statisticsDialog::addItem(string domainName, int n, float perc)
{
    int rowc = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(rowc);
    ui->tableWidget->setItem(rowc,0,new QTableWidgetItem(QString::fromStdString(domainName)));
    ui->tableWidget->setItem(rowc,1,new QTableWidgetItem(QString::number(n)));
    ui->tableWidget->setItem(rowc,2,new QTableWidgetItem(QString::number(perc)));
}

void statisticsDialog::on_pushButton_clicked()
{
    this->~statisticsDialog();
}
