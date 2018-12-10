/*************************************KVRBTreeNode************************************/
/**
 * @brief Override the assignment operator so that it creates
 * a deep copy of the RHS object.
 *
 * @param other Ref to RHS object; deep copy this structure
 * @return *this ref for chaining assignments
 */
    template<class K, class V>
KVRBTreeNode<K,V>& KVRBTreeNode<K,V>::operator=(const KVRBTreeNode<K,V> &other)
{
    if (other.m_key_sp){
        //        std::cout << *(other.m_key_sp) << std::endl;
        //
        m_key_sp.reset(new K(*(other.m_key_sp)));   
        m_val_sp.reset(new V(*(other.m_val_sp))); 

        //        if (other.m_leftChild_sp){ 
        //            *m_leftChild_sp = *(other.m_leftChild_sp);
        //        }
        //        if (other.m_rightChild_sp){
        //            *m_rightChild_sp = *(other.m_rightChild_sp);
        //        }
    }   

    return *this;
}

    template<class K, class V>
std::shared_ptr<KVRBTreeNode<K,V> > KVRBTreeNode<K,V>::parent()
{
    return m_parent_wp.lock();
}

    template<class K, class V>
std::shared_ptr<KVRBTreeNode<K,V> > KVRBTreeNode<K,V>::grandparent()
{
    auto p = parent();
    return p ? p->parent() : nullptr;
}

    template<class K, class V>
std::shared_ptr<KVRBTreeNode<K,V> > KVRBTreeNode<K,V>::uncle()
{
    auto gp = grandparent();
    if (gp){
        bool parentIsLeftChild = *(parent()->m_key_sp) < *(gp->m_key_sp);

        if (parentIsLeftChild){
            return gp->m_rightChild_sp;
        } else {
            return gp->m_leftChild_sp;
        }
    }
    return nullptr;
}

    template<class K, class V>
std::shared_ptr<KVRBTreeNode<K,V> > KVRBTreeNode<K,V>::sibling()
{
}
/***************************************KVRBTree*************************************/
/**
 * @brief Constructor
 * 
 * This ctor creates a valid root node
 */
    template<class K, class V>
KVRBTree<K,V>::KVRBTree()
{
    m_rootNode_sp.reset(new KVRBTreeNode<K,V>);
}
/**
 * @brief C opy ctor
 *
 * Creates a deep copy of entire data structure
 *
 * @param other Data structure to copy
 */
    template<class K, class V>
KVRBTree<K,V>::KVRBTree(const KVRBTree<K,V> &other)
{
    *this = other;    
}
/**
 * @brief Assignment operator
 *
 * Creates a deep copy of entire data structure
 *
 * @param other Data structure to copy
 * @return ref to populated object for assignment chaining
 */
    template<class K, class V>
KVRBTree<K,V>& KVRBTree<K,V>::operator=(const KVRBTree<K,V> &other)
{
    if (this == &other){ // Prevent potential problems from self-assignment
        return *this;
    }

    m_rootNode_sp = copyPreOrder(other.m_rootNode_sp); // copyPreOrder(other.m_rootNode_sp);

    return *this;
}

/**
 * @brief Preorder traversal to copy the entire binary tree
 *
 * @param curNode The node currently being copied and traversed
 * @return curNode Return curNode for linking up the tree
 */
    template<class K, class V>
std::shared_ptr<KVRBTreeNode<K,V> > KVRBTree<K,V>::copyPreOrder(const std::shared_ptr<KVRBTreeNode<K,V> > curNode)
{
    if (curNode == nullptr) // Return nullptr if curNode is null
        return nullptr;

    std::shared_ptr<KVRBTreeNode<K,V> > newNode(new KVRBTreeNode<K,V>); // Create new node on the heap
    *newNode = *curNode; // Deep copy curNode into newNode

    // Assign newNode's leftChild to copy of curNode's leftChild:   
    newNode->m_leftChild_sp = copyPreOrder(curNode->m_leftChild_sp);

    // Assign newNode's rightChild to copy of curNode's righChild:
    newNode->m_rightChild_sp = copyPreOrder(curNode->m_rightChild_sp); 

    return newNode;
}

/**
 * @brief Insert an object
 *
 * Calls insertInternal to place a COPY of the val object
 * into the associative array and update the root node if needed.
 * Overwrites the value at key if key is already in use.
 *
 * @param key Key associated with value
 * @param val Value which is stored at location key
 */
    template<class K, class V>
