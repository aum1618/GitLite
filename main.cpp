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
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem> // For filesystem operations

using namespace std;
namespace fs = std::experimental::filesystem;
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

void AddNode(AVLTree& avlTree) {
	// Take in the data input from the user
	string data;
	cout << "Enter the data you want to add: ";
	cin >> data;

	// Add the node to the AVL tree
	//avlTree.insert(data, sha256(data));

	// Log the change to untracked.txt
	logChange("Add=" + data);

	cout << "Data added successfully: " << data << endl;
}

void DeleteNode(AVLTree& avlTree) {
	// Take key input from the user
	string key;
	cout << "Enter the key you want to delete: ";
	cin >> key;

	// Delete the node from the AVL tree
	//avlTree.deleteKey(key);
	if (true) {
		// Log the change to untracked.txt
		logChange("Delete=" + key);
		cout << "Key deleted successfully: " << key << endl;
	}
	else {
		cout << "Error: Key not found in the tree." << endl;
	}
}

void UpdateNode(AVLTree& avlTree) {
	// Take in key of old data and new data from the user
	string oldData, newData;
	cout << "Enter the old data you want to update: ";
	cin >> oldData;
	cout << "Enter the new data: ";
	cin >> newData;

	// Update the node in the AVL tree
	//avlTree.deleteKey(oldData);
	if (true) {
		//avlTree.insert(newData, sha256(newData));

		// Log the changes to untracked.txt
		logChange("Delete=" + oldData);
		logChange("Add=" + newData);

		cout << "Data updated successfully: " << oldData << " to " << newData << endl;
	}
	else {
		cout << "Error: Old data not found in the tree." << endl;
	}
}


