#include "oblastcrtanja.h"
#include <iostream>
#include <algorithm>
#define EPS 0//0.001  //0.00001

OblastCrtanja::OblastCrtanja(QWidget *parent) : QWidget(parent)
{
    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, Qt::white);

    setUpdatesEnabled(true);

    setAutoFillBackground(true);
    setPalette(Pal);

    alg = NULL;
    podesiOlovke();
}

void OblastCrtanja::pokerniAlgoritam(bool daLiJeAlgoritamPokrenut)
{
    if(!daLiJeAlgoritamPokrenut){
        alg = new Algoritam(duzi);
        connect(alg, SIGNAL(kraj()), this, SLOT(krajAlgoritma()));
        connect(alg, SIGNAL(crtaj()), this, SLOT(iscrtaj1()));
        connect(alg, SIGNAL(finished()), alg, SLOT(deleteLater()));
        alg->start();
    }
    else{
        probudiAlgoritam();
        return;
        //spavaj();
        //update();
       // repaint();
       // std::cout << "crtam" << std::endl;
    }
}

bool OblastCrtanja::ucitajDuz(double a, double b, double c, double d)
{
    auto p = duzi.insert(Duz(a,b,c,d));
    return p.second;
    //update();
    //repaint();
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

        if(alg && alg->preseci.find(it->A) != alg->preseci.end())
            painter.setPen(pen_presek);
        else if(alg && alg->detektovaniPreseci.find(it->A) != alg->detektovaniPreseci.end())
            painter.setPen(pen_detektovani);
        else
            painter.setPen(pen_A);
        painter.drawPoint(it->A.x,it->A.y);

        if(alg && alg->preseci.find(it->B) != alg->preseci.end())
            painter.setPen(pen_presek);
        else if(alg && alg->detektovaniPreseci.find(it->B) != alg->detektovaniPreseci.end())
            painter.setPen(pen_detektovani);
        else painter.setPen(pen_B);
        painter.drawPoint(it->B.x,it->B.y);

        if(alg && alg->noviSusedi.find(*it) != alg->noviSusedi.end())
            painter.setPen(pen_susednaDuz);
        else if (alg && alg->status.find(*it) != alg->status.end())
            painter.setPen(pen_aktivnaDuz);
        else
            painter.setPen(pen_duz);
        painter.drawLine(it->A.x, it->A.y, it->B.x, it->B.y);
    }
    if(alg){
        for(auto it = alg->preseci.begin(); it != alg->preseci.end(); it++){
            painter.setPen(pen_presek);
            painter.drawPoint(it->first.x, it->first.y);
        }
        for(auto it = alg->detektovaniPreseci.begin(); it != alg->detektovaniPreseci.end(); ++it){
            painter.setPen(pen_detektovani);
            painter.drawPoint(it->x, it->y);
        }
    }
    painter.setPen(pen_sweep);
    painter.drawLine(0, alg->sweep, this->height(), alg->sweep);
}

void OblastCrtanja::podesiOlovke()
{
    pen_A.setWidth(8);
    pen_A.setCapStyle(Qt::RoundCap);
    pen_B.setWidth(8);
    pen_B.setCapStyle(Qt::RoundCap);
    pen_presek.setWidth(10);
    pen_presek.setCapStyle(Qt::RoundCap);
    pen_detektovani.setWidth(9);
    pen_detektovani.setCapStyle(Qt::RoundCap);
    pen_aktivnaDuz.setWidth(2);
    pen_susednaDuz.setWidth(2);
    pen_duz.setWidthF(1.5);
    pen_sweep.setWidth(1);

    pen_A.setColor(Qt::green);
    pen_B.setColor(Qt::blue);
    pen_presek.setColor(Qt::darkMagenta);
    pen_detektovani.setColor(Qt::darkYellow);
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
    //std::cout << "kraj oblastcrtanja" << std::endl;
    //duzi.clear();
    alg->sweep = 0;
    emit sigKrajAlgoritma(alg->preseci);
    delete alg;
    alg = NULL;
    // paintEvent(NULL);
}

void OblastCrtanja::iscrtaj1()
{
    update();
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
    if(y + EPS < other.y) return true;
    if(y > other.y + EPS) return false;
    if(x + EPS < other.x) return true;
    return false;
}


