#include "HavayoluSistemi.h"
#include <set>
#include <cctype>

void ucaklariYukle(std::priority_queue<Ucak>& kule);

// Sistem ayağa kalktığında çalışır. Tüm veri yapılarını başlatır ve txt dosyalarını okur.
HavayoluSistemi::HavayoluSistemi() {
    yolcuAgaciKoku = nullptr;

    // Kuledeki öncelikli kuyruğu (Priority Queue) uçaklarla dolduruyoruz
    ucaklariYukle(kuleYonetimi.getKuyruk());

    // Diğer tüm sistem verilerini txt dosyalarından RAM'e (veri yapılarına) çekiyoruz.
    yolcuAgaciKoku = nullptr;
    yolculariYukle();
    seferleriYukle();
    bagajlariYukle();
    rotalariYukle();

    cout << "Havalimani Sistemi: Veriler basariyla yuklendi." << endl;
}


// DESTRUCTOR (YIKICI METOT)
// Program kapandığında RAM'de çöp (Memory Leak) kalmaması için dinamik bellekleri temizler.

HavayoluSistemi::~HavayoluSistemi() {
    agaciTemizle(yolcuAgaciKoku); // İkili arama ağacını (BST) temizle

    // Seferlerin içindeki dinamik bağlı listeleri (Linked List) temizle
    for (auto& cift : seferSistemi) {
        seferListesiniTemizle(cift.second.yolcuListesiBasi);
    }
}


// SEFER YÜKLEME (DOSYA OKUMA VE BAĞLI LİSTE - LINKED LIST OLUŞTURMA)
void HavayoluSistemi::seferleriYukle() {
    ifstream dosya("seferler.txt");
    string satir;
    if (!dosya.is_open()) {
        cout<<"dosya açılamadı!!";
    }

    while (getline(dosya, satir)) {
        stringstream ss(satir);
        string sNo, uId, kalkis, varis, zaman, pnrList;

        // Virgülle ayrılmış verileri parçala
        getline(ss, sNo, ',');
        getline(ss, uId, ',');
        getline(ss, kalkis, ',');
        getline(ss, varis, ',');
        getline(ss, zaman, ',');
        getline(ss, pnrList, ',');

        Sefer s;
        s.seferNo = sNo;
        s.ucakId = stoi(uId);
        s.kalkisSehri = kalkis;
        s.varisSehri = varis;
        s.ucusZamani = stoll(zaman);

        // Yolcuların PNR numaralarını noktalı virgülle ayırıp Bağlı Listeye (Linked List) ekle
        stringstream ssPnr(pnrList);
        string tPnr;
        while (getline(ssPnr, tPnr, ';')) {
            if (!tPnr.empty()){
                s.yolcuPnrListesi.push_back(tPnr);

                // Başa ekleme mantığı ile yeni düğüm oluşturuluyor
                PnrNode* yeni = new PnrNode();
                yeni->pnr = tPnr;
                yeni->next = s.yolcuListesiBasi;
                s.yolcuListesiBasi = yeni;
            }
        }
        // Hazırlanan seferi haritaya (Map) ekle
        seferSistemi[sNo] = s;
    }
    dosya.close();
}


// Ağacın düğümlerini bellekten güvenli bir şekilde silmek için özyineli fonksiyon.
void HavayoluSistemi::agaciTemizle(YolcuNode* kok) {
    if (kok == nullptr) return;
    agaciTemizle(kok->sol);
    agaciTemizle(kok->sag);
    delete kok;
}


// BAĞLI LİSTE TEMİZLEME
void HavayoluSistemi::seferListesiniTemizle(PnrNode*& bas) {
    PnrNode* akim = bas;
    while (akim != nullptr) {
        PnrNode* sonraki = akim->next;
        delete akim;
        akim = sonraki;
    }
    bas = nullptr;
}


// ==============================================================================
// YOLCU ARAMA (STD::MAP - RED-BLACK TREE MANTIĞI İLE HIZLI ERİŞİM)
// Manuel İkili Arama Ağacı (BST) yerine C++ STL içindeki std::map kullanılmıştır.
// Neden?: std::map arka planda kendi kendini dengeleyen (Self-Balancing) bir
// Red-Black Tree (Kırmızı-Siyah Ağaç) veri yapısı kullanır.
// Bu mühendislik tercihi sayesinde, sisteme on binlerce yolcu eklense bile
// ağacın tek tarafa yığılması (Unbalanced Tree) engellenir ve arama işlemi
// her zaman en kötü senaryoda bile garanti edilen O(\log n) hızında gerçekleşir.
// ==============================================================================
Yolcu HavayoluSistemi::pnrIleYolcuBul(string pnr) {
    // Haritada (Red-Black Tree düğümlerinde) PNR aranıyor.
    // Bulunursa yolcu nesnesini, bulunamazsa boş obje döndürür.
    if (harita.find(pnr) != harita.end()) return harita[pnr];
    return Yolcu();
}

