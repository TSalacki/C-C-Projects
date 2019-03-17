#ifndef AISDI_MAPS_TREEMAP_H
#define AISDI_MAPS_TREEMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>

namespace aisdi
{

template <typename KeyType, typename ValueType>
class TreeMap
{
public:
  using key_type = KeyType;
  using mapped_type = ValueType;
  using value_type = std::pair<const key_type, mapped_type>;
  using size_type = std::size_t;
  using reference = value_type&;
  using const_reference = const value_type&;
private:
  struct Node{
    Node *left;
    Node *right;
    Node *parent;
    value_type data;
    Node(const KeyType& key) : left(nullptr), right(nullptr), parent(nullptr), data(std::make_pair(key, mapped_type{}))
    {}
    Node(const KeyType& key, const ValueType& value) : left(nullptr), right(nullptr), parent(nullptr), data(std::make_pair(key, value))
    {}
  };
  Node *root; //pointer to root
  Node *first; //pointer to the first (sorted) element
  Node *last; //pointer to the last (sorted) element
  size_t tree_size; //number of tree elements
public:
  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;

  TreeMap() : root(nullptr), first(nullptr), last(nullptr), tree_size(0)
  {}

  ~TreeMap()
  {
      while (!isEmpty()) //we clear current tree
      remove(root->data.first);
  }

  TreeMap(std::initializer_list<value_type> list) : TreeMap()
  {
    for(auto it = list.begin(); it != list.end(); ++it)
    {
         (*this)[it->first]=it->second;
    }
    //(void)list; // disables "unused argument" warning, can be removed when method is implemented.
    //throw std::runtime_error("TODO1");
  }

  Node* copy(Node* copiedTree, Node* parent) //copying function
  {
    if(copiedTree==nullptr) return nullptr; //if we copied empty tree then return nullptr

    Node *newTree = new Node(copiedTree->data.first, copiedTree->data.second); //we copy node
    newTree->parent = parent; //we set parent of new node
    newTree->left = copy(copiedTree->left, newTree);  //we copy left subtree
    newTree->right = copy(copiedTree->right, newTree); //we copy right subtree
    return newTree;
  }

  TreeMap(const TreeMap& other) : TreeMap()
  {
    root=copy(other.root, nullptr); //we pass root and nullptr - becouse parent of root is null
    if(root) //we proceed only if we actually copied something
    {
      tree_size=other.tree_size; //we set number of elements
      //we set first
      Node *tmp = root;
      while(tmp->left) tmp=tmp->left;
      first = tmp;
      //and now we set last
      tmp=root;
      while(tmp->right) tmp=tmp->right;
      last=tmp;
    } 
    //(void)other;
    //throw std::runtime_error("TODO2");
  }

  TreeMap(TreeMap&& other)
  {
    root=other.root;
    first=other.first;
    last=other.last;
    tree_size=other.tree_size;
    other.tree_size=0;
    other.root=nullptr;
    //(void)other;
    //throw std::runtime_error("TODO3");
  }

  TreeMap& operator=(const TreeMap& other)
  {
    if(root!=other.root)  //we do sth only if we assign sth other
    {
      while (!isEmpty()) //we clear current tree
      remove(root->data.first);

      root=copy(other.root, nullptr); //and we copy! 
      if(root) //we proceed only if we actually copied something, then its like in copy constructor
      {
        tree_size=other.tree_size; //we set number of elements
        //we set first
        Node *tmp = root;
        while(tmp->left) tmp=tmp->left;
        first = tmp;
        //and now we set last
        tmp=root;
        while(tmp->right) tmp=tmp->right;
        last=tmp;
      } 
    }
    return *this;
    //(void)other;
    //throw std::runtime_error("TODO4");
  }

  TreeMap& operator=(TreeMap&& other)
  {
    while (!isEmpty()) //we clear current tree
    remove(root->data.first);

    root=other.root;
    first=other.first;
    last=other.last;
    tree_size=other.tree_size;
    other.tree_size=0;
    other.root=nullptr;
    
    return *this;
    //(void)other;
    //throw std::runtime_error("TODO5");
  }

  bool isEmpty() const
  {
    return !tree_size;
  }

