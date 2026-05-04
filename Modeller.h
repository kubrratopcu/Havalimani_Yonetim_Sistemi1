#ifndef MODELLER_H
#define MODELLER_H

#include <string>
#include <vector>

using namespace std;

/**
 * @brief Ucak Modeli
 * Kübra'nın Priority Queue (Öncelikli Kuyruk) yapısında kullanılacaktır.
 */
struct Ucak {
    int id;              // Benzersiz Uçak Kimliği
    string havayolu;     // Havayolu Şirketi
    int yakit;           // Yakıt Miktarı (0-100 arası gibi düşünülebilir)
    long long varisZamani; // YYYYMMDDHHMM formatında (Örn: 202605101530)
    int kapasite = 120; // Sadece toplam sınırı bilmek için
    /**
     * @brief Öncelik Belirleme Operatörü
     * Kuyrukta uçakları şu mantıkla sıralar:
     * 1. Önce varış zamanı en yakın (küçük sayı) olan en üste çıkar.
     * 2. Eğer zamanlar aynıysa, yakıtı en az olan (kritik durum) en üste çıkar.
     */
    bool operator<(const Ucak& diger) const {
        if (varisZamani != diger.varisZamani) {
            // Priority Queue 'max-heap' mantığıyla çalıştığı için
            // '>' operatörü küçük olan zamanı (en yakını) üste taşır.
            return varisZamani > diger.varisZamani;
        }
        // Zamanlar eşitse yakıtı az olan öncelik kazanır.
        return yakit > diger.yakit;
    }
};

/**
 * @brief Sefer Modeli
 * Yolcuları ve uçakları bağlayan köprü yapısı.
 */
struct Sefer {
    string seferNo;      // Uçuş Kodu (Örn: TK1920)
    int ucakId;          // Ucak::id ile eşleşen kimlik
    string kalkisYeri;
    string varisYeri;
    // Bu seferdeki koltukların doluluk durumunu tutan matris
    // [20] sıra, [6] sütun (A, B, C, D, E, F)
    bool koltukDurumu[20][6] = {false};
    long long ucusZamani; // Uçağın varış zamanı ile aynı tutulacak veri
    vector<string> yolcuPnrListesi; // Bu uçuştaki yolcuların PNR listesi
    // Adem'in LIFO (Son Giren İlk Çıkar) mantığı burada çalışacak:
    stack<Bagaj> kargoBolumu;
};

/**
 * @brief Yolcu Modeli
 * Beyza'nın Hash Table (unordered_map) yapısı için.
 */
struct Yolcu {
    string ad;
    string soyad;
    string pnr;          // Hash anahtarı (Key)
    string koltuk;
};

/**
 * @brief Bagaj Modeli
 * Adem'in Stack (Yığın) yapısı için.
 */
struct Bagaj {
    int id;
    string pnr_sahibi;   // Yolcu::pnr ile eşleşen kimlik
    float agirlik;
};

/**
 * @brief Rota Modeli
 * Kübra'nın Dijkstra Algoritması için.
 */
struct Rota {
    string kaynak;
    string hedef;
    int mesafe;
};

#endif // MODELLER_H