#ifndef __ORGANISM_H__
#define __ORGANISM_H__

#include <concepts>
#include <optional>
#include <type_traits>
#include <tuple>

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
    const species_t &species;
    const uint64_t vitality;


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

    constexpr bool is_plant() const {
        return !can_eat_meat && !can_eat_plants;
    }

    template <typename species_t2, bool can_eat_meat2, bool can_eat_plants2>
    constexpr bool can_eat(Organism <species_t2, can_eat_meat2, can_eat_plants2> o) const {
        if (o.is_plant()) {
            return can_eat_plants;
        }
        return can_eat_meat;
    }

    template <typename species_t2, bool can_eat_meat2, bool can_eat_plants2>
    constexpr Organism<species_t, can_eat_meat, can_eat_plants> eat(Organism <species_t2, can_eat_meat2, can_eat_plants2> o) const {
        if (can_eat(o)) {
            if (vitality > o.get_vitality()) {
                return Organism<species_t, can_eat_meat, can_eat_plants>
                        (species, vitality + o.get_vitality() / (o.is_plant() ? 1 : 2));
            }
        }
        if (o.can_eat(*this)) {
            return Organism<species_t, can_eat_meat, can_eat_plants>
                    (species, (o.get_vitality() >= vitality) ? 0 : vitality);
        }
        return *this;
    }

   constexpr Organism breed(Organism o) const {
        return Organism<species_t, can_eat_meat, can_eat_plants> (species, (vitality + o.get_vitality()) / 2);
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
          Organism<species_t, sp2_eats_m, sp2_eats_p> organism2) {
    static_assert(!(!sp1_eats_m && !sp1_eats_p && !sp2_eats_m && !sp2_eats_p), "rosliny nie moga sie spotykac");
    if (organism1.get_vitality() == 0 || organism2.get_vitality() == 0) {
        return std::make_tuple(
                Organism<species_t, sp1_eats_m, sp1_eats_p> (organism1.get_species(), organism1.get_vitality()),
                Organism<species_t, sp2_eats_m, sp2_eats_p> (organism2.get_species(), organism2.get_vitality()),
                std::nullopt);
    }
    if (organism1.get_species() == organism2.get_species()) {
        if (sp1_eats_m == sp2_eats_m && sp1_eats_p == sp2_eats_p) {
            return std::make_tuple(
                    Organism<species_t, sp1_eats_m, sp1_eats_p> (organism1.get_species(), organism1.get_vitality() + 5),
                    Organism<species_t, sp2_eats_m, sp2_eats_p> (organism2.get_species(), organism2.get_vitality()),
                    std::nullopt);
        }
    }
    //return std::make_tuple(organism1, organism2, organism1);
    return std::make_tuple(organism1.eat(organism2), organism2.eat(organism1), std::nullopt);
}

template <typename species_t, bool sp1_eats_m, bool sp1_eats_p, typename ... Args>
constexpr Organism<species_t, sp1_eats_m, sp1_eats_p>
encounter_series(Organism<species_t, sp1_eats_m, sp1_eats_p> organism1, Args ... args);


#endif // __ORGANISM_H__
