#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "materialxml.h"
#include "materialcontainer.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // enable/disable the different search options
    ui->srcContainer->setEnabled(ui->srcCheckBox->isChecked());
    ui->dstContainer->setEnabled(ui->dstCheckBox->isChecked());
    ui->flagsContainer->setEnabled(ui->flagsCheckBox->isChecked());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open Xml", ".", "Xml files (*.xml)");
    MaterialXml::materialDataFromXML(fileName);

    for (int i = 0; i < materialList.length(); i++) {
        Material material = materialList.at(i);

        qInfo() <<  material.srcFactor;
    }

}

void MainWindow::on_flagsCheckBox_clicked()
{
    bool isChecked = ui->flagsCheckBox->isChecked();
    ui->flagsContainer->setEnabled(isChecked);
}

void MainWindow::on_dstCheckBox_clicked()
{
    bool isChecked = ui->dstCheckBox->isChecked();
    ui->dstContainer->setEnabled(isChecked);
}

void MainWindow::on_srcCheckBox_clicked()
{
    bool isChecked = ui->srcCheckBox->isChecked();
    ui->srcContainer->setEnabled(isChecked);
}

void MainWindow::on_searchPushButton_clicked()
{

}

void MainWindow::on_actionOpen_Folder_triggered()
{
    QString directory = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home",
                                                         QFileDialog::ShowDirsOnly
                                                         | QFileDialog::DontResolveSymlinks);
}
