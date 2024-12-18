#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

// Structure to hold student information
struct Student {
    int rollNumber;
    string name;
};

// Function to load student data from file
void loadStudents(Student students[], int &studentCount, const string& filename) {
    ifstream file(filename);
    string line;
    studentCount = 0;
    
    while (getline(file, line)) {
        stringstream ss(line);
        string rollnostr;
        getline(ss, rollnostr, ','); // Roll number
        students[studentCount].rollNumber = stoi(rollnostr);
        getline(ss, students[studentCount].name); // Name
        studentCount++;
    }
    
    file.close();
}

// Function to save attendance data
void saveAttendance(Student students[], int studentCount,string date) {
    ofstream attendanceFile("attendance_log.txt", ios::app);
    attendanceFile << "Date: " << date << endl;
    
    for (int i = 0; i < studentCount; i++) {
        char attendanceStatus;
        cout << "Enter attendance for Roll Number " << students[i].rollNumber << " (" << students[i].name << ") [P/A]: ";
        cin >> attendanceStatus;
        attendanceFile << students[i].rollNumber << ", " << (attendanceStatus == 'P' || attendanceStatus == 'p' ? "Present" : "Absent") << endl;
    }

    attendanceFile.close();
}

// Function to calculate attendance percentage
void AttendancePercentage(Student students[], int studentCount) {
    ifstream attendanceFile("attendance_log.txt");
    if(!attendanceFile.is_open()){
            cerr << "Error: Unable to open attendance_log.txt" << endl;
            return;
        }
    string line;
    int totalDays[studentCount] = {0};
    int presentDays[studentCount] = {0};
    
    while (getline(attendanceFile, line)) {
        if (line.find("Date:") != string::npos) continue;
        
        stringstream ss(line);
        string rollNumberStr, status;
        getline(ss, rollNumberStr, ',');
        getline(ss, status);
        
        int rollNumber = stoi(rollNumberStr);
        
        for (int i = 0; i < studentCount; i++) {
            if (students[i].rollNumber == rollNumber) {
                totalDays[i]++;
                if (status==" Present") {
                    presentDays[i]++;
                }
            }
        }
    }
    
    attendanceFile.close();
    
    // Generate summary file
    ofstream summaryFile("attendance_summary.txt");
    if(!summaryFile){
            cerr << "Error: Unable to open attendance_log.txt" << endl;
            return;
        }
    for (int i = 0; i < studentCount; i++) {
        double percentage =(totalDays[i] > 0) ? ((double)presentDays[i] / totalDays[i]) * 100 : 0;
        summaryFile << "Roll Number: " << students[i].rollNumber << ", Name: " << students[i].name 
                    << ", Attendance: " <<fixed<<setprecision(2)<< percentage << "%" << endl;
    }

    summaryFile.close();
}

// Function to export individual report
void individual_report(Student students[],int studentcount){
    int totaldays[studentcount]={0};
    int presentdays[studentcount]={0};

    ifstream attendancelog("attendance_log.txt");
    if(!attendancelog){
            cerr << "Error: Unable to open attendance_log.txt" << endl;
            return;
        }
    string line;
    string rollnostr;
    string status;
    while(getline(attendancelog,line)){
        if(line.find("Date:")!=string::npos)continue;
        stringstream ss(line);
        getline(ss,rollnostr,',');
        getline(ss,status);
        int rollno=stoi(rollnostr);
    for(int i=0;i<studentcount;i++){
            if(rollno==students[i].rollNumber){
                totaldays[i]++;
                if(status==" Present"){
                    presentdays[i]++;
                }
            }
        }
    }
    attendancelog.close();
    for(int i=0;i<studentcount;i++){
        ofstream individualreport(to_string(students[i].rollNumber)+"_report.txt");
        if(!individualreport){
            cerr << "Error: Unable to open file "+ to_string(students[i].rollNumber) + ".txt" << endl;
            return;
        }
        individualreport<<"Attendance Report for Roll number "<<students[i].rollNumber<<" - "<<students[i].name<<endl;
        individualreport<<"Total Days : "<<totaldays[i]<<endl;
        individualreport<<"Days Present : "<<presentdays[i]<<endl;
        individualreport<<"Days Absent : "<<totaldays[i]-presentdays[i]<<endl;
        double percentage = (totaldays[i] > 0) ? ((double)presentdays[i] / totaldays[i]) * 100 : 0;
        individualreport<<"Attendance Percentage : "<<setprecision(2)<<fixed<<percentage<<" % ."<<endl;
        individualreport.close();
    }

}

// Function to export students below 80% attendance
void below80(Student student[],int studentcount){
    ifstream attendance("attendance_log.txt");
    string rollnostr;
    string status;
    string line;
    int totalDays[studentcount]={0};
    int presentDays[studentcount]={0};
    while(getline(attendance,line)){
        if(line.find("Date:") != string::npos) continue;
        stringstream ss(line);
        getline(ss,rollnostr,',');
        int rollno=stoi(rollnostr);
        getline(ss,status);
        for(int i=0;i<studentcount;i++){
            if(student[i].rollNumber==rollno){
                totalDays[i]++;
                if(status==" Present"){
                    presentDays[i]++;
                }
            }
        }
    }
    attendance.close();
    ofstream lowattendance("below80.txt");
    if(!lowattendance.is_open()){
        cerr<<"Error! File could not open.";
        return;
    }
    for (int i = 0; i < studentcount; i++)
    {
        double percentage=(totalDays[i]>0)? ((double)presentDays[i]/totalDays[i]) * 100 : 0;
        if(percentage<80){
            lowattendance<<"Roll no: "<<student[i].rollNumber<<", Name: "<<student[i].name
            <<", Attendance: "<<fixed<<setprecision(2)<<percentage<<" %"<<endl;
        }
    }
    lowattendance.close();
        
         
}

// Main function
int main() {
    cout << "Student Attendance Management System" << endl;
    
    Student students[100]; // Array to store student data (can hold up to 100 students)
    int studentCount = 0;

    // Load student data
    loadStudents(students, studentCount, "studentdetails.txt");
    
    // Menu for marking attendance
    char choice;
    do {
        cout << "\nMenu:\n";
        cout << "1. Mark Attendance\n";
        cout << "2. Export Percentage File\n";
        cout << "3. Export Individual Report\n";
        cout << "4. Below 80 %\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == '1') {
            // Mark attendance
            string date;
            cout << "Enter the date (yyyy-mm-dd): ";
            cin >> date;
            saveAttendance(students, studentCount, date);
        }
        else if (choice == '2') {
            // Calculate attendance percentage
            AttendancePercentage(students, studentCount);
            cout << "Attendance report generated!" << endl;
        }
        else if (choice == '3') {
            // Calculate and generate individual reports
            individual_report(students, studentCount);
            cout << "Attendance report generated!" << endl;
        }
        else if (choice == '4') {
            // find students having less than 80 percentage
            below80(students, studentCount);
            cout << "Attendance report generated!" << endl;
        }
    } while (choice != '5');
    
    return 0;
}
