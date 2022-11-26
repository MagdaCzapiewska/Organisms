#ifndef __ORGANISM_H__
#define __ORGANISM_H__

#include <concepts>
#include <optional>

/*

 Koniecznie używać typename

 template<...>
 using Carnivore = Organism(...);

 using Carnivore = Organism(true, false);
 using Omnivore = Organism(true, true);
 using Herbivore = Organism(false, true);
 using Plant = Organism(false, false);


 */

template <typename species_t, bool can_eat_meat, bool can_eat_plants> class Organism {
    // typ species_t określa gatunek, powinien spełniać koncept std::equality_comparable

private:
    species_t species;
    uint64_t vitality;
    // metody umożliwiające eleganckie rozwiązanie zadania

public:
    Organism(species_t const &species, uint64_t vitality);

    uint64_t get_vitality() {
        return this->vitality;
    }

    const species_t &get_species() {
        return this->species;
    }

    bool is_dead() {
        return (this->vitality == 0);
    }

};

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
