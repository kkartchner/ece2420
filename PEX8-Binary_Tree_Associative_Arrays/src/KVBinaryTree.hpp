#ifndef KVLIST_HPP
#define KVLIST_HPP

#include "KeyVal.hpp"
#include <iostream>

//forward declaration
template<class K, class V>
class KVBinaryTree;

/**
 * @class KVBinaryTreeNode
 *
 * The purpose of this class is to store key value pairs in a sorted list.
 * Note that all methods and members are private and only accessible by the
 * friend class KVBinaryTree.
 *
 * The reason that the KVBinaryTreeNode class and the KVBinaryTree class are 
 * separate is to make usage cleaner.  We must be able to create KVBinaryTree 
 * objects on the stack just as we would any other object.  Since additions and
 * deletions to the list may change the root node, it is not effective to 
 * create a root node as the base object
 *
 * This separate private class also allows us to remove some default constuctors
 * and use only deep copy assignment in this class while exposing all options
 * in the public class.
 */
template<class K, class V>
class KVBinaryTreeNode : public std::enable_shared_from_this<KVBinaryTreeNode<K, V> >
{
    friend class KVBinaryTree<K, V>;
    private:
    /**
     * @brief Default Constructor
     */
    KVBinaryTreeNode() = default;

    /**
     * @brief Remove the copy constructor
     */
    KVBinaryTreeNode(const KVBinaryTreeNode &other) = delete;
    /**
     * @brief Override the assignment operator so that it creates
     * a deep copy of the RHS object.
     *
     * @param other Ref to RHS object; deep copy this structure
     * @return *this ref for chaining assignments
     */
    KVBinaryTreeNode &operator=(const KVBinaryTreeNode &other);
    /** @brief Ptr to key; may be null for last item in list */
    std::shared_ptr<K> m_key_sp;
    /** @brief Ptr to value; may be null for last item in list */
    std::shared_ptr<V> m_val_sp;
    /** @brief Ptr to next node in list; may be null for last item in list */
    std::shared_ptr<KVBinaryTreeNode> m_leftChild_sp;
    /** @brief Ptr to the right child */
    std::shared_ptr<KVBinaryTreeNode> m_rightChild_sp;
    /** @brief Weak ptr to parent node; may be null for first item in list 
      note that weak ref is used to avoid mem leak islands*/
    std::weak_ptr<KVBinaryTreeNode<K, V> > m_parent_wp;
};

/**
 * @class KVBinaryTree
 */
template<class K, class V>
class KVBinaryTree : public KeyVal<K, V>
{
    public:
        /**
         * @brief Constructor
         * 
         * This ctor creates a valid root node
         */
        KVBinaryTree<K,V>();
        /**
         * @brief Copy ctor
         *
         * Creates a deep copy of entire data structure
         *
         * @param other Data structure to copy
         */
        KVBinaryTree<K,V>(const KVBinaryTree<K,V> &other);
        /**
         * @brief Assignment operator
         *
         * Creates a deep copy of entire data structure
         *
         * @param other Data structure to copy
         * @return ref to populated object for assignment chaining
         */
        KVBinaryTree<K,V> &operator=(const KVBinaryTree<K,V> &other);
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
        virtual void insert(const K &key,
                const V &val) override;
        /**
         * @brief Remove an object from the associative array
         *
         * Calls the delInternal function to remove the key/value pair from the array
         * and update the root node if needed. If the key is not found, no action is taken.
         *
         * @param key Key for which key/val pair is removed
         */
        virtual void del(const K &key) override;
        /**
         * @brief Get a pointer to the value at key
         *
         * Returns a shared_ptr to the value located at the
         * provided key if it exists. Otherwise, returns a nullptr.
         *
         * @param key Key for which value is returned
         * @return ptr to value if key existed
         */
        virtual std::shared_ptr<V> get(const K &key) override;
        /**
         * @brief Execute callback for each entry
         *
         * Calls the provided callback function on every item in the associative array. 
         * Sorted least to greatest
         *
         * @param callback Function to be called with each item in the associative array
         */
        virtual void forEach(std::function<void(const K &key, V &val)> callback) override;

    private:
        /**
         * @brief Insert a node and return new root
         *
         * @param key Key to insert
         * @param val Value associated with key
         * @return New root of node list
         */
        std::shared_ptr<KVBinaryTreeNode <K, V> > insertInternal(const K &key,
                const V &val);
        /**
         * @brief Delete a node and return new root
         *
         * @param key Key to insert
         * @return New root of node list
         */
        std::shared_ptr<KVBinaryTreeNode<K, V> > delInternal(const K &key);
        /** @brief Track root node */
        std::shared_ptr<KVBinaryTreeNode<K, V> > m_rootNode_sp;

        /**
         * @brief Traverse the binary tree in order, and call the provided callback 
         * function on each one. 
         *
         * @param curNode The node currently being traversed.
         * @param callback The callback function to execute
         */
        void traverseInOrder(std::shared_ptr<KVBinaryTreeNode<K,V> > curNode,
                std::function<void(const K &key, V &val)> callback);

        /**
         * @brief Recursive preorder traversal for copying the entire binary tree
         *
         * @param curNode The node currently being copied and traversed
         * @return curNode Return curNode for linking up the tree
         */
        std::shared_ptr<KVBinaryTreeNode<K,V> > copyPreOrder(const std::shared_ptr<KVBinaryTreeNode<K,V> > curNode);
};

#include "KVBinaryTree.tpp"

#endif /* KVLIST_HPP */
