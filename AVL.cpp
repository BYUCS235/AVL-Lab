#ifndef BST_CPP_T2K_10_17_2022
#define BST_CPP_T2K_10_17_2022

#include <iostream>
#include "AVL.h"

AVL::AVL()
{
    this->root = NULL;
    this->nextId = 0;
};

AVL::~AVL()
{
    clear();
    if (this->root)
    {
        delete this->root;
    }
}

/**
 * @brief Return the root node for this tree.
 * @return the root node for this tree.
 */
Node *AVL::getRootNode() const
{
    return this->root;
}

////
////
////
////

/**
 * @brief Attempt to add the given value to the BST tree.
 * Rebalances the tree if data is successfully added.
 *
 * @return true if added
 * @return false if unsuccessful (i.e. the value is already in tree)
 */
bool AVL::add(int data)
{
    if (this->root == NULL)
    {
        this->root = new Node(data, nextId);
        this->nextId++;
        return true;
    }
    return addToSubtree(data, this->root);
}

/**
 * @brief Attempt to add the given value to the given subtree.
 * Rebalances the tree if data is successfully added.
 *
 * @param data
 * @param localRoot
 * @return true if added
 * @return false if unsuccessful (i.e. the value is already in tree)
 */
bool AVL::addToSubtree(int data, Node *localRoot)
{
    // Case 1: left subtree
    if (data < localRoot->getData())
    {
        if (localRoot->getLeftChild() == NULL)
        {
            localRoot->setLeftChild(new Node(data, nextId));
            this->nextId++;
            updateHeight(localRoot,
                         localRoot->getLeftChild(),  // updateChild
                         localRoot->getRightChild(), // otherChild
                         1);
            return true; // Added node successfully
        }
        else
        {
            bool addedNodeSuccessfully = addToSubtree(data, localRoot->getLeftChild());
            if (addedNodeSuccessfully)
            {
                updateHeight(localRoot,
                             localRoot->getLeftChild(),  // updateChild
                             localRoot->getRightChild(), // otherChild
                             1);
            }
            return addedNodeSuccessfully;
        }
    }

    // Case 2: right subtree
    else if (data > localRoot->getData())
    {
        if (localRoot->getRightChild() == NULL)
        {
            localRoot->setRightChild(new Node(data, nextId));
            updateHeight(localRoot,
                         localRoot->getRightChild(), // updateChild
                         localRoot->getLeftChild(),  // otherChild
                         1);
            this->nextId++;
            return true; // Added node successfully
        }
        else
        {
            bool addedNodeSuccessfully = addToSubtree(data, localRoot->getRightChild());
            if (addedNodeSuccessfully)
            {
                updateHeight(localRoot,
                             localRoot->getRightChild(), // updateChild
                             localRoot->getLeftChild(),  // otherChild
                             1);
            }
            return addedNodeSuccessfully;
        }
    }

    // Case 3: current node (i.e. duplicate)
    else
    {
        return false; // Failed to add node
    }

    // TODO Add() rebalancing
}

/**
 * @brief Update the height of the given node.
 * Updates height by the given increment, based on the heights of its children.
 * Assumes child HAS ALREADY been updated by the given amount.
 *
 * @param localRoot The node to update.
 * @param updateChild The child node whose height changed. Does NOT accept null values.
 * @param otherChild The child node whose height did not change. Can be NULL.
 * @param increment The amount by which to increment the height. Either 1 or -1.
 * @return true if this node's height changed.
 * @return false otherwise, i.e. if otherChild's height was much greater than updateChild's.
 */
