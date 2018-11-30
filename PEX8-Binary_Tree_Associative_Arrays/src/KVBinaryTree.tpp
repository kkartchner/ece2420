/**
 * @brief Override the assignment operator so that it creates
 * a deep copy of the RHS object.
 *
 * @param other Ref to RHS object; deep copy this structure
 * @return *this ref for chaining assignments
 */
    template<class K, class V>
KVBinaryTreeNode<K,V>& KVBinaryTreeNode<K,V>::operator=(const KVBinaryTreeNode<K,V> &other)
{
    m_key_sp.reset(new K(*(other.m_key_sp)));   
    m_val_sp.reset(new V(*(other.m_val_sp))); 

    return *this;
}
/**
 * @brief Constructor
 */
    template<class K, class V>
KVBinaryTree<K,V>::KVBinaryTree()
{
    m_rootNode_sp = nullptr;
}
/**
 * @brief Copy ctor
 *
 * Creates a deep copy of entire data structure
 *
 * @param other Data structure to copy
 */
    template<class K, class V>
KVBinaryTree<K,V>::KVBinaryTree(const KVBinaryTree<K,V> &other)
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
KVBinaryTree<K,V>& KVBinaryTree<K,V>::operator=(const KVBinaryTree<K,V> &other)
{
    if (this == &other){ // Prevent potential problems from self-assignment
        return *this;
    }

    m_rootNode_sp = copyPreOrder(other.m_rootNode_sp);

    return *this;
}
/**
 * @brief Preorder traversal to copy the entire binary tree
 *
 * @param curNode The node currently being copied and traversed
 * @return curNode Return curNode for linking up the tree
 */
    template<class K, class V>
std::shared_ptr<KVBinaryTreeNode<K,V> > KVBinaryTree<K,V>::copyPreOrder(const std::shared_ptr<KVBinaryTreeNode<K,V> > curNode)
{
    if (curNode == nullptr) // Return nullptr if curNode is null
        return nullptr;

    std::shared_ptr<KVBinaryTreeNode<K,V> > newNode(new KVBinaryTreeNode<K,V>); // Create new node on the heap
    *newNode = *curNode; // Deep copy curNode into newNode

    // Assign newNode's leftChild to copy of curNode's leftChild:   
    newNode->m_leftChild_sp = copyPreOrder(curNode->m_leftChild_sp);
    1
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
void KVBinaryTree<K,V>::insert(const K &key, const V &val)
{
    m_rootNode_sp = insertInternal(key, val);
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
void KVBinaryTree<K,V>::del(const K &key)
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
std::shared_ptr<V> KVBinaryTree<K,V>::get(const K &key)
{
    auto curNode = m_rootNode_sp;

    while (curNode){ // Loop while curNode is not null:
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
 * Sorted least to greatest.
 *
 * @param callback Function to be called with each item in the associative array
 */
    template<class K, class V>
void KVBinaryTree<K,V>::forEach(std::function<void(const K &key,V &val)> callback)
{
    traverseInOrder(m_rootNode_sp, callback);   
} 
/**
 * @brief Traverse the binary tree in order, and call the provided callback 
 * function on each one. 
 *
 * @param curNode The node currently being traversed.
 * @param callback The callback function to execute
 */
    template<class K, class V>
void KVBinaryTree<K,V>::traverseInOrder(std::shared_ptr<KVBinaryTreeNode<K,V> > curNode,
        std::function<void(const K &key, V &val)> callback)
{
    if (curNode == nullptr)
        return;

    // Traverse left child subtree:
    traverseInOrder(curNode->m_leftChild_sp, callback);

    // Execute callback on current node:
    callback(*(curNode->m_key_sp), *(curNode->m_val_sp));

    // Traverse right child subtree:
    traverseInOrder(curNode->m_rightChild_sp, callback); 
}


/**
 * @brief Insert a node and return new root
 *
 * @param key Key to insert
 * @param val Value associated with key
 * @return New root of node list
 */
    template<class K, class V>
std::shared_ptr<KVBinaryTreeNode <K,V> > KVBinaryTree<K,V>::insertInternal(const K &key, const V &val)
{
    std::shared_ptr<KVBinaryTreeNode<K,V> > newNode(new KVBinaryTreeNode<K,V>),
        curNode = m_rootNode_sp, prevNode = nullptr, root = m_rootNode_sp;

    newNode->m_key_sp.reset(new K(key));
    newNode->m_val_sp.reset(new V(val));

    if (root == nullptr){
        root = newNode;
    } else {
        // Find where newNode should be inserted into the list (ordered least to greatest by key)
        while (curNode){
            if (key < *(curNode->m_key_sp)){ // Key less than curNode's key
                prevNode = curNode;
                curNode = curNode->m_leftChild_sp;
            } else if (key > *(curNode->m_key_sp)) { // Key greater than curNode's key
                prevNode = curNode;
                curNode = curNode->m_rightChild_sp;
            } else {
                // Key already in use. Replace curNode val with new val.
                *(curNode->m_val_sp) = val;

                return root;
            } 
        }

        // Correctly insert newNode before curNode:
        if (key < *(prevNode->m_key_sp)){         // newNode on left of parent
            prevNode->m_leftChild_sp = newNode;
        } else {                                  // newNode on right of parent
            prevNode->m_rightChild_sp = newNode;
        }
    }
    return root;
}
/**
 * @brief Delete a node and return new root
 *
 * @param key Key to insert
 * @return New root of node list
 */
    template<class K, class V>
std::shared_ptr<KVBinaryTreeNode <K,V> > KVBinaryTree<K,V>::delInternal(const K &key)
{
    std::shared_ptr<KVBinaryTreeNode<K,V> > curNode = m_rootNode_sp, prevNode = nullptr,
        root = m_rootNode_sp;
    if (root){
        while (curNode){
            if (key < *(curNode->m_key_sp)){ // Key less than curNode's key
                prevNode = curNode;
                curNode = curNode->m_leftChild_sp;
            } else if (key > *(curNode->m_key_sp)) {  // Key greater than curNode's key
                prevNode = curNode;
                curNode = curNode->m_rightChild_sp;
            } else { 
                // Key equal to curNode's key. Key found, so break.
                break;
            } 
        }

        if (curNode){
            if (curNode->m_leftChild_sp && curNode->m_rightChild_sp) { 
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
                }
            } 

            else if (curNode->m_rightChild_sp){
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
                }
            }

            else { 
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
    }

    return root;
}
