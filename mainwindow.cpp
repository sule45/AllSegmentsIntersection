#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ucitajDuz()
{
    // PROVERITI DA LI SU UNESENA SVA 4 BROJA!
    double a = ui->lineEdit->text().toDouble();
    double b = ui->lineEdit_2->text().toDouble();
    double c = ui->lineEdit_3->text().toDouble();
    double d = ui->lineEdit_4->text().toDouble();

    if(a <= 0 || b <= 0 || c <= 0 || d <= 0){
        ui->labelaUneteDuzi->setText("Sve koordinate moraju biti pozitivne!");
        return;
    }

    bool t = ui->widget->ucitajDuz(a,b,c,d);
    ui->labelaUneteDuzi->setText(t ? "Duz je uspesno uneta." : "Ova duz je ranije unesena.");
    ui->widget->update();
}

void MainWindow::odrediPreseke()
{

}

void MainWindow::on_pushButton_3_clicked() //ocisti duzi
{
    ui->widget->ocistiSve();
    ui->labelaUneteDuzi->setText("Duzi su ukonjene");
    ui->widget->update();
}