bool AVL::updateHeight(Node *localRoot, Node *updateChild, Node *otherChild, int increment)
{
    // Note: assumes child node HAS ALREADY BEEN updated.

    // Check for invalid input
    if (increment != 1 && increment != -1)
    {
        std::cout << "Error: called updateHeight() with the increment not equal to 1 or -1" << std::endl;
        throw;
    }

    // Check for null child nodes
    if (updateChild == NULL || otherChild == NULL)
    {
#ifdef DEBUG
        std::cout << "updateHeight(root=" << localRoot->id << ") (has at least 1 NULL child)" << std::endl;
#endif
        localRoot->setHeight(localRoot->getHeight() + increment);
        return true; // Updated height
    }

    /*
    All possible combinations

    prev,post, other, increment

    1,2, 0, +   1->2 yes
    1,2, 1, +   1->2 yes
    1,2, 2, +   2->2 no
    1,2, 3, +   3->3 no

    2,1, 0, -   2->1 yes
    2,1, 1, -   2->1 yes
    2,1, 2, -   2->2 no
    2,1, 3, -   3->3 no

    increment height iff max(prev,post) >= other
    */

    // Find max(old height, new height)
    int maxHeight = increment > 0 ? updateChild->getHeight() : updateChild->getHeight() - increment;
    int otherHeight = otherChild->getHeight();

    if (maxHeight >= otherHeight)
    {
        localRoot->setHeight(localRoot->getHeight() + increment);
        return true; // Updated height
    }

    return false; // Did not update height
}

////
////
////
//// remove()
////
////
////

/**
 * @brief Attempts to remove the given value from the BST tree.
 * Rebalances the tree if data is successfully removed
 *
 * @param data The value to remove.
 *
 * @return true if successfully removed
 * @return false if remove is unsuccessful (i.e. the value is not in the tree)
 */
bool AVL::remove(int data)
{
    bool rmvResult = removeFromSubtree(data, this->root);
    return rmvResult;
    // TODO Remove() rebalancing
}

/**
 * @brief Attempt to remove the given value from the given node's subtree.
 *
 * @param data The value to remove.
 * @param localRoot The root of the subtree to search.
 * @return true if the value was removed successfully.
 * @return false otherwise, i.e. if the value is not in the subtree.
 */
bool AVL::removeFromSubtree(int data, Node *&localRoot)
{
    // Case 0: NULL
    if (localRoot == NULL)
    {
        return false; // Remove failed
    }

    // Case 1: left subtree
    else if (data < localRoot->data)
    {
        if (localRoot->getLeftChild() == NULL)
        {
            return false; // Remove failed
        }

        // Update height
        // callRemoveNode=false: recursively calls removeFromSubtree()
        else
        {
            bool out = updateHeightsAndRemove(localRoot,
                                              localRoot->getLeftChildRef(), // rmvTreeRef
                                              localRoot->getRightChild(),   // otherTree
                                              false, data);
            return out;
        }
    }

    // Case 2: right subtree
    else if (data > localRoot->data)
    {
        if (localRoot->getRightChild() == NULL)
        {
            return false; // Remove failed
        }

        // Update height
        // callRemoveNode=false: recursively calls removeFromSubtree()
        else
        {
            bool out = updateHeightsAndRemove(localRoot,
                                              localRoot->getRightChildRef(), // rmvTreeRef
                                              localRoot->getLeftChild(),     // otherTree
                                              false, data);
            return out;
        }
    }

    // Case 3: current node
    else
    {
        // No left child
        if (localRoot->getLeftChild() == NULL)
        {
            removeNode(localRoot);
            return true; // Removed node successfully
        }

        // Has left child
        else
        {
            // Update height
            // callRemoveNode=true: calls removeNode() instead of removeFromSubtree()
            updateHeightsAndRemove(localRoot,
                                   localRoot->getLeftChildRef(), // rmvTreeRef
                                   localRoot->getRightChild(),   // otherTree
                                   true, data);

            return true; // Removed node successfully
        }
    }
}

/**
 * @brief Updates the height at the given node, recursively calling the indicated remove funtion.
 *
 * @param localRoot The root of the subtree.
 * @param rmvTreeRef The child node that might contain the value to remove.
 * @param otherTree The other child node.
 * @param callRemoveNode Calls removeNode() if true, otherwise removeFromSubtree() is called.
 * @param data The value to remove if applicable.
 * @return true if the value was removed successfully.
 * @return false otherwise, i.e. if the value is not in the subtree.
 */
bool AVL::updateHeightsAndRemove(Node *&localRoot, Node *&rmvTreeRef, Node *otherTree, bool callRemoveNode, int data)
{

    // Base case
    if (callRemoveNode)
    {
        removeNode(localRoot);
        if (otherTree == NULL)
        {
            // TODO Make sure localRoot's parent has its height updated
        }
    }

    // Recurse down rmvTreeRef
    else
    {
        // Recurse
        bool removedSuccessfully = removeFromSubtree(data, rmvTreeRef);

        // Return without updating if the remove failed
        if (!removedSuccessfully)
        {

            return false;
        }
        updateHeight(localRoot, rmvTreeRef, otherTree, -1);
    }

    return true;
}

