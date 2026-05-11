#include <iostream>   // Standart giriş-çıkış işlemleri
#include <fstream>    // Dosya okuma/yazma (ifstream, ofstream)
#include <sstream>    // String parçalama işlemleri (stringstream)
#include <queue>      // Öncelikli kuyruk (priority_queue) kullanımı
#include <map>        // Anahtar-değer eşleşmesi (Graf ve mesafe tutmak için)
#include <vector>     // Dinamik diziler (Komşuluk listesi için)
#include <set>        // Sıralı veri tutma (Dijkstra'da öncelik yönetimi için)
#include "Modeller.h" // Ucak ve diğer özel veri modelleri

using namespace std;


void ucaklariYukle(priority_queue<Ucak>& kule) {
    ifstream dosya("ucaklar.txt"); // Dosya akışını başlat
    string satir;

    // Dosya kontrolü: Dosya yerinde mi veya açılabilir mi?
    if (!dosya.is_open()) {
        cerr << "[!] Hata: ucaklar.txt acilamadi!" << endl;
        return;
    }

    // Dosyayı satır satır dönerek verileri ayrıştır
    while (getline(dosya, satir)) {
        stringstream ss(satir);
        string id, havayolu, yakit, zaman, acilStr;

        // CSV formatında (virgülle ayrılmış) verileri oku
        getline(ss, id, ',');
        getline(ss, havayolu, ',');
        getline(ss, yakit, ',');
        getline(ss, zaman, ',');
        getline(ss, acilStr, ',');

        // Kritik veriler eksikse bu satırı atla
        if (id.empty() || yakit.empty() || zaman.empty()) continue;

        // Okunan string verilerini uygun veri tiplerine dönüştür
        Ucak u;
        u.id = stoi(id);
        u.havayolu = havayolu;
        u.yakit = stoi(yakit);
        u.varisZamani = stoll(zaman);
        u.acilDurum = (acilStr == "1"); // "1" ise uçak acil iniş önceliğine sahip

        kule.push(u); // Uçak nesnesini öncelikli kuyruğa ekle
    }

    dosya.close(); // Dosya erişimini kapat
    cout << "[-] Kule: Ucaklar yuklendi ve siralandi." << endl;
}

/*
  Şehirler arası rotaları 'rotalar.txt' dosyasından okuyarak graf yapısını oluşturur.
  graf: Başlangıç şehri anahtarına karşılık (hedef, mesafe) çiftlerini tutan map.
 */
void rotalariYukle(map<string, vector<pair<string, int>>>& graf) {
    ifstream dosya("rotalar.txt");
    string satir;

    if (!dosya.is_open()) {
        cerr << "[!] Hata: rotalar.txt acilamadi!" << endl;
        return;
    }

    while (getline(dosya, satir)) {
        stringstream ss(satir);
        string kaynak, hedef, mesafe;

        // Virgülle ayrılmış rota verilerini parçala
        getline(ss, kaynak, ',');
        getline(ss, hedef, ',');
        getline(ss, mesafe, ',');

        if (kaynak.empty() || hedef.empty() || mesafe.empty())
            continue;

        // Yönlü graf yapısına ekleme: Kaynak -> {Hedef, Mesafe}
        graf[kaynak].push_back({hedef, stoi(mesafe)});
    }

    dosya.close();
    cout << "[-] Kule: Rotalar graf yapisina islendi." << endl;
}

/* Dijkstra algoritmasını kullanarak iki şehir arasındaki en kısa mesafeyi hesaplar.
 *
  graf: Rota bilgilerini içeren komşuluk listesi.
  baslangic: Rotanın başlayacağı şehir adı.
  hedef: Ulaşılmak istenen şehir adı.
 */
void enKisaYoluBul(map<string, vector<pair<string, int>>>& graf, string baslangic, string hedef) {

    // 1. Girdi Standardizasyonu: Şehir isimlerinin ilk harfini büyük yap (Case Sensitivity önlemi)
    if (!baslangic.empty()) baslangic[0] = toupper(baslangic[0]);
    if (!hedef.empty()) hedef[0] = toupper(hedef[0]);

    // 2. Varlık Kontrolü: Başlangıç şehri sistemde tanımlı mı?
    if (graf.find(baslangic) == graf.end()) {
        cout << "[!] Hata: '" << baslangic << "' sehri rota listesinde yok!" << endl;
        return;
    }

    // 3. Mesafelerin İlklendirilmesi
    map<string, int> mesafeler;
    const int SONSUZ = 1e9; // Ulaşılamaz noktalar için büyük bir değer

    // Tüm düğümleri (kalkış ve varış noktaları) başlangıçta sonsuz kabul et
    for (auto const& [kalkis, komsular] : graf) {
        mesafeler[kalkis] = SONSUZ;
        for (auto& komsu : komsular) {
            mesafeler[komsu.first] = SONSUZ;
        }
    }

    // Hedef şehir graf üzerinde hiç tanımlanmamışsa hata ver
    if (mesafeler.find(hedef) == mesafeler.end()) {
        cout << "[!] Hata: '" << hedef << "' sehri ucus aginda bulunmuyor!" << endl;
        return;
    }

    // Başlangıç noktasının kendine uzaklığı 0'dır
    mesafeler[baslangic] = 0;

    // Dijkstra için öncelikli işlem seti: {Mesafe, Şehir İsmi}
    set<pair<int, string>> kuyruk;
    kuyruk.insert({0, baslangic});

    // 4. Algoritma Döngüsü
    while (!kuyruk.empty()) {
        // En kısa mesafeye sahip düğümü seç (set otomatik sıralar)
        auto it = kuyruk.begin();
        int mevcutMesafe = it->first;
        string u = it->second;
        kuyruk.erase(it);

        // Hedefe ulaştıysak daha fazla aramaya gerek yok
        if (u == hedef) break;

        // Mevcut düğümün komşularını gez
        for (auto& komsu : graf[u]) {
            string v = komsu.first;
            int agirlik = komsu.second;

            // Gevşetme (Relaxation): Daha kısa bir yol bulundu mu?
            if (mevcutMesafe + agirlik < mesafeler[v]) {
                kuyruk.erase({mesafeler[v], v}); // Eski mesafeyi kuyruktan çıkar
                mesafeler[v] = mevcutMesafe + agirlik; // Mesafeyi güncelle
                kuyruk.insert({mesafeler[v], v}); // Yeni mesafeyi kuyruğa ekle
            }
        }
    }

    // 5. Sonuç Yazdırma
    if (mesafeler[hedef] == SONSUZ) {
        cout << "[!] '" << hedef << "' sehrine yol bulunamadi!" << endl;
    } else {
        cout << "[#] " << baslangic << " -> " << hedef << " en kisa mesafe: " << mesafeler[hedef] << " km" << endl;
    }
}