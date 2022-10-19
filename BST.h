#ifndef BST_H_T2K_10_17_2022
#define BST_H_T2K_10_17_2022

#include <sstream>
#include "AVLInterface.h"
#include "Node.h"

class BST : AVLInterface
{
private:
    Node *root;
    int nextId;

    void clearSubtree(Node *&localRoot);
    bool addToSubtree(int data, Node *localRoot);
    void addSubtreeToSStream(Node *localRoot, std::stringstream &ss, std::string prefix) const;

    // Remove helper functions
    int removeFromSubtree(int data, Node *&localRoot);
    int BST::removeFromSubtreeAndUpdateHeights(int data, Node *&localRoot, Node *&rmvTreeRef, Node *otherTree);
    Node *removeNode(Node *&localRoot);
    Node *removeNodeWith2Children(Node *&localRoot);

public:
    BST();
    ~BST();

    Node *getRootNode() const;
    bool add(int data);
    bool remove(int data);
    void clear();
    std::string toString() const;
};

#endif
