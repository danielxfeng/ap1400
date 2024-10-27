#include <iostream>
#include "espresso_based.h"

EspressoBased::EspressoBased(const EspressoBased &esp) {
    if (!esp.ingredients.empty()) {
        for (auto ingredient : esp.ingredients) {
            ingredients.push_back(ingredient);
        }
    }
    name = esp.name;
}

void EspressoBased::operator=(const EspressoBased &esp) {
    if (this != &esp) {
        ingredients.clear();
        if (!esp.ingredients.empty()) {
            for (auto ingredient : esp.ingredients) {
                ingredients.push_back(ingredient);
            }
        }
        name = esp.name;
    }
}

EspressoBased::~EspressoBased() {
    for(const auto& i : ingredients)
        delete i;
    ingredients.clear();
}

void EspressoBased::brew() {
    std::cout << "Your " << get_name() << " coffee is here, "
              << "the amount is: " << price() << "." << std::endl;
}

std::vector<Ingredient *> &EspressoBased::get_ingredients() {
    return ingredients;
}

