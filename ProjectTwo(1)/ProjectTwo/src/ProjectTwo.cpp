//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Stephen Cessna
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================


// Structure to hold course information
struct Course {
	string courseNum;
	string courseName;
	string preReq;
	string preReqT;
	vector<string> preReqs;
	Course(){

	}
};

// Structure to create a search tree
struct Node {
	Course course;
	Node *left;
	Node *right;

	Node(){
		left = nullptr;
		right = nullptr;
	}

	Node(Course courses) : Node() {
		this->course = courses;
	}
};

//============================================================================
// Binary Search Tree class definition
//============================================================================


class CourseTree {

private:
	Node* root;

	void addNode(Node* node, Course course);
	void printSchedule(Node* node);
	void printCourseInfo(Node* node, string courseNum);

public:
	CourseTree();
	virtual ~CourseTree();
	void Delete(Node* node);
	void Insert(Course course);
	int NumPrerequisites(Course course);
	void PrintSchedule();
	void PrintCourseInfo(string courseNum);
};

// Default constructor
CourseTree::CourseTree(){
	root=nullptr;
}

// Detructor
CourseTree::~CourseTree(){
	Delete(root);
}

//Deletes node
void CourseTree::Delete(Node* node){
	if(node){
		Delete(node->left);
		Delete(node->right);
		delete node;
	}
}

// Insert a course
void CourseTree::Insert(Course course){
	if (root == nullptr){
		root = new Node(course);
	} else {
		this->addNode(root, course);
	}
}
// Determines how many prerequisites are in each course
int CourseTree::NumPrerequisites(Course course){
	int count = 0;
	for(unsigned int i = 0; i < course.preReqs.size(); i++){
		if (course.preReqs.at(i).length()>0)
		count++;
	}
	return count;
}

void CourseTree::PrintSchedule(){
	this->printSchedule(root);
}

void CourseTree::PrintCourseInfo(string courseNum){
	this->printCourseInfo(root, courseNum);
}

// Adds nodes to sort the binary search tree
void CourseTree::addNode(Node* node, Course course){
	if(node->course.courseNum.compare(course.courseNum) > 0){
		if (node->left == nullptr){
			node->left = new Node(course);
		}else{
			this->addNode(node->left, course);
		}
	}else{
		if(node->right == nullptr){
			node->right= new Node(course);
		}else{
			this->addNode(node->right, course);
		}
	}
}

// Displays the whole schedule and it's prerequisites
void CourseTree::printSchedule(Node* node){
	if (node != nullptr){
		printSchedule(node->left);
		cout<< node->course.courseNum << ", " << node->course.courseName << endl;
		printSchedule(node->right);
	}
	return;
}

// Displays a certain course inputed by used and its prerequisites
void CourseTree::printCourseInfo(Node* curr, string courseNum){
	while(curr != nullptr){
		if(curr->course.courseNum.compare(courseNum) == 0){
			cout << endl << curr->course.courseNum << ", " << curr->course.courseName << endl;
			unsigned int size = NumPrerequisites(curr->course);
			cout << "Prerequisites: ";
			unsigned int i = 0;
			for (i = 0; i < size; i++) {
				cout << curr->course.preReqs.at(i);
			if (i != size - 1){
				cout << ", ";
			}
			}
			if(i == 0){
				cout << "No prerequisites required.";
			cout << endl;
			return;
			}
		}else if (courseNum.compare(curr->course.courseNum) < 0){
			curr = curr->left;
		}else{
			curr = curr->right;
		}
	}
	cout << "Course " << courseNum << " not found." << endl;
}

//============================================================================
// Static methods used for testing
//============================================================================

/**void loadCourses(string csvPath, CourseTree* ct){
	cout<< "Loading CSV file " << csvPath << endl;

	csv::Parser file = csv::Parser(csvPath);
}*/

void loadCourses(string csvPath, CourseTree* ct) {

// Open courseInfo file
try {
	ifstream courseList(csvPath);
	if (courseList.is_open()) {
		while (!courseList.eof()) {
			//cout << "Open" << endl;
			// A vector will hold each of the courses information
			vector<string> courseInfo;
			string courseData;
			getline(courseList, courseData);
			while (courseData.length() > 0) {
				unsigned int comma = courseData.find(',');
				if (comma < 100) {
					//cout << "Open" << endl;

					courseInfo.push_back(courseData.substr(0, comma));
					courseData.erase(0, comma + 1);
				}
				// Add the last course after the final comma
				else {
					courseInfo.push_back(courseData.substr(0, courseData.length()));
					courseData = " ";
				}
			}

			//insert info/prerequisites into Binary Search Tree
			Course course;
			course.courseNum = courseInfo[0];
			course.courseName = courseInfo[1];
			for (unsigned int i = 2; i < courseInfo.size(); i++) {
				course.preReqs.push_back(courseInfo[i]);
			}
			ct->Insert(course);
		}
		courseList.close();

	}
}
	catch (csv::Error& e) {
		cerr << e.what() << endl;
		//cout << "error" << endl;
	}


}

int main(int argc, char* argv[]){

	string csvPath, courseId;
	switch(argc) {
	case 2:
		csvPath = argv[1];
		break;
	case 3:
		csvPath = argv[1];
		courseId = argv[2];
		break;
	default:
		csvPath = "CourseInfo.csv";

	}


// Menu
CourseTree* ct;
ct = new CourseTree();
Course course;
cout << "\nWelcome to the course planner.\n" << endl;


int userChoice = 0;

while(userChoice != 9){

	cout << "   1. Load Data Structure." << endl;
	cout << "   2. Print Course List." << endl;
	cout << "   3. Print Course." << endl;
	cout << "   9. Exit." << endl;
	cout << "\nWhat would you like to do? ";
	cin >> userChoice;

	switch (userChoice){

	case 1:
	// Determine if the file was opened- load successful
		loadCourses(csvPath, ct);
		cout << "Courses have been loaded.\n" << endl;
		break;

	//Outputs a sample schedule
	case 2:
		if (ct != nullptr) {
			cout << "Here is a sample schedule:\n" << endl;
			ct->PrintSchedule();
			cout << endl;
		}else{
			cout << "Load courses first - option 1\n" << endl;
		}
		break;

	//Allows user to input specific course for more information
	case 3:
		if (ct != nullptr) {
			if (courseId.length() == 0) {
				cout << "What course do you want to know about? ";
				cin >> courseId;
				for (auto& userChoice : courseId) userChoice = toupper(userChoice);
			}
			ct->PrintCourseInfo(courseId);
			cout << endl;
		}else{
			cout << "Load courses first - option 1\n" << endl;
		}
		courseId = "";
		break;
	// User entered invalid data or exited the program
	default:
		if (userChoice != 9){
			cout << userChoice << " is not a valid option\n" << endl;
		break;
		}
	}
}

	cout << "\nThank you for using the course planner!" << endl;
	return 0;
}








