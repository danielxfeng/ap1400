//
// Created by Daniel X Feng
// Created Date: 1 Dec 2023.
//

#ifndef SERVER_H
#define SERVER_H

#include <memory>
#include <string>
#include <map>
#include <atomic>
#include <mutex>
#include <vector>
#include "client.h"

// The pending transactions.
extern std::vector<std::string> pending_trxs;

class Client;

// The parameter of a thread;
struct ThreadWorkerParameters {
    std::atomic<bool> &finished;
    std::map<size_t, std::shared_ptr<Client>> &nonce_map;
    std::mutex &mtx;
    std::size_t &winner_nonce;
};

// The Server class for a simple implementation of simulating what is happening in a cryptocurrency.
// A centralized server to keep track of the clients and transactions.
class Server {
public:
    Server() = default;

    // Create a new Client with the given id.
    // Add a random 4 digital number at the end when there is a duplicated id.
    // Each client should be assigned with 5 coins at the beginning.
    std::shared_ptr<Client> add_client(std::string id);

    // Get a pointer to a Client using its id.
    std::shared_ptr<Client> get_client(std::string id) const;

    // Return the wallet value of the client with username id.
    double get_wallet(std::string id);

    // Return the available wallet value of the client with username id.
    double get_available_balance(std::string id);

    // Return each property separately by parsing this string format and outputting,
    // and throw a runtime error when the format is error.
    // For example if there is a trx string: "ali-hamed-1.5", means that ali sends 1.5 coins to hamed.
    static bool parse_trx(std::string trx, std::string &sender, std::string &receiver, double &value);

    // Return result of adding a pending Transaction.
    // Return true when 1 authenticated sender's signature, and 2 enough money in sender's wallet.
    bool add_pending_trx(std::string trx, std::string signature);

    // Return the nouce of successful mine, and take effect of all the successful transactions.
    size_t mine();

private:
    // Map of: client : amount of wallet.
    std::map<std::shared_ptr<Client>, double> clients;
    // Map of client ids: id : client.
    std::map<std::string, std::shared_ptr<Client>> client_ids;
    // Map of: client : available balance of wallet.
    std::map<std::shared_ptr<Client>, double> clients_available_bal;

    // Allow function show_wallets to visit the private property clients.
    friend void show_wallets(const Server& server);

    // A helper method for method mine to mine:
    size_t mine_helper(const std::string &mempool, std::map<size_t, std::shared_ptr<Client>> &nonce_map);

    // A helper method for method mine to effective transactions.
    void effective_transactions();
};

// Print all clients of a server.
void show_wallets(const  Server& server);

#endif //SERVER_H