// KULE KONTROL (PRIORITY QUEUE - MAX HEAP MANTIĞI)

std::string HavayoluSistemi::siradakiUcagiIndir() {
    if (kuleYonetimi.bosMu()) return "Kuyruk Bos";

    // Öncelikli kuyruktan acil durumu en yüksek veya yakıtı en az olan uçağı çekiyoruz
    Ucak enOncelikli = kuleYonetimi.enOncelikliyiGetir();
    kuleYonetimi.pop(); // Uçağı kuyruktan sil (İndi kabul et)

    return to_string(enOncelikli.id) + " (" + enOncelikli.havayolu + ")";
}

// BAGAJ YÜKLEME (STACK - YIĞIN VERİ YAPISI)
// Uçak kargo ambarlarının fiziksel yapısı gereği, bagajlar "LIFO"
// (Last In First Out - Son Giren İlk Çıkar) mantığı ile yüklenir ve boşaltılır.
// Bu fiziksel kısıtlamayı simüle etmek için C++ STL içindeki std::stack kullanılmıştır.

void HavayoluSistemi::bagajlariYukle() {
    ifstream dosya("bagajlar.txt");
    string satir;
    if (!dosya.is_open()) return;

    while (getline(dosya, satir)) {
        stringstream ss(satir);
        string bId, pnr, agirlik;
        getline(ss, bId, ',');
        getline(ss, pnr, ',');
        getline(ss, agirlik, ',');

        if(bId.empty() || pnr.empty()) continue;

        Bagaj b;
        b.id = stoi(bId);
        b.pnr_sahibi = pnr;
        b.agirlik = stof(agirlik);

        // Seferler ve yolcular üzerinde O(N*M) karmaşıklığında arama yapıyoruz.
        for (auto& cift : seferSistemi) {
            for (string yPnr : cift.second.yolcuPnrListesi) {
                if (yPnr == pnr) {
                    // PNR eşleştiğinde bagaj ilgili seferin kargosuna Push edilir.
                    // Push İşlemi: Zaman karmaşıklığı O(1)'dir. Bagaj yığının en üstüne (Top) yerleşir.
                    cift.second.kargoBolumu.push(b);
                }
            }
        }
    }
    dosya.close();
}

// BGAJ TAHLİYE (STACK - LIFO MANTIĞI)
// Yığın yapısı gereği son giren bagaj ilk çıkar (LIFO - Last In First Out).
vector<string> HavayoluSistemi::bagajlariTahliyeEt(string seferNo) {
    vector<string> liste;
    if (seferSistemi.find(seferNo) == seferSistemi.end()) return liste;

    Sefer& secilenSefer = seferSistemi[seferNo];
    int sira = 1;

    // Stack boşalana kadar en üstteki (top) elemanı alıp çıkarıyoruz (pop)
    while (!secilenSefer.kargoBolumu.empty()) {
        Bagaj b = secilenSefer.kargoBolumu.top();
        string bilgi = to_string(sira) + ". Bagaj -> ID: " + to_string(b.id) + " (PNR: " + b.pnr_sahibi + ")";
        liste.push_back(bilgi);
        secilenSefer.kargoBolumu.pop();
        sira++;
    }
    return liste;
}


