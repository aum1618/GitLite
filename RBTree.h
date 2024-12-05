#pragma once
#include <iostream>
//#include "FileHandling.h"

using namespace std;

template <typename T> class RedBlackTree {
public:
    string rootFile;

    void updateNodeData(string filename, T data, string leftFile, string rightFile,bool isRed) {
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
            //updateNodeData(h, data, leftFile, rightFile, isRedVal);
        }
        else if (key > data) {
            cout << "Going right from " << h << endl;
            rightFile = insertHelper(rightFile, key);
            //updateNodeData(h, data, leftFile, rightFile, isRedVal);
           
        }
        else return h;
        
        ifstream newhFile(h);
        string newline;
        bool newisRedVal;
        while (getline(newhFile, newline)) {
            
            if (newline.find("isRed=") == 0) newisRedVal = (newline.substr(6) == "1");
        }
        newhFile.close();
        updateNodeData(h, data, leftFile, rightFile, newisRedVal);

        // Fix Red-Black tree violations
       
        if (isRed(h) && isRed(leftFile))
        {
            if (isRed(findSibling(h)))
            {
                updateColor(findSibling(h), false);
                updateColor(h,  false);
                updateColor(findParent(h), true);
            }
            else
            {
                h = rotateRight(findParent(h));
                
                updateColor(h, false);
                updateColor(getRightFile(h), true);
            }
        }
        if (isRed(rightFile) && isRed(h))
        {
            if (isRed(findSibling(h)))
            {
                updateColor(findSibling(h), false);
                updateColor(h, false);
                updateColor(findParent(h), true);
            }
            else
            {
                h = rotateLeft(findParent(h));
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


