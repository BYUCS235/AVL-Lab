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
//    add()
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
 * Assumes localRoot is not null.
 *
 * @param data
 * @param localRoot
 * @return true if added
 * @return false if unsuccessful (i.e. the value is already in tree)
 */
Result AVL::addToSubtree(int data, Node *localRoot)
{
    // Case 1: left subtree
    if (data < localRoot->getData())
    {
        return updateHeightsAndAddToSubtree(data, localRoot,
                                            localRoot->getLeftChild(),
                                            localRoot->getRightChild(), true);
    }

    // Case 2: right subtree
    else if (data > localRoot->getData())
    {
        return updateHeightsAndAddToSubtree(data, localRoot,
                                            localRoot->getRightChild(),
                                            localRoot->getLeftChild(), false);
    }

    // Case 3: current node (i.e. duplicate)
    else
    {
        return FAIL; // Failed to add node
    }

    // TODO Add() rebalancing
}

/**
 * @brief Update heights and add the given value to the indicated subtree.
 * Formatted to use pointers to the child to be updated and the other child
 * rather than hardcoded left and right. The one exception which requires isLeft
 * is updating localRoot's child pointers.
 *
 * Assumes localRoot is not null. Child node args can be null.
 *
 * @param data
 * @param localRoot
 * @return true if added
 * @return false if unsuccessful (i.e. the value is already in tree)
 */
Result AVL::updateHeightsAndAddToSubtree(int data, Node *localRoot, Node *updateChild, Node *otherChild, int isLeft)
{
    if (updateChild == NULL)
    {
        // Only action that requires knowing left vs. right (as opposed to
        // just using update and other) is updating localRoot's child pointers
        if (isLeft)
        {
            localRoot->setLeftChild(new Node(data, nextId));
            updateChild = localRoot->getLeftChild();
        }
        else
        {
            localRoot->setRightChild(new Node(data, nextId));
            updateChild = localRoot->getRightChild();
        }

        bool wasHeightUpdated = updateHeight(localRoot);
        this->nextId++;
        return (wasHeightUpdated) ? SUCCESS_UPDATE : SUCCESS_NO_UPDATE; // Added node successfully
    }
    else
    {
        Result childResult = addToSubtree(data, updateChild);

        if (childResult == FAIL)
        {
            return FAIL;
        }

        bool wasHeightUpdated = false;
        if (childResult == SUCCESS_UPDATE)
        {
            wasHeightUpdated = updateHeight(localRoot);
        }
        return (wasHeightUpdated) ? SUCCESS_UPDATE : SUCCESS_NO_UPDATE;
    }
}

/**
 * @brief Update the height of the given node.
 * Updates height based on the heights of its children.
 *
 * Assumes localRoot is not null. Child node args can be null.
 * Assumes child HAS ALREADY been updated by the given amount.
 *
 * @param localRoot The node to update.
 * @param updateChild The child node whose height changed. Does NOT accept null values.
 * @param otherChild The child node whose height did not change. Can be NULL.
 * @return true if this node's height changed.
 * @return false otherwise, i.e. if otherChild's height was much greater than updateChild's.
 */
bool AVL::updateHeight(Node *localRoot)
{
    Node *left = localRoot->getLeftChild();
    Node *right = localRoot->getRightChild();
    int oldHeight = localRoot->getHeight();
    int newHeight;

    // Check for null child nodes
    if (left == NULL && right == NULL)
    {
        return false;
    }
    else if (right == NULL)
    {
        newHeight = left->getHeight() + 1;
    }
    else if (left == NULL)
    {
        newHeight = right->getHeight() + 1;
    }
    else
    {
        int lHeight = left->getHeight();
        int rHeight = right->getHeight();
        int maxChildHeight = (lHeight > rHeight) ? lHeight : rHeight;
        newHeight = maxChildHeight + 1;
    }

    // Calculate heights

    if (newHeight == oldHeight)
    {
        return false; // Did not update height
    }

    localRoot->setHeight(newHeight);
    return true; // Updated height
}

////
////
////
//   remove()
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
Result AVL::removeFromSubtree(int data, Node *&localRoot)
{
    if (localRoot == NULL)
    {
        // Case 0: NULL
        return FAIL; // Remove failed
    }
    else if (data < localRoot->data)
    {
        // Case 1: left subtree
        if (localRoot->getLeftChild() == NULL)
        {
            return FAIL; // Remove failed
        }
        else
        {
            // Update height
            Result result = updateHeightsAndRemove(localRoot,
                                                   localRoot->getLeftChildRef(), // rmvTreeRef
                                                   localRoot->getRightChild(),   // otherTree
                                                   data);
            return result;
        }
    }
    else if (data > localRoot->data)
    {
        // Case 2: right subtree
        if (localRoot->getRightChild() == NULL)
        {
            return FAIL; // Remove failed
        }
        else
        {
            // Update height
            Result result = updateHeightsAndRemove(localRoot,
                                                   localRoot->getRightChildRef(), // rmvTreeRef
                                                   localRoot->getLeftChild(),     // otherTree
                                                   data);
            return result;
        }
    }
    else
    {
        // Case 3: current node is the node to remove

        if (localRoot->getLeftChild() == NULL)
        {
            removeNode(localRoot);
            return SUCCESS_UPDATE; // Removed node successfully
        }

        // Case 3B: Node to remove has left child
        else
        {
            removeNode(localRoot);
            return SUCCESS_UPDATE; // Removed node successfully; higher nodes may need to update heights
        }
    }
}

/**
 * @brief Updates the height at the given node, recursively calling removeFromSubtree().
 *
 * @param localRoot The root of the subtree.
 * @param rmvTreeRef The child node that might contain the value to remove.
 * @param otherTree The other child node.
 * @param data The value to remove if applicable.
 * @return true if the value was removed successfully.
 * @return false otherwise, i.e. if the value is not in the subtree.
 */