  mapped_type& operator[](const key_type& key)
  {
    if(!root)
    {
      Node * NewElement = new Node(key);
      root = NewElement;
      first = NewElement;
      last = NewElement;
      ++tree_size;
      return root->data.second;
    }else
    {
      Node *tmp = root;
      if(key<first->data.first)
      {
        Node * NewElement = new Node(key);
        first->left = NewElement;
        NewElement->parent = first;
        first = NewElement;
        ++tree_size;
        return first->data.second;
      }else if(key>last->data.first)
      {
        Node * NewElement = new Node(key);
        last->right = NewElement;
        NewElement->parent = last;
        last = NewElement;
        ++tree_size;
        return last->data.second;
      }
      while(key!=tmp->data.first)
      {
        if(key>tmp->data.first) 
        {
          if(tmp->right) tmp = tmp->right;
          else
          {
            Node * NewElement = new Node(key);
            tmp->right = NewElement;
            NewElement->parent = tmp;
            ++tree_size;
            return tmp->right->data.second;
          } 
        }else
        {
          if(tmp->left) tmp = tmp->left;
          else
          {
            Node * NewElement = new Node(key);
            tmp->left = NewElement;
            NewElement->parent = tmp;
            ++tree_size;
            return tmp->left->data.second;
          }
        }
      }
      return tmp->data.second;
    }
    //(void)key;
    //throw std::runtime_error("TODO7");
  }

  const mapped_type& valueOf(const key_type& key) const
  {
    if(!root) throw std::out_of_range("Tree is empty!");
    else
    {
      Node *tmp=root;
      while(key!=tmp->data.first)
      {
        if(key>tmp->data.first)
        {
          if(tmp->right) tmp=tmp->right;
            else throw std::out_of_range("Tree is no such element!");
        }
        else
        {
          if(tmp->left) tmp=tmp->left;
            else throw std::out_of_range("Tree is no such element!");
        }
      }
      return tmp->data.second;
    }
    //(void)key;
    //throw std::runtime_error("TODO8");
  }

  mapped_type& valueOf(const key_type& key)
  {
    if(!root) throw std::out_of_range("Tree is empty!");
    else
    {
      Node *tmp=root;
      while(key!=tmp->data.first)
      {
        if(key>tmp->data.first)
        {
          if(tmp->right) tmp=tmp->right;
            else throw std::out_of_range("Tree is no such element!");
        }
        else
        {
          if(tmp->left) tmp=tmp->left;
            else throw std::out_of_range("Tree is no such element!");
        }
      }
      return tmp->data.second;
    }
    //(void)key;
    //throw std::runtime_error("TODO9");
  }

  const_iterator find(const key_type& key) const
  {
    Node *tmp=root;
    if(root==nullptr) //if tree is empty return null
    {
      return nullptr;
    }else             //if it's not empty, we search
    {
      while(tmp)
      {
        if(tmp->data.first>key)   //if key is smaller we go left
        {
            tmp=tmp->left;
        }else if(tmp->data.first<key)     //if key is bigger we go down
        {
            tmp=tmp->right;
        }else                     //otherwise we found it!
        {
            return ConstIterator(tmp, this);
        }
      }
    }
    return ConstIterator(tmp, this);
    //(void)key;
    //throw std::runtime_error("TODO10");
  }

  iterator find(const key_type& key)
  {
    Node *tmp=root;
    if(root==nullptr) //if tree is empty return null
    {
      this->begin();
    }else             //if it's not empty, we search
    {
      while(tmp)
      {
        if(tmp->data.first>key)   //if key is smaller we go left
        {
            tmp=tmp->left;
        }else if(tmp->data.first<key)     //if key is bigger we go down
        {
            tmp=tmp->right;
        }else                     //otherwise we found it!
        {
            return Iterator(tmp, this);
        }
      }
    }
    return Iterator(tmp, this);
    //(void)key;
    //throw std::runtime_error("TODO11");
  }

