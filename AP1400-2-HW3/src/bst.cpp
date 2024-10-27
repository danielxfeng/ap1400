#include <iomanip>
#include <memory>
#include <deque>
#include <utility>
#include "bst.h"

BST::Node::Node(int value, BST::Node *left = nullptr, BST::Node *right = nullptr)
        : value(value), left(left), right(right) {}

BST::Node::Node(const Node *&node) {
    value = node->value;
    left = nullptr;
    right = nullptr;
}

bool BST::Node::operator==(const int &other) const {
    return value == other;
}

bool BST::Node::operator!=(const int &other) const {
    return value != other;
}

bool BST::Node::operator<(const int &other) const {
    return value < other;
}

bool BST::Node::operator<=(const int &other) const {
    return value <= other;
}

bool BST::Node::operator>(const int &other) const {
    return value > other;
}

bool BST::Node::operator>=(const int &other) const {
    return value >= other;
}

// ---------------------------------------
// Belows are implementations of class BST.
//----------------------------------------

BST::BST() {
    root = nullptr;
    size = 0;
}

BST::BST(std::initializer_list<int> list) : BST() {
    for (auto n : list) this->add_node(n);
}

// ** The helper recursive function to help the copy constructor.
BST::Node *copy_constructor_helper(const BST::Node *curr);

BST::Node *copy_constructor_helper(const BST::Node *curr) {
    // Base case: return nullptr when unavailable;
    if (!curr) return nullptr;
    // Copy the node itself.
    auto *node = new BST::Node(*curr);
    // Recursive call to children node.
    node->left = copy_constructor_helper(curr->left);
    node->right = copy_constructor_helper(curr->right);
    return node;
}

BST::BST(const BST &other) noexcept {
    root = copy_constructor_helper(other.root);
    size = other.size;
}

BST::BST(BST &&other) noexcept {
    // Take over the given rvalue;
    root = other.root;
    size = other.size;
    // Clear the old resource.
    other.root = nullptr;
    other.size = 0;
}

void BST::delete_tree() {
    if (!root) {return;};
    std::vector<BST::Node *> nodes;
    bfs([&nodes](BST::Node *&node) { nodes.push_back(node); });
    for (auto &node: nodes)
        delete node;
    root = nullptr;
    size = 0;
}

BST &BST::operator=(const BST &other) {
    if (this != &other) {
        // Clear the existing resource.
        delete_tree();
        // Copy properties.
        root = copy_constructor_helper(other.root);
        size = other.size;
    }
    return *this;
}

BST &BST::operator=(BST &&other) noexcept {
    if (this != &other) {
        // Clear the existing resource.
        delete_tree();
        // Take over the given rvalue.
        root = other.root;
        size = other.size;
        // // Clear the old resource.
        other.root = nullptr;
        other.size = 0;
    }
    return *this;
}

BST::~BST() {
    delete_tree();
}

BST::Node *&BST::get_root() {
    return root;
}

// An iteration version tree traversal implementation.
// Use deque to iterate all nodes in the tree, the sentinel node is excluded.
void BST::bfs(std::function<void(BST::Node *&)> func) {
    // Initialize a deque and put root to it..
    std::deque<Node *> deque;
    deque.push_back(root);
    // Iterate until there is not any child.
    while (!deque.empty()) {
        // Pop from top of deque.
        BST::Node *&curr = deque.front();
        deque.pop_front();
        // Perform the high order function.
        func(curr);
        // Add curr's children to the bottom of deque.
        if (curr->left) deque.push_back(curr->left);
        if (curr->right) deque.push_back(curr->right);
    }
}

size_t BST::length() {
    return size;
}

// Return an empty node for adding, return nullptr for not found.
// Tail recursion for performance.
BST::Node **helper_find_empty_node(int value, BST::Node **root);

BST::Node **helper_find_empty_node(int value, BST::Node **root) {
    // Base case: if root is not exist
    if (!root || !*root) return root;
    // Base case: if found, return nullptr;
    if (**root == value) return nullptr;
    // Recursive call
    BST::Node **next = value < **root? &(*root)->left : &(*root)->right;
    return helper_find_empty_node(value, next);
}

// Add a new node by recursive call.
bool BST::add_node(int value) {
    // Find a proper position.
    BST::Node **node = helper_find_empty_node(value, &root);
    // Return false if there is no such node, which means the value is already in the tree.
    if (!node) return false;
    // Add node;
    *node = new BST::Node(value);
    // Update the size.
    size++;
    return true;
}

// The help method return a pair<node's pointer, parent's pointer> of the value.
// Tail recursion for performance.
std::pair<BST::Node **, BST::Node **> helper_find_node(int value, BST::Node **root);

std::pair<BST::Node **, BST::Node **> helper_find_node(int value, BST::Node **root) {
    auto empty_pair = std::make_pair(nullptr, nullptr);
    // Base case: empty node.
    if (!root) return empty_pair;
    // Base case: the value is root node, only the node exists.
    if (value == **root) return std::make_pair(root, nullptr);
    BST::Node **next = value < **root ? &(*root)->left : &(*root)->right;
    // Base case: the value is not in the tree.
    if (!next || !*next) return empty_pair;
    // Base case: the value is in children.
    if (value == **next) return std::make_pair(next, root);
    // Tail Recursion
    return helper_find_node(value, next);
}