void KVRBTree<K,V>::insert(const K &key, const V &val)
{
    m_rootNode_sp = insertInternal(key, val);
    //   std::cout << *(m_rootNode_sp->m_key_sp) << std::endl;
} 
/**
 * @brief Remove an object from the associative array
 *
 * Calls the delInternal function to remove the key/value pair from the array
 * and update the root node if needed. If the key is not found, no action is taken.
 *
 * @param key Key for which key/val pair is removed
 */
    template<class K, class V>
void KVRBTree<K,V>::del(const K &key)
{
    m_rootNode_sp = delInternal(key);
} 
/**
 * @brief Get a pointer to the value at key
 *
 * Returns a shared_ptr to the value located at the
 * provided key if it exists. Otherwise, returns a nullptr.
 *
 * @param key Key for which value is returned
 * @return ptr to value if key existed
 */
    template<class K, class V>
std::shared_ptr<V> KVRBTree<K,V>::get(const K &key)
{
    auto curNode = m_rootNode_sp;

    while (curNode->m_key_sp){ 
        // Loop while curNode is not null
        if (key == *(curNode->m_key_sp)){ 
            // If key equal's curNode's key, the key has been found so return
            // the shared_ptr to the value at curNode
            return curNode->m_val_sp;
        } else if (key < *(curNode->m_key_sp)){
            // Else if key is less than curNode key, go left on the tree.
            curNode = curNode->m_leftChild_sp;
        } else { 
            // Else key is greater than curNode key, so go right on the tree.
            curNode = curNode->m_rightChild_sp;
        }
    }
    return nullptr; // Return nullptr if key not found in tree
} 
/**
 * @brief Execute callback for each entry
 *
 * Calls the provided callback function on every item in the associative array. 
 * Sorted least to greatet
 *
 * @param callback Function to be called with each item in the associative array
 */
    template<class K, class V>
void KVRBTree<K,V>::forEach(std::function<void(const K &key,V &val)> callback)
{
    traverseInOrder(m_rootNode_sp, callback);   
} 

    template<class K, class V>
void KVRBTree<K,V>::traverseInOrder(std::shared_ptr<KVRBTreeNode<K,V> > node,
        std::function<void(const K &key, V &val)> callback)
{
    if (!node->m_key_sp)
        return;
    traverseInOrder(node->m_leftChild_sp, callback);
    //  auto lkey = node->m_leftChild_sp->m_key_sp;
    //  std::cout <<"[" << (lkey ? *lkey : "null") << ", ";
    callback(*(node->m_key_sp), *(node->m_val_sp));
    //    auto rkey = node->m_rightChild_sp->m_key_sp;
    //    std::cout << ", " << (rkey ? *rkey : "null") << "]" << std::endl;
    //
    traverseInOrder(node->m_rightChild_sp, callback); 

}

    template<class K, class V>
