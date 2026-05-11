#include <iostream>
#include <fstream>       // Dosya okuma/yazma işlemleri (File I/O) için
#include <sstream>       // String parçalama (Parsing) işlemleri için
#include <map>           // Red-Black Tree tabanlı arama yapısı (O(log n))
#include <unordered_map> // Hash Table tabanlı eşleştirme yapısı (O(1))
#include <string>
#include <vector>        // Dinamik diziler (Listeler) için
#include "Modeller.h"

using namespace std;

// ==============================================================================
// 1. SEFERLERİ YÜKLEME (DOSYA OKUMA VE MAP ENTEGRASYONU)
// seferler.txt dosyasındaki virgülle ayrılmış (CSV) veriler okunur.
// Sefer numaraları "Primary Key" kabul edilerek, veriler arama maliyeti
// O(\log n) olan std::map (Red-Black Tree) veri yapısına yüklenir.
// ==============================================================================
void seferleriYukle(map<string, Sefer>& seferSistemi) {
    ifstream dosya("seferler.txt");
    string satir;

    if (!dosya.is_open()) return;

    // Satır satır okuma döngüsü (End of File'a kadar)
    while (getline(dosya, satir)) {
        stringstream ss(satir);
        string seferNo, ucakId, kalkis, varis, zaman;

        // CSV formatındaki satırı ',' (virgül) ayırıcısına (delimiter) göre parçala
        getline(ss, seferNo, ',');
        getline(ss, ucakId, ',');
        getline(ss, kalkis, ',');
        getline(ss, varis, ',');
        getline(ss, zaman, ',');

        Sefer s;
        s.seferNo = seferNo;
        s.ucakId = stoi(ucakId);     // String veriyi Tamsayıya (Integer) çevir
        s.ucusZamani = stoll(zaman); // String veriyi Uzun Tamsayıya (Long Long) çevir

        // [TEST]: Bagaj eşleşmesini garantilemek için geçici bir PNR eklemesi
        s.yolcuPnrListesi.push_back("PNR111");

        // Sefer objesini Map yapısına ekle. Key = Sefer Numarası, Value = Sefer Objesi
        seferSistemi[seferNo] = s;
    }
    dosya.close();
    cout << "[-] Sefer Sistemi: Seferler yuklendi." << endl;
}

// ==============================================================================
// 2. BAGAJLARI YÜKLEME (STACK / YIĞIN MİMARİSİ)
// bagajlar.txt dosyasından okunan kargolar, ait oldukları yolcunun bulunduğu
// uçağın kargo bölümüne yüklenir. Kargo bölümü std::stack (Yığın) yapısındadır.
// Bu fiziksel uçak yüklemesini (LIFO mantığı) bilgisayar ortamında simüle eder.
// ==============================================================================
void bagajlariYukle(map<string, Sefer>& seferSistemi) {
    ifstream dosya("bagajlar.txt");
    string satir;

    if (!dosya.is_open()) return;

    while (getline(dosya, satir)) {
        stringstream ss(satir);
        string bId, pnr, agirlik;

        getline(ss, bId, ',');
        getline(ss, pnr, ',');
        getline(ss, agirlik, ',');

        // Uniform Initialization ile Bagaj Struct'ı oluşturuluyor
        Bagaj b = {
            stoi(bId),
            pnr,
            stof(agirlik)  // String'i Ondalıklı Sayıya (Float) çevir
        };

        // Sistemdeki tüm seferleri (O(N)) ve seferin yolcularını (O(M)) gez
        for (auto& cift : seferSistemi) {
            for (string yPnr : cift.second.yolcuPnrListesi) {
                // Eğer bagajın PNR numarası, bu seferdeki bir yolcuyla eşleşirse
                if (yPnr == pnr) {
                    // Bagajı Stack'in en üstüne (Top) ekle. İşlem maliyeti: O(1)
                    cift.second.kargoBolumu.push(b);
                }
            }
        }
    }
    dosya.close();
    cout << "[-] Kargo Sistemi: Bagajlar ucaklarin yiginina (Stack) yuklendi." << endl;
}

// ==============================================================================
// 3. BAGAJLARI TAHLİYE ETME (LIFO - SON GİREN İLK ÇIKAR) VE HASH TABLE ARAMASI
// Uçak hedefe vardığında Stack yapısındaki kargo boşaltılır.
// Ekrana yolcu ismini yazdırmak için unordered_map (Hash Table) kullanılır.
// ==============================================================================
void bagajlariTahliyeEt(Sefer& secilenSefer, unordered_map<string, Yolcu>& yolcuHaritasi) {
    cout << "\n[LIFO] " << secilenSefer.seferNo << " seferi bagajlari bosaltiliyor..." << endl;

    if (secilenSefer.kargoBolumu.empty()) {
        cout << "(!) Bu seferde bagaj bulunmamaktadir." << endl;
        return;
    }

    int sira = 1;
    // Stack boşalana kadar en üstteki (Top) elemanı al ve sil (Pop)
    while (!secilenSefer.kargoBolumu.empty()) {
        Bagaj b = secilenSefer.kargoBolumu.top();

        string sahibi = "Bilinmiyor";

        // HASH TABLE ARAMASI: unordered_map sayesinde PNR araması O(1) sabit zamanda yapılır.
        // count() fonksiyonu, o anahtarın tabloda olup olmadığını inanılmaz bir hızla kontrol eder.
        if (yolcuHaritasi.count(b.pnr_sahibi)) {
            sahibi = yolcuHaritasi[b.pnr_sahibi].ad + " " + yolcuHaritasi[b.pnr_sahibi].soyad;
        }

        cout << sira << ". Tahliye Edilen Bagaj -> ID: " << b.id
             << " | Sahibi: " << sahibi << " (" << b.pnr_sahibi << ")" << endl;

        // Okunan bagajı uçaktan (Yığından) sil
        secilenSefer.kargoBolumu.pop();
        sira++;
    }
    cout << "[✔] Tum bagajlar teslim noktasina gonderildi." << endl;
}