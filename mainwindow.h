#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "testmapperdevice.h"
#include "mapperdbthread.h"
#include "mapper/mapper_cpp.h"

#define MAX_SIGS 5

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private Q_SLOTS:

    void on_pushButtonStart_clicked();

    void on_pushButtonStop_clicked();

    void on_pushButtonCreate_clicked();

    void on_pushButtonScanDB_clicked();

    void on_pushButtonDestroy_clicked();

    void on_pushButtonAddDev_clicked();

    void on_comboBoxSrc_currentIndexChanged(const QString &arg1);

    void on_comboBoxDst_currentIndexChanged(const QString &arg1);

    void on_listWidgetSrc_currentRowChanged(int currentRow);

    void on_listWidgetDst_currentRowChanged(int currentRow);

    void on_pushButtonMap_clicked();

private:
    void createDevs();
    void stopDevs();
    void freeDevs();
    void createMaps();

    void checkMapListSelection();

    Ui::MainWindow *ui;
    std::vector<testmapperdevice*> testDevices;

    mapperdbthread *mapperDb;


};

#endif // MAINWINDOW_H
