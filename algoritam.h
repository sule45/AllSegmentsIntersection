#ifndef ALGORITAM_H
#define ALGORITAM_H

#include <QThread>
#include <set>
#include <map>
#include "oblastcrtanja.h"

struct Event {
    Event(Point E, Duz d1, Duz d2);

    Point E;
};

class Algoritam : public QThread
{
    Q_OBJECT
public:
    explicit Algoritam(QObject *parent = 0);

signals:

public slots:

protected:
    void run();

friend class OblastCrtanja;

private:
    std::map<Point, std::set<Duz> > eventQueue;
    // tacka je dogadjaj, a uz to su joj pridruzene duzi koje su sa njom
    // ako neka tacka ima vise od jedne pridruzene duzi, to znaci da je ona
    // presek.
};

#endif // ALGORITAM_H
