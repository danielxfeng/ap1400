//
// Created by Daniel X Feng
// Created Date: 1 Dec 2023.
//

#ifndef AP_TRANSACTION_H
#define AP_TRANSACTION_H

#include "server.h"

// This class respects a Transaction.
class Transaction {
public:
    // Default Transaction.
    Transaction() = default;

    // Construct from a trx string.
    Transaction(const std::string &trx);

    // Construct from sender, receiver and value.
    Transaction(const std::string &sender, const std::string &receiver, const double value);

    // Output the instance to a trx string;
    std::string to_string();

    // Return the sender.
    std::string get_sender();

    // Return the receiver.
    std::string get_receiver();

    // Return the value.
    double get_value();

private:
    // The sender of a Transaction.
    std::string sender;
    // The receiver of a Transaction.
    std::string receiver;
    // The value of a Transaction.
    double value{};
};

#endif //AP_TRANSACTION_H
