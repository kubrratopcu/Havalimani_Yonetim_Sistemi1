#include "HavayoluSistemi.h"
#include <set>    // Set veri yapısını kullanabilmek için şarttır
#include <cctype> // toupper() fonksiyonu için gereklidir
// ---------------------------------------------------------
// YAPICI METOT (CONSTRUCTOR)
// ---------------------------------------------------------
HavayoluSistemi::HavayoluSistemi() {
    yolcuAgaciKoku = nullptr;

    // VERİLERİ BURADA YÜKLEMELİSİN
    seferleriYukle();
    bagajlariYukle();
    // rotalariYukle(); // Varsa ekle

    cout << "[✔] Havalimani Sistemi: Veriler basariyla yuklendi." << endl;
}

// ---------------------------------------------------------
// YIKICI METOT (DESTRUCTOR)
// ---------------------------------------------------------
HavayoluSistemi::~HavayoluSistemi() {
    // Bellek Sızıntısını Önleme: Program kapanırken BST'yi RAM'den siler
    agaciTemizle(yolcuAgaciKoku);

    // Her seferin içindeki dinamik bağlı listeleri (PNR listeleri) tek tek gez ve sil
    for (auto& cift : seferSistemi) {
        // cift.second ifadesi 'Sefer' nesnesini temsil eder
        seferListesiniTemizle(cift.second.yolcuListesiBasi);
    }
}

// ---------------------------------------------------------
// SEFERLERİ YÜKLEME (Dosya Parçalama ve Map Kaydı)
// ---------------------------------------------------------
void HavayoluSistemi::seferleriYukle() {
    ifstream dosya("seferler.txt"); // Dosyayı okuma modunda aç
    string satir;
    if (!dosya.is_open()) return;   // Dosya yoksa veya bozuksa sessizce çık

    while (getline(dosya, satir)) { // Satır satır oku
        stringstream ss(satir);     // Satırı kelime kelime parçalamak için stringstream'e ver
        string sNo, uId, kalkis, varis, zaman, pnrList;

        // Virgülle ayrılmış CSV formatını değişkenlere ata
        getline(ss, sNo, ',');
        getline(ss, uId, ',');
        getline(ss, kalkis, ',');
        getline(ss, varis, ',');
        getline(ss, zaman, ',');
        getline(ss, pnrList, ',');

        Sefer s;                    // Yeni bir sefer nesnesi oluştur
        s.seferNo = sNo;
        s.ucakId = stoi(uId);       // String'i Integer'a çevir
        s.ucusZamani = stoll(zaman); // String'i Long Long'a (tarih için) çevir

        // PNR Listesini Parçalama (Örn: PNR1;PNR2 formatını ayırır)
        stringstream ssPnr(pnrList);
        string tPnr;
        while (getline(ssPnr, tPnr, ';')) {
            if (!tPnr.empty()){
                s.yolcuPnrListesi.push_back(tPnr); // Dinamik vector'e ekle
                // --- LINKED LIST ENTEGRASYONU (Kritik Adım) ---
                PnrNode* yeni = new PnrNode();
                yeni->pnr = tPnr;
                yeni->next = s.yolcuListesiBasi; // Başa ekleme mantığı (en hızlısı)
                s.yolcuListesiBasi = yeni;            }
        }
        seferSistemi[sNo] = s;      // Map yapısına 'SeferNo' anahtarıyla kaydet (O(log n) hız)
    }
    dosya.close();                  // Kaynakları serbest bırakmak için dosyayı kapat
}

// ---------------------------------------------------------
// BELLEK TEMİZLEME YARDIMCILARI (Recursive & Pointer)
// ---------------------------------------------------------

// İkili Arama Ağacı'nı (BST) bellekten silen fonksiyon
void HavayoluSistemi::agaciTemizle(YolcuNode* kok) {
    if (kok == nullptr) return; // Eğer dal bittiyse dur

    agaciTemizle(kok->sol);     // Önce sol alt dalları sil (Post-order)
    agaciTemizle(kok->sag);     // Sonra sağ alt dalları sil
    delete kok;                 // Düğümün kendisini RAM'den temizle
}

