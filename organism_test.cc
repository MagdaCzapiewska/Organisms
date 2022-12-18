#include "organism.h"
#include <cstdint>
#include <cstring>
#include <iostream>

#if TEST_NUM == 101
#include <string>
#endif

using namespace std;

// Pomocnicze deklaracje uĹźywane w testach

class Noncomparable {};

class Species {
    const char* sound; // UĹźywanie std::string w constexpr nie jest wspierane w GCC 10.2.1.
public:
    constexpr Species(const char* _sound) : sound(_sound) {};
    constexpr bool operator==(const Species &) const = default;
    constexpr const char* make_sound() const {return sound;};
};

constexpr char bark[] = "Woof!";
constexpr char meow[] = "Meow!";
constexpr char squeak[] = "Squeak!";
constexpr char silence[] = "...";

constexpr Species dog(bark);
constexpr Species cat(meow);
constexpr Species rabbit(squeak);
constexpr Species lettuce(silence);

using species_id_t = uint8_t;
const species_id_t dog_id = 1;
const species_id_t wolf_id = 2;
const species_id_t pine_id = 3;
const species_id_t elephant_id = 4;

int main() {
#if TEST_NUM == 101
    // To jest przykĹad z treĹci zadania.
    // PrzykĹad uĹźycia funkcji encounter: wilk zjada psa.
    constexpr Omnivore<species_id_t> dog(dog_id, 10);
    constexpr Carnivore<species_id_t> wolf(wolf_id, 100);

    constexpr auto encounter_result = encounter(wolf, dog);
    constexpr auto wolf_result = get<0>(encounter_result);
    constexpr auto dog_result = get<1>(encounter_result);
    constexpr auto child_result = get<2>(encounter_result);

    static_assert(wolf_result.get_vitality() == 105);
    static_assert(dog_result.is_dead());
    static_assert(!child_result.has_value());

    // PrzykĹad uĹźycia encounter_series:
    // wilk spotyka martwego psa (dog_result z przykĹadu powyĹźej â nic nie robi,
    // sosnÄ â nic nie robi, psa, ktĂłrego zjada, i sĹonia , ktĂłry jest zbyt
    // silny, Ĺźeby go zjeĹÄ, zatem nic nie robi.
    constexpr Plant<species_id_t> pine(pine_id, 34);
    constexpr Herbivore<species_id_t> elephant(elephant_id, 500);
    constexpr auto wolf_result_2 = encounter_series(wolf, dog_result, pine, dog, elephant);

    static_assert(!wolf_result_2.is_dead());
    static_assert(wolf_result_2.get_vitality() == 105);

    // Funkcja get_species() powinna zwracaÄ gatunek.
    static_assert(wolf.get_species() == wolf_id);
#endif

#if TEST_NUM == 201
    constexpr Organism<species_id_t, true, true> o(dog_id, 12);
#endif

#if TEST_NUM == 202
    // Podstawowe konstruktory
    constexpr Herbivore<Species> o1(rabbit, 12);
    constexpr Carnivore<Species> o2(cat, 12);
    constexpr Omnivore<Species> o3(dog, 12);
    constexpr Plant<Species> o4(lettuce, 12);
#endif

#if TEST_NUM == 203
    constexpr Herbivore<Species> h(rabbit, 12);
    static_assert(h.get_vitality() == 12);
    constexpr Omnivore<Species> o(rabbit, 13);
    static_assert(o.get_vitality() == 13);
#endif

#if TEST_NUM == 204
    static constexpr int one = 1;
    static constexpr int two = 2;
    constexpr Herbivore<int> h(one, 12);
    static_assert(h.get_species() == one);
    constexpr Omnivore<int> o(two, 13);
    static_assert(o.get_species() == two);
#endif

#if TEST_NUM == 205
    constexpr Herbivore<Species> h(rabbit, 12);
    static_assert(h.get_species() == rabbit);
    static_assert(h.get_species() != dog);
    constexpr Omnivore<Species> o(dog, 13);
    static_assert(o.get_species() == dog);
#endif

#if TEST_NUM == 206
    static_assert(strcmp(cat.make_sound(), "Meow!") == 0);
#endif

#if TEST_NUM == 301
    // Przynajmniej jeden test w kaĹźdej grupie powinien siÄ kompilowaÄ.
#endif

#if TEST_NUM == 302
    // Ma siÄ nie kompilowaÄ: typ Noncomparable nie speĹnia std::equality_comparable.
    static constexpr Noncomparable nc;
    constexpr Herbivore<Noncomparable> h(nc, 12);
#endif

#if TEST_NUM == 303
    // Ma siÄ nie kompilowaÄ: rĂłĹźne typy identyfikatora gatunku.
    constexpr Herbivore<Species> h(rabbit, 1);
    constexpr Herbivore<species_id_t> d(dog_id, 1);
    encounter(h, d);
#endif

#if TEST_NUM == 304
    // Ma siÄ nie kompilowaÄ: prĂłba spotkania ze sobÄ dwĂłch roĹlin.
    constexpr Plant<Species> l1(lettuce, 1);
    constexpr Plant<Species> l2(lettuce, 1);
    encounter(l1, l2);
#endif

#if TEST_NUM == 401
    // Spotkanie umarĹego: nic siÄ nie zmienia.
    constexpr Carnivore<Species> c(cat, 10);
    constexpr Omnivore<Species> d(dog, 0);
    constexpr auto result = encounter(c, d);
    constexpr auto cc = std::get<0>(result);
    static_assert(cc.get_vitality() == 10);
    static_assert(cc.get_species() == cat);
    constexpr auto dd = std::get<1>(result);
    static_assert(dd.get_vitality() == 0);
    static_assert(dd.get_species() == dog);
    static_assert(!std::get<2>(result).has_value());
#endif

#if TEST_NUM == 402
    // KrĂłliki majÄ duĹźÄ witalnoĹÄ.
    constexpr Herbivore<Species> organism1(rabbit, UINT64_MAX - 5);
    constexpr Herbivore<Species> organism2(rabbit, UINT64_MAX);
    constexpr auto result = encounter(organism1, organism2);
    constexpr auto result1 = std::get<0>(result);
    static_assert(result1.get_vitality() == UINT64_MAX - 5);
    static_assert(result1.get_species() == rabbit);
    constexpr auto result2 = std::get<1>(result);
    static_assert(result2.get_vitality() == UINT64_MAX);
    static_assert(result2.get_species() == rabbit);
    static_assert(std::get<2>(result).has_value());
    constexpr auto result3 = *std::get<2>(result);
    static_assert(result3.get_vitality() == UINT64_MAX - 3);
    static_assert(result3.get_species() == rabbit);
#endif

#if TEST_NUM == 403
    // KrĂłliki majÄ duĹźÄ witalnoĹÄ.
    constexpr Herbivore<Species> organism1(rabbit, UINT64_MAX);
    constexpr Herbivore<Species> organism2(rabbit, UINT64_MAX - 5);
    constexpr auto result = encounter(organism1, organism2);
    constexpr auto result1 = std::get<0>(result);
    static_assert(result1.get_vitality() == UINT64_MAX);
    static_assert(result1.get_species() == rabbit);
    constexpr auto result2 = std::get<1>(result);
    static_assert(result2.get_vitality() == UINT64_MAX - 5);
    static_assert(result2.get_species() == rabbit);
    static_assert(std::get<2>(result).has_value());
    constexpr auto result3 = *std::get<2>(result);
    static_assert(result3.get_vitality() == UINT64_MAX - 3);
    static_assert(result3.get_species() == rabbit);
#endif

#if TEST_NUM == 404
    // Sprawdzamy, czy przy tym samym Species rĂłĹźne preferencje Ĺźywieniowe powodujÄ
    // traktowanie jako inny gatunek. KrĂłlik-zabĂłjca powinien zjeĹÄ zwykĹego.
    constexpr Herbivore<Species> organism1(rabbit, 10);
    constexpr Carnivore<Species> organism2(rabbit, 15);
    constexpr auto result = encounter(organism1, organism2);
    constexpr auto result1 = std::get<0>(result);
    static_assert(result1.get_vitality() == 0);
    static_assert(result1.get_species() == rabbit);
    constexpr auto result2 = std::get<1>(result);
    static_assert(result2.get_vitality() == 20);
    static_assert(result2.get_species() == rabbit);
    static_assert(!std::get<2>(result).has_value());
#endif

#if TEST_NUM == 405
    // Sprawdzamy wzajemny brak zdolnoĹci do konsumpcji.
    constexpr Herbivore<Species> organism1(rabbit, 10);
    constexpr Herbivore<Species> organism2(dog, 15);
    constexpr auto result = encounter(organism1, organism2);
    constexpr auto result1 = std::get<0>(result);
    static_assert(result1.get_vitality() == 10);
    static_assert(result1.get_species() == rabbit);
    constexpr auto result2 = std::get<1>(result);
    static_assert(result2.get_vitality() == 15);
    static_assert(result2.get_species() == dog);
    static_assert(!std::get<2>(result).has_value());
#endif

#if TEST_NUM == 501
    // ReguĹa 6: dochodzi do walki.
    constexpr Omnivore<Species> organism1(dog, 10);
    constexpr Carnivore<Species> organism2(cat, 15);
    constexpr auto result = encounter(organism1, organism2);
    constexpr auto result1 = std::get<0>(result);
    static_assert(result1.get_vitality() == 0);
    static_assert(result1.get_species() == dog);
    constexpr auto result2 = std::get<1>(result);
    static_assert(result2.get_vitality() == 20);
    static_assert(result2.get_species() == cat);
    static_assert(!std::get<2>(result).has_value());
#endif

#if TEST_NUM == 502
    // ReguĹa 6b: dochodzi do wyniszczajÄcej walki.
    constexpr Omnivore<Species> organism1(dog, 10);
    constexpr Carnivore<Species> organism2(cat, 10);
    constexpr auto result = encounter(organism1, organism2);
    constexpr auto result1 = std::get<0>(result);
    static_assert(result1.get_vitality() == 0);
    static_assert(result1.get_species() == dog);
    constexpr auto result2 = std::get<1>(result);
    static_assert(result2.get_vitality() == 0);
    static_assert(result2.get_species() == cat);
    static_assert(!std::get<2>(result).has_value());
#endif

#if TEST_NUM == 503
    // ReguĹa 7: roĹlinoĹźerca zjada roĹlinÄ.
    constexpr Plant<Species> organism1(lettuce, 100);
    constexpr Herbivore<Species> organism2(rabbit, 10);
    constexpr auto result = encounter(organism1, organism2);
    constexpr auto result1 = std::get<0>(result);
    static_assert(result1.get_vitality() == 0);
    static_assert(result1.get_species() == lettuce);
    constexpr auto result2 = std::get<1>(result);
    static_assert(result2.get_vitality() == 110);
    static_assert(result2.get_species() == rabbit);
    static_assert(!std::get<2>(result).has_value());
#endif

#if TEST_NUM == 504
    // ReguĹa 7: wszystkoĹźerca zjada roĹlinÄ.
    constexpr Omnivore<Species> organism1(dog, 10);
    constexpr Plant<Species> organism2(lettuce, 1);
    constexpr auto result = encounter(organism1, organism2);
    constexpr auto result1 = std::get<0>(result);
    static_assert(result1.get_vitality() == 11);
    static_assert(result1.get_species() == dog);
    constexpr auto result2 = std::get<1>(result);
    static_assert(result2.get_vitality() == 0);
    static_assert(result2.get_species() == lettuce);
    static_assert(!std::get<2>(result).has_value());
#endif

#if TEST_NUM == 505
    // ReguĹa 8a: wystÄpuje nieudana prĂłba drapieĹźnictwa.
    constexpr Omnivore<Species> organism1(dog, 10);
    constexpr Herbivore<Species> organism2(rabbit, 10);
    constexpr auto result = encounter(organism1, organism2);
    constexpr auto result1 = std::get<0>(result);
    static_assert(result1.get_vitality() == 10);
    static_assert(result1.get_species() == dog);
    constexpr auto result2 = std::get<1>(result);
    static_assert(result2.get_vitality() == 10);
    static_assert(result2.get_species() == rabbit);
    static_assert(!std::get<2>(result).has_value());
#endif

#if TEST_NUM == 506
    // ReguĹa 8b: wystÄpuje udana prĂłba drapieĹźnictwa.
    constexpr Herbivore<Species> organism1(rabbit, 10);
    constexpr Omnivore<Species> organism2(dog, 20);
    constexpr auto result = encounter(organism1, organism2);
    constexpr auto result1 = std::get<0>(result);
    static_assert(result1.get_vitality() == 0);
    static_assert(result1.get_species() == rabbit);
    constexpr auto result2 = std::get<1>(result);
    static_assert(result2.get_vitality() == 25);
    static_assert(result2.get_species() == dog);
    static_assert(!std::get<2>(result).has_value());
#endif

#if TEST_NUM == 601
    constexpr Omnivore<Species> organism(dog, 10);

    constexpr auto result = encounter_series(organism);

    static_assert(result.get_vitality() == 10);
    static_assert(result.get_species() == dog);
#endif

#if TEST_NUM == 602
    constexpr Omnivore<Species> organism(dog, 10);
    constexpr Herbivore<Species> r1(rabbit, 10);
    constexpr Plant<Species> l2(lettuce, 2);
    constexpr Herbivore<Species> r3(rabbit, 10);
    constexpr Omnivore<Species> d4(dog, 10);
    constexpr Herbivore<Species> d5(dog, 2);

    constexpr auto result = encounter_series(organism, r1, l2, r3, d4, d5);

    static_assert(result.get_vitality() == 18);
    static_assert(result.get_species() == dog);
#endif

#if TEST_NUM == 603
    constexpr Omnivore<Species> organism(dog, 10);
    constexpr Herbivore<Species> r1(rabbit, 10);
    constexpr Plant<Species> l2(lettuce, 2);
    constexpr Herbivore<Species> r3(rabbit, 10);
    constexpr Carnivore<Species> c4(cat, 100);
    constexpr Plant<Species> l5(lettuce, 1);
    constexpr Omnivore<Species> d6(dog, 10);

    constexpr auto result = encounter_series(organism, r1, l2, r3, c4, l5, d6);

    static_assert(result.get_vitality() == 0);
    static_assert(result.get_species() == dog);
#endif
}