  void remove(const key_type& key)
  {
    if(!root) throw std::out_of_range("Tree is empty!");
    auto it=find(key); //we find our key
    if(it==nullptr) throw std::out_of_range("There is no such element!"); //if there is no such element we throw
    Node *tmp = it.ptr; //if we found our key, we obtain pointer to it
    --tree_size;
    if(tmp==first) first=tmp->parent;
    if(tmp==last) last=tmp->parent;
    if(tmp->left && tmp->right) //we have both subtrees
    {
      Node *smallest=tmp;
      smallest=tmp->right;
      while(smallest->left) smallest=smallest->left; //we find the smallest element from right subtree
      if(smallest == smallest->parent->left)
      {
        if(smallest->right) //we have to match pointers
        {
          smallest->parent->left=smallest->right;
          smallest->right->parent=smallest->parent;
        }
        else
          smallest->parent->left=nullptr;
      }else
      {
        if(smallest->right) //we have to match pointers
        {
          smallest->parent->right=smallest->right;
          smallest->right->parent=smallest->parent;
        }
        else
          smallest->parent->right=nullptr;
      }
      smallest->parent=tmp->parent;
      if(tmp->left!=smallest) //in case our right child is smallest
      {  
        smallest->left=tmp->left;
        if(tmp->left) tmp->left->parent=smallest;
      }
      else smallest->left=nullptr;

      if(tmp->right!=smallest) //in case our right child is smallest
      {  
        smallest->right=tmp->right;
        if(tmp->right) tmp->right->parent=smallest;
      }
      else smallest->right=nullptr;

      if(tmp==root){
        root=smallest;
        delete tmp;
        return;
      }
      if(tmp->parent->data.first>tmp->data.first) //if we are left child
        tmp->parent->left=smallest;
      else //otherwise we are right child
        tmp->parent->right=smallest;

      delete tmp;
      return;
    }else if(tmp->left) //we have only left subtree
    {
      if(tmp==root)
      {
        root=tmp->left;
        tmp->left->parent=nullptr;
        if(tmp==last) last=tmp->left;
        delete tmp;
        return;
      }
      if(tmp->parent->data.first>tmp->data.first) //if we are left child
        tmp->parent->left=tmp->left;
      else //otherwise we are right child
        tmp->parent->right=tmp->left;
      tmp->left->parent=tmp->parent;
      delete tmp;
      return;
    }else if(tmp->right) //we have only right subtree
    {
      if(tmp==root)
      {
        root=tmp->right;
        tmp->right->parent=nullptr;
        if(tmp==first) first=tmp->right;
        delete tmp;
        return;
      }
      if(tmp->parent->data.first>tmp->data.first) //if we are left child
        tmp->parent->left=tmp->right;
      else //we are right child
        tmp->parent->right=tmp->right;
      tmp->right->parent=tmp->parent;
      delete tmp;
      return; 
    }else //we are leaf!
    {
      if(tmp==root) 
      { 
          root=nullptr;
          delete tmp;
          return;
      }
      if(tmp->parent->data.first>tmp->data.first) //if we are left child
        tmp->parent->left=tmp->left;
      else //we are right child
        tmp->parent->right=tmp->left;
      delete tmp;
      return;
    }
  }

  void remove(const const_iterator& it)
  {
    remove(it->first);
    //(void)it;
    //throw std::runtime_error("TODO13");
  }

  size_type getSize() const
  {
    return tree_size;
    //throw std::runtime_error("TODO14");
  }

  bool operator==(const TreeMap& other) const
  {
    if(tree_size!=other.tree_size) return 0;
    auto it = begin();
    auto it2 = other.begin();
    for(; it!=end(); ++it, ++it2)
    {
      if(it->first!=it2->first || it->second!=it2->second) return 0;
    }
    return 1;
    //(void)other;
    //throw std::runtime_error("TODO15");
  }

  bool operator!=(const TreeMap& other) const
  {
    return !(*this == other);
  }

  iterator begin()
  {
    return Iterator(first, this);
    //throw std::runtime_error("TODO16");
  }

  iterator end()
  {
    return Iterator(nullptr, this);
    //throw std::runtime_error("TODO17");
  }

  const_iterator cbegin() const
  {
    return ConstIterator(first, this);
    //throw std::runtime_error("TODO18");
  }

  const_iterator cend() const
  {
    return ConstIterator(nullptr, this);
    //throw std::runtime_error("TODO19");
  }

  const_iterator begin() const
  {
    return cbegin();
  }

  const_iterator end() const
  {
    return cend();
  }

};

template <typename KeyType, typename ValueType>
class TreeMap<KeyType, ValueType>::ConstIterator
{
  friend class TreeMap<KeyType, ValueType>;
public:
  using reference = typename TreeMap::const_reference;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename TreeMap::value_type;
  using pointer = const typename TreeMap::value_type*;
protected:
  Node *ptr, *last_parent;
  TreeMap<KeyType, ValueType> const * treePtr;
public: 
  ConstIterator(Node* ptr = nullptr, TreeMap<KeyType, ValueType> const * treePtr = nullptr)  : ptr(ptr), treePtr (treePtr)
  {
    last_parent=nullptr;
  }

