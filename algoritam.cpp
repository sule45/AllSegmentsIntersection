#include "algoritam.h"
#include <iostream>

Algoritam::Algoritam(std::set<Duz>& duzi, bool threadMode, QObject *parent) :
    QThread(parent), threadMode(threadMode)
{
    for(auto it = duzi.begin(); it != duzi.end(); it++){
        eventQueue[it->A].insert(*it);
        eventQueue[it->B];
    }
}

void Algoritam::run(){
    std::cout << "pocetno spavanje" << std::endl;
    spavaj();

    while (!eventQueue.empty()){
        auto p = eventQueue.begin();
        eventQueue.erase(p);
        obradiDogadjaj(p->first, p->second);
    }

    std::cout << "kraj, budim spavaca posledji put" << std::endl;
    probudiCrtaca();
    msleep(50); // ovo nije bas tacno, ali nije ni strasno
    emit kraj();
}

void Algoritam::obradiDogadjaj(Point P, std::set<Duz>& U){
    std::cout << "algoritam odradjuje posao " << std::endl;
    probudiCrtaca();
    std::cout << "uspavljujem algoritam " << std::endl;
    spavaj();
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

bool Algoritam::duzStatusComp::operator()(const Duz &first, const Duz &second) const {
    return first < second;
}
