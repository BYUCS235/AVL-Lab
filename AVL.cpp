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

/*
 * Returns the root node for this tree
 *
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

/*
 * Attempts to add the given value to the BST tree
 * Rebalances the tree if data is successfully added
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

bool AVL::addToSubtree(int data, Node *localRoot)
{
    // Case 1: left subtree
    if (data < localRoot->getData())
    {
        if (localRoot->getLeftChild() == NULL)
        {
            localRoot->setLeftChild(new Node(data, nextId));
            updateHeight(localRoot, true, 1);
            this->nextId++;
            return true;
        }
        else
        {
            bool result = addToSubtree(data, localRoot->getLeftChild());
            if (result)
            {
                updateHeight(localRoot, true, 1);
            }
            return result;
        }
    }

    // Case 2: right subtree
    else if (data > localRoot->getData())
    {
        if (localRoot->getRightChild() == NULL)
        {
            localRoot->setRightChild(new Node(data, nextId));
            updateHeight(localRoot, false, 1);
            this->nextId++;
            return true;
        }
        else
        {
            bool result = addToSubtree(data, localRoot->getRightChild());
            if (result)
            {
                updateHeight(localRoot, false, 1);
            }
            return result;
        }
    }

    // Case 3: current node (i.e. duplicate)
    else
    {
        return false;
    }

    // TODO Add() rebalancing
}

void AVL::updateHeight(Node *node, bool isLeft, int increment)
{
    // Note: assumes child node HAS ALREADY BEEN updated.

    // Check for invalid input
    if (increment != 1 && increment != -1)
    {
        std::cerr << "Warning: called incrementHeight() with the increment not equal to 1 or -1" << std::endl;
        return;
    }

    Node *updateChild = (isLeft) ? node->getLeftChild() : node->getRightChild();
    Node *otherChild = (isLeft) ? node->getRightChild() : node->getLeftChild();

    // Check for null child nodes
    if (updateChild == NULL)
    {
        std::cerr << "Warning: called incrementHeight() with updateChild equal to NULL" << std::endl;
        return;
    }
    if (otherChild == NULL)
    {
        node->setHeight(node->getHeight() + increment);
        return;
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
        node->setHeight(node->getHeight() + increment);
    }
}

////
//// remove()
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
    int result = removeFromSubtree(data, this->root);
    return result != -1;
    // TODO Remove() rebalancing
}

/**
 * @brief Attempt to remove the given value from the given node's subtree.
 *
 * @param data The value to remove.
 * @param localRoot The root of the subtree to search.
 *
 * @return 1 if removed and heights further up should not be updated
 * @return 0 if removed and heights further up may need to be updated
 * @return -1 if remove is unsuccessful (i.e. the value is not in the tree)
 */
int AVL::removeFromSubtree(int data, Node *&localRoot)
{
    // Case 0: NULL
    if (localRoot == NULL)
    {
        return -1;
    }

    // Case 1: left subtree
    else if (data < localRoot->data)
    {
        if (localRoot->getLeftChild() == NULL)
        {
            return -1;
        }

        // Update height
        // callRemoveNode=false: recursively calls removeFromSubtree()
        return updateHeightsAndRemove(localRoot,
                                      localRoot->getLeftChildRef(), // rmvTreeRef
                                      localRoot->getRightChild(),   // otherTree
                                      false, data);
    }

    // Case 2: right subtree
    else if (data > localRoot->data)
    {
        if (localRoot->getRightChild() == NULL)
        {
            return -1;
        }

        // Update height
        // callRemoveNode=false: recursively calls removeFromSubtree()
        return updateHeightsAndRemove(localRoot,
                                      localRoot->getRightChildRef(), // rmvTreeRef
                                      localRoot->getLeftChild(),     // otherTree
                                      false, data);
    }

    // Case 3: current node
    else
    {
        // No left child
        if (localRoot->getLeftChild() == NULL)
        {
            return removeNode(localRoot);
        }

        // Has left child
        else
        {
            // Update height
            // callRemoveNode=true: calls removeNode() instead of removeFromSubtree()
            return updateHeightsAndRemove(localRoot,
                                          localRoot->getLeftChildRef(), // rmvTreeRef
                                          localRoot->getRightChild(),   // otherTree
                                          true, data);
        }
    }
}

