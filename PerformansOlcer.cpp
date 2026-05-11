#include <string>
#include <chrono>
#include <iostream> // Bunu ekle


// Eğer "std::" yazmakla uğraşmak istemiyorsan bunu da ekleyebilirsin:
using namespace std;

class PerformansOlcer {
    std::chrono::time_point<std::chrono::high_resolution_clock> baslangic;
    string ad; // Artık "using namespace std" dediysen hata vermez
public:
    PerformansOlcer(string islemAd) : ad(islemAd) {
        baslangic = std::chrono::high_resolution_clock::now();
    }
    ~PerformansOlcer() {
        auto bitis = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> sure = bitis - baslangic;
        cout << "\n[PERFORMANS] " << ad << ": " << sure.count() << " ms" << endl;
    }
};