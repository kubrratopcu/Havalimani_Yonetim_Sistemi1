#ifndef HAVAYOLUSISTEMI_H
#define HAVAYOLUSISTEMI_H

#include <iostream>
#include <map>
#include <queue>
#include <vector>
#include <unordered_map>
#include <stack>
#include <string>
#include <fstream>
#include <sstream>
#include "Modeller.h"

using namespace std;

// Havalimanı Yönetim Sistemi Ana Sınıfı
class HavayoluSistemi {
private:
    // --- ÖZEL VERİ YAPILARI (Private: Sadece bu sınıf erişebilir) ---
    map<string, Sefer> seferSistemi;             // Sefer No'ya göre tüm uçuş detaylarını saklayan sözlük
    priority_queue<Ucak> kule;                   // İniş önceliğine (yakıt/acil durum) göre uçakları sıralayan yığın (Heap)
    map<string, vector<pair<string, int>>> graf; // Şehirler arası uçuş rotalarını tutan komşuluk listesi
    unordered_map<string, Yolcu> harita;         // PNR kodundan yolcu ismine O(1) hızında ulaşmamızı sağlayan Hash Table
    YolcuNode* yolcuAgaciKoku;                   // Yolcuları alfabetik sıralayan İkili Arama Ağacı'nın (BST) başlangıç noktası

public:
    // --- SINIF METOTLARI (Public: Dışarıdan çağrılabilir) ---

    // Yapıcı Metot (Constructor): Sistem belleğe yüklendiğinde ilk ayarları yapar
    HavayoluSistemi();

    // Yıkıcı Metot (Destructor): Bellekte 'new' ile açılan alanları temizleyerek sızıntıları önler
    // Hata buradaydı, bu satırın mutlaka burada olması gerekiyor!
    ~HavayoluSistemi();

    // Veri Yükleme İşlemleri
    void seferleriYukle();   // 'seferler.txt' dosyasını satır satır parçalar
    void bagajlariYukle();   // 'bagajlar.txt' dosyasını okuyup ilgili sefere Stack olarak ekler
    void bagajlariTahliyeEt(string seferNo); // Stack yapısını kullanarak LIFO mantığıyla bagaj boşaltır

    // Bellek Yönetimi Yardımcıları (Rekürsif ve Pointer işlemleri)
    void agaciTemizle(YolcuNode* kok);        // BST'yi Post-Order mantığıyla tamamen siler
    void seferListesiniTemizle(PnrNode*& bas); // Bağlı listeleri düğüm düğüm gezerek yok eder
};

#endif // HAVAYOLUSISTEMI_H