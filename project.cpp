#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>

using namespace std;

struct Date
{
    int month;
    int day;
    int year;
}; 

struct Group
{
    int id;
    vector<string> members;
};

struct Project
{
    int id;
    string description;
    Date deadline;
};

struct Submission
{
    int groupID;
    int projectID;
    Date submissionDate;
};

int inputNumberValidation(int min)
{   
    int input;
    while (true)
    {
        cout << "Input: ";
        if (cin >> input && input >= min)
            break;
        else
        {
            cout << "Your input must be an interger larger than " << min << ".\n";
            cin.clear();
            cin.ignore();
        }
    }
    cin.ignore();
    return input;
}

Date inputDateValidation()
{
    Date date;

    cout << "\tDay: ";
    while (true)
    {
        if(cin >> date.day && (date.day <= 31) && (date.day > 0))
            break;
        else
        {
            cout << "\tInput for day must be an interger larger than 0 and smaller than 31.\n";
            cin.clear();
            cin.ignore();
        }
    }
    cout << "\tMonth: ";
    while (true)
    {
        if(cin >> date.month && (date.month <= 12) && (date.month > 0))
            break;
        else
        {
            cout << "\tInput for month must be an interger larger than 0 and smaller than 12.\n";
            cin.clear();
            cin.ignore();
        }
    }
    cout << "\tYear: ";
    while (true)
    {
        if(cin >> date.year && (date.year > 0))
            break;
        else
        {
            cout << "\tInput for year must be an interger larger than 0.\n";
            cin.clear();
            cin.ignore();
        }
    }
    cin.ignore();
    return date;
}

Date getDateNow(){
    Date now;

    time_t rawtime;
    struct tm *info;
    time(&rawtime);
    info = localtime(&rawtime);

    now.month = info->tm_mon + 1;
    now.day = info->tm_mday;
    now.year = info->tm_year + 1900;

    return now;
}

bool compareTime(Date &date1, Date &date2)
{
    if (date1.year > date2.year)
        return true;
    else if (date1.year < date2.year)
        return false;
    if (date1.month > date2.month)
        return true;
    else if (date1.month < date2.month)
        return false;
    if (date1.day > date2.day)
        return true;
    else if (date1.day < date2.day)
        return false;
    return false;
}

bool checkExistInVector(vector<string> &vec, string &text)
{
    bool result = false;

    for (int i = 0; i < vec.size(); i++)
    {
        if (vec[i] == text)
        {
            result = true;
            break;
        }
    }
    return result;
}

void showMainMenu()
{
    cout << "\n";
    cout << "==================================================\n";
    cout << "CHECK THE SUBMISSION OF THE PROJECTS OF GROUPS.\n";
    cout << "--------------------------------------------------\n";
    cout << "1. Group information.\n";
    cout << "2. Project deadline declaration.\n";
    cout << "3. Submit project.\n";
    cout << "4. Statistic.\n";
    cout << "5. Overall statistic.\n";
    cout << "6. Find groups do not complete or submit on time.\n";
    cout << "0. Quit.\n";
    cout << "--------------------------------------------------\n";
    cout << "==================================================\n\n";
}

void getGroupInfo(vector<Group> &groups)
{
    int groupSize, membersSize;
    string name;
    vector<string> nameVec;

    for (int i = 0; i < groups.size(); i++)
        for (int j = 0; j < groups[i].members.size(); j++)
            nameVec.push_back(groups[i].members[j]);


    cout << "\nEnter number of group.\n";
    groupSize = inputNumberValidation(1);
    for (int i = 0; i < groupSize; i++)
    {   
        Group group;
        group.id = groups.size()+1;
        cout << "\nEnter number of members in group " << groups.size()+1 << ".\n";
        membersSize = inputNumberValidation(1);
        for (int j = 0; j < membersSize; j++)
        {
            cout << "Enter the name of member " << j+1 << ": ";
            getline(cin, name);
            if (checkExistInVector(nameVec, name))
            {
                j--;
                cout << "\tThis member is entered.\n";
            }
            else
            {
                nameVec.push_back(name);
                group.members.push_back(name);
            }
        }
        groups.push_back(group);
    }
}

void displayGroupInfo(vector<Group> &groups)
{
    cout << endl;
    cout << setprecision(2);
    cout << left << setw(9) << "Group" << setw(26) << "Student's name" << endl;
    cout << "-----------------------------------\n";
    for (int i = 0; i < groups.size(); i++)
    {   
        cout << left << setw(9) << groups[i].id << setw(26) << groups[i].members[0] << endl;
        for (int j = 1; j < groups[i].members.size(); j++)
        {
            cout << left << setw(9) << "" << setw(26) << groups[i].members[j] << endl;
        }
    }
}

