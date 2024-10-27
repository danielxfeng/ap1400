//
// Created by Daniel X Feng
// Created Date: 1 Dec 2023.
//

#include <sstream>
#include <string>
#include "Transaction.h"

// A helper function to split a string.
std::vector<std::string> split(const std::string &str, char delim);

Transaction::Transaction(const std::string &trx) {
    // Split trx by '-'.
    std::vector<std::string> v = split(trx, '-');
    // Throw a runtime error if the vector size is Not 3.
    if (v.size() != 3) {
        throw std::runtime_error("Illegal arguments: " + trx);
    }
    // Assign values to trans.
    sender = v[0];
    receiver = v[1];
    // String to double.
    value = std::stod(v[2]);
    // Check if the values are valid.
    if (value <= 0) {
        throw std::runtime_error("Illegal arguments: " + trx);
    }
}

Transaction::Transaction(const std::string &sender, const std::string &receiver, const double value)
        : Transaction(sender + '-' + receiver + '-' + std::to_string(value)) {}

std::string Transaction::to_string() {
    return sender + '-' + receiver + '-' + std::to_string(value);
}

std::string Transaction::get_sender() {
    return sender;
}

std::string Transaction::get_receiver() {
    return receiver;
}

double Transaction::get_value() {
    return value;
}

std::vector<std::string> split(const std::string &str, char delim) {
    std::istringstream iss(str);
    std::vector<std::string> tokens;
    std::string token;
    while (std::getline(iss, token, delim)) {
        tokens.push_back(token);
    }
    return tokens;
}