Result AVL::updateHeightsAndRemove(Node *&localRoot, Node *&rmvTreeRef, Node *otherTree, int data)
{
    // Recurse
    Result childResult = removeFromSubtree(data, rmvTreeRef);

    // Return without updating if the remove failed
    if (childResult == FAIL)
    {
        return FAIL;
    }
    bool wasHeightUpdated = true;
    if (childResult == SUCCESS_UPDATE)
    {
        wasHeightUpdated = updateHeight(localRoot);
    }

    return (wasHeightUpdated) ? SUCCESS_UPDATE : SUCCESS_NO_UPDATE;
}

/**
 * @brief Remove a node and update its subtree.
 * Removes the local root at the given reference pointer and updates the pointer to the new local root. Called as the base case for updateHeightsAndRemove().
 *
 * @param rmvNodeRef A pointer reference to the node to remove.
 */
void AVL::removeNode(Node *&rmvNodeRef)
{

    Node *left = rmvNodeRef->getLeftChild();
    Node *right = rmvNodeRef->getRightChild();

    // Root has no left child
    if (left == NULL)
    {
        // Case 1: Root has no children
        if (right == NULL)
        {
            Node *rmvNode = rmvNodeRef;
            rmvNodeRef = NULL;
            delete rmvNode;
        }

        // Case 2: Root has right child only
        else
        {
#ifdef DEBUG
            std::cout << "removeNode case 2" << std::endl;
#endif
            Node *rmvNode = rmvNodeRef;
            rmvNodeRef = right;
            delete rmvNode;
        }
    }

    // Root does have a left child
    else
    {
        // Case 3: Root has left child only

        if (right == NULL)
        {
            // Case 3: Root has left child only
            Node *rmvNode = rmvNodeRef;
            rmvNodeRef = left;
            delete rmvNode;
        }
        else
        {
            // Case 4: Root has left and right children
            removeNodeWith2Children(rmvNodeRef);
        }
    }
}

/**
 * @brief Remove a node with two children and update its subtree.
 * Removes the local root at the given reference pointer and updates the pointer to the new local root. Throws an error if one of the node's children is NULL.
 *
 * @param rmvNodeRef A pointer reference to the node to remove.
 */
void AVL::removeNodeWith2Children(Node *&rmvNodeRef)
{
    Node *left = rmvNodeRef->getLeftChild();
    Node *right = rmvNodeRef->getRightChild();

    // Case 1: left has no right child
    if (left->getRightChild() == NULL)
    {
        Node *rmvNode = rmvNodeRef;
        rmvNodeRef = left;
        rmvNodeRef->setRightChild(right);
        delete rmvNode;
        updateHeight(rmvNodeRef);
    }

    // Case 2: left does have a right child
    else
    {
        // Find (the parent of) the new root
        Node *rootParent = NULL;
        updateHeightsAndFindReplacement(left, rootParent);

        // Delete old root and update localRoot pointer
        Node *rmvNode = rmvNodeRef;
        rmvNodeRef = rootParent->getRightChild();
        delete rmvNode;

        // Update pointers to remove the new root from its previous location
        rootParent->setRightChild(rootParent->getRightChild()->getLeftChild());

        // Set new root's children
        rmvNodeRef->setLeftChild(left);   // Set new root->left
        rmvNodeRef->setRightChild(right); // Set new root->right

        // Set new root's height
        updateHeight(rmvNodeRef);
    }
}

/**
 * @brief Search for the node to replace a removed node, updating heights.
 * Originally called on the left child of the node to be removed. Recursively searches down the right subtree of currentNode until finding a node (the parent of the new root node) that has no right-right grandchild. Updates node heights along the way.
 *
 * Requires currentNode->getRightChild() != NULL and rootParent == NULL.
 *
 * @param currentNode The current node.
 * @param rootParent A reference pointer which will be set to point to the parent node of the new root.
 * @return true if the current node's height was updated.
 * @return false otherwise, i.e. if the left child's height was much greater than the right child's.
 */
bool AVL::updateHeightsAndFindReplacement(Node *currentNode, Node *&rootParent)
{
    // Base case: found the new root
    if (currentNode->getRightChild()->getRightChild() == NULL)
    {
        rootParent = currentNode;
        return updateRootParentHeight(currentNode);
    }

    else
    {
        // Recurse
        bool childUpdated = updateHeightsAndFindReplacement(currentNode->getRightChild(), rootParent);

        // Update height if necessary

        if (!childUpdated)
        {
            return false;
        }

        bool result = updateHeight(currentNode);
        return result;
    }
}

bool AVL::updateRootParentHeight(Node *rootParent)
{
    Node *right = rootParent->getRightChild()->getLeftChild();
    Node *left = rootParent->getLeftChild();
    int oldHeight = rootParent->getHeight();
    int newHeight;
    if (right == NULL && left == NULL)
    {
        newHeight = 0;
    }
    else if (right == NULL)
    {
        newHeight = left->getHeight() + 1;
    }
    else if (left == NULL)
    {
        newHeight = right->getHeight() + 1;
    }
    else
    {
        int lHeight = left->getHeight();
        int rHeight = right->getHeight();
        newHeight = (lHeight > rHeight) ? lHeight + 1 : rHeight + 1;
    }

    if (newHeight == oldHeight)
    {
        return false;
    }

    rootParent->setHeight(newHeight);
    return true;
}

////
//   clear()
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
//   toString()
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

    ss << prefix << localRoot->getData() << "(" << localRoot->id << ",h=" << localRoot->height << ")";
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