/**
 * @brief Updates the height at the given node, recusrively calling the indicated remove funtion.
 *
 * @param localRoot The root of the subtree.
 * @param rmvTreeRef The child node tha might contain the value to remove.
 * @param otherTree The other child node.
 * @param callRemoveNode Calls removeNode() if true, otherwise removeFromSubtree()
 * @param data The value to remove if applicable.
 *
 * @return 1 if removed and heights further up should not be updated
 * @return 0 if removed and heights further up may need to be updated
 * @return -1 if remove is unsuccessful (i.e. the value is not in the tree)
 */
int AVL::updateHeightsAndRemove(Node *&localRoot, Node *&rmvTreeRef, Node *otherTree, bool callRemoveNode, int data)
{
    // Save rmvTree's old height for comparison
    int prevRmvTreeHeight = rmvTreeRef->getHeight();

    // Recurse down rmvTree
    int result;
    if (callRemoveNode)
    {
        result = removeNode(localRoot);
    }
    else
    {
        if (!data)
        {
            std::cerr << "Problem! Called updateHeightsAndRemove() with callRemoveNode=false, but recieved NULL value for data." << std::endl;
            throw;
        }

        result = removeFromSubtree(data, rmvTreeRef);
    }

    // Simply pass along result if either:
    // removal failed, i.e. data wasn't found (-1)
    // don't-update-heights flag was recieved (1)
    if (result != 0)
    {
        return result;
    }

    // Update height if necessary
    else
    {
        // If otherTree's height outweighs rmvTree's
        // Means this node's height won't change, even if removal succeeded
        if (otherTree != NULL && otherTree->getHeight() >= prevRmvTreeHeight)
        {
            // Flag higher nodes not to change their heights
            return 1;
        }

        // If otherTree == NULL
        // or its height isn't as large enough to outweigh rmvTree's
        else
        {
            // Decrement height
            localRoot->setHeight(localRoot->getHeight() - 1);
            return 0;
        }
    }
}

/**
 * @brief Remove a node and update its subtree.
 * Removes the node at the given reference pointer and updates the pointer to the new local root.
 *
 * @param localRoot The node to be removed.
 */
int AVL::removeNode(Node *&localRoot)
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
            // No height updates necessary at this level or lower
            return 0;
        }

        // Case 2: Root has right child only
        else
        {
            Node *rmvNode = localRoot;
            localRoot = right;
            delete rmvNode;
            // No height updates necessary at this level or lower
            return 0;
        }
    }

    // Root has left child
    else
    {
        // Case 3: Root has left child only
        if (right == NULL)
        {
            Node *rmvNode = localRoot;
            localRoot = left;
            delete rmvNode;
            // No height updates necessary at this level or lower
            return 0;
        }

        // Case 4: Root has left and right children
        else
        {
            return removeNodeWith2Children(localRoot);
        }
    }
}

/**
 * @brief Remove a Node that has both a left and a right child.
 *
 * @param localRoot
 */
int AVL::removeNodeWith2Children(Node *&localRoot)
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
        // No height updates necessary at this level or lower
        return 0;
    }

    // Case 2: left has a right child
    else
    {
        // Find (the parent of) the new root
        Node *rootParent;
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

        return 0;
    }
}

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

        if (currentNode->getLeftChild() == NULL)
        {
            // Update height
            currentNode->setHeight(currentNode->getHeight() - 1);
            return true;
        }

        int lHeight = currentNode->getLeftChild()->getHeight();
        int rHeight = currentNode->getRightChild()->getHeight();
        if (lHeight > rHeight)
        {
            // Left subtree outweighs right. Don't update height
            return false;
        }

        // Update height
        currentNode->setHeight(currentNode->getHeight() - 1);
        return true;
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
//// toString()
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
