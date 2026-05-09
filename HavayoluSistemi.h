#pragma once

#include <iostream>
#include <map>
#include <queue>
#include <vector>
#include <set>
#include <unordered_map>
#include <stack>
#include <string>
#include "Modeller.h"

using namespace std;

class HavayoluSistemi {
private:
    map<string, Sefer> seferSistemi;
    priority_queue<Ucak> kule;
    map<string, vector<pair<string, int>>> graf;
    unordered_map<string, Yolcu> harita;
    YolcuNode* yolcuAgaciKoku;

public:
    HavayoluSistemi();
    ~HavayoluSistemi();

    void agaciTemizle(YolcuNode* kok);
    void seferListesiniTemizle(PnrNode*& bas);
};