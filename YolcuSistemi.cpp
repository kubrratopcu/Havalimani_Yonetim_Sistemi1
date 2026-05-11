#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include "HavayoluSistemi.h" // Sınıf tanımı için gerekli
#include "Modeller.h"
#include "PerformansOlcer.cpp"

using namespace std;

// --- LINKED LIST EKLEME FONKSİYONU ---
void seferListesineEkle(PnrNode*& bas, string pnr) {
    PnrNode* yeni = new PnrNode();
    yeni->pnr = pnr;
    yeni->next = nullptr;

    if (bas == nullptr) {
        bas = yeni;
    } else {
        PnrNode* gecici = bas;
        while (gecici->next != nullptr) {
            gecici = gecici->next;
        }
        gecici->next = yeni;
    }
}

// --- DÜĞÜM OLUŞTURMA ---
struct YolcuNode* yeniDugumOlustur(Yolcu y) {
    struct YolcuNode* yeni = new YolcuNode();
    yeni->veri = y;
    yeni->sol = nullptr;
    yeni->sag = nullptr;
    return yeni;
}

// --- BST AĞACA EKLEME ---
struct YolcuNode* agacaYolcuEkle(struct YolcuNode* kok, Yolcu y) {
    if (kok == nullptr) {
        return yeniDugumOlustur(y);
    }

    if (y.ad < kok->veri.ad) {
        kok->sol = agacaYolcuEkle(kok->sol, y);
    } else {
        kok->sag = agacaYolcuEkle(kok->sag, y);
    }
    return kok;
}
/*
// --- DOSYADAN YÜKLEME (HAVAYOLU SISTEMI SINIFINA AIT) ---
void HavayoluSistemi::yolculariYukle(std::string dosyaAdi) {
    // 1. Temizlik: Her yeni testte verilerin üst üste binmemesi için
    harita.clear();
    yolcuAgaciKoku = nullptr; // .h dosyasındaki ismin bu olduğu için 'kok' yerine bunu yazdık

    // 2. Ölçüm
    PerformansOlcer olcer("Yolcu Yukleme - " + dosyaAdi);

    // 3. Dosya Açma
    ifstream dosya(dosyaAdi);
    string satir;

    if (!dosya.is_open()) {
        cerr << "[!] Hata: " << dosyaAdi << " dosyasi acilamadi!" << endl;
        return;
    }

    while (getline(dosya, satir)) {
        if (satir.empty()) continue;

        stringstream ss(satir);
        string ad, soyad, pnr, koltuk;

        getline(ss, ad, ',');
        getline(ss, soyad, ',');
        getline(ss, pnr, ',');
        getline(ss, koltuk, ',');

        Yolcu y = {ad, soyad, pnr, koltuk};

        // HASH TABLE: PNR ile hızlı erişim
        harita[pnr] = y;

        // BST (AĞAÇ): yolcuAgaciKoku değişkenini güncelliyoruz
        yolcuAgaciKoku = agacaYolcuEkle(yolcuAgaciKoku, y);
    }

    dosya.close();
    cout << "[-] Basariyla Yuklendi: " << dosyaAdi << " (Harita Boyutu: " << harita.size() << ")" << endl;
}
*/
// --- LISTELEME (IN-ORDER) ---
void yolculariListele(struct YolcuNode* kok) {
    if (kok == nullptr) return;
    yolculariListele(kok->sol);
    cout << "    * " << kok->veri.ad << " " << kok->veri.soyad << " (" << kok->veri.pnr << ")" << endl;
    yolculariListele(kok->sag);
}
/*
// --- BELLEK TEMIZLEME ---
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
*/