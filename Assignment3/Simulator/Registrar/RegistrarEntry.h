#pragma once

#include "../../common/GameManagerRegistration.h"
#include "../../common/TankAlgorithmRegistration.h"
#include "../../common/PlayerRegistration.h"

#include <cassert>
#include <string>
// === Exception Struct === //
struct InvalidRegistrationEntryException
{
    std::string reason;
};

// === RegistrarEntries Factory === //
template <typename T>
concept IsFactory = std::is_same_v<TankAlgorithmFactory, T> ||
                    std::is_same_v<PlayerFactory, T> ||
                    std::is_same_v<GameManagerFactory, T>;
template <IsFactory Factory>
class RegistrarEntry
{
private:
    // === Attributes === //
    std::string name;
    Factory factory;

    // === Private Functions === //
    bool validateEntryName()
    {
        return this->name != "";
    };

    bool validateFactory()
    {
        if (factory == nullptr)
        {
            return false;
        }
        return true;
    }

public:
    // === Consturctor === //
    RegistrarEntry(const std::string &name) : name(name), factory(nullptr) {}
    
    // === Destructor === //
    virtual ~RegistrarEntry() = default;

    // === Public Functions === //
    // === Getters === //
    std::string getName() const { return this->name; }
    Factory getFactory() const { return this->factory; }

    // === Setters === //
    virtual void setFactory(const Factory &factory)
    {
        assert(this->factory == nullptr);
        this->factory = factory;
    };

    // === Validation === //
    virtual void validateEntry()
    {
        if (!validateEntryName())
        {
            throw InvalidRegistrationEntryException("Invalid Entry Name");
        }

        else if (!validateFactory())
        {
            throw InvalidRegistrationEntryException("Invalid Factory");
        }
    };
};
