#pragma once

#include <iostream>
#include <cassert>
#include <limits>
#include <iterator>
#include <compare>

#include "file_object.h"

template <typename K, typename V>
class Tree;

template <typename K, typename V>
struct Node {
    using value_type = std::pair<const K, V>;

    std::aligned_storage_t<
        sizeof(value_type),
        alignof(value_type)
    > kv{};

    Node* parent = nullptr;
    Node* left   = nullptr;
    Node* right  = nullptr;

    int height = 1;

    value_type& getPair() {
        return *std::launder(reinterpret_cast<value_type*>(&kv));
    }

    const value_type& getPair() const {
        return *std::launder(reinterpret_cast<const value_type*>(&kv));
    }
};

template <typename K, typename V, bool is_const>
class Iterator {
private:
    using node_ptr_t = std::conditional_t<is_const, const Node<K,V>, Node<K,V>>*;
    node_ptr_t node = nullptr;

    friend class Tree<K,V>;
    friend class Iterator<K,V, !is_const>;

public:
    using value_type = std::pair<const K, V>;
    using difference_type = ptrdiff_t;
    using pointer = std::conditional_t<is_const, const value_type*, value_type*>;
    using reference = std::conditional_t<is_const, const value_type&, value_type&>;
    using iterator_category = std::bidirectional_iterator_tag;

    Iterator() noexcept = default;
    explicit Iterator(node_ptr_t n) noexcept : node(n) {}

    template <bool other_const>
    Iterator(const Iterator<K,V,other_const>& o) noexcept requires (is_const >= other_const)
        : node(o.node) {}

    template <bool other_const>
    Iterator& operator=(const Iterator<K,V,other_const>& o) noexcept requires (is_const >= other_const) &&
                                                                        std::copy_constructible<value_type>
 {
        node = o.node;
        return *this;
    }

    reference operator*() const noexcept { return node->getPair(); }
    pointer operator->() const noexcept { return &node->getPair(); }

    Iterator& operator++() noexcept {
        Node<K,V>* cur = const_cast<Node<K,V>*>(node);

        if (cur->parent == nullptr) {
            // sentinel
            return *this;
        }

        if (cur->right) {
            cur = cur->right;
            while (cur->left)
                cur = cur->left;
        } else {
            Node<K,V>* p = cur->parent;
            while (p && cur == p->right) {
                cur = p;
                p = p->parent;
            }
            if (cur->parent != nullptr)
                cur = p;
        }

        node = cur;
        return *this;
    }

    Iterator operator++(int) noexcept { auto c = *this; ++(*this); return c; }

    Iterator& operator--() noexcept {
        Node<K,V>* cur = const_cast<Node<K,V>*>(node);

        if (cur->parent == nullptr) {
            node = cur->right;
            return *this;
        }

        if (cur->left) {
            cur = cur->left;
            while (cur->right)
                cur = cur->right;
        } else {
            Node<K,V>* p = cur->parent;
            while (p && cur == p->left) {
                cur = p;
                p = p->parent;
            }
            if (cur->parent != nullptr)
                cur = p;
        }

        node = cur;
        return *this;
    }

    Iterator operator--(int) noexcept { auto c = *this; --(*this); return c; }

    template <bool OC>
    bool operator==(const Iterator<K,V,OC>& o) const noexcept { return node == o.node; }
    template <bool OC>
    bool operator!=(const Iterator<K,V,OC>& o) const noexcept { return node != o.node; }
};

static_assert(std::bidirectional_iterator<Iterator<int,int,false>>);
static_assert(std::bidirectional_iterator<Iterator<int,int,true>>);

template <typename K, typename V>
class Tree {
private:
    Node<K,V>* rotateLeft(Node<K,V>*);
    Node<K,V>* rotateRight(Node<K,V>*);
    Node<K,V>* rotateLeftRight(Node<K,V>*);
    Node<K,V>* rotateRightLeft(Node<K,V>*);

    Node<K,V>* balance(Node<K,V>* n);

    int height(Node<K,V>* n) const {
        return n ? n->height : 0; 
    }
    int balanceFactor(Node<K,V>* n) const { 
        return height(n->left) - height(n->right); 
    }
    void updateHeight(Node<K,V>* n) { 
        n->height = 1 + std::max(height(n->left), height(n->right)); 
    }

public:
    Node<K,V>* minimum(Node<K,V>* n) const {
        if (!n) return nullptr;
        while (n->left) n = n->left;
        return n;
    }

