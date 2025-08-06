#pragma once
#include "RegistrarEntry.h"

#include <vector>
#include <string>
#include <iostream>

template <IsFactory Factory>
class Registrar
{
private:
    std::vector<RegistrarEntry<Factory>> entries;
    static Registrar<Factory> registrar;

    // === Constructor === //
    Registrar() {};

public:
    static Registrar<Factory> &getRegistrar()
    {
        return Registrar<Factory>::registrar;
    };

    // === Create Entries === //
    void createEntry(const std::string &name)
    {
        entries.emplace_back(name);

        // handling duplicate files loading (does not create additional instance so uses same factory)
        for (auto& entry : entries){
            if (entry.getName() == name && entry.getFactory() != nullptr){
                addFactoryToLastEntry(entry.getFactory());
                break;
            }
        }
    }

    // === Add Data To Entries === //
    void addFactoryToLastEntry(const Factory &f)
    {
        entries.back().setFactory(f);
    }

    void validateLastEntry() 
    {
        entries.back().validateEntry();
    }

    // === Deligation From Vector === //
    void deleteLastEntry()
    {
        entries.pop_back();
    }

    auto begin() const
    {
        return entries.begin();
    }

    auto end() const
    {
        return entries.end();
    }

    std::size_t count() const
    {
        return entries.size();
    }

    void clear()
    {
        entries.clear();
    }

    // === Operators === //
    RegistrarEntry< Factory> operator[](std::size_t index)
    {
        return entries[index];
    }
};
template <IsFactory Factory>
Registrar<Factory> Registrar<Factory>::registrar;


// == USE EXAMPLES === //
// {

//     Registrar<GameManagerFactory>::getRegistrar().createEntry("manager1");
//     Registrar<GameManagerFactory>::getRegistrar().addFactoryToLastEntry(GameManagerFactory{});

//     Registrar<TankAlgorithmFactory>::getRegistrar().createEntry("algo1");
//     Registrar<TankAlgorithmFactory>::getRegistrar().addFactoryToLastEntry(TankAlgorithmFactory{});

//     Registrar<PlayerFactory>::getRegistrar().createEntry("player1");
//     Registrar<PlayerFactory>::getRegistrar().addFactoryToLastEntry(PlayerFactory{});
// };
