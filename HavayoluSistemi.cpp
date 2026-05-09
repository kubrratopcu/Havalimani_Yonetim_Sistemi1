#include "HavayoluSistemi.h"

// ==========================================
// CONSTRUCTOR (YAPICI METOT)
// ==========================================
// Arayüz ayağa kalktığında veya sistem nesnesi oluşturulduğunda ilk burası çalışır.
HavayoluSistemi::HavayoluSistemi() {
    // Başlangıçta yolcu ağacımız boş olduğu için kök düğümünü nullptr yapıyoruz.
    // İleride txt dosyalarından veri okuma fonksiyonlarını da buraya ekleyeceğiz.
    yolcuAgaciKoku = nullptr;
}

// ==========================================
// DESTRUCTOR (YIKICI METOT)
// ==========================================
// Program kapandığında arka planda otomatik olarak tetiklenir.
// C++'ta "new" ile oluşturulan her şey "delete" ile silinmezse RAM'de asılı kalır (Memory Leak).
HavayoluSistemi::~HavayoluSistemi() {
    // 1. Ağaçtaki tüm yolcu düğümlerini RAM'den siler
    agaciTemizle(yolcuAgaciKoku);

    // 2. Map içindeki tüm seferleri tek tek gezer (auto& referans ile)
    for (auto& cift : seferSistemi) {
        // Her bir seferin içindeki PNR bağlı listesini (Linked List) temizler
        seferListesiniTemizle(cift.second.yolcuListesiBasi);
    }
}

// ==========================================
// BELLEK TEMİZLEME ALGORİTMALARI
// ==========================================

// İkili arama ağacını (BST) Post-Order (Sol-Sağ-Kök) mantığıyla gezip yok eder.
// Rekürsif (kendi kendini çağıran) bir fonksiyondur.
void HavayoluSistemi::agaciTemizle(YolcuNode* kok) {
    if (kok == nullptr) return; // Dalın sonuna geldiysek geri dön

    agaciTemizle(kok->sol);     // Önce sol dalı temizle
    agaciTemizle(kok->sag);     // Sonra sağ dalı temizle
    delete kok;                 // En son düğümün kendisini bellekten sil
}

// Bir sefere ait yolcuların PNR numaralarını tutan Bağlı Listeyi (Linked List) temizler.
void HavayoluSistemi::seferListesiniTemizle(PnrNode*& bas) {
    PnrNode* akim = bas; // Listede gezinmek için geçici işaretçi (pointer)

    // Liste sonuna (nullptr) gelene kadar döngüyü çalıştır
    while (akim != nullptr) {
        PnrNode* sonraki = akim->next; // Bir sonraki düğümü kaybetmemek için yedekte tut
        delete akim;                   // Mevcut düğümü RAM'den sil
        akim = sonraki;                // Bir sonraki düğüme geç
    }
    bas = nullptr; // Listenin başını güvenliğe almak için sıfırla
}