    Node<K,V>* maximum(Node<K,V>* n) const {
        if (!n) return nullptr;
        while (n->right) n = n->right;
        return n;
    }

private:
    void deleteSubtree(Node<K,V>* n) noexcept {
        if (!n) return;
        deleteSubtree(n->left);
        deleteSubtree(n->right);
        deallocate(n);
    }
    
    void updateSentinelLinks() noexcept {
        if (root) {
            root->parent = sentinel;
            sentinel->left = minimum(root);
            sentinel->right = maximum(root);
        } else {
            sentinel->left = sentinel;
            sentinel->right = sentinel;
        }
    }

public:
    using value_type = std::pair<const K, V>;
    using key_type = K;
    using mapped_type = V;
    using reference = value_type&;
    using const_reference = const value_type&;
    using iterator = Iterator<K, V, false>;
    using const_iterator = Iterator<K, V, true>;
    using difference_type = ptrdiff_t;
    using size_type = size_t;
    using Compare = std::less<key_type>;
private:
    Node<K, V>* root = nullptr;
    Node<K, V>* sentinel = nullptr; // sentinel node, end() points to it
    size_type tree_size = 0; 
    Compare comp;

    struct insert_pos {
        Node<K, V>* parent = nullptr;
        bool insert_left = false;

        bool exists = false;
        Node<K, V>* node = nullptr;
    };

    void rebalanceUp(Node<K,V>* start_parent) noexcept {
        Node<K,V>* cur = start_parent;

        while (cur && cur != sentinel) {
            Node<K,V>* parent = cur->parent;
            Node<K,V>* new_root = balance(cur);

            if (parent == sentinel) {
                root = new_root;
                if (new_root)
                    new_root->parent = sentinel;
            } else {
                if (parent->left == cur)
                    parent->left = new_root;
                else
                    parent->right = new_root;

                if (new_root)
                    new_root->parent = parent;
            }
            cur = parent;
        }
    }

    insert_pos findPosition(const key_type& key) const {
        Node<K, V>* cur = root;
        Node<K, V>* parent = nullptr;

        while (cur) {
            parent = cur;

            if (comp(key, cur->getPair().first)) {
                cur = cur->left;
            }
            else if (comp(cur->getPair().first, key)) {
                cur = cur->right;
            }
            else {
                // эквивалентные ключи
                return insert_pos{
                    .exists = true,
                    .node = cur
                };
            }
        }

        if (!parent) {
            // пустое дерево
            return insert_pos{
                .parent = nullptr,
                .insert_left = false,
                .exists = false,
                .node = nullptr
            };
        }

        bool insert_left = comp(key, parent->getPair().first);

        return insert_pos{
            .parent = parent,
            .insert_left = insert_left,
            .exists = false,
            .node = nullptr
        };
    }

    void linkNode(Node<K,V>* new_node, const insert_pos& pos) noexcept {
        if (!pos.parent) {
            root = new_node;
            new_node->parent = sentinel;
        }
        else {
            new_node->parent = pos.parent;

            if (pos.insert_left) {
                pos.parent->left = new_node;
            } else {
                pos.parent->right = new_node;
            }
        }

        rebalanceUp(new_node->parent);

        updateSentinelLinks();
    }

