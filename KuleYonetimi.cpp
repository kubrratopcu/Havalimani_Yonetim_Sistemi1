#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <map>
#include <vector>
#include "Modeller.h"

using namespace std;

// Uçakları dosyadan okuyup Priority Queue'ya (Öncelik Kuyruğu atar
void ucaklariYukle(priority_queue<Ucak>& kule) {
    ifstream dosya("ucaklar.txt"); // Dosyayı okumak için aç
    string satir;

    if (!dosya.is_open()) {
        cerr << "Hata: ucaklar.txt acilamadi!" << endl;
        return;
    }

    // Dosyayı satır satır sonuna kadar oku
    while (getline(dosya, satir)) {
        stringstream ss(satir);
        string id, havayolu, yakit, zaman;

        // Virgülle ayrılmış verileri ilgili değişkenlere böl
        getline(ss, id, ',');
        getline(ss, havayolu, ',');
        getline(ss, yakit, ',');
        getline(ss, zaman, ',');

        // Okunan string verilerini uygun tiplere dönüştürerek Ucak nesnesi oluştur
        Ucak u;
        u.id = stoi(id);
        u.havayolu = havayolu;
        u.yakit = stoi(yakit);
        u.varisZamani = stoll(zaman);

        kule.push(u); // Kuyruğa ekle (Heap yapısı sayesinde otomatik sıralanır)
    }

    dosya.close();
    cout << "[-] Kule: Ucaklar yuklendi ve siralandi." << endl;
}

// Rotaları Graf yapısına (Komşuluk Listesi) yükler
void rotalariYukle(map<string, vector<pair<string, int>>>& graf) {
    ifstream dosya("rotalar.txt");
    string satir;

    if (!dosya.is_open()) return; // Dosya açılamazsa işlem yapmadan çık

    // Rotaları satır satır oku
    while (getline(dosya, satir)) {
        stringstream ss(satir);
        string kaynak, hedef, mesafe;

        // Virgülle ayrılmış güzergah bilgilerini ayrıştır
        getline(ss, kaynak, ',');
        getline(ss, hedef, ',');
        getline(ss, mesafe, ',');

        // String olan mesafeyi int'e çevirip grafın komşuluk listesine ekle
        graf[kaynak].push_back({hedef, stoi(mesafe)});
    }

    dosya.close();
    cout << "[-] Kule: Rotalar graf yapisina islendi." << endl;
}