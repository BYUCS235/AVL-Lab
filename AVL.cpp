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
#ifdef DEBUG
    std::cout << "add(" << data << ")" << std::endl;
#endif

    if (this->root == NULL)
    {
        this->root = new Node(data, nextId);
        this->nextId++;
#ifdef DEBUG
        std::cout << "Finished add(" << data << ")" << std::endl;
#endif

        return true;
    }
#ifdef DEBUG
    std::cout << "Finished add(" << data << ")" << std::endl;
#endif

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

        bool wasHeightUpdated = updateHeight(localRoot, updateChild, otherChild, 1);
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
            wasHeightUpdated = updateHeight(localRoot, updateChild, otherChild, 1);
        }
        return (wasHeightUpdated) ? SUCCESS_UPDATE : SUCCESS_NO_UPDATE;
    }
}

/**
 * @brief Update the height of the given node.
 * Updates height by the given increment, based on the heights of its children.
 *
 * Assumes localRoot is not null. Child node args can be null.
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

#ifdef DEBUG
    std::cout << "updateHeight(root=" << localRoot->id << ", u=" << updateChild->id << ", o=" << otherChild->id << ")" << std::endl;
#endif

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

    // Calculate heights
    int oldHeight = localRoot->getHeight();

    int updateChildHeight = updateChild->getHeight();
    int otherChildHeight = otherChild->getHeight();
    int maxChildHeight = (updateChildHeight > otherChildHeight) ? updateChildHeight : otherChildHeight;
    int newHeight = maxChildHeight + 1;

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
#ifdef DEBUG
    std::cout << "remove(" << data << ")" << std::endl;
    std::cout << this->toString() << std::endl
              << std::endl;
#endif

    bool rmvResult = removeFromSubtree(data, this->root);

#ifdef DEBUG
    std::cout << "Finished remove(" << data << "), returning removeFromSubtree value of " << rmvResult << std::endl;
#endif

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
    // Case 0: NULL
    if (localRoot == NULL)
    {
#ifdef DEBUG
        std::cout << "removeFromSubtree(" << data << ", null) - returning false" << std::endl;
#endif
        return FAIL; // Remove failed
    }

    // Case 1: left subtree
    else if (data < localRoot->data)
    {
#ifdef DEBUG
        std::cout << "removeFromSubtree(" << data << ", rootID=" << localRoot->id << ")" << std::endl;
#endif
        if (localRoot->getLeftChild() == NULL)
        {
#ifdef DEBUG
            std::cout << "left child is null, returning false" << std::endl;
#endif
            return FAIL; // Remove failed
        }

        else
        {
            // Update height
            Result result = updateHeightsAndRemove(localRoot,
                                                   localRoot->getLeftChildRef(), // rmvTreeRef
                                                   localRoot->getRightChild(),   // otherTree
                                                   data);
#ifdef DEBUG
            std::cout << "Finished removeFromSubtree(" << data << ", returning updateHeightsAndRemove() result of " << out << std::endl;
#endif
            return result;
        }
    }

    // Case 2: right subtree
    else if (data > localRoot->data)
    {
#ifdef DEBUG
        std::cout << "removeFromSubtree(" << data << ", rootID=" << localRoot->id << ")" << std::endl;
#endif
        if (localRoot->getRightChild() == NULL)
        {
#ifdef DEBUG
            std::cout << "right child is null, returning false" << std::endl;
#endif
            return FAIL; // Remove failed
        }

        else
        {
            // Update height
            Result result = updateHeightsAndRemove(localRoot,
                                                   localRoot->getRightChildRef(), // rmvTreeRef
                                                   localRoot->getLeftChild(),     // otherTree
                                                   data);
#ifdef DEBUG
            std::cout << "Finished removeFromSubtree(" << data << ", returning updateHeightsAndRemove() value " << out << std::endl;
#endif
            return result;
        }
    }

    // Case 3: current node is the node to remove
    else
    {
#ifdef DEBUG
        std::cout << "case 3, ";
#endif
        // Case 3A: Node to remove has no left child
        if (localRoot->getLeftChild() == NULL)
        {
#ifdef DEBUG
            std::cout << "no left child" << std::endl;
#endif
            removeNode(localRoot);
#ifdef DEBUG
            std::cout << "Finished removeFromSubtree(" << data << ", returning true" << std::endl;
#endif
            return SUCCESS_UPDATE; // Removed node successfully
        }

        // Case 3B: Node to remove has left child
        else
        {
#ifdef DEBUG
            std::cout << "has left child" << std::endl;
#endif
            // Update height
            // callRemoveNode=true: calls removeNode() instead of removeFromSubtree()
            // TODO Replace with its own code?

            removeNode(localRoot);
#ifdef DEBUG
            std::cout << "Finished removeFromSubtree(" << data << "), returning true" << std::endl;
#endif
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
#ifdef DEBUG
    int otherTreeId = (otherTree) ? otherTree->id : -1;
    std::cout << "updateHeightsAndRemove(rootID=" << localRoot->id << ", rmvID=" << rmvTreeRef->id << ", otherID=" << otherTreeId << ", rmvNode=" << callRemoveNode << ", data=" << data << ")" << std::endl;
#endif

    // Recurse
    Result childResult = removeFromSubtree(data, rmvTreeRef);

    // Return without updating if the remove failed
    if (childResult == FAIL)
    {
#ifdef DEBUG
        std::cout << "updateHeightsAndRemove: call to removeFromSubtree returned false, returning false" << std::endl;
#endif
        return FAIL;
    }
    bool wasHeightUpdated = true;
    if (childResult == SUCCESS_UPDATE)
    {
        wasHeightUpdated = updateHeight(localRoot, rmvTreeRef, otherTree, -1);
    }

#ifdef DEBUG
    std::cout << "Finished updateHeightsAndRemove(data=" << data << "), returning true" << std::endl;
#endif
    return (wasHeightUpdated) ? SUCCESS_UPDATE : SUCCESS_NO_UPDATE;
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
    std::cout << "removeNode(id=" << localRoot->id << ")" << std::endl;
#endif

    Node *left = localRoot->getLeftChild();
    Node *right = localRoot->getRightChild();

    // Root has no left child
    if (left == NULL)
    {
        // Case 1: Root has no children
        if (right == NULL)
        {
#ifdef DEBUG
            std::cout << "removeNode case 1" << std::endl;
#endif
            Node *rmvNode = localRoot;
            localRoot = NULL;
            delete rmvNode;
        }

        // Case 2: Root has right child only
        else
        {
#ifdef DEBUG
            std::cout << "removeNode case 2" << std::endl;
#endif
            Node *rmvNode = localRoot;
            localRoot = right;
            delete rmvNode;
        }
    }

    // Root does have a left child
    else
    {
        // Case 3: Root has left child only
#ifdef DEBUG
        std::cout << "removeNode case 3" << std::endl;
#endif

        if (right == NULL)
        {
            Node *rmvNode = localRoot;
            localRoot = left;
            delete rmvNode;
        }

        // Case 4: Root has left and right children
        else
        {
#ifdef DEBUG
            std::cout << "removeNode case 4" << std::endl;
#endif
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
#ifdef DEBUG
        std::cout << "removeNodeWith2Children case 1" << std::endl;
#endif
        Node *rmvNode = localRoot;
        localRoot = left;
        localRoot->setRightChild(right);
        delete rmvNode;
    }

    // Case 2: left does have a right child
    else
    {
#ifdef DEBUG
        std::cout << "removeNodeWith2Children case 2" << std::endl;
#endif
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
#ifdef DEBUG
    std::cout << "updateHeightsAndFindReplacement(id=" << currentNode->id << ")" << std::endl;
#endif
    // Base case
    if (currentNode->getRightChild()->getRightChild() == NULL)
    {
        rootParent = currentNode;
#ifdef DEBUG
        std::cout << "current.right.right==NULL, setting rootParent and returning true" << std::endl;
#endif
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
#ifdef DEBUG
            std::cout << "child didn't update, returning false" << std::endl;
#endif
            return false;
        }

        bool result = updateHeight(currentNode,                  // localRoot
                                   currentNode->getRightChild(), // updateChild
                                   currentNode->getLeftChild(),  // otherChild
                                   -1);                          // increment
#ifdef DEBUG
        std::cout << "Finished updateHeightsAndFindReplacement(id=" << currentNode->id << "), returning updateHeight() value of " << result << std::endl;
#endif
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