// EN KISA ROTA BULMA (DIJKSTRA ALGORİTMASI)
// Graflar (Ağaçlar) üzerinde en kısa yolu bulmak için Kenar Gevşetme yapar.
string HavayoluSistemi::enKisaRota(string kalkis, string varis) {
    if (kalkis == varis) return "Ayni sehri sectiniz.";

    map<string, int> mesafeler;
    map<string, string> ebeveyn; // Rotayı geriye dönük izleyebilmek için

    // BÜTÜN şehirleri algoritmaya sonsuz (1e9) mesafe ile tanıtıyoruz
    for (auto const& [sehir, komsular] : graf) {
        mesafeler[sehir] = 1e9;
        ebeveyn[sehir] = "";
        for (auto const& komsu : komsular) {
            mesafeler[komsu.first] = 1e9;
            ebeveyn[komsu.first] = "";
        }
    }

    // Kalkış şehri grafımızda yoksa işlem yapma
    if (mesafeler.find(kalkis) == mesafeler.end()) return "Rota bulunamadi.";

    // Başlangıç noktasının mesafesi 0'dır. Sürekli minimumu bulmak için Set (Min-Heap benzeri) kullanıyoruz.
    mesafeler[kalkis] = 0;
    set<pair<int, string>> kuyruk;
    kuyruk.insert({0, kalkis});

    while (!kuyruk.empty()) {
        string u = kuyruk.begin()->second;
        int d = kuyruk.begin()->first;
        kuyruk.erase(kuyruk.begin()); // En kısa mesafeli düğümü kuyruktan çıkar

        if (u == varis) break; // Hedefe ulaştıysak algoritmayı erken bitir (Optimizasyon)

        // Düğümün tüm komşularını gez ve kenar gevşetme (Relaxation) yap
        for (auto& komsu : graf[u]) {
            if (d + komsu.second < mesafeler[komsu.first]) {
                kuyruk.erase({mesafeler[komsu.first], komsu.first});
                mesafeler[komsu.first] = d + komsu.second;
                ebeveyn[komsu.first] = u;
                kuyruk.insert({mesafeler[komsu.first], komsu.first});
            }
        }
    }

    // Hedef şehre ulaşılamamışsa (mesafe hala sonsuzsa)
    if (mesafeler.find(varis) == mesafeler.end() || mesafeler[varis] >= 1e9) {
        return "Rota bulunamadi.";
    }

    // Ebeveyn haritasını kullanarak hedef şehirden geriye doğru rotayı oluştur
    string yol = "";
    for (string s = varis; s != ""; s = ebeveyn[s]) {
        yol = s + (yol == "" ? "" : " -> " + yol);
    }
    return "En Kisa Rota: " + yol + "\nMesafe: " + to_string(mesafeler[varis]) + " km";
}


// YOLCU BİLGİLERİNİ YÜKLEME VE MAP'E ATAMA

void HavayoluSistemi::yolculariYukle() {
    ifstream dosya("yolcular.txt");
    string satir;
    if (!dosya.is_open()) return;

    while (getline(dosya, satir)) {
        stringstream ss(satir);
        string ad, soyad, pnr, koltuk;
        getline(ss, ad, ',');
        getline(ss, soyad, ',');
        getline(ss, pnr, ',');
        getline(ss, koltuk, ',');

        harita[pnr] = {ad, soyad, pnr, koltuk};
    }
    dosya.close();
}


// PNR İLE UÇUŞ SEFER BİLGİSİ SORGULAMA

string HavayoluSistemi::yolcununUcusBilgisiniGetir(string pnr) {
    for (auto const& cift : seferSistemi) {
        for (string yPnr : cift.second.yolcuPnrListesi) {
            if (yPnr == pnr) return " | Sefer: " + cift.second.seferNo;
        }
    }
    return " | Sefer: Bulunamadi";
}

// GRAF (ADJACENCY LIST) OLUŞTURMA VE ROTALARI YÜKLEME
// Dijkstra algoritmasının kullanacağı çift yönlü (undirected) graf altyapısını kurar.

void HavayoluSistemi::rotalariYukle() {
    ifstream dosya("rotalar.txt");
    string satir;
    if (!dosya.is_open()) return;

    while (getline(dosya, satir)) {
        // Windows işletim sistemlerindeki görünmez satır sonu (\r) karakterini temizle
        if (!satir.empty() && satir.back() == '\r') {
            satir.pop_back();
        }

        stringstream ss(satir);
        string k, h, m;
        getline(ss, k, ',');
        getline(ss, h, ',');
        getline(ss, m, ',');

        // 1. Şehir isimlerinin başındaki ve sonundaki boşlukları trimle (Tıraşlama)
        if(!k.empty()) {
            k.erase(0, k.find_first_not_of(" \t"));
            k.erase(k.find_last_not_of(" \t") + 1);
        }
        if(!h.empty()) {
            h.erase(0, h.find_first_not_of(" \t"));
            h.erase(h.find_last_not_of(" \t") + 1);
        }

        // 2. Mesafenin içindeki tüm sayı dışı boşlukları temizle
        string temizM = "";
        for (char c : m) {
            if (!isspace(c)) {
                temizM += c;
            }
        }
        m = temizM;

        // 3. Adjacency List (Komşuluk Listesi) içine düğümleri (Node) ÇİFT YÖNLÜ ekle
        if (!k.empty() && !h.empty() && !m.empty()) {
            int mesafe = stoi(m);
            graf[k].push_back({h, mesafe}); // Kalkıştan -> Varışa
            graf[h].push_back({k, mesafe}); // Varıştan -> Kalkışa (Dönüş rotası)
        }
    }
    dosya.close();
}