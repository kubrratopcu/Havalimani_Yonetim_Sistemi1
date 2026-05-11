#include <iostream>
#include <queue>
#include <unordered_map>
#include <map>
#include <vector>
#include "Modeller.h"
#include "PerformansOlcer.cpp"


#include "mainwindow.h"
#include <QApplication>

#include <iostream>
#include <fstream>
#include <string>

#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char *argv[]) {


    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
/*
void yolcu1kUret() {
    // Proje klasöründe yolcular_1k.txt adında bir dosya oluşturur
    std::ofstream dosya("yolcular_1k.txt");

    if (!dosya.is_open()) {
        std::cerr << "Dosya olusturulamadi!" << std::endl;
        return;
    }

    for (int i = 1; i <= 1000; i++) {
        // Format: PNR, Ad, Soyad, Koltuk
        // Örnek çıktı: PNR1,Yolcu1,Soyad1,2A
        dosya << "PNR" << i << ","
              << "Yolcu" << i << ","
              << "Soyad" << i << ","
              << (i % 30 + 1) << (char)('A' + (i % 6))
              << "\n";
    }
}

    void yolcu10kUret() {
        // Proje klasöründe yolcular_1k.txt adında bir dosya oluşturur
        std::ofstream dosya("yolcular_10k.txt");

        if (!dosya.is_open()) {
            std::cerr << "Dosya olusturulamadi!" << std::endl;
            return;
        }

        for (int i = 1; i <= 10000; i++) {
            // Format: PNR, Ad, Soyad, Koltuk
            // Örnek çıktı: PNR1,Yolcu1,Soyad1,2A
            dosya << "PNR" << i << ","
                  << "Yolcu" << i << ","
                  << "Soyad" << i << ","
                  << (i % 30 + 1) << (char)('A' + (i % 6))
                  << "\n";
        }

    dosya.close();
    std::cout << "yolcular_10k.txt basariyla olusturuldu (1.000 Kayit)." << std::endl;
}

void yolcu100kUret() {
    // Proje klasöründe yolcular_1k.txt adında bir dosya oluşturur
    std::ofstream dosya("yolcular_100k.txt");

    if (!dosya.is_open()) {
        std::cerr << "Dosya olusturulamadi!" << std::endl;
        return;
    }

    for (int i = 1; i <= 100000; i++) {
        // Format: PNR, Ad, Soyad, Koltuk
        // Örnek çıktı: PNR1,Yolcu1,Soyad1,2A
        dosya << "PNR" << i << ","
              << "Yolcu" << i << ","
              << "Soyad" << i << ","
              << (i % 30 + 1) << (char)('A' + (i % 6))
              << "\n";
    }
}
void yolcu500kUret() {
    // Proje klasöründe yolcular_500k.txt adında bir dosya oluşturur
    std::ofstream dosya("yolcular_50000k.txt");

    if (!dosya.is_open()) {
        std::cerr << "Dosya olusturulamadi!" << std::endl;
        return;
    }

    for (int i = 1; i <= 500000; i++) {
        // Format: PNR, Ad, Soyad, Koltuk
        // Örnek çıktı: PNR1,Yolcu1,Soyad1,2A
        dosya << "PNR" << i << ","
              << "Yolcu" << i << ","
              << "Soyad" << i << ","
              << (i % 30 + 1) << (char)('A' + (i % 6))
              << "\n";
    }
}



using namespace std;
void bagajlariTahliyeEt(Sefer& secilenSefer, unordered_map<string, Yolcu>& yolcuHaritasi);

// --- FONKSİYON BİLDİRİMLERİ (KuleYonetimi.cpp ve diğerlerinden gelecek) ---
void ucaklariYukle(priority_queue<Ucak>& kule);
void rotalariYukle(map<string, vector<pair<string, int>>>& graf);
void enKisaYoluBul(map<string, vector<pair<string, int>>>& graf, string baslangic, string hedef);

// Diğer modüllerden (Beyza ve Adem)
struct YolcuNode;
void yolculariYukle(unordered_map<string, Yolcu>& harita, YolcuNode*& kok);
void yolculariListele(YolcuNode* kok);
void seferleriYukle(map<string, Sefer>& seferSistemi);
void bagajlariYukle(map<string, Sefer>& seferSistemi);

int main() {
    yolcu1kUret();
    yolcu10kUret();
    yolcu100kUret();
    yolcu500kUret();

    // 1. VERİ YAPILARINI HAZIRLA
    priority_queue<Ucak> kuleKuyrugu;
    map<string, vector<pair<string, int>>> ucusGrafi;
    unordered_map<string, Yolcu> yolcuHaritasi;
    YolcuNode* bstKoku = nullptr;
    map<string, Sefer> seferler;

    // 2. VERİLERİ DOSYADAN YÜKLE (Açılışta bir kez yapılır)
    ucaklariYukle(kuleKuyrugu);
    rotalariYukle(ucusGrafi);
    yolculariYukle(yolcuHaritasi, bstKoku);
    seferleriYukle(seferler);
    bagajlariYukle(seferler);
    HavayoluSistemi sistem;
    int secim;
    do {
        cout << "\n===== HAVALIMANI YONETIM PANELI =====" << endl;
        cout << "1. Inis Izni Bekleyen Ucaklari Gor (Priority Queue)" << endl;
        cout << "2. Alfabetik Yolcu Listesi Yazdir (BST Tree)" << endl;
        cout << "3. En Kisa Rota Hesapla (Dijkstra - Graph)" << endl;
        cout << "4. Bagaj Tahliye Sirasini Goster (Stack)" << endl;
        cout << "5. Farkli veri boyutlari icin performanslari gor."<< endl;
        cout << "0. Cikis" << endl;
        cout << "Seciminiz: ";
        cin >> secim;

        switch (secim) {
            case 1: { // Tüm bekleyen uçakları göster
                if (!kuleKuyrugu.empty()) {
                    priority_queue<Ucak> gecici = kuleKuyrugu;
                    cout << "\n--- INIS SIRASI (Oncelik Sirali) ---" << endl;
                    while (!gecici.empty()) {
                        Ucak u = gecici.top();
                        cout << "ID: " << u.id << " | Sirket: " << u.havayolu
                             << " | Yakit: " << u.yakit
                             << " | Acil: " << (u.acilDurum ? "EVET" : "HAYIR") << endl;
                        gecici.pop();
                    }
                } else cout << "Kuyruk bos!" << endl;
                break;
            }

            case 2:
                cout << "\n[A-Z] YOLCU LISTESI:" << endl;
            yolculariListele(bstKoku);
            break;

            case 3: {
                string nerden, nereye;
                cout << "Kalkis Sehri: "; cin >> nerden;
                cout << "Varis Sehri: "; cin >> nereye;
                enKisaYoluBul(ucusGrafi, nerden, nereye);
                break;
            }

            case 4: { // Bagaj tahliyesini başlat
                string sNo;
                cout << "Sefer No girin (Orn: TK1920): "; cin >> sNo;
                if (seferler.count(sNo)) {
                    bagajlariTahliyeEt(seferler[sNo], yolcuHaritasi);
                } else cout << "[!] Sefer bulunamadi!" << endl;
                break;
            }
            case 5: {
                string dosyaIsimleri[] = {"yolcular_1k.txt", "yolcular_10k.txt", "yolcular_100k.txt", "yolcular_500k.txt"};
                string pnrlar[] = {"PNR1000", "PNR10000", "PNR100000", "PNR500000"};

                cout << "\n--- OTOMATIK ANALIZ BASLIYOR ---\n";

                for(int i = 0; i < 4; i++) {
                    cout << "\n>>> " << dosyaIsimleri[i] << " Analiz Ediliyor..." << endl;

                    // 1. EKLEME ISLEMI (BST ve Hash Table'a veri yukleme)
                    // Fonksiyonun icindeki PerformansOlcer bu sureyi 'Yolcu Yukleme' adiyla basacak.
                    sistem.yolculariYukle(dosyaIsimleri[i]);

                    // 2. ARAMA ISLEMI (Hash Table uzerinden PNR arama)
                    {
                        PerformansOlcer aramaOlcer("Arama Islemi (Hash Table)");
                        sistem.pnrIleYolcuBul(pnrlar[i]);
                    }

                    // 3. GÜNCELLEME / İŞLEME (Heap/Array - Uçak İniş İşlemi gibi)
                    {
                        PerformansOlcer guncellemeOlcer("Guncelleme Islemi (Heap/Array)");
                        // Burada heap yapısı uzerinden siradaki ucagi indirme islemi yapabilirsin
                        sistem.siradakiUcagiIndir();
                    }
                }

                cout << "\n--- Analiz Bitti. Konsol ciktilarindaki ms degerlerini tabloya yaziniz. ---\n";
                break;
            }
    }
        }while (secim != 0);
        return 0;
    }

*/


