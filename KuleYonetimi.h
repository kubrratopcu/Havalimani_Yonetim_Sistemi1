#ifndef KULEYONETIMI_H
#define KULEYONETIMI_H

#include <queue>
#include "Modeller.h"

class KuleYonetimi {
public:
    // KuleYonetimi.h içine public kısmına ekle:
    std::priority_queue<Ucak>& getKuyruk() { return kuyruk; }
    void ekle(Ucak u) { kuyruk.push(u); }
    void pop() { if(!kuyruk.empty()) kuyruk.pop(); }
    Ucak enOncelikliyiGetir() { return kuyruk.top(); }
    bool bosMu() { return kuyruk.empty(); }

private:
    std::priority_queue<Ucak> kuyruk;
};

#endif