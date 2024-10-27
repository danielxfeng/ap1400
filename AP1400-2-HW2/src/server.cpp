//
// Created by Daniel X Feng
// Created Date: 1 Dec 2023.
//

#include <random>
#include <thread>
#include "server.h"
#include "crypto.h"
#include "Transaction.h"

std::vector<std::string> pending_trxs;

// A helper function to return 4 random digits string.
std::string get_random_digits();

// A helper function to join a string vector.
std::string join_vector(const std::vector<std::string> &v);

// A helper function to determine if there is triple 0 in the firs 10 number of given hash value.
bool containsTripleZero(const std::string &hash);

// The thread worker function for method mine_helper.
void mine_worker(const std::shared_ptr<Client> &client, const std::string &mempool,
                 ThreadWorkerParameters &parameters);

std::shared_ptr<Client> Server::add_client(std::string id) {
    const double INIT_BALANCE = 5.0;
    // Get ptr by id.
    auto ptr = get_client(id);
    // There is a duplicated id, add 4 random digits at the end of id.
    if (ptr) {
        id += get_random_digits();
        return add_client(id);
    }
    // Add a new client.
    std::shared_ptr<Client> client = std::make_shared<Client>(id, *this);
    // Insert into client_ids
    client_ids[id] = client;
    // Insert into clients, and apply the rule: Each client should be assigned with 5 coins at the beginning.
    clients[client] = INIT_BALANCE;
    clients_available_bal[client] = INIT_BALANCE;
    return client;
}

std::shared_ptr<Client> Server::get_client(std::string id) const {
    auto iter = client_ids.find(id);
    // Return nullptr when not exist.
    if (iter == client_ids.end()) return nullptr;
    // Return point when found.
    return (*iter).second;
}

double Server::get_wallet(std::string id) {
    // Get ptr by id.
    auto ptr = get_client(id);
    // Throw error when there is not a client with the given id.
    if (!ptr) throw std::runtime_error("There is no client with the id: " + id);
    return clients[ptr];
}

double Server::get_available_balance(std::string id) {
    // Get ptr by id.
    auto ptr = get_client(id);
    // Throw error when there is not a client with the given id.
    if (!ptr) throw std::runtime_error("There is no client with the id: " + id);
    return clients_available_bal[ptr];
}

bool Server::parse_trx(std::string trx, std::string &sender, std::string &receiver, double &value) {
    Transaction trans{trx};
    sender = trans.get_sender();
    receiver = trans.get_receiver();
    value = trans.get_value();
    return true;
}

bool Server::add_pending_trx(std::string trx, std::string signature) {
    Transaction trans{trx};
    std::string sender = trans.get_sender();
    std::shared_ptr<Client> sender_ptr = get_client(sender);
    // Check if the sender's wallet has enough money.
    bool isEnoughMoney = get_available_balance(sender) >= trans.get_value();
    // Check the signature is valid.
    bool isAuthenticated = crypto::verifySignature(sender_ptr->get_publickey(),
                                                   trx, std::move(signature));
    // Return false when shortage of balance of unauthenticated.
    if (!isEnoughMoney || !isAuthenticated) {
        return false;
    }
    // Add the trx to pending trxs after checking.
    pending_trxs.push_back(std::move(trx));
    clients_available_bal[sender_ptr] -= trans.get_value();
    return true;
}

// There are 3 steps of a mine.
// 1. Generate the mempool.
// Generate a string called mempool by concentrated pending transactions.
// For example: "ali-hamed-1.5", "mhmd-maryam-2.25" -> "ali-hamed-1.5mhmd-maryam-2.25".
// 2. Mine.
// Asks each Client for a number called nonce, added to mempool, then calculates the sha256 of the final mempool.
// If the mine is successful: for each nonce if the generated sha256 has 3 zeros in a row in the first 10 numbers,
// the client who called the correct nonce will be awarded with 6.25 coins.
// 3. Effect the transactions.
// All the transactions will be removed from pending and the effect of them will be applied on the clients
// after a successful mine.
size_t Server::mine() {
    const double AWARD = 6.25;
    // Generate the mempool.
    std::string mempool = join_vector(pending_trxs);
    // Mine.
    // Add a map: nonce : client
    std::map<size_t, std::shared_ptr<Client>> nonce_map;
    size_t winner_nonce = mine_helper(mempool, nonce_map);
    std::shared_ptr<Client> winner_client = nonce_map[winner_nonce];
    // Award the winner
    clients[winner_client] += AWARD;
    // Effective all transactions.
    effective_transactions();
    return winner_nonce;
}

size_t Server::mine_helper(const std::string &mempool, std::map<size_t, std::shared_ptr<Client>> &nonce_map) {
    // Define the symbol of finished.
    std::atomic<bool> finished = false;
    // Define a lock.
    std::mutex mtx;
    // Define the winner nonce;
    std::size_t winner_nonce;
    // Define and start the threads;
    std::vector<std::thread> threads;
    for (auto &pair: clients) {
        // Build the parameters for the thread worker.
        ThreadWorkerParameters tw{finished, nonce_map, mtx, winner_nonce};
        // Start threads.
        threads.emplace_back([client = pair.first, &mempool, &tw]() {
            mine_worker(client, mempool, tw);
        });
    }
    // Wait for all threads end.
    for (std::thread &t: threads) {
        t.join();
    }
    return winner_nonce;
}

void Server::effective_transactions() {
    // Iterator all pending transactions.
    for (const std::string &trx: pending_trxs) {
        Transaction trans{trx};
        double value = trans.get_value();
        // Debit from sender's account.
        clients[client_ids[trans.get_sender()]] -= value;
        // Credit to receiver's account.
        clients[client_ids[trans.get_receiver()]] += value;
    }
    // Delete all items in the pending_trxs vector.
    pending_trxs.clear();
}

std::string get_random_digits() {
    // Initialize the random number generator
    static std::default_random_engine e(std::random_device{}());
    // Create a distribution for digits (0-9)
    std::uniform_int_distribution<int> u(0, 9);
    std::string res;
    for (size_t i = 0; i < 4; i++) {
        // Generate a random digit and add it to the string
        res += std::to_string(u(e));
    }
    return res;
}

std::string join_vector(const std::vector<std::string> &v) {
    std::string result;
    for (const auto &str: v) {
        result += str;
    }
    return result;
}

bool containsTripleZero(const std::string &hash) {
    // Cut the hash.
    std::string str = hash.substr(0, 10);
    return str.find("000") != std::string::npos;
}

void mine_worker(const std::shared_ptr<Client> &client, const std::string &mempool,
                 ThreadWorkerParameters &parameters) {
    while (!parameters.finished) {
        // Asks each Client for a number called nonce.
        std::size_t nonce = client->generate_nonce();
        // write to nonce_map
        if (nonce) {
            std::lock_guard<std::mutex> lock(parameters.mtx);
            parameters.nonce_map[nonce] = client;
        }
        // Calculates the sha256 of the final mempool.
        std::string hash = crypto::sha256(mempool + std::to_string(nonce));
        // Check if there is 3 of 0 in the first 10 numbers of hash.
        bool isFound = containsTripleZero(hash);
        // Modify the global variable and return;
        if (isFound) {
            parameters.winner_nonce = nonce;
            parameters.finished = true;
            return;
        }
    }
}

void show_wallets(const Server& server) {
    std::cout << std::string(20, '*') << std::endl;
    for(const auto& client: server.clients)
        std::cout << client.first->get_id() <<  " : "  << client.second << std::endl;
    std::cout << std::string(20, '*') << std::endl;
}
