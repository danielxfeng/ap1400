//
// Created by Daniel X Feng
// Created Date: 1 Dec 2023.
//

#include <random>
#include "client.h"
#include "server.h"
#include "crypto.h"
#include "Transaction.h"


Client::Client(std::string id, const Server &server) : id(std::move(id)), server(&server) {
    // Generate key pairs;
    crypto::generate_key(public_key, private_key);
}

std::string Client::get_id() {
    return id;
}

std::string Client::get_publickey() const {
    return public_key;
}

double Client::get_wallet() {
    return const_cast<Server&>(*server).get_wallet(id);
}

double Client::get_avl_wallet() {
    return const_cast<Server&>(*server).get_wallet(id);
}

std::string Client::sign(std::string txt) const {
    return crypto::signMessage(private_key, std::move(txt));
}

// There are 2 steps to transfer money:
// 1. Create Transaction trx and Sign by the private key.
// For example if there is a trx string: "ali-hamed-1.5", means that ali sends 1.5 coins to hamed.
// 2. Submit to server and return the result.
// Use both the signature and Transaction's signature to create a pending Transaction
// in the Server using add_pending_trx function.
bool Client::transfer_money(std::string receiver, double value) {
    // Check if there is enough money and the receiver is valid;
    if (get_avl_wallet() < value || !server->get_client(receiver)) return false;
    Transaction trans{id, receiver, value};
    std::string trx = trans.to_string();
    const_cast<Server&>(*server).add_pending_trx(trx, sign(trx));
    return true;
}

size_t Client::generate_nonce() {
    // Initialize the random number generator
    static std::default_random_engine e(std::random_device{}());
    // Create a random int
    std::uniform_int_distribution<int> u(0, 999999999);
    return u(e);
}