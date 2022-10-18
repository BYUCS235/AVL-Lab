#ifndef BST_CPP_T2K_10_17_2022
#define BST_CPP_T2K_10_17_2022

#include <iostream>
#include "BST.h"

BST::BST()
{
    this->root = NULL;
    this->nextId = 0;
};

BST::~BST()
{
    clear();
    if (this->root)
    {
        delete this->root;
    }
}

/*
 * Returns the root node for this tree
 *
 * @return the root node for this tree.
 */
Node *BST::getRootNode() const
{
    return this->root;
}

////
////
////
////

/*
 * Attempts to add the given value to the BST tree
 * Rebalances the tree if data is successfully added
 *
 * @return true if added
 * @return false if unsuccessful (i.e. the value is already in tree)
 */
bool BST::add(int data)
{
    if (this->root == NULL)
    {
        this->root = new Node(data, nextId);
        this->nextId++;
        return true;
    }
    return addToSubtree(data, this->root);
}

bool BST::addToSubtree(int data, Node *localRoot)
{
    // Case 1: left subtree
    if (data < localRoot->getData())
    {
        if (localRoot->getLeftChild() == NULL)
        {
            localRoot->setLeftChild(new Node(data, nextId));
            this->nextId++;
            return true;
        }
        else
        {
            return addToSubtree(data, localRoot->getLeftChild());
        }
    }

    // Case 2: right subtree
    else if (data > localRoot->getData())
    {
        if (localRoot->getRightChild() == NULL)
        {
            localRoot->setRightChild(new Node(data, nextId));
            this->nextId++;
            return true;
        }
        else
        {
            return addToSubtree(data, localRoot->getRightChild());
        }
    }

    // Case 3: current node (i.e. duplicate)
    else
    {
        return false;
    }

    // TODO Add() rebalancing
}

////
//// remove()
////

/**
 * @brief Attempts to remove the given value from the BST tree.
 * Rebalances the tree if data is successfully removed
 *
 * @param data The value to remove.
 * @return true if successfully removed
 * @return false if remove is unsuccessful (i.e. the value is not in the tree)
 */
bool BST::remove(int data)
{
    return removeFromSubtree(data, this->root);
    // TODO Remove() rebalancing
}

/**
 * @brief Attempt to remove the given value from the given node's subtree.
 *
 * @param data The value to remove.
 * @param localRoot The root of the subtree to search.
 * @return true if successfully removed
 * @return false if remove is unsuccessful (i.e. the value is not in the tree)
 */
bool BST::removeFromSubtree(int data, Node *&localRoot)
{
    // Case 0: NULL
    if (localRoot == NULL)
    {
        return false;
    }

    // Case 1: left subtree
    else if (data < localRoot->data)
    {
        if (localRoot->getLeftChild() == NULL)
        {
            return false;
        }
        return removeFromSubtree(data, localRoot->getLeftChildRef());
    }

    // Case 2: right subtree
    else if (data > localRoot->data)
    {
        if (localRoot->getRightChild() == NULL)
        {
            return false;
        }
        return removeFromSubtree(data, localRoot->getRightChildRef());
    }

    // Case 4: current node
    else
    {
        Node *temp = removeNode(localRoot);
        delete temp;
        return true;
    }
}

/**
 * @brief Remove a node and update its subtree.
 * Removes the node at the given reference pointer and updates the pointer to the new local root.
 *
 * @param localRoot The node to be removed.
 */
Node *BST::removeNode(Node *&localRoot)
{
#ifdef DEBUG
    if (localRoot == NULL)
    {
        std::cout << "Problem! Called removeNode(NULL), should be unreachable" << std::endl;
        throw;
    }
#endif

    Node *left = localRoot->getLeftChild();
    Node *right = localRoot->getRightChild();

    // Root has no left child
    if (left == NULL)
    {
        // Case 1: Root has no children
        if (right == NULL)
        {
            Node *out = localRoot;
            localRoot = NULL;
            return out;
        }

        // Case 2: Root has right child only
        else
        {
            Node *out = localRoot;
            localRoot = right;
            return out;
        }
    }

    // Root has left child
    else
    {
        // Case 3: Root has left child only
        if (right == NULL)
        {
            Node *out = localRoot;
            localRoot = left;
            return out;
        }

        // Case 4: Root has left and right children
        else
        {
            Node *out = removeNodeWith2Children(localRoot);
            return out;
        }
    }
}

/**
 * @brief Remove a Node that has both a left and a right child.
 *
 * @param localRoot
 */
Node *BST::removeNodeWith2Children(Node *&localRoot)
{
#ifdef DEBUG
    if (localRoot == NULL)
    {
        std::cout << "Problem! Called removeNodeWith2Children(NULL), should be unreachable" << std::endl;
        throw;
    }

    if (localRoot->getLeftChild() == NULL || localRoot->getRightChild() == NULL)
    {
        std::cout << "Problem! Called removeNodeWith2Children() without two children, should be unreachable" << std::endl;
        throw;
    }
#endif

    Node *left = localRoot->getLeftChild();
    Node *right = localRoot->getRightChild();

    // Case 1: left has no right child
    if (left->getRightChild() == NULL)
    {
        Node *out = localRoot;
        localRoot = left;
        localRoot->setRightChild(right);
        return out;
    }

    // Case 2: left has a right child
    else
    {

        // Find (parent of) new root node
        Node *newRootParent = left;
        while (newRootParent->getRightChild()->getRightChild()) // while newRootParent is a grandparent (on the right)
        {
            newRootParent = newRootParent->getRightChild();
        }

        // Assert: newRootParent->right is the new root (and has no right child)

        // Update pointers
        Node *out = localRoot;
        localRoot = newRootParent->getRightChild(); // Set root pointer

        newRootParent->setRightChild(newRootParent->getRightChild()->getLeftChild()); // Save new root's old left child

        localRoot->setLeftChild(left);   // Set new root->left
        localRoot->setRightChild(right); // Set new root->right

        return out;
    }
}

////
//// clear()
////

/*
 * Removes all nodes from the tree, resulting in an empty tree.
 */
void BST::clear()
{
    clearSubtree(this->root); // clear root
}

void BST::clearSubtree(Node *&localRoot)
{
    if (localRoot == NULL)
    {
        return;
    }

    if (localRoot->getLeftChild() != NULL)
    {
        clearSubtree(localRoot->getLeftChildRef());
    }

    if (localRoot->getRightChild() != NULL)
    {
        clearSubtree(localRoot->getRightChildRef());
    }

    delete localRoot;
    localRoot = NULL;
}

////
//// toString()
////

std::string BST::toString() const
{
    std::stringstream ss;
    addSubtreeToSStream(this->root, ss, "");
    return ss.str();
}

void BST::addSubtreeToSStream(Node *localRoot, std::stringstream &ss, std::string prefix) const
{
    if (localRoot == NULL)
    {
        ss << prefix << "NULL" << std::endl;
        return;
    }

    ss << prefix << localRoot->getData() << "(" << localRoot->id << ")";
    prefix += "  ";

    if (localRoot->getLeftChild() == NULL && localRoot->getRightChild() == NULL)
    {
        ss << " NULL NULL" << std::endl;
    }
    else
    {
        ss << std::endl;
        addSubtreeToSStream(localRoot->getRightChild(), ss, prefix);
        addSubtreeToSStream(localRoot->getLeftChild(), ss, prefix);
    }
}

#endif
