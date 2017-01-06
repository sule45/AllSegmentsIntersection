#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "oblastcrtanja.h"

class pointComp;

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
    void ucitajDuz();
    void odrediPreseke();


    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_actionOpis_porgrama_triggered();

public slots:
    void krajAlgoritma(std::map<Point, std::set<Duz>, pointComp > v);

private:
    Ui::MainWindow *ui;
    bool daLiJeAlgoritamPokrenut;
};

#endif // MAINWINDOW_H
