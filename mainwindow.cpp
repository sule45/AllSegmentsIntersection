#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>

#include <cstdlib>
#include <ctime>

#define EPS 0 //0.0005

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->widget, SIGNAL(sigKrajAlgoritma(std::map<Point,std::set<Duz>,pointComp>)), this,
                          SLOT(krajAlgoritma(   std::map<Point,std::set<Duz>,pointComp>))       );
    daLiJeAlgoritamPokrenut = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ucitajDuz()
{
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
    if(!daLiJeAlgoritamPokrenut){
        daLiJeAlgoritamPokrenut = true;
        ui->pushButton_3->setDisabled(true);
        ui->pushButton_2->setText("sledeci korak");
        ui->widget->pokerniAlgoritam(false);
    }
    else{
        ui->widget->pokerniAlgoritam(true);
        ui->widget->update();
        ui->gridLayout->update();
        ui->centralWidget->update();
    }
//    ui->widget->update();
//    QCoreApplication::processEvents();

}

void MainWindow::on_pushButton_3_clicked() //ocisti duzi
{
    ui->textBrowser->setHtml("");
    ui->widget->ocistiSve();
    ui->labelaUneteDuzi->setText("Duzi su ukonjene");
    ui->widget->update();
}

void MainWindow::on_pushButton_4_clicked()
{
    ui->widget->ocistiSve();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"));
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, "Greska", "Ne moze da se otvori datoteka", QMessageBox::Ok);
    }
    else
    {
        QTextStream t(&file);
        double a, b, c, d;

        while(true){
            t >> a >> b >> c >> d;
            if(a == 0 && b == 0 && c == 0 && d == 0)
                break;
           /* if(b == d){
                if(a < c){
                    d += EPS;
                    b -= EPS;
                }
                else{
                    b += EPS;
                    d -= EPS;
                }
            }*/
            ui->widget->ucitajDuz(a,b,c,d);
            //std::cout << a << b << c << d << std::endl;
        };

        file.close();
    }
}

void MainWindow::krajAlgoritma(std::map<Point, std::set<Duz>, pointComp> v)
{
    QString html;
    for(auto it = v.begin(); it != v.end(); it++){
        html += QString("(%1, %2)<p align=\"right\">").arg(QString::number(it->first.x), QString::number(it->first.y));
        for(auto it2 = it->second.begin(); it2 != it->second.end(); it2++){
            html += QString("(%1, %2)->(%3, %4)<br>").arg(QString::number(it2->A.x), QString::number(it2->A.y),
                                                            QString::number(it2->B.x), QString::number(it2->B.y));
        }
        html += "</p>";
    }

    ui->textBrowser->setHtml(html);

    //std::cout << "kraj MainWindow" << std::endl;
    daLiJeAlgoritamPokrenut = false;
    ui->pushButton_3->setDisabled(false);
    ui->pushButton_2->setText("Pokreni algoritam");
}

void MainWindow::on_pushButton_5_clicked() // GENERISI
{
    int n = ui->lineEdit_5->text().toInt();
    if(n == 0)
        n = 10;
    srand (time(NULL));
    for(int i = 0; i < n; i++){
        int a = rand() % ui->widget->width();
        int b = rand() % ui->widget->height();
        int c = rand() % ui->widget->width();
        int d = rand() % ui->widget->height();
        if(a == c && b == d){
            i--;
            continue;
        }
        ui->widget->ucitajDuz(a,b,c,d);
        ui->widget->update();
        ui->centralWidget->update();
        ui->gridLayout->update();
    }
}