void saveGroupInfo(vector<Group> &groups)
{
    ofstream outputFile;
    outputFile.open("group.txt");

    if (!outputFile)
        cout << "Not found file group.txt in the folder.\n";
    else
    {
        for (int i = 0; i < groups.size(); i++)
        {
            for (int j = 0; j < groups[i].members.size(); j++)
            {
                outputFile << groups[i].members[j] << ",";
            }
            outputFile << "\n";
        }
        outputFile.close();
    }
}

void loadGroupInfo(vector<Group> &groups)
{
    ifstream inputFile;
    string text, delim = ",";
    int i = 0;

    inputFile.open("group.txt");
    if (!inputFile)
        cout << "Not found file group.txt in the folder.\n";
    else
    {
        while (getline(inputFile, text))
        {
            vector<string> members;
            int prev = 0, pos = 0;
            Group group;
            group.id = i+1;

            do
            {   
                string name;
                pos = text.find(delim, prev);
                if (pos == string::npos) 
                    pos = text.length();
                name = text.substr(prev, pos-prev);
                if (!name.empty()) 
                    group.members.push_back(name);
                prev = pos + delim.length();
            }
            while (pos < text.length() && prev < text.length());
    
            groups.push_back(group);
            i++;
        }
        cout << "Your data is loaded.\n";
        inputFile.close();
    }
}

void GroupInformationMenu(vector<Group> &groups)
{
    int choice;

    do
    {
        cout << "\n";
        cout << "==================================================\n";
        cout << "Group Information.\n";
        cout << "--------------------------------------------------\n";
        cout << "1. Input information of group.\n";
        cout << "2. Display information.\n";
        cout << "3. Save group information.\n";
        cout << "4. Load group information.\n";
        cout << "0. Back to the main menu.\n";
        cout << "--------------------------------------------------\n";
        cout << "==================================================\n\n";

        cout << "Enter the number equivalent to the action you want.\n";
        choice = inputNumberValidation(0);

        switch (choice)
        {
            case 1:
                getGroupInfo(groups);
                break;

            case 2:
                displayGroupInfo(groups);
                break;
                
            case 3:
                saveGroupInfo(groups);
                break;
                
            case 4:
                loadGroupInfo(groups);
                break;

            case 0:
                cout << "Back to the main menu.\n";
                break;
            
            default:
                cout << "Invalid selection. Try again.\n";
        }
    } while (choice != 0);
}

void displayProjects(vector<Project> &projects)
{
    cout << endl;
    cout << setprecision(2);
    cout << left << setw(9) << "Project" << setw(12) << "Deadline" << setw(60) << "Description" << endl;
    cout << "-----------------------------------\n";
    for (int i = 0; i < projects.size(); i++)
    {   
        cout << left << setw(9) << projects[i].id << right << setw(2) << setfill('0') << projects[i].deadline.day << "/" << setw(2) << projects[i].deadline.month << "/" << left << setw(6) << setfill(' ') << projects[i].deadline.year << setw(60) << projects[i].description << endl;
    }
}

void inputProjects(vector<Project> &projects)
{
    int projectsSize;

    cout << "\nEnter number of projects.\n";
    projectsSize = inputNumberValidation(0);

    for (int i = 0; i < projectsSize; i++)
    {
        Project project;
        project.id = projects.size()+1;
        
        cout << "Short description about project " << projects.size()+1 << ": ";
        getline(cin, project.description);
        cout << "Submission deadline for project " << projects.size()+1 << " (DD/MM/YYYY): \n";
        project.deadline = inputDateValidation();
        projects.push_back(project);
    }
    displayProjects(projects);
}

void submitProjects(vector<Submission> &subs)
{
    Submission sub;

    cout << "Enter a group number.\n";
    sub.groupID = inputNumberValidation(1);
    cout << "Enter a project number.\n";
    sub.projectID = inputNumberValidation(1);
    cout << "Submission Date(DD/MM/YYYY): \n";
    sub.submissionDate = inputDateValidation();
    subs.push_back(sub);
}


