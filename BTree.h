#pragma once
// C++ Program to Implement B-Tree
#include <iostream>
using namespace std;

// class for the node present in a B-Tree
template <typename T, int ORDER>
class BTreeNode {
public:
	// Array of keys
	T keys[ORDER - 1];
	// Array of child pointers
	BTreeNode* children[ORDER];
	// Current number of keys
	int n;
	// True if leaf node, false otherwise
	bool leaf;

	BTreeNode(bool isLeaf = true) : n(0), leaf(isLeaf) {
		for (int i = 0; i < ORDER; i++)
			children[i] = nullptr;
	}
};

// class for B-Tree
template <typename T, int ORDER>
class BTree {
private:
	BTreeNode<T, ORDER>* root; // Pointer to root node

	// Function to split a full child node
	void splitChild(BTreeNode<T, ORDER>* x, int i) {
		BTreeNode<T, ORDER>* y = x->children[i];
		BTreeNode<T, ORDER>* z = new BTreeNode<T, ORDER>(y->leaf);
		z->n = ORDER / 2 - 1;

		for (int j = 0; j < ORDER / 2 - 1; j++)
			z->keys[j] = y->keys[j + ORDER / 2];

		if (!y->leaf) {
			for (int j = 0; j < ORDER / 2; j++)
				z->children[j] = y->children[j + ORDER / 2];
		}

		y->n = ORDER / 2 - 1;

		for (int j = x->n; j >= i + 1; j--)
			x->children[j + 1] = x->children[j];

		x->children[i + 1] = z;

		for (int j = x->n - 1; j >= i; j--)
			x->keys[j + 1] = x->keys[j];

		x->keys[i] = y->keys[ORDER / 2 - 1];
		x->n = x->n + 1;
	}

	// Function to insert a key in a non-full node
	void insertNonFull(BTreeNode<T, ORDER>* x, T k) {
		int i = x->n - 1;

		if (x->leaf) {
			while (i >= 0 && k < x->keys[i]) {
				x->keys[i + 1] = x->keys[i];
				i--;
			}

			x->keys[i + 1] = k;
			x->n = x->n + 1;
		}
		else {
			while (i >= 0 && k < x->keys[i])
				i--;

			i++;
			if (x->children[i]->n == ORDER - 1) {
				splitChild(x, i);

				if (k > x->keys[i])
					i++;
			}
			insertNonFull(x->children[i], k);
		}
	}

	// Function to traverse the tree
	void traverse(BTreeNode<T, ORDER>* x) {
		int i;
		for (i = 0; i < x->n; i++) {
			if (!x->leaf)
				traverse(x->children[i]);
			cout << " " << x->keys[i];
		}

		if (!x->leaf)
			traverse(x->children[i]);
	}

	// Function to search a key in the tree
	BTreeNode<T, ORDER>* search(BTreeNode<T, ORDER>* x, T k) {
		int i = 0;
		while (i < x->n && k > x->keys[i])
			i++;

		if (i < x->n && k == x->keys[i])
			return x;

		if (x->leaf)
			return nullptr;

		return search(x->children[i], k);
	}

	// Function to find the predecessor
	T getPredecessor(BTreeNode<T, ORDER>* node, int idx) {
		BTreeNode<T, ORDER>* current = node->children[idx];
		while (!current->leaf)
			current = current->children[current->n];
		return current->keys[current->n - 1];
	}

	// Function to find the successor
	T getSuccessor(BTreeNode<T, ORDER>* node, int idx) {
		BTreeNode<T, ORDER>* current = node->children[idx + 1];
		while (!current->leaf)
			current = current->children[0];
		return current->keys[0];
	}

	// Function to fill child node
	void fill(BTreeNode<T, ORDER>* node, int idx) {
		if (idx != 0 && node->children[idx - 1]->n >= ORDER / 2)
			borrowFromPrev(node, idx);
		else if (idx != node->n && node->children[idx + 1]->n >= ORDER / 2)
			borrowFromNext(node, idx);
		else {
			if (idx != node->n)
				merge(node, idx);
			else
				merge(node, idx - 1);
		}
	}

	// Function to borrow from previous sibling
	void borrowFromPrev(BTreeNode<T, ORDER>* node, int idx) {
		BTreeNode<T, ORDER>* child = node->children[idx];
		BTreeNode<T, ORDER>* sibling = node->children[idx - 1];

		for (int i = child->n - 1; i >= 0; --i)
			child->keys[i + 1] = child->keys[i];

		if (!child->leaf) {
			for (int i = child->n; i >= 0; --i)
				child->children[i + 1] = child->children[i];
		}

		child->keys[0] = node->keys[idx - 1];

		if (!child->leaf)
			child->children[0] = sibling->children[sibling->n];

		node->keys[idx - 1] = sibling->keys[sibling->n - 1];

		child->n += 1;
		sibling->n -= 1;
	}

	// Function to borrow from next sibling
	void borrowFromNext(BTreeNode<T, ORDER>* node, int idx) {
		BTreeNode<T, ORDER>* child = node->children[idx];
		BTreeNode<T, ORDER>* sibling = node->children[idx + 1];

		child->keys[child->n] = node->keys[idx];

		if (!child->leaf)
			child->children[child->n + 1] = sibling->children[0];

		node->keys[idx] = sibling->keys[0];

		for (int i = 1; i < sibling->n; ++i)
			sibling->keys[i - 1] = sibling->keys[i];

		if (!sibling->leaf) {
			for (int i = 1; i <= sibling->n; ++i)
				sibling->children[i - 1] = sibling->children[i];
		}

		child->n += 1;
		sibling->n -= 1;
	}

	// Function to merge two nodes
	void merge(BTreeNode<T, ORDER>* node, int idx) {
		BTreeNode<T, ORDER>* child = node->children[idx];
		BTreeNode<T, ORDER>* sibling = node->children[idx + 1];

		child->keys[ORDER / 2 - 1] = node->keys[idx];

		for (int i = 0; i < sibling->n; ++i)
			child->keys[i + ORDER / 2] = sibling->keys[i];

		if (!child->leaf) {
			for (int i = 0; i <= sibling->n; ++i)
				child->children[i + ORDER / 2] = sibling->children[i];
		}

		for (int i = idx + 1; i < node->n; ++i)
			node->keys[i - 1] = node->keys[i];

		for (int i = idx + 2; i <= node->n; ++i)
			node->children[i - 1] = node->children[i];

		child->n += sibling->n + 1;
		node->n--;

		delete sibling;
	}

	// Function to remove a key from a non-leaf node
	void removeFromNonLeaf(BTreeNode<T, ORDER>* node, int idx) {
		T k = node->keys[idx];

		if (node->children[idx]->n >= ORDER / 2) {
			T pred = getPredecessor(node, idx);
			node->keys[idx] = pred;
			remove(node->children[idx], pred);
		}
		else if (node->children[idx + 1]->n >= ORDER / 2) {
			T succ = getSuccessor(node, idx);
			node->keys[idx] = succ;
			remove(node->children[idx + 1], succ);
		}
		else {
			merge(node, idx);
			remove(node->children[idx], k);
		}
	}

	// Function to remove a key from a leaf node
	void removeFromLeaf(BTreeNode<T, ORDER>* node, int idx) {
		for (int i = idx + 1; i < node->n; ++i)
			node->keys[i - 1] = node->keys[i];

		node->n--;
	}

	// Function to remove a key from the tree
	void remove(BTreeNode<T, ORDER>* node, T k) {
		int idx = 0;
		while (idx < node->n && node->keys[idx] < k)
			++idx;

		if (idx < node->n && node->keys[idx] == k) {
			if (node->leaf)
				removeFromLeaf(node, idx);
			else
				removeFromNonLeaf(node, idx);
		}
		else {
			if (node->leaf) {
				cout << "The key " << k << " is not present in the tree\n";
				return;
			}

			bool flag = ((idx == node->n) ? true : false);

			if (node->children[idx]->n < ORDER / 2)
				fill(node, idx);

			if (flag && idx > node->n)
				remove(node->children[idx - 1], k);
			else
				remove(node->children[idx], k);
		}
	}

public:
	BTree() { root = new BTreeNode<T, ORDER>(true); }

	// Function to insert a key in the tree
	void insert(T k) {
		if (root->n == ORDER - 1) {
			BTreeNode<T, ORDER>* s = new BTreeNode<T, ORDER>(false);
			s->children[0] = root;
			root = s;
			splitChild(s, 0);
			insertNonFull(s, k);
		}
		else
			insertNonFull(root, k);
	}

	// Function to traverse the tree
	void traverse() {
		if (root != nullptr)
			traverse(root);
	}

	// Function to search a key in the tree
	BTreeNode<T, ORDER>* search(T k) {
		return (root == nullptr) ? nullptr : search(root, k);
	}

	// Function to remove a key from the tree
	void remove(T k) {
		if (!root) {
			cout << "The tree is empty\n";
			return;
		}

		remove(root, k);

		if (root->n == 0) {
			BTreeNode<T, ORDER>* tmp = root;
			if (root->leaf)
				root = nullptr;
			else
				root = root->children[0];

			delete tmp;
		}
	}
};

