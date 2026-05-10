#pragma once

#include <string>
#include <vector>
#include <stack>

using namespace std;

struct Bagaj {
    int id;
    string pnr_sahibi;
    float agirlik;
};

struct Ucak {
    int id;
    string havayolu;
    int yakit;
    long long varisZamani;
    bool acilDurum;

    // Priority Queue (Öncelikli Kuyruk) için sıralama mantığı
    bool operator<(const Ucak& diger) const {
        // Acil durumu olan uçak her zaman önceliklidir
        if (acilDurum != diger.acilDurum) {
            return !acilDurum;
        }
        // Acil durumlar eşitse, yakıtı AZ olan uçak önceliklidir
        return yakit > diger.yakit;
    }
};

// 2. YOLCU YAPISI
struct Sefer {
    string seferNo;
    int ucakId;
    long long ucusZamani;
    vector<string> yolcuPnrListesi;
    stack<Bagaj> kargoBolumu;
    bool koltukDurumu[20][6] = {false};
    struct PnrNode* yolcuListesiBasi = nullptr;
};

struct Yolcu {
    string ad;
    string soyad;
    string pnr;
    string koltukNo;
};


// İkili Arama Ağacı (BST) Düğüm Yapısı(bellek temızlıgı ıcın)
struct YolcuNode {
    Yolcu veri;
    YolcuNode *sol;
    YolcuNode *sag;
};
// 3. BAGAJ YAPISI (Stack İçin)




// 4. LINKED LIST (Bağlı Liste) DÜĞÜM YAPISI
struct PnrNode {
    string pnr;
    struct PnrNode *next;
};

// 5. SEFER YAPISI
