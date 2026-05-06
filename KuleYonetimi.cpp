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
#include <set>   // set veri yapısı (öncelikli kuyruk gibi kullanılacak)

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

    // Tüm şehirler için başlangıçta mesafeyi "sonsuz" yap
    // (1e9 ≈ çok büyük bir sayı → ulaşılmamış anlamında)
    for (auto const& [sehir, _] : graf)
        mesafeler[sehir] = 1e9;

    // Başlangıç noktasının mesafesi 0'dır
    mesafeler[baslangic] = 0;

    // ---------------------------------------------
    // 2. ÖNCELİKLİ KUYRUK (SET) OLUŞTUR
    // ---------------------------------------------
    // pair<int, string> → (mesafe, şehir)
    // set → otomatik olarak küçükten büyüğe sıralar
    set<pair<int, string>> kuyruk;

    // Başlangıç düğümünü kuyruğa ekle
    kuyruk.insert({0, baslangic});

    // ---------------------------------------------
    // 3. ANA DÖNGÜ (DIJKSTRA)
    // ---------------------------------------------
    while (!kuyruk.empty()) {

        // En küçük mesafeli düğümü al
        // (set'in başı her zaman en küçük değerdir)
        string u = kuyruk.begin()->second;

        // Kuyruktan çıkar (işlendi artık)
        kuyruk.erase(kuyruk.begin());

        // -----------------------------------------
        // KOMŞULARI GEZ
        // -----------------------------------------
        // graf[u] → u şehrinin komşuları
        for (auto& komsu : graf[u]) {

            // komsu.first → komşu şehir (v)
            // komsu.second → o yola ait ağırlık (mesafe)
            string v = komsu.first;
            int agirlik = komsu.second;

            // -------------------------------------
            // RELAXATION (EN KRİTİK KISIM 💥)
            // -------------------------------------
            // Eğer u üzerinden gitmek daha kısa ise
            if (mesafeler[u] + agirlik < mesafeler[v]) {

                // Eski değeri kuyruktan sil
                // (set'te güncelleme yok → silip tekrar ekliyoruz)
                kuyruk.erase({mesafeler[v], v});

                // Yeni daha kısa mesafeyi ata
                mesafeler[v] = mesafeler[u] + agirlik;

                // Güncellenmiş değeri tekrar kuyruğa ekle
                kuyruk.insert({mesafeler[v], v});
            }
        }
    }

    // ---------------------------------------------
    // 4. SONUÇ YAZDIR
    // ---------------------------------------------
    cout << "[#] " << baslangic << " -> " << hedef
         << " en kisa mesafe: " << mesafeler[hedef]
         << " km" << endl;
}