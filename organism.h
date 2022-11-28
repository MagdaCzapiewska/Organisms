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
                    Organism<species_t, sp1_eats_m, sp1_eats_p> (organism1.get_species(), organism1.get_vitality()),
                    Organism<species_t, sp2_eats_m, sp2_eats_p> (organism2.get_species(), organism2.get_vitality()),
                    Organism<species_t, sp1_eats_m, sp2_eats_p>
                            (organism1.get_species(),(organism1.get_vitality() + organism2.get_vitality()) / 2));
        }
    }
    /*if (sp1_eats_m && sp1_eats_p) {
        if (sp2_eats_m) {
            //walcza
        }
        //zjada albo nie zjada
    }
    if (sp1_eats_m && !sp1_eats_p) {
        if (sp2_eats_m) {
            //walcza
        }
        if (sp2_eats_p) {
            //zjada albo nie zjada
        }
        //sp2 jest roslina, wiec sp1 go nie zjada
    }
    if (!sp1_eats_m && sp1_eats_p) {
        if (!sp2_eats_m && !sp2_eats_p) {
            //zjada
        }
        if (sp2_eats_m) {
            //org2 moze zjesc org1
        }
        //oboje jedza tylko rosliny wiec sie nie zjadaja
    }*/
    if (sp1_eats_m && sp2_eats_m) {
        if (organism1.get_vitality() > organism2.get_vitality()) {
            return std::make_tuple(
                    Organism<species_t, sp1_eats_m, sp1_eats_p> (organism1.get_species(), organism1.get_vitality() + organism2.get_vitality() / 2),
                    Organism<species_t, sp2_eats_m, sp2_eats_p> (organism2.get_species(), 0),
                    std::nullopt);
        }
        if (organism2.get_vitality() > organism1.get_vitality()) {
            return std::make_tuple(
                    Organism<species_t, sp1_eats_m, sp1_eats_p> (organism1.get_species(), 0),
                    Organism<species_t, sp2_eats_m, sp2_eats_p> (organism2.get_species(), organism2.get_vitality() + organism1.get_vitality() / 2),
                    std::nullopt);
        }
        else {
            return std::make_tuple(
                    Organism<species_t, sp1_eats_m, sp1_eats_p> (organism1.get_species(), 0),
                    Organism<species_t, sp2_eats_m, sp2_eats_p> (organism2.get_species(), 0),
                    std::nullopt);
        }
    }
    if ((!sp1_eats_p && !sp2_eats_m && !sp2_eats_p) || (!sp2_eats_p && !sp1_eats_m && !sp1_eats_p)) {
        return std::make_tuple(
                Organism<species_t, sp1_eats_m, sp1_eats_p> (organism1.get_species(), organism1.get_vitality()),
                Organism<species_t, sp2_eats_m, sp2_eats_p> (organism2.get_species(), organism2.get_vitality()),
                std::nullopt);
    }
    if (sp1_eats_p && !sp2_eats_m && !sp2_eats_p) {
        return std::make_tuple(
                Organism<species_t, sp1_eats_m, sp1_eats_p> (organism1.get_species(), organism1.get_vitality() + organism2.get_vitality()),
                Organism<species_t, sp2_eats_m, sp2_eats_p> (organism2.get_species(), 0),
                std::nullopt);
    }
    if (sp2_eats_p && !sp1_eats_m && !sp1_eats_p) {
        return std::make_tuple(
                Organism<species_t, sp1_eats_m, sp1_eats_p> (organism1.get_species(), 0),
                Organism<species_t, sp2_eats_m, sp2_eats_p> (organism2.get_species(), organism2.get_vitality() + organism1.get_vitality()),
                std::nullopt);
    }
    if (sp1_eats_m) {
        //a zjada b
    }
    if(sp2_eats_m) {
        //b zjada a
    }
}

template <typename species_t, bool sp1_eats_m, bool sp1_eats_p, typename ... Args>
constexpr Organism<species_t, sp1_eats_m, sp1_eats_p>
encounter_series(Organism<species_t, sp1_eats_m, sp1_eats_p> organism1, Args ... args);


#endif // __ORGANISM_H__
