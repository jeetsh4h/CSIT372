#include "person.hpp"

struct Node {
    Person data;
    Node* left;
    Node* right;

    Node() {
        this->left = nullptr;
        this->right = nullptr;
        this->data = Person();
    }

    Node(Person data) {
        this->left = nullptr;
        this->right = nullptr;
        this->data = data;
    }
};


struct BSTree {
    Node *root = new Node();

    void addNode(Person data);
    Person* search(std::string name) const;
    void rebalanceTree();
};


void BSTree::addNode(Person data) {
    Node *new_node = new Node(data);

/* assumes that data is empty */
    if (this->root->data == Person()) {
        this->root = new_node;
        return;
    };

    Node *current = this->root;
    while (true) {
        if (new_node->data < current->data) {
            if (current->left == nullptr) {
                current->left = new_node;
                return;
            }
            current = current->left;
        } else {
            if (current->right == nullptr) {
                current->right = new_node;
                return;
            }
            current = current->right;
        }
    }
}

Person* BSTree::search(std::string name) const {
    Node *current = this->root;

    unsigned short n_access = 1;

    while (current != nullptr) {
        if (current->data.first_name == name) {
            printf("Accesses: %d\n", n_access);
            return &current->data;
        } else if (name < current->data.first_name) {
            current = current->left;\
            n_access++;
        } else {
            current = current->right;
            n_access++;
        }
    }
    printf("Accesses: %d\n", n_access);
    return nullptr;
}

void BSTree::rebalanceTree() {
    // NotImplemented
}