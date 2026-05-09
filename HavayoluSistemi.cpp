#include "HavayoluSistemi.h"

// ---------------------------------------------------------
// YAPICI METOT (CONSTRUCTOR)
// ---------------------------------------------------------
HavayoluSistemi::HavayoluSistemi() {
    // Başlangıçta alfabetik yolcu ağacı boş olduğu için kökü boşa işaretliyoruz
    yolcuAgaciKoku = nullptr;
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
            if (!tPnr.empty()) s.yolcuPnrListesi.push_back(tPnr); // Dinamik vector'e ekle
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