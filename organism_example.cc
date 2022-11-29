#include <string>
#include <iostream>
#include "organism.h"

using namespace std;
using species_id_t = uint8_t;

const species_id_t dog_id = 1;
const species_id_t wolf_id = 2;
const species_id_t pine_id = 3;
const species_id_t elephant_id = 4;

const species_id_t wilk_id = 5;

const species_id_t m1 = 1;
const species_id_t m2 = 2;

const species_id_t w1 = 3;
const species_id_t w2 = 4;

const species_id_t r1 = 5;
const species_id_t r2 = 6;

const species_id_t p1 = 7;
const species_id_t p2 = 8;

/*struct X { bool operator== (const X &) = delete; };*/

int main() {

    // Przykład użycia funkcji encounter: wilk zjada psa.
    constexpr Omnivore<species_id_t> dog(dog_id, 10);
    constexpr Carnivore<species_id_t> wolf(wolf_id, 100);
    //constexpr Carnivore<X> wolf(X(), 100); - nie działa to dobrze :)

    static_assert(wolf.get_vitality() == 100);
    static_assert(wolf.is_dead() == false);

    constexpr auto encounter_result = encounter(wolf, dog);
    constexpr auto wolf_result = get<0>(encounter_result);
    constexpr auto dog_result = get<1>(encounter_result);
    constexpr auto child_result = get<2>(encounter_result);

    static_assert(wolf_result.get_vitality() == 105);
    static_assert(dog_result.is_dead());
    static_assert(!child_result.has_value());

    //moje testy

    //te same gatunki
    constexpr Carnivore<species_id_t> wilk(wilk_id, 100);
    constexpr Carnivore<species_id_t> wilczyca(wilk_id, 50);

    constexpr auto gody = encounter(wilk, wilczyca);
    constexpr auto wilk_wynik = get<0>(gody);
    constexpr auto wilczyca_wynik = get<1>(gody);
    constexpr auto wilczek = get<2>(gody);

    static_assert(wilk_wynik.get_vitality() == 100);
    static_assert(wilczyca_wynik.get_vitality() == 50);
    static_assert(wilczek.has_value());
    static_assert(wilczek->get_vitality());

    //te same gatunki ale inne preferencje
    constexpr Herbivore<species_id_t> wilk_wege(wilk_id, 50);

    constexpr auto wege = encounter(wilk, wilk_wege);
    constexpr auto wege_wygrany = get<0>(wege);
    constexpr auto wege_zjedozny = get<1>(wege);
    constexpr auto wege_child = get<2>(wege);

    static_assert(wege_wygrany.get_vitality() == 125);
    static_assert(wege_zjedozny.is_dead());
    static_assert(!wege_child.has_value());


    constexpr Plant<species_id_t> roslina1(p1, 100);
    constexpr Plant<species_id_t> roslina2(p2, 200);

    constexpr Omnivore<species_id_t> wszystko1(w1, 100);
    constexpr Omnivore<species_id_t> wszystko2(w2, 200);
    constexpr Omnivore<species_id_t> wszystko3(w2, 100);

    constexpr Carnivore<species_id_t> mieso1(m1, 100);
    constexpr Carnivore<species_id_t> mieso2(m2, 200);
    constexpr Carnivore<species_id_t> mieso3(m2, 100);

    constexpr Herbivore<species_id_t> wege1(r1, 100);
    constexpr Herbivore<species_id_t> wege2(r2, 200);
    constexpr Herbivore<species_id_t> wege3(r2, 100);

    // miesozerca roslina
    {
        constexpr auto mr = encounter(mieso1, roslina1);
        constexpr auto mr_result1 = get<0>(mr);
        constexpr auto mr_result2 = get<1>(mr);
        constexpr auto mr_result3 = get<2>(mr);

        static_assert(mr_result1.get_vitality() == mieso1.get_vitality());
        static_assert(mr_result2.get_vitality() == roslina1.get_vitality());
        static_assert(!mr_result3.has_value());
    }

    {
        constexpr auto mr = encounter(mieso2, roslina1);
        constexpr auto mr_result1 = get<0>(mr);
        constexpr auto mr_result2 = get<1>(mr);
        constexpr auto mr_result3 = get<2>(mr);

        static_assert(mr_result1.get_vitality() == mieso2.get_vitality());
        static_assert(mr_result2.get_vitality() == roslina1.get_vitality());
        static_assert(!mr_result3.has_value());
    }

    {
        constexpr auto mr = encounter(mieso1, roslina2);
        constexpr auto mr_result1 = get<0>(mr);
        constexpr auto mr_result2 = get<1>(mr);
        constexpr auto mr_result3 = get<2>(mr);

        static_assert(mr_result1.get_vitality() == mieso1.get_vitality());
        static_assert(mr_result2.get_vitality() == roslina2.get_vitality());
        static_assert(!mr_result3.has_value());
    }

    // roslinozerca miesozerca
    {
        constexpr auto mr = encounter(wege1, mieso2);
        constexpr auto mr_result1 = get<0>(mr);
        constexpr auto mr_result2 = get<1>(mr);
        constexpr auto mr_result3 = get<2>(mr);

        static_assert(mr_result1.is_dead());
        static_assert(mr_result2.get_vitality() == 250);
        static_assert(!mr_result3.has_value());
    }

    {
        constexpr auto mr = encounter(mieso2, wege1);
        constexpr auto mr_result1 = get<0>(mr);
        constexpr auto mr_result2 = get<1>(mr);
        constexpr auto mr_result3 = get<2>(mr);

        static_assert(mr_result1.get_vitality() == 250);
        static_assert(mr_result2.is_dead());
        static_assert(!mr_result3.has_value());
    }

    {
        constexpr auto mr = encounter(wege1, mieso1);
        constexpr auto mr_result1 = get<0>(mr);
        constexpr auto mr_result2 = get<1>(mr);
        constexpr auto mr_result3 = get<2>(mr);

        static_assert(mr_result1.get_vitality() == wege1.get_vitality());
        static_assert(mr_result2.get_vitality() == mieso1.get_vitality());
        static_assert(!mr_result3.has_value());
    }

    // miesozerca miesozerca
    // pierwszy wygrywa
    {
        constexpr auto mm = encounter(mieso2, mieso1);
        constexpr auto mm_result1 = get<0>(mm);
        constexpr auto mm_result2 = get<1>(mm);
        constexpr auto mm_result3 = get<2>(mm);

        static_assert(mm_result1.get_vitality() == 250);
        static_assert(mm_result2.is_dead());
        static_assert(!mm_result3.has_value());
    }
    // drugi wygrywa
    {
        constexpr auto mm = encounter(mieso1, mieso2);
        constexpr auto mm_result1 = get<0>(mm);
        constexpr auto mm_result2 = get<1>(mm);
        constexpr auto mm_result3 = get<2>(mm);

        static_assert(mm_result1.is_dead());
        static_assert(mm_result2.get_vitality() == 250);
        static_assert(!mm_result3.has_value());
    }

    // wyniszczająca walka
    {
        constexpr auto mm = encounter(mieso1, mieso3);
        constexpr auto mm_result1 = get<0>(mm);
        constexpr auto mm_result2 = get<1>(mm);
        constexpr auto mm_result3 = get<2>(mm);

        static_assert(mm_result1.is_dead());
        static_assert(mm_result2.is_dead());
        static_assert(!mm_result3.has_value());
    }

    // roslinozerca roslinozerca
    // pierwszy ma wiecej
    {
        constexpr auto rr = encounter(wege2, wege1);
        constexpr auto rr_result1 = get<0>(rr);
        constexpr auto rr_result2 = get<1>(rr);
        constexpr auto rr_result3 = get<2>(rr);

        static_assert(rr_result1.get_vitality() == wege2.get_vitality());
        static_assert(rr_result2.get_vitality() == wege1.get_vitality());
        static_assert(!rr_result3.has_value());
    }

    // drugi ma wiecej
    {
        constexpr auto rr = encounter(wege1, wege2);
        constexpr auto rr_result1 = get<0>(rr);
        constexpr auto rr_result2 = get<1>(rr);
        constexpr auto rr_result3 = get<2>(rr);

        static_assert(rr_result1.get_vitality() == wege1.get_vitality());
        static_assert(rr_result2.get_vitality() == wege2.get_vitality());
        static_assert(!rr_result3.has_value());
    }

    // mają tyle samo
    {
        constexpr auto rr = encounter(wege1, wege3);
        constexpr auto rr_result1 = get<0>(rr);
        constexpr auto rr_result2 = get<1>(rr);
        constexpr auto rr_result3 = get<2>(rr);

        static_assert(rr_result1.get_vitality() == wege1.get_vitality());
        static_assert(rr_result2.get_vitality() == wege3.get_vitality());
        static_assert(!rr_result3.has_value());
    }

    // roslinozerca roslina
    {
        constexpr auto rp = encounter(wege1, roslina2);
        constexpr auto rp_result1 = get<0>(rp);
        constexpr auto rp_result2 = get<1>(rp);
        constexpr auto rp_result3 = get<2>(rp);

        static_assert(rp_result1.get_vitality() == 300);
        static_assert(rp_result2.is_dead());
        static_assert(!rp_result3.has_value());
    }

    {
        constexpr auto rp = encounter(roslina2, wege1);
        constexpr auto rp_result1 = get<0>(rp);
        constexpr auto rp_result2 = get<1>(rp);
        constexpr auto rp_result3 = get<2>(rp);

        static_assert(rp_result1.is_dead());
        static_assert(rp_result2.get_vitality() == 300);
        static_assert(!rp_result3.has_value());
    }
    
    // wszystkozerca wszystkozerca
    {
        constexpr auto ww = encounter(wszystko2, wszystko1);
        constexpr auto ww_result1 = get<0>(ww);
        constexpr auto ww_result2 = get<1>(ww);
        constexpr auto ww_result3 = get<2>(ww);

        static_assert(ww_result1.get_vitality() == 250);
        static_assert(ww_result2.is_dead());
        static_assert(!ww_result3.has_value());
    }
    
    // drugi wygrywa
    {
        constexpr auto ww = encounter(wszystko1, wszystko2);
        constexpr auto ww_result1 = get<0>(ww);
        constexpr auto ww_result2 = get<1>(ww);
        constexpr auto ww_result3 = get<2>(ww);

        static_assert(ww_result1.is_dead());
        static_assert(ww_result2.get_vitality() == 250);
        static_assert(!ww_result3.has_value());
    }
    
    // wyniszczająca walka
    {
        constexpr auto ww = encounter(wszystko1, wszystko3);
        constexpr auto ww_result1 = get<0>(ww);
        constexpr auto ww_result2 = get<1>(ww);
        constexpr auto ww_result3 = get<2>(ww);

        static_assert(ww_result1.is_dead());
        static_assert(ww_result2.is_dead());
        static_assert(!ww_result3.has_value());
    }

    // wszystkozerca miesozerca
    {
        constexpr auto wm = encounter(wszystko2, mieso1);
        constexpr auto wm_result1 = get<0>(wm);
        constexpr auto wm_result2 = get<1>(wm);
        constexpr auto wm_result3 = get<2>(wm);

        static_assert(wm_result1.get_vitality() == 250);
        static_assert(wm_result2.is_dead());
        static_assert(!wm_result3.has_value());
    }

    // drugi wygrywa
    {
        constexpr auto wm = encounter(mieso1, wszystko2);
        constexpr auto wm_result1 = get<0>(wm);
        constexpr auto wm_result2 = get<1>(wm);
        constexpr auto wm_result3 = get<2>(wm);

        static_assert(wm_result1.is_dead());
        static_assert(wm_result2.get_vitality() == 250);
        static_assert(!wm_result3.has_value());
    }

    // wyniszczająca walka
    {
        constexpr auto wm = encounter(mieso1, wszystko3);
        constexpr auto wm_result1 = get<0>(wm);
        constexpr auto wm_result2 = get<1>(wm);
        constexpr auto wm_result3 = get<2>(wm);

        static_assert(wm_result1.is_dead());
        static_assert(wm_result2.is_dead());
        static_assert(!wm_result3.has_value());
    }

    // miesozerca wszystkozerca
    {
        constexpr auto wm = encounter(mieso2, wszystko1);
        constexpr auto wm_result1 = get<0>(wm);
        constexpr auto wm_result2 = get<1>(wm);
        constexpr auto wm_result3 = get<2>(wm);

        static_assert(wm_result1.get_vitality() == 250);
        static_assert(wm_result2.is_dead());
        static_assert(!wm_result3.has_value());
    }

    // drugi wygrywa
    {
        constexpr auto wm = encounter(wszystko1, mieso2);
        constexpr auto wm_result1 = get<0>(wm);
        constexpr auto wm_result2 = get<1>(wm);
        constexpr auto wm_result3 = get<2>(wm);

        static_assert(wm_result1.is_dead());
        static_assert(wm_result2.get_vitality() == 250);
        static_assert(!wm_result3.has_value());
    }

    // wyniszczająca walka
    {
        constexpr auto wm = encounter(wszystko3, mieso1);
        constexpr auto wm_result1 = get<0>(wm);
        constexpr auto wm_result2 = get<1>(wm);
        constexpr auto wm_result3 = get<2>(wm);

        static_assert(wm_result1.is_dead());
        static_assert(wm_result2.is_dead());
        static_assert(!wm_result3.has_value());
    }

    // wszystkozerca roslina
    {
        constexpr auto wm = encounter(wszystko1, roslina2);
        constexpr auto wm_result1 = get<0>(wm);
        constexpr auto wm_result2 = get<1>(wm);
        constexpr auto wm_result3 = get<2>(wm);

        static_assert(wm_result1.get_vitality() == 300);
        static_assert(wm_result2.is_dead());
        static_assert(!wm_result3.has_value());
    }

    {
        constexpr auto wm = encounter(roslina2, wszystko1);
        constexpr auto wm_result1 = get<0>(wm);
        constexpr auto wm_result2 = get<1>(wm);
        constexpr auto wm_result3 = get<2>(wm);

        static_assert(wm_result1.is_dead());
        static_assert(wm_result2.get_vitality() == 300);
        static_assert(!wm_result3.has_value());
    }

    // wszystkozerca roslinozerca
    // pierwszy wygrywa
    {
        constexpr auto wm = encounter(wszystko2, wege1);
        constexpr auto wm_result1 = get<0>(wm);
        constexpr auto wm_result2 = get<1>(wm);
        constexpr auto wm_result3 = get<2>(wm);

        static_assert(wm_result1.get_vitality() == 250);
        static_assert(wm_result2.is_dead());
        static_assert(!wm_result3.has_value());
    }

    // drugi wygrywa
    {
        constexpr auto wm = encounter(wege1, wszystko2);
        constexpr auto wm_result1 = get<0>(wm);
        constexpr auto wm_result2 = get<1>(wm);
        constexpr auto wm_result3 = get<2>(wm);

        static_assert(wm_result1.is_dead());
        static_assert(wm_result2.get_vitality() == 250);
        static_assert(!wm_result3.has_value());
    }

    // rowno
    {
        constexpr auto wm = encounter(wege1, wszystko3);
        constexpr auto wm_result1 = get<0>(wm);
        constexpr auto wm_result2 = get<1>(wm);
        constexpr auto wm_result3 = get<2>(wm);

        static_assert(wm_result1.get_vitality() == wege1.get_vitality());
        static_assert(wm_result2.get_vitality() == wszystko3.get_vitality());
        static_assert(!wm_result3.has_value());
    }
    // rowno w druga strone
    {
        constexpr auto wm = encounter(wszystko3, wege1);
        constexpr auto wm_result1 = get<0>(wm);
        constexpr auto wm_result2 = get<1>(wm);
        constexpr auto wm_result3 = get<2>(wm);

        static_assert(wm_result1.get_vitality() == wszystko3.get_vitality());
        static_assert(wm_result2.get_vitality() == wege1.get_vitality());
        static_assert(!wm_result3.has_value());
    }


    // Przykład użycia encounter_series:
    // wilk spotyka martwego psa (dog_result z przykładu powyżej – nic nie robi,
    // sosnę – nic nie robi, psa, którego zjada, i słonia , który jest zbyt
    // silny, żeby go zjeść, zatem nic nie robi.
    //constexpr Plant<species_id_t> pine(pine_id, 34);
    //constexpr Herbivore<species_id_t> elephant(elephant_id, 500);
    //constexpr auto wolf_result_2 = encounter_series(wolf, dog_result, pine, dog, elephant);

    //static_assert(!wolf_result_2.is_dead());
    //static_assert(wolf_result_2.get_vitality() == 105);

    // Funkcja get_species() powinna zwracać gatunek.
    //static_assert(wolf.get_species() == wolf_id);
}
