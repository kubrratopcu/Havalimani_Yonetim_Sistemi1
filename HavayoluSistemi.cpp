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

void HavayoluSistemi::seferleriYukle() {
    ifstream dosya("seferler.txt");
    string satir;
    if (!dosya.is_open()) return;

    while (getline(dosya, satir)) {
        stringstream ss(satir);
        // DEĞİŞKENLERİ BURADA TANIMLIYORUZ (Hata almamak için şart!)
        string sNo, uId, kalkis, varis, zaman, pnrList;

        getline(ss, sNo, ',');
        getline(ss, uId, ',');
        getline(ss, kalkis, ',');
        getline(ss, varis, ',');
        getline(ss, zaman, ',');
        getline(ss, pnrList, ',');

        Sefer s;
        s.seferNo = sNo;
        s.ucakId = stoi(uId);
        s.ucusZamani = stoll(zaman);

        stringstream ssPnr(pnrList);
        string tPnr;
        while (getline(ssPnr, tPnr, ';')) {
            if (!tPnr.empty()) s.yolcuPnrListesi.push_back(tPnr);
        }
        seferSistemi[sNo] = s;
    }
    dosya.close();
    cout << "[-] Seferler yuklendi." << endl;
}

void HavayoluSistemi::bagajlariYukle() {
    ifstream dosya("bagajlar.txt");
    string satir;
    if (!dosya.is_open()) return;

    while (getline(dosya, satir)) {
        stringstream ss(satir);
        string bId, pnr, agirlik; // Tanımlamalar yapıldı

        getline(ss, bId, ',');
        getline(ss, pnr, ',');
        getline(ss, agirlik, ',');

        if(bId.empty() || pnr.empty()) continue;

        Bagaj b = { stoi(bId), pnr, stof(agirlik) };

        for (auto& cift : seferSistemi) {
            for (string& yPnr : cift.second.yolcuPnrListesi) {
                if (yPnr == pnr) {
                    cift.second.kargoBolumu.push(b);
                }
            }
        }
    }
    dosya.close();
    cout << "[-] Bagajlar Stack yapisina eklendi." << endl;
}

void HavayoluSistemi::bagajlariTahliyeEt(string seferNo) {
    if (seferSistemi.find(seferNo) == seferSistemi.end()) {
        cout << "Sefer bulunamadi!" << endl;
        return;
    }

    Sefer& secilenSefer = seferSistemi[seferNo];
    int sira = 1;
    while (!secilenSefer.kargoBolumu.empty()) {
        Bagaj b = secilenSefer.kargoBolumu.top();

        string isim = "Bilinmiyor";
        if (harita.count(b.pnr_sahibi)) {
            isim = harita[b.pnr_sahibi].ad + " " + harita[b.pnr_sahibi].soyad;
        }

        cout << sira << ". Bagaj Sahibi: " << isim << " (" << b.pnr_sahibi << ")" << endl;
        secilenSefer.kargoBolumu.pop();
        sira++;
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