#ifndef OBLASTCRTANJA_H
#define OBLASTCRTANJA_H

#include "algoritam.h"
#include <QWidget>
#include <QPainter>
#include <set>

struct Point;
struct Duz;
class Algoritam;
struct pointComp;

class OblastCrtanja : public QWidget
{
    Q_OBJECT
public:
    explicit OblastCrtanja(QWidget *parent = 0);

    bool ucitajDuz(double a, double b, double c, double d);
    void ocistiSve();
    void iscrtajSliku(QPainter& painter);
    void pokerniAlgoritam(bool daLiJeAlgoritamPokrenut); // poziva se iz mainWin kad se trazi sledeci korak

private:
    void podesiOlovke();
    void probudiAlgoritam();
    void spavaj();

public slots:
    void krajAlgoritma();
    void iscrtaj1();

signals:
    void sigKrajAlgoritma(std::map<Point, std::set<Duz>, pointComp> preseci);

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

private:
    std::set<Duz> duzi;
    QPen pen_A, pen_B, pen_presek, pen_aktivnaDuz, pen_susednaDuz, pen_duz, pen_sweep;
    QPen pen_detektovani;

    Algoritam* alg;
};

struct Point {
    Point(){}
    Point(double x, double y):x(x),y(y){}
    double x, y;

    bool operator == (const Point& other) const {
        return x == other.x && y == other.y;
    }

    bool operator< (const Point& other) const;
};

struct Duz {
    Duz(Point a, Point b):A(a),B(b){}
    Duz(double a, double b, double c, double d);
    Point A, B;
    bool operator < (const Duz& other) const;
};

#endif // OBLASTCRTANJA_H
