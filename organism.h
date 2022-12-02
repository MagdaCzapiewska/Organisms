#ifndef __ORGANISM_H__
#define __ORGANISM_H__

#include <concepts>
#include <optional>
#include <type_traits>
#include <tuple>

template<typename species_t, bool can_eat_meat, bool can_eat_plants>
requires std::equality_comparable<species_t>
class Organism {
private:
    const species_t species;
    const uint64_t vitality;

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

    template<bool o_can_eat_meat, bool o_can_eat_plants>
    constexpr bool can_eat(Organism <species_t, o_can_eat_meat, o_can_eat_plants> const o) const {
        if (o.is_plant()) {
            return can_eat_plants;
        }
        return can_eat_meat;
    }

    template<bool o_can_eat_meat, bool o_can_eat_plants>
    constexpr Organism<species_t, can_eat_meat, can_eat_plants>
    eat(Organism<species_t, o_can_eat_meat, o_can_eat_plants> const o) const {
        if (can_eat(o)) {
            if (o.is_plant()) {
                return Organism<species_t, can_eat_meat, can_eat_plants>(species, vitality + o.get_vitality());
            }
            if (vitality > o.get_vitality()) {
                return Organism<species_t, can_eat_meat, can_eat_plants>(species, vitality + o.get_vitality() / 2);
            }
        }
        if (o.can_eat(*this)) {
            if (is_plant()) {
                return Organism<species_t, can_eat_meat, can_eat_plants>(species, 0);
            }
            if (o.get_vitality() > vitality || (can_eat(o) && o.get_vitality() == vitality)) {
                return Organism<species_t, can_eat_meat, can_eat_plants>(species, 0);
            }
        }
        return *this;
    }

    template<bool o_can_eat_meat, bool o_can_eat_plants>
    constexpr Organism<species_t, can_eat_meat, can_eat_plants>
    breed(Organism<species_t, o_can_eat_meat, o_can_eat_plants> const o) const {
        return Organism<species_t, can_eat_meat, can_eat_plants> (species, (vitality + o.get_vitality()) / 2);
    }

    template<bool o_can_eat_meat, bool o_can_eat_plants>
    constexpr Organism<species_t, can_eat_meat, can_eat_plants>
    operator+(Organism<species_t, o_can_eat_meat, o_can_eat_plants> const o) const {
        return get<0>(encounter(*this, o));
    }
};

template<typename species_t>
using Carnivore = Organism<species_t, true, false>;

template<typename species_t>
using Omnivore = Organism<species_t, true, true>;

template<typename species_t>
using Herbivore = Organism<species_t, false, true>;

template<typename species_t>
using Plant = Organism<species_t, false, false>;

template<typename species_t, bool o1_eats_m, bool o1_eats_p, bool o2_eats_m, bool o2_eats_p>
constexpr std::tuple<Organism<species_t, o1_eats_m, o1_eats_p>,
                     Organism<species_t, o2_eats_m, o2_eats_p>,
                     std::optional<Organism<species_t, o1_eats_m, o1_eats_p>>>
encounter(Organism<species_t, o1_eats_m, o1_eats_p> organism1, Organism<species_t, o2_eats_m, o2_eats_p> organism2) {
    static_assert(!(organism1.is_plant() && organism2.is_plant()), "plants can't meet - they don't move");
    if (organism1.is_dead() || organism2.is_dead()) {
        return {organism1, organism2, std::nullopt};
    }
    if (organism1.get_species() == organism2.get_species()) {
        if (o1_eats_m == o2_eats_m && o1_eats_p == o2_eats_p) {
            return {organism1, organism2, organism1.breed(organism2)};
        }
    }
    return {organism1.eat(organism2), organism2.eat(organism1), std::nullopt};
}

template<typename species_t, bool o1_eats_m, bool o1_eats_p, typename ... Args>
constexpr Organism<species_t, o1_eats_m, o1_eats_p>
encounter_series(Organism<species_t, o1_eats_m, o1_eats_p> organism1, Args ... args) {
    return (organism1 + ... + args);
}

#endif // __ORGANISM_H__