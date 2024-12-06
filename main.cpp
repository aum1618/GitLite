#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Hashes.h"
#include "AVL.h"
#include "BTree.h"
#include "RBTree.h"
#include "menu.h"
#include <vector>

using namespace std;

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
	int rowIndex = 0;

	while (getline(file, row)) {
		stringstream ss(row);
		string columnValue;
		vector<string> rowData;

		// Split the row into columns
		string cell;
		while (getline(ss, cell, ',')) {
			rowData.push_back(cell);
		}

		// Get the value of the selected column
		if (columnChoice - 1 >= 0 && columnChoice - 1 < static_cast<int>(rowData.size())) {
			columnValue = rowData[columnChoice - 1];
		}
		else {
			cout << "Error: Missing value in selected column for row " << rowIndex << ". Skipping.\n";
			rowIndex++;
			continue;
		}

		// Generate hash for the key
		string keyData = columnValue + "_" + to_string(rowIndex);
		cout << "Key: " << keyData << endl;
		string hashKey = sha256(keyData);

		cout << "Hash: " << hashKey << endl;
		cout << "Row: " << row << endl;
		cout << "----------------------------------\n";
		avlTree.insert(row, hashKey);

		rowIndex++;
	}

	file.close();  // Close the file

	// Step 2: Menu Loop
	do {
		cin.ignore();
		displayMenu();
		cin >> choice;

		switch (choice) {
		case 1:
			commit();
			break;
		case 2:
			createBranch();
			break;
		case 3:
			switchBranch();
			break;
		case 4:
			viewCommitHistory();
			break;
		case 5:
			displayTreeStructure();
			break;
		case 6:
			mergeBranches();
			break;
		case 7:
			deleteBranch();
			break;
		case 8:
			saveRepository();
			break;
		case 9:
			loadRepository();
			break;
		case 10:
			detectCorruption();
			break;
		case 11:
			cout << "\nExiting GitLite. Goodbye!\n";
			break;
		default:
			cout << "\nInvalid choice. Please try again.\n";
		}
	} while (choice != 11);

	// Free the dynamically allocated memory before exiting
	freeColumns(columns, numColumns);

	return 0;
}