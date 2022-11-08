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

Node *&AVL::getRootNodeRef()
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
    Result result = addToSubtree(data, this->root);
    return result != FAIL;
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
Result AVL::addToSubtree(int data, Node *&localRoot)
{
    Result result;

    // Case 1: left subtree
    if (data < localRoot->getData())
    {
        result = updateHeightsAndAddToSubtree(data, localRoot,
                                              localRoot->getLeftChildRef(), true);
    }

    // Case 2: right subtree
    else if (data > localRoot->getData())
    {
        result = updateHeightsAndAddToSubtree(data, localRoot,
                                              localRoot->getRightChildRef(), false);
    }

    // Case 3: current node (i.e. duplicate)
    else
    {
        result = FAIL; // Failed to add node
    }

    return result;
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
Result AVL::updateHeightsAndAddToSubtree(int data, Node *&localRoot, Node *&updateChild, int isLeft)
{
    Result result = FAIL;

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

        this->nextId++;
    }
    else
    {
        Result childResult = addToSubtree(data, updateChild);

        // Return without updating if add failed
        if (childResult == FAIL)
        {
            return FAIL;
        }
    }

    bool wasHeightUpdated = updateHeight(localRoot);
    result = (wasHeightUpdated) ? SUCCESS_UPDATE : SUCCESS_NO_UPDATE; // Added node successfully

    if (localRoot != NULL)
    {
        rebalance(localRoot);
    }

    return result;
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
        newHeight = Node::LEAF_HEIGHT;
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
    Result result = removeFromSubtree(data, this->root);
    return result != FAIL;
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
    Result result;

    if (localRoot == NULL)
    {
        // Case 0: NULL
        result = FAIL; // Remove failed
    }
    else if (data < localRoot->data)
    {
        // Case 1: left subtree
        if (localRoot->getLeftChild() == NULL)
        {
            result = FAIL; // Remove failed
        }
        else
        {
            // Update height
            result = updateHeightsAndRemove(localRoot,
                                            localRoot->getLeftChildRef(), // rmvTreeRef
                                            localRoot->getRightChild(),   // otherTree
                                            data);
        }
    }
    else if (data > localRoot->data)
    {
        // Case 2: right subtree
        if (localRoot->getRightChild() == NULL)
        {
            result = FAIL; // Remove failed
        }
        else
        {
            // Update height
            result = updateHeightsAndRemove(localRoot,
                                            localRoot->getRightChildRef(), // rmvTreeRef
                                            localRoot->getLeftChild(),     // otherTree
                                            data);
        }
    }
    else
    {
        // Case 3: current node is the node to remove
        removeNode(localRoot);
        result = SUCCESS_UPDATE; // Removed node successfully
    }

    if (localRoot != NULL)
    {
        rebalance(localRoot);
    }

    return result;
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
    Result result;

    // Recurse
    Result childResult = removeFromSubtree(data, rmvTreeRef);

    if (childResult == FAIL)
    {
        result = FAIL;
    }
    else
    {
        bool wasHeightUpdated = updateHeight(localRoot);
        result = (wasHeightUpdated) ? SUCCESS_UPDATE : SUCCESS_NO_UPDATE;
    }

    return result;
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
        // Cases 1 and 2: Root has no children/right child only (no rebalance needed?)
        Node *rmvNode = rmvNodeRef;
        rmvNodeRef = right;
        delete rmvNode;
    }
    else
    {
        if (right == NULL)
        {
            // Case 3: Root has left child only (no rebalance needed?)
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

    if (left->getRightChild() == NULL)
    {
        // Case 1: left has no right child

        Node *rmvNode = rmvNodeRef;
        rmvNodeRef = left;
        rmvNodeRef->setRightChild(right);
        delete rmvNode;
    }
    else
    {
        // Case 2: left does have a right child

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
    }

    // Set new root's height and rebalance
    updateHeight(rmvNodeRef);
    if (rmvNodeRef != NULL)
    {
        rebalance(rmvNodeRef);
    }
}

/**
 * @brief Search for the node to replace a removed node, updating heights.
 * Originally called on the left child of the node to be removed. Recursively searches down the right subtree of currentNode until finding a node (the parent of the new root node) that has no right-right grandchild, then stores it in rootParent and updates rmvNodeRef. Balances nodes along  the way.
 *
 * Requires currentNode->getRightChild() != NULL and rootParent == NULL.
 *
 * @param currentNode The current node.
 * @param rootParent A reference pointer which will be set to point to the parent node of the new root.
 * @param rmvNodeRef The reference pointer to the node to be removed.
 * @return true if the current node's height was updated.
 * @return false otherwise, i.e. if the left child's height was much greater than the right child's.
 */
bool AVL::updateHeightsAndFindReplacement(Node *currentNode, Node *&rootParent)
{
    bool heightUpdated = true;

    // Base case: found the new root
    if (currentNode->getRightChild()->getRightChild() == NULL)
    {
        rootParent = currentNode;
        heightUpdated = updateRootParentHeight(currentNode);
    }
    else
    {
        // Recurse
        bool childUpdated = updateHeightsAndFindReplacement(currentNode->getRightChild(), rootParent);

        // Update height and rebalance if necessary

        if (!childUpdated)
        {
            heightUpdated = false;
        }
        else
        {
            heightUpdated = updateHeight(currentNode);
        }
    }
    rebalance(currentNode); // TODO Rebalance AFTER updating pointers
    return heightUpdated;
}

void AVL::swapNodes(Node *&node, Node *&other)
{
    // Save node's children
    Node *nodeL = node->getLeftChild();
    Node *nodeR = node->getRightChild();

    // Swap children
    node->setLeftChild(other->getLeftChild());
    node->setRightChild(other->getRightChild());
    other->setLeftChild(nodeL);
    other->setRightChild(nodeR);

    // Update parent references
    Node *nodePtr = node; // Save pointer, not ref
    node = other;
    other = nodePtr;
}

void AVL::deleteRmvNode(Node *&rmvNodeRef) {
    Node *rmvNode = rmvNodeRef;
    rmvNodeRef = rmvNode->getLeftChild();
    delete rmvNode;
}

bool AVL::updateRootParentHeight(Node *rootParent)
{
    // TODO Remove and replace with updateHeight()?
    Node *right = rootParent->getRightChild()->getLeftChild();
    Node *left = rootParent->getLeftChild();
    int oldHeight = rootParent->getHeight();
    int newHeight;

    if (right == NULL && left == NULL) // No children
    {
        newHeight = 0;
    }
    else if (right == NULL) // Left child only
    {
        newHeight = left->getHeight() + 1;
    }
    else if (left == NULL) // Right child only
    {
        newHeight = right->getHeight() + 1;
    }
    else // Two children
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
//    rebalancing functions
////

/**
 * @brief Rebalances the given node's subtree.
 *
 * @param localRoot The node to rebalance.
 * @return true if the tree was updated.
 * @return false if the tree was already balanced and nothing changed.
 */
bool AVL::rebalance(Node *&localRoot)
{
    if (localRoot->getBalance() >= 2)
    {
        if (localRoot->getRightChild()->getBalance() < 0)
        {
            // RL tree
            rotateRight(localRoot->getRightChildRef()); // Convert RL -> RR
        }
        // RR tree
        rotateLeft(localRoot);
        return true;
    }
    else if (localRoot->getBalance() <= -2)
    {
        if (localRoot->getLeftChild()->getBalance() > 0)
        {
            // LR tree
            rotateLeft(localRoot->getLeftChildRef()); // Convert LR -> LL
        }
        // LL tree
        rotateRight(localRoot);
        return true;
    }

    return false;
}

/**
 * @brief Rotate the AVL tree left around the given node.
 * Assumes pivot's right child is non-null.
 *
 * @param pivot The node to pivot around.
 */
void AVL::rotateLeft(Node *&pivot)
{
    Node *origin = pivot; // Pointer to the pivot node, not a ref to the pivot location

    Node *right = origin->getRightChild();
    Node *center = right->getLeftChild(); // Changes from right's left child to origin's right child

    // Update references
    origin->setRightChild(center);
    right->setLeftChild(origin);
    pivot = right;

    // Update heights
    updateHeight(origin);
    updateHeight(right);
}

/**
 * @brief Rotate the AVL tree right around the given node.
 * Assumes pivot's left child is non-null.
 *
 * @param pivot The node to pivot around.
 */
void AVL::rotateRight(Node *&pivot)
{
    Node *origin = pivot; // Pointer to the pivot node, not a ref to the pivot location

    Node *left = origin->getLeftChild();
    Node *center = left->getRightChild(); // Changes from left's right child to origin's left child

    // Update references
    origin->setLeftChild(center);
    left->setRightChild(origin);
    pivot = left;

    // Update heights
    updateHeight(origin);
    updateHeight(left);
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
