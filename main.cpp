/**************************************
* FAST University Islamabad           *
* Course: Data Structures FALL 2024   *
* SEMESTER PROJECT: GITLITE           *
* SECTION: CS-D                       *
* GROUP MEMBERS:                      *
* 1. MUHAMMAD UMER ABBAS (23I-0692)   *
* 2. ADNAN AHMAD (23I-0715)           *
* 3. ABDULLAH DURRANI (23I-0505)      *
* 4. MUHAMMAD ZAIN (23I-0855)         *
***************************************/


#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <experimental/filesystem> // For filesystem operations
#include <iomanip>
#include <Windows.h>
#include <wincrypt.h>
#pragma comment(lib, "Crypt32.lib")
using namespace std;
namespace fs = experimental::filesystem;


/***********************************************************
					File Handling Functions
 ***********************************************************/
void createFile(const string& filename, const string& content) {
	ofstream file(filename); // Open in write mode (creates file if it doesn't exist)
	if (file) {
		file << content;
		cout << "File created and written successfully.\n";
	}
	else {
		cerr << "Error creating file.\n";
	}
	file.close();
}
void readFile(const string& filename) {
	ifstream file(filename); // Open in read mode
	if (file) {
		string line;
		cout << "Reading file contents:\n";
		while (getline(file, line)) {
			cout << line << endl;
		}
	}
	else {
		cerr << "Error reading file. File might not exist.\n";
	}
	file.close();
}
void updateFile(const string& filename, const string& newContent) {
	fstream file(filename, ios::in | ios::out); // Open in read and write mode
	if (file) {
		file.seekp(0, ios::end); // Move the write pointer to the end
		file << "\n" << newContent;
		cout << "File updated successfully.\n";
	}
	else {
		cerr << "Error updating file. File might not exist.\n";
	}
	file.close();
}
void deleteFile(const string& filename) {
	if (remove(filename.c_str()) == 0) {
		cout << "File deleted successfully.\n";
	}
	else {
		cerr << "Error deleting file. File might not exist.\n";
	}
}
void renameFile(const string& oldName, const string& newName) {
	// Open the old file in read mode
	ifstream oldFile(oldName);
	if (!oldFile) {
		cerr << "Error: Unable to open the old file.\n";
		return;
	}

	// Create a new file and write the content of the old file to it
	ofstream newFile(newName);
	if (!newFile) {
		cerr << "Error: Unable to create the new file.\n";
		oldFile.close();
		return;
	}

	string line;
	while (getline(oldFile, line)) {
		newFile << line << endl;
	}

	oldFile.close();
	newFile.close();

	// Delete the old file after successfully creating the new file
	if (remove(oldName.c_str()) == 0) {
		cout << "File renamed successfully.\n";
	}
	else {
		cerr << "Error deleting the old file.\n";
	}
}


/***********************************************************
					Hashing Functions
***********************************************************/
string sha256(const string& input) {
	// Initialize variables
	HCRYPTPROV hCryptProv = NULL;
	HCRYPTHASH hHash = NULL;
	BYTE rgbHash[32];
	DWORD cbHash = 32;
	stringstream ss;

	// Acquire a cryptographic provider
	if (!CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
		cerr << "CryptAcquireContext failed" << endl;
		return "";
	}

	// Create a hash object
	if (!CryptCreateHash(hCryptProv, CALG_SHA_256, 0, 0, &hHash)) {
		cerr << "CryptCreateHash failed" << endl;
		CryptReleaseContext(hCryptProv, 0);
		return "";
	}

	// Hash the input data
	if (!CryptHashData(hHash, reinterpret_cast<const BYTE*>(input.c_str()), input.length(), 0)) {
		cerr << "CryptHashData failed" << endl;
		CryptDestroyHash(hHash);
		CryptReleaseContext(hCryptProv, 0);
		return "";
	}

	// Get the hash value
	if (!CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0)) {
		cerr << "CryptGetHashParam failed" << endl;
		CryptDestroyHash(hHash);
		CryptReleaseContext(hCryptProv, 0);
		return "";
	}

	// Convert to hexadecimal string
	for (DWORD i = 0; i < cbHash; i++) {
		ss << hex << setw(2) << setfill('0') << static_cast<int>(rgbHash[i]);
	}

	// Clean up
	CryptDestroyHash(hHash);
	CryptReleaseContext(hCryptProv, 0);

	return ss.str();
}
int instructorHash(int number) {
	int hash = 1;
	while (number > 0) {
		hash *= number % 10;
		number /= 10;
	}
	return hash % 29;
}
int instructorHash(const string& str) {
	long long hash = 1;
	for (int i = 0; i < str.length(); i++) {
		hash *= str[i];
	}
	return hash % 29;
}


/***********************************************************
					 AVL TREE CLASS
 ***********************************************************/
class AVLTree {
public:
	string rootFolder = "tree/";
	string rootFile;
	int* selectedColumns;
	const int numColumns = 3;

	void setSelectColumns(int a, int b, int c) {
		selectedColumns[0] = a; //current selected column
		selectedColumns[1] = b; //prev column
		selectedColumns[2] = c; //next column
	}

	string getSelectedColumn(string data) {
		stringstream ss(data);
		string item;
		int index = 0;
		while (getline(ss, item, ',')) {
			if (index == selectedColumns[0]) {
				return item;
			}
			index++;
		}
		return "";
	}

	void printTree() {
		printFileTree(rootFile);
	}


	void printFileTree(string filename, string indent = "", bool isLeft = true) {
		if (filename == "null") {
			cout << indent << (isLeft ? "|-- " : "^-- ") << "NULL" << endl;
			return;
		}

		ifstream file(resolveFilePath(filename));
		if (!file.is_open()) {
			cerr << "Error: Could not open file " << filename << endl;
			return;
		}

		string line;
		string leftFile, rightFile;
		string data;
		while (getline(file, line)) {
			if (line.find("data=") == 0) {
				data = line.substr(5);
			}
			if (line.find("Left=") == 0) {
				leftFile = line.substr(5);
			}
			if (line.find("Right=") == 0) {
				rightFile = line.substr(6);
			}
		}
		file.close();

		cout << indent << (isLeft ? "|-- " : "^-- ") << getSelectedColumn(data) << endl;

		indent += isLeft ? "|   " : "    ";
		printFileTree(leftFile, indent, true);
		printFileTree(rightFile, indent, false);
	}


	string computeHash(string data) {
		cout << "Data: " << data << endl;
		int commaCount = 0;
		for (char c : data) {
			if (c == ',') {
				commaCount++;
			}
		}

		// Total values will be commas + 1
		int numValues = commaCount + 1;

		// Dynamically allocate the array
		string* values = new string[numValues];

		// Split the string by commas and populate the array
		stringstream ss(data);
		string item;
		int index = 0;
		while (getline(ss, item, ',')) {
			values[index++] = item;
		}


		// Determine the selected, previous, and next columns
		string selectedValue, previousValue, nextValue;


		// Get the selected value
		selectedValue = values[selectedColumns[0]];
		previousValue = values[selectedColumns[1]];
		nextValue = values[selectedColumns[2]];

		// Combine the three values into keyData
		string keyData = previousValue + "_" + selectedValue + "_" + nextValue;
		cout << "Key: " << keyData << endl;

		// Generate hash for the key
		string hashKey = sha256(keyData);
		return hashKey;

	}


	int fileHeight(string filename) {

		if (filename == "null")
			return 0;
		ifstream file(resolveFilePath(filename));
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
		ifstream file(resolveFilePath(filename));
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
		ifstream file(resolveFilePath(filename));
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

		ofstream outFile(resolveFilePath(filename));
		if (!outFile.is_open()) {
			cerr << "Error: Could not write to file " << filename << endl;
			return;
		}
		outFile << content;
		outFile.close();
	}

	void updateNodeData(string filename, string data, string leftFile, string rightFile, int height) {
		ofstream file(resolveFilePath(filename));
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
		ifstream y(resolveFilePath(yFile));
		if (!y.is_open()) {
			cerr << "Error: Could not open file " << yFile << endl;
			return yFile;
		}

		string line;
		string yLeftFile, yRightFile;
		string yData;
		while (getline(y, line)) {
			if (line.find("data=") == 0) {
				yData = line.substr(5);
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
		ifstream x(resolveFilePath(xFile));
		if (!x.is_open()) {
			cerr << "Error: Could not open file " << xFile << endl;
			return yFile;
		}

		string xRightFile, xLeftFile;
		string xData;
		while (getline(x, line)) {
			if (line.find("data=") == 0) {
				xData = line.substr(5);
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
		ifstream x(resolveFilePath(xFile));
		if (!x.is_open()) {
			cerr << "Error: Could not open file " << xFile << endl;
			return xFile;
		}

		string line;
		string xLeftFile, xRightFile;
		string xData;
		while (getline(x, line)) {
			if (line.find("data=") == 0) {
				xData = line.substr(5);
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
		ifstream y(resolveFilePath(yFile));
		if (!y.is_open()) {
			cerr << "Error: Could not open file " << yFile << endl;
			return xFile;
		}

		string yRightFile, yLeftFile;
		string yData;
		while (getline(y, line)) {
			if (line.find("data=") == 0) {
				yData = line.substr(5);
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


	void insert(string key) {
		string newRootFile = insertHelper(rootFile, key);
		cout << "old rootfile is : " << rootFile << "new root file is : " << newRootFile << endl;
		cout << endl;
		rootFile = newRootFile;
	}



	string insertHelper(string nodeFile, string key) {
		cout << "Inserting " << key << " into " << nodeFile << endl;
		string newHash = computeHash(key);
		if (nodeFile == "null") {
			string newFileName = "node_" + newHash + ".txt";
			string content = "data=" + key + "\n";
			content += "Left=null\n";
			content += "Right=null\n";
			content += "Height=1\n";
			createFile(resolveFilePath(newFileName), content);
			return newFileName;
		}

		ifstream node(resolveFilePath(nodeFile));
		if (!node.is_open()) {
			cerr << "Error: Could not open file " << nodeFile << endl;
			return nodeFile;
		}

		string line;
		string leftFile, rightFile;
		string data;
		while (getline(node, line)) {
			if (line.find("data=") == 0) {
				data = line.substr(5);
			}
			if (line.find("Left=") == 0) {
				leftFile = line.substr(5);
			}
			if (line.find("Right=") == 0) {
				rightFile = line.substr(6);
			}
		}
		string currentHash = computeHash(data);
		node.close();

		if (newHash < currentHash) {
			leftFile = insertHelper(leftFile, key);
			cout << "returned value: " << leftFile << "node file: " << nodeFile << endl;
		}
		else if (newHash > currentHash) {
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
			ifstream leftChild(resolveFilePath(leftFile));
			string leftChildData;
			if (leftChild.is_open()) {
				string line;
				while (getline(leftChild, line)) {
					if (line.find("data=") == 0) {
						leftChildData = line.substr(5);
						break;
					}
				}
				leftChild.close();
			}
			string leftChildHash = computeHash(leftChildData);
			if (newHash < leftChildHash) {
				cout << "Right rotate on " << nodeFile << endl;
				return rightRotate(nodeFile); // Left-Left Case
			}
			else {
				cout << "Left-Right rotate on " << leftFile << endl;
				string newFile = leftRotate(leftFile);
				int nodeHeight = fileHeight(nodeFile);
				updateNodeData(nodeFile, data, newFile, rightFile, nodeHeight);
				return rightRotate(nodeFile); // Left-Right Case
			}
		}
		else if (balance < -1) { // Right-heavy
			ifstream rightChild(resolveFilePath(rightFile));
			string rightChildData;
			if (rightChild.is_open()) {
				string line;
				while (getline(rightChild, line)) {
					if (line.find("data=") == 0) {
						rightChildData = line.substr(5);
						break;
					}
				}
				rightChild.close();
			}
			string rightChildHash = computeHash(rightChildData);
			if (newHash > rightChildHash) {
				cout << "Left rotate on " << nodeFile << endl;
				return leftRotate(nodeFile); // Right-Right Case
			}
			else {
				cout << "Right-Left rotate on " << rightFile << endl;
				string newFile = rightRotate(rightFile);
				int nodeHeight = fileHeight(nodeFile);
				updateNodeData(nodeFile, data, leftFile, newFile, nodeHeight);
				return leftRotate(nodeFile); // Right-Left Case
			}
		}
		return nodeFile;
	}





	void inorderFile() {
		cout << "Root: " << rootFile << endl;
		int index = 1; // Initialize the index counter
		inorderHelper(rootFile, index); // Pass the counter by reference
		cout << endl;
	}

	void inorderHelper(string filename, int& index) {
		if (filename == "null")
			return;

		ifstream file(resolveFilePath(filename));
		if (!file.is_open()) {
			cerr << "Error: Could not open file " << filename << endl;
			return;
		}

		string line;
		string leftFile, rightFile;
		string data;
		while (getline(file, line)) {
			if (line.find("data=") == 0) {
				data = line.substr(5);
			}
			if (line.find("Left=") == 0) {
				leftFile = line.substr(5);
			}
			if (line.find("Right=") == 0) {
				rightFile = line.substr(6);
			}
		}
		file.close();

		// Recursive call for the left subtree
		inorderHelper(leftFile, index);

		// Print the current node with the index
		cout << "----------------------------------------\n";
		cout << index++ << ". " << data << endl; // Increment the index after printing
		cout << "----------------------------------------\n";

		// Recursive call for the right subtree
		inorderHelper(rightFile, index);
	}

	string getDataByIndex(int targetIndex) {
		int currentIndex = 1; // Start with the first index
		string result = "Index not found"; // Default value if index is invalid
		getDataByIndexHelper(rootFile, targetIndex, currentIndex, result);
		return result;
	}

	void getDataByIndexHelper(const string& filename, int targetIndex, int& currentIndex, string& result) {
		if (filename == "null")
			return;

		ifstream file(resolveFilePath(filename));
		if (!file.is_open()) {
			cerr << "Error: Could not open file " << filename << endl;
			return;
		}

		string line;
		string leftFile, rightFile;
		string data;
		while (getline(file, line)) {
			if (line.find("data=") == 0) {
				data = line.substr(5);
			}
			if (line.find("Left=") == 0) {
				leftFile = line.substr(5);
			}
			if (line.find("Right=") == 0) {
				rightFile = line.substr(6);
			}
		}
		file.close();

		// Search the left subtree
		getDataByIndexHelper(leftFile, targetIndex, currentIndex, result);

		// Check if the current index matches the target index
		if (currentIndex == targetIndex) {
			result = data;
			return;
		}
		currentIndex++;

		// Search the right subtree
		getDataByIndexHelper(rightFile, targetIndex, currentIndex, result);
	}



	bool searchFile(string key) {
		return searchHelper(rootFile, key);
	}

	bool searchHelper(string filename, string key) {
		string newHash = computeHash(key);
		if (filename == "null")
			return false;
		ifstream file(resolveFilePath(filename));
		if (!file.is_open()) {
			cerr << "Error: Could not open file " << filename << endl;
			return false;
		}
		string line;
		string leftFile, rightFile;
		string data;
		while (getline(file, line)) {
			if (line.find("data=") == 0) {
				data = line.substr(5);
			}
			if (line.find("Left=") == 0) {
				leftFile = line.substr(5);
			}
			if (line.find("Right=") == 0) {
				rightFile = line.substr(6);
			}
		}
		file.close();
		string currentHash = computeHash(data);

		if (currentHash == newHash)
			return true;
		if (newHash < currentHash)
			return searchHelper(leftFile, key);
		return searchHelper(rightFile, key);
	}

	//Deletion
	// Add this method to the AVLTree class

	string deleteHelper(string nodeFile, string key) {
		if (nodeFile == "null") return "null";

		string leftFile, rightFile;
		string data;

		// Read current node data
		ifstream node(resolveFilePath(nodeFile));
		if (!node.is_open()) {
			cerr << "Error: Could not open file " << nodeFile << endl;
			return nodeFile;
		}
		string line;
		while (getline(node, line)) {
			if (line.find("data=") == 0) data = line.substr(5);
			if (line.find("Left=") == 0) leftFile = line.substr(5);
			if (line.find("Right=") == 0) rightFile = line.substr(6);
		}
		node.close();

		string nodeHash = computeHash(data);
		string keyHash = computeHash(key);

		// Perform BST delete
		if (keyHash < nodeHash) {
			leftFile = deleteHelper(leftFile, key);
		}
		else if (keyHash > nodeHash) {
			rightFile = deleteHelper(rightFile, key);
		}
		else {
			cout << "LeftChild: " << leftFile << " RightChild: " << rightFile << endl;
			// Node with one or no child
			if (leftFile == "null" || rightFile == "null") {
				string temp = (leftFile != "null") ? leftFile : rightFile;
				remove(resolveFilePath(nodeFile).c_str()); // Delete the file
				return temp;
			}
			cout << "----------------------------------------\n";
			// Node with two children: Use inorder predecessor
			string maxNodeFile = findMax(leftFile);
			cout << "Max Node: " << maxNodeFile << endl;
			string maxNodeData = getNodeData(maxNodeFile);
			cout << "Max Node Data: " << maxNodeData << endl;
			deleteKey(maxNodeData);
			//reRead the left and right files after deletion
			ifstream node(resolveFilePath(nodeFile));
			if (!node.is_open()) {
				cerr << "Error: Could not open file " << nodeFile << endl;
				return nodeFile;
			}
			string line;
			while (getline(node, line)) {
				if (line.find("Left=") == 0) leftFile = line.substr(5);
				if (line.find("Right=") == 0) rightFile = line.substr(6);
			}
			node.close();
			cout << "Deleted " << maxNodeData << " from " << leftFile << endl;
			renameFile(resolveFilePath(nodeFile), resolveFilePath(maxNodeFile));
			cout << "Renamed " << nodeFile << " to " << maxNodeFile << endl;
			updateNodeData(maxNodeFile, maxNodeData, leftFile, rightFile, fileHeight(maxNodeFile));
			cout << "Updated " << maxNodeFile << " with new data" << endl;
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
			}
			else {
				leftFile = leftRotate(leftFile); // Left-Right case
				return rightRotate(nodeFile);
			}
		}
		// Right Heavy
		if (balance < -1) {
			if (fileBalanceFactor(rightFile) <= 0) {
				return leftRotate(nodeFile); // Right-Right case
			}
			else {
				rightFile = rightRotate(rightFile); // Right-Left case
				return leftRotate(nodeFile);
			}
		}

		return nodeFile;
	}

	// Public method to delete a key
	void deleteKey(string key) {
		rootFile = deleteHelper(rootFile, key);
		cout << "Deleted " << key << ". Updated root file: " << rootFile << endl;
	}

	// Helper to find the maximum value in a subtree
	string findMax(string nodeFile) {
		while (true) {
			ifstream file(resolveFilePath(nodeFile));
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
	string getNodeData(string nodeFile) {

		ifstream file(resolveFilePath(nodeFile));
		if (!file.is_open()) {
			cerr << "Error: Could not open file " << nodeFile << endl;
			return ""; // Error case
		}
		string line;
		string data;
		while (getline(file, line)) {
			if (line.find("data=") == 0) {
				data = line.substr(5);
				break;
			}
		}
		file.close();
		return data;
	}

	string rootMerkleHash() {
		return rootMerkleHashHelper(rootFile);
	}

	string rootMerkleHashHelper(string filename) {
		if (filename == "null")
			return "";
		ifstream file(resolveFilePath(filename));
		if (!file.is_open()) {
			cerr << "Error: Could not open file " << filename << endl;
			return "";
		}
		string line;
		string leftFile, rightFile;
		string data;
		while (getline(file, line)) {
			if (line.find("data=") == 0) {
				data = line.substr(5);
			}
			if (line.find("Left=") == 0) {
				leftFile = line.substr(5);
			}
			if (line.find("Right=") == 0) {
				rightFile = line.substr(6);
			}
		}
		file.close();

		string leftHash = rootMerkleHashHelper(leftFile);
		string rightHash = rootMerkleHashHelper(rightFile);
		return merkleHash(computeHash(data), merkleHash(leftHash, rightHash));
	}

	string merkleHash(string leftHash, string rightHash) {
		string combined = leftHash + rightHash;
		return sha256(combined);
	}
	string resolveFilePath(const string& filename) {
		if (filename == "null") {
			return "null";
		}
		return rootFolder + filename;
	}

	AVLTree()
	{
		rootFile = "null";
		selectedColumns = new int[3];
		if (!fs::exists(rootFolder)) {
			fs::create_directory(rootFolder);
		}
		else {
			fs::remove_all(rootFolder);
			fs::create_directory(rootFolder);
		}
	}

};


/***********************************************************
					Red Black Tree Class/
***********************************************************/
template <typename T> class RedBlackTree {
public:
	string rootFile;

	void updateNodeData(string filename, T data, string leftFile, string rightFile, bool isRed) {
		ofstream file(filename);
		if (!file.is_open()) {
			cerr << "Error: Could not open file " << filename << endl;
			return;
		}

		file << "data=" << data << endl;
		file << "Left=" << leftFile << endl;
		file << "Right=" << rightFile << endl;
		file << "isRed=" << isRed << endl;
		file.close();
		cout << "Updated node " << filename << " with data=" << data << ", left=" << leftFile << ", right=" << rightFile << ", isRed=" << isRed << endl;
	}

	bool isRed(string filename) {
		if (filename == "null") return false;

		ifstream file(filename);
		if (!file.is_open()) {
			cerr << "Error: Could not open file " << filename << endl;
			return false;
		}

		string line;
		while (getline(file, line)) {
			if (line.find("isRed=") == 0) {
				bool red = line.substr(6) == "1";
				cout << "Node " << filename << " isRed=" << red << endl;
				return red;
			}
		}
		file.close();
		return false;
	}
	string rotateLeft(string yFile) {
		ifstream y(yFile);
		if (!y.is_open()) {
			cerr << "Error: Could not open file " << yFile << endl;
			return yFile;
		}

		string line;
		string yLeftFile, yRightFile;
		int yData;
		bool yIsRed;
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
			if (line.find("isRed=") == 0) yIsRed = (line.substr(6) == "1");
		}
		y.close();

		// Read right child node (x)
		string xFile = yRightFile;
		ifstream x(xFile);
		if (!x.is_open()) {
			cerr << "Error: Could not open file " << xFile << endl;
			return yFile;
		}
		bool xIsRed;
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
			if (line.find("isRed=") == 0) xIsRed = (line.substr(6) == "1");
		}
		x.close();

		// Perform the left rotation by updating files
		updateNodeData(yFile, yData, yLeftFile, xLeftFile, yIsRed);
		updateNodeData(xFile, xData, yFile, xRightFile, xIsRed);

		return xFile;
	}

	string rotateRight(string yFile) {
		ifstream y(yFile);
		if (!y.is_open()) {
			cerr << "Error: Could not open file " << yFile << endl;
			return yFile;
		}

		string line;
		string yLeftFile, yRightFile;
		int yData;
		bool yIsRed;
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
			if (line.find("isRed=") == 0) yIsRed = (line.substr(6) == "1");

		}
		y.close();

		// Step 1: Read left child node (x)
		string xFile = yLeftFile;
		ifstream x(xFile);
		if (!x.is_open()) {
			cerr << "Error: Could not open file " << xFile << endl;
			return yFile;
		}
		bool xIsRed;
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
			if (line.find("isRed=") == 0) xIsRed = (line.substr(6) == "1");
		}
		x.close();

		// Step 3: Perform the right rotation by updating files
		updateNodeData(yFile, yData, xRightFile, yRightFile, yIsRed);
		updateNodeData(xFile, xData, xLeftFile, yFile, xIsRed);

		return xFile;
	}



	void flipColors(string h) {
		cout << "Flipping colors for node " << h << endl;
		ifstream hFile(h);
		if (!hFile.is_open()) {
			cerr << "Error: Could not open file " << h << endl;
			return;
		}

		string line;
		string hLeftFile, hRightFile;
		T hData;
		bool hIsRed;
		while (getline(hFile, line)) {
			if (line.find("data=") == 0) hData = stoi(line.substr(5));
			if (line.find("Left=") == 0) hLeftFile = line.substr(5);
			if (line.find("Right=") == 0) hRightFile = line.substr(6);
			if (line.find("isRed=") == 0) hIsRed = (line.substr(6) == "1");
		}
		hFile.close();

		updateNodeData(h, hData, hLeftFile, hRightFile, !hIsRed);

		if (hLeftFile != "null") {
			ifstream leftFile(hLeftFile);
			T leftData;
			string leftLeft, leftRight;
			bool leftIsRed;
			while (getline(leftFile, line)) {
				if (line.find("data=") == 0) leftData = stoi(line.substr(5));
				if (line.find("Left=") == 0) leftLeft = line.substr(5);
				if (line.find("Right=") == 0) leftRight = line.substr(6);
				if (line.find("isRed=") == 0) leftIsRed = (line.substr(6) == "1");
			}
			leftFile.close();
			updateNodeData(hLeftFile, leftData, leftLeft, leftRight, !leftIsRed);
		}

		if (hRightFile != "null") {
			ifstream rightFile(hRightFile);
			T rightData;
			string rightLeft, rightRight;
			bool rightIsRed;
			while (getline(rightFile, line)) {
				if (line.find("data=") == 0) rightData = stoi(line.substr(5));
				if (line.find("Left=") == 0) rightLeft = line.substr(5);
				if (line.find("Right=") == 0) rightRight = line.substr(6);
				if (line.find("isRed=") == 0) rightIsRed = (line.substr(6) == "1");
			}
			rightFile.close();
			updateNodeData(hRightFile, rightData, rightLeft, rightRight, !rightIsRed);
		}
		cout << "Color flip completed" << endl;
	}

	string insertHelper(string h, T key) {
		cout << "Inserting " << key << " into " << h << endl;
		if (h == "null") {
			string newFileName = "rb_node_" + to_string(key) + ".txt";
			string content = "data=" + to_string(key) + "\n";
			content += "Left=null\n";
			content += "Right=null\n";
			content += "isRed=1\n";
			createFile(newFileName, content);
			cout << "Created new red node: " << newFileName << endl;
			return newFileName;
		}

		ifstream hFile(h);
		string line;
		string leftFile, rightFile;
		T data;
		bool isRedVal;
		while (getline(hFile, line)) {
			if (line.find("data=") == 0) data = stoi(line.substr(5));
			if (line.find("Left=") == 0) leftFile = line.substr(5);
			if (line.find("Right=") == 0) rightFile = line.substr(6);
			if (line.find("isRed=") == 0) isRedVal = (line.substr(6) == "1");
		}
		hFile.close();

		if (key < data) {
			cout << "Going left from " << h << endl;
			leftFile = insertHelper(leftFile, key);
		}
		else if (key > data) {
			cout << "Going right from " << h << endl;
			rightFile = insertHelper(rightFile, key);

		}
		else return h;

		updateNodeData(h, data, leftFile, rightFile, isRedVal);

		// Fix Red-Black tree violations

		if (isRed(leftFile) && isRed(getLeftFile(leftFile)))
		{
			if (isRed(rightFile))
			{
				updateColor(rightFile, false);
				updateColor(h, true);
				updateColor(leftFile, false);
			}
			else
			{
				h = rotateRight(h);

				updateColor(h, false);
				updateColor(getRightFile(h), true);
			}
		}
		if (isRed(rightFile) && isRed(getRightFile(rightFile)))
		{
			if (isRed(leftFile))
			{
				updateColor(leftFile, false);
				updateColor(h, true);
				updateColor(rightFile, false);
			}
			else
			{
				h = rotateLeft(h);
				updateColor(h, false);
				updateColor(getLeftFile(h), true);
			}
		}
		if (isRed(leftFile) && isRed(getRightFile(leftFile)))
		{
			if (isRed(rightFile))
			{
				updateColor(rightFile, false);
				updateColor(h, true);
				updateColor(leftFile, false);
			}
			else
			{
				leftFile = rotateLeft(leftFile);
				updateNodeData(h, data, leftFile, rightFile, isRedVal);
				h = rotateRight(h);
				updateColor(h, false);
				updateColor(getRightFile(h), true);
			}
		}
		if (isRed(rightFile) && isRed(getLeftFile(rightFile)))
		{
			if (isRed(leftFile))
			{
				updateColor(leftFile, false);
				updateColor(h, true);
				updateColor(rightFile, false);
			}
			else
			{
				rightFile = rotateRight(rightFile);
				updateNodeData(h, data, leftFile, rightFile, isRedVal);
				h = rotateLeft(h);
				updateColor(h, false);
				updateColor(getLeftFile(h), true);
			}
		}
		return h;
	}

	void inorderHelper(string filename) {
		if (filename == "null") return;

		ifstream file(filename);
		if (!file.is_open()) {
			cerr << "Error: Could not open file " << filename << endl;
			return;
		}

		string line;
		string leftFile, rightFile;
		T data;
		bool isRed;
		while (getline(file, line)) {
			if (line.find("data=") == 0) data = stoi(line.substr(5));
			if (line.find("Left=") == 0) leftFile = line.substr(5);
			if (line.find("Right=") == 0) rightFile = line.substr(6);
			if (line.find("isRed=") == 0) isRed = (line.substr(6) == "1");
		}
		file.close();

		inorderHelper(leftFile);
		cout << data << "(" << (isRed ? "R" : "B") << ") ";
		inorderHelper(rightFile);
	}

	RedBlackTree() {
		rootFile = "null";
		cout << "Created new Red-Black Tree with null root" << endl;
	}

	void insert(T key) {
		cout << "\nInserting " << key << " into Red-Black Tree" << endl;
		cout << "Current root: " << rootFile << endl;

		rootFile = insertHelper(rootFile, key);

		// Root must always be black
		ifstream root(rootFile);
		if (root.is_open()) {
			string line;
			T rootData;
			string leftFile, rightFile;
			while (getline(root, line)) {
				if (line.find("data=") == 0) rootData = stoi(line.substr(5));
				if (line.find("Left=") == 0) leftFile = line.substr(5);
				if (line.find("Right=") == 0) rightFile = line.substr(6);
			}
			root.close();
			updateNodeData(rootFile, rootData, leftFile, rightFile, false);
		}
		cout << "New root: " << rootFile << " (set to black)" << endl;
	}

	void inorder() {
		cout << "\nInorder traversal of Red-Black Tree:" << endl;
		cout << "Root: " << rootFile << endl;
		inorderHelper(rootFile);
		cout << endl;
	}
	string getLeftFile(string filename) {
		if (filename == "null") return "null";

		ifstream file(filename);
		if (!file.is_open()) {
			cerr << "Error: Could not open file " << filename << endl;
			return "null";
		}

		string line;
		string leftFile;
		while (getline(file, line)) {
			if (line.find("Left=") == 0) {
				leftFile = line.substr(5);
				break;
			}
		}
		file.close();
		return leftFile;
	}

	string getRightFile(string filename) {
		if (filename == "null") return "null";

		ifstream file(filename);
		if (!file.is_open()) {
			cerr << "Error: Could not open file " << filename << endl;
			return "null";
		}

		string line;
		string rightFile;
		while (getline(file, line)) {
			if (line.find("Right=") == 0) {
				rightFile = line.substr(6);
				break;
			}
		}
		file.close();
		return rightFile;
	}

	string findParent(string nodeFile) {
		if (nodeFile == "null" || nodeFile == rootFile) {
			return "null";
		}

		string current = rootFile;
		T nodeData;

		// Get node's data value
		ifstream nodeStream(nodeFile);
		string line;
		while (getline(nodeStream, line)) {
			if (line.find("data=") == 0) {
				nodeData = stoi(line.substr(5));
				break;
			}
		}
		nodeStream.close();

		while (current != "null")
		{
			string leftChild = getLeftFile(current);
			string rightChild = getRightFile(current);

			if (leftChild == nodeFile || rightChild == nodeFile) {
				return current;
			}

			ifstream currentFile(current);
			T currentData;
			while (getline(currentFile, line)) {
				if (line.find("data=") == 0) {
					currentData = stoi(line.substr(5));
					break;
				}
			}
			currentFile.close();

			if (nodeData < currentData) {
				current = leftChild;
			}
			else {
				current = rightChild;
			}
		}
		return "null";
	}

	string findSibling(string nodeFile) {
		if (nodeFile == "null" || nodeFile == rootFile) {
			return "null"; // Root or null node has no sibling
		}

		string parent = findParent(nodeFile);
		if (parent == "null") {
			return "null";
		}

		string leftChild = getLeftFile(parent);
		string rightChild = getRightFile(parent);

		if (leftChild == nodeFile) {
			return rightChild;
		}
		else {
			return leftChild;
		}
	}

	string findUncle(string nodeFile) {
		if (nodeFile == "null" || nodeFile == rootFile) {
			return "null"; // Root or null node has no uncle
		}

		string parent = findParent(nodeFile);
		if (parent == "null" || parent == rootFile) {
			return "null"; // No uncle if no parent or parent is root
		}

		string grandParent = findParent(parent);
		if (grandParent == "null") {
			return "null"; // No uncle if no grandparent
		}

		// Get uncle - if parent is left child, uncle is right child and vice versa
		string leftGP = getLeftFile(grandParent);
		string rightGP = getRightFile(grandParent);

		if (leftGP == parent) {
			return rightGP;
		}
		else {
			return leftGP;
		}
	}

	string findGrandParent(string nodeFile) {
		if (nodeFile == "null" || nodeFile == rootFile) {
			return "null"; // Root or null node has no grandparent
		}

		string parent = findParent(nodeFile);
		if (parent == "null" || parent == rootFile) {
			return "null"; // No grandparent if no parent or parent is root
		}

		return findParent(parent); // Return parent's parent (grandparent)
	}

	void updateColor(string filename, bool newColor) {
		if (filename == "null") return;

		ifstream inFile(filename);
		if (!inFile.is_open()) {
			cerr << "Error: Could not open file " << filename << endl;
			return;
		}

		// Read current node data
		string line;
		T data;
		string leftFile, rightFile;
		while (getline(inFile, line)) {
			if (line.find("data=") == 0) data = stoi(line.substr(5));
			if (line.find("Left=") == 0) leftFile = line.substr(5);
			if (line.find("Right=") == 0) rightFile = line.substr(6);
		}
		inFile.close();

		// Update node with new color
		updateNodeData(filename, data, leftFile, rightFile, newColor);
		cout << "new Color is: " << newColor << endl;
		cout << "Updated color of node " << filename << " to " << (newColor ? "Red" : "Black") << endl;
	}

	void renameFile(const string& oldName, const string& newName) {
		if (rename(oldName.c_str(), newName.c_str()) != 0) {
			cerr << "Error renaming file " << oldName << " to " << newName << endl;
			return;
		}
		cout << "Renamed file " << oldName << " to " << newName << endl;
	}
};



/***********************************************************
					B Tree Class/
***********************************************************/
template <typename T, int ORDER> class BTreeNode {
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
		/*	if (remove(filename.c_str()) == 0) {
				cout << "Node file " << filename << " deleted successfully.\n";
			}
			else {
				cerr << "Error deleting node file: " << filename << endl;
			}*/
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


/***********************************************************
					MENU FUNCTIONS
 ***********************************************************/

void displayTreeStructure(AVLTree& tree) {
	cout << "\nDisplaying tree structure...\n";
	tree.printTree();
	// Code to visualize the tree structure
}

void displayMenu() {
	cout << "\033[2J\033[H"; // ANSI escape sequence to clear screen

	cout << "\033[1;32m";
	cout << "====================================\n";
	cout << "      Welcome to GitLite Menu       \n";
	cout << "====================================\n";
	cout << "\033[0m";

	cout << "\nSelect an action:\n";
	cout << "------------------------------------\n";
	cout << "1. Create a Commit\n";
	cout << "2. Create a Branch\n";
	cout << "3. Switch Branch\n";
	cout << "4. View Commit History\n";
	cout << "5. Display Tree Structure\n";
	cout << "6. Merge Branches\n";
	cout << "7. Delete a Branch\n";
	cout << "8. Revert Commit\n";
	cout << "9. Add Node\n";
	cout << "10. Update Node\n";
	cout << "11. Delete Node\n";
	cout << "12. Exit\n";
	cout << "------------------------------------\n";
	cout << "Enter your choice: ";
}

void displayInitializationHeader() {
	cout << "\033[1;34m";
	cout << "====================================\n";
	cout << "      GitLite Initialization         \n";
	cout << "====================================\n";
	cout << "\033[0m";
}

void customStrCopy(char*& dest, const string& src) {
	// Allocate memory for the destination string
	dest = new char[src.length() + 1];
	// Copy each character from the source string to the destination
	for (size_t i = 0; i < src.length(); i++) {
		dest[i] = src[i];
	}
	// Add the null terminator at the end
	dest[src.length()] = '\0';
}

bool readCSVFileAndDisplayColumns(const string& filename, char**& columns, int& numColumns) {
	ifstream file(filename);
	if (!file.is_open()) {
		cout << "Error: Could not open file '" << filename << "'.\n";
		return false;
	}

	string line;
	// Read the first line (column names)
	if (getline(file, line)) {
		// Count the number of commas in the line to determine the number of columns
		numColumns = 1;  // Start with 1 since we are looking for the first column
		for (char ch : line) {
			if (ch == ',') {
				numColumns++;  // Count the commas
			}
		}

		// Dynamically allocate memory for columns
		columns = new char* [numColumns];  // Array of char pointers for each column

		// Split the line by commas and store each column in the columns array
		stringstream ss(line);
		string column;
		int columnIndex = 0;
		while (getline(ss, column, ',')) {
			columns[columnIndex] = new char[column.length() + 1];  // Allocate memory for the column string
			customStrCopy(columns[columnIndex], column.c_str());  // Copy the column string to the allocated memory
			cout << columnIndex + 1 << ". " << column << endl;
			columnIndex++;
		}

		file.close();
		return true;
	}
	else {
		cout << "Error: Could not read any data from the file.\n";
		return false;
	}
}

void freeColumns(char** columns, int numColumns) {
	for (int i = 0; i < numColumns; ++i) {
		delete[] columns[i];  // Free each column string
	}
	delete[] columns;  // Free the array of pointers
}

void logChange(const string& change) {
	const string untrackedFile = "gitLite/untracked.txt";
	ofstream file(untrackedFile, ios::app);
	if (file.is_open()) {
		file << change << endl;
		file.close();
	}
	else {
		cout << "Error: Unable to open " << untrackedFile << " for writing.\n";
	}
}

string getCurrentBranch() {
	const string indexFile = "gitLite/index.txt";
	ifstream index(indexFile);
	string currentBranch;
	if (index.is_open()) {
		getline(index, currentBranch);
		currentBranch = currentBranch.substr(currentBranch.find("=") + 1);
		index.close();
	}
	else {
		cout << "Error: Unable to read current branch from index.txt\n";
	}
	return currentBranch;
}

void clearUntrackedFile() {
	const string untrackedFile = "gitLite/untracked.txt";
	ofstream file(untrackedFile, ofstream::trunc);
	file.close();
}

void AddNode(AVLTree& avlTree) {
	// Take in the data input from the user
	string data;
	cout << "Enter the data you want to add: ";
	cin.ignore();  // To clear the newline character left by previous input
	getline(cin, data);

	// Add the node to the AVL tree
	avlTree.insert(data);

	// Log the change to untracked.txt
	logChange("Add=" + data);

	cout << "Data added successfully: " << data << endl;
}

void DeleteNode(AVLTree& avlTree) {
	//show user all the data of the tree with index
	cout << "Data in the tree: " << endl;
	avlTree.inorderFile();
	// Take in the key input from the user


	int key;
	cout << "Enter the number of the Data you want to delete: ";
	cin >> key;

	string data = avlTree.getDataByIndex(key);

	// Delete the node from the AVL tree
	avlTree.deleteKey(data);
	// Log the change to untracked.txt
	logChange("Delete=" + data);
	cout << "Key deleted successfully: " << key << endl;

}

void UpdateNode(AVLTree& avlTree) {
	// Take in key of old data and new data from the user
	cout << "Data in the tree: " << endl;
	avlTree.inorderFile();
	// Take in the key input from the user


	int key;
	cout << "Enter the number of the Data you want to delete: ";
	cin >> key;

	string oldData = avlTree.getDataByIndex(key);
	cout << "old data: " << oldData << endl;
	string newData;
	cout << "Enter the new data: ";
	cin.ignore();  // To clear the newline character left by previous input
	getline(cin, newData);

	avlTree.deleteKey(oldData);
	logChange("Delete=" + oldData);
	avlTree.insert(newData);
	logChange("Add=" + newData);
	cout << "Data updated successfully: " << oldData << " to " << newData << endl;

}

void mergeBranches(AVLTree& avlTree) {
	// Step 1: Take input for the branch name to merge
	string branchToMerge;
	cout << "Enter the name of the branch to merge: ";
	cin >> branchToMerge;

	const string gitLiteDir = "gitLite";
	const string branchesDir = gitLiteDir + "/branches";
	const string branchesFile = gitLiteDir + "/branches.txt";
	const string indexFile = gitLiteDir + "/index.txt";

	string currentBranch = getCurrentBranch();
	//compare the commits of the current branch with the branch to merge and from the point they diverge apply the changes to the current branch
	const string currentBranchDir = branchesDir + "/" + currentBranch;
	const string branchToMergeDir = branchesDir + "/" + branchToMerge;
	const string currentCommitsFile = currentBranchDir + "/commits.txt";
	const string branchToMergeCommitsFile = branchToMergeDir + "/commits.txt";

	// Read the commits list of the current branch
	ifstream currentCommits(currentCommitsFile);
	if (!currentCommits.is_open()) {
		cout << "Error: Unable to open commits.txt for current branch.\n";
		return;
	}

	vector<string> currentCommitFiles;
	string currentCommitFile;
	while (getline(currentCommits, currentCommitFile)) {
		currentCommitFiles.push_back(currentCommitFile);
	}
	currentCommits.close();

	// Read the commits list of the branch to merge
	ifstream branchToMergeCommits(branchToMergeCommitsFile);
	if (!branchToMergeCommits.is_open()) {
		cout << "Error: Unable to open commits.txt for branch to merge.\n";
		return;
	}

	vector<string> branchToMergeCommitFiles;
	string branchToMergeCommitFile;
	while (getline(branchToMergeCommits, branchToMergeCommitFile)) {
		branchToMergeCommitFiles.push_back(branchToMergeCommitFile);
	}
	branchToMergeCommits.close();

	// Find the common commit point
	int commonCommitIndex = 0;
	for (int i = 0; i < currentCommitFiles.size() && i < branchToMergeCommitFiles.size(); ++i) {
		if (currentCommitFiles[i] == branchToMergeCommitFiles[i]) {
			commonCommitIndex = i;
		}
		else {
			break;
		}
	}

	// Start from the common commit point and apply changes from the branch to merge
	for (int i = commonCommitIndex + 1; i < branchToMergeCommitFiles.size(); ++i) {
		string currentCommitFile = branchToMergeCommitFiles[i];
		string commitPath = branchToMergeDir + "/" + currentCommitFile;

		// Open the commit file
		ifstream commit(commitPath);
		if (!commit.is_open()) {
			cout << "Error: Unable to open commit file: " << currentCommitFile << "\n";
			continue;
		}

		// Apply the changes in this commit
		string line;
		while (getline(commit, line)) {
			if (line.find("Add=") == 0) {
				// Apply add operation
				string data = line.substr(4);  // Remove "Add=" prefix
				avlTree.insert(data);  // Assuming insert adds the data
			}
			else if (line.find("Delete=") == 0) {
				// Apply delete operation
				string data = line.substr(7);  // Remove "Delete=" prefix
				avlTree.deleteKey(data);  // Assuming deleteKey removes the data
			}
		}
		commit.close();

		// copy the commit file to the current branch
		fs::copy(commitPath, currentBranchDir + "/" + currentCommitFile, fs::copy_options::overwrite_existing);
		// Update the commits list of the current branch
		ofstream currentCommitsOut(currentCommitsFile, ios::app);
		currentCommitsOut << currentCommitFile << endl;
		currentCommitsOut.close();

		cout << "Changes from commit " << currentCommitFile << " applied to the current branch.\n";



	}
	cout << "Branches merged successfully.\n";

}

void setupGitLiteRepository() {
	// Create the gitLite directory
	const string gitLiteDir = "gitLite";
	if (!fs::exists(gitLiteDir)) {
		fs::create_directory(gitLiteDir);
		cout << "Created directory: " << gitLiteDir << endl;
	}
	else {
		// Clear the directory if it already exists
		fs::remove_all(gitLiteDir);
		fs::create_directory(gitLiteDir);
	}

	// Create the branches folder
	const string branchesDir = gitLiteDir + "/branches";
	if (!fs::exists(branchesDir)) {
		fs::create_directory(branchesDir);
		cout << "Created directory: " << branchesDir << endl;
	}

	// Create the branches.txt file
	const string branchesFile = gitLiteDir + "/branches.txt";
	if (!fs::exists(branchesFile)) {
		ofstream(branchesFile).close();
		cout << "Created file: " << branchesFile << endl;
	}

	// Create the index.txt file
	const string indexFile = gitLiteDir + "/index.txt";
	if (!fs::exists(indexFile)) {
		ofstream(indexFile).close();
		cout << "Created file: " << indexFile << endl;
	}

	// Create the untracked.txt file
	const string untrackedFile = gitLiteDir + "/untracked.txt";
	if (!fs::exists(untrackedFile)) {
		ofstream(untrackedFile).close();
		cout << "Created file: " << untrackedFile << endl;
	}
	//setup main branch add to branches.txt create folder for main branch and add current branch to index.txt
	ofstream branches(branchesFile, ios::app);
	branches << "main" << endl;
	branches.close();
	const string mainBranchDir = branchesDir + "/main";
	if (!fs::exists(mainBranchDir)) {
		fs::create_directory(mainBranchDir);
		cout << "Created directory: " << mainBranchDir << endl;
	}
	ofstream index(indexFile);
	index << "current_branch=main" << endl;
	index.close();
}

string lastCommitFile(const string& branchDir) {
	const string commitsFile = branchDir + "/commits.txt";
	ifstream commits(commitsFile);
	string lastCommitFile;
	string mostRecentCommit;
	while (getline(commits, mostRecentCommit)) {
		lastCommitFile = mostRecentCommit;
	}
	commits.close();
	return lastCommitFile;
}

void updateIndexFile(const string& branchName) {
	const string indexFile = "gitLite/index.txt";
	ofstream index(indexFile);
	if (index.is_open()) {
		index << "current_branch=" << branchName << endl;
		index.close();
	}
	else {
		cout << "Error: Unable to update index.txt\n";
	}
}

void commit(AVLTree& tree) {
	const string gitLiteDir = "gitLite";
	const string indexFile = gitLiteDir + "/index.txt";
	const string untrackedFile = gitLiteDir + "/untracked.txt";
	string commitMessage;
	cout << "Enter commit message: ";
	cin.ignore(); // To clear the newline character left by previous input
	getline(cin, commitMessage);

	string currentBranch = getCurrentBranch();

	const string branchDir = gitLiteDir + "/branches/" + currentBranch;
	const string commitsFile = branchDir + "/commits.txt";

	// Calculate root hash
	string rootHash = tree.rootMerkleHash();
	cout << "Root hash: " << rootHash << endl;

	// Find the most recent commit in commits.txt
	ifstream commits(commitsFile);
	string lastCommitFile;
	string mostRecentCommit;
	while (getline(commits, mostRecentCommit)) {
		lastCommitFile = mostRecentCommit;
	}
	commits.close();

	// Compare root hash with the most recent commit
	if (!lastCommitFile.empty()) {
		string lastCommitPath = branchDir + "/" + lastCommitFile;
		ifstream lastCommit(lastCommitPath);
		if (lastCommit.is_open()) {
			string lastRootHash;
			string line;
			while (getline(lastCommit, line)) {
				if (line.find("Hash=") == 0) {
					lastRootHash = line.substr(5); // Extract hash after "Hash="
					break;
				}
			}
			lastCommit.close();

			if (lastRootHash == rootHash) {
				cout << "No changes detected. Commit not created." << endl;
				return;
			}
		}
	}

	// Create new commit
	time_t now = time(0);
	string timestamp = to_string(now);
	string commithash = sha256(commitMessage + timestamp);
	string newCommitFile = "commit_" + commithash + ".txt";
	ofstream commitFile(branchDir + "/" + newCommitFile);

	if (commitFile.is_open()) {
		commitFile << "Hash=" << rootHash << endl; // Root hash
		commitFile << "Timestamp=" << timestamp << endl;
		commitFile << "Message=" << commitMessage << endl;

		// Add untracked changes
		ifstream untracked(untrackedFile);
		string change;
		while (getline(untracked, change)) {
			commitFile << change << endl;
		}
		untracked.close();
		commitFile.close();

		// Clear untracked.txt
		clearUntrackedFile();

		// Update commits.txt
		ofstream commitsOut(commitsFile, ios::app);
		commitsOut << newCommitFile << endl;
		commitsOut.close();

		cout << "Commit created: " << newCommitFile << endl;
	}
	else {
		cout << "Error: Unable to create commit file." << endl;
	}
}

void revertCommit(AVLTree& avlTree) {
	const string gitLiteDir = "gitLite";
	const string indexFile = gitLiteDir + "/index.txt";
	string currentBranch = getCurrentBranch();
	const string branchDir = gitLiteDir + "/branches/" + currentBranch;
	const string commitDir = branchDir + "/commits";
	string commitsFile = commitDir + "/commits.txt";

	// Get commit file name from user
	string commitToRevert;
	cout << "Enter the commit file name (without .txt): ";
	cin >> commitToRevert;

	// Read the commits list
	ifstream commits(commitsFile);
	if (!commits.is_open()) {
		cout << "Error: Unable to open commits.txt.\n";
		return;
	}

	vector<string> commitFiles;
	string commitFile;
	while (getline(commits, commitFile)) {
		commitFiles.push_back(commitFile);
	}
	commits.close();

	// Find the commit index to revert to
	auto it = find(commitFiles.begin(), commitFiles.end(), commitToRevert + ".txt");
	if (it == commitFiles.end()) {
		cout << "Error: Commit not found.\n";
		return;
	}

	int commitIndex = distance(commitFiles.begin(), it);

	// Start from the latest commit and work backwards
	for (int i = commitFiles.size() - 1; i > commitIndex; --i) {
		string currentCommitFile = commitFiles[i];
		string commitPath = commitDir + "/" + currentCommitFile;

		// Open the commit file
		ifstream commit(commitPath);
		if (!commit.is_open()) {
			cout << "Error: Unable to open commit file: " << currentCommitFile << "\n";
			continue;
		}

		// Reverse the changes in this commit
		string line;
		while (getline(commit, line)) {
			if (line.find("Add=") == 0) {
				// Undo add operation
				string data = line.substr(4);  // Remove "Add=" prefix
				avlTree.deleteKey(data);  // Assuming deleteKey removes the data
			}
			else if (line.find("Delete=") == 0) {
				// Undo delete operation
				string data = line.substr(7);  // Remove "Delete=" prefix
				avlTree.insert(data);  // Assuming insert adds the data
			}
		}
		commit.close();

		// Delete the commit file after revert
		remove(commitPath.c_str());

		// Remove from commits.txt
		ofstream commitList(commitsFile);
		for (int j = 0; j < commitFiles.size(); ++j) {
			if (j != i) {
				commitList << commitFiles[j] << endl;
			}
		}
		commitList.close();
	}

	cout << "Reverted all commits back to: " << commitToRevert << endl;
}

void revertCommit(AVLTree& avlTree, string commitToRevert) {
	const string gitLiteDir = "gitLite";
	const string indexFile = gitLiteDir + "/index.txt";
	string currentBranch = getCurrentBranch();
	const string branchDir = gitLiteDir + "/branches/" + currentBranch;
	string commitsFile = branchDir + "/commits.txt";

	// Read the commits list
	ifstream commits(commitsFile);
	if (!commits.is_open()) {
		cout << "Error: Unable to open commits.txt.\n";
		return;
	}

	vector<string> commitFiles;
	string commitFile;
	while (getline(commits, commitFile)) {
		commitFiles.push_back(commitFile);
	}
	commits.close();

	// Find the commit index to revert to
	auto it = find(commitFiles.begin(), commitFiles.end(), commitToRevert);
	if (it == commitFiles.end()) {
		cout << "Error: Commit not found.\n";
		return;
	}

	int commitIndex = distance(commitFiles.begin(), it);

	// Start from the latest commit and work backwards
	for (int i = commitFiles.size() - 1; i > commitIndex; --i) {
		string currentCommitFile = commitFiles[i];
		string commitPath = branchDir + "/" + currentCommitFile;

		// Open the commit file
		ifstream commit(commitPath);
		if (!commit.is_open()) {
			cout << "Error: Unable to open commit file: " << currentCommitFile << "\n";
			continue;
		}

		// Reverse the changes in this commit
		string line;
		while (getline(commit, line)) {
			if (line.find("Add=") == 0) {
				// Undo add operation
				string data = line.substr(4);  // Remove "Add=" prefix
				avlTree.deleteKey(data);  // Assuming deleteKey removes the data
			}
			else if (line.find("Delete=") == 0) {
				// Undo delete operation
				string data = line.substr(7);  // Remove "Delete=" prefix
				avlTree.insert(data);  // Assuming insert adds the data
			}
		}
		commit.close();
	}

	cout << "Reverted all commits back to: " << commitToRevert << endl;
}

void deleteBranch() {
	// Step 1: Take input for the branch name to delete
	string branchToDelete;
	cout << "Enter the name of the branch to delete: ";
	cin >> branchToDelete;

	const string gitLiteDir = "gitLite";
	const string branchesDir = gitLiteDir + "/branches";
	const string branchesFile = gitLiteDir + "/branches.txt";
	const string indexFile = gitLiteDir + "/index.txt";

	// Step 2: Check if the branch exists in branches.txt
	ifstream branches(branchesFile);
	if (!branches.is_open()) {
		cout << "Error: Unable to open branches.txt\n";
		return;
	}

	vector<string> branchesList;
	string line;
	bool branchFound = false;

	while (getline(branches, line)) {
		if (line == branchToDelete) {
			branchFound = true;
		}
		branchesList.push_back(line);
	}
	branches.close();

	if (!branchFound) {
		cout << "Error: Branch '" << branchToDelete << "' does not exist in branches.txt\n";
		return;
	}

	// Step 3: Delete the branch folder
	const string branchDir = branchesDir + "/" + branchToDelete;
	try {
		if (fs::exists(branchDir)) {
			fs::remove_all(branchDir);
			cout << "Branch folder '" << branchToDelete << "' deleted successfully.\n";
		}
		else {
			cout << "Error: Branch folder does not exist\n";
			return;
		}
	}
	catch (const fs::filesystem_error& e) {
		cout << "Error: Unable to delete the branch folder: " << e.what() << endl;
		return;
	}

	// Step 4: Remove the branch name from branches.txt
	ofstream branchesOut(branchesFile);
	if (!branchesOut.is_open()) {
		cout << "Error: Unable to open branches.txt for writing\n";
		return;
	}

	// Write back all branches except the deleted one
	for (const auto& branch : branchesList) {
		if (branch != branchToDelete) {
			branchesOut << branch << endl;
		}
	}
	branchesOut.close();
	cout << "Branch name '" << branchToDelete << "' removed from branches.txt\n";

	// Step 5: Update index.txt if the current branch is being deleted
	ifstream index(indexFile);
	if (!index.is_open()) {
		cout << "Error: Unable to open index.txt\n";
		return;
	}

	string currentBranch;
	getline(index, currentBranch);
	index.close();

	// If the branch to delete is the current branch, update index.txt
	if (currentBranch.find(branchToDelete) != string::npos) {
		cout << "The branch to delete is the current branch. Please set a new branch as the current one.\n";
		// You may prompt the user to set a new current branch or default to the first branch in branches.txt
		string newCurrentBranch;
		cout << "Enter the name of the new current branch: ";
		cin >> newCurrentBranch;

		// Check if the new branch exists
		bool newBranchExists = false;
		for (const auto& branch : branchesList) {
			if (branch == newCurrentBranch) {
				newBranchExists = true;
				break;
			}
		}

		if (!newBranchExists) {
			cout << "Error: The new branch does not exist. Please select a valid branch.\n";
			return;
		}

		// Update index.txt with the new current branch
		ofstream indexOut(indexFile);
		if (!indexOut.is_open()) {
			cout << "Error: Unable to update index.txt\n";
			return;
		}

		indexOut << "current_branch=" << newCurrentBranch << endl;
		indexOut.close();
		cout << "Current branch updated to: " << newCurrentBranch << endl;
	}
}

void goToCommit(AVLTree& avlTree) {
	const string gitLiteDir = "gitLite";
	const string indexFile = gitLiteDir + "/index.txt";
	string currentBranch = getCurrentBranch();
	const string branchDir = gitLiteDir + "/branches/" + currentBranch;
	string commitsFile = branchDir + "/commits.txt";

	// Get commit file name from user
	string commitToGoTo;
	cout << "Enter the commit file name (without .txt): ";
	cin >> commitToGoTo;

	// Read the commits list
	ifstream commits(commitsFile);
	if (!commits.is_open()) {
		cout << "Error: Unable to open commits.txt.\n";
		return;
	}

	vector<string> commitFiles;
	string commitFile;
	while (getline(commits, commitFile)) {
		commitFiles.push_back(commitFile);
	}
	commits.close();

	// Find the commit index to go to
	auto it = find(commitFiles.begin(), commitFiles.end(), commitToGoTo + ".txt");
	if (it == commitFiles.end()) {
		cout << "Error: Commit not found.\n";
		return;
	}

	int commitIndex = distance(commitFiles.begin(), it);

	// Start from the selected commit and apply all subsequent changes
	for (int i = commitIndex; i < commitFiles.size(); ++i) {
		string currentCommitFile = commitFiles[i];
		string commitPath = branchDir + "/" + currentCommitFile;

		// Open the commit file
		ifstream commit(commitPath);
		if (!commit.is_open()) {
			cout << "Error: Unable to open commit file: " << currentCommitFile << "\n";
			continue;
		}

		// Apply the changes in this commit
		string line;
		while (getline(commit, line)) {
			if (line.find("Add=") == 0) {
				// Apply add operation
				string data = line.substr(4);  // Remove "Add=" prefix
				avlTree.insert(data);  // Assuming insert adds the data
			}
			else if (line.find("Delete=") == 0) {
				// Apply delete operation
				string data = line.substr(7);  // Remove "Delete=" prefix
				avlTree.deleteKey(data);  // Assuming deleteKey removes the data
			}
		}
		commit.close();
	}

	cout << "Applied changes from commit " << commitToGoTo << " onward.\n";
}

void goToCommit(AVLTree& avlTree, string commitToGoTo) {
	const string gitLiteDir = "gitLite";
	const string indexFile = gitLiteDir + "/index.txt";
	string currentBranch = getCurrentBranch();
	const string branchDir = gitLiteDir + "/branches/" + currentBranch;
	string commitsFile = branchDir + "/commits.txt";



	// Read the commits list
	ifstream commits(commitsFile);
	if (!commits.is_open()) {
		cout << "Error: Unable to open commits.txt.\n";
		return;
	}

	vector<string> commitFiles;
	string commitFile;
	while (getline(commits, commitFile)) {
		commitFiles.push_back(commitFile);
	}
	commits.close();

	// Find the commit index to go to
	auto it = find(commitFiles.begin(), commitFiles.end(), commitToGoTo);
	if (it == commitFiles.end()) {
		cout << "Error: Commit not found.\n";
		return;
	}

	int commitIndex = distance(commitFiles.begin(), it);

	// Start from the selected commit and apply all subsequent changes
	for (int i = commitIndex; i < commitFiles.size(); ++i) {
		string currentCommitFile = commitFiles[i];
		string commitPath = branchDir + "/" + currentCommitFile;

		// Open the commit file
		ifstream commit(commitPath);
		if (!commit.is_open()) {
			cout << "Error: Unable to open commit file: " << currentCommitFile << "\n";
			continue;
		}

		// Apply the changes in this commit
		string line;
		while (getline(commit, line)) {
			if (line.find("Add=") == 0) {
				// Apply add operation
				string data = line.substr(4);  // Remove "Add=" prefix
				avlTree.insert(data);  // Assuming insert adds the data
			}
			else if (line.find("Delete=") == 0) {
				// Apply delete operation
				string data = line.substr(7);  // Remove "Delete=" prefix
				avlTree.deleteKey(data);  // Assuming deleteKey removes the data
			}
		}
		commit.close();
	}

	cout << "Applied changes from commit " << commitToGoTo << " onward.\n";
}

void viewCommitHistory() {
	const string gitLiteDir = "gitLite";
	const string indexFile = gitLiteDir + "/index.txt";
	string currentBranch = getCurrentBranch();
	const string branchDir = gitLiteDir + "/branches/" + currentBranch;
	string commitsFile = branchDir + "/commits.txt";

	// Read the commits list
	ifstream commits(commitsFile);
	if (!commits.is_open()) {
		cout << "Error: Unable to open commits.txt.\n";
		return;
	}

	cout << "Commit History for branch '" << currentBranch << "':\n";
	string commitFile;
	while (getline(commits, commitFile)) {
		// open the commit file and print the commit hash, timestamp, and message
		string commitPath = branchDir + "/" + commitFile;
		cout << "--------------------------------------------" << endl;
		cout << "Commit: " << commitFile.substr(0, commitFile.find(".txt")) << endl;
		ifstream commit(commitPath);
		if (commit.is_open()) {
			string line;
			while (getline(commit, line)) {
				if (line.find("Hash=") == 0) {
					cout << "Root Hash: " << line.substr(5) << endl;
				}
				else if (line.find("Timestamp=") == 0) {
					cout << "Timestamp: " << line.substr(10) << endl;
				}
				else if (line.find("Message=") == 0) {
					cout << "Message: " << line.substr(8) << endl;
				}
			}
			commit.close();
		}
		cout << endl;
		cout << "--------------------------------------------" << endl;
	}
	commits.close();
}

void checkoutBranch(AVLTree& tree) {
	//input branch name to switch to,
	//  get the current branch name,
	// revert all commits of current branch,
	// update index.txt,
	//  apply all commits of the branch being switched to
	const string gitLiteDir = "gitLite";


	string branchName;
	cout << "Enter the name of the branch to switch to: ";
	cin >> branchName;

	string currentBranch = getCurrentBranch();

	// get the first commit of the current branch
	string currentBranchDir = gitLiteDir + "/branches/" + currentBranch;
	string currentCommitsFile = currentBranchDir + "/commits.txt";
	string firstCommitFile;
	ifstream commits(currentCommitsFile);
	getline(commits, firstCommitFile);
	commits.close();
	cout << "First commit of current branch: " << firstCommitFile << endl;

	// revert all commits of the current branch
	revertCommit(tree, firstCommitFile);

	// update index.txt
	updateIndexFile(branchName);

	// apply all commits of the branch being switched to
	string branchDir = gitLiteDir + "/branches/" + branchName;
	string commitsFile = branchDir + "/commits.txt";
	ifstream commitsIn(commitsFile);
	string commitFile;
	getline(commitsIn, commitFile);
	commitsIn.close();
	goToCommit(tree, commitFile);

	cout << "Switched to branch: " << branchName << endl;


}

void createBranch() {
	// Step 1: Take input for the new branch name
	string newBranchName;
	cout << "Enter the name of the new branch: ";
	cin >> newBranchName;

	// Step 2: Read the current branch name from the index.txt file
	const string gitLiteDir = "gitLite";
	string currentBranch = getCurrentBranch();

	// Step 3: Get the last commit name (deflection point)
	// In a real-world scenario, you would use a version control system (e.g., Git) to get this info.
	// For now, let's mock this with "mock_commit" as the name of the last commit.
	string deflectionPoint = lastCommitFile(currentBranch);  // Replace with actual commit logic if needed.

	// Step 4: Copy the folder of the current branch to the new branch folder
	const string branchesDir = gitLiteDir + "/branches";
	const string currentBranchDir = branchesDir + "/" + currentBranch;
	const string newBranchDir = branchesDir + "/" + newBranchName;

	try {
		// Check if the new branch already exists
		if (fs::exists(newBranchDir)) {
			cout << "Error: Branch '" << newBranchName << "' already exists.\n";
			return;
		}

		// Create the new branch directory and copy files from the current branch
		fs::copy(currentBranchDir, newBranchDir, fs::copy_options::recursive);

		cout << "Branch '" << newBranchName << "' created successfully.\n";

		// Step 5: Add the new branch name to branches.txt
		ofstream branchesFile(gitLiteDir + "/branches.txt", ios::app);
		if (branchesFile.is_open()) {
			branchesFile << newBranchName << endl;
			branchesFile.close();
		}
		else {
			cout << "Error: Unable to update branches.txt\n";
			return;
		}

		// Step 6: Create the new index.txt for the new branch and write deflection point
		ofstream newBranchIndex(newBranchDir + "/index.txt");
		if (newBranchIndex.is_open()) {
			newBranchIndex << "deflection_point=" << deflectionPoint << endl;
			newBranchIndex.close();
			cout << "Created index.txt for new branch with deflection point: " << deflectionPoint << endl;
		}
		else {
			cout << "Error: Unable to create index.txt for new branch\n";
			return;
		}

		// Step 7: Update the current branch in index.txt to the new branch name
		updateIndexFile(newBranchName);

	}
	catch (const fs::filesystem_error& e) {
		cout << "Error: " << e.what() << endl;
	}
}

/***********************************************************
					MAIN FUNCTION
 ***********************************************************/
int main() {
	string csvFile, column, treeType;
	char** columns = nullptr;  // Pointer for storing column names
	int choice, numColumns;

	// Step 1: Initialization
	displayInitializationHeader();
	cout << "Enter the CSV file name: ";
	cin >> csvFile;

	// Read the CSV file and display columns
	if (!readCSVFileAndDisplayColumns(csvFile, columns, numColumns)) {
		return 1; // Exit if CSV cannot be read
	}

	// Ask user to select a column
	cout << "\nSelect a column to use as the tree key (enter number): ";
	int columnChoice;
	cin >> columnChoice;

	if (columnChoice < 1 || columnChoice > numColumns) {
		cout << "\nInvalid column choice. Exiting...\n";
		freeColumns(columns, numColumns);  // Free the memory before exiting
		return 1;
	}
	column = columns[columnChoice - 1];  // Set the selected column

	cout << "\nChoose tree type:\n";
	cout << "1. AVL\n2. B\n3. Red-Black\n";
	cout << "Select tree type (enter number): ";
	cin >> treeType;

	cout << "\nRepository initialized successfully with tree type " << treeType << " using column " << column << ".\n";

	// Initialize the AVL tree
	AVLTree avlTree;

	// Open the CSV file and start reading rows
	ifstream file(csvFile);
	if (!file.is_open()) {
		cout << "Error: Unable to open CSV file.\n";
		freeColumns(columns, numColumns);
		return 1;
	}

	string header;
	getline(file, header);  // Skip the header row
	string row;
	int a, b, c;
	a = columnChoice - 1;
	b = columnChoice % numColumns;
	c = (columnChoice + 1) % numColumns;
	avlTree.setSelectColumns(a, b, c);


	while (getline(file, row)) {

		avlTree.insert(row);

	}
	avlTree.inorderFile();

	file.close();  // Close the file
	setupGitLiteRepository();


	// Step 2: Menu Loop
	do {
		cin.ignore();
		displayMenu();
		cin >> choice;

		switch (choice) {
		case 1:
			commit(avlTree);
			break;
		case 2:
			createBranch();
			break;
		case 3:
			checkoutBranch(avlTree);
			break;
		case 4:
			viewCommitHistory();
			break;
		case 5:
			displayTreeStructure(avlTree);
			break;
		case 6:
			mergeBranches(avlTree);
			break;
		case 7:
			deleteBranch();
			break;
		case 8:
			revertCommit(avlTree);
			break;
		case 9:
			AddNode(avlTree);
			break;
		case 10:
			UpdateNode(avlTree);
			break;
		case 11:
			DeleteNode(avlTree);
			break;
		case 12:
			cout << "\nExiting GitLite. Goodbye!\n";
			break;
		default:
			cout << "\nInvalid choice. Please try again.\n";
		}
	} while (choice != 12);

	// Free the dynamically allocated memory before exiting
	freeColumns(columns, numColumns);

	return 0;
}






