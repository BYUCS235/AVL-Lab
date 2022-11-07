#ifndef NODE_CPP_T2K_10_17_2022
#define NODE_CPP_T2K_10_17_2022

#include <iostream>
#include <sstream>
#include "Node.h"

Node::Node()
{
    //
}

Node::Node(int data, int id)
{
    this->data = data;
    this->id = id;
    this->height = LEAF_HEIGHT;
    setLeftChild(NULL);
    setRightChild(NULL);
}

Node::Node(int data, Node *leftChild, Node *rightChild, int id)
{
    this->data = data;
    this->id = id;
    this->leftChild = leftChild;
    this->rightChild = rightChild;

    this->height = 0;
    if (leftChild != NULL)
    {
        setHeight(leftChild->getHeight() + 1);
    }
    if (rightChild != NULL)
    {
        if (rightChild->getHeight() + 1 > this->height)
        {
            setHeight(rightChild->getHeight() + 1);
        }
    }
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

////
//    Getters
////

/**
 * @brief Returns the data that is stored in this node.
 *
 * @return the data that is stored in this node.
 */
int Node::getData() const
{
    return this->data;
}

/**
 * @brief Returns a string containing data about this node.
 *
 * @return a string containing data about this node.
 */
std::string Node::getNodeData() const
{
    std::stringstream ss;
    // ss << getData() << "(" << this->id << ",h" << getNodeHeight() << ")";
    ss << getData() << "(h" << getNodeHeight() << ")";
    return ss.str();
}

int Node::getBalance() const
{
    int lHeight = (leftChild == NULL) ? -1 : leftChild->getHeight();
    int rHeight = (rightChild == NULL) ? -1 : rightChild->getHeight();
    return rHeight - lHeight;
}

/**
 * @brief Returns the height of this node.
 *
 * @return int The height of this tree with this node as the local root.
 */
int Node::getHeight()
{
    return getNodeHeight();
}

/**
 * @brief Returns the height of this node.
 *
 * @return int The height of this tree with this node as the local root.
 */
int Node::getNodeHeight() const
{
    return this->height;
}

/*
 * Returns the left child of this node or null if it doesn't have one.
 *
 * @return the left child of this node or null if it doesn't have one.
 */
Node *Node::getLeftChild() const
{
    return this->leftChild;
}

/*
 * Returns the right child of this node or null if it doesn't have one.
 *
 * @return the right child of this node or null if it doesn't have one.
 */
Node *Node::getRightChild() const
{
    return this->rightChild;
}

/**
 * @brief Returns a reference to the left child of this node.
 * Throws an error if it doesn't have one.
 * @return Node*&
 */
Node *&Node::getLeftChildRef()
{
    return this->leftChild;
}

/**
 * @brief Returns a reference to the right child of this node.
 * Throws an error if it doesn't have one.
 * @return Node*&
 */
Node *&Node::getRightChildRef()
{
    return this->rightChild;
}

std::string Node::toString() const
{
    std::stringstream ss;

    if (this->leftChild == NULL)
    {
        ss << "_";
    }
    else
    {
        ss << "[" + this->leftChild->toString() + "]";
    }

    ss << " " << getNodeData() << " ";

    if (this->rightChild == NULL)
    {
        ss << "_";
    }
    else
    {
        ss << "[" + this->rightChild->toString() + "]";
    }
    return ss.str();
}

////
//    setters
////

void Node::setRightChild(Node *right)
{
    this->rightChild = right;
}

void Node::setLeftChild(Node *left)
{
    this->leftChild = left;
}

void Node::setHeight(int height)
{
    this->height = height;
}

#endif
