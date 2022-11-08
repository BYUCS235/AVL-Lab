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

    // rebalance() helper functions
    void rotateLeft(Node *&pivot);
    void rotateRight(Node *&pivot);

    // add() helper functions
    Result addToSubtree(int data, Node *&localRoot);
    Result updateHeightsAndAddToSubtree(int data, Node *&localRoot, Node *&updateChild, int isLeft);

    // remove() helper functions
    Result removeFromSubtree(int data, Node *&localRoot);
    Result updateHeightsAndRemove(Node *&localRoot, Node *&rmvTreeRef, Node *otherTree, int data);
    void removeNode(Node *&localRoot);
    void removeNodeWith2Children(Node *&localRoot);
    bool updateHeightsAndFindReplacement(Node *currentNode, Node *&rootParent);
    bool updateHeightsAndReplace(Node *currentNode, Node *&rootParent, Node *&rmvNodeRef);
    bool updateRootParentHeight(Node *rootParent);

public:
    AVL();
    ~AVL();

    Node *getRootNode() const;
    Node *&getRootNodeRef();
    bool add(int data);
    bool remove(int data);
    bool rebalance(Node *&pivot);
    void clear();
    std::string toString() const;
};

#endif
