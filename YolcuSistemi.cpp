#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include "Modeller.h"

using namespace std;

// --- LINKED LIST (BAĞLI LİSTE) DÜĞÜM YAPISI ---


// ikili arama ağacı (BST) için temel düğüm yapısı oluşturulması
struct YolcuNode {
    Yolcu veri;             // Yolcu bilgilerini tutan struct
    struct YolcuNode *sol;  // Sol kol (Pointer)
    struct YolcuNode *sag;  // Sağ kol (Pointer)
};
// Burada Yolcu struct yapısı tüm yolcuya ait değişkenler için geçerlidir. Tüm değişkenleri alır.

// --- LINKED LIST (BAĞLI LİSTE) EKLEME FONKSİYONU ---
// Seferin içindeki PNR listesine yeni bir düğüm ekler (Linked List Mantığı)
// --- LINKED LIST (BAĞLI LİSTE) EKLEME FONKSİYONU ---
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

// malloc ve NULL mantığıyla yeni düğüm oluşturma fonksiyonu
// C++ mantığıyla güvenli düğüm oluşturma fonksiyonu
struct YolcuNode* yeniDugumOlustur(Yolcu y) {
    struct YolcuNode* yeni = new YolcuNode();

    yeni->veri = y;         // Veriyi düğümün içine kopyala
    yeni->sol = nullptr;    // Sol kolu boşalt (nullptr ile)
    yeni->sag = nullptr;    // Sağ kolu boşalt (nullptr ile)
    return yeni;            // Hazırlanan düğümün adresini döndür
}

// Ağaca Ekleme (Recursive)
// Yolcunun adına bakarak alfabetik olarak nereye gideceğini bulur.
struct YolcuNode* agacaYolcuEkle(struct YolcuNode* kok, Yolcu y) {
    // Eğer baktığımız dal boşsa, yeni düğümü buraya yerleştir
    if (kok == NULL) {
        return yeniDugumOlustur(y);
    }

    //y.ad mantığı: y tüm struct yapısı .ad ise o struct yapısındaki ad değişkeni yani yolcunun ismi

    // Alfabetik karşılaştırma: Yeni isim mevcut düğümden küçükse SOLA
    if (y.ad < kok->veri.ad) {
        kok->sol = agacaYolcuEkle(kok->sol, y);
    }
    // Alfabetik olarak büyükse SAĞA
    else {
        kok->sag = agacaYolcuEkle(kok->sag, y);
    }

    return kok; // Bağlantıları güncel tutmak için kökü döndür
}

// Dosyadan Yükleme
void yolculariYukle(unordered_map<string, Yolcu>& harita, struct YolcuNode*& kok) {
    ifstream dosya("yolcular.txt");
    string satir;

    if (!dosya.is_open()) {
        cerr << "Hata: yolcular.txt acilamadi!" << endl;
        return;
    }

    while (getline(dosya, satir)) {
        stringstream ss(satir);
        string ad, soyad, pnr, koltuk;

        getline(ss, ad, ',');
        getline(ss, soyad, ',');
        getline(ss, pnr, ',');
        getline(ss, koltuk, ',');

        Yolcu y = {ad, soyad, pnr, koltuk};

        // HASH TABLE: PNR ile hızlı erişim için
        harita[pnr] = y;

        // BST (AĞAÇ): Alfabetik listeleme için
        kok = agacaYolcuEkle(kok, y);
    }
    dosya.close();
    cout << "[-] Yolcu Sistemi: Veriler C mantigiyla yuklendi." << endl;
}

// In-order Listeleme (Sol-Kök-Sağ)
// Bu fonksiyon ağacı en soldan başlayarak gezer, böylece A'dan Z'ye çıktı verir.
void yolculariListele(struct YolcuNode* kok) {
    if (kok == NULL) return; // Durma koşulu

    yolculariListele(kok->sol); // Önce küçükleri (Solu) gez

    // Mevcut (Kök) düğümü yazdır
    cout << "    * " << kok->veri.ad << " " << kok->veri.soyad << " (" << kok->veri.pnr << ")" << endl;

    yolculariListele(kok->sag); // Sonra büyükleri (Sağı) gez
}

void yeniYolcuEkle(unordered_map<string, Yolcu>& harita, YolcuNode*& kok, Sefer& secilenSefer) {
    string ad, soyad, pnr;
    cout << "Ad: "; cin >> ad;
    cout << "Soyad: "; cin >> soyad;
    pnr = "PNR" + to_string(rand() % 9000 + 1000);

    bool yerBulundu = false; // Yer bulup bulmadığımızı takip eden bayrak (flag)

    // Koltuk Bulma (Matriste ilk boş yeri bulur)
    for(int i = 0; i < 20; i++) {
        for(int j = 0; j < 6; j++) {
            if(!secilenSefer.koltukDurumu[i][j]) {
                secilenSefer.koltukDurumu[i][j] = true;
                string koltukNo = to_string(i + 1) + (char)('A' + j);

                Yolcu y = {ad, soyad, pnr, koltukNo};
                harita[pnr] = y;
                kok = agacaYolcuEkle(kok, y);

                // --- LINKED LIST ENTEGRASYONU ---
                // Yeni yolcuyu seferin içindeki bağlı listeye de ekliyoruz
                seferListesineEkle(secilenSefer.yolcuListesiBasi, pnr);

                cout << "[+] Yolcu Kaydedildi! PNR: " << pnr << " Koltuk: " << koltukNo << endl;
                yerBulundu = true;
                return; // Yer bulunduğu an fonksiyon biter
            }
        }
    }

    // EĞER DÖNGÜLER BİTTİ VE BURAYA GELDİYSEK:
    if (!yerBulundu) {
        cout << "[!!!] HATA: Bu seferde (Sefer No: " << secilenSefer.seferNo
             << ") bos koltuk kalmamistir! Kayit yapilamadi." << endl;
    }
}
// Belleği serbest bırakma (Memory Deallocation)
// Belleği serbest bırakma (Memory Deallocation)
void agaciTemizle(YolcuNode* kok) {
    if (kok == nullptr) return;
    agaciTemizle(kok->sol);
    agaciTemizle(kok->sag);
    delete kok; // free YERİNE delete kullanıldı
}

void seferListesiniTemizle(PnrNode*& bas) {
    PnrNode* akim = bas;
    while (akim != nullptr) {
        PnrNode* sonraki = akim->next;
        delete akim; // free YERİNE delete kullanıldı
        akim = sonraki;
    }
    bas = nullptr; // Başlangıç işaretçisini sıfırla
}