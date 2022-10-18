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
    if (this->leftChild == NULL)
    {
        throw;
    }
    return this->leftChild;
}

/**
 * @brief Returns a reference to the right child of this node.
 * Throws an error if it doesn't have one.
 * @return Node*&
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
    this->rightChild = right;
}

void Node::setLeftChild(Node *left)
{
    this->leftChild = left;
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

/**
 * @brief Returns the height of this node.
 *
 * @return int The height of this tree with this node as the local root.
 */
int Node::getHeight()
{
    // TODO Implement fast getHeight()

    int height = 1;

    if (getLeftChild())
    {
        height = getLeftChild()->getHeight() + 1;
    }

    if (getRightChild())
    {
        int rHeight = getRightChild()->getHeight() + 1;
        if (rHeight > height)
        {
            return rHeight;
        }
    }

    return height;
}

#endif
