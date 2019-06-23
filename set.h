#include<iostream>
#include<vector>

template<class ValueType>
class Set{
    struct Node{
	ValueType key;
	size_t height;
	Node* left;
	Node* right;
	Node* anc;
	Node(ValueType k) {
	    key = k;
	    left = right = 0;
	    height = 1;
	    anc = nullptr;
	}
    };
    int height(Node* v) {
	return v ? v->height : 0;
    }
    int balance(Node* v) {
	return (int)height(v->right) - (int)height(v->left);
    }
    void update(Node* v) {
	v->height = std::max(height(v->left), height(v->right)) + 1;
	if (v->left != nullptr) {
	    v->left->anc = v;
	}
	if (v->right != nullptr) {
	    v->right->anc = v;
	}
    }
    Node* right_rotate(Node* v) {
	Node* ancv = v->anc;
	Node* l = v->left;
	v->left = l->right;
	l->right = v;
	l->anc = ancv;
	update(l);
	update(v);
	return l;
    }
    Node* left_rotate(Node* v) {
	Node* ancv = v->anc;
	Node* r = v->right;
	v->right = r->left;
	r->left = v;
	r->anc = ancv;
	update(v);
	update(r);
	return r;
    }
    Node* fix(Node* v) {
	update(v);
	if (balance(v) == 2) {
	    if (balance(v->right) < 0) {
		v->right = right_rotate(v->right);
	    }
	    return left_rotate(v);
	} else if (balance(v) == -2) {
	    if (balance(v->left) > 0) {
		v->left = left_rotate(v->left);
	    }
	    return right_rotate(v);
	}
	return v;
    }
    bool in(Node* v, ValueType k) const {
	if (v == 0) {
	    return false;
	}
	if (!(v->key < k) && !(k < v->key)) {
	    return true;
	} else if (v->key < k) {
	    return in(v->right, k);
	} else {
	    return in(v->left, k);
	}
    }
    Node* back_insert(Node* v, ValueType k) {
	if (!v) {
	    return new Node(k);
	}
	if (!(v->key < k) && !(k < v->key)) {
	    return v;
	}
	if (k < v->key) {
	    Node* newleft = back_insert(v->left, k);
	    v->left = newleft;
	} else {
	    Node* newright = back_insert(v->right, k);
	    v->right = newright;
	}
	return fix(v);
    }
    Node* get_min(Node* v) const {
	if (v == 0) {
	    return 0;
	}
	if (v->left) {
	    return(get_min(v->left));
	}
	return v;
    }
    Node* remove_min(Node* v) {
	if (v->left == 0) {
	    return v->right;
	}
	v->left = remove_min(v->left);
	return fix(v);
    }
    Node* back_erase(Node* v, ValueType k) {
	if (v == 0) {
	    return 0;
	}
	if (k < v->key) {
	    v->left = back_erase(v->left, k);
	} else if (v->key < k) {
	    v->right = back_erase(v->right, k);
	} else {
	    Node* l = v->left;
	    Node* r = v->right;
	    delete v;
	    if (r == 0) {
		return l;
	    }
	    Node* mn = get_min(r);
	    mn->right = remove_min(r);
	    mn->left = l;
	    return fix(mn);
	}
	return fix(v);
    }
    Node* root;
    size_t treeSize;
    void del(Node* v) {
	if (v == 0) {
	    return;
	}
	del(v->left);
	del(v->right);
	delete v;
    }
    Node* back_upper(Node* v, Node* elem) const {
	if (v == 0) {
	    return 0;
	}
	if (elem == 0) {
	    return 0;
	}
	if (!(v->key < elem->key) && !(elem->key < v->key)) {
	    return get_min(v->right);
	} else if (!(v->key < elem->key)) {
	    Node* ansleft = back_upper(v->left, elem);
	    if (ansleft == 0) {
		return v;
	    } else {
		return ansleft;
	    }
	} else {
	    return back_upper(v->right, elem);
	}
    }
    Node* back_lower(Node* v, Node* elem) const {
	if (v == 0) {
	    return 0;
	}
	if (elem == 0) {
	    return 0;
	}
	if (!(v->key < elem->key) && !(elem->key < v->key)) {
	    return get_max(v->left);
	} else if (v->key < elem->key) {
	    Node* ansright = back_lower(v->right, elem);
	    if (ansright == 0) {
		return v;
	    } else {
		return ansright;
	    }
	} else {
	    return back_lower(v->left, elem);
	}
    }
    Node* get_max(Node* v) const {
	if (v == nullptr) {
	    return nullptr;
	}
	if (v->right != nullptr) {
	    return get_max(v->right);
	}
	return v;
    }
public:
    class iterator{
	Node* elem;
	const Set* set;
	bool isend;
    public:
	iterator() : elem(nullptr), set(nullptr), isend(false) {}
	iterator(const Set* set, Node* elem, bool isend) : elem(elem), set(set), isend(isend) {}
	iterator& operator=(const iterator& b) {
	    if (*this == b) {
		return *this;
	    }
	    elem = b.elem;
	    set = b.set;
	    isend = b.isend;
	    return *this;
	}
	iterator& operator++() {
	    elem = set->back_upper(set->root, elem);
	    if (elem == 0) {
		isend = true;
	    }
	    return *this;
	}
	iterator& operator--() {
	    if (isend) {
		elem = set->get_max(set->root);
		isend = false;
	    } else {
		elem = set->back_lower(set->root, elem);
	    }
	    return *this;
	}
	iterator& operator++(int) {
	    iterator save = *this;
	    ++(*this);
	    return save;
	}
	iterator& operator--(int) {
	    iterator save = *this;
	    --(*this);
	    return save;
	}
	bool operator==(const iterator& b) {
	    if (b.isend == true && isend == false) {
		return false;
	    } else if (b.isend == false && isend == true) {
		return false;
	    } else if (b.isend == true && isend == true) {
		return true;
	    } else {
		return elem == b.elem;
	    }
	}
	bool operator!=(const iterator& b) {
	    return !(*this == b);
	}
	const ValueType operator*() const {
	    if (elem == nullptr) {
		return ValueType();
	    }
	    return elem->key;
	}
	const ValueType* operator->() const {
	    return &(elem->key);
	}
    };
    Set() {
	root = nullptr;
	treeSize = 0;
    }
    template <typename InputIterator>
    Set(InputIterator begin, InputIterator end) {
	root = nullptr;
	treeSize = 0;
	while(begin != end) {
	    insert(*begin);
	    begin++;
	}
    }
    Set(const std::initializer_list<ValueType> &list) {
	root = nullptr;
	treeSize = 0;
	for (auto elem: list) {
	    insert(elem);
	}
    }
    Set(const Set & other) {
	treeSize = 0;
	root = nullptr;
	for (auto elem : other) {
	    insert(elem);
	}
    }
    void dfs_insert(const Node* v) {
	if (v == nullptr) {
	    return;
	}
	insert(v->key);
	dfs_insert(v->left);
	dfs_insert(v->right);
    }
    Set& operator=(const Set& b) {
	if (b.root == root) {
	    return *this;
	}
	treeSize = 0;
	del(root);
	root = nullptr;
	dfs_insert(b.root);
	return *this;
    }
    iterator back_find(Node* v, ValueType k) const {
	if (v == 0) {
	    return iterator(this, 0, true);
	}
	if (!(v->key < k) && !(k < v->key)) {
	    return iterator(this, v, false);
	} else if (k < v->key) {
	    return back_find(v->left, k);
	} else {
	    return back_find(v->right, k);
	}
    }
    std::pair<bool, iterator> back_lower_bound(Node* v, ValueType k) const {
	if (v == 0) {
	    return {false, iterator(this, 0, true)};
	}
	if (!(v->key < k) && !(k < v->key)) {
	    return {true, iterator(this, v, false)};
	} else if (k < v->key) {
	    auto ansleft = back_lower_bound(v->left, k);
	    if (ansleft.first == false) {
		return {true, iterator(this, v, false)};
	    } else {
		return ansleft;
	    }
	} else {
	    return back_lower_bound(v->right, k);
	}
    }
    void insert(ValueType k) {
	if (!in(root, k)) {
	    treeSize++;
	    root = back_insert(root, k);
	}
    }
    void erase(ValueType k) {
	if (in(root, k)) {
	    treeSize--;
	    root = back_erase(root, k);
	}
    }
    size_t size() const {
	return treeSize;
    }
    bool empty() const {
	return treeSize == 0;
    }
    iterator begin() const {
	if (treeSize == 0) {
	    return iterator(this, nullptr, true);
	} else {
	    return iterator(this, get_min(root), false);
	}
    }
    iterator end() const {
	return iterator(this, nullptr, true);
    }
    iterator find(ValueType k) const {
	if (!in(root, k)) {
	    return iterator(this, nullptr, true);
	}
	return back_find(root, k);
    }
    iterator lower_bound(ValueType k) const {
	auto ans = back_lower_bound(root, k);
	return ans.second;
    }
    ~Set() {
	treeSize = 0;
	del(root);
    }
};
