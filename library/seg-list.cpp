#include <bits/stdc++.h>
using namespace std;

// 添字で挿入・削除できる区間モノイド列。
// 葉を双方向リストでつなぎ、内部ノードは AVL 木で順序・要素数・区間積を管理する。
// op は結合的で、idn はその両側単位元であること。可換性は不要。
// insert / erase / set / query: O(log N) 回の op、O(log N) の木操作。
// value: O(log N)、all_query: O(1)、to_vector: O(N)。
template <typename T, typename Op>
class SegList {
    struct Node {
        Node *left = nullptr, *right = nullptr;
        Node *prev = nullptr, *next = nullptr; // 葉だけが使用する
        int size = 1, height = 1;
        T product;

        explicit Node(const T &value) : product(value) {}
        Node(Node *l, Node *r, T value)
            : left(l), right(r), size(l->size + r->size),
              height(max(l->height, r->height) + 1), product(move(value)) {}
        bool is_leaf() const { return left == nullptr; }
    };

    Node *root = nullptr, *head = nullptr, *tail = nullptr;
    Op op;
    T idn;

    static int height(const Node *node) { return node ? node->height : 0; }

    void pull(Node *node) {
        node->size = node->left->size + node->right->size;
        node->height = max(node->left->height, node->right->height) + 1;
        node->product = op(node->left->product, node->right->product);
    }

    Node *rotate_left(Node *node) {
        Node *top = node->right;
        node->right = top->left;
        top->left = node;
        pull(node);
        pull(top);
        return top;
    }

    Node *rotate_right(Node *node) {
        Node *top = node->left;
        node->left = top->right;
        top->right = node;
        pull(node);
        pull(top);
        return top;
    }

    Node *balance(Node *node) {
        pull(node);
        if (height(node->left) - height(node->right) > 1) {
            if (height(node->left->left) < height(node->left->right))
                node->left = rotate_left(node->left);
            return rotate_right(node);
        }
        if (height(node->right) - height(node->left) > 1) {
            if (height(node->right->right) < height(node->right->left))
                node->right = rotate_right(node->right);
            return rotate_left(node);
        }
        return node;
    }

    Node *insert_at(Node *node, int pos, Node *leaf) {
        if (!node) return leaf;
        if (node->is_leaf()) {
            if (pos == 0) return new Node(leaf, node, op(leaf->product, node->product));
            return new Node(node, leaf, op(node->product, leaf->product));
        }
        if (pos <= node->left->size)
            node->left = insert_at(node->left, pos, leaf);
        else
            node->right = insert_at(node->right, pos - node->left->size, leaf);
        return balance(node);
    }

    Node *erase_at(Node *node, int pos) {
        if (node->is_leaf()) {
            delete node;
            return nullptr;
        }
        if (pos < node->left->size)
            node->left = erase_at(node->left, pos);
        else
            node->right = erase_at(node->right, pos - node->left->size);

        if (!node->left || !node->right) {
            Node *remaining = node->left ? node->left : node->right;
            delete node;
            return remaining;
        }
        return balance(node);
    }

    Node *leaf_at(int pos) const {
        Node *node = root;
        while (!node->is_leaf()) {
            if (pos < node->left->size)
                node = node->left;
            else {
                pos -= node->left->size;
                node = node->right;
            }
        }
        return node;
    }

    void set_at(Node *node, int pos, const T &value) {
        if (node->is_leaf()) {
            node->product = value;
            return;
        }
        if (pos < node->left->size)
            set_at(node->left, pos, value);
        else
            set_at(node->right, pos - node->left->size, value);
        pull(node);
    }

    T query_at(const Node *node, int l, int r) const {
        if (l == 0 && r == node->size) return node->product;
        int left_size = node->left->size;
        if (r <= left_size) return query_at(node->left, l, r);
        if (l >= left_size) return query_at(node->right, l - left_size, r - left_size);
        return op(query_at(node->left, l, left_size),
                  query_at(node->right, 0, r - left_size));
    }

    static void destroy(Node *node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    SegList(Op op, T idn) : op(move(op)), idn(move(idn)) {}

    // O(N) で構築する。
    SegList(const vector<T> &values, Op op, T idn)
        : SegList(move(op), move(idn)) {
        vector<Node *> leaves;
        leaves.reserve(values.size());
        for (const T &value : values) {
            Node *leaf = new Node(value);
            leaf->prev = tail;
            if (tail) tail->next = leaf;
            else head = leaf;
            tail = leaf;
            leaves.push_back(leaf);
        }
        auto build = [&](auto &&self, int l, int r) -> Node * {
            if (l == r) return leaves[l];
            int mid = l + (r - l) / 2;
            Node *left = self(self, l, mid);
            Node *right = self(self, mid + 1, r);
            return new Node(left, right, this->op(left->product, right->product));
        };
        if (!leaves.empty()) root = build(build, 0, (int)leaves.size() - 1);
    }

    ~SegList() { destroy(root); }
    SegList(const SegList &) = delete;
    SegList &operator=(const SegList &) = delete;

    int size() const { return root ? root->size : 0; }
    bool empty() const { return root == nullptr; }

    // pos 個の要素の後へ挿入する。0 <= pos <= size()。
    void insert(int pos, const T &value) {
        assert(0 <= pos && pos <= size());
        Node *successor = pos == size() ? nullptr : leaf_at(pos);
        Node *predecessor = successor ? successor->prev : tail;
        Node *leaf = new Node(value);
        root = insert_at(root, pos, leaf);
        leaf->prev = predecessor;
        leaf->next = successor;
        if (predecessor) predecessor->next = leaf;
        else head = leaf;
        if (successor) successor->prev = leaf;
        else tail = leaf;
    }

    // pos 番目の要素を削除する。0 <= pos < size()。
    void erase(int pos) {
        assert(0 <= pos && pos < size());
        Node *leaf = leaf_at(pos);
        if (leaf->prev) leaf->prev->next = leaf->next;
        else head = leaf->next;
        if (leaf->next) leaf->next->prev = leaf->prev;
        else tail = leaf->prev;
        root = erase_at(root, pos);
    }

    void set(int pos, const T &value) {
        assert(0 <= pos && pos < size());
        set_at(root, pos, value);
    }

    T value(int pos) const {
        assert(0 <= pos && pos < size());
        return leaf_at(pos)->product;
    }

    // 半開区間 [l, r) の積。空区間には単位元を返す。
    T query(int l, int r) const {
        assert(0 <= l && l <= r && r <= size());
        return l == r ? idn : query_at(root, l, r);
    }

    T all_query() const { return root ? root->product : idn; }

    vector<T> to_vector() const {
        vector<T> result;
        result.reserve(size());
        for (Node *node = head; node; node = node->next)
            result.push_back(node->product);
        return result;
    }
};

/*
auto op = [](const string &a, const string &b) { return a + b; };
SegList<string, decltype(op)> seq(op, string{});
seq.insert(0, "a");
seq.insert(1, "b");
seq.insert(1, "X");
// seq.query(0, 3) == "aXb"
seq.erase(1);
*/
