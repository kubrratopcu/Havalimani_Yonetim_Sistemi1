#include "HavayoluSistemi.h"
#include <set>
#include <cctype>

HavayoluSistemi::HavayoluSistemi() {
    yolcuAgaciKoku = nullptr;
    yolculariYukle();
    seferleriYukle();
    bagajlariYukle();
    rotalariYukle();
    cout << "[✔] Havalimani Sistemi: Veriler basariyla yuklendi." << endl;
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
        s.kalkisSehri = kalkis;
        s.varisSehri = varis;
        s.ucusZamani = stoll(zaman);

        stringstream ssPnr(pnrList);
        string tPnr;
        while (getline(ssPnr, tPnr, ';')) {
            if (!tPnr.empty()){
                s.yolcuPnrListesi.push_back(tPnr);
                PnrNode* yeni = new PnrNode();
                yeni->pnr = tPnr;
                yeni->next = s.yolcuListesiBasi;
                s.yolcuListesiBasi = yeni;
            }
        }
        seferSistemi[sNo] = s;
    }
    dosya.close();
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

Yolcu HavayoluSistemi::pnrIleYolcuBul(string pnr) {
    if (harita.find(pnr) != harita.end()) return harita[pnr];
    return Yolcu();
}

std::string HavayoluSistemi::siradakiUcagiIndir() {
    if (kuleYonetimi.bosMu()) return "Kuyruk Bos";

    Ucak enOncelikli = kuleYonetimi.enOncelikliyiGetir();
    kuleYonetimi.pop();

    return to_string(enOncelikli.id) + " (" + enOncelikli.havayolu + ")";
}

void HavayoluSistemi::bagajlariYukle() {
    ifstream dosya("bagajlar.txt");
    string satir;
    if (!dosya.is_open()) return;

    while (getline(dosya, satir)) {
        stringstream ss(satir);
        string bId, pnr, agirlik;
        getline(ss, bId, ',');
        getline(ss, pnr, ',');
        getline(ss, agirlik, ',');

        if(bId.empty() || pnr.empty()) continue;

        Bagaj b;
        b.id = stoi(bId);
        b.pnr_sahibi = pnr;
        b.agirlik = stof(agirlik);

        for (auto& cift : seferSistemi) {
            for (string yPnr : cift.second.yolcuPnrListesi) {
                if (yPnr == pnr) cift.second.kargoBolumu.push(b);
            }
        }
    }
    dosya.close();
}

vector<string> HavayoluSistemi::bagajlariTahliyeEt(string seferNo) {
    vector<string> liste;
    if (seferSistemi.find(seferNo) == seferSistemi.end()) return liste;

    Sefer& secilenSefer = seferSistemi[seferNo];
    int sira = 1;
    while (!secilenSefer.kargoBolumu.empty()) {
        Bagaj b = secilenSefer.kargoBolumu.top();
        string bilgi = to_string(sira) + ". Bagaj -> ID: " + to_string(b.id) + " (PNR: " + b.pnr_sahibi + ")";
        liste.push_back(bilgi);
        secilenSefer.kargoBolumu.pop();
        sira++;
    }
    return liste;
}

string HavayoluSistemi::enKisaRota(string kalkis, string varis) {
    if (kalkis == varis) return "Ayni sehri sectiniz.";
    if (graf.find(kalkis) == graf.end()) return "Rota bulunamadi.";

    map<string, int> mesafeler;
    map<string, string> ebeveyn;
    for (auto const& [sehir, komsular] : graf) {
        mesafeler[sehir] = 1e9;
        ebeveyn[sehir] = "";
    }

    mesafeler[kalkis] = 0;
    set<pair<int, string>> kuyruk;
    kuyruk.insert({0, kalkis});

    while (!kuyruk.empty()) {
        string u = kuyruk.begin()->second;
        int d = kuyruk.begin()->first;
        kuyruk.erase(kuyruk.begin());
        if (u == varis) break;
        for (auto& komsu : graf[u]) {
            if (d + komsu.second < mesafeler[komsu.first]) {
                kuyruk.erase({mesafeler[komsu.first], komsu.first});
                mesafeler[komsu.first] = d + komsu.second;
                ebeveyn[komsu.first] = u;
                kuyruk.insert({mesafeler[komsu.first], komsu.first});
            }
        }
    }

    if (mesafeler[varis] >= 1e9) return "Rota bulunamadi.";
    string yol = "";
    for (string s = varis; s != ""; s = ebeveyn[s]) yol = s + (yol == "" ? "" : " -> " + yol);
    return "En Kisa Rota: " + yol + "\nMesafe: " + to_string(mesafeler[varis]) + " km";
}

void HavayoluSistemi::yolculariYukle() {
    ifstream dosya("yolcular.txt");
    string satir;
    if (!dosya.is_open()) return;

    while (getline(dosya, satir)) {
        stringstream ss(satir);
        string ad, soyad, pnr, koltuk;
        getline(ss, ad, ',');
        getline(ss, soyad, ',');
        getline(ss, pnr, ',');
        getline(ss, koltuk, ',');
        harita[pnr] = {ad, soyad, pnr, koltuk};
    }
    dosya.close();
}

string HavayoluSistemi::yolcununUcusBilgisiniGetir(string pnr) {
    for (auto const& cift : seferSistemi) {
        for (string yPnr : cift.second.yolcuPnrListesi) {
            if (yPnr == pnr) return " | Sefer: " + cift.second.seferNo;
        }
    }
    return " | Sefer: Bulunamadi";
}

void HavayoluSistemi::rotalariYukle() {
    ifstream dosya("rotalar.txt");
    string satir;
    if (!dosya.is_open()) return;
    while (getline(dosya, satir)) {
        stringstream ss(satir);
        string k, h, m;
        getline(ss, k, ','); getline(ss, h, ','); getline(ss, m, ',');
        if (!k.empty()) graf[k].push_back({h, stoi(m)});
    }
    dosya.close();
}