void KVRBTree<K,V>::printTree()
{
    std::list<std::shared_ptr<KVRBTreeNode<K,V> > > workList;
    std::list<int> levelList;
    workList.push_back(m_rootNode_sp);
    levelList.push_back(0);

    while (!workList.empty())
    {
        auto pCurrent = workList.front();
        auto level = levelList.front();
        auto pParent = pCurrent->parent();
        //       auto gParent = pCurrent->grandparent();
        //       auto uncle = pCurrent->uncle();
        auto color = (pCurrent->m_color == Color::Red) ? "RED" : "BLACK";

        workList.pop_front();
        levelList.pop_front();

        std::cout << "{K:"<< *(pCurrent->m_key_sp) << ", ";
        std::cout << "C:"<< color << ", ";
        std::cout << "L:"<< level << ", ";

        if (pParent)
            std::cout << "PK:"<< *(pParent->m_key_sp) << "}";
        else
            std::cout << "PK:NULL}";
        //       if (gParent)
        //           std::cout << ", GPK:"<< *(gParent->m_key_sp);
        //       else
        //           std::cout << ", GPK:NULL";
        //       if (uncle && uncle->m_key_sp)
        //           std::cout << ", UK:"<< *(uncle->m_key_sp) << "} ";
        //       else
        //           std::cout << ", UK:NULL} ";

        if (pCurrent->m_leftChild_sp->m_key_sp)
        {
            workList.push_back(pCurrent->m_leftChild_sp);
            levelList.push_back(level + 1);
        }

        if (pCurrent->m_rightChild_sp->m_key_sp)
        {
            workList.push_back(pCurrent->m_rightChild_sp);
            levelList.push_back(level + 1);
        }

        if (!levelList.empty() && (level != levelList.front())){
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}


/**
 * @brief Insert a node and return new root
 *
 * @param key Key to insert
 * @param val Value associated with key
 * @return New root of node list
 */
    template<class K, class V>
std::shared_ptr<KVRBTreeNode <K,V> > KVRBTree<K,V>::insertInternal(const K &key, const V &val)
{
    std::shared_ptr<KVRBTreeNode<K,V> > curNode = m_rootNode_sp, prevNode = nullptr, root = m_rootNode_sp;

    // Find where newNode should be inserted into the list (ordered least to greatest by key)
    while (curNode->m_key_sp){
        if (key < *(curNode->m_key_sp)){
            prevNode = curNode;
            curNode = curNode->m_leftChild_sp;
        } else if (key > *(curNode->m_key_sp)) {
            prevNode = curNode;
            curNode = curNode->m_rightChild_sp;
        } else {
            // Key already in use. Replace curNode val with new val.
            *(curNode->m_val_sp) = val;

            return root;
        } 
    }

    curNode->m_key_sp.reset(new K(key));
    curNode->m_val_sp.reset(new V(val));
    curNode->m_color = Color::Red;

    curNode->m_leftChild_sp.reset(new KVRBTreeNode<K,V>(curNode));
    curNode->m_rightChild_sp.reset(new KVRBTreeNode<K,V>(curNode));

    handleRedUncles(curNode);
   // handleBlackUncles(curNode);

    return root;
}

    template<class K, class V>
void KVRBTree<K,V>::handleRedUncles(std::shared_ptr<KVRBTreeNode<K,V> > curNode)
{
    if (curNode == m_rootNode_sp){
        m_rootNode_sp->m_color = Color::Black;
        return;
    }
    
    auto parent = curNode->parent();
    auto uncle = curNode->uncle();
    auto gp = curNode->grandparent();
    
    if (parent && parent->isRed()){
        if (uncle && uncle->isRed()){
            curNode->parent()->m_color = Color::Black;
            curNode->uncle()->m_color = Color::Black;

            gp->m_color = Color::Red;

            handleRedUncles(gp);
        }
    }
}

    template<class K, class V>
void KVRBTree<K,V>::handleBlackUncles(std::shared_ptr<KVRBTreeNode<K,V> > curNode)
{
    auto parent = curNode->parent();
    auto uncle = curNode->uncle();
    auto gp = curNode->grandparent();
    
    bool curIsLeftChild = *(curNode->m_key_sp) < *(parent->m_key_sp);
    bool parentIsLeftChild = (*(parent->m_key_sp) < *(gp->m_key_sp));
    auto subRoot = gp;
    if (parent && parent->isRed()){
        if (uncle && uncle->isBlack()){
            if (parentIsLeftChild && curIsLeftChild){
                // left-left
                subRoot = rightRotate(gp);
            } else if (parentIsLeftChild){
                // left-right case
                subRoot = doubleLeftRight(gp);    
                
            } else if (curIsLeftChild){
                // right-left case
                subRoot = doubleRighLeft(gp);
            } else {
                // right-right case
                subRoot = leftRotate(gp);
            }
            if (*(m_rootNode_sp->m_key_sp) == *(subRoot->m_key_sp)){
                m_rootNode_sp = subRoot;
            }
        }
    }
}

    template<class K, class V>
void KVRBTree<K,V>::connectNodes(std::shared_ptr<KVRBTreeNode<K,V> > parent,
        std::shared_ptr<KVRBTreeNode<K,V> > child)
{
    parent = child;
    child->m_parent_wp = parent;
}
/**
 * @brief Perform a right rotation on the specified node
 *  
 * @param curNode The node to perform rotation on
 */
    template<class K, class V>
std::shared_ptr<KVRBTreeNode<K,V> > KVRBTree<K,V>::rightRotate(
        std::shared_ptr<KVRBTreeNode<K,V> > curNode)
{
    auto leftChild = curNode->m_leftChild_sp; // Store current left child
    connectNodes(curNode->m_leftChild_sp, leftChild->m_rightChild_sp);
    connectNodes(leftChild->m_rightChild_sp, curNode);

    return leftChild;
}
/**
 * @brief Perform a right rotation on the specified node
 *  
 * @param curNode The node to perform rotation on
 */
    template<class K, class V>
std::shared_ptr<KVRBTreeNode<K,V> > KVRBTree<K,V>::leftRotate(
        std::shared_ptr<KVRBTreeNode<K,V> > curNode)
{
    auto rightChild = curNode->m_rightChild_sp;
    connectNodes(curNode->m_rightChild_sp, rightChild->m_leftChild_sp);
    connectNodes(rightChild->m_leftChild_sp, curNode);

    return rightChild;
}
/**
 * @brief Perform a right rotation on the specified node
 *  
 * @param curNode The node to perform rotation on
 */
    template<class K, class V>
std::shared_ptr<KVRBTreeNode<K,V> > KVRBTree<K,V>::doubleLeftRight(
        std::shared_ptr<KVRBTreeNode<K,V> > curNode)
{
    connectNodes(curNode->m_leftChild_sp, leftRotate(curNode->m_leftChild_sp));

    return rightRotate(curNode);
}
/**
 * @brief Perform a right rotation on the specified node
 *  
 * @param curNode The node to perform rotation on
 */
    template<class K, class V>
std::shared_ptr<KVRBTreeNode<K,V> > KVRBTree<K,V>::doubleRighLeft(
        std::shared_ptr<KVRBTreeNode<K,V> > curNode)
{
    connectNodes(curNode->m_rightChild_sp, rightRotate(curNode->m_rightChild_sp));

    return leftRotate(curNode);
}

/**
 * @brief Delete a node and return new root
 *
 * @param key Key to insert
 * @return New root of node list
 */
    template<class K, class V>
std::shared_ptr<KVRBTreeNode <K,V> > KVRBTree<K,V>::delInternal(const K &key)
{
    std::shared_ptr<KVRBTreeNode<K,V> > curNode = m_rootNode_sp, prevNode = nullptr,
        root = m_rootNode_sp;
    if (root){
        while (curNode){
            if (key < *(curNode->m_key_sp)){
                // Key less than curNode's key
                prevNode = curNode;
                curNode = curNode->m_leftChild_sp;
            } else if (key > *(curNode->m_key_sp)) {
                // Key greater than curNode's key
                prevNode = curNode;
                curNode = curNode->m_rightChild_sp;
            } else { 
                // Key equal to curNode's key. Key found, so break.
                break;
            } 
        }

        if (curNode){
            if (curNode->m_leftChild_sp && curNode->m_rightChild_sp) { 
                std::cout << "both children" << std::endl; 
                // Case: Has both a left and right child
                // Replace curNode with successor (lowest right-side leaf node)

                auto startNode = prevNode = curNode; // Shared ptrs for tracking

                curNode = curNode->m_rightChild_sp; // Go right once

                // Then go left as far as possible to find successor:
                bool wentLeft = false;
                while (curNode->m_leftChild_sp) {
                    prevNode = curNode;
                    curNode = curNode->m_leftChild_sp;
                    wentLeft = true;
                }

                // Change startNode key and value to successor's key and value:
                *(startNode->m_key_sp) = *(curNode->m_key_sp);
                *(startNode->m_val_sp) = *(curNode->m_val_sp); 

                // Remove the successor node (curNode) from the tree:
                if (wentLeft){
                    prevNode->m_leftChild_sp = curNode->m_rightChild_sp;
                } else {
                    prevNode->m_rightChild_sp = curNode->m_rightChild_sp;
                }
            }

            else if (curNode->m_leftChild_sp){

                std::cout << "only left child" << std::endl;
                // Case: Has only a left child
                // Replace curNode with it's left child 

                if (curNode == root){
                    // If curNode is the root
                    root = curNode->m_leftChild_sp;
                } else {
                    if (*(curNode->m_key_sp) > *(prevNode->m_key_sp)) {
                        // If curNode is on the right of parent
                        prevNode->m_rightChild_sp = curNode->m_leftChild_sp;
                    } else {
                        // If curNode is on the left of parent
                        prevNode->m_leftChild_sp = curNode->m_leftChild_sp;
                    }
                    //curNode->m_leftChild_sp->m_parent_wp = prevNode;
                }
            } 

            else if (curNode->m_rightChild_sp){
                std::cout << "only right child" << std::endl;
                // Case: Has only a right child
                // Replace curNode with it's right child 

                if (curNode == root){
                    // If curNode is the root
                    root = curNode->m_rightChild_sp;
                } else {
                    if (*(curNode->m_key_sp) > *(prevNode->m_key_sp)) {
                        // If curNode is on the right of parent
                        prevNode->m_rightChild_sp = curNode->m_rightChild_sp;
                    } else {
                        // If curNode is on the left of parent
                        prevNode->m_leftChild_sp = curNode->m_rightChild_sp;
                    }
                    // curNode->m_rightChild_sp->m_parent_wp = prevNode;
                }
            }

            else { 
                std::cout << "no children" << std::endl;
                // Case: Has no children
                // Simply remove the node from it's parent

                if (curNode == root){
                    // If curNode is the root
                    root = nullptr;
                } else {
                    if (*(curNode->m_key_sp) > *(prevNode->m_key_sp)){
                        // If curNode is on right of parent
                        prevNode->m_rightChild_sp = nullptr;
                    } else {
                        // If curNode is on left of parent
                        prevNode->m_leftChild_sp = nullptr;
                    }
                }
            }
        }   
        else {
            std::cout << "key " << key << " not found" << std::endl;
        }
    }

    return root;
}
