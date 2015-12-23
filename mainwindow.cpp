#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButtonMap->setEnabled(false);
    ui->pushButtonUnmap->setEnabled(false);
    mapperDb = new mapperdbthread();
    mapperDb->start();

}

MainWindow::~MainWindow()
{
    delete ui;

    if (mapperDb != NULL) {
        mapperDb->stopThread();
        mapperDb->wait();
    }

    stopDevs();
    freeDevs();

}

void MainWindow::createDevs()
{
    stopDevs();
    int numDevs = ui->spinBoxDevs->value();
    for (int i=0 ;i<numDevs; i++)
    {
        QString dev_name = "mydevice_"+QString::number(i);
        int numIns = 1+3*(float)qrand()/RAND_MAX;
        int numOuts = 1+3*(float)qrand()/RAND_MAX;
        qDebug() <<"creating " << dev_name <<" with" << numIns<<"inputs and"<< numOuts<< "outputs";
        testDevices.push_back(new testmapperdevice(numIns,numOuts,dev_name));
    }

}

void MainWindow::on_pushButtonStart_clicked()
{
    for (int i=0; i<testDevices.size(); i++)
    {
        if (!testDevices.at(i)->isRunning()) {
            qDebug() <<"starting thread";
            testDevices.at(i)->start();
            QThread::sleep(1);
        }
    }
}

void MainWindow::on_pushButtonStop_clicked()
{
    stopDevs();
}

void MainWindow::stopDevs()
{
    for (int i=0; i<testDevices.size(); i++)
    {
        testDevices.at(i)->stopRunning();
        testDevices.at(i)->wait();
    }
}

void MainWindow::freeDevs()
{
    while (testDevices.size())
    {
        delete testDevices.at(testDevices.size()-1);
        testDevices.pop_back();
    }
}

void MainWindow::on_pushButtonCreate_clicked()
{
    stopDevs();
    createDevs();
}


void MainWindow::createMaps()
{

}

void MainWindow::on_pushButtonScanDB_clicked()
{
    QString res = "scanning db...\n\n";
    ui->plainTextEditOutput->setPlainText(res);

    ui->comboBoxDst->clear();
    ui->comboBoxSrc->clear();


    ui->listWidgetSrc->clear();
    ui->listWidgetDst->clear();

    for (auto devname : mapperDb->getDeviceList())
    {
        res+=devname;
        res+="\n";
        ui->comboBoxSrc->addItem(devname);
        ui->comboBoxDst->addItem(devname);
    }

//    for (;qdev != qdev.end(); qdev++)
//    {
//      res+= (*qdev).name().c_str();
//      res+="\n";
//    }

    ui->plainTextEditOutput->setPlainText(res);
    ui->plainTextEditOutput->setReadOnly(true);
}

void MainWindow::on_pushButtonDestroy_clicked()
{

}

void MainWindow::on_pushButtonAddDev_clicked()
{
    int numDevs = testDevices.size();
    QString dev_name = "mydevice_"+QString::number(numDevs);
    int numIns = ui->spinBoxIns->value();
    int numOuts = ui->spinBoxOuts->value();
    qDebug() <<"creating " << dev_name <<" with" << numIns<<"inputs and"<< numOuts<< "outputs";
    testmapperdevice * testdev = new testmapperdevice(numIns,numOuts,dev_name);
    testdev->start();
    testDevices.push_back(testdev);
}

void MainWindow::on_comboBoxSrc_currentIndexChanged(const QString &arg1)
{

    qDebug()<<"Selected " << arg1;
    if (arg1 == "")
        return; //hackety hack

    ui->listWidgetSrc->clear();
    std::vector<QString> list = mapperDb->getSigList(arg1, MAPPER_DIR_OUTGOING);
    for (const auto signame : list)
    {
        qDebug() <<"device: " << arg1 <<" out signal: " << signame;
        ui->listWidgetSrc->addItem(signame);
    }
   // ui->listWidgetSrc->setCurrentRow(-1);
    checkMapListSelection();

}


void MainWindow::on_comboBoxDst_currentIndexChanged(const QString &arg1)
{
    qDebug()<<"Selected " << arg1;
    if (arg1 == "")
        return; //hackety hack
    ui->listWidgetDst->clear();
    std::vector<QString> list = mapperDb->getSigList(arg1, MAPPER_DIR_INCOMING);
    for (const auto signame : list)
    {
        qDebug() <<"device: " << arg1 <<" in signal: " << signame;
        ui->listWidgetDst->addItem(signame);
    }
    //ui->listWidgetDst->setCurrentRow(-1);
    checkMapListSelection();
}


void MainWindow::on_listWidgetSrc_currentRowChanged(int currentRow)
{
    checkMapListSelection();

}

void MainWindow::checkMapListSelection()
{
    if (ui->listWidgetSrc->currentRow()>=0 && ui->listWidgetDst->currentRow()>=0)
    {
        ui->pushButtonMap->setEnabled(true);
    }
    else
    {
        ui->pushButtonMap->setEnabled(false);
    }
}

void MainWindow::on_listWidgetDst_currentRowChanged(int currentRow)
{
    checkMapListSelection();
}

void MainWindow::on_pushButtonMap_clicked()
{
    QString src_dev = ui->comboBoxSrc->currentText();
    QString dst_dev = ui->comboBoxDst->currentText();
    QString src_sig = ui->listWidgetSrc->currentItem()->text();
    QString dst_sig = ui->listWidgetDst->currentItem()->text();

    //todo: multiple sources... obviously not possible with such a menu scheme.

    qDebug() << "Mapping from" << src_dev<<":"<<src_sig <<
                "to" << dst_dev<<":"<<dst_sig;

    mapperDb->makeMap(src_dev, dst_dev, src_sig, dst_sig);
}