  ConstIterator(const ConstIterator& other) : ConstIterator(other.ptr, other.treePtr)
  {
    last_parent=other.last_parent;
    //throw std::runtime_error("TODO20");
  }

  Node *minValue(Node *n)
  {
    Node *curr=n;
    while(curr->left)
    {
      curr=curr->left;
    }
    return curr;
  }

  ConstIterator& operator++()
  {
    if(ptr==nullptr) throw std::out_of_range("Iterating after end!");
    if(ptr->right)
    {
      ptr = minValue(ptr->right);
      return *this;
    }
    Node *tmp=ptr->parent;
    while(tmp!=nullptr && ptr==tmp->right)
    {
      ptr=tmp;
      tmp=tmp->parent;
    }
    ptr=tmp;
    return *this;
    //throw std::runtime_error("TODO21");
  }

  ConstIterator operator++(int)
  {
    if(ptr==nullptr) throw std::out_of_range("Iterating after end!");
    auto tmp(*this);
    if(ptr->right)
    {
      ptr = minValue(ptr->right);
      return *this;
    }
    Node *tmp2=ptr->parent;
    while(tmp2!=nullptr && ptr==tmp2->right)
    {
      ptr=tmp2;
      tmp2=tmp2->parent;
    }
    ptr=tmp2;
    tmp=ptr;
    return tmp;
    //throw std::runtime_error("TODO22");
  }

  ConstIterator& operator--()
  {
    if(ptr==nullptr && treePtr->last==nullptr) throw std::out_of_range("Tree empty!");
    if(ptr==nullptr && last_parent==treePtr->first) throw std::out_of_range("Decrementing before start!");
    else{
      ptr=treePtr->last;
      return *this;
    }
    if(ptr->left)
    {
      ptr = minValue(ptr->left);
      return *this;
    }
    Node *tmp=ptr->parent;
    while(tmp!=nullptr && ptr==tmp->left)
    {
      ptr=tmp;
      tmp=tmp->parent;
    }
    ptr=tmp;
    last_parent=ptr;
    return *this;
    //throw std::runtime_error("TODO23");
  }

  ConstIterator operator--(int)
  {
    if(ptr==nullptr && treePtr->last==nullptr) throw std::out_of_range("Tree empty!");
    auto tmp(*this);
    if(ptr==nullptr && last_parent==treePtr->first) throw std::out_of_range("Decrementing before start!");
    else{
      ptr=treePtr->last;
      return *this;
    }
    if(ptr->left)
    {
      ptr = minValue(ptr->left);
      return *this;
    }
    Node *tmp2=ptr->parent;
    while(tmp2!=nullptr && ptr==tmp2->left)
    {
      ptr=tmp2;
      tmp2=tmp2->parent;
    }
    ptr=tmp2;
    last_parent=ptr;
    tmp=ptr;
    return tmp;
    //throw std::runtime_error("TODO24");
  }

  reference operator*() const
  { 
    if(ptr==nullptr) throw std::out_of_range("end of tree!");
    return ptr->data; 
    //throw std::runtime_error("TODO25");
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  bool operator==(const ConstIterator& other) const
  {
    return ptr==other.ptr;
    //(void)other;
    //throw std::runtime_error("TODO26");
  }

  bool operator!=(const ConstIterator& other) const
  {
    return !(*this == other);
  }
};

template <typename KeyType, typename ValueType>
class TreeMap<KeyType, ValueType>::Iterator : public TreeMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename TreeMap::reference;
  using pointer = typename TreeMap::value_type*;

  explicit Iterator(Node *ptr = nullptr, TreeMap<KeyType, ValueType> const * treePtr = nullptr): const_iterator(ptr, treePtr)
  {}

  Iterator(const ConstIterator& other)
    : ConstIterator(other)
  {}

  Iterator& operator++()
  {
    ConstIterator::operator++();
    return *this;
  }

  Iterator operator++(int)
  {
    auto result = *this;
    ConstIterator::operator++();
    return result;
  }

  Iterator& operator--()
  {
    ConstIterator::operator--();
    return *this;
  }

  Iterator operator--(int)
  {
    auto result = *this;
    ConstIterator::operator--();
    return result;
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  reference operator*() const
  {
    // ugly cast, yet reduces code duplication.
    return const_cast<reference>(ConstIterator::operator*());
  }
};

}

#endif /* AISDI_MAPS_MAP_H */