/**
 * @brief Remove a node and update its subtree.
 * Removes the local root at the given reference pointer and updates the pointer to the new local root. Called as the base case for updateHeightsAndRemove().
 *
 * @param localRoot A pointer reference to the node to remove.
 */
void AVL::removeNode(Node *&localRoot)
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
            Node *rmvNode = localRoot;
            localRoot = NULL;
            delete rmvNode;
        }

        // Case 2: Root has right child only
        else
        {
            Node *rmvNode = localRoot;
            localRoot = right;
            delete rmvNode;
        }
    }

    // Root does have a left child
    else
    {
        // Case 3: Root has left child only
        if (right == NULL)
        {
            Node *rmvNode = localRoot;
            localRoot = left;
            delete rmvNode;
        }

        // Case 4: Root has left and right children
        else
        {
            removeNodeWith2Children(localRoot);
        }
    }
}

/**
 * @brief Remove a node with two children and update its subtree.
 * Removes the local root at the given reference pointer and updates the pointer to the new local root. Throws an error if one of the node's children is NULL.
 *
 * @param localRoot A pointer reference to the node to remove.
 */
void AVL::removeNodeWith2Children(Node *&localRoot)
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
        Node *rmvNode = localRoot;
        localRoot = left;
        localRoot->setRightChild(right);
        delete rmvNode;
    }

    // Case 2: left does have a right child
    else
    {
        // Find (the parent of) the new root
        Node *rootParent = NULL;
        updateHeightsAndFindReplacement(left, rootParent);

        // Delete old root and update localRoot pointer
        Node *rmvNode = localRoot;
        localRoot = rootParent->getRightChild();
        delete rmvNode;

        // Update pointers to remove the new root from its previous location
        rootParent->setRightChild(rootParent->getRightChild()->getLeftChild());

        // Set new root's children
        localRoot->setLeftChild(left);   // Set new root->left
        localRoot->setRightChild(right); // Set new root->right
    }
}

/**
 * @brief Search for the node to replace a removed node, updating heights.
 * Originally called on the left child of the node to be removed. Recursively searches down the right subtree of currentNode until finding a node (the parent of the new root node) that has no right-right grandchild. Updates node heights along the way. Requires currentNode->getRightChild() != NULL and rootParent == NULL.
 *
 * @param currentNode The current node.
 * @param rootParent A reference pointer which will be set to point to the parent node of the new root.
 * @return true if the current node's height was updated.
 * @return false otherwise, i.e. if the left child's height was much greater than the right child's.
 */
bool AVL::updateHeightsAndFindReplacement(Node *currentNode, Node *&rootParent)
{
    // Base case
    if (currentNode->getRightChild()->getRightChild() == NULL)
    {
        rootParent = currentNode;
        return true;
    }

    else
    {
        // Recurse
        bool childUpdated = updateHeightsAndFindReplacement(currentNode->getRightChild(), rootParent);

        // Update height if necessary

        // If child didn't update, parent doesn't need to update
        if (!childUpdated)
        {
            return false;
        }

        bool result = updateHeight(currentNode,                  // localRoot
                                   currentNode->getRightChild(), // updateChild
                                   currentNode->getLeftChild(),  // otherChild
                                   -1);                          // increment
        return result;
    }
}

////
//// clear()
////

/*
 * Removes all nodes from the tree, resulting in an empty tree.
 */
void AVL::clear()
{
    clearSubtree(this->root); // clear root
    this->nextId = 0;
}

void AVL::clearSubtree(Node *&localRoot)
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
////
////
//// toString()
////
////
////

std::string AVL::toString() const
{
    std::stringstream ss;
    addSubtreeToSStream(this->root, ss, "");
    return ss.str();
}

void AVL::addSubtreeToSStream(Node *localRoot, std::stringstream &ss, std::string prefix) const
{
    if (localRoot == NULL)
    {
        ss << prefix << "NULL" << std::endl;
        return;
    }

    ss << prefix << localRoot->getData() << "(" << localRoot->height << ")";
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