    Node<K,V>* unlinkNode(Node<K,V>* n) noexcept {
    Node<K,V>* rebalance_start = nullptr;

    if (!n->left || !n->right) {
        Node<K,V>* child  = n->left ? n->left : n->right;
        Node<K,V>* parent = n->parent;

        if (child)
            child->parent = parent;

        if (parent == sentinel) {
            root = child;
            if (child)
                child->parent = sentinel;
        } else if (parent->left == n) {
            parent->left = child;
        } else {
            parent->right = child;
        }

        rebalance_start = parent;
    }
    else {
        Node<K,V>* succ = minimum(n->right);
        Node<K,V>* succ_parent = succ->parent;

        if (succ_parent == n) {
            succ->left = n->left;
            succ->left->parent = succ;

            Node<K,V>* parent = n->parent;
            succ->parent = parent;

            if (parent == sentinel) {
                root = succ;
            } else if (parent->left == n) {
                parent->left = succ;
            } else {
                parent->right = succ;
            }

            rebalance_start = succ;
        }
        else {
            Node<K,V>* succ_child = succ->right;

            succ_parent->left = succ_child;
            if (succ_child)
                succ_child->parent = succ_parent;

            succ->left = n->left;
            succ->right = n->right;
            succ->left->parent  = succ;
            succ->right->parent = succ;

            Node<K,V>* parent = n->parent;
            succ->parent = parent;

            if (parent == sentinel) {
                root = succ;
            } else if (parent->left == n) {
                parent->left = succ;
            } else {
                parent->right = succ;
            }

            rebalance_start = succ_parent;
        }
    }

    deallocate(n);
    return rebalance_start;
}


    void initSentinel() {
        sentinel = new Node<K,V>;

        sentinel->parent = nullptr;
        sentinel->left = sentinel;
        sentinel->right = sentinel;
    }
    
    void destroySentinel() noexcept {
        if (sentinel) {
            delete sentinel;
            sentinel = nullptr;
        }
    }

    Node<K,V>* findNode(const K& key) const noexcept {
        Node<K,V>* cur = root;

        while (cur) {
            if (comp(key, cur->getPair().first)) {
                cur = cur->left;
            }
            else if (comp(cur->getPair().first, key)) {
                cur = cur->right;
            }
            else {
                return cur;
            }
        }
        return nullptr;
    }

public:
    Tree() noexcept(std::is_nothrow_default_constructible_v<value_type>) {
        initSentinel();
    }

    ~Tree() {
        clear();
        destroySentinel();
    }

    Tree(const Tree& o) requires std::copy_constructible<value_type>
         : Tree() {
        insert(cend(), o.cbegin(), o.cend());
    }

    Tree(Tree&& o) noexcept(std::is_nothrow_default_constructible_v<value_type>) : Tree() {
        swap(o);
    }

    Tree(size_type n, const value_type& kv)
        : Tree() {
        insert(end(), n, kv);
    }

    template <std::input_iterator It>
    Tree(It i, It j) 
    requires std::constructible_from<value_type, std::iter_reference_t<It>>
        : Tree() {
        insert(end(), i, j);
    }

    Tree(std::initializer_list<value_type> il)
        requires std::move_constructible<value_type>
        : Tree() {
        insert(end(), std::move(il));
    }

    iterator find(const K& key) noexcept {
        Node<K,V>* node = findNode(key);
        return node ? iterator(node) : end();
    }

    const_iterator find(const K& key) const noexcept {
        Node<K,V>* node = findNode(key);
        return node ? const_iterator(node) : cend();
    }

    std::vector<K> keys() const {
        std::vector<K> result;
        result.reserve(size());
        for (auto it = begin(); it != end(); ++it) {
            result.push_back(it->first);
        }
        return result;
    } // не покрыта эта строчка тк конструктор и reserve могут бросать

    V& operator[](const K& key) {
        auto it = find(key);
        if (it != end()) {
            return it->second;
        }
        auto result = insert(std::make_pair(key, V{}));
        return result.first->second;
    }

    V& operator[](K&& key) {
        auto it = find(key);
        if (it != end()) {
            return it->second;
        }
        auto result = emplace(end(), key, V());
        return result.first->second;
    }

    const V& at(const K& key) const {
        auto it = find(key);
        if (it == end()) throw std::out_of_range("key not found");
        return it->second;
    }

    V& at(const K& key) {
        auto it = find(key);
        if (it == end()) throw std::out_of_range("key not found");
        return it->second;
    }

    size_type erase(const K& key) noexcept {
        Node<K,V>* target = findNode(key);
        if (!target)
            return 0;

        Node<K,V>* rebalance_start = unlinkNode(target);

        if (rebalance_start)
            rebalanceUp(rebalance_start);

        --tree_size;
        updateSentinelLinks();
        return 1;
    }


    size_type count(const K& key) const noexcept {
        return find(key) != cend() ? 1 : 0;
    }

    Node<K, V>* getRoot() const noexcept { return root; }

    size_type size() const noexcept {
        return tree_size;
    }
    
    size_type max_size() const noexcept {
        return std::numeric_limits<size_type>::max();
    }

    bool empty() const noexcept {
        return tree_size == 0;
    }
    
    void swap(Tree& o) noexcept {
        std::swap(root, o.root); 
        std::swap(tree_size, o.tree_size);
        std::swap(comp, o.comp);
        updateSentinelLinks();
        o.updateSentinelLinks();
    }

    iterator begin() noexcept {
        return iterator(sentinel->left);
    }

    iterator end() noexcept {
        return iterator(sentinel);
    }

    const_iterator begin() const noexcept {
        return const_iterator(sentinel->left);
    }

    const_iterator end() const noexcept {
        return const_iterator(sentinel);
    }

    const_iterator cbegin() const noexcept {
        return const_iterator(sentinel->left);
    }

    const_iterator cend() const noexcept {
        return const_iterator(sentinel);
    }

    template <typename... Args>
    std::pair<iterator, bool>
    emplace(const_iterator p, Args&&... args)
        requires std::constructible_from<value_type, Args...>
    {
        // поддерживаем strong-safety
        value_type tmp(std::forward<Args>(args)...);
        const K& key = tmp.first;

        insert_pos pos = findPosition(key);
        if (pos.exists) {
            return { iterator(pos.node), false };
        }

        Node<K,V>* new_node = allocate(std::move(tmp));

        // только щас вставляем
        linkNode(new_node, pos);
        ++tree_size;

        return { iterator(new_node), true };
    }


    iterator insert(const_iterator p, const value_type& val) 
        requires std::copy_constructible<value_type> {
        return emplace(p, val.first, val.second).first;
    }

    iterator insert(const_iterator p, value_type&& val) 
        requires std::move_constructible<value_type> {
        return emplace(p, std::move(val.first), std::move(val.second)).first;
    }

    // по стандарту map вставляем 1 элемент
    iterator insert(const_iterator p, size_type n, const value_type& val)
        requires std::copy_constructible<value_type>
    {
        if (n == 0) return iterator(const_cast<Node<K,V>*>(p.node));

        Tree tmp(*this);
        auto result = tmp.emplace(tmp.end(), val.first, val.second);
        if (!result.second) {
            // элемент уже есть
            return find(val.first);
        }

        swap(tmp);
        return find(val.first);
    }


    template <std::input_iterator It>
    iterator insert(const_iterator p, It first, It last)
        requires std::copy_constructible<value_type>
    {
        if (first == last) {
            return iterator(const_cast<Node<K,V>*>(p.node));
        }

        // можно так потому что requires
        Tree tmp(*this);

        bool has_first_inserted = false;
        K first_key;

        for (It it = first; it != last; ++it) {
            const value_type& val = *it;
            auto result = tmp.emplace(p, val.first, val.second);
            if (result.second && !has_first_inserted) {
                first_key = val.first;
                has_first_inserted = true;
            }
        }

        if (!has_first_inserted) {
            // ничего не вставилось
            return end();
        }

        swap(tmp);
        return find(first_key);
    }


    iterator insert(const_iterator p, std::initializer_list<value_type> il)
        requires std::move_constructible<value_type> {
        return insert(p, il.begin(), il.end());
    }

    iterator erase(const_iterator p) noexcept
    {
        if (p == end())
            return end();

        Node<K,V>* node = const_cast<Node<K,V>*>(p.node);

        iterator next = iterator(node);
        ++next;

        Node<K,V>* rebalance_start = unlinkNode(node);

        if (rebalance_start)
            rebalanceUp(rebalance_start);

        --tree_size;
        updateSentinelLinks();

        return next;
    }

    const_iterator erase(const_iterator first, const_iterator last) noexcept
    {
        while (first != last) {
            first = erase(first);
        }
        return last;
    }

    void clear() noexcept {
        deleteSubtree(root);
        root = nullptr;
        tree_size = 0;
        updateSentinelLinks();
    }

    template <std::forward_iterator It>
    void assign(It i, It j)
        requires std::copy_constructible<value_type> {
        Tree tmp;

        for (; i != j; ++i) {
            tmp.insert(tmp.end(), *i);
        }

        swap(tmp);
    }

    void assign(std::initializer_list<value_type> il)
        requires std::copy_constructible<value_type> {
        Tree tmp;

        for (const auto& value : il) {
            tmp.insert(end(), value);
        }

        swap(tmp);
    }

    void assign(size_type n, const value_type& kv)
        requires std::copy_constructible<value_type> {
        Tree tmp;

        for (size_type i = 0; i < n; ++i) {
            tmp.insert(end(), kv);
        }

        swap(tmp);
    }

    Tree& operator = (const Tree& o) 
    requires std::copy_constructible<value_type>{
        if (this == &o) {
            return *this;
        }
        assign(o.cbegin(), o.cend());
        return *this;
    }
    Tree& operator = (Tree&& o) noexcept {
        swap(o);
        return *this;
    }

    Tree& operator = (std::initializer_list<value_type> il) 
    requires std::move_constructible<value_type> {
        assign(std::move(il));
        return *this;
    }

    bool operator == (const Tree& o) const {
        return tree_size == o.tree_size &&
            std::equal(begin(), end(), o.begin(), o.end());
    }

    std::strong_ordering operator<=>(const Tree& o) const {
        auto it1 = begin();
        auto it2 = o.begin();
        auto end1 = end();
        auto end2 = o.end();

        while (it1 != end1 && it2 != end2) {
            auto cmp = std::compare_three_way{}(it1->first, it2->first);
            if (cmp != 0) {  // 0 = equal
                return cmp;
            }
            auto val_cmp = std::compare_three_way{}(it1->second, it2->second);
            if (val_cmp != 0) {
                return val_cmp;
            }
            ++it1;
            ++it2;
        }

        if (it1 == end1 && it2 == end2) {
            return std::strong_ordering::equal;
        }
        return (it1 == end1) ? std::strong_ordering::less : std::strong_ordering::greater;
    }

    template <typename... Args>
    Node<K,V>* allocate(Args&&... args) {
        Node<K,V>* node = new Node<K,V>;
        new (&node->kv) Node<K,V>::value_type(std::forward<Args>(args)...);

        node->parent = nullptr;
        node->left   = nullptr;
        node->right  = nullptr;
        node->height = 1;

        return node;
    }
    void deallocate(Node<K,V>* node) noexcept {
        if (!node) return;

        node->getPair().~value_type();
        delete node;
    }
};

template <typename K, typename V>
Node<K,V>* Tree<K,V>::rotateLeft(Node<K,V>* x) {
    assert(x);
    Node<K,V>* y = x->right;
    assert(y);

    Node<K,V>* beta = y->left;

    y->left = x;
    x->right = beta;

    y->parent = x->parent;
    x->parent = y;
    if (beta)
        beta->parent = x;

    updateHeight(x);
    updateHeight(y);

    return y;
}

template <typename K, typename V>
Node<K,V>* Tree<K,V>::rotateRight(Node<K,V>* y) {
    assert(y);
    Node<K,V>* x = y->left;
    assert(x);

    Node<K,V>* beta = x->right;

    x->right = y;
    y->left = beta;

    x->parent = y->parent;
    y->parent = x;
    if (beta)
        beta->parent = y;

    updateHeight(y);
    updateHeight(x);

    return x;
}

template <typename K, typename V>
Node<K,V>* Tree<K,V>::rotateLeftRight(Node<K,V>* z) {
    z->left = rotateLeft(z->left);
    return rotateRight(z);
}

template <typename K, typename V>
Node<K,V>* Tree<K,V>::rotateRightLeft(Node<K,V>* z) {
    z->right = rotateRight(z->right);
    return rotateLeft(z);
}

template <typename K, typename V>
Node<K,V>* Tree<K,V>::balance(Node<K,V>* n) {
    if (!n) return n;

    updateHeight(n);

    int bf = balanceFactor(n);

    if (bf > 1 && balanceFactor(n->left) >= 0)
        return rotateRight(n);

    if (bf > 1 && balanceFactor(n->left) < 0)
        return rotateLeftRight(n);

    if (bf < -1 && balanceFactor(n->right) <= 0)
        return rotateLeft(n);

    if (bf < -1 && balanceFactor(n->right) > 0)
        return rotateRightLeft(n);

    return n;
}