#include <iostream>   // cout, cerr
#include <fstream>    // dosya okuma (ifstream)
#include <sstream>    // stringstream (CSV parçalama)
#include <queue>      // priority_queue
#include <map>        // map veri yapısı
#include <vector>     // vector (komşuluk listesi)
#include <set>        // set (öncelikli kuyruk gibi kullanılıyor)
#include "Modeller.h"

using namespace std;
// KuleYonetimi.cpp içindeki fonksiyonun en başına şu satırları ekle:

// =====================================================
// UÇAKLARI DOSYADAN OKUYUP PRIORITY QUEUE'YA YÜKLER
// =====================================================
void ucaklariYukle(priority_queue<Ucak>& kule) {

    // Dosyayı aç
    ifstream dosya("ucaklar.txt");
    string satir;

    // Dosya açılamazsa hata ver ve çık
    if (!dosya.is_open()) {
        cerr << "[!] Hata: ucaklar.txt acilamadi!" << endl;
        return;
    }

    // Dosyayı satır satır oku
    while (getline(dosya, satir)) {

        // Satırı parçalamak için stringstream kullan
        stringstream ss(satir);

        // CSV alanları
        // CSV alanları
        string id, havayolu, yakit, zaman, acilStr;

        // Virgüle göre ayır
        getline(ss, id, ',');        // uçak ID
        getline(ss, havayolu, ',');  // havayolu adı
        getline(ss, yakit, ',');     // yakıt miktarı
        getline(ss, zaman, ',');     // varış zamanı
        getline(ss, acilStr, ',');   // acil durum (0 veya 1) --- YENİ EKLENDİ

        if (id.empty() || yakit.empty() || zaman.empty()) continue;

        Ucak u;
        u.id = stoi(id);
        u.havayolu = havayolu;
        u.yakit = stoi(yakit);
        u.varisZamani = stoll(zaman);
        u.acilDurum = (acilStr == "1"); // Eğer okunan değer "1" ise true olur --- YENİ EKLENDİ

        kule.push(u); // Kuyruğa ekle
    }


    dosya.close();

    // Bilgi mesajı
    cout << "[-] Kule: Ucaklar yuklendi ve siralandi." << endl;
}




// =====================================================
// ROTALARI GRAF YAPISINA (KOMŞULUK LİSTESİ) YÜKLER
// =====================================================
void rotalariYukle(map<string, vector<pair<string, int>>>& graf) {

    ifstream dosya("rotalar.txt");
    string satir;

    // Dosya açılamazsa hata ver
    if (!dosya.is_open()) {
        cerr << "[!] Hata: rotalar.txt acilamadi!" << endl;
        return;
    }

    // Satır satır oku
    while (getline(dosya, satir)) {

        stringstream ss(satir);

        string kaynak, hedef, mesafe;

        // CSV verisini ayır
        getline(ss, kaynak, ','); // başlangıç şehir
        getline(ss, hedef, ',');  // hedef şehir
        getline(ss, mesafe, ','); // mesafe

        // Eksik veri varsa atla
        if (kaynak.empty() || hedef.empty() || mesafe.empty())
            continue;

        // Graf'a ekle
        // pair<hedef, mesafe>
        graf[kaynak].push_back({hedef, stoi(mesafe)});
    }

    dosya.close();

    cout << "[-] Kule: Rotalar graf yapisina islendi." << endl;
}


// =====================================================
// DIJKSTRA ALGORİTMASI İLE EN KISA YOL HESAPLAMA
// =====================================================
void enKisaYoluBul(map<string, vector<pair<string, int>>>& graf, string baslangic, string hedef) {
    // 1. Girdi Temizliği (Harf Büyütme)
    if (!baslangic.empty()) baslangic[0] = toupper(baslangic[0]);
    if (!hedef.empty()) hedef[0] = toupper(hedef[0]);

    // 2. Graf Kontrolü
    if (graf.find(baslangic) == graf.end()) {
        cout << "[!] Hata: '" << baslangic << "' sehri rota listesinde yok!" << endl;
        return;
    }

    // 3. Mesafe ve Kuyruk Tanımlamaları (Sadece BİR kez yapılmalı)
    map<string, int> mesafeler;
    // --- KuleYonetimi.cpp içindeki enKisaYoluBul fonksiyonu ---

    // 1. Önce tüm şehirleri (kalkış ve varış fark etmeksizin) sonsuz yap
    for (auto const& [kalkis, komsular] : graf) {
        mesafeler[kalkis] = 1e9;
        for (auto& komsu : komsular) {
            mesafeler[komsu.first] = 1e9; // Hedef şehirleri de sonsuz yap!
        }
    }

    // 2. Eğer hedef şehir hala haritada yoksa, o şehir gerçekten uçuş ağında yoktur
    if (mesafeler.find(hedef) == mesafeler.end()) {
        cout << "[!] Hata: '" << hedef << "' sehri ucus aginda bulunmuyor!" << endl;
        return;
    }

    mesafeler[baslangic] = 0; // Başlangıç noktası sıfır [cite: 344]
    mesafeler[baslangic] = 0;
    set<pair<int, string>> kuyruk;
    kuyruk.insert({0, baslangic});

    // 4. Ana Dijkstra Döngüsü
    while (!kuyruk.empty()) {
        auto it = kuyruk.begin();
        int mevcutMesafe = it->first;
        string u = it->second;
        kuyruk.erase(it);

        if (u == hedef) break;

        for (auto& komsu : graf[u]) {
            string v = komsu.first;
            int agirlik = komsu.second;

            if (mevcutMesafe + agirlik < mesafeler[v]) {
                kuyruk.erase({mesafeler[v], v});
                mesafeler[v] = mevcutMesafe + agirlik;
                kuyruk.insert({mesafeler[v], v});
            }
        }
    }

    // 5. Sonuç Yazdırma
    if (mesafeler[hedef] == 1e9) {
        cout << "[!] '" << hedef << "' sehrine yol bulunamadi!" << endl;
    } else {
        cout << "[#] " << baslangic << " -> " << hedef << " en kisa mesafe: " << mesafeler[hedef] << " km" << endl;
    }
} // Fonksiyon burada biter