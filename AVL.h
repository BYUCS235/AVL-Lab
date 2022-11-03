#ifndef BST_H_T2K_10_17_2022
#define BST_H_T2K_10_17_2022

#include <sstream>
#include "AVLInterface.h"
#include "Node.h"

enum Result
{
    FAIL = 1,
    SUCCESS_UPDATE = 2,
    SUCCESS_NO_UPDATE = 3
};

class AVL : AVLInterface
{
private:
    Node *root;
    int nextId;

    void clearSubtree(Node *&localRoot);
    bool updateHeight(Node *localRoot);
    void addSubtreeToSStream(Node *localRoot, std::stringstream &ss, std::string prefix) const;

    // rebalancing functions
    bool rebalance(Node *&pivot);
    void rotateLeft(Node *&pivot);
    void rotateRight(Node *&pivot);

    // add() helper functions
    Result addToSubtree(int data, Node *localRoot);
    Result updateHeightsAndAddToSubtree(int data, Node *localRoot, Node *updateChild, Node *otherChild, int isLeft);

    // remove() helper functions
    Result removeFromSubtree(int data, Node *&localRoot);
    Result updateHeightsAndRemove(Node *&localRoot, Node *&rmvTreeRef, Node *otherTree, int data);
    void removeNode(Node *&localRoot);
    void removeNodeWith2Children(Node *&localRoot);
    bool updateHeightsAndFindReplacement(Node *currentNode, Node *&rootParent);
    bool updateRootParentHeight(Node *rootParent);

public:
    AVL();
    ~AVL();

    Node *getRootNode() const;
    bool add(int data);
    bool remove(int data);
    void clear();
    std::string toString() const;
};

#endif
