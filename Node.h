#ifndef NODE_H_T2K_10_17_2022
#define NODE_H_T2K_10_17_2022

#include <string>
#include "NodeInterface.h"

struct Node : public NodeInterface
{
private:
    friend class AVL;
    Node *leftChild;
    Node *rightChild;
    int data;
    int height;
    int id;

public:
    Node();
    Node(int data, int id);
    Node(int data, Node *leftChild, Node *rightChild, int id);
    ~Node();

    // getters
    int getData() const;
    int getBalance() const;
    int getHeight();
    Node *getLeftChild() const;
    Node *getRightChild() const;
    Node *&getLeftChildRef();
    Node *&getRightChildRef();
    std::string toString() const;

    // setters
    void setLeftChild(Node *left);
    void setRightChild(Node *right);
    void setHeight(int height);
};

#endif
