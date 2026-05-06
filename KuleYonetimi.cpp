#include <iostream>   // cout, cerr
#include <fstream>    // dosya okuma (ifstream)
#include <sstream>    // stringstream (CSV parçalama)
#include <queue>      // priority_queue
#include <map>        // map veri yapısı
#include <vector>     // vector (komşuluk listesi)
#include <set>        // set (öncelikli kuyruk gibi kullanılıyor)
#include "Modeller.h"

using namespace std;

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
        string id, havayolu, yakit, zaman;

        // Virgüle göre ayır
        getline(ss, id, ',');        // uçak ID
        getline(ss, havayolu, ',');  // havayolu adı
        getline(ss, yakit, ',');     // yakıt miktarı
        getline(ss, zaman, ',');     // varış zamanı

        // Eğer veri eksikse bu satırı atla
        if (id.empty() || yakit.empty() || zaman.empty())
            continue;

        // Ucak nesnesi oluştur
        Ucak u;
        u.id = stoi(id);             // string → int
        u.havayolu = havayolu;
        u.yakit = stoi(yakit);
        u.varisZamani = stoll(zaman); // string → long long

        // Priority Queue'ya ekle
        // NOT: Bu yapı otomatik olarak önceliğe göre sıralar (heap)
        kule.push(u);
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
void enKisaYoluBul(map<string, vector<pair<string, int>>>& graf,
                   string baslangic,
                   string hedef) {

    // ---------------------------------------------
    // 1. MESAFE HARİTASI OLUŞTUR
    // ---------------------------------------------
    map<string, int> mesafeler;

    // Tüm şehirler için başlangıçta mesafeyi sonsuz yap
    // (1e9 = ulaşılmamış gibi düşünülür)
    for (auto const& [sehir, _] : graf)
        mesafeler[sehir] = 1e9;

    // Eğer başlangıç şehir graf'ta yoksa hata ver
    if (!graf.count(baslangic)) {
        cout << "[!] Baslangic noktasi graf'ta yok!" << endl;
        return;
    }

    // Başlangıç noktası sıfır
    mesafeler[baslangic] = 0;

    // ---------------------------------------------
    // 2. ÖNCELİKLİ KUYRUK (SET)
    // ---------------------------------------------
    // pair<mesafe, şehir>
    set<pair<int, string>> kuyruk;

    // Başlangıç düğümünü ekle
    kuyruk.insert({0, baslangic});

    // ---------------------------------------------
    // 3. ANA DIJKSTRA DÖNGÜSÜ
    // ---------------------------------------------
    while (!kuyruk.empty()) {

        // En küçük mesafeli elemanı al
        auto it = kuyruk.begin();

        int mevcutMesafe = it->first;
        string u = it->second;

        // Kuyruktan çıkar (işlendi)
        kuyruk.erase(it);

        // Eğer hedefe ulaştıysak çık (optimizasyon)
        if (u == hedef)
            break;

        // -----------------------------------------
        // KOMŞULARI DOLAŞ
        // -----------------------------------------
        for (auto& komsu : graf[u]) {

            string v = komsu.first;   // komşu şehir
            int agirlik = komsu.second; // yol mesafesi

            // -------------------------------------
            // RELAXATION (en kritik adım)
            // -------------------------------------
            // Eğer u üzerinden gitmek daha kısa ise
            if (mevcutMesafe + agirlik < mesafeler[v]) {

                // Eski değeri kuyruktan sil
                kuyruk.erase({mesafeler[v], v});

                // Yeni daha kısa mesafeyi ata
                mesafeler[v] = mevcutMesafe + agirlik;

                // Güncellenmiş değeri kuyruğa ekle
                kuyruk.insert({mesafeler[v], v});
            }
        }
    }

    // ---------------------------------------------
    // 4. SONUÇ
    // ---------------------------------------------
    if (mesafeler[hedef] == 1e9) {
        cout << "[!] Yol bulunamadi!" << endl;
    } else {
        cout << "[#] " << baslangic << " -> " << hedef
             << " en kisa mesafe: "
             << mesafeler[hedef] << " km" << endl;
    }
}
// NOT:
// Dijkstra algoritması negatif ağırlıklı kenarlarda doğru sonuç vermez.
// Bu sistem yalnızca pozitif mesafeler için uygundur.