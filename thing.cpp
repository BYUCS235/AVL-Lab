#include "BST.h"

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
int BST::updateHeightsAndRemove(Node *&localRoot, Node *&rmvTreeRef, Node *otherTree, bool callRemoveNode, int data = NULL)
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
