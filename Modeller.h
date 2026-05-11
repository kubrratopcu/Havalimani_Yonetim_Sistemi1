#pragma once

#include <string>
#include <vector>
#include <stack>

using namespace std;

// 1. BAGAJ YAPISI (LIFO - Stack mantığı için)
struct Bagaj {
    int id;              // Bagajın benzersiz numarası
    string pnr_sahibi;   // Bagajın hangi yolcuya ait olduğu (PNR eşleşmesi)
    float agirlik;       // Bagajın ağırlığı (kg)
};

// 2. UÇAK YAPISI (Priority Queue sıralama mantığı ile)
struct Ucak {
    int id;              // Uçuş/Kuyruk numarası
    string havayolu;     // Şirket adı
    int yakit;           // Mevcut yakıt yüzdesi
    long long varisZamani;
    bool acilDurum;      // Kritik durum (true ise en başa geçer)


    // Priority Queue (Öncelikli Kuyruk) için karşılaştırma operatörü.
     // C++'da priority_queue 'en büyük' elemanı başa aldığı için mantık ters kurulur.

    bool operator<(const Ucak& diger) const {
        // Kriter 1: Acil durumu olan uçak her zaman önceliklidir (Kuyruğun başına gider)
        if (acilDurum != diger.acilDurum) {
            return !acilDurum;
        }
        // Kriter 2: Acil durumlar eşitse, yakıtı AZ olan uçağa öncelik verilir
        return yakit > diger.yakit;
    }
};

// 3. SEFER YAPISI (Uçuş Bilgileri ve Veri Yapıları)
struct Sefer {
    string seferNo;      // Örn: TK1920
    int ucakId;          // Uçak nesnesi ile ilişkilendirme
    string kalkisSehri;
    string varisSehri;
    long long ucusZamani;

    vector<string> yolcuPnrListesi;  // Hızlı erişim için PNR listesi
    stack<Bagaj> kargoBolumu;        // Bagajlar için Stack (Son giren ilk çıkar - LIFO)

    // Uçak içindeki koltuk düzeni (20 sıra, 6 sütun - false: boş, true: dolu)
    bool koltukDurumu[20][6] = {false};

    struct PnrNode* yolcuListesiBasi = nullptr; // Bağlı liste (Linked List) başlangıcı
};

// 4. YOLCU VE BST (İkili Arama Ağacı) YAPISI
struct Yolcu {
    string ad;
    string soyad;
    string pnr;          // Arama anahtarı (Key)
    string koltukNo;     // Örn: 12A
};


 // Yolcuları PNR koduna göre sıralı tutan BST düğümü.
 // PNR sorgulamalarında O(log n) performans sağlar.

struct YolcuNode {
    Yolcu veri;
    YolcuNode *sol;      // Küçük (alfabetik) PNR'lar sola
    YolcuNode *sag;      // Büyük (alfabetik) PNR'lar sağa
};

// 5. BAĞLI LİSTE (Linked List) DÜĞÜMÜ
struct PnrNode {
    string pnr;
    struct PnrNode *next; // Bir sonraki yolcu düğümüne işaretçi
};