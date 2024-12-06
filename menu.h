#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include<cstring>
#include <iomanip> // For text alignment

using namespace std;

// Function to create a commit
void commit() {
    string commitMsg;
    cout << "\nEnter commit message: ";
    cin.ignore();  // To clear the newline character left by previous input
    getline(cin, commitMsg);
    cout << "\nCommit successful with message: \"" << commitMsg << "\"\n";
}

// Function to create a branch
void createBranch() {
    string branchName;
    cout << "\nEnter new branch name: ";
    cin >> branchName;
    cout << "\nBranch '" << branchName << "' created successfully.\n";
}

// Function to switch branches
void switchBranch() {
    string branchName;
    cout << "\nEnter branch name to switch to: ";
    cin >> branchName;
    cout << "\nSwitched to branch '" << branchName << "'.\n";
}

// Function to view commit history
void viewCommitHistory() {
    cout << "\nViewing commit history...\n";
    // Code to show commit history
}

// Function to display the tree structure
void displayTreeStructure() {
    cout << "\nDisplaying tree structure...\n";
    // Code to visualize the tree structure
}

// Function to merge branches
void mergeBranches() {
    string sourceBranch, targetBranch;
    cout << "\nEnter source branch to merge: ";
    cin >> sourceBranch;
    cout << "Enter target branch to merge into: ";
    cin >> targetBranch;
    cout << "\nMerging '" << sourceBranch << "' into '" << targetBranch << "'.\n";
    // Code to handle merging branches
}

// Function to delete a branch
void deleteBranch() {
    string branchName;
    cout << "\nEnter branch name to delete: ";
    cin >> branchName;
    cout << "\nBranch '" << branchName << "' deleted successfully.\n";
}

// Function to save the repository
void saveRepository() {
    cout << "\nRepository saved successfully.\n";
    // Code to save the repository
}

// Function to load the repository
void loadRepository() {
    cout << "\nRepository loaded successfully.\n";
    // Code to load the repository
}

// Function to detect data corruption
void detectCorruption() {
    cout << "\nDetecting data corruption...\n";
    // Code to detect data corruption
}

// Function to display the menu in a beautiful format
void displayMenu() {
    // Clear screen for a fresh look (works for most terminals)
    //cout << "\033[2J\033[H"; // ANSI escape sequence to clear screen

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
    cout << "8. Save Repository\n";
    cout << "9. Load Repository\n";
    cout << "10. Detect Data Corruption\n";
    cout << "11. Exit\n";
    cout << "------------------------------------\n";
    cout << "Enter your choice: ";
}

// Function to display the header for the initialization
void displayInitializationHeader() {
    cout << "\033[1;34m";
    cout << "====================================\n";
    cout << "      GitLite Initialization         \n";
    cout << "====================================\n";
    cout << "\033[0m";
}
void customStrCopy(char*& dest, const std::string& src) {
    // Allocate memory for the destination string
    dest = new char[src.length() + 1];
    // Copy each character from the source string to the destination
    for (size_t i = 0; i < src.length(); i++) {
        dest[i] = src[i];
    }
    // Add the null terminator at the end
    dest[src.length()] = '\0';
}
// Function to read a CSV file and display column names using char pointers
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


// Function to free dynamically allocated memory for columns
void freeColumns(char** columns, int numColumns) {
    for (int i = 0; i < numColumns; ++i) {
        delete[] columns[i];  // Free each column string
    }
    delete[] columns;  // Free the array of pointers
}


