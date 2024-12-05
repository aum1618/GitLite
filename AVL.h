#pragma once
#include <iostream>
#include "FileHandling.h"

using namespace std;



// Template class representing the AVL tree
template <typename T> class AVLTree {
public:
	string rootFile;


	int fileHeight(string filename) {

		if (filename == "null")
			return 0;
		ifstream file(filename);
		if (!file.is_open()) {
			cerr << "Error: Could not open file " << filename << endl;
			return 0;
		}
		string line;
		int height = 0;
		while (getline(file, line)) {
			if (line.find("Height=") == 0)
				height = stoi(line.substr(7)); // Adjust if T is not int
		}
		file.close();
		return height;


	}


	int fileBalanceFactor(string filename) {

		if (filename == "null")
			return 0;
		ifstream file(filename);
		if (!file.is_open()) {
			cerr << "Error: Could not open file " << filename << endl;
			return 0;
		}
		string line;
		string leftFile;
		string rightFile;
		while (getline(file, line)) {
			if (line.find("Left=") == 0)
				leftFile = line.substr(5);
			else if (line.find("Right=") == 0)
				rightFile = line.substr(6);
		}
		file.close();
		return fileHeight(leftFile) - fileHeight(rightFile);
	}


	void updateNodeHeight(string filename, int newHeight) {
		ifstream file(filename);
		if (!file.is_open()) {
			cerr << "Error: Could not open file " << filename << endl;
			return;
		}

		string content;
		string line;
		while (getline(file, line)) {
			if (line.find("Height=") == 0) {
				line = "Height=" + to_string(newHeight);
			}
			content += line + "\n";
		}
		file.close();

		ofstream outFile(filename);
		if (!outFile.is_open()) {
			cerr << "Error: Could not write to file " << filename << endl;
			return;
		}
		outFile << content;
		outFile.close();
	}

	void updateNodeData(string filename, int data, string leftFile, string rightFile, int height) {
		ofstream file(filename);
		if (!file.is_open()) {
			cerr << "Error: Could not open file " << filename << endl;
			return;
		}

		file << "data=" << data << endl;
		file << "Left=" << leftFile << endl;
		file << "Right=" << rightFile << endl;
		file << "Height=" << height << endl; // height is recalculated later after rotation
		file.close();
	}

	string rightRotate(string yFile) {
		ifstream y(yFile);
		if (!y.is_open()) {
			cerr << "Error: Could not open file " << yFile << endl;
			return yFile;
		}

		string line;
		string yLeftFile, yRightFile;
		int yData;
		while (getline(y, line)) {
			if (line.find("data=") == 0) {
				yData = stoi(line.substr(5));
			}
			if (line.find("Left=") == 0) {
				yLeftFile = line.substr(5);
			}
			if (line.find("Right=") == 0) {
				yRightFile = line.substr(6);
			}
		}
		y.close();

		// Step 1: Read left child node (x)
		string xFile = yLeftFile;
		ifstream x(xFile);
		if (!x.is_open()) {
			cerr << "Error: Could not open file " << xFile << endl;
			return yFile;
		}

		string xRightFile, xLeftFile;
		int xData;
		while (getline(x, line)) {
			if (line.find("data=") == 0) {
				xData = stoi(line.substr(5));
			}
			if (line.find("Right=") == 0) {
				xRightFile = line.substr(6);
			}
			if (line.find("Left=") == 0) {
				xLeftFile = line.substr(5);
			}
		}
		x.close();



		// Step 3: Perform the right rotation by updating files
		updateNodeData(yFile, yData, xRightFile, yRightFile, fileHeight(yFile));
		updateNodeData(xFile, xData, xLeftFile, yFile, fileHeight(xFile));

		// Step 4: Update the heights of the nodes
		updateNodeHeight(yFile, max(fileHeight(xRightFile), fileHeight(yRightFile)) + 1);
		updateNodeHeight(xFile, max(fileHeight(xLeftFile), fileHeight(yFile)) + 1);

		return xFile;
	}


	string leftRotate(string xFile) {
		ifstream x(xFile);
		if (!x.is_open()) {
			cerr << "Error: Could not open file " << xFile << endl;
			return xFile;
		}

		string line;
		string xLeftFile, xRightFile;
		int xData;
		while (getline(x, line)) {
			if (line.find("data=") == 0) {
				xData = stoi(line.substr(5));
			}
			if (line.find("Left=") == 0) {
				xLeftFile = line.substr(5);
			}
			if (line.find("Right=") == 0) {
				xRightFile = line.substr(6);
			}
		}
		x.close();

		// Step 1: Read right child node (y)
		string yFile = xRightFile;
		ifstream y(yFile);
		if (!y.is_open()) {
			cerr << "Error: Could not open file " << yFile << endl;
			return xFile;
		}

		string yRightFile, yLeftFile;
		int yData;
		while (getline(y, line)) {
			if (line.find("data=") == 0) {
				yData = stoi(line.substr(5));
			}
			if (line.find("Right=") == 0) {
				yRightFile = line.substr(6);
			}
			if (line.find("Left=") == 0) {
				yLeftFile = line.substr(5);
			}
		}
		y.close();

		// Step 3: Perform the left rotation by updating files
		updateNodeData(yFile, yData, xFile, yRightFile, fileHeight(yFile));
		updateNodeData(xFile, xData, xLeftFile, yLeftFile, fileHeight(xFile));

		// Step 4: Update the heights of the nodes
		updateNodeHeight(xFile, max(fileHeight(xLeftFile), fileHeight(yLeftFile)) + 1);
		updateNodeHeight(yFile, max(fileHeight(xFile), fileHeight(yRightFile)) + 1);

		return yFile;
	}


	void insert(T key) {
		string newRootFile = insertHelper(rootFile, key);
		cout << "old rootfile is : " << rootFile << "new root file is : " << newRootFile << endl;
		cout << endl;
		rootFile = newRootFile;
	}



	string insertHelper(string nodeFile, T key) {
		cout << "Inserting " << key << " into " << nodeFile << endl;
		if (nodeFile == "null") {
			string newFileName = "node_" + to_string(key) + ".txt";
			string content = "data=" + to_string(key) + "\n";
			content += "Left=null\n";
			content += "Right=null\n";
			content += "Height=1\n";
			createFile(newFileName, content);
			return newFileName;
		}

		ifstream node(nodeFile);
		if (!node.is_open()) {
			cerr << "Error: Could not open file " << nodeFile << endl;
			return nodeFile;
		}

		string line;
		string leftFile, rightFile;
		int data;
		while (getline(node, line)) {
			if (line.find("data=") == 0) {
				data = stoi(line.substr(5));
			}
			if (line.find("Left=") == 0) {
				leftFile = line.substr(5);
			}
			if (line.find("Right=") == 0) {
				rightFile = line.substr(6);
			}
		}
		node.close();

		if (key < data) {
			leftFile = insertHelper(leftFile, key);
			cout << "returned value: " << leftFile << "node file: "<<nodeFile<< endl;
		}
		else if (key > data) {
			rightFile = insertHelper(rightFile, key);
			cout << "returned value: " << rightFile << "node file: " << nodeFile << endl;

		}
		int height = fileHeight(nodeFile);
		cout << "Height of " << nodeFile << " is " << height << endl;
		updateNodeData(nodeFile, data, leftFile, rightFile, height);
		updateNodeHeight(nodeFile, max(fileHeight(leftFile), fileHeight(rightFile)) + 1);
		cout << "New Height of " << nodeFile << " is now " << fileHeight(nodeFile) << endl;
		int balance = fileBalanceFactor(nodeFile);
		cout << "Balance factor of " << nodeFile << " is " << balance << endl;
		if (balance > 1) { // Left-heavy
			ifstream leftChild(leftFile);
			int leftChildData;
			if (leftChild.is_open()) {
				string line;
				while (getline(leftChild, line)) {
					if (line.find("data=") == 0) {
						leftChildData = stoi(line.substr(5));
						break;
					}
				}
				leftChild.close();
			}
			if (key < leftChildData) {
				cout << "Right rotate on " << nodeFile << endl;
				return rightRotate(nodeFile); // Left-Left Case
			}
			else {
				cout << "Left-Right rotate on " << leftFile << endl;
				string newFile=leftRotate(leftFile);
				int nodeHeight = fileHeight(nodeFile);
				updateNodeData(nodeFile, data, newFile, rightFile,nodeHeight);
				return rightRotate(nodeFile); // Left-Right Case
			}
		}
		else if (balance < -1) { // Right-heavy
			ifstream rightChild(rightFile);
			int rightChildData;
			if (rightChild.is_open()) {
				string line;
				while (getline(rightChild, line)) {
					if (line.find("data=") == 0) {
						rightChildData = stoi(line.substr(5));
						break;
					}
				}
				rightChild.close();
			}
			if (key > rightChildData) {
				cout << "Left rotate on " << nodeFile << endl;
				return leftRotate(nodeFile); // Right-Right Case
			}
			else {
				cout << "Right-Left rotate on " << rightFile << endl;
				string newFile = rightRotate(rightFile);
				int nodeHeight = fileHeight(nodeFile);
				updateNodeData(nodeFile, data, leftFile, newFile,nodeHeight);
				return leftRotate(nodeFile); // Right-Left Case
			}
		}
			return nodeFile;
	}





	void inorderFile() {
		cout << "Root: " << rootFile << endl;
		inorderHelper(rootFile);
		cout << endl;
	}

	void inorderHelper(string filename) {
		if (filename == "null")
			return;
		ifstream file(filename);
		if (!file.is_open()) {
			cerr << "Error: Could not open file " << filename << endl;
			return;
		}
		string line;
		string leftFile, rightFile;
		int data;
		while (getline(file, line)) {
			if (line.find("data=") == 0) {
				data = stoi(line.substr(5));
			}
			if (line.find("Left=") == 0) {
				leftFile = line.substr(5);
			}
			if (line.find("Right=") == 0) {
				rightFile = line.substr(6);
			}
		}
		file.close();

		inorderHelper(leftFile);
		cout << data << " ";
		inorderHelper(rightFile);
	}



	bool searchFile(T key) {
		return searchHelper(rootFile, key);
	}

	bool searchHelper(string filename, T key) {
		if (filename == "null")
			return false;
		ifstream file(filename);
		if (!file.is_open()) {
			cerr << "Error: Could not open file " << filename << endl;
			return false;
		}
		string line;
		string leftFile, rightFile;
		int data;
		while (getline(file, line)) {
			if (line.find("data=") == 0) {
				data = stoi(line.substr(5));
			}
			if (line.find("Left=") == 0) {
				leftFile = line.substr(5);
			}
			if (line.find("Right=") == 0) {
				rightFile = line.substr(6);
			}
		}
		file.close();

		if (data == key)
			return true;
		if (key < data)
			return searchHelper(leftFile, key);
		return searchHelper(rightFile, key);
	}

	//Deletion
	// Add this method to the AVLTree class

string deleteHelper(string nodeFile, T key) {
    if (nodeFile == "null") return "null";

    string leftFile, rightFile;
    int data;

    // Read current node data
    ifstream node(nodeFile);
    if (!node.is_open()) {
        cerr << "Error: Could not open file " << nodeFile << endl;
        return nodeFile;
    }
    string line;
    while (getline(node, line)) {
        if (line.find("data=") == 0) data = stoi(line.substr(5));
        if (line.find("Left=") == 0) leftFile = line.substr(5);
        if (line.find("Right=") == 0) rightFile = line.substr(6);
    }
    node.close();

    // Perform BST delete
    if (key < data) {
        leftFile = deleteHelper(leftFile, key);
    } else if (key > data) {
        rightFile = deleteHelper(rightFile, key);
    } else {
        // Node with one or no child
        if (leftFile == "null" || rightFile == "null") {
            string temp = (leftFile != "null") ? leftFile : rightFile;
            remove(nodeFile.c_str()); // Delete the file
            return temp;
        }

        // Node with two children: Use inorder predecessor
        string maxNodeFile = findMax(leftFile);
        int maxNodeData = getNodeData(maxNodeFile);
        deleteHelper(leftFile, maxNodeData);
		renameFile(nodeFile, maxNodeFile);
		updateNodeData(maxNodeFile, maxNodeData, leftFile, rightFile,fileHeight(maxNodeFile));
		return maxNodeFile;
    }

    // Update the current node with new links
    updateNodeData(nodeFile, data, leftFile, rightFile, fileHeight(nodeFile));
    updateNodeHeight(nodeFile, max(fileHeight(leftFile), fileHeight(rightFile)) + 1);

    // Balance the node
    int balance = fileBalanceFactor(nodeFile);

    // Left Heavy
    if (balance > 1) {
        if (fileBalanceFactor(leftFile) >= 0) {
            return rightRotate(nodeFile); // Left-Left case
        } else {
            leftFile = leftRotate(leftFile); // Left-Right case
            return rightRotate(nodeFile);
        }
    }
    // Right Heavy
    if (balance < -1) {
        if (fileBalanceFactor(rightFile) <= 0) {
            return leftRotate(nodeFile); // Right-Right case
        } else {
            rightFile = rightRotate(rightFile); // Right-Left case
            return leftRotate(nodeFile);
        }
    }

    return nodeFile;
}

// Public method to delete a key
void deleteKey(T key) {
    rootFile = deleteHelper(rootFile, key);
    cout << "Deleted " << key << ". Updated root file: " << rootFile << endl;
}

	// Helper to find the maximum value in a subtree
	string findMax(string nodeFile) {
		while (true) {
			ifstream file(nodeFile);
			if (!file.is_open()) {
				cerr << "Error: Could not open file " << nodeFile << endl;
				return "null";
			}
			string line, rightFile;
			while (getline(file, line)) {
				if (line.find("Right=") == 0) rightFile = line.substr(6);
			}
			file.close();

			if (rightFile == "null") break;
			nodeFile = rightFile;
		}
		return nodeFile;
	}

	// Helper to get node data from a file
	int getNodeData(string nodeFile) {
		ifstream file(nodeFile);
		if (!file.is_open()) {
			cerr << "Error: Could not open file " << nodeFile << endl;
			return -1; // Error case
		}
		string line;
		int data;
		while (getline(file, line)) {
			if (line.find("data=") == 0) {
				data = stoi(line.substr(5));
				break;
			}
		}
		file.close();
		return data;
	}

	AVLTree()
	{
		rootFile = "null";
	}

};