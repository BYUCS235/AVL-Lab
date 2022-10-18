#ifndef NODE_CPP_T2K_10_17_2022
#define NODE_CPP_T2K_10_17_2022

#include "Node.h"

Node::Node(int data, int id)
{
    this->data = data;
    this->id = id;
    this->leftChild = NULL;
    this->rightChild = NULL;
}

Node::Node(int data, Node *leftChild, Node *rightChild, int id)
{
    // TODO
    this->data = data;
    this->id = id;
    this->leftChild = leftChild;
    this->rightChild = rightChild;
}

Node::~Node()
{
    if (leftChild != NULL)
    {
        leftChild = NULL;
    }
    if (rightChild != NULL)
    {
        rightChild = NULL;
    }
}

/*
 * Returns the data that is stored in this node
 *
 * @return the data that is stored in this node.
 */
int Node::getData() const
{
    return this->data;
}

/*
 * Returns the left child of this node or null if it doesn't have one.
 *
 * @return the left child of this node or null if it doesn't have one.
 */
Node *Node::getLeftChild() const
{
    if (this->leftChild == NULL) // TODO? Unneeded null check?
    {
        return NULL;
    }
    return this->leftChild;
}

/*
 * Returns the right child of this node or null if it doesn't have one.
 *
 * @return the right child of this node or null if it doesn't have one.
 */
Node *Node::getRightChild() const
{
    if (this->rightChild == NULL) // TODO? Unneeded null check?
    {
        return NULL;
    }
    return this->rightChild;
}

/*
 * Returns the left child of this node or null if it doesn't have one.
 *
 * @return the left child of this node or null if it doesn't have one.
 */
Node *&Node::getLeftChildRef()
{
    if (this->leftChild == NULL)
    {
        throw;
    }
    return this->leftChild;
}

/*
 * Returns the right child of this node or null if it doesn't have one.
 *
 * @return the right child of this node or null if it doesn't have one.
 */
Node *&Node::getRightChildRef()
{
    if (this->rightChild == NULL)
    {
        throw;
    }
    return this->rightChild;
}

void Node::setRightChild(Node *right)
{
    if (right == NULL) // TODO? Unneeded null check?
    {
        this->rightChild = NULL;
    }
    else
    {
        this->rightChild = right;
    }
}

void Node::setLeftChild(Node *left)
{
    if (left == NULL) // TODO? Unneeded null check?
    {
        this->leftChild = NULL;
    }
    else
    {
        this->leftChild = left;
    }
}

std::string Node::toString() const
{
    std::string str = std::to_string(this->data);
    str += "(" + std::to_string(this->id) + ")";

    if (this->leftChild == NULL)
    {
        str += " _";
    }
    else
    {
        str += " [" + this->leftChild->toString() + "]";
    }
    if (this->rightChild == NULL)
    {
        str += " _";
    }
    else
    {
        str += " [" + this->rightChild->toString() + "]";
    }
    return str;
}

#endif
