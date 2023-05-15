#include "avl_tree.hpp"

namespace avl_tree {
template <typename Key, typename Value,
          typename Comparator = DefaultComparator<Key>>
uint8_t AVLTree<Key, Value, Comparator>::height(TreeNode *node) {
    if (!node) {
        return 0;
    }

    return node->height_;
}

template <typename Key, typename Value,
          typename Comparator = DefaultComparator<Key>>
void AVLTree<Key, Value, Comparator>::post_order(
    void (*handler)(TreeNode *node)) {
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
}

template <typename Key, typename Value,
          typename Comparator = DefaultComparator<Key>>
Value *AVLTree<Key, Value, Comparator>::findAux(const Key &key,
                                                TreeNode *node) {
    if (!node) {
        return nullptr;
    }

    int res_comp = comp_(key, node->key);

    // key < node->key
    if (res_comp == -1) {
        return findAux(key, node->left_);
    }
    // key > node->key
    if (res_comp == 1) {
        return findAux(key, node->right_);
    }

    // key == node->key
    return &node->val;
}

template <typename Key, typename Value,
          typename Comparator = DefaultComparator<Key>>
TreeNode *AVLTree<Key, Valut, Comparator>::insertAux(const Key &key,
                                                     const Value &val,
                                                     TreeNode *node) {
    if (!node) {
        return new TreeNode(key, val);
    }

    int res_comp = comp_(key, node->key_);

    // key < node->key
    if (res_comp == -1) {
        node->left_ = insertAux(key, val, node->left_);
    }
    // key > node->key
    if (res_comp == 1) {
        node->right_ = insertAux(key, val, node->right_);
    }

    // key == node->key
    return balance(node);
}

template <typename Key, typename Value,
          typename Comparator = DefaultComparator<Key>>
TreeNode *AVLTree<Key, Valut, Comparator>::eraseAux(const Key &key,
                                                    TreeNode *node) {
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
}

template <typename Key, typename Value,
          typename Comparator = DefaultComparator<Key>>
TreeNode *
AVLTree<Key, Valut, Comparator>::findAndRemoveMinNode(TreeNode *node) {
    if (!node->left_) {
        return balance(node);
    }

    TreeNode *prev_node = node;
    TreeNode *cur_node = node->left_;
    while (cur_node->left_) {
        cur_node = cur_node->left_;
        prev_node = cur_node;
    }

    prev_node->left_ = cur_node->right_;
    cur_node->right_ = node;
    return balance(cur_node);
}

template <typename Key, typename Value,
          typename Comparator = DefaultComparator<Key>>
void AVLTree<Key, Valut, Comparator>::fixHeight(TreeNode *node) {
    node->height_ = std::max(height(node->left_), height(node->right_)) + 1;
}

template <typename Key, typename Value,
          typename Comparator = DefaultComparator<Key>>
int8_t AVLTree<Key, Valut, Comparator>::bFactor(TreeNode *node) {
    return height(node->right_) - height(node->left_);
}

template <typename Key, typename Value,
          typename Comparator = DefaultComparator<Key>>
TreeNode *AVLTree<Key, Valut, Comparator>::balance(TreeNode *node) {
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
}

template <typename Key, typename Value,
          typename Comparator = DefaultComparator<Key>>
TreeNode *AVLTree<Key, Valut, Comparator>::rotateLeft(TreeNode *node) {
    TreeNode *tmp = node->right_;
    node->right_ = tmp->left_;
    tmp->left_ = node;
    fixHeight(node);
    fixHeight(tmp);
    return tmp;
};

template <typename Key, typename Value,
          typename Comparator = DefaultComparator<Key>>
TreeNode *AVLTree<Key, Valut, Comparator>::rotateRight(TreeNode *node) {
    TreeNode *tmp = node->left_;
    node->left_ = tmp->right_;
    tmp->right_ = node;
    fixHeight(node);
    fixHeight(tmp);
    return tmp;
};
}   // namespace avl_tree