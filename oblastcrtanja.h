#ifndef OBLASTCRTANJA_H
#define OBLASTCRTANJA_H

#include <QWidget>
#include <QPainter>
#include <set>

struct Point {
    Point(){}
    Point(double x, double y):x(x),y(y){}
    double x, y;

    bool operator < (const Point& other) const;
};

struct Duz {
    Duz(Point a, Point b):A(a),B(b){}
    Duz(double a, double b, double c, double d);
    Point A, B;

    bool operator < (const Duz& other) const;
};

class OblastCrtanja : public QWidget
{
    Q_OBJECT
public:
    explicit OblastCrtanja(QWidget *parent = 0);

    bool ucitajDuz(double a, double b, double c, double d);
    void ocistiSve();
    void iscrtajSliku(QPainter& painter);

signals:

public slots:


protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

private:
    std::set<Duz> duzi;
    QPen pen_A, pen_B, pen_presek, pen_aktivnaDuz, pen_susednaDuz, pen_duz, pen_sweep;
};





#endif // OBLASTCRTANJA_H
