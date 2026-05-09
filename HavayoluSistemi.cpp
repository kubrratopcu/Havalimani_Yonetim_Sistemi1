#include "HavayoluSistemi.h"

HavayoluSistemi::HavayoluSistemi() {
    yolcuAgaciKoku = nullptr;
}

HavayoluSistemi::~HavayoluSistemi() {
    agaciTemizle(yolcuAgaciKoku);

    for (auto& cift : seferSistemi) {
        seferListesiniTemizle(cift.second.yolcuListesiBasi);
    }
}

void HavayoluSistemi::agaciTemizle(YolcuNode* kok) {
    if (kok == nullptr) return;
    agaciTemizle(kok->sol);
    agaciTemizle(kok->sag);
    delete kok;
}

void HavayoluSistemi::seferListesiniTemizle(PnrNode*& bas) {
    PnrNode* akim = bas;
    while (akim != nullptr) {
        PnrNode* sonraki = akim->next;
        delete akim;
        akim = sonraki;
    }
    bas = nullptr;
}