// Bağlı Listeyi (Linked List) düğüm düğüm silen fonksiyon
void HavayoluSistemi::seferListesiniTemizle(PnrNode*& bas) {
    PnrNode* akim = bas;        // Mevcut düğümü tutan işaretçi
    while (akim != nullptr) {   // Listenin sonuna kadar git
        PnrNode* sonraki = akim->next; // Bir sonraki düğümü yedekle
        delete akim;            // Mevcut düğümü sil
        akim = sonraki;         // Yedeklediğin düğüme geç
    }
    bas = nullptr;              // Listenin başını tamamen sıfırla
}

// PNR ile Yolcu Bulma (Hash Table)
Yolcu HavayoluSistemi::pnrIleYolcuBul(string pnr) {
    if (harita.find(pnr) != harita.end()) {
        return harita[pnr]; // O(1) hızında bulur
    }
    return Yolcu(); // Bulunamazsa boş döner
}

// 1. Kule Yönetimi (Priority Queue)
string HavayoluSistemi::siradakiUcagiIndir() {
    if (!kule.empty()) {
        Ucak enOncelikli = kule.top();
        kule.pop();
        // 'ucakKodu' yerine 'id' kullanıyoruz
        return "Ucak ID: " + to_string(enOncelikli.id) + " (" + enOncelikli.havayolu + ")";
    }
    return "Kule Bos";
}
// 1. BAGAJLARI YÜKLEME (Dosyadan Okuma Taslağı)
void HavayoluSistemi::bagajlariYukle() {
    // Şimdilik boş bırakabilirsin, hata vermemesi için gövde olması yeterli.
}

// 2. BAGAJ TAHLİYE (Stack - LIFO Mantığı)
vector<string> HavayoluSistemi::bagajlariTahliyeEt(string seferNo) {
    vector<string> liste;

    // Sefer sisteminde bu uçuş var mı kontrol et
    if (seferSistemi.find(seferNo) != seferSistemi.end()) {
        // Seferin içindeki kargoBolumu (stack) yapısını boşaltıyoruz
        while (!seferSistemi[seferNo].kargoBolumu.empty()) {
            Bagaj b = seferSistemi[seferNo].kargoBolumu.top();
            string bilgi = "Bagaj ID: " + to_string(b.id) + " - Sahibi: " + b.pnr_sahibi;
            liste.push_back(bilgi);
            seferSistemi[seferNo].kargoBolumu.pop();
        }
    }
    return liste;
}

// 3. ROTA HESAPLAMA (Graph/Dijkstra Taslağı)
string HavayoluSistemi::enKisaRota(string kalkis, string varis) {
    if (kalkis == varis) return "Ayni sehri sectiniz.";

    // Girdi temizliği: İlk harfleri büyük yapıyoruz
    if (!kalkis.empty()) kalkis[0] = toupper(kalkis[0]);
    if (!varis.empty()) varis[0] = toupper(varis[0]);

    // Başlangıç şehri graf yapısında var mı kontrol et
    if (graf.find(kalkis) == graf.end()) {
        return "Hata: '" + kalkis + "' sehri rota listesinde yok!";
    }

    // Mesafeleri sonsuz (1e9) olarak başlatıyoruz
    map<string, int> mesafeler;
    for (auto const& [sehir, komsular] : graf) {
        mesafeler[sehir] = 1e9;
        for (auto& komsu : komsular) {
            mesafeler[komsu.first] = 1e9;
        }
    }

    mesafeler[kalkis] = 0;
    set<pair<int, string>> kuyruk; // Min-Priority Queue görevi görür
    kuyruk.insert({0, kalkis});

    while (!kuyruk.empty()) {
        string u = kuyruk.begin()->second;
        int mevcutMesafe = kuyruk.begin()->first;
        kuyruk.erase(kuyruk.begin());

        if (u == varis) break;

        // Komşuları gez (Adjacency List üzerinden)
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

    if (mesafeler[varis] == 1e9) {
        return "Hata: '" + varis + "' sehrine ucus yolu bulunamadi!";
    }

    return kalkis + " -> " + varis + " | En kisa mesafe: " + to_string(mesafeler[varis]) + " km";
}
