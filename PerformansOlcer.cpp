#include <string>
#include <chrono>   // Zaman ölçümü için gerekli kütüphane
#include <iostream> // Standart giriş/çıkış (cout) için

using namespace std;

// PerformansOlcer
 // Bir işlemin başlangıç ve bitiş zamanı arasındaki farkı ölçer.

 // Bu sınıf Nesne Yönelimli Programlama'daki 'Destructor' (Yıkıcı Metot) mantığını kullanır.
 // Nesne oluşturulduğunda süre başlar, nesne kapsam dışına çıktığında (scope bittiğinde)
 // süreyi hesaplayıp ekrana yazdırır.

class PerformansOlcer {
    // İşlemin başladığı anı yüksek çözünürlükte tutar
    std::chrono::time_point<std::chrono::high_resolution_clock> baslangic;
    string ad; // Ölçülen işlemin adı (Örn: "Dijkstra Algoritması")

public:

     //  Constructor: Nesne oluşturulduğu an kronometreyi başlatır.
     //  islemAd: Performans raporunda görünecek başlık.

    PerformansOlcer(string islemAd) : ad(islemAd) {
        baslangic = std::chrono::high_resolution_clock::now();
    }


     // @brief Destructor: Nesne yok edildiği an süreyi durdurur ve milisaniye cinsinden yazdırır.

    ~PerformansOlcer() {
        // Bitiş zamanını al
        auto bitis = std::chrono::high_resolution_clock::now();

        // Bitiş ve başlangıç arasındaki farkı hesapla (milisaniye cinsinden)
        std::chrono::duration<double, std::milli> sure = bitis - baslangic;

        // Sonucu formatlı bir şekilde ekrana bas
        cout << "\n[PERFORMANS] " << ad << ": " << sure.count() << " ms" << endl;
    }
};