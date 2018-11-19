template<class K, class V>
/**
 * @brief provide an assignment operator which deep copies our
 * data structure
 *
 * @param other Ref to RHS object; deep copy this structure
 * @return ListKeyNodeVal ref for chaining assignments
 */
KVListNode<K,V>& KVListNode<K,V>::operator=(const KVListNode<K,V> &other)
{
    std::cout << "KVListNode assn operator called." << std::endl;
    m_key_sp.reset(new K(*(other.m_key_sp)));   
    m_val_sp.reset(new V(*(other.m_val_sp))); 

    return *this;
}

/**
 * @brief Constructor
 * 
 * This ctor creates a valid root node
 */
    template<class K, class V>
KVList<K,V>::KVList()
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
KVList<K,V>::KVList(const KVList<K,V> &other)
{
    std::cout << "copy ctor called." << std::endl;
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
KVList<K,V>& KVList<K,V>::operator=(const KVList<K,V> &other)
{
    std::cout << "assignment operator called." << std::endl;
    if (this == &other){ // Prevent potential problems from self-assignment
        return *this;
    }

    m_rootNode_sp.reset(new KVListNode<K,V>); 
    *m_rootNode_sp = *(other.m_rootNode_sp); // Copy the root node from other to this

    auto curOther = other.m_rootNode_sp->m_next_sp,
         prevNode = m_rootNode_sp;

    while (curOther != nullptr){
        std::shared_ptr<KVListNode<K,V> > newNode(new KVListNode<K,V>);
        *newNode = *curOther; // Copy current node of other into newNode

        prevNode->m_next_sp = newNode; // Connect newNode to previous node
        newNode->m_prev_wp = prevNode;

        prevNode = newNode; // Advance to next node in other list for copying
        curOther = curOther->m_next_sp;
    }

    return *this;
}
/**
 * @brief Insert an object
 *
 * This will place a COPY of the val object
 * into the associative array
 *
 * Note that since an insert may change the root node, I have 
 * created an "internal" function that returns the new root.
 * this function will call the internal version and then reset
 * the root node if needed.  This model will make follow on
 * PEXs where recursion is required more clean/understandable.
 *
 * @param key Key associated with value
 * @param val Value which is stored at location key
 */
    template<class K, class V>
void KVList<K,V>::insert(const K &key, const V &val)
{
    m_rootNode_sp = insertInternal(key, val);
} 
/**
 * @brief Remove an object from the associative array
 *
 * This will remove the key/value pair from the array
 * If the key is not found, no action is taken
 *
 * Note that since a delete may change the root node, I have 
 * created an "internal" function that returns the new root.
 * this function will call the internal version and then reset
 * the root node if needed.  This model will make follow on
 * PEXs where recursion is required more clean/understandable.
 *
 * @param key Key for which key/val pair is removed
 */
    template<class K, class V>
void KVList<K,V>::del(const K &key)
{
    m_rootNode_sp = delInternal(key);
} 
/**
 * @brief Get a pointer to value
 *
 * Given a key, a shared_ptr to a value is returned.
 * note that if the key did not exist, then the returned
 * ptr is not valid
 *
 * @param key Key for which value is returned
 * @return ptr to value if key existed
 */
    template<class K, class V>
std::shared_ptr<V> KVList<K,V>::get(const K &key)
{
    auto curNode = m_rootNode_sp;
    while (curNode && key > *(curNode->m_key_sp)){ // Iterate to where the key should be
        curNode = curNode->m_next_sp;
    }

    if (!curNode || *(curNode->m_key_sp) != key){ // If curNode is null, or curNode's value does not equal
        // the key provided
        std::cout << "Key \"" << key << "\" not found in list. Get unsuccessful." << std::endl;
        return nullptr;
    } else { // curNode is not null and value is found
        std::cout << "Value \""<< *(curNode->m_val_sp) << "\" found at key \"" << key << "\" "<<  std::endl;

        return curNode->m_val_sp;
    }
} 
/**
 * @brief Execute callback for each entry
 *
 * Rather than force students to create an inner iterator class,
 * this functiona allows a callback function to be executed for
 * every item in the associative array.  Note that callbacks should
 * be called in order of keys sorted least to greatest
 *
 * @param callback Function to be called with each item in the associative array
 */
    template<class K, class V>
void KVList<K,V>::forEach(std::function<void(const K &key,V &val)> callback)
{
    for (auto cur = m_rootNode_sp; cur != nullptr; cur = cur->m_next_sp){
        callback(*(cur->m_key_sp), *(cur->m_val_sp));
    }
} 
/**
 * @brief Insert a node and return new root
 *
 * @param key Key to insert
 * @param val Value associated with key
 * @return New root of node list
 */
    template<class K, class V>
std::shared_ptr<KVListNode <K,V> > KVList<K,V>::insertInternal(const K &key, const V &val)
{
    std::shared_ptr<KVListNode<K,V> > newNode(new KVListNode<K,V>),
        curNode = m_rootNode_sp, prevNode = nullptr, root = m_rootNode_sp;

    newNode->m_key_sp.reset(new K(key));
    newNode->m_val_sp.reset(new V(val));

    // Find where newNode should be inserted into the list (ordered least to greatest by key)
    while (curNode && key > *(curNode->m_key_sp)){
        prevNode = curNode;
        curNode = curNode->m_next_sp;
    }

    if (curNode && key == *(curNode->m_key_sp)){ // If key already exists
        std::cout << "Key \"" << key << "\" already in use."
            " Key/val not inserted." << std::endl;
    } else { // Correctly insert newNode before curNode:
        newNode->m_next_sp = curNode; 
        if (curNode){ // curNode is not null
            curNode->m_prev_wp = newNode;
        }
        if (curNode == m_rootNode_sp){ // If newNode was inserted before the root node
            root = newNode;     //     return as the new root node
        } else {                
            prevNode->m_next_sp = newNode;
            newNode->m_prev_wp = prevNode;
        }

        std::cout << "Key \"" << key << "\" with value \"" << val << "\""
            " successfully inserted." << std::endl;

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
std::shared_ptr<KVListNode <K,V> > KVList<K,V>::delInternal(const K &key)
{
    std::shared_ptr<KVListNode<K,V> > curNode = m_rootNode_sp, prevNode = nullptr,
        root = m_rootNode_sp;

    while (curNode && key > *(curNode->m_key_sp)){
        prevNode = curNode;
        curNode = curNode->m_next_sp;
    }

    if (!curNode || *(curNode->m_key_sp) != key){ // If curNode is null, or curNode's value does not equal
        // the key provided
        std::cout << "Key \"" << key << "\" not found in list. Remove unsuccessful." << std::endl;

    } else { // curNode is not null and value is found
        // Delete the key/val pair:
        if (prevNode){                      // If prevNode is not null 
            prevNode->m_next_sp = curNode->m_next_sp; //      Remove curNode by changing prevNode's m_next_sp
            //      from curNode to curNode's m_next_sp
            if (curNode->m_next_sp){           // If curNode's next is not null
                curNode->m_next_sp->m_prev_wp = prevNode; // Link it back to prevNode
            }

        } else {    // Else the node being deleted is the root node
            root = curNode->m_next_sp; // So return the node after curNode as the new root
        }

        std::cout << "Key \"" << key << "\" and associated value successfully removed." << std::endl;

    }

    return root;
}