BST::Node **BST::find_node(int value) {
    std::pair<BST::Node**, BST::Node**> res = helper_find_node(value, &root);
    return res.first;
}

// Return the pointer to the parent node by recursive call.
BST::Node **BST::find_parent(int value) {
    std::pair<BST::Node**, BST::Node**> res = helper_find_node(value, &root);
    return res.second;
}

// The helper function to return the rightmost node of a tree.
// Tail recursion for performance.
BST::Node **find_successor_in_left(BST::Node **root);

BST::Node **find_successor_in_left(BST::Node **root) {
    // Base case: found and return if the node has not a right child.
    if (!(*root)->right) return root;
    // Tail recursive call
    return find_successor_in_left(&(*root)->right);
}

BST::Node **BST::find_successor(int value) {
    BST::Node **curr = find_node(value);
    // Return nullptr if the node is not exists or it has not a left sub-tree.
    if (!curr || !(*curr)->left) return nullptr;
    return find_successor_in_left(&(*curr)->left);
}

BST::Node *BST::delete_helper_less_children(BST::Node **node, BST::Node **parent, BST::Node **successor) {
    BST::Node* del_node = *node;
    // The successor is either the child node or nullptr.
    successor = (*node)->left ? &(*node)->left : &(*node)->right;
    // Assign the parent's node;
    if (!parent) {
        root = *successor;
    } else {
        *node = *successor;
    }
    return del_node;
}

BST::Node *BST::delete_helper_two_children(int value, BST::Node **node) {
    // Get the successor.
    BST::Node **successor = find_successor(value);
    BST::Node *del_node = *successor;
    // Swap the value;
    (*node)->value = (*successor)->value;
    // Assign the successor parent's node;
    if ((*successor)->left) {
        *successor = (*successor)->left;
    } else {
        *successor = nullptr;
    }
    return del_node;
}

bool BST::delete_node(int value) {
    // Get the parent and node.
    std::pair<BST::Node**, BST::Node**> res = helper_find_node(value, &root);
    BST::Node **node = res.first;
    BST::Node **parent = res.second;
    // Return when not found.
    if (!node) return false;
    // The successor of node.
    BST::Node **successor;
    // Create a point to the node which will be deleted;
    BST::Node *del_node;
    // For a node which has only 1 or 0 child.
    if (!((*node)->left && (*node)->right)) {
        del_node = delete_helper_less_children(node, parent, successor);
    } else { // Delete a node which has only two children.
        del_node = delete_helper_two_children(value, node);
    }
    // Delete the useless node.
    delete del_node;
    // Update the size.
    size--;
    return true;
}

BST &BST::operator++() {
    // Every element ++.
    bfs([](BST::Node *&node){node->value++;});
    return *this;
}

BST BST::operator++(int) {
    // Copy a bst.
    BST temp = *this;
    // Every element ++.
    bfs([](BST::Node *&node){node->value++;});
    return temp;
}

std::ostream &operator<<(std::ostream &os, const BST &bst) {
    // Set print format
    os << std::setfill('*') << std::setw(18 * 5) << std::endl;
    // Get a vector of BFS nodes.
    std::vector<BST::Node *> nodes;
    const_cast<BST &>(bst).bfs([&nodes](BST::Node *&node) { nodes.push_back(node);});
    for (auto &node: nodes) {
        os << *node;
    }
    os << "binary search tree size: " << bst.size << std::endl;
    os << std::setfill('*') << std::setw(18 * 5) << std::endl;
    return os;
}

std::ostream &operator<<(std::ostream &os, const BST::Node &node) {
    // Check if it is nullptr.
    if (!&node) return os;
    // Set print format
    os << std::setiosflags(std::ios::left) << std::setfill(' ') << std::setw(15) << &node << "=> value:"
       << std::setiosflags(std::ios::left) << std::setfill(' ') << std::setw(15) << node.value << "=> left:"
       << std::setiosflags(std::ios::left) << std::setfill(' ') << std::setw(15) << node.left << "=> right:"
       << std::setiosflags(std::ios::left) << std::setfill(' ') << std::setw(15) << node.right << std::endl;
    return os;
}

// ---------------------------------------------------
// Belows are comp operate override functions for node.
// ---------------------------------------------------

bool operator==(const int &value, const BST::Node &node) {
    return value == node.value;
}

bool operator!=(const int &value, const BST::Node &node) {
    return value != node.value;
}

bool operator<(const int &value, const BST::Node &node) {
    return value < node.value;
}

bool operator<=(const int &value, const BST::Node &node) {
    return value <= node.value;
}

bool operator>(const int &value, const BST::Node &node) {
    return value > node.value;
}

bool operator>=(const int &value, const BST::Node &node) {
    return value >= node.value;
}