void setupGitLiteRepository() {
	// Create the gitLite directory
	const string gitLiteDir = "gitLite";
	if (!fs::exists(gitLiteDir)) {
		fs::create_directory(gitLiteDir);
		cout << "Created directory: " << gitLiteDir << endl;
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
string calculateRootHash(AVLTree& tree) {
	return tree.postOrderTraversalHash(); // Assume AVLTree has this implemented
}

void commit(AVLTree& tree) {
	const string gitLiteDir = "gitLite";
	const string indexFile = gitLiteDir + "/index.txt";
	const string untrackedFile = gitLiteDir + "/untracked.txt";

	// Get current branch from index.txt
	ifstream index(indexFile);
	string currentBranch;
	if (index.is_open()) {
		getline(index, currentBranch);
		currentBranch = currentBranch.substr(currentBranch.find("=") + 1);
		index.close();
	}
	else {
		cout << "Error: Unable to read current branch from index.txt\n";
		return;
	}

	const string branchDir = gitLiteDir + "/branches/" + currentBranch;
	const string commitsFile = branchDir + "/commits.txt";

	// Calculate root hash
	string rootHash = calculateRootHash(tree);

	// Check if commits exist
	ifstream commits(commitsFile);
	string lastCommitFile;
	if (commits.is_open() && getline(commits, lastCommitFile, '\n')) {
		// Read last commit file to compare root hashes
		string lastCommitPath = branchDir + "/" + lastCommitFile;
		ifstream lastCommit(lastCommitPath);
		if (lastCommit.is_open()) {
			string lastRootHash;
			getline(lastCommit, lastRootHash);
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
	string newCommitFile = "commit_" + timestamp + ".txt";
	ofstream commitFile(branchDir + "/" + newCommitFile);

	if (commitFile.is_open()) {
		commitFile << rootHash << endl; // Root hash
		commitFile << "Timestamp: " << timestamp << endl;

		// Add untracked changes
		ifstream untracked(untrackedFile);
		string change;
		while (getline(untracked, change)) {
			commitFile << change << endl;
		}
		untracked.close();
		commitFile.close();

		// Clear untracked.txt
		ofstream clearUntracked(untrackedFile, ofstream::trunc);
		clearUntracked.close();

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
	const string commitsFile = gitLiteDir + "/branches/main/commits.txt";  // Assume main branch

	// Get current branch from index.txt
	ifstream index(indexFile);
	string currentBranch;
	if (index.is_open()) {
		getline(index, currentBranch);
		currentBranch = currentBranch.substr(currentBranch.find("=") + 1);
		index.close();
	}
	else {
		cout << "Error: Unable to read current branch from index.txt\n";
		return;
	}

	const string branchDir = gitLiteDir + "/branches/" + currentBranch;
	const string commitDir = branchDir + "/commits";

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
				logChange("Delete=" + data);
			}
			else if (line.find("Delete=") == 0) {
				// Undo delete operation
				string data = line.substr(7);  // Remove "Delete=" prefix
				avlTree.insert(data, sha256(data));  // Assuming insert adds the data
				logChange("Add=" + data);
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

void deleteBranch() {
	// Step 1: Take input for the branch name to delete
	std::string branchToDelete;
	std::cout << "Enter the name of the branch to delete: ";
	std::cin >> branchToDelete;

	const std::string gitLiteDir = "gitLite";
	const std::string branchesDir = gitLiteDir + "/branches";
	const std::string branchesFile = gitLiteDir + "/branches.txt";
	const std::string indexFile = gitLiteDir + "/index.txt";

	// Step 2: Check if the branch exists in branches.txt
	std::ifstream branches(branchesFile);
	if (!branches.is_open()) {
		std::cout << "Error: Unable to open branches.txt\n";
		return;
	}

	std::vector<std::string> branchesList;
	std::string line;
	bool branchFound = false;

	while (std::getline(branches, line)) {
		if (line == branchToDelete) {
			branchFound = true;
		}
		branchesList.push_back(line);
	}
	branches.close();

	if (!branchFound) {
		std::cout << "Error: Branch '" << branchToDelete << "' does not exist in branches.txt\n";
		return;
	}

	// Step 3: Delete the branch folder
	const std::string branchDir = branchesDir + "/" + branchToDelete;
	try {
		if (fs::exists(branchDir)) {
			fs::remove_all(branchDir);
			std::cout << "Branch folder '" << branchToDelete << "' deleted successfully.\n";
		}
		else {
			std::cout << "Error: Branch folder does not exist\n";
			return;
		}
	}
	catch (const fs::filesystem_error& e) {
		std::cout << "Error: Unable to delete the branch folder: " << e.what() << std::endl;
		return;
	}

	// Step 4: Remove the branch name from branches.txt
	std::ofstream branchesOut(branchesFile);
	if (!branchesOut.is_open()) {
		std::cout << "Error: Unable to open branches.txt for writing\n";
		return;
	}

	// Write back all branches except the deleted one
	for (const auto& branch : branchesList) {
		if (branch != branchToDelete) {
			branchesOut << branch << std::endl;
		}
	}
	branchesOut.close();
	std::cout << "Branch name '" << branchToDelete << "' removed from branches.txt\n";

	// Step 5: Update index.txt if the current branch is being deleted
	std::ifstream index(indexFile);
	if (!index.is_open()) {
		std::cout << "Error: Unable to open index.txt\n";
		return;
	}

	std::string currentBranch;
	std::getline(index, currentBranch);
	index.close();

	// If the branch to delete is the current branch, update index.txt
	if (currentBranch.find(branchToDelete) != std::string::npos) {
		std::cout << "The branch to delete is the current branch. Please set a new branch as the current one.\n";
		// You may prompt the user to set a new current branch or default to the first branch in branches.txt
		std::string newCurrentBranch;
		std::cout << "Enter the name of the new current branch: ";
		std::cin >> newCurrentBranch;

		// Check if the new branch exists
		bool newBranchExists = false;
		for (const auto& branch : branchesList) {
			if (branch == newCurrentBranch) {
				newBranchExists = true;
				break;
			}
		}

		if (!newBranchExists) {
			std::cout << "Error: The new branch does not exist. Please select a valid branch.\n";
			return;
		}

		// Update index.txt with the new current branch
		std::ofstream indexOut(indexFile);
		if (!indexOut.is_open()) {
			std::cout << "Error: Unable to update index.txt\n";
			return;
		}

		indexOut << "current_branch=" << newCurrentBranch << std::endl;
		indexOut.close();
		std::cout << "Current branch updated to: " << newCurrentBranch << std::endl;
	}
}

void goToCommit(AVLTree& avlTree) {
	const string gitLiteDir = "gitLite";
	const string indexFile = gitLiteDir + "/index.txt";
	const string commitsFile = gitLiteDir + "/branches/main/commits.txt";  // Assume main branch

	// Get current branch from index.txt
	ifstream index(indexFile);
	string currentBranch;
	if (index.is_open()) {
		getline(index, currentBranch);
		currentBranch = currentBranch.substr(currentBranch.find("=") + 1);
		index.close();
	}
	else {
		cout << "Error: Unable to read current branch from index.txt\n";
		return;
	}

	const string branchDir = gitLiteDir + "/branches/" + currentBranch;
	const string commitDir = branchDir + "/commits";

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
		string commitPath = commitDir + "/" + currentCommitFile;

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
				avlTree.insert(data, sha256(data));  // Assuming insert adds the data
				logChange("Add=" + data);
			}
			else if (line.find("Delete=") == 0) {
				// Apply delete operation
				string data = line.substr(7);  // Remove "Delete=" prefix
				avlTree.deleteKey(data);  // Assuming deleteKey removes the data
				logChange("Delete=" + data);
			}
		}
		commit.close();
	}

	cout << "Applied changes from commit " << commitToGoTo << " onward.\n";
}

void createBranch() {
	// Step 1: Take input for the new branch name
	std::string newBranchName;
	std::cout << "Enter the name of the new branch: ";
	std::cin >> newBranchName;

	// Step 2: Read the current branch name from the index.txt file
	const std::string gitLiteDir = "gitLite";
	const std::string indexFile = gitLiteDir + "/index.txt";
	std::string currentBranch;

	std::ifstream index(indexFile);
	if (index.is_open()) {
		std::string line;
		std::getline(index, line);  // Read the first line which contains the current branch info
		currentBranch = line.substr(line.find("=") + 1);
		index.close();
	}
	else {
		std::cout << "Error: Unable to read current branch from index.txt\n";
		return;
	}

	// Step 3: Get the last commit name (deflection point)
	// In a real-world scenario, you would use a version control system (e.g., Git) to get this info.
	// For now, let's mock this with "mock_commit" as the name of the last commit.
	std::string deflectionPoint = "mock_commit";  // Replace with actual commit logic if needed.

	// Step 4: Copy the folder of the current branch to the new branch folder
	const std::string branchesDir = gitLiteDir + "/branches";
	const std::string currentBranchDir = branchesDir + "/" + currentBranch;
	const std::string newBranchDir = branchesDir + "/" + newBranchName;

	try {
		// Check if the new branch already exists
		if (fs::exists(newBranchDir)) {
			std::cout << "Error: Branch '" << newBranchName << "' already exists.\n";
			return;
		}

		// Create the new branch directory and copy files from the current branch
		fs::copy(currentBranchDir, newBranchDir, fs::copy_options::recursive);

		std::cout << "Branch '" << newBranchName << "' created successfully.\n";

		// Step 5: Add the new branch name to branches.txt
		std::ofstream branchesFile(gitLiteDir + "/branches.txt", std::ios::app);
		if (branchesFile.is_open()) {
			branchesFile << newBranchName << std::endl;
			branchesFile.close();
		}
		else {
			std::cout << "Error: Unable to update branches.txt\n";
			return;
		}

		// Step 6: Create the new index.txt for the new branch and write deflection point
		std::ofstream newBranchIndex(newBranchDir + "/index.txt");
		if (newBranchIndex.is_open()) {
			newBranchIndex << "deflection_point=" << deflectionPoint << std::endl;
			newBranchIndex.close();
			std::cout << "Created index.txt for new branch with deflection point: " << deflectionPoint << std::endl;
		}
		else {
			std::cout << "Error: Unable to create index.txt for new branch\n";
			return;
		}

		// Step 7: Update the current branch in index.txt to the new branch name
		std::ofstream indexOut(indexFile);
		if (indexOut.is_open()) {
			indexOut << "current_branch=" << newBranchName << std::endl;
			indexOut.close();
			std::cout << "Current branch updated to: " << newBranchName << std::endl;
		}
		else {
			std::cout << "Error: Unable to update index.txt\n";
		}

	}
	catch (const fs::filesystem_error& e) {
		std::cout << "Error: " << e.what() << std::endl;
	}
}

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
		stringstream ss(row);
		string cell;
		vector<string> rowData;

		// Split the row into columns
		while (getline(ss, cell, ',')) {
			rowData.push_back(cell);
		}

		// Determine the selected, previous, and next columns
		string selectedValue, previousValue, nextValue;


		// Get the selected value
		selectedValue = rowData[a];
		previousValue = rowData[b];
		nextValue = rowData[c];

		// Combine the three values into keyData
		string keyData = previousValue + "_" + selectedValue + "_" + nextValue;
		cout << "Key: " << keyData << endl;

		// Generate hash for the key
		string hashKey = sha256(keyData);

		// Debug output
		cout << "Hash: " << hashKey << endl;
		cout << "Row: " << row << endl;
		cout << "----------------------------------\n";

		// Optionally insert into the AVL tree
		avlTree.insert(row, hashKey);

	}

	file.close();  // Close the file
	setupGitLiteRepository();


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
			AddNode(avlTree);
			break;
		case 12:
			UpdateNode(avlTree);
			break;
		case 13:
			DeleteNode(avlTree);
			break;
		case 14:
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
