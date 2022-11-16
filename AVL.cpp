#ifndef BST_CPP_T2K_10_17_2022
#define BST_CPP_T2K_10_17_2022

#include <iostream>
#include "AVL.h"

std::string cprint(Node *node);

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

    // Case 0: NULL
    if (localRoot == NULL)
    {
        localRoot = new Node(data, nextId);
        nextId++;
        result = SUCCESS_UPDATE;
    }

    // Case 1: left subtree
    else if (data < localRoot->getData())
    {
        result = addToSubtree(data, localRoot->getLeftChildRef());
    }

    // Case 2: right subtree
    else if (data > localRoot->getData())
    {
        result = addToSubtree(data, localRoot->getRightChildRef());
    }

    // Case 3: current node (i.e. duplicate)
    else
    {
        result = FAIL; // Failed to add node
    }

    rebalance(localRoot);

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
            result = removeFromSubtree(data, localRoot->getLeftChildRef());
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
            result = removeFromSubtree(data, localRoot->getRightChildRef());
        }
    }
    else
    {
        // Case 3: current node is the node to remove
        removeNode(localRoot);
        result = SUCCESS_UPDATE; // Removed node successfully
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

    if (left == NULL)
    // Node to remove has no left child
    {
        Node *rmvNode = rmvNodeRef;
        rmvNodeRef = right;
        delete rmvNode;
    }
    else
    // Node to remove does have a left child
    {
        if (left->getRightChild() != NULL) // Left has a right child
        {
            removeSwap(rmvNodeRef, rmvNodeRef->getLeftChildRef());
        }
        else // Left does not have a right child
        {
            Node *rmvNode = rmvNodeRef;
            rmvNodeRef = left;
            rmvNodeRef->setRightChild(right);
            delete rmvNode;
        }
        rebalance(rmvNodeRef);
    }
}

std::string cprint(Node *node)
{
    if (node == NULL)
        return "NULL";

    return std::to_string(node->getData());
}

/**
 * @brief Perform a swap on a node to be removed, updating heights and rebalancing.
 * Originally called on the left child of the node to be removed. Recursively searches down the right subtree of currentParent until finding a node (the parent of the new root node) that has no right-right grandchild, then stores it in currentParent and updates rmvNodeRef. Balances nodes along  the way.
 *
 * Requires currentParent->getRightChild() != NULL and currentParent == NULL.
 *
 * @param rmvNodeRef The reference pointer to the node to be removed.
 * @param currentParent The current node.
 * @return true if the current node's height was updated.
 * @return false otherwise, i.e. if the left child's height was much greater than the right child's.
 */
Result AVL::removeSwap(Node *&rmvNodeRef, Node *&currentParent)
{
    Result result = FAIL;

    // Base case: found the new root's parent
    if (currentParent->getRightChild()->getRightChild() == NULL)
    {
        Node *&replaceRef = currentParent->getRightChildRef();
        swapNodes(replaceRef, rmvNodeRef); // Swap rmvNode with replacement node
        deleteRmvNode(replaceRef);         // Once rmvNode is in replacement location, it can be safely deleted
    }
    else
    {
        // Recurse
        result = removeSwap(rmvNodeRef, currentParent->getRightChildRef());
    }

    rebalance(currentParent);

    return result;
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

void AVL::deleteRmvNode(Node *&rmvNodeRef)
{
    Node *rmvNode = rmvNodeRef;
    rmvNodeRef = rmvNode->getLeftChild();
    delete rmvNode;
}

////
//    rebalancing functions
////

/**
 * @brief Rebalances the given node's subtree.
 *
 * @param localRoot The node to rebalance.
 * @return FAIL if the tree was already balanced and nothing changed.
 * @return SUCCESS_UPDATE if the tree was updated and the root's height was changed.
 * @return SUCCESS_NO_UPDATE if the tree was updated but the root's height did not change.
 */
Result AVL::rebalance(Node *&localRoot)
{
    if (localRoot == NULL)
    {
        return FAIL;
    }
    updateHeight(localRoot);

    int oldHeight = localRoot->getHeight();
    if (localRoot->getBalance() >= 2)
    {
        if (localRoot->getRightChild()->getBalance() < 0)
        {
            // RL tree
            rotateRight(localRoot->getRightChildRef()); // Convert RL -> RR
        }
        // RR tree
        rotateLeft(localRoot);
        return (localRoot->getHeight() == oldHeight) ? SUCCESS_NO_UPDATE : SUCCESS_UPDATE;
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
        return (localRoot->getHeight() == oldHeight) ? SUCCESS_NO_UPDATE : SUCCESS_UPDATE;
    }

    return FAIL;
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
