#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include "HavayoluSistemi.h"
#include "Modeller.h"
#include "PerformansOlcer.cpp"

using namespace std;

// 1. LINKED LIST (BAĞLI LİSTE) İŞLEMLERİ


// Sefer bazlı PNR listesinin sonuna yeni bir düğüm ekler.
 //Bu yapı, bir uçuşa ait yolcuların PNR kodlarını sırayla tutar.
// bas: Listenin başlangıç düğümüne referans (Pointer Reference).
// pnr: Eklenecek olan PNR kodu.

void seferListesineEkle(PnrNode*& bas, string pnr) {
    // Bellekte yeni bir düğüm oluştur ve verisini ata
    PnrNode* yeni = new PnrNode();
    yeni->pnr = pnr;
    yeni->next = nullptr;

    // Liste boşsa, yeni düğüm artık listenin başıdır
    if (bas == nullptr) {
        bas = yeni;
    } else {
        // Liste boş değilse, listenin sonuna (nullptr olana kadar) git
        PnrNode* gecici = bas;
        while (gecici->next != nullptr) {
            gecici = gecici->next;
        }
        // Son düğümün 'next' değerini yeni düğüme bağla
        gecici->next = yeni;
    }
}

// 2. BST (İKİLİ ARAMA AĞACI) İŞLEMLERİ


// Yeni bir BST düğümü (YolcuNode) oluşturur.
 // y: İçerisine yazılacak Yolcu nesnesi.
 // Oluşturulan düğümün adresi.

struct YolcuNode* yeniDugumOlustur(Yolcu y) {
    struct YolcuNode* yeni = new YolcuNode();
    yeni->veri = y;
    yeni->sol = nullptr; // Sol dal başlangıçta boş
    yeni->sag = nullptr; // Sağ dal başlangıçta boş
    return yeni;
}

// Yolcuları alfabetik isim sırasına göre BST ağacına yerleştirir.
// kok: Ağacın (veya alt ağacın) kök düğümü.
// y: Eklenecek yolcu bilgisi.
 // Güncellenmiş kök düğüm adresi.

struct YolcuNode* agacaYolcuEkle(struct YolcuNode* kok, Yolcu y) {
    // Eğer ağaç boşsa veya yaprak düğüme ulaşıldıysa yeni düğümü buraya ekle
    if (kok == nullptr) {
        return yeniDugumOlustur(y);
    }

    // İsim karşılaştırması: Yeni isim köktekinden alfabetik olarak küçükse SOLA git
    if (y.ad < kok->veri.ad) {
        kok->sol = agacaYolcuEkle(kok->sol, y);
    }
    // Değilse SAĞA git
    else {
        kok->sag = agacaYolcuEkle(kok->sag, y);
    }
    return kok;
}


 // Ağaçtaki tüm yolcuları 'In-order Traversal' (Sol-Kök-Sağ) ile listeler.
 // Bu yöntem, verilerin alfabetik olarak sıralı bir şekilde ekrana basılmasını sağlar.
 // kok: Listelenmeye başlanacak kök düğüm.

void yolculariListele(struct YolcuNode* kok) {
    if (kok == nullptr) return;

    // Önce sol daldakileri gez (Alfabetik olarak en öndekiler)
    yolculariListele(kok->sol);

    // Mevcut (kök) düğümdeki bilgiyi yazdır
    cout << "    * " << kok->veri.ad << " " << kok->veri.soyad << " (" << kok->veri.pnr << ")" << endl;

    // Sonra sağ daldakileri gez
    yolculariListele(kok->sag);
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