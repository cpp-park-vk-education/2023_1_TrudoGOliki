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
            std::exchange(val, Value());
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
        root_ = insertAux(key, std::move(val), root_);
    };
    void erase(const Key &key) { root_ = eraseAux(key, root_); };

    ~AVLTree() {
        post_order([](TreeNode *node) { delete node; });
    }

  private:
    TreeNode *root_;
    Comparator comp_;

    void post_order(void (*handler)(TreeNode *node)) {
        std::stack<TreeNode *> stack1;
        std::stack<TreeNode *> stack2;
        TreeNode *cur_node = root_;
        if (cur_node == nullptr) {
            return;
        }
        stack1.push(cur_node);
        while (!stack1.empty()) {
            cur_node = stack1.top();
            stack1.pop();
            stack2.push(cur_node);
            if (cur_node->left_) {
                stack1.push(cur_node->left_);
            }
            if (cur_node->right_) {
                stack1.push(cur_node->right_);
            }
        }

        for (; !stack2.empty();) {
            cur_node = stack2.top();
            handler(cur_node);
            stack2.pop();
        }
    };

    uint8_t height(TreeNode *node) {
        if (!node) {
            return 0;
        }

        return node->height_;
    }

    // findAux() return nullptr if node with key not exist
    Value *findAux(const Key &key, TreeNode *node) {
        if (!node) {
            return nullptr;
        }

        int res_comp = comp_(key, node->key_);

        // key < node->key
        if (res_comp == -1) {
            return findAux(key, node->left_);
        }
        // key > node->key
        if (res_comp == 1) {
            return findAux(key, node->right_);
        }

        // key == node->key
        return &node->val_;
    };

    // insertAux() can throw AVLError
    TreeNode *insertAux(const Key &key, Value &&val, TreeNode *node) {
        if (!node) {
            return new TreeNode(key, std::move(val));
        }

        int res_comp = comp_(key, node->key_);
        if (res_comp == 0) {
            throw AVLError("in insertAux can`t insert value, because this key "
                           "already exist.");
        }

        // key < node->key
        if (res_comp == -1) {
            node->left_ = insertAux(key, std::move(val), node->left_);
        }
        // key > node->key
        if (res_comp == 1) {
            node->right_ = insertAux(key, std::move(val), node->right_);
        }

        // key == node->key
        return balance(node);
    };

    TreeNode *eraseAux(const Key &key, TreeNode *node) {
        if (!node) {
            return nullptr;
        }

        int res_comp = comp_(key, node->key_);

        // key < node->key
        if (res_comp == -1) {
            node->left_ = eraseAux(key, node->left_);
        } else if (res_comp == 1) {
            // key > node->key
            node->right_ = eraseAux(key, node->right_);
        } else {
            // key == node->key
            TreeNode *left = node->left_;
            TreeNode *right = node->right_;

            delete node;

            if (!right) {
                return left;
            }

            TreeNode *min_node = findAndRemoveMinNode(right);

            min_node->left_ = left;

            return balance(min_node);
        }

        return balance(node);
    };

    TreeNode *findAndRemoveMinNode(TreeNode *node) {
        if (!node->left_) {
            return node;
        }

        TreeNode *prev_node = node;
        TreeNode *cur_node = node->left_;
        while (cur_node->left_) {
            cur_node = cur_node->left_;
            prev_node = cur_node;
        }

        prev_node->left_ = cur_node->right_;
        cur_node->right_ = node;
        return cur_node;
    };

    void fixHeight(TreeNode *node) {
        node->height_ = std::max(height(node->left_), height(node->right_)) + 1;
    };

    int8_t bFactor(TreeNode *node) {
        return height(node->right_) - height(node->left_);
    };

    TreeNode *balance(TreeNode *node) {
        fixHeight(node);

        int8_t bF = bFactor(node);
        if (bF == 2) {
            if (bFactor(node->right_) < 0) {
                node->right_ = rotateRight(node->right_);
            }
            return rotateLeft(node);
        }

        if (bF == -2) {
            if (bFactor(node->left_) > 0) {
                node->left_ = rotateLeft(node->left_);
            }
            return rotateRight(node);
        }

        return node;
    };

    TreeNode *rotateLeft(TreeNode *node) {
        TreeNode *tmp = node->right_;
        node->right_ = tmp->left_;
        tmp->left_ = node;
        fixHeight(node);
        fixHeight(tmp);
        return tmp;
    };

    TreeNode *rotateRight(TreeNode *node) {
        TreeNode *tmp = node->left_;
        node->left_ = tmp->right_;
        tmp->right_ = node;
        fixHeight(node);
        fixHeight(tmp);
        return tmp;
    };
};

}   // namespace avl_tree