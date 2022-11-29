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
