#ifndef MODELLER_H
#define MODELLER_H

#include <string>
#include <vector>
#include <stack> // Stack (Yığın) yapısı için eklendi

using namespace std;

struct Bagaj {
    int id;
    string pnr_sahibi;   // Yolcu::pnr ile eşleşen kimlik (Hash Table için)
    float agirlik;
};

struct Sefer {
    string seferNo;
    int ucakId;          // Hangi fiziksel uçağın bu seferi yaptığı
    long long ucusZamani;

    // Bu seferdeki koltukların doluluk durumunu tutan matris
    // [20] sıra, [6] sütun (A, B, C, D, E, F)
    bool koltukDurumu[20][6] = {false};

    vector<string> yolcuPnrListesi; // Seferdeki yolcuların PNR listesi

    // Adem'in LIFO (Son Giren İlk Çıkar) mantığı burada çalışacak:
    // Her seferin kendi bagaj yığını (Stack) olur.
    stack<Bagaj> kargoBolumu;
};

struct Ucak {
    int id;
    string havayolu;
    int yakit;
    long long varisZamani;
    int kapasite = 120; // Sadece toplam sınırı bilmek için

    // Priority Queue (Heap) için çok kriterli sıralama mantığı
    bool operator<(const Ucak& diger) const {
        // Önce zamana bak, zamanlar eşitse yakıtı az olan öne geçsin
        if (varisZamani != diger.varisZamani) return varisZamani > diger.varisZamani;
        return yakit > diger.yakit;
    }
};

struct Yolcu {
    string ad;
    string soyad;
    string pnr;          // Hash anahtarı (Key)
    string koltuk;       // Sadece etiket/bilgi amaçlı (Fiziksel kontrol Sefer'in matrisinde yapılır)
};

struct Rota {
    string kaynak;       // Başlangıç Şehri (Düğüm/Node A)
    string hedef;        // Varış Şehri (Düğüm/Node B)
    int mesafe;          // İki şehir arası uzaklık (Kenar Ağırlığı/Edge Weight)
};

#endif