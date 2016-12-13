#include "algoritam.h"
#include <iostream>
#include <vector>

#define EPS1 0.00001
#define EPS2 0.0000001
#define EPS3 0.000001

double Algoritam::sweep = 0;

Algoritam::Algoritam(std::set<Duz>& duzi, bool threadMode, QObject *parent) :
    QThread(parent), threadMode(threadMode)
{
    for(auto it = duzi.begin(); it != duzi.end(); it++){
        eventQueue[it->A].insert(*it);
        eventQueue[it->B];
    }
}

void Algoritam::run(){
    //std::cout << "pocetno spavanje" << std::endl;
    spavaj();

    while (!eventQueue.empty()){
        //std::cout << "eventQueue.size() = " << eventQueue.size() << std::endl;
        noviSusedi.clear();
        auto p = eventQueue.begin();
        eventQueue.erase(p);
        sweep = p->first.y;
        obradiDogadjaj(p->first, p->second);

        //std::cout << "algoritam odradjuje posao " << std::endl;
        probudiCrtaca();
        //std::cout << "uspavljujem algoritam " << std::endl;
        spavaj();
    }

    std::cout << "kraj, budim spavaca posledji put" << std::endl;
    probudiCrtaca();
    msleep(50); // ovo nije bas pravilno, ali nije ni strasno
    emit kraj();
}

void Algoritam::obradiDogadjaj(const Point& P, std::set<Duz>& U){

    std::vector<Duz> C,L;

    detektovaniPreseci.erase(P);

    auto q = status.lower_bound(Duz(P.x - EPS1, sweep - 1, P.x - EPS1, sweep + 1));
    auto p = status.upper_bound(Duz(P.x + EPS1, sweep - 1, P.x + EPS1, sweep + 1));

    for(auto it = q; it != p; it++){
        if(it->B == P){
            L.push_back(*it);
            //std::cout << it->B.x << " " << it->B.y << " " << P.x << " "<< P.y << std::endl;
        }
        else{
            C.push_back(*it);
            //std::cout << it->B.x << " " << it->B.y << " " << P.x << " "<< P.y << std::endl;
        }
    }

    for(auto it = status.begin(); it!=status.end(); it++)
         std::cout << it->A.x << " " << it->A.y << " " << it->B.x << " "<< it->B.y << std::endl;
//    std::cout << "pre brisanja: " << status.size() << std::endl;
    status.erase(q,p);
//    std::cout << "posle brisanja: " << status.size() << std::endl;
    if(U.size() + C.size() + L.size() > 1){//prijavi presek
        preseci[P].insert(preseci[P].end(), L.begin(), L.end());
        preseci[P].insert(preseci[P].end(), C.begin(), C.end());
        preseci[P].insert(preseci[P].end(), U.begin(), U.end());
    }

    status.insert(C.begin(), C.end());
    status.insert(U.begin(), U.end());
    std::cout << "posle ubacivanja: " << status.size() << std::endl;
    for(auto it = status.begin(); it!=status.end(); it++)
         std::cout << it->A.x << " " << it->A.y << " " << it->B.x << " "<< it->B.y << std::endl;



    if(U.size() + C.size() == 0){
        auto lb = status.lower_bound(Duz(P.x - EPS1, sweep - 1, P.x - EPS1, sweep + 1));
        auto it = lb;
        if(it != status.begin()){
            it--;
            pronadjiNoviDogadjaj(P, lb, it);
        }
    }
    else{
        auto lb = status.lower_bound(Duz(P.x - EPS1, sweep - 1, P.x - EPS1, sweep + 1));
        auto ub = status.upper_bound(Duz(P.x + EPS1, sweep - 1, P.x + EPS1, sweep + 1));
        auto it1 = lb;
        if(it1 != status.begin()){
            it1--;
            pronadjiNoviDogadjaj(P, it1, lb);
        }
        auto it2 = ub;
        if(it2 != status.begin()){
            it2--;
            pronadjiNoviDogadjaj(P, it2, ub);
        }

    }
}
bool intersection(Duz a, Duz b, Point* P);
void Algoritam::pronadjiNoviDogadjaj(const Point& P,
                                     const std::set<Duz, duzStatusComp>::iterator& it1,
                                     const std::set<Duz, duzStatusComp>::iterator& it2)
{
    if(it2 != status.begin() && it2 != status.end()){
        noviSusedi.insert(*it1);
        noviSusedi.insert(*it2);
        Point Q;
        if(intersection(*it1, *it2, &Q)){
            if(P < Q){//P je tacka preseka na kojoj je trenutno sweepline
                eventQueue[Q];
                detektovaniPreseci.insert(Q);
            }
        }
    }
}

void Algoritam::probudiCrtaca(){
    if(threadMode){
        mut.lock();
        keyPressed.wakeAll();
        mut.unlock();
    }
}

void Algoritam::spavaj(){

    if(threadMode){
        mut.lock();
        keyPressed.wait(&mut);
        mut.unlock();
    }
}

bool get_line_intersection(double p0_x, double p0_y, double p1_x, double p1_y,
    double p2_x, double p2_y, double p3_x, double p3_y, double *i_x, double *i_y)
{
    double s1_x, s1_y, s2_x, s2_y;
    s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
    s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

    double s, t;
    s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
    t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
    {
        // Collision detected
        if (i_x != NULL)
            *i_x = p0_x + (t * s1_x);
        if (i_y != NULL)
            *i_y = p0_y + (t * s1_y);
        return true;
    }

    return false; // No collision
}

bool intersection(Duz a, Duz b, Point* P){
    return get_line_intersection(a.A.x, a.A.y, a.B.x, a.B.y, b.A.x, b.A.y, b.B.x, b.B.y, &P->x, &P->y);
}

bool Algoritam::duzStatusComp::operator()(const Duz &first, const Duz &second) const {
        Point P, Q;

        intersection(first, Duz(0, sweep, 1000, sweep), &P);
        intersection(second, Duz(0, sweep, 1000, sweep), &Q);

        if(first.A.x == 10 && second.A.x == 600){
            std::cout << "sa" << P.x << " " << P.y << " " << Q.x << " " << Q.y << std::endl;
        }


        if (P.x < Q.x - EPS3) return true;
        if (Q.x < P.x - EPS3) return false;

        intersection(first , Duz(0, sweep+EPS2, 1000, sweep+EPS2), &P);
        intersection(second, Duz(0, sweep+EPS2, 1000, sweep+EPS2), &Q);

        if(first.A.x == 10 && second.A.x == 600){
            std::cout << "sa"  << P.x << " " << P.y << " " << Q.x << " " << Q.y << std::endl;
        }

        if (P.x < Q.x) return true;
        if (Q.x < P.x) return false;
        return false;
}
