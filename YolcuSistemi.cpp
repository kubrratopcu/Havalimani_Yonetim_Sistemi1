#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include "Modeller.h"

using namespace std;

// İkili Arama Ağacı (BST) için düğüm yapısı
struct YolcuNode {
    Yolcu veri;
    YolcuNode *sol, *sag;
    YolcuNode(Yolcu y) : veri(y), sol(nullptr), sag(nullptr) {}
};

// Yolcuyu adına göre alfabetik olarak ağaca ekleyen özyinelemeli (recursive) fonksiyon
YolcuNode* agacaYolcuEkle(YolcuNode* kok, Yolcu y) {
    if (kok == nullptr) return new YolcuNode(y); // Boş yere ulaşıldığında yeni düğümü oluştur

    if (y.ad < kok->veri.ad)
        kok->sol = agacaYolcuEkle(kok->sol, y); // İsim alfabetik olarak önce geliyorsa sola git
    else
        kok->sag = agacaYolcuEkle(kok->sag, y); // İsim sonra geliyorsa sağa git

    return kok;
}

// Dosyadan okunan yolcuları hem Hash Table'a hem de BST'ye yükler
void yolculariYukle(unordered_map<string, Yolcu>& harita, YolcuNode*& kok) {
    ifstream dosya("yolcular.txt");
    string satir;

    if (!dosya.is_open()) {
        cerr << "Hata: yolcular.txt acilamadi!" << endl;
        return;
    }

    // Dosyayı satır satır oku
    while (getline(dosya, satir)) {
        stringstream ss(satir);
        string ad, soyad, pnr, koltuk;

        // Virgülle ayrılmış yolcu bilgilerini değişkenlere aktar
        getline(ss, ad, ',');
        getline(ss, soyad, ',');
        getline(ss, pnr, ',');
        getline(ss, koltuk, ',');

        Yolcu y = {ad, soyad, pnr, koltuk};

        harita[pnr] = y;              // PNR ile anında arama (O(1)) için Hash Table'a ekle
        kok = agacaYolcuEkle(kok, y); // Alfabetik sıralama yapabilmek için Ağaca (BST) ekle
    }

    dosya.close();
    cout << "[-] Yolcu Sistemi: Yolcular Hash Table ve BST'ye yuklendi." << endl;
}

// In-order (Sol-Kök-Sağ) dolaşma yöntemiyle yolcuları alfabetik sırada yazdırır
void yolculariListele(YolcuNode* kok) {
    if (kok == nullptr) return; // Ağaç/Düğüm boşsa geri dön

    yolculariListele(kok->sol); // Önce alfabetik olarak daha küçük olan sol tarafı gez
    cout << "    * " << kok->veri.ad << " " << kok->veri.soyad << " (" << kok->veri.pnr << ")" << endl;
    yolculariListele(kok->sag); // Sonra alfabetik olarak daha büyük olan sağ tarafı gez
}