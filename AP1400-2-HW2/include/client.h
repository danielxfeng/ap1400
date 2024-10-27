//
// Created by Daniel X Feng
// Created Date: 1 Dec 2023.
//

#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include "server.h"

class Server;

// The client class for a simple implementation of simulating what is happening in a cryptocurrency.
// This class use the server to transfer money to each other and most importantly mine transactions to receive rewards.
class Client {
public:
    // Assigning the specified variables using the inputs.
    // Also generates RSA keys for the client (public and private keys).
    Client(std::string id, const Server &server);

    // Returns the client's id.
    std::string get_id();

    // Returns the client's public key.
    std::string get_publickey() const;

    // Returns the amount of money the client has.
    double get_wallet();

    // Returns the amount of available balance the client has.
    double get_avl_wallet();

    // Signs the input with the private key and returns the signature.
    std::string sign(std::string txt) const;

    // Returns the result of creating a Transaction in the server according to its inputs.
    bool transfer_money(std::string receiver, double value);

    // Returns a random number as a nonce so the server uses it for mining.
    size_t generate_nonce();

private:
    // An instance of the server.
    Server const* const server;
    // The client's id.
    const std::string id;
    // The client's public key.
    std::string public_key;
    // The client's private key.
    std::string private_key;
};

#endif //CLIENT_H