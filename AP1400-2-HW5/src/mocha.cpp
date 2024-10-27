#include<string>
#include "mocha.h"
#include "sub_ingredients.h"

Mocha::Mocha() : Cappuccino() {
    name = "Mocha";
    ingredients.push_back(new Chocolate(1));
}

Mocha::~Mocha() {
    for(const auto& i : side_items)
        delete i;
    side_items.clear();
}
