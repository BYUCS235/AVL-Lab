int BST::removeFromSubtreeAndUpdateHeights(int data, Node *&localRoot, Node *&rmvTreeRef, Node *otherTree)
{
    // Save rmvTree's old height for comparison
    int prevRmvTreeHeight = rmvTreeRef->getHeight();

    // Recurse down rmvTree
    int result = removeFromSubtree(data, rmvTreeRef);

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
