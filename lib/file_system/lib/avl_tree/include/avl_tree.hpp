#pragma once

#include <cstdint>
#include <memory>
#include <stack>
#include <stdexcept>

namespace avl_tree {
class AVLError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

template <typename T> struct DefaultComparator {
    int operator()(const T &l, const T &r) {
        if (l < r) {
            return -1;
        }
        if (l == r) {
            return 0;
        }

        return 1;
    }
};

template <typename Key, typename Value,
          typename Comparator = DefaultComparator<Key>>
class AVLTree {
    struct TreeNode {
      public:
        TreeNode(const Key &key, Value &&val)
            : key_(key), left_(nullptr), right_(nullptr), height_(1) {
            std::exchange(val, nullptr);
        };
        const Key key_;
        Value val_;

        uint8_t height_;
        TreeNode *left_;
        TreeNode *right_;
    };

  public:
    AVLTree(Comparator comp = Comparator()) : root_(nullptr), comp_(comp){};

    Value *find(const Key &key) { return findAux(key, root_); };
    void insert(const Key &key, Value &&val) {
        root_ = insertAux(key, val, root_);
    };
    void erase(const Key &key) { root_ = eraseAux(key, root_); };

    ~AVLTree() {
        post_order([](TreeNode *node) { delete node; });
    };

  private:
    TreeNode *root_;
    Comparator comp_;

    void post_order(void (*handler)(TreeNode *node));

    uint8_t height(TreeNode *node);

    Value *findAux(const Key &key, TreeNode *node);
    TreeNode *insertAux(const Key &key, Value &&val, TreeNode *node);
    TreeNode *eraseAux(const Key &key, TreeNode *node);

    TreeNode *findAndRemoveMinNode(TreeNode *node);
    void fixHeight(TreeNode *node);
    int8_t bFactor(TreeNode *node);
    TreeNode *balance(TreeNode *node);
    TreeNode *rotateLeft(TreeNode *node);
    TreeNode *rotateRight(TreeNode *node);
};

}   // namespace avl_tree