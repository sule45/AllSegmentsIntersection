#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "oblastcrtanja.h"

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

public slots:
    void krajAlgoritma();

private:
    Ui::MainWindow *ui;
    bool daLiJeAlgoritamPokrenut;
};

#endif // MAINWINDOW_H
