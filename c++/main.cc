#include <iostream> // std::cout
#include <memory> // unique_ptr
#include <stdexcept> // std::runtime_error
#include <utility> // std::pair

enum class method { push_back, push_front };

template <class TK, class TV>
class BinaryTree {
  // Elements from nested class are not visible from the container class. That's why we choose structs.
  struct Node {
    std::pair <TK,TV> keyVal;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
    Node(TK& key, TV& val, Node * l, Node * r) : keyVal{key, val}, left{l}, right{r} {}
    ~Node() = default;
  };
  // pointer to root node of the binary tree
  // by default it is initialize to nullptr
  std::unique_ptr<Node> root;

 public:

  void insert(const TK& key, const TV& val)
  {
    if(root == nullptr)
    {
      root.reset(new Node{key, val, nullptr, nullptr});
      return;
    }
    
  }

  void push_front(const T& t) {
    // auto old = head.release();
    // auto tmp = new Node{t,old};
    // head.reset(tmp);
    head.reset(new Node{t, head.release()});
  }
  void push_back(const T& t);

 public:
  template <class ot>
  friend std::ostream& operator<<(std::ostream&, const List<ot>&);
  List() = default;
  void push(const T& t, method m = method::push_front) {
    if (head == nullptr) {
      // Node * tmp = new Node{t,nullptr};
      // head.reset(tmp);
      head.reset(new Node{t, nullptr});
      return;
    }
    switch (m) {
      case method::push_back:
        push_back(t);
        break;

      case method::push_front:
        push_front(t);
        break;
      default:
        throw std::runtime_error{"unknown method\n"};
    };
  }
  class Iterator;
  class ConstIterator;
  Iterator begin() { return Iterator{head.get()}; }
  Iterator end() { return Iterator{nullptr}; }

  ConstIterator begin() const { return ConstIterator{head.get()}; }
  ConstIterator end() const { return ConstIterator{nullptr}; }
};

template <class T>
class List<T>::Iterator {
  using Node = List<T>::Node;

  Node* current;

 public:
  Iterator(Node* n) : current{n} {}
  T& operator*() const { return current->val; }
  Iterator& operator++() {
    current = current->next.get();
    return *this;
  }
  // Iterator operator++(int){
  //   Iterator it{*this};
  //   ++(*this);
  //   return it;
  // }
  bool operator==(const Iterator& other) { return current == other.current; }
  bool operator!=(const Iterator& other) { return !(*this == other); }
};

template <class T>
class List<T>::ConstIterator : public List<T>::Iterator {
 public:
  using parent = List<T>::Iterator;
  using parent::Iterator;
  const T& operator*() const { return parent::operator*(); }
  // using parent::operator==;
  // using parent::operator!=;
};

template <typename T>
void List<T>::push_back(const T& t) {
  auto tmp = head.get();
  while (tmp->next != nullptr) {  // equivalent while(tmp->next)
    tmp = tmp->next.get();
  }
  tmp->next.reset(new Node{t, nullptr});
}

template <class T>
std::ostream& operator<<(std::ostream& os, const List<T>& l) {
  // auto tmp = l.head.get();
  // while (tmp != nullptr) { 	// while(tmp)
  //   os << tmp->val << " ";
  //   tmp = tmp->next.get();
  // }
  // os<<std::endl;
  // return os;

  // auto it = l.begin();
  // auto stop = l.end();

  // for(; it!=stop; ++it)
  //   os << *it << " ";

  for (const auto& x : l)
    os << x << " ";

  return os;
}

int main() {
  List<int> list;
  list.push(3);                       // calls push_front
  list.push(5, method::push_back);    // calls push_back
  list.push(42, method::push_front);  // calls push_front

  for (auto& x : list) {
    ++x;
  }

  
  // for (auto& x : list) {
  //   ++x;
  // }
  std::cout << list;
  return 0;
}
