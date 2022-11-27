#ifndef __ORGANISM_H__
#define __ORGANISM_H__

#include <concepts>
#include <optional>
#include <type_traits>

/*

 Koniecznie używać typename

 // użyć static_assert do sprawdzenia typów
 // korzystać z <type_traits>
 // if constexpr is evaluated at compile time, whereas if is not - zamiast SFINAE
 // (branches get be rejected at compile time and never be compiled)
 // concepts zamiast enable if

 // używać auto przy templatkach

 */

template <typename species_t, bool can_eat_meat, bool can_eat_plants>
requires std::equality_comparable<species_t>
class Organism {
    // typ species_t określa gatunek, powinien spełniać koncept std::equality_comparable

private:
    species_t species;
    uint64_t vitality;
    // metody umożliwiające eleganckie rozwiązanie zadania

public:
    constexpr Organism(species_t const &species, uint64_t vitality) : species(species), vitality(vitality) {};


    constexpr uint64_t get_vitality() const {
        return vitality;
    }

    constexpr const species_t &get_species() const {
        return species;
    }

    constexpr bool is_dead() const {
        return vitality == 0;
    }

};

template <typename species_t>
using Carnivore = Organism<species_t, true, false>;

template <typename species_t>
using Omnivore = Organism<species_t, true, true>;

template <typename species_t>
using Herbivore = Organism<species_t, false, true>;

template <typename species_t>
using Plant = Organism<species_t, false, false>;

template <typename species_t, bool sp1_eats_m, bool sp1_eats_p, bool sp2_eats_m, bool sp2_eats_p>
constexpr std::tuple<Organism<species_t, sp1_eats_m, sp1_eats_p>,
        Organism<species_t, sp2_eats_m, sp2_eats_p>,
        std::optional<Organism<species_t, sp1_eats_m, sp1_eats_p>>>
encounter(Organism<species_t, sp1_eats_m, sp1_eats_p> organism1,
          Organism<species_t, sp2_eats_m, sp2_eats_p> organism2);

template <typename species_t, bool sp1_eats_m, bool sp1_eats_p, typename ... Args>
constexpr Organism<species_t, sp1_eats_m, sp1_eats_p>
encounter_series(Organism<species_t, sp1_eats_m, sp1_eats_p> organism1, Args ... args);


#endif // __ORGANISM_H__
