//
// Created by Daniel X Feng on 1/12/2023.
//
#ifndef BST_H
#define BST_H

#include <iostream>
#include <functional>

// This class is an implementation of the Binary Search Tree.
class BST {
public:
    // This inner class respects a node of the BST.
    class Node {
    public:
        // Default constructor that will create a null node;
        Node() = default;

        // Constructor that initialize the properties;
        Node(int value, Node *left, Node *right);

        // Copy constructor that will copy a node.
        // Copy the value only, left and right pointers will be nullptr.
        Node(const Node *&node);

        // The value of the node.
        int value;
        // Return a pointer to the left child of the tree.
        // Return nullptr for null.
        Node *left;
        // Return a pointer to the right leaf of the tree.
        // Return nullptr for null.
        Node *right;

        // Override the operator << to support beautiful print.
        friend std::ostream &operator<<(std::ostream &os, const Node &node);

        // Override comp operators.
        bool operator==(const int &other) const;

        bool operator!=(const int &other) const;

        bool operator<(const int &other) const;

        bool operator<=(const int &other) const;

        bool operator>(const int &other) const;

        bool operator>=(const int &other) const;
    };

    // Default constructor that will create a empty BST;
    BST();

    // Constructor for take a vector of int.
    BST(std::initializer_list<int> list);

    // Copy Constructor for copy a BST
    BST(const BST &other) noexcept;

    // Moving Constructor for BST.
    BST(BST &&other) noexcept;

    // Copy assignment constructor.
    BST &operator=(const BST &other);

    // Move assignment constructor.
    BST &operator=(BST &&other) noexcept;

    // Destructor
    ~BST();

    // Return the pointer reference to the root node.
    Node *&get_root();

    // Iterate the tree by BFS(Breadth-first search), then apply func to each node.
    // Sounds like function Map.
    void bfs(std::function<void(Node *&node)> func);

    // Return the number of nodes inside the tree.
    size_t length();

    // Add a node to the tree.
    bool add_node(int value);

    // Return the pointer to a node with the same value in the tree.
    Node **find_node(int value);

    // Return the parent's node's pointer by the given value.
    Node **find_parent(int value);

    // Return the successor's node's pointer by the given value.
    // Used for delete only, so only return the rightmost node of the left sub tree.
    Node **find_successor(int value);

    // Delete a node from the tree by the given value.
    bool delete_node(int value);

    // Override the operator << to support beautiful print.
    friend std::ostream &operator<<(std::ostream &os, const BST &bst);

    // Override the operator prefix ++ by add 1 to each node in the tree.
    BST &operator++();

    // Override the operator postfix ++ by add 1 to each node in the tree.
    BST operator++(int);

private:
    // The pointer to root node.
    Node *root;
    // The size of the tree.
    size_t size;

    // A helper method to iterate all nodes and delete them.
    void delete_tree();

    // A helper method to help to perform the deleting when there is a leaf or has only one child.
    BST::Node *delete_helper_less_children(BST::Node **node, BST::Node **parent, BST::Node **successor);

    // A helper method to help to perform the deleting when there are two children.
    BST::Node *delete_helper_two_children(int value, BST::Node **node);
};

// Override comp operators.
bool operator==(const int &value, const BST::Node &node);

bool operator!=(const int &value, const BST::Node &node);

bool operator<(const int &value, const BST::Node &node);

bool operator<=(const int &value, const BST::Node &node);

bool operator>(const int &value, const BST::Node &node);

bool operator>=(const int &value, const BST::Node &node);

#endif //BST_H