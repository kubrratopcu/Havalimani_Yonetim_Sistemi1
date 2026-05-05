#include <unordered_map>
#include <iostream>

using namespace std;

// (Not: Yolcu yapısının (struct/class) bu kod bloğundan önce
// tanımlandığı varsayılmıştır. İçerisinde 'ad', 'soyad' ve 'pnr' olmalıdır.)

// --- Ağaç Düğümü (Node) Yapısı ---
// Ağaç veri yapısındaki her bir elemanı (düğümü) temsil eden yapı.
struct YolcuNode {
    Yolcu veri;         // Düğümün içinde tutulan asıl veri (Yolcu nesnesi)
    YolcuNode *sol;     // Alfabetik olarak daha "küçük" (önce gelen) düğümü işaret eden gösterici (pointer)
    YolcuNode *sag;     // Alfabetik olarak daha "büyük" (sonra gelen) düğümü işaret eden gösterici (pointer)

    // Kurucu (Constructor) Fonksiyon: Yeni bir düğüm oluşturulduğunda ilk değerlerini atar.
    // 'sol' ve 'sag' başlangıçta nullptr (boş) olarak ayarlanır çünkü yeni eklenen bir düğümün henüz çocukları yoktur (o bir yapraktır).
    YolcuNode(Yolcu y) : veri(y), sol(nullptr), sag(nullptr) {}
};

// --- BST'ye (İkili Arama Ağacına) Alfabetik Ekleme Fonksiyonu ---
// Bu fonksiyon özyinelemeli (recursive) çalışır. Doğru yeri bulana kadar ağacın dallarında aşağı iner.
YolcuNode* agacaYolcuEkle(YolcuNode* kok, Yolcu y) {
    // 1. Temel Durum (Base Case):
    // Eğer bulunduğumuz düğüm boşsa (nullptr), aradığımız boş yeri bulduk demektir.
    // Yeni düğümü burada oluşturup geri döndürüyoruz.
    if (kok == nullptr) return new YolcuNode(y);

    // 2. Sola Gitme Durumu:
    // Eklenmek istenen yolcunun adı, mevcut düğümdeki yolcunun adından alfabetik olarak önce geliyorsa.
    // (Örn: "Ahmet" < "Mehmet")
    if (y.ad < kok->veri.ad)
        // Sol alt ağaca git ve ekleme işlemini orada tekrarla.
        // Dönen yeni adresi mevcut düğümün sol çocuğu olarak güncelle.
        kok->sol = agacaYolcuEkle(kok->sol, y);

    // 3. Sağa Gitme Durumu:
    // Eklenmek istenen yolcunun adı alfabetik olarak sonra geliyorsa veya aynıysa.
    // (Örn: "Zeynep" > "Mehmet")
    else
        // Sağ alt ağaca git ve işlemi tekrarla.
        kok->sag = agacaYolcuEkle(kok->sag, y);

    // Ağacın yapısı bozulmasın diye, üzerinde işlem yaptığımız düğümün kendisini (kökünü) geri döndürüyoruz.
    return kok;
}

// --- Ağacı Alfabetik Yazdıran Fonksiyon (In-order Traversal) ---
// İkili arama ağaçlarında verileri küçükten büyüğe (alfabetik) sıralı almak için
// "In-order" (Sol Alt Ağaç -> Kök -> Sağ Alt Ağaç) gezinme yöntemi kullanılır.
void yolculariListele(YolcuNode* kok) {
    // Temel Durum: Eğer düğüm boşsa geri dön (fonksiyondan çık).
    // Bu aynı zamanda ağacın o dalının sonuna geldiğimizi belirtir.
    if (kok == nullptr) return;

    // 1. Adım: Önce sol alt ağacı (alfabetik olarak en küçük olanları) ziyaret et.
    yolculariListele(kok->sol);

    // 2. Adım: Mevcut düğümdeki veriyi (kökü) ekrana yazdır.
    // Sol taraf tamamen yazdırıldıktan sonra sıra ortadaki elemana gelir.
    cout << kok->veri.ad << " " << kok->veri.soyad << " - PNR: " << kok->veri.pnr << endl;

    // 3. Adım: Son olarak sağ alt ağacı (alfabetik olarak daha büyük olanları) ziyaret et.
    yolculariListele(kok->sag);
}