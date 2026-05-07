#include <iostream>
#include <queue>
#include <unordered_map>
#include <map>
#include <vector>
#include "Modeller.h"

using namespace std;

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

    int secim;
    do {
        cout << "\n===== HAVALIMANI YONETIM PANELI =====" << endl;
        cout << "1. Inis Izni Bekleyen Ucaklari Gor (Priority Queue)" << endl;
        cout << "2. Alfabetik Yolcu Listesi Yazdir (BST Tree)" << endl;
        cout << "3. En Kisa Rota Hesapla (Dijkstra - Graph)" << endl;
        cout << "4. Bagaj Tahliye Sirasini Goster (Stack)" << endl;
        cout << "0. Cikis" << endl;
        cout << "Seciminiz: ";
        cin >> secim;

        switch (secim) {
            case 1:
                if (!kuleKuyrugu.empty()) {
                    Ucak enAcil = kuleKuyrugu.top();
                    cout << "\n[!] INIS SIRASI BASINDAKI UCAK:" << endl;
                    cout << "ID: " << enAcil.id << " | Sirket: " << enAcil.havayolu
                         << " | Yakit: " << enAcil.yakit << endl;
                } else cout << "Kuyruk bos!" << endl;
                break;

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

            case 4:
                cout << "\n[LIFO] BAGAJ TAHLIYE SIMULASYONU:" << endl;
                // Burada Adem'in Stack yapısını ekrana basacağız
                cout << "Bagajlar ucak ambarindan son girenden ilk girene dogru cikariliyor..." << endl;
                break;

            case 0:
                cout << "Sistem kapatiliyor..." << endl;
                break;

            default:
                cout << "Gecersiz secim!" << endl;
        }
    } while (secim != 0);

    return 0;
}