#include<string>
#include "cappuccino.h"
#include "sub_ingredients.h"

Cappuccino::Cappuccino() {
    name = "Cappuccino";
    ingredients.push_back(new Espresso(2));
    ingredients.push_back(new Milk(2));
    ingredients.push_back(new MilkFoam(1));
}

Cappuccino::Cappuccino(const Cappuccino &cap) {
    name = "Cappuccino";
    ingredients.push_back(new Espresso(2));
    ingredients.push_back(new Milk(2));
    ingredients.push_back(new MilkFoam(1));
    if (this != &cap) {
        for(const auto& i : side_items)
            delete i;
        side_items.clear();
        for (auto ing : cap.side_items) {
            side_items.push_back(ing->clone());
        }
    }
}

Cappuccino::~Cappuccino() {
    for(const auto& i : side_items)
        delete i;
    side_items.clear();
}

void Cappuccino::operator=(const Cappuccino &cap) {
    if (this != &cap) {
        for(const auto& i : side_items)
            delete i;
        side_items.clear();
        for (auto ing : cap.side_items) {
            side_items.push_back(ing->clone());
        }
    }
}

std::string Cappuccino::get_name() {
    return name;
}

double Cappuccino::price() {
    double price = 0;
    for (auto ing : ingredients) {
        price += ing->price();
    }
    for (auto ing : side_items) {
        price += ing->price();
    }
    return price;
}

void Cappuccino::add_side_item(Ingredient *side) {
    side_items.push_back(side);
}

std::vector<Ingredient *> &Cappuccino::get_side_items() {
    return side_items;
}
