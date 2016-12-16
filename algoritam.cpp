#include "algoritam.h"
#include <iostream>
#include <vector>
#include <set>
#include <map>

//#define EPS1 0.0001
#define EPS2 0.000001
#define EPS3 0.00000001

double Algoritam::sweep = 0;
double Algoritam::dogX = 0;

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
        dogX = p->first.x;
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
    Duz d(P.x, sweep - 1, P.x, sweep + 1);
    auto r = status.lower_bound(d);
//    auto q = status.lower_bound(Duz(P.x - EPS1, sweep - 1, P.x - EPS1, sweep + 1));
//    auto p = status.upper_bound(Duz(P.x + EPS1, sweep - 1, P.x + EPS1, sweep + 1));
    auto q = r;
    auto p = r;
    poredjenjeDuzi comparatorManje;
    while(q != status.begin()){
        q--;
        if(comparatorManje(*q, d)){
            q++;
            break;
        }
    }

    while(p != status.end()){
        if(comparatorManje(d, *p))
            break;
        p++;
    }

    for(auto it = q; it != p; it++){
        if(it->B == P)
            L.push_back(*it);
        else
            C.push_back(*it);
    }

    status.erase(q,p);

//treba zakomentarisati!
//    for(auto it = status.begin(); it != status.end(); it++)
//         std::cout << it->A.x << " " << it->A.y << " " << it->B.x << " "<< it->B.y << std::endl;

    if(U.size() + C.size() + L.size() > 1){//prijavi presek
        preseci[P].insert(L.begin(), L.end());
        preseci[P].insert(C.begin(), C.end());
        preseci[P].insert(U.begin(), U.end());
    }

    status.insert(C.begin(), C.end());
    status.insert(U.begin(), U.end());

//zakomentarisati
//    std::cout << "posle ubacivanja: " << status.size() << std::endl;
//    for(auto it = status.begin(); it != status.end(); it++)
//         std::cout << it->A.x << " " << it->A.y << " " << it->B.x << " "<< it->B.y << std::endl;

    if(U.size() + C.size() == 0){
        auto lb = status.lower_bound(d);
        while(lb != status.begin()){
            lb--;
            if(comparatorManje(*lb, d)){
                lb++;
                break;
            }
        }
        auto it = lb;
        if(it != status.begin()){
            it--;
            pronadjiNoviDogadjaj(P, lb, it);
        }
    }
    else{
        auto lb = status.lower_bound(d);
        auto ub = lb;

        while(lb != status.begin()){
            lb--;
            if(comparatorManje(*lb, d)){
                lb++;
                break;
            }
        }
        while(ub != status.end()){
            if(comparatorManje(d, *ub))
                break;
            ub++;
        }

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
//        probudiCrtaca();
//        spavaj();
        if(intersection(*it1, *it2, &Q)){
            if(P < Q){//P je tacka preseka na kojoj je trenutno sweepline
                eventQueue[Q];
                detektovaniPreseci.insert(Q);
//                probudiCrtaca();
//                spavaj();
            }
        }
//        noviSusedi.clear();
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

        if(first.A.y == first.B.y){
            intersection(second, Duz(0, sweep, 1080, sweep), &P);
            if(dogX < P.x)//vodoravna manja
                return true;
            if(dogX == P.x)//vodoravna veca
                return false;
            return false;//vodoravna veca
        }
        if(second.A.y == second.B.y){
            intersection(first, Duz(0, sweep, 1080, sweep), &P);
            if(P.x < dogX)
                return true;//vodoravna veca
            if(P.x == dogX)
                return true; //vodoravna veca
            return false;// vodoravna manja
        }

        intersection(first, Duz(0, sweep, 1080, sweep), &P);
        intersection(second, Duz(0, sweep, 1080, sweep), &Q);
//EPS2
        if (P.x < Q.x - EPS3) return true;
        if (Q.x < P.x - EPS3) return false;

        intersection(first , Duz(0, sweep+EPS2, 1000, sweep+EPS2), &P);
        intersection(second, Duz(0, sweep+EPS2, 1000, sweep+EPS2), &Q);

        if (P.x < Q.x) return true;
        if (Q.x < P.x) return false;
        return false;
}

bool Algoritam::poredjenjeDuzi::operator()(const Duz &first, const Duz &second) const
{
    Point P, Q;

    if(first.A.y == first.B.y)
        return true;
    if(second.A.y == second.B.y)
        return false;

    intersection(first, Duz(0, sweep, 1080, sweep), &P);
    intersection(second, Duz(0, sweep, 1080, sweep), &Q);

    if (P.x < Q.x - EPS3)
        return true;
    //if (Q.x < P.x - EPS3) return false;

    return false;
}


bool pointComp::operator()(const Point &first, const Point &second) const
{
    if(first.y + EPS2 < second.y) return true;
    if(first.y > second.y + EPS2) return false;
    if(first.x + EPS2 < second.x) return true;
    return false;
}
