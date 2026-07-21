/*
CodeAlpha Internship - Task 1
CGPA Calculator

The idea:
    - Ask the student how many courses they took.
    - For each course, ask for the letter grade and the credit hours.
    - Convert the letter grade into grade points (A = 4.0, B = 3.0, etc).
    - GPA (for one semester) = total grade points / total credit hours.
    - Here we only calculate one semester, then call it the "CGPA"
      as the task asks (in real life CGPA is the average across many
      semesters, but for this task we just do it for the courses given).
*/

#include <iostream>
#include <string>
using namespace std;

const int MAX_COURSES = 100; // maximum number of courses this program supports

//converts a letter grade (like 'A', 'B+') into a grade point value.
//returns -1 if the grade is not recognized.
double gradeToPoint(string grade) {
    if (grade == "A")  return 4.0;
    if (grade == "A-") return 3.7;
    if (grade == "B+") return 3.3;
    if (grade == "B")  return 3.0;
    if (grade == "B-") return 2.7;
    if (grade == "C+") return 2.3;
    if (grade == "C")  return 2.0;
    if (grade == "C-") return 1.7;
    if (grade == "D")  return 1.0;
    if (grade == "F")  return 0.0;
    return -1; // invalid grade
}

int main() {
    int numCourses;

    cout << "=== CGPA CALCULATOR ===" << endl;
    cout << "Enter number of courses: ";
    cin >> numCourses;

    if (numCourses > MAX_COURSES) {
        cout << "Too many courses! This program supports up to " << MAX_COURSES << " courses." << endl;
        return 1;
    }

    //arrays to store each course's data
    string grades[MAX_COURSES];
    double creditHours[MAX_COURSES];
    double gradePoints[MAX_COURSES];

    double totalCredits = 0;
    double totalGradePoints = 0;

    //looping through each course and collect input
    for (int i = 0; i < numCourses; i++) {
        cout << "Course " << (i + 1) << ":" << endl;

        double points = -1;
        string grade;

        //keep asking until a valid grade is entered
        while (points == -1) 
        {
            cout << "Enter grade (A, A-, B+, B, B-, C+, C, C-, D, F): ";
            cin >> grade;
            points = gradeToPoint(grade);
            if (points == -1) 
            {
                cout << "Invalid grade, please try again." << endl;
            }
        }

        double credit;
        cout << "Enter credit hours for this course: ";
        cin >> credit;

        grades[i] = grade;
        creditHours[i] = credit;
        gradePoints[i] = points * credit;

        totalCredits += credit;
        totalGradePoints += gradePoints[i];
    }

    double gpa = totalGradePoints / totalCredits;

    //display a summary of everything entered
    cout << "=== SUMMARY ===" << endl;
    for (int i = 0; i < numCourses; i++) 
    {
        cout << "Course " << (i + 1) << " | Grade: " << grades[i] << " | Credit Hours: " << creditHours[i] << " | Grade Points: " << gradePoints[i] << endl;
    }

    cout << "Total Credit Hours: " << totalCredits << endl;
    cout << "Total Grade Points: " << totalGradePoints << endl;
    cout << "Final CGPA: " << gpa << endl;

    return 0;
}