#include <iostream>
#include <fstream>
#include <string>

using namespace std;

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