void statisticProject(vector<Group> &groups, vector<Project> &projects, vector<Submission> &subs)
{
    Date now = getDateNow();
    bool done;
    int projectNum;


    cout << "Enter a project number.\n";
    projectNum = inputNumberValidation(1);

    cout << endl;
    cout << setprecision(2);
    cout << left << setw(9) << "Group" << "Project " << projectNum << endl;
    cout << "------------------------------------\n";

    for (int groupNum = 1; groupNum <= groups.size(); groupNum++)
    {
        done = false;
        for (int subNum = 0; subNum < subs.size(); subNum++)
        {
            if (subs[subNum].projectID == projectNum)
            {
                if(subs[subNum].groupID == groupNum)
                {
                    done = true;
                    if(compareTime(projects[projectNum-1].deadline, subs[subNum].submissionDate))
                        cout << left << setw(9) << groupNum << setw (12) << "On Time" << endl;
                    else
                        cout << left << setw(9) << groupNum << setw (12) << "Late" << endl;
                }
            }
        }
        if(!done)
        {
            if(compareTime(projects[projectNum-1].deadline, now))
                cout << left << setw(9) << groupNum << setw (12) << "Not yet submitted" << endl;
            else
                cout << left << setw(9) << groupNum << setw (12) << "Late" << endl;
        }
    }
}

void statisticGroup(vector<Group> &groups, vector<Project> &projects, vector<Submission> &subs)
{
    Date now = getDateNow();
    bool done;
    int groupNum;


    cout << "Enter a group number.\n";
    groupNum = inputNumberValidation(1);

    cout << endl;
    cout << setprecision(2);
    cout << left << setw(9) << "Project" << "Group " << groupNum << endl;
    cout << "------------------------------------\n";

    for (int projectNum = 1; projectNum <= projects.size(); projectNum++)
    {
        done = false;
        for (int subNum = 0; subNum < subs.size(); subNum++)
        {
            if (subs[subNum].projectID == projectNum)
            {
                if(subs[subNum].groupID == groupNum)
                {
                    done = true;
                    if(compareTime(projects[projectNum-1].deadline, subs[subNum].submissionDate))
                        cout << left << setw(9) << projectNum << setw (12) << "On Time" << endl;
                    else
                        cout << left << setw(9) << projectNum << setw (12) << "Late" << endl;
                }
            }
        }
        if(!done)
        {
            if(compareTime(projects[projectNum-1].deadline, now))
                cout << left << setw(9) << projectNum << setw (12) << "Not yet submitted" << endl;
            else
                cout << left << setw(9) << projectNum << setw (12) << "Late" << endl;
        }
    }
}

void showStatistic(vector<Group> &groups, vector<Project> &projects, vector<Submission> &subs){
    int choice;

    do
    {
        cout << "\n";
        cout << "==================================================\n";
        cout << "Statistic.\n";
        cout << "--------------------------------------------------\n";
        cout << "1. Show state of submissin by a project number.\n";
        cout << "2. Show state of submission by a group number.\n";
        cout << "0. Back to the main menu.\n";
        cout << "--------------------------------------------------\n";
        cout << "==================================================\n\n";

        cout << "Enter the number equivalent to the action you want.\n";
        choice = inputNumberValidation(0);

        switch (choice)
        {
            case 1:
                statisticProject(groups, projects, subs);
                break;

            case 2:
                statisticGroup(groups, projects, subs);
                break;

            case 0:
                cout << "Back to the main menu.\n";
                break;

            default:
                break;
        }      
    } while (choice != 0);
    
}

void overalStatistic(vector<Project> &projects)
{   
    Date date;

    ofstream outputFile;
    outputFile.open("overall_statistic.txt");

    if (!outputFile)
        cout << "Not found file group.txt in the folder. Cannot save this data.\n";

    cout << "Show statistic from beginning of the course to (DD/MM/YY):\n";
    date = inputDateValidation();


    cout << endl;
    cout << setprecision(2);
    cout << left << setw(9) << "Project" << setw(12) << "Deadline" << setw(60) << "Description" << endl;
    cout << "-----------------------------------\n";

    if(outputFile)
        outputFile << left << setw(9) << "Project" << setw(12) << "Deadline" << setw(60) << "Description" << endl << "-----------------------------------\n";

    for (int i = 0; i < projects.size(); i++)
    {
        if(compareTime(date, projects[i].deadline))
        {
            cout << left << setw(9) << projects[i].id << right << setw(2) << setfill('0') << projects[i].deadline.day << "/" << setw(2) << projects[i].deadline.month << "/" << left << setw(6) << setfill(' ') << projects[i].deadline.year << setw(60) << projects[i].description << endl;
            if(outputFile)
                outputFile << left << setw(9) << projects[i].id << right << setw(2) << setfill('0') << projects[i].deadline.day << "/" << setw(2) << projects[i].deadline.month << "/" << left << setw(6) << setfill(' ') << projects[i].deadline.year << setw(60) << projects[i].description << endl;
        }
    }
    if(outputFile) 
        outputFile.close();
}

void overallStatistic(vector<Group> &groups, vector<Project> &projects, vector<Submission> &subs)
{  
    Date date;

    ofstream outputFile;
    outputFile.open("overall_statistic.txt");

    if (!outputFile)
        cout << "Not found file group.txt in the folder. Cannot save this data.\n";
    else
        return;

    cout << "Show statistic from beginning of the course to (DD/MM/YY):\n";
    date = inputDateValidation();

    Date now = getDateNow();
    bool done;

    cout << endl;
    cout << setprecision(2);
    cout << left << setw(9) << "Group";
    outputFile << left << setw(9) << "Group";
    for (int i = 0; i < projects.size(); i++)
    {
        if(compareTime(date, projects[i].deadline))
        {
            cout  << "Project " << setw(12) << i+1;
            outputFile  << "Project " << setw(12) << i+1;
        }
    }
    cout << "\n-----------------------------------------------------------------\n";
    outputFile << "\n-----------------------------------------------------------------\n";

    for (int groupNum = 1; groupNum <= groups.size(); groupNum++)
    {
        cout << left << setw(9) << groupNum;
        outputFile << left << setw(9) << groupNum;
        for (int projectNum = 1; projectNum <= projects.size(); projectNum++)
        {
            if(compareTime(date, projects[projectNum].deadline))
            {
                done = false;
                for (int subNum = 0; subNum < subs.size(); subNum++)
                {
                    if (subs[subNum].projectID == projectNum)
                    {
                        if(subs[subNum].groupID == groupNum)
                        {
                            done = true;
                            if(compareTime(projects[projectNum-1].deadline, subs[subNum].submissionDate))
                            {
                                cout << setw (20) << "On Time";
                                outputFile << setw (20) << "On Time";
                            }
                                
                            else
                            {
                                cout << setw(20) << "Late";
                                outputFile << setw(20) << "Late";
                            }
                        }
                    }
                }
                if(!done)
                {
                    if(compareTime(projects[projectNum-1].deadline, now))
                    {
                        cout << setw (20) << "Not yet submitted";
                        outputFile << setw (20) << "Not yet submitted";
                    }
                        
                    else
                    {
                        cout << setw (20) << "Not yet submitted";
                        outputFile << setw (20) << "Not yet submitted";
                    }
                }
            }

        }
        cout << endl;
    }
}

void groupStatistic(vector<Group> &groups, vector<Project> &projects, vector<Submission> &subs)
{   
    Date now = getDateNow();
    bool check, done;

    cout << endl;
    cout << setprecision(2);
    cout << "Groups do not complete or submit on time.\n";
    cout << "------------------------------------\n";

    for (int groupNum = 1; groupNum <= groups.size(); groupNum++)
    {
        done = true;
        for (int projectNum = 1; projectNum <= projects.size(); projectNum++)
        {
            check = false;
            for (int subNum = 0; subNum < subs.size(); subNum++)
            {
                if (subs[subNum].projectID == projectNum)
                {
                    if(subs[subNum].groupID == groupNum)
                    {
                        if(!compareTime(projects[projectNum-1].deadline, subs[subNum].submissionDate))
                            done = false;
                        check = true;
                    }
                }
            }  
        }
        if(!done && projects.size() > 0)
        {
            cout << "Group " << groupNum << "\t\tDo not submit on time\n";
        }
        else if(!check && projects.size() > 0)
        {
            cout << "Group " << groupNum << "\t\tDo not complete\n";
        }
        if (projects.size() == 0)
        {
            cout << "\tProjects have not been imported.\n";
        }
    }   
}


int main()
{
    vector<Group> groups;
    vector<Project> projects;
    vector<Submission> subs;

    int mainMenuChoice;

    do
    {
        showMainMenu();
        cout << "Enter the number equivalent to the action you want.\n";
        mainMenuChoice = inputNumberValidation(0);

        switch (mainMenuChoice)
        {
            case 1:
                GroupInformationMenu(groups);
                break;

            case 2:
                inputProjects(projects);
                break;
                
            case 3:
                submitProjects(subs);
                break;
                
            case 4:
                showStatistic(groups, projects, subs);
                break;
                
            case 5:
                overallStatistic(groups, projects, subs);
                break;
                
            case 6:
                groupStatistic(groups, projects, subs);
                break;
            
            case 0:
                cout << "Quit the program.\n";
                break;

            default:
                cout << "Invalid selection. Try again.\n";
        }
    } while (mainMenuChoice!=0);
    
    return 0;
}