#pragma once

#include <functional>
#include <queue>
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
            : key_(key), val_(std::move(val)), height_(1), left_(nullptr),
              right_(nullptr){};

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

    std::vector<Key> getAllKeys() {
        std::vector<Key> result = std::vector<Key>();
        AVLTree::bfs(
            [&result](TreeNode *node) { result.push_back(node->key_); });

        return result;
    }

    std::vector<std::pair<Key, Value>> getAll() {
        std::vector<std::pair<Key, Value>> result =
            std::vector<std::pair<Key, Value>>();
        AVLTree::bfs([&result](TreeNode *node) {
            result.push_back({node->key_, node->val_});
        });

        return result;
    }

  private:
    TreeNode *root_;
    Comparator comp_;

    void post_order(const std::function<void(TreeNode *)> &handler) {
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

    void bfs(const std::function<void(TreeNode *)> &handler) {
        if (root_ != nullptr) {
            std::queue<TreeNode *> q;
            q.push(root_);

            while (!q.empty()) {
                TreeNode *cur_node = q.front();
                q.pop();
                handler(cur_node);

                if (cur_node->left_) {
                    q.push(cur_node->left_);
                }
                if (cur_node->right_) {
                    q.push(cur_node->right_);
                }
            }
        }
    };

    uint8_t height(TreeNode *node) {
        if (!node) {
            return 0;
        }

        return node->height_;
    };

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

    // insertAux() throw AVLError if node with same key exist
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

    // eraseAux() throw AVLError when node with passed key not exist
    TreeNode *eraseAux(const Key &key, TreeNode *node) {
        if (!node) {
            throw AVLError(
                "in eraseAux: node with passed key not exist in tree");
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

        std::stack<TreeNode *> stack_balance;
        TreeNode *prev_node = node;
        TreeNode *cur_node = node->left_;
        stack_balance.push(prev_node);
        while (cur_node->left_) {
            prev_node = cur_node;
            cur_node = cur_node->left_;
            stack_balance.push(prev_node);
        }

        prev_node->left_ = cur_node->right_;
        cur_node->right_ = node;

        while (!stack_balance.empty()) {
            TreeNode *tmp = stack_balance.top();
            stack_balance.pop();
            balance(tmp);
        }

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