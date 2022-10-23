#ifndef BST_H_T2K_10_17_2022
#define BST_H_T2K_10_17_2022

#include <sstream>
#include "AVLInterface.h"
#include "Node.h"

class AVL : AVLInterface
{
private:
    Node *root;
    int nextId;

    void clearSubtree(Node *&localRoot);
    bool addToSubtree(int data, Node *localRoot);
    void addSubtreeToSStream(Node *localRoot, std::stringstream &ss, std::string prefix) const;

    // Remove helper functions
    int removeFromSubtree(int data, Node *&localRoot);
    int updateHeightsAndRemove(Node *&localRoot, Node *&rmvTreeRef, Node *otherTree, bool callRemoveNode, int data);
    int removeNode(Node *&localRoot);
    int removeNodeWith2Children(Node *&localRoot);
    bool updateHeightsAndFindReplacement(Node *currentNode, Node *&rmvLocation);

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
