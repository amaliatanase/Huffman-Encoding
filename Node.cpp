#include "Node.h"

Node::Node(uint8_t symbol, uint64_t frequency) {
    this->symbol = symbol;
    this->frequency = frequency;
    this->left = nullptr;
    this->right = nullptr;
}

Node * Node::join(Node *leftChild, Node *rightChild) {
    uint64_t parentFrequency = leftChild->frequency + rightChild->frequency;
    Node* parent = new Node('$', parentFrequency); //use new in order to permit accessing the node after finishing join execution
    parent->left = leftChild;
    parent->right = rightChild;
    return parent;
}
