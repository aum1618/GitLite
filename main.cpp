#include <iostream>
using namespace std;
#include "Hashes.h"
#include "AVL.h"
#include "BTree.h"
#include "RBTree.h"
#include"string";
#include"menu.h"

int main()
{

	//string csvFile, column, treeType;
	//char** columns = nullptr;  // Pointer for storing column names
	//int choice, numColumns;

	//// Step 1: Initialization
	//displayInitializationHeader();
	//cout << "Enter the CSV file name: ";
	//cin >> csvFile;

	//// Read the CSV file and display columns
	//if (!readCSVFileAndDisplayColumns(csvFile, columns, numColumns)) {
	//	return 1; // Exit if CSV cannot be read
	//}

	//// Ask user to select a column
	//cout << "\nSelect a column to use as the tree key (enter number): ";
	//int columnChoice;
	//cin >> columnChoice;

	//if (columnChoice < 1 || columnChoice > numColumns) {
	//	cout << "\nInvalid column choice. Exiting...\n";
	//	freeColumns(columns, numColumns);  // Free the memory before exiting
	//	return 1;
	//}
	//column = columns[columnChoice - 1];  // Set the selected column

	//cout << "\nChoose tree type:\n";
	//cout << "1. AVL\n2. B\n3. Red-Black\n";
	//cout << "Select tree type (enter number): ";
	//cin >> treeType;

	//cout << "\nRepository initialized successfully with tree type " << treeType << " using column " << column << ".\n";

	//// Step 2: Menu Loop
	//do {
	//	cin.ignore();
	//	displayMenu();
	//	cin >> choice;

	//	switch (choice) {
	//	case 1:
	//		commit();
	//		break;

	//	case 2:
	//		createBranch();
	//		break;

	//	case 3:
	//		switchBranch();
	//		break;

	//	case 4:
	//		viewCommitHistory();
	//		break;

	//	case 5:
	//		displayTreeStructure();
	//		break;

	//	case 6:
	//		mergeBranches();
	//		break;

	//	case 7:
	//		deleteBranch();
	//		break;

	//	case 8:
	//		saveRepository();
	//		break;

	//	case 9:
	//		loadRepository();
	//		break;

	//	case 10:
	//		detectCorruption();
	//		break;

	//	case 11:
	//		cout << "\nExiting GitLite. Goodbye!\n";
	//		break;

	//	default:
	//		cout << "\nInvalid choice. Please try again.\n";
	//	}
	//} while (choice != 11);

	//// Free the dynamically allocated memory before exiting
	//freeColumns(columns, numColumns);
	/*AVLTree<int> tree;

	tree.insert(40);
	tree.insert(59);
	tree.insert(19);
	tree.insert(30);
	tree.insert(10);
	tree.insert(27);
	tree.insert(20);
	tree.insert(25);
	tree.insert(31);
	tree.insert(34);
	tree.insert(11);

	cout << "In-order traversal: ";
	tree.inorderFile();

	cout << "Search for 20: " << (tree.searchFile(20) ? "Found" : "Not Found") << endl;
	cout << "Search for 50: " << (tree.searchFile(50) ? "Found" : "Not Found") << endl;*/
    RedBlackTree<int> rb;
	rb.insert(40);
	rb.insert(59);
	rb.insert(19);
	rb.insert(30);
	rb.insert(10);
	rb.insert(27);
	rb.insert(20);
	//rb.insert(25);
	//rb.insert(31);
	//rb.insert(34);
	//rb.insert(11);
	rb.inorder();
	return 0;
}