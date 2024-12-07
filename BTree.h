#pragma once
// C++ Program to Implement B-Tree
#include <iostream>
#include <cstdio>  // For std::remove
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


/*
	keys comma sepeated

*/

// class for B-Tree
template <typename T, int ORDER>
class BTree {
private:
	BTreeNode<T, ORDER>* root; // Pointer to root node

	
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

	BTree() { //root = new BTreeNode<T, ORDER>(true); 

		root = nullptr;
	}



	
	// Helper function to generate children filenames string
	string generateChildrenString(BTreeNode<T, ORDER>* node) {
		if (node->leaf) return "";

		string children;
		for (int i = 0; i <= node->n; i++) {
			if (i > 0) children += ",";
			children += generateNodeFilename(node->children[i]);
		}
		return children;
	}

	

	void createNode(const string& filename, const string& keys, const string& children = "", bool isLeaf = true) {
		ofstream file(filename);
		if (file) {
			file << "Keys = " << keys << "\n";
			if (!children.empty()) file << "Children = " << children << "\n";
			file << "Leaf = " << (isLeaf ? 1 : 0) << "\n";
			cout << "Node file " << filename << " created successfully.\n";
		}
		else {
			cerr << "Error creating node file: " << filename << endl;
		}
		file.close();
	}


	// Function to split a full child node in a file-based B-tree
	void splitChild(BTreeNode<T, ORDER>* x, int i) {
		// Get the child node to be split
		BTreeNode<T, ORDER>* y = x->children[i];

		// Create a new node
		BTreeNode<T, ORDER>* z = new BTreeNode<T, ORDER>(y->leaf);
		z->n = ORDER / 2 - 1;

		// Copy the second half of keys from y to z
		for (int j = 0; j < ORDER / 2 - 1; j++)
			z->keys[j] = y->keys[j + ORDER / 2];

		// If not a leaf, copy the second half of children
		if (!y->leaf) {
			for (int j = 0; j < ORDER / 2; j++)
				z->children[j] = y->children[j + ORDER / 2];
		}

		// Reduce the number of keys in the original node
		y->n = ORDER / 2 - 1;

		// Shift children in the parent node
		for (int j = x->n; j >= i + 1; j--)
			x->children[j + 1] = x->children[j];

		// Insert the new node as a child
		x->children[i + 1] = z;

		// Shift keys in the parent node
		for (int j = x->n - 1; j >= i; j--)
			x->keys[j + 1] = x->keys[j];

		// Move the median key to the parent
		x->keys[i] = y->keys[ORDER / 2 - 1];
		x->n = x->n + 1;

		// Update files for the nodes involved in splitting
		string yFilename = generateNodeFilename(y);
		string zFilename = generateNodeFilename(z);
		string xFilename = generateNodeFilename(x);

		// Update y's file
		createNode(yFilename, generateKeysString(y),
			!y->leaf ? generateChildrenString(y) : "", y->leaf);

		// Create file for the new node z
		createNode(zFilename, generateKeysString(z),
			!z->leaf ? generateChildrenString(z) : "", z->leaf);

		// Update parent node x's file
		createNode(xFilename, generateKeysString(x),
			generateChildrenString(x), false);
	}

	

	// Function to insert a key in a non-full node
	void insertNonFull(BTreeNode<T, ORDER>* x, T k) {
		int i = x->n - 1;

		if (x->leaf) {
			// Insert key in the leaf node
			while (i >= 0 && k < x->keys[i]) {
				x->keys[i + 1] = x->keys[i];
				i--;
			}

			x->keys[i + 1] = k;
			x->n = x->n + 1;

			// Update the node's file
			string filename = generateNodeFilename(x);
			createNode(filename, generateKeysString(x), "", true);
		}
		else {
			// Find the child to insert into
			while (i >= 0 && k < x->keys[i])
				i--;
			i++;

			// Check if the child is full
			if (x->children[i]->n == ORDER - 1) {
				// Split the child
				splitChild(x, i);

				// Adjust insertion point if necessary
				if (k > x->keys[i])
					i++;
			}

			// Recursively insert into the appropriate child
			insertNonFull(x->children[i], k);

			// Update the parent node's file
			string filename = generateNodeFilename(x);
			createNode(filename, generateKeysString(x),
				generateChildrenString(x), false);
		}
	}

	void insert(T k) {
		// Case 1: Tree is empty
		if (root == nullptr) {
			root = new BTreeNode<T, ORDER>(true);
			root->keys[0] = k;
			root->n = 1;

			// Create file for the root node
			string filename = generateNodeFilename(root);
			createNode(filename, generateKeysString(root), "", true);

			cout << "Root node created and key inserted: " << k << endl;
			return;
		}

		// Case 2: Root is full
		if (root->n == ORDER - 1) {
			// Create new root
			BTreeNode<T, ORDER>* newRoot = new BTreeNode<T, ORDER>(false);
			newRoot->children[0] = root;

			// Split the old root
			splitChild(newRoot, 0);

			// Determine which child to insert into
			int i = (k > newRoot->keys[0]) ? 1 : 0;

			// Insert the key
			insertNonFull(newRoot->children[i], k);

			// Update root
			root = newRoot;

			// Create file for new root
			string newRootFilename = generateNodeFilename(root);
			createNode(newRootFilename, generateKeysString(root),
				generateChildrenString(root), false);

			cout << "Root was split. New root created with key: " << root->keys[0] << endl;
		}
		else {
			// Insert into non-full root
			insertNonFull(root, k);

			// Update root's file
			string filename = generateNodeFilename(root);
			createNode(filename, generateKeysString(root),
				generateChildrenString(root), root->leaf);

			cout << "Key inserted: " << k << endl;
		}
	}
	// Helper functions
	string generateNodeFilename(BTreeNode<T, ORDER>* node) {
		string filename = "node_";
		for (int i = 0; i < node->n; i++) {
			if (i > 0) filename += "_";
			filename += to_string(node->keys[i]);
		}
		filename += ".txt";
		return filename;
	}

	string generateKeysString(BTreeNode<T, ORDER>* node) {
		string keys;
		for (int i = 0; i < node->n; i++) {
			if (i > 0) keys += ",";
			keys += to_string(node->keys[i]);
		}
		return keys;
	}


	void deleteFile(const string& filename) {
		if (std::remove(filename.c_str()) == 0) {
			cout << "Node file " << filename << " deleted successfully.\n";
		}
		else {
			cerr << "Error deleting node file: " << filename << endl;
		}
	}

	void readNode(const string& filename, string& keys, string& children, bool& isLeaf) {
		ifstream file(filename);
		if (file) {
			string line;
			while (getline(file, line)) {
				if (line.find("Keys = ") == 0) {
					keys = line.substr(7); // Extract keys after "Keys = "
				}
				else if (line.find("Children = ") == 0) {
					children = line.substr(11); // Extract children paths
				}
				else if (line.find("Leaf = ") == 0) {
					isLeaf = (line.substr(7) == "1");
				}
			}
			cout << "Node file " << filename << " read successfully.\n";
		}
		else {
			cerr << "Error reading node file: " << filename << endl;
		}
		file.close();
	}


	void updateNode(const string& filename, const string& newKeys, const string& newChildren = "") {
		fstream file(filename, ios::in | ios::out);
		if (file) {
			string content, line;
			while (getline(file, line)) {
				if (line.find("Keys = ") == 0) {
					line += "," + newKeys; // Append new keys
				}
				else if (line.find("Children = ") == 0 && !newChildren.empty()) {
					line += "," + newChildren; // Append new children
				}
				content += line + "\n";
			}
			file.close();
			ofstream outFile(filename);
			outFile << content;
			outFile.close();
			cout << "Node file " << filename << " updated successfully.\n";
		}
		else {
			cerr << "Error updating node file: " << filename << endl;
		}
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

