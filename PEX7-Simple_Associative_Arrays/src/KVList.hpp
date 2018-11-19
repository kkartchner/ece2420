#ifndef KVLIST_HPP
#define KVLIST_HPP

#include "KeyVal.hpp"
#include <iostream>

/*********************************************************************
 * This is an example header without any implementation that you
 * may find useful in your effort to create an associative array.
 *
 * You are not required to conform to this header in any way, you are
 * only required to conform to the overall interface header provided
 * in KeyVal.hpp.
 *
 * However, there are helpful hints contained here that may make both
 * this PEX and future PEXes which build on the base interface easier
 * for you to implement.  A careful study of this architecture is 
 * highly recommended. *
 ********************************************************************/

//forward declaration
template<class K, class V>
class KVList;

/**
 * @class KVListNode
 *
 * The purpose of this class is to store key value pairs in a sorted list.
 * Note that all methods and members are private and only accessible by the
 * friend class KVList.
 *
 * The reason that the KVListNode class and the KVList class are 
 * separate is to make usage cleaner.  We must be able to create KVList 
 * objects on the stack just as we would any other object.  Since additions and
 * deletions to the list may change the root node, it is not effective to 
 * create a root node as the base object
 *
 * This separate private class also allows us to remove some default constuctors
 * and use only deep copy assignment in this class while exposing all options
 * in the public class.
 */
template<class K, class V>
class KVListNode : public std::enable_shared_from_this<KVListNode<K, V> >
{
friend class KVList<K, V>;
/*public: 
  ~KVListNode(){
    std::cout<< "Deleting <"<< *m_key_sp << "," << *m_val_sp << ">" << std::endl;
  }*/
private:
  /**
   * @brief Default Constructor
   */
  KVListNode() = default;
    
  /**
   * @brief Remove the copy constructor
   */
  KVListNode(const KVListNode &other) = delete;
  /**
   * @brief provide an assignment operator which deep copies our
   * data structure
   *
   * @param other Ref to RHS object; deep copy this structure
   * @return ListKeyNodeVal ref for chaining assignments
   */
  KVListNode &operator=(const KVListNode &other);
  /** @brief Ptr to key; may be null for last item in list */
  std::shared_ptr<K> m_key_sp;
  /** @brief Ptr to value; may be null for last item in list */
  std::shared_ptr<V> m_val_sp;
  /** @brief Ptr to next node in list; may be null for last item in list */
  std::shared_ptr<KVListNode> m_next_sp;
  /** @brief Weak ptr to prev node in list; may be null for first item in list 
       note that weak ref is used to avoid mem leak islands*/
  std::weak_ptr<KVListNode<K, V> > m_prev_wp;
};

/**
 * @class KVList
 */
template<class K, class V>
class KVList : public KeyVal<K, V>
{
public:
  /**
   * @brief Constructor
   * 
   * This ctor creates a valid root node
   */
  KVList<K,V>();
  /**
   * @brief Copy ctor
   *
   * Creates a deep copy of entire data structure
   *
   * @param other Data structure to copy
   */
  KVList<K,V>(const KVList<K,V> &other);
  /**
   * @brief Assignment operator
   *
   * Creates a deep copy of entire data structure
   *
   * @param other Data structure to copy
   * @return ref to populated object for assignment chaining
   */
  KVList<K,V> &operator=(const KVList<K,V> &other);
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
  virtual void insert(const K &key,
                      const V &val) override;
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
  virtual void del(const K &key) override;
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
  virtual std::shared_ptr<V> get(const K &key) override;
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
  virtual void forEach(std::function<void(const K &key, V &val)> callback) override;

private:
  /**
   * @brief Insert a node and return new root
   *
   * @param key Key to insert
   * @param val Value associated with key
   * @return New root of node list
   */
  std::shared_ptr<KVListNode <K, V> > insertInternal(const K &key,
                                                         const V &val);
  /**
   * @brief Delete a node and return new root
   *
   * @param key Key to insert
   * @return New root of node list
   */
  std::shared_ptr<KVListNode <K, V> > delInternal(const K &key);
  /** @breif Track root node */
  std::shared_ptr<KVListNode<K, V> > m_rootNode_sp;
};

#include "KVList.tpp"

#endif /* KVLIST_HPP */
