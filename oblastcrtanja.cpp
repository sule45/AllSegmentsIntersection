#include "oblastcrtanja.h"
#include <iostream>

OblastCrtanja::OblastCrtanja(QWidget *parent) : QWidget(parent)
{
    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, Qt::white);

    setAutoFillBackground(true);
    setPalette(Pal);

    podesiOlovke();
}

bool OblastCrtanja::pokerniAlgoritam(bool daLiJeAlgoritamPokrenut)
{
    if(!daLiJeAlgoritamPokrenut){
        alg = new Algoritam(duzi);
        connect(alg, SIGNAL(kraj()), this, SLOT(krajAlgoritma()));
        connect(alg, SIGNAL(finished()), alg, SLOT(deleteLater()));
        alg->start();
    }
    else{
        probudiAlgoritam();
        spavaj();
        std::cout << "crtam" << std::endl;
    }
    //privremeno resenje
    return false;
}

bool OblastCrtanja::ucitajDuz(double a, double b, double c, double d)
{
    auto p = duzi.insert(Duz(a,b,c,d));
    return p.second;
}

void OblastCrtanja::ocistiSve()
{
    // TREBA JOS STOSTA POCISTITI, ALI MOZDA JE BOLJE TE OSTALE
    // STRUKTURE POCISTITI POSLE ALGORITMA, JER SU ONE IPAK DEO
    // ALGORITMA, A duzi SU DEO GUI-ja
    duzi.clear();
}

void OblastCrtanja::iscrtajSliku(QPainter &painter)
{
    for(auto it = duzi.begin(); it != duzi.end(); it++){
        painter.setPen(pen_A);
        painter.drawPoint(it->A.x,it->A.y);
        painter.setPen(pen_B);
        painter.drawPoint(it->B.x,it->B.y);
        painter.setPen(pen_duz);
        painter.drawLine(it->A.x, it->A.y, it->B.x, it->B.y);
    }
}

void OblastCrtanja::podesiOlovke()
{
    pen_A.setWidth(8);
    pen_A.setCapStyle(Qt::RoundCap);
    pen_B.setWidth(8);
    pen_B.setCapStyle(Qt::RoundCap);
    pen_presek.setWidth(10);
    pen_presek.setCapStyle(Qt::RoundCap);
    pen_aktivnaDuz.setWidth(2);
    pen_susednaDuz.setWidth(2);
    pen_duz.setWidthF(1.5);
    pen_sweep.setWidth(1);

    pen_A.setColor(Qt::green);
    pen_B.setColor(Qt::blue);
    pen_presek.setColor(Qt::red);
    pen_aktivnaDuz.setColor(Qt::red);
    pen_susednaDuz.setColor(Qt::green);
    pen_duz.setColor(Qt::black);
    pen_sweep.setColor(Qt::red);

    pen_sweep.setStyle(Qt::DashLine);

}

void OblastCrtanja::probudiAlgoritam()
{
    alg->mut.lock();
    alg->keyPressed.wakeAll();
    alg->mut.unlock();
}

void OblastCrtanja::spavaj()
{
    alg->mut.lock();
    alg->keyPressed.wait(&alg->mut);
    alg->mut.unlock();
}

void OblastCrtanja::krajAlgoritma()
{
    std::cout << "kraj oblastcrtanja" << std::endl;
    //duzi.clear();
    delete alg;
    emit sigKrajAlgoritma();
   // paintEvent(NULL);
}

void OblastCrtanja::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    iscrtajSliku(painter);
}

Duz::Duz(double a, double b, double c, double d)
{
    A.x = a; A.y = b;
    B.x = c; B.y = d;
    if(B < A){
        Point T = A;
        A = B;
        B = T;
    }
}

bool Duz::operator <(const Duz &other) const {
    if(A < other.A) return true;
    if(other.A < A) return false;
    if(B < other.B) return true;
    return false;
}

bool Point::operator <(const Point &other) const {
    if(y < other.y) return true;
    if(y > other.y) return false;
    if(x < other.x) return true;
    return false;
}
