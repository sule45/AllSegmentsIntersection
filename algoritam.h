#ifndef ALGORITAM_H
#define ALGORITAM_H

#include <QThread>
#include <set>
#include <map>
#include "oblastcrtanja.h"
#include <QMutex>
#include <QWaitCondition>

struct Point;
struct Duz;

class Algoritam : public QThread
{
    Q_OBJECT
public:
    explicit Algoritam(std::set<Duz>& duzi, bool threadMode = true, QObject *parent = 0);

    struct duzStatusComp {
        bool operator() (const Duz& first, const Duz& second) const;
    };

    struct poredjenjeDuzi {
        bool operator() (const Duz& first, const Duz& second) const;
    };

private:
    void dodajDogadjaj(); //fali implementacija
    void probudiCrtaca();
    void spavaj();
    void run();
    void obradiDogadjaj(const Point& P, std::set<Duz>& U);
    void pronadjiNoviDogadjaj(const Point& P,
                              const std::set<Duz, duzStatusComp>::iterator& it1,
                              const std::set<Duz, duzStatusComp>::iterator& it2);


signals:
    void kraj();

private:
    /*tacka je dogadjaj, a uz to su joj pridruzene duzi koje su sa njom
     ako neka tacka ima vise od jedne pridruzene duzi, to znaci da je ona
     presek.*/
    std::map<Point, std::set<Duz> > eventQueue;
    std::set<Duz, duzStatusComp> status;
    std::map<Point, std::set<Duz> > preseci;
    std::set<Point> detektovaniPreseci;
    std::set<Duz> noviSusedi;

public:
    static double sweep;
    static double dogX;
friend class OblastCrtanja;
friend class duzStatusComp;
private:
    QMutex mut;
    QWaitCondition keyPressed;
    /* threadMode -> ako je true, onda se spavaj() izvrsava, i
                     i budi nas drugi proces
                  -> ako je false, onda f-ja spavaj ne radi nista,
                     i algoritam se moze koristiti bez prekidanja.*/
    bool threadMode;
};

#endif // ALGORITAM_H
