#include "Admin.h"
#include "Authorization.h"
#include "PoliticalParty.h"
#include "Election.h"
#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include "DateTime.h"


Profile& Admin::getProfile() {
    return adminProfile;
}

const Profile& Admin::getProfile() const {
    return adminProfile;
}

bool Admin::loadAdminProfile(const string& cnic, const string& dataFilePath) {
    if (adminProfile.loadFromFile(cnic, dataFilePath)) {
        return true;
    }
    else {
        return false;
    }
}

bool Admin::updateLoginTimestamp(const string& dataFilePath) {
    if (adminProfile.updateTimestampInFile(adminProfile.getCnic(), dataFilePath, DateTime::getCurrentDateTime().toString())) {
        adminProfile.setLastLoginTimestamp(DateTime::getCurrentDateTime().toString());
        return true;
    }
    else {
        return false;
    }
}

void Admin::displayAdminMenu() {
    system("cls");
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t.....................A D M I N   D A S H B O A R D....................\n";
    cout << "\t\t\t\t\t\t......................................................................\n";
    string nameHeader = "Logged In: " + adminProfile.getFullName() + " (Admin)";
    string timeHeader = "Login Time: " + adminProfile.getLastLoginTimestamp();

    int spaces_name = 66 - nameHeader.length(); if (spaces_name < 0) spaces_name = 0;
    string pad_name(spaces_name / 2, ' ');
    cout << "\t\t\t\t\t\t.." << pad_name << nameHeader << pad_name << (spaces_name % 2 != 0 ? " " : "") << "..\n";

    int spaces_time = 66 - timeHeader.length(); if (spaces_time < 0) spaces_time = 0;
    string pad_time(spaces_time / 2, ' ');
    cout << "\t\t\t\t\t\t.." << pad_time << timeHeader << pad_time << (spaces_time % 2 != 0 ? " " : "") << "..\n";

    cout << "\t\t\t\t\t\t..                                                                  ..\n";
    cout << "\t\t\t\t\t\t..                     '1' User Management                          ..\n";
    cout << "\t\t\t\t\t\t..                     '2' Party Management                         ..\n";
    cout << "\t\t\t\t\t\t..                     '3' Election Management                      ..\n";
    cout << "\t\t\t\t\t\t..                     '4' Account Approval                         ..\n";
    cout << "\t\t\t\t\t\t..                     '7' View/Update Profile                      ..\n";
    cout << "\t\t\t\t\t\t..                     '9' Log Out                                  ..\n";
    cout << "\t\t\t\t\t\t..                     '0' Main Menu                                ..\n";
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t                      Enter your choice here: ";
}

void Admin::displayDashboard() {
    displayAdminMenu();
}

void Admin::Dashboard() {
    string entry; int choice;
    while (true) {
        displayDashboard();
        getline(cin, entry, '\n');
        if (entry.length() == 1 && isdigit(entry.at(0))) {
            try { choice = stoi(entry); }
            catch (...) { invalidEntry(); continue; }
            switch (choice) {
            case 1: userManagement(); break;
            case 2: partyManagement(); break;
            case 3: electionManagement(); break;
            case 4: accountApproval(); break;
            case 7: viewAdminProfile(); break;
            case 9: loginPanel(); return;
            case 0: mainPanel(); return;
            default: invalidEntry(); break;
            }
        }
        else { invalidEntry(); }
    }
}

void Admin::displayUserManagementMenu() {
    system("cls");
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t....................U S E R   M A N A G E M E N T.....................\n";
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t..                                                                  ..\n";
    cout << "\t\t\t\t\t\t..                       '1' Add User                               ..\n";
    cout << "\t\t\t\t\t\t..                       '2' Delete User                            ..\n";
    cout << "\t\t\t\t\t\t..                       '3' Modify User                            ..\n";
    cout << "\t\t\t\t\t\t..                       '4' Search User                            ..\n";
    cout << "\t\t\t\t\t\t..                       '5' List All Users                         ..\n";
    cout << "\t\t\t\t\t\t..                       '0' Go Back                                ..\n";
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t                      Enter your choice here: ";
}

void Admin::userManagement() {
    string entry; int choice;
    while (true) {
        displayUserManagementMenu();
        getline(cin, entry, '\n');
        if (entry.length() == 1 && isdigit(entry.at(0))) {
            try { choice = stoi(entry); }
            catch (...) { invalidEntry(); continue; }
            switch (choice) {
            case 1: addUser(); break;
            case 2: deleteUser(); break;
            case 3: modifyUser(); break;
            case 4: searchUser(); break;
            case 5: listAllUsers(); break;
            case 0: return;
            default: invalidEntry(); break;
            }
        }
        else { invalidEntry(); }
    }
}

void Admin::addUser() {
    system("cls");
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t.......................A D D   N E W   U S E R........................\n";
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t..               Redirecting to User Sign Up Process...             ..\n";
    cout << "\t\t\t\t\t\t......................................................................\n";
    Sleep(2000);

    UserSignUp userSignUpProcess;
    userSignUpProcess.signUp();

    displayMessage("User Sign Up process completed. Returning to User Management.", false);
}

bool isValidCnicForDelete(const string& cnic) {
    if (cnic.length() != 13) {
        displayMessage("Invalid CNIC: Must be 13 digits.", true);
        return false;
    }
    if (!containsOnlyDigits(cnic)) {
        displayMessage("Invalid CNIC: Must contain only digits.", true);
        return false;
    }
    return true;
}

int readFileToArray(const string& filePath, string linesArray[], int maxLines) {
    ifstream inFile(filePath);
    if (!inFile.is_open()) {
        return 0;
    }
    int count = 0;
    string line;
    while (count < maxLines && getline(inFile, line)) {
        if (!line.empty()) {
            linesArray[count++] = line;
        }
    }
    inFile.close();
    return count;
}

bool writeArrayToFile(const string& filePath, const string linesArray[], int count) {
    ofstream outFile(filePath, ios::trunc);
    if (!outFile.is_open()) {
        displayMessage("Error: Cannot open file for writing: " + filePath, true);
        return false;
    }
    for (int i = 0; i < count; ++i) {
        outFile << linesArray[i] << endl;
    }
    outFile.close();
    return true;
}

void Admin::deleteUser() {
    system("cls");
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t...........................D E L E T E   U S E R......................\n";
    cout << "\t\t\t\t\t\t......................................................................\n";
    string cnicToDelete;
    cout << "\t\t\t\t\t\tEnter CNIC (13 digit): ";
    getline(cin, cnicToDelete);

    if (!isValidCnicForDelete(cnicToDelete)) {
        return;
    }

    bool userFoundAndDeleted = false;

    string* credLines = new string[MAX_FILE_LINES];
    string* dataLines = new string[MAX_FILE_LINES];
    string* approvLines = new string[MAX_FILE_LINES];

    if (credLines == NULL || dataLines == NULL || approvLines == NULL) {
        displayMessage("Critical Error: Memory allocation failed.", true);
        if (credLines != NULL) delete[] credLines;
        if (dataLines != NULL) delete[] dataLines;
        if (approvLines != NULL) delete[] approvLines;
        return;
    }

    int credCount = readFileToArray(USER_CREDENTIALS_FILE_PATH, credLines, MAX_FILE_LINES);
    int credDeleteIndex = -1;
    for (int i = 0; i < credCount; ++i) {
        size_t dashPos = credLines[i].find('-');
        if (dashPos != string::npos) {
            string recordCnic = credLines[i].substr(0, dashPos);
            if (recordCnic == cnicToDelete) {
                credDeleteIndex = i;
                break;
            }
        }
    }
    if (credDeleteIndex != -1) {
        userFoundAndDeleted = true;
        for (int i = credDeleteIndex; i < credCount - 1; ++i) {
            credLines[i] = credLines[i + 1];
        }
        credCount--;
        if (!writeArrayToFile(USER_CREDENTIALS_FILE_PATH, credLines, credCount)) {
            displayMessage("Failed to update credentials file.", true);
        }
    }

    int dataCount = readFileToArray(USER_DATA_FILE_PATH, dataLines, MAX_FILE_LINES);
    int dataDeleteIndex = -1;
    for (int i = 0; i < dataCount; ++i) {
        size_t dashPos = dataLines[i].find('-');
        if (dashPos != string::npos) {
            string recordCnic = dataLines[i].substr(0, dashPos);
            if (recordCnic == cnicToDelete) {
                dataDeleteIndex = i;
                break;
            }
        }
    }
    if (dataDeleteIndex != -1) {
        userFoundAndDeleted = true;
        for (int i = dataDeleteIndex; i < dataCount - 1; ++i) {
            dataLines[i] = dataLines[i + 1];
        }
        dataCount--;
        if (!writeArrayToFile(USER_DATA_FILE_PATH, dataLines, dataCount)) {
            displayMessage("Failed to update user data file.", true);
        }
    }

    int approvCount = readFileToArray(APPROVAL_FILE, approvLines, MAX_FILE_LINES);
    int approvDeleteIndex = -1;
    for (int i = 0; i < approvCount; ++i) {
        size_t dashPos = approvLines[i].find('-');
        if (dashPos != string::npos) {
            string recordCnic = approvLines[i].substr(0, dashPos);
            if (recordCnic == cnicToDelete) {
                approvDeleteIndex = i;
                break;
            }
        }
    }
    if (approvDeleteIndex != -1) {
        for (int i = approvDeleteIndex; i < approvCount - 1; ++i) {
            approvLines[i] = approvLines[i + 1];
        }
        approvCount--;
        if (!writeArrayToFile(APPROVAL_FILE, approvLines, approvCount)) {
            displayMessage("Failed to update approval file.", true);
        }
    }

    if (userFoundAndDeleted) {
        displayMessage("User (" + cnicToDelete + ") deleted successfully.", false);
    }
    else {
        displayMessage("User (" + cnicToDelete + ") not found.", true);
    }

    delete[] credLines;
    delete[] dataLines;
    delete[] approvLines;
}

bool adminSimpleIsValidName(const string& name) {
    if (name.empty() || name.length() > 50) return false;
    for (unsigned int i = 0; i < name.length(); ++i) {
        if (!isalpha(name[i]) && !isspace(name[i])) return false;
    }
    return !name.empty();
}

bool adminSimpleIsValidAge(const string& ageStr, int& age) {
    if (!containsOnlyDigits(ageStr) || ageStr.empty()) return false;
    try { age = stoi(ageStr); }
    catch (...) { return false; }
    return age > 0 && age < 120;
}

bool adminSimpleIsValidPhone(const string& phone) {
    if (phone.length() != 13 || phone.substr(0, 3) != "+92") return false;
    return containsOnlyDigits(phone.substr(3));
}

bool adminSimpleIsValidPassword(const string& pass) {
    return !pass.empty() && pass.length() >= 8 && pass.length() <= 30;
}

bool adminSimpleIsValidGender(const string& gender) {
    if (gender.empty() || gender.length() > 20) return false;
    for (unsigned int i = 0; i < gender.length(); ++i) {
        if (!isalpha(gender[i])) return false;
    }
    return !gender.empty();
}
bool adminSimpleIsValidPostalCode(const string& code) {
    return code.length() == 5 && containsOnlyDigits(code);
}
bool adminSimpleIsValidGenericText(const string& text) {
    if (text.empty() || text.length() > 100) return false;
    for (unsigned int i = 0; i < text.length(); ++i) {
        if (!isalnum(text[i]) && !isspace(text[i]) && text[i] != '/' && text[i] != ',' && text[i] != '.') return false;
    }
    return !text.empty();
}

string adminConstructLine(const string fields[], int count, char delimiter) {
    string line = "";
    for (int i = 0; i < count; ++i) {
        line += fields[i];
        if (i < count - 1) {
            line += delimiter;
        }
    }
    return line;
}

void displayModifyUserFieldMenu() {
    cout << "\n\t\t\t\t\t\t.........................Which field to modify?.......................\n";
    cout << "\t\t\t\t\t\t.. 1. First Name                10. Postal Code                     ..\n";
    cout << "\t\t\t\t\t\t.. 2. Last Name                 11. Account Status                  ..\n";
    cout << "\t\t\t\t\t\t.. 3. Password                  12. Overall Candidate Status        ..\n";
    cout << "\t\t\t\t\t\t.. 4. Age                       13. Party Affiliation (ID or NONE)  ..\n";
    cout << "\t\t\t\t\t\t.. 5. Phone Number              14. Dependent Candidate Status      ..\n";
    cout << "\t\t\t\t\t\t.. 6. Gender                    15. Election Participation Status   ..\n";
    cout << "\t\t\t\t\t\t.. 7. State                     16. Participating Election ID       ..\n";
    cout << "\t\t\t\t\t\t.. 8. City                                                          ..\n";
    cout << "\t\t\t\t\t\t.. 9. Town                                                          ..\n";
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t.. 99. Save Changes                                                 ..\n";
    cout << "\t\t\t\t\t\t.. 0. Discard Changes & Go Back                                     ..\n";
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t                      Enter your choice here: ";
}

void Admin::modifyUser() {
    system("cls");
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t..........................M O D I F Y   U S E R.......................\n";
    cout << "\t\t\t\t\t\t......................................................................\n";
    string cnicToModify;
    cout << "\t\t\t\t\t\tEnter CNIC of the user to modify (13 digits): ";
    getline(cin, cnicToModify);

    if (!isValidCnicForDelete(cnicToModify)) {
        return;
    }

    string* credLines = new string[MAX_FILE_LINES];
    string* dataLines = new string[MAX_FILE_LINES];

    if (credLines == NULL || dataLines == NULL) {
        displayMessage("Critical Error: Memory allocation failed for modify.", true);
        if (credLines != NULL) delete[] credLines;
        if (dataLines != NULL) delete[] dataLines;
        return;
    }

    int credCount = readFileToArray(USER_CREDENTIALS_FILE_PATH, credLines, MAX_FILE_LINES);
    int dataCount = readFileToArray(USER_DATA_FILE_PATH, dataLines, MAX_FILE_LINES);

    int credModifyIndex = -1;
    for (int i = 0; i < credCount; ++i) {
        if (credLines[i].substr(0, credLines[i].find('-')) == cnicToModify) {
            credModifyIndex = i;
            break;
        }
    }

    int dataModifyIndex = -1;
    for (int i = 0; i < dataCount; ++i) {
        if (dataLines[i].substr(0, dataLines[i].find('-')) == cnicToModify) {
            dataModifyIndex = i;
            break;
        }
    }

    if (credModifyIndex == -1) {
        displayMessage("User (" + cnicToModify + ") not found in credentials. Cannot modify.", true);
        delete[] credLines; delete[] dataLines;
        return;
    }

    string userCredFields[USER_CRED_FIELD_COUNT];
    string userDataFields[DATA_FIELD_COUNT];
    bool credParsed = false;
    bool dataParsed = false;

    if (adminParseLine(credLines[credModifyIndex], '-', userCredFields, USER_CRED_FIELD_COUNT) == USER_CRED_FIELD_COUNT) {
        credParsed = true;
    }
    else {
        displayMessage("Error parsing credential data for " + cnicToModify, true);
        delete[] credLines; delete[] dataLines;
        return;
    }

    if (dataModifyIndex != -1) {
        if (adminParseLine(dataLines[dataModifyIndex], '-', userDataFields, DATA_FIELD_COUNT) == DATA_FIELD_COUNT) dataParsed = true;
    }
    else {
        displayMessage("User (" + cnicToModify + ") data record not found. Some fields unavailable.", false);
    }

    bool changesMade = false;
    string input, newValue;
    int choice;

    while (true) {
        system("cls");
        cout << "\t\t\t\t\t\t......................................................................\n";
        cout << "\t\t\t\t\t\t..........................M O D I F Y   U S E R.......................\n";
        cout << "\t\t\t\t\t\t......................................................................\n";
        cout << "\t\t\t\t\t\tModifying User CNIC: " << cnicToModify << "\n";
        cout << "\t\t\t\t\t\tCurrent FName: " << userCredFields[USER_CRED_FNAME_IDX] << ", LName: " << userCredFields[USER_CRED_LNAME_IDX] << "\n";
        if (dataParsed) cout << "\t\t\t\t\t\tCurrent Age: " << userDataFields[DATA_AGE_IDX] << ", Phone: " << userDataFields[DATA_PHONE_IDX] << "\n";
        else cout << "\t\t\t\t\t\t(User data file record missing or corrupted for some fields)\n";
        cout << "\t\t\t\t\t\t......................................................................\n";

        displayModifyUserFieldMenu();
        getline(cin, input);
        if (input.empty() || !containsOnlyDigits(input)) { invalidEntry(); continue; }
        try { choice = stoi(input); }
        catch (...) { invalidEntry(); continue; }

        switch (choice) {
        case 1:
            cout << "\t\t\t\t\t\tEnter new First Name: "; getline(cin, newValue);
            if (adminSimpleIsValidName(newValue)) {
                userCredFields[USER_CRED_FNAME_IDX] = newValue;
                if (dataParsed) userDataFields[DATA_FNAME_IDX] = newValue;
                changesMade = true; displayMessage("First Name updated.", false);
            }
            else { displayMessage("Invalid First Name.", true); }
            break;
        case 2:
            cout << "\t\t\t\t\t\tEnter new Last Name: "; getline(cin, newValue);
            if (adminSimpleIsValidName(newValue)) {
                userCredFields[USER_CRED_LNAME_IDX] = newValue;
                if (dataParsed) userDataFields[DATA_LNAME_IDX] = newValue;
                changesMade = true; displayMessage("Last Name updated.", false);
            }
            else { displayMessage("Invalid Last Name.", true); }
            break;
        case 3:
            cout << "\t\t\t\t\t\tEnter new Password: "; hidePasswordInput(newValue);
            if (adminSimpleIsValidPassword(newValue)) {
                userCredFields[USER_CRED_PASS_IDX] = newValue;
                changesMade = true; displayMessage("Password updated.", false);
            }
            else { displayMessage("Invalid Password.", true); }
            break;
        case 4:
            if (!dataParsed) { displayMessage("User data record not available for Age.", true); break; }
            cout << "\t\t\t\t\t\tEnter new Age: "; getline(cin, newValue);
            int tempAge;
            if (adminSimpleIsValidAge(newValue, tempAge)) {
                userDataFields[DATA_AGE_IDX] = newValue;
                changesMade = true; displayMessage("Age updated.", false);
            }
            else { displayMessage("Invalid Age.", true); }
            break;
        case 5:
            if (!dataParsed) { displayMessage("User data record not available for Phone.", true); break; }
            cout << "\t\t\t\t\t\tEnter new Phone (+92XXXXXXXXXX): "; getline(cin, newValue);
            if (adminSimpleIsValidPhone(newValue)) {
                userDataFields[DATA_PHONE_IDX] = newValue;
                changesMade = true; displayMessage("Phone updated.", false);
            }
            else { displayMessage("Invalid Phone Number.", true); }
            break;
        case 6:
            if (!dataParsed) { displayMessage("User data record not available for Gender.", true); break; }
            cout << "\t\t\t\t\t\tEnter new Gender: "; getline(cin, newValue);
            if (adminSimpleIsValidGender(newValue)) {
                userDataFields[DATA_GENDER_IDX] = newValue;
                changesMade = true; displayMessage("Gender updated.", false);
            }
            else { displayMessage("Invalid Gender.", true); }
            break;
        case 7:
            if (!dataParsed) { displayMessage("User data record not available for State.", true); break; }
            cout << "\t\t\t\t\t\tEnter new State: "; getline(cin, newValue);
            if (adminSimpleIsValidGenericText(newValue)) {
                userDataFields[DATA_STATE_IDX] = newValue;
                changesMade = true; displayMessage("State updated.", false);
            }
            else { displayMessage("Invalid State.", true); }
            break;
        case 8:
            if (!dataParsed) { displayMessage("User data record not available for City.", true); break; }
            cout << "\t\t\t\t\t\tEnter new City: "; getline(cin, newValue);
            if (adminSimpleIsValidGenericText(newValue)) {
                userDataFields[DATA_CITY_IDX] = newValue;
                changesMade = true; displayMessage("City updated.", false);
            }
            else { displayMessage("Invalid City.", true); }
            break;
        case 9:
            if (!dataParsed) { displayMessage("User data record not available for Town.", true); break; }
            cout << "\t\t\t\t\t\tEnter new Town: "; getline(cin, newValue);
            if (adminSimpleIsValidGenericText(newValue)) {
                userDataFields[DATA_TOWN_IDX] = newValue;
                changesMade = true; displayMessage("Town updated.", false);
            }
            else { displayMessage("Invalid Town.", true); }
            break;
        case 10:
            if (!dataParsed) { displayMessage("User data record not available for Postal Code.", true); break; }
            cout << "\t\t\t\t\t\tEnter new Postal Code (5 digits): "; getline(cin, newValue);
            if (adminSimpleIsValidPostalCode(newValue)) {
                userDataFields[DATA_POSTAL_IDX] = newValue;
                changesMade = true; displayMessage("Postal Code updated.", false);
            }
            else { displayMessage("Invalid Postal Code.", true); }
            break;
        case 11:
            cout << "\t\t\t\t\t\tEnter new Account Status ('" << ACTIVE_STATUS << "' Active, '" << PENDING_STATUS << "' Pending): "; getline(cin, newValue);
            if (newValue.length() == 1 && (newValue[0] == ACTIVE_STATUS || newValue[0] == PENDING_STATUS)) {
                userCredFields[USER_CRED_STATUS_IDX] = newValue;
                changesMade = true;
                displayMessage("Account Status updated.", false);
                if (newValue[0] == PENDING_STATUS) {
                    ofstream approvOut(APPROVAL_FILE, ios::app);
                    if (approvOut.is_open()) {
                        approvOut << cnicToModify << "-" << PENDING_STATUS << endl;
                        approvOut.close();
                    }
                }
                else {
                    string* approvLines = new string[MAX_FILE_LINES];
                    if (approvLines == NULL) { displayMessage("Memory error.", true); break; }
                    int approvCount = readFileToArray(APPROVAL_FILE, approvLines, MAX_FILE_LINES);
                    int approvUserIndex = -1;
                    for (int i = 0; i < approvCount; ++i) {
                        if (approvLines[i].substr(0, approvLines[i].find('-')) == cnicToModify) {
                            approvUserIndex = i; break;
                        }
                    }
                    if (approvUserIndex != -1) {
                        for (int i = approvUserIndex; i < approvCount - 1; ++i) approvLines[i] = approvLines[i + 1];
                        approvCount--;
                        writeArrayToFile(APPROVAL_FILE, approvLines, approvCount);
                    }
                    delete[] approvLines;
                }
            }
            else { displayMessage("Invalid status. Use '1' or '0'.", true); }
            break;
        case 12:
            cout << "\t\t\t\t\t\tIs user a Candidate overall? ('1' Yes, '0' No): "; getline(cin, newValue);
            if (newValue == "1" || newValue == "0") {
                userCredFields[USER_CRED_ISCANDIDATE_IDX] = newValue;
                if (newValue == "0") { 
                    userCredFields[USER_CRED_ISDEPENDENTCANDIDATE_IDX] = "0";
                    userCredFields[USER_CRED_ISPARTICIPATING_IDX] = "0";
                    userCredFields[USER_CRED_PARTICIPELECID_IDX] = "NONE";
                }
                else if (userCredFields[USER_CRED_PARTYID_IDX] != "NONE") {
                    userCredFields[USER_CRED_ISDEPENDENTCANDIDATE_IDX] = "1";
                }
                else { 
                    userCredFields[USER_CRED_ISDEPENDENTCANDIDATE_IDX] = "0";
                }
                changesMade = true; displayMessage("Overall Candidate Status updated.", false);
            }
            else { displayMessage("Invalid status. Use '1' or '0'.", true); }
            break;
        case 13:
            cout << "\t\t\t\t\t\tEnter new Party ID (4 digits or 'NONE'): "; getline(cin, newValue);
            if ((newValue.length() == 4 && containsOnlyDigits(newValue)) || newValue == "NONE") {
                if (newValue != "NONE") {
                    PoliticalParty tempParty; bool partyExists = false;
                    ifstream partiesFile(PARTIES_FILE_PATH); string pLine;
                    while (getline(partiesFile, pLine)) { if (pLine.substr(0, pLine.find('-')) == newValue) { partyExists = true; break; } }
                    partiesFile.close();
                    if (!partyExists) { displayMessage("Party ID " + newValue + " does not exist.", true); break; }
                }
                userCredFields[USER_CRED_PARTYID_IDX] = newValue;
                if (newValue == "NONE") {
                    userCredFields[USER_CRED_ISPRESIDENT_IDX] = "0";
                    userCredFields[USER_CRED_ISDEPENDENTCANDIDATE_IDX] = "0";
                }
                else {
                    if (userCredFields[USER_CRED_ISCANDIDATE_IDX] == "1") userCredFields[USER_CRED_ISDEPENDENTCANDIDATE_IDX] = "1";
                }
                changesMade = true; displayMessage("Party Affiliation updated.", false);
            }
            else { displayMessage("Invalid Party ID format.", true); }
            break;
        case 14:
            cout << "\t\t\t\t\t\tIs user a Party Dependent Candidate? ('1' Yes, '0' No): "; getline(cin, newValue);
            if (newValue == "1" || newValue == "0") {
                if (newValue == "1" && (userCredFields[USER_CRED_PARTYID_IDX] == "NONE" || userCredFields[USER_CRED_ISCANDIDATE_IDX] == "0")) {
                    displayMessage("Cannot be dependent without party or if not candidate overall.", true);
                }
                else {
                    userCredFields[USER_CRED_ISDEPENDENTCANDIDATE_IDX] = newValue;
                    changesMade = true; displayMessage("Dependent Candidate Status updated.", false);
                }
            }
            else { displayMessage("Invalid status. Use '1' or '0'.", true); }
            break;
        case 15:
            cout << "\t\t\t\t\t\tIs user Participating in an Election as Candidate? ('1' Yes, '0' No): "; getline(cin, newValue);
            if (newValue == "1" || newValue == "0") {
                if (newValue == "1" && userCredFields[USER_CRED_ISCANDIDATE_IDX] == "0") {
                    displayMessage("Cannot participate if not an overall candidate.", true);
                }
                else {
                    userCredFields[USER_CRED_ISPARTICIPATING_IDX] = newValue;
                    if (newValue == "0") userCredFields[USER_CRED_PARTICIPELECID_IDX] = "NONE";
                    changesMade = true; displayMessage("Election Participation Status updated.", false);
                }
            }
            else { displayMessage("Invalid status. Use '1' or '0'.", true); }
            break;
        case 16:
            cout << "\t\t\t\t\t\tEnter Election ID participating in (or 'NONE'): "; getline(cin, newValue);
            if (newValue == "NONE" || (newValue.length() == 4 && containsOnlyDigits(newValue))) { // Basic ID format check
                if (newValue != "NONE" && userCredFields[USER_CRED_ISPARTICIPATING_IDX] == "0") {
                    displayMessage("User must be set to 'Participating' to assign an Election ID.", true);
                }
                else if (newValue == "NONE" && userCredFields[USER_CRED_ISPARTICIPATING_IDX] == "1") {
                    displayMessage("To remove Election ID, first set Participation Status to '0'.", true);
                }
                else {
                    userCredFields[USER_CRED_PARTICIPELECID_IDX] = newValue;
                    changesMade = true; displayMessage("Participating Election ID updated.", false);
                }
            }
            else { displayMessage("Invalid Election ID format ('NONE' or 4 digits).", true); }
            break;
        case 99:
            if (changesMade) {
                credLines[credModifyIndex] = adminConstructLine(userCredFields, USER_CRED_FIELD_COUNT, '-');
                if (!writeArrayToFile(USER_CREDENTIALS_FILE_PATH, credLines, credCount)) {
                    displayMessage("Failed to save credentials file.", true);
                }
                if (dataParsed && dataModifyIndex != -1) {
                    userDataFields[DATA_TIMESTAMP_IDX] = DateTime::getCurrentDateTime().toString();
                    dataLines[dataModifyIndex] = adminConstructLine(userDataFields, DATA_FIELD_COUNT, '-');
                    if (!writeArrayToFile(USER_DATA_FILE_PATH, dataLines, dataCount)) {
                        displayMessage("Failed to save user data file.", true);
                    }
                }
                displayMessage("Changes saved successfully.", false);
            }
            else {
                displayMessage("No changes were made.", false);
            }
            delete[] credLines; delete[] dataLines;
            return;
        case 0:
            displayMessage("Changes discarded.", false);
            delete[] credLines; delete[] dataLines;
            return;
        default:
            invalidEntry();
            break;
        }
    }
}

string formatDisplayLine(const string& text, int totalWidth = 70) {
    int availableSpace = totalWidth - 4;
    string content = text;
    if ((int)content.length() > availableSpace) {
        content = content.substr(0, availableSpace);
    }
    content += string(availableSpace - content.length(), ' ');
    return content;
}

void Admin::searchUser() {
    system("cls");
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t..........................S E A R C H   U S E R.......................\n";
    cout << "\t\t\t\t\t\t......................................................................\n";
    string cnicToSearch;
    cout << "\t\t\t\t\t\tEnter CNIC of the user to search (13 digits): ";
    getline(cin, cnicToSearch);

    if (!isValidCnicForDelete(cnicToSearch)) {
        return;
    }

    string* credLines = new string[MAX_FILE_LINES];
    string* dataLines = new string[MAX_FILE_LINES];

    if (credLines == NULL || dataLines == NULL) {
        displayMessage("Critical Error: Memory allocation failed for search.", true);
        if (credLines != NULL) delete[] credLines;
        if (dataLines != NULL) delete[] dataLines;
        return;
    }

    int credCount = readFileToArray(USER_CREDENTIALS_FILE_PATH, credLines, MAX_FILE_LINES);
    int dataCount = readFileToArray(USER_DATA_FILE_PATH, dataLines, MAX_FILE_LINES);

    int credFoundIndex = -1;
    for (int i = 0; i < credCount; ++i) {
        if (credLines[i].substr(0, credLines[i].find('-')) == cnicToSearch) {
            credFoundIndex = i;
            break;
        }
    }

    int dataFoundIndex = -1;
    for (int i = 0; i < dataCount; ++i) {
        if (dataLines[i].substr(0, dataLines[i].find('-')) == cnicToSearch) {
            dataFoundIndex = i;
            break;
        }
    }

    system("cls");
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t......................S E A R C H   R E S U L T S.....................\n";
    cout << "\t\t\t\t\t\t......................................................................\n";

    if (credFoundIndex == -1 && dataFoundIndex == -1) {
        cout << "\t\t\t\t\t\t.." << formatDisplayLine("      User with CNIC (" + cnicToSearch + ") not found.") << "..\n";
    }
    else {
        cout << "\t\t\t\t\t\t.." << formatDisplayLine("      User CNIC: " + cnicToSearch) << "..\n";
        cout << "\t\t\t\t\t\t......................................................................\n";
        cout << "\t\t\t\t\t\t.." << formatDisplayLine("                   --- Credentials Information ---") << "..\n";

        if (credFoundIndex != -1) {
            string userCredFields[USER_CRED_FIELD_COUNT];
            if (adminParseLine(credLines[credFoundIndex], '-', userCredFields, USER_CRED_FIELD_COUNT) == USER_CRED_FIELD_COUNT) {
                cout << "\t\t\t\t\t\t.." << formatDisplayLine("      Password: " + userCredFields[USER_CRED_PASS_IDX]) << "..\n";
                string accStatusStr = "Unknown";
                if (userCredFields[USER_CRED_STATUS_IDX].length() > 0) {
                    if (userCredFields[USER_CRED_STATUS_IDX][0] == ACTIVE_STATUS) accStatusStr = "   Account is Active.";
                    else if (userCredFields[USER_CRED_STATUS_IDX][0] == PENDING_STATUS) accStatusStr = "   Account is Pending Approval.";
                }
                cout << "\t\t\t\t\t\t.." << formatDisplayLine("   " + accStatusStr) << "..\n";

                string overallCandStatus = (userCredFields[USER_CRED_ISCANDIDATE_IDX] == "1") ? "Yes" : "No";
                cout << "\t\t\t\t\t\t.." << formatDisplayLine("   Candidate Overall: " + overallCandStatus) << "..\n";
                string partyAffiliation = "   Not affiliated.";
                if (userCredFields[USER_CRED_PARTYID_IDX] != "NONE") {
                    partyAffiliation = "   Party ID: " + userCredFields[USER_CRED_PARTYID_IDX];
                    if (userCredFields[USER_CRED_ISPRESIDENT_IDX] == "1") partyAffiliation += " (President)";
                }
                cout << "\t\t\t\t\t\t.." << formatDisplayLine(partyAffiliation) << "..\n";
                string depCandStatus = (userCredFields[USER_CRED_ISDEPENDENTCANDIDATE_IDX] == "1") ? "Yes" : "No";
                cout << "\t\t\t\t\t\t.." << formatDisplayLine("   Dependent Candidate: " + depCandStatus) << "..\n";
                string elecPartStatus = (userCredFields[USER_CRED_ISPARTICIPATING_IDX] == "1") ? "Yes" : "No";
                cout << "\t\t\t\t\t\t.." << formatDisplayLine("   Participating in Election: " + elecPartStatus) << "..\n";
                if (userCredFields[USER_CRED_ISPARTICIPATING_IDX] == "1") {
                    cout << "\t\t\t\t\t\t.." << formatDisplayLine("   Participating Election ID: " + userCredFields[USER_CRED_PARTICIPELECID_IDX]) << "..\n";
                }

            }
            else {
                cout << "\t\t\t\t\t\t.." << formatDisplayLine("   Error parsing credentials data.") << "..\n";
            }
        }
        else {
            cout << "\t\t\t\t\t\t.." << formatDisplayLine("   No credentials record found.") << "..\n";
        }
        cout << "\t\t\t\t\t\t......................................................................\n";
        cout << "\t\t\t\t\t\t.." << formatDisplayLine("                   --- Profile Data Information ---") << "..\n";

        if (dataFoundIndex != -1) {
            string userDataFields[DATA_FIELD_COUNT];
            if (adminParseLine(dataLines[dataFoundIndex], '-', userDataFields, DATA_FIELD_COUNT) == DATA_FIELD_COUNT) {
                cout << "\t\t\t\t\t\t.." << formatDisplayLine("      Full Name: " + userDataFields[DATA_FNAME_IDX] + " " + userDataFields[DATA_LNAME_IDX]) << "..\n";
                cout << "\t\t\t\t\t\t.." << formatDisplayLine("      Age: " + userDataFields[DATA_AGE_IDX] + " years") << "..\n";
                cout << "\t\t\t\t\t\t.." << formatDisplayLine("      Phone: " + userDataFields[DATA_PHONE_IDX]) << "..\n";
                cout << "\t\t\t\t\t\t.." << formatDisplayLine("      Gender: " + userDataFields[DATA_GENDER_IDX]) << "..\n";
                cout << "\t\t\t\t\t\t.." << formatDisplayLine("      Address: " + userDataFields[DATA_TOWN_IDX] + ", " + userDataFields[DATA_CITY_IDX] + ", " + userDataFields[DATA_STATE_IDX]) << "..\n";
                cout << "\t\t\t\t\t\t.." << formatDisplayLine("      Postal: " + userDataFields[DATA_POSTAL_IDX]) << "..\n";
                cout << "\t\t\t\t\t\t.." << formatDisplayLine("      Last Update/Signup: " + userDataFields[DATA_TIMESTAMP_IDX]) << "..\n";
            }
            else {
                cout << "\t\t\t\t\t\t.." << formatDisplayLine("   Error parsing profile data.") << "..\n";
            }
        }
        else {
            cout << "\t\t\t\t\t\t.." << formatDisplayLine("   No profile data record found.") << "..\n";
        }
    }
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\tPress Enter to continue...";
    string temp;
    getline(cin, temp);

    delete[] credLines;
    delete[] dataLines;
}

void Admin::listAllUsers() {
    system("cls");
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t........................L I S T   A L L   U S E R S...................\n";
    cout << "\t\t\t\t\t\t......................................................................\n";

    string* credLines = new string[MAX_FILE_LINES];
    string* dataLinesBuffer = new string[MAX_FILE_LINES];

    if (credLines == NULL || dataLinesBuffer == NULL) {
        displayMessage("Critical Error: Memory allocation failed.", true);
        if (credLines != NULL) delete[] credLines;
        if (dataLinesBuffer != NULL) delete[] dataLinesBuffer;
        return;
    }

    int credCount = readFileToArray(USER_CREDENTIALS_FILE_PATH, credLines, MAX_FILE_LINES);
    int dataBufferCount = readFileToArray(USER_DATA_FILE_PATH, dataLinesBuffer, MAX_FILE_LINES);


    cout << "\t\t\t\t\t\t.." << formatDisplayLine("                      Total Users Found: " + to_string(credCount)) << "..\n";
    cout << "\t\t\t\t\t\t......................................................................\n";

    if (credCount == 0) {
        cout << "\t\t\t\t\t\t.." << formatDisplayLine("No users registered in the system.") << "..\n";
    }
    else {
        for (int i = 0; i < credCount; ++i) {
            cout << "\t\t\t\t\t\t.." << formatDisplayLine("                      Information for User " + to_string(i + 1) + ":") << "..\n";
            cout << "\t\t\t\t\t\t......................................................................\n";

            string currentCredLine = credLines[i];
            string userCredFields[USER_CRED_FIELD_COUNT];
            string userCnic = "N/A";

            if (adminParseLine(currentCredLine, '-', userCredFields, USER_CRED_FIELD_COUNT) >= 1) {
                userCnic = userCredFields[USER_CRED_CNIC_IDX];
                cout << "\t\t\t\t\t\t.." << formatDisplayLine("  CNIC: " + userCnic) << "..\n";

                if (adminParseLine(currentCredLine, '-', userCredFields, USER_CRED_FIELD_COUNT) == USER_CRED_FIELD_COUNT) {
                    cout << "\t\t\t\t\t\t.." << formatDisplayLine("  Full Name (creds): " + userCredFields[USER_CRED_FNAME_IDX] + " " + userCredFields[USER_CRED_LNAME_IDX]) << "..\n";
                    cout << "\t\t\t\t\t\t.." << formatDisplayLine("  Password: " + userCredFields[USER_CRED_PASS_IDX]) << "..\n";
                    string accStatusStr = (userCredFields[USER_CRED_STATUS_IDX] == "1") ? "Active" : "Pending";
                    cout << "\t\t\t\t\t\t.." << formatDisplayLine("  Account Status: " + accStatusStr) << "..\n";
                    string overallCand = (userCredFields[USER_CRED_ISCANDIDATE_IDX] == "1") ? "Yes" : "No";
                    cout << "\t\t\t\t\t\t.." << formatDisplayLine("  Cand. Overall: " + overallCand) << "..\n";
                    string partyAff = (userCredFields[USER_CRED_PARTYID_IDX] == "NONE") ? "None" : userCredFields[USER_CRED_PARTYID_IDX];
                    if (userCredFields[USER_CRED_ISPRESIDENT_IDX] == "1") partyAff += " (Pres)";
                    cout << "\t\t\t\t\t\t.." << formatDisplayLine("  Party ID: " + partyAff) << "..\n";
                    string depCand = (userCredFields[USER_CRED_ISDEPENDENTCANDIDATE_IDX] == "1") ? "Yes" : "No";
                    cout << "\t\t\t\t\t\t.." << formatDisplayLine("  Dependent Cand: " + depCand) << "..\n";
                    string partElec = (userCredFields[USER_CRED_ISPARTICIPATING_IDX] == "1") ? ("Yes (ID: " + userCredFields[USER_CRED_PARTICIPELECID_IDX] + ")") : "No";
                    cout << "\t\t\t\t\t\t.." << formatDisplayLine("  Particip. Election: " + partElec) << "..\n";
                }
                else {
                    cout << "\t\t\t\t\t\t.." << formatDisplayLine("  Error parsing full credentials.") << "..\n";
                }
            }
            else {
                cout << "\t\t\t\t\t\t.." << formatDisplayLine("  Error: Could not parse CNIC from credentials.") << "..\n";
            }

            bool dataRecordFound = false;
            for (int j = 0; j < dataBufferCount; ++j) {
                if (dataLinesBuffer[j].substr(0, dataLinesBuffer[j].find('-')) == userCnic) {
                    string userDataFields[DATA_FIELD_COUNT];
                    if (adminParseLine(dataLinesBuffer[j], '-', userDataFields, DATA_FIELD_COUNT) == DATA_FIELD_COUNT) {
                        cout << "\t\t\t\t\t\t.." << formatDisplayLine("  Full Name (data): " + userDataFields[DATA_FNAME_IDX] + " " + userDataFields[DATA_LNAME_IDX]) << "..\n";
                        cout << "\t\t\t\t\t\t.." << formatDisplayLine("  Age: " + userDataFields[DATA_AGE_IDX]) << "..\n";
                        cout << "\t\t\t\t\t\t.." << formatDisplayLine("  Phone: " + userDataFields[DATA_PHONE_IDX]) << "..\n";
                        cout << "\t\t\t\t\t\t.." << formatDisplayLine("  Gender: " + userDataFields[DATA_GENDER_IDX]) << "..\n";
                        cout << "\t\t\t\t\t\t.." << formatDisplayLine("  Address: " + userDataFields[DATA_TOWN_IDX] + ", " + userDataFields[DATA_CITY_IDX] + ", " + userDataFields[DATA_STATE_IDX]) << "..\n";
                        cout << "\t\t\t\t\t\t.." << formatDisplayLine("  Postal: " + userDataFields[DATA_POSTAL_IDX]) << "..\n";
                        cout << "\t\t\t\t\t\t.." << formatDisplayLine("  Record Timestamp: " + userDataFields[DATA_TIMESTAMP_IDX]) << "..\n";
                    }
                    else {
                        cout << "\t\t\t\t\t\t.." << formatDisplayLine("    Error parsing profile data.") << "..\n";
                    }
                    dataRecordFound = true;
                    break;
                }
            }
            if (!dataRecordFound && userCnic != "N/A") {
                cout << "\t\t\t\t\t\t.." << formatDisplayLine("  No profile data record found.") << "..\n";
            }
            cout << "\t\t\t\t\t\t......................................................................\n";
        }
    }

    cout << "\t\t\t\t\t\tPress Enter to go Back...";
    string temp;
    getline(cin, temp);

    delete[] credLines;
    delete[] dataLinesBuffer;
}


string generateUniqueElectionIDAdmin() {
    srand(time(0));
    string newID;
    bool idIsUnique;
    int attempts = 0;
    const int MAX_ID_ATTEMPTS = 1000;

    do {
        idIsUnique = true;
        int num = 1000 + (rand() % 9000);
        newID = to_string(num);

        ifstream inFile(ELECTIONS_FILE_PATH);
        if (inFile.is_open()) {
            string line;
            string tempId, tempType, tempStatus, tempEndDate, tempScope;
            while (getline(inFile, line)) {
                if (Election::parseElectionLine(line, tempId, tempType, tempStatus, tempEndDate, tempScope)) {
                    if (tempId == newID) {
                        idIsUnique = false;
                        break;
                    }
                }
            }
            inFile.close();
        }
        attempts++;
        if (attempts > MAX_ID_ATTEMPTS && !idIsUnique) {
            displayMessage("Error: Could not generate a unique election ID.", true);
            return "ERR_ID";
        }
    } while (!idIsUnique);
    return newID;
}

void Admin::displayElectionManagementMenu() {
    system("cls");
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t................E L E C T I O N   M A N A G E M E N T.................\n";
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t..                                                                  ..\n";
    cout << "\t\t\t\t\t\t..                       '1' Create Election                        ..\n";
    cout << "\t\t\t\t\t\t..                       '2' Modify Election (Upcoming Only)        ..\n";
    cout << "\t\t\t\t\t\t..                       '3' Delete Election (Upcoming Only)        ..\n";
    cout << "\t\t\t\t\t\t..                       '4' List All Elections                     ..\n";
    cout << "\t\t\t\t\t\t..                       '5' Manage Lifecycle (Start/End/Cancel)    ..\n";
    cout << "\t\t\t\t\t\t..                       '6' View Election Results (Admin)          ..\n";
    cout << "\t\t\t\t\t\t..                       '0' Go Back (Admin Dashboard)              ..\n";
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t                      Enter your choice here: ";
}

void Admin::electionManagement() {
    string entry;
    int choice;
    while (true) {
        displayElectionManagementMenu();
        getline(cin, entry, '\n');

        if (entry.length() == 1 && isdigit(entry.at(0))) {
            try { choice = stoi(entry); }
            catch (...) { invalidEntry(); continue; }

            switch (choice) {
            case 1: createElection(); break;
            case 2: modifyElection(); break;
            case 3: deleteElection(); break;
            case 4: listAllElections(); break;
            case 5: manageElectionLifecycle(); break;
            case 6: viewElectionResultsAdmin(); break;
            case 0: return;
            default: invalidEntry(); break;
            }
        }
        else { invalidEntry(); }
    }
}

void Admin::createElection() {
    system("cls");
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t................... C R E A T E   E L E C T I O N ....................\n";
    cout << "\t\t\t\t\t\t......................................................................\n";

    string electionTypeStr, scopeInfo;
    int typeChoiceNum;

    while (true) {
        cout << "\t\t\t\t\t\tSelect Election Type:\n";
        cout << "\t\t\t\t\t\t  1. National\n  2. Regional\n  3. Metropolitan\n  4. Local\n";
        cout << "\t\t\t\t\t\tEnter choice (1-4): ";
        string choiceInput;
        getline(cin, choiceInput);
        if (choiceInput.length() == 1 && isdigit(choiceInput[0])) {
            typeChoiceNum = choiceInput[0] - '0';
            if (typeChoiceNum >= 1 && typeChoiceNum <= 4) {
                if (typeChoiceNum == 1) electionTypeStr = "National";
                else if (typeChoiceNum == 2) electionTypeStr = "Regional";
                else if (typeChoiceNum == 3) electionTypeStr = "Metropolitan";
                else if (typeChoiceNum == 4) electionTypeStr = "Local";
                break;
            }
        }
        invalidEntry();
    }

    if (electionTypeStr == "Regional") {
        cout << "\t\t\t\t\t\tEnter Region/Province Name: "; getline(cin, scopeInfo);
        while (scopeInfo.empty() || scopeInfo.find('-') != string::npos || scopeInfo.find(',') != string::npos) {
            displayMessage("Invalid scope.", true); cout << "\t\t\t\t\t\tEnter Region/Province Name: "; getline(cin, scopeInfo);
        }
    }
    else if (electionTypeStr == "Metropolitan") {
        cout << "\t\t\t\t\t\tEnter City Name: "; getline(cin, scopeInfo);
        while (scopeInfo.empty() || scopeInfo.find('-') != string::npos || scopeInfo.find(',') != string::npos) {
            displayMessage("Invalid scope.", true); cout << "\t\t\t\t\t\tEnter City Name: "; getline(cin, scopeInfo);
        }
    }
    else if (electionTypeStr == "Local") {
        string town, city;
        cout << "\t\t\t\t\t\tEnter Town Name: "; getline(cin, town);
        while (town.empty() || town.find('-') != string::npos || town.find(',') != string::npos) {
            displayMessage("Invalid town.", true); cout << "\t\t\t\t\t\tEnter Town Name: "; getline(cin, town);
        }
        cout << "\t\t\t\t\t\tEnter City Name (for context): "; getline(cin, city);
        while (city.empty() || city.find('-') != string::npos || city.find(',') != string::npos) {
            displayMessage("Invalid city.", true); cout << "\t\t\t\t\t\tEnter City Name (for context): "; getline(cin, city);
        }
        scopeInfo = town + "," + city;
    }
    else {
        scopeInfo = "NATIONAL";
    }

    ifstream checkFile(ELECTIONS_FILE_PATH);
    if (checkFile.is_open()) {
        string line;
        string id_check, type_check, status_check, endDate_check, scope_check;
        while (getline(checkFile, line)) {
            if (Election::parseElectionLine(line, id_check, type_check, status_check, endDate_check, scope_check)) {
                if (type_check == electionTypeStr && (status_check == "Upcoming" || status_check == "Ongoing")) {
                    if (electionTypeStr == "National") {
                        displayMessage("An '" + electionTypeStr + "' election is already Upcoming or Ongoing.", true);
                        checkFile.close();
                        return;
                    }
                    else if (scope_check == scopeInfo) {
                        displayMessage("An '" + electionTypeStr + "' election for scope '" + scopeInfo + "' is already Upcoming or Ongoing.", true);
                        checkFile.close();
                        return;
                    }
                }
            }
        }
        checkFile.close();
    }

    string electionID = generateUniqueElectionIDAdmin();
    if (electionID == "ERR_ID") {
        displayMessage("Failed to generate Election ID. Aborting.", true);
        return;
    }
    string statusFromFile = "Upcoming";
    string endDateTimeStr = "N/A";

    ofstream outFile(ELECTIONS_FILE_PATH, ios::app);
    if (!outFile.is_open()) {
        displayMessage("Error: Could not open elections file.", true);
        return;
    }
    outFile << electionID << "-" << electionTypeStr << "-" << statusFromFile << "-" << endDateTimeStr << "-" << scopeInfo << endl;
    outFile.close();

    displayMessage("Election ID " + electionID + " created successfully. Status: Upcoming", false);
}


void Admin::modifyElection() {
    system("cls");
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t................... M O D I F Y   E L E C T I O N ....................\n";
    cout << "\t\t\t\t\t\t............(Only Type and Scope of 'Upcoming' elections).............\n";
    cout << "\t\t\t\t\t\t......................................................................\n";
    listAllElections();
    cout << "\t\t\t\t\t\t......................................................................\n";

    string idToModify;
    cout << "\t\t\t\t\t\tEnter Election ID to modify: ";
    getline(cin, idToModify);

    string fileLines[MAX_FILE_LINES];
    int lineCount = readFileToArray(ELECTIONS_FILE_PATH, fileLines, MAX_FILE_LINES);
    int modifyIndex = -1;
    string currentId, currentType, currentStatus, currentEndDateStr, currentScope;

    for (int i = 0; i < lineCount; ++i) {
        if (Election::parseElectionLine(fileLines[i], currentId, currentType, currentStatus, currentEndDateStr, currentScope)) {
            if (currentId == idToModify) {
                modifyIndex = i;
                break;
            }
        }
    }

    if (modifyIndex == -1) {
        displayMessage("Election ID not found.", true);
        return;
    }

    if (currentStatus != "Upcoming") {
        displayMessage("Can only modify Type/Scope of 'Upcoming' elections.", true);
        return;
    }

    cout << "\t\t\t\t\t\tModifying Election ID: " << currentId << " (Status: Upcoming)\n";
    cout << "\t\t\t\t\t\tCurrent Type: " << currentType << "\n";
    cout << "\t\t\t\t\t\tCurrent Scope: " << currentScope << "\n";
    cout << "\t\t\t\t\t\t......................................................................\n";

    string newType = currentType;
    string newScope = currentScope;
    bool changesMade = false;
    string choice;

    cout << "\t\t\t\t\t\tModify Election Type and Scope? (y/n): ";
    getline(cin, choice);
    if (choice == "y" || choice == "Y") {
        while (true) {
            cout << "\t\t\t\t\t\tSelect NEW Election Type:\n";
            cout << "\t\t\t\t\t\t  1. National\n";
            cout << "\t\t\t\t\t\t  2. Regional\n";
            cout << "\t\t\t\t\t\t  3. Metropolitan\n";
            cout << "\t\t\t\t\t\t  4. Local\n";
            cout << "\t\t\t\t\t\tEnter choice (1-4): ";
            string typeChoiceInput;
            getline(cin, typeChoiceInput);
            int typeChoiceNum = -1;
            if (typeChoiceInput.length() == 1 && isdigit(typeChoiceInput[0])) typeChoiceNum = typeChoiceInput[0] - '0';

            if (typeChoiceNum >= 1 && typeChoiceNum <= 4) {
                if (typeChoiceNum == 1) newType = "National";
                else if (typeChoiceNum == 2) newType = "Regional";
                else if (typeChoiceNum == 3) newType = "Metropolitan";
                else if (typeChoiceNum == 4) newType = "Local";
                break;
            }
            else { invalidEntry(); }
        }

        string tempScopeInput;
        if (newType == "Regional") {
            cout << "\t\t\t\t\t\tEnter NEW Region/Province Name: "; getline(cin, tempScopeInput);
            while (tempScopeInput.empty() || tempScopeInput.find('-') != string::npos || tempScopeInput.find(',') != string::npos) {
                displayMessage("Invalid scope.", true); cout << "\t\t\t\t\t\tEnter NEW Region/Province Name: "; getline(cin, tempScopeInput);
            }
        }
        else if (newType == "Metropolitan") {
            cout << "\t\t\t\t\t\tEnter NEW City Name: "; getline(cin, tempScopeInput);
            while (tempScopeInput.empty() || tempScopeInput.find('-') != string::npos || tempScopeInput.find(',') != string::npos) {
                displayMessage("Invalid scope.", true); cout << "\t\t\t\t\t\tEnter NEW City Name: "; getline(cin, tempScopeInput);
            }
        }
        else if (newType == "Local") {
            string town, city;
            cout << "\t\t\t\t\t\tEnter NEW Town Name: "; getline(cin, town);
            while (town.empty() || town.find('-') != string::npos || town.find(',') != string::npos) {
                displayMessage("Invalid town.", true); cout << "\t\t\t\t\t\tEnter NEW Town Name: "; getline(cin, town);
            }
            cout << "\t\t\t\t\t\tEnter NEW City Name (context): "; getline(cin, city);
            while (city.empty() || city.find('-') != string::npos || city.find(',') != string::npos) {
                displayMessage("Invalid city.", true); cout << "\t\t\t\t\t\tEnter NEW City Name (context): "; getline(cin, city);
            }
            tempScopeInput = town + "," + city;
        }
        else {
            tempScopeInput = "NATIONAL";
        }
        newScope = tempScopeInput;
        changesMade = true;
    }

    if (changesMade) {
        fileLines[modifyIndex] = currentId + "-" + newType + "-" + currentStatus + "-" + currentEndDateStr + "-" + newScope;
        if (writeArrayToFile(ELECTIONS_FILE_PATH, fileLines, lineCount)) {
            displayMessage("Election " + currentId + " modified successfully.", false);
        }
        else {
            displayMessage("Error: Failed to save modifications.", true);
        }
    }
    else {
        displayMessage("No changes were made.", false);
    }
}


void Admin::listAllElections() {
    system("cls");
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t................. L I S T   A L L   E L E C T I O N S ................\n";
    cout << "\t\t\t\t\t\t......................................................................\n";

    ifstream electionsFile(ELECTIONS_FILE_PATH);
    if (!electionsFile.is_open()) {
        displayMessage("Error: Elections file not found.", true); return;
    }
    string line;
    int count = 0;
    string id, type, statusFromFile, endDateTimeStr, scope;

    while (getline(electionsFile, line)) {
        if (line.empty()) continue;

        if (Election::parseElectionLine(line, id, type, statusFromFile, endDateTimeStr, scope)) {
            count++;
            cout << "\t\t\t\t\t\tElection " << count << ":\n";
            cout << "\t\t\t\t\t\t  ID: " << id << "\n";
            cout << "\t\t\t\t\t\t  Type: " << type << "\n";
            cout << "\t\t\t\t\t\t  Status: " << statusFromFile << "\n";
            if (endDateTimeStr != "N/A") {
                cout << "\t\t\t\t\t\t  Ends:   " << endDateTimeStr << "\n";
            }
            else {
                cout << "\t\t\t\t\t\t  Ends:   Not Started Yet\n";
            }
            if (!scope.empty()) {
                if (type == "National") cout << "\t\t\t\t\t\t  Scope: Nationwide\n";
                else if (type == "Local" && scope.find(',') != string::npos) cout << "\t\t\t\t\t\t  Scope: Town=" << scope.substr(0, scope.find(',')) << ", City=" << scope.substr(scope.find(',') + 1) << "\n";
                else cout << "\t\t\t\t\t\t  Scope: " << scope << "\n";
            }
            else {
                cout << "\t\t\t\t\t\t  Scope: Unknown\n";
            }
            cout << "\t\t\t\t\t\t......................................................................\n";
        }
        else {
            cout << "\t\t\t\t\t\tWarning: Malformed election line skipped: " << line.substr(0, 50) << "...\n";
            cout << "\t\t\t\t\t\t......................................................................\n";
        }
    }
    electionsFile.close();

    if (count == 0) {
        cout << "\t\t\t\t\t\t..                   No elections found in the system.              ..\n";
        cout << "\t\t\t\t\t\t......................................................................\n";
    }
    cout << "\t\t\t\t\t\tPress Enter to continue...";
    string temp;
    getline(cin, temp);
}


void Admin::manageElectionLifecycle() {
    system("cls");
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t.......... M A N A G E   E L E C T I O N   L I F E C Y C L E .........\n";
    cout << "\t\t\t\t\t\t......................................................................\n";
    listAllElections();
    cout << "\t\t\t\t\t\t......................................................................\n";

    string idToManage;
    cout << "\t\t\t\t\t\tEnter Election ID to manage (Start/End/Cancel): ";
    getline(cin, idToManage);

    string fileLines[MAX_FILE_LINES];
    int lineCount = readFileToArray(ELECTIONS_FILE_PATH, fileLines, MAX_FILE_LINES);
    int manageIndex = -1;
    string electionID, type, currentStatusFromFile, currentEndDateStr, scopeInfo;
    bool found = false;

    for (int i = 0; i < lineCount; ++i) {
        if (Election::parseElectionLine(fileLines[i], electionID, type, currentStatusFromFile, currentEndDateStr, scopeInfo)) {
            if (electionID == idToManage) {
                manageIndex = i;
                found = true;
                break;
            }
        }
    }

    if (!found) {
        displayMessage("Election ID not found.", true);
        return;
    }

    cout << "\t\t\t\t\t\tManaging Election: " << electionID << " (" << type << ")\n";
    cout << "\t\t\t\t\t\tCurrent Status: " << currentStatusFromFile << ", Current End Date: " << currentEndDateStr << "\n";
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\tSelect Action:\n";
    cout << "\t\t\t\t\t\t  1. START Election (Upcoming -> Ongoing)\n";
    cout << "\t\t\t\t\t\t  2. END Election & PUBLISH RESULTS (Ongoing -> Completed)\n";
    cout << "\t\t\t\t\t\t  3. CANCEL Election (Upcoming/Ongoing -> Cancelled)\n";
    cout << "\t\t\t\t\t\t  0. Go Back\n";
    cout << "\t\t\t\t\t\tEnter choice: ";
    string choiceInput;
    getline(cin, choiceInput);

    string newStatus = currentStatusFromFile;
    string finalEndDateStr = currentEndDateStr;
    bool triggerPublishResults = false;

    if (choiceInput == "1") {
        if (currentStatusFromFile != "Upcoming") {
            displayMessage("Can only START an 'Upcoming' election.", true); return;
        }
        newStatus = "Ongoing";

        cout << "\t\t\t\t\t\tStarting Election " << electionID << ". Set the END Date & Time.\n";
        DateTime newEndDT;
        bool validEndTime = false;
        while (!validEndTime) {
            cout << "\t\t\t\t\t\tEnter END Date & Time (YYYY-MM-DD HH:MM:SS): ";
            string endTimeInput;
            getline(cin, endTimeInput);
            newEndDT = Election::parseDateTimeString(endTimeInput);
            if (newEndDT.isObjectValid() && newEndDT > DateTime::getCurrentDateTime()) {
                validEndTime = true;
                finalEndDateStr = newEndDT.toString();
            }
            else {
                displayMessage("Invalid END time or not in the future.", true);
            }
        }
        cout << "\t\t\t\t\t\tElection END time set to: " << finalEndDateStr << "\n"; Sleep(1000);

    }
    else if (choiceInput == "2") {
        if (currentStatusFromFile != "Ongoing") {
            displayMessage("Can only END an 'Ongoing' election.", true); return;
        }
        DateTime endDT = Election::parseDateTimeString(currentEndDateStr);
        if (endDT.isObjectValid() && DateTime::getCurrentDateTime() < endDT) {
            cout << "\t\t\t\t\t\tWarning: Marking completed before set end time (" << currentEndDateStr << ").\n";
            cout << "\t\t\t\t\t\tProceed? (y/n): ";
            string confirm; getline(cin, confirm);
            if (confirm != "y" && confirm != "Y") {
                displayMessage("Action cancelled.", false); return;
            }
        }
        newStatus = "Completed";
        triggerPublishResults = true;

    }
    else if (choiceInput == "3") {
        if (currentStatusFromFile == "Completed" || currentStatusFromFile == "Cancelled") {
            displayMessage("Cannot CANCEL an election that is already '" + currentStatusFromFile + "'.", true); return;
        }
        newStatus = "Cancelled";
        finalEndDateStr = "N/A";

        string userLines[MAX_FILE_LINES];
        int userCount = readFileToArray(USER_CREDENTIALS_FILE_PATH, userLines, MAX_FILE_LINES);
        bool credentialsUpdated = false;
        for (int i = 0; i < userCount; ++i) {
            string userFields[USER_CRED_FIELD_COUNT];
            if (adminParseLine(userLines[i], '-', userFields, USER_CRED_FIELD_COUNT) == USER_CRED_FIELD_COUNT) {
                if (userFields[USER_CRED_PARTICIPELECID_IDX] == electionID) {
                    userFields[USER_CRED_ISPARTICIPATING_IDX] = "0";
                    userFields[USER_CRED_PARTICIPELECID_IDX] = "NONE";
                    userLines[i] = adminConstructLine(userFields, USER_CRED_FIELD_COUNT, '-');
                    credentialsUpdated = true;
                }
            }
        }
        if (credentialsUpdated) {
            if (writeArrayToFile(USER_CREDENTIALS_FILE_PATH, userLines, userCount)) {
                displayMessage("Candidate participation status reset for cancelled election.", false);
            }
            else {
                displayMessage("ERROR: Failed to reset candidate participation status!", true);
            }
        }
    }
    else if (choiceInput == "0") {
        displayMessage("No changes made.", false);
        return;
    }
    else {
        invalidEntry();
        return;
    }

    fileLines[manageIndex] = electionID + "-" + type + "-" + newStatus + "-" + finalEndDateStr + "-" + scopeInfo;
    if (writeArrayToFile(ELECTIONS_FILE_PATH, fileLines, lineCount)) {
        displayMessage("Election " + electionID + " status updated to '" + newStatus + "'.", false);

        if (triggerPublishResults) {
            displayMessage("Calculating and publishing results for Election ID: " + electionID, false);
            Sleep(1500);
            Election* electionToPublish = NULL;
            if (type == "National") electionToPublish = new NationalElection(electionID, newStatus);
            else if (type == "Regional") electionToPublish = new RegionalElection(electionID, newStatus, scopeInfo);
            else if (type == "Metropolitan") electionToPublish = new MetropolitanElection(electionID, newStatus, scopeInfo);
            else if (type == "Local") {
                size_t comma_pos = scopeInfo.find(',');
                if (comma_pos != string::npos) electionToPublish = new LocalElection(electionID, newStatus, scopeInfo.substr(0, comma_pos), scopeInfo.substr(comma_pos + 1));
            }

            if (electionToPublish) {
                if (finalEndDateStr != "N/A") electionToPublish->setEndDateTime(Election::parseDateTimeString(finalEndDateStr));
                electionToPublish->concludeAndPublishResults();
                delete electionToPublish;
            }
            else {
                displayMessage("Error: Could not create election object to publish results.", true);
            }
        }
    }
    else {
        displayMessage("Error: Failed to update election record for " + electionID + ".", true);
    }
}


void Admin::deleteElection() {
    system("cls");
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t................... D E L E T E   E L E C T I O N ....................\n";
    cout << "\t\t\t\t\t\t...............(Only 'Upcoming' elections can be deleted).............\n";
    cout << "\t\t\t\t\t\t......................................................................\n";
    listAllElections();
    cout << "\t\t\t\t\t\t......................................................................\n";

    string idToDelete;
    cout << "\t\t\t\t\t\tEnter Election ID to delete: ";
    getline(cin, idToDelete);

    string fileLines[MAX_FILE_LINES];
    int lineCount = readFileToArray(ELECTIONS_FILE_PATH, fileLines, MAX_FILE_LINES);
    int deleteIndex = -1;
    string currentId, currentType, currentStatus, currentEndDateStr, currentScope;

    for (int i = 0; i < lineCount; ++i) {
        if (Election::parseElectionLine(fileLines[i], currentId, currentType, currentStatus, currentEndDateStr, currentScope)) {
            if (currentId == idToDelete) {
                if (currentStatus != "Upcoming") {
                    displayMessage("Cannot delete election with status '" + currentStatus + "'.", true);
                    return;
                }
                deleteIndex = i;
                break;
            }
        }
    }

    if (deleteIndex == -1) {
        displayMessage("Election ID not found or is not 'Upcoming'.", true);
        return;
    }

    for (int i = deleteIndex; i < lineCount - 1; ++i) {
        fileLines[i] = fileLines[i + 1];
    }
    lineCount--;

    if (writeArrayToFile(ELECTIONS_FILE_PATH, fileLines, lineCount)) {
        displayMessage("Election " + idToDelete + " deleted successfully.", false);
        string userLines[MAX_FILE_LINES];
        int userCount = readFileToArray(USER_CREDENTIALS_FILE_PATH, userLines, MAX_FILE_LINES);
        bool credentialsUpdated = false;
        for (int i = 0; i < userCount; ++i) {
            string userFields[USER_CRED_FIELD_COUNT];
            if (adminParseLine(userLines[i], '-', userFields, USER_CRED_FIELD_COUNT) == USER_CRED_FIELD_COUNT) {
                if (userFields[USER_CRED_PARTICIPELECID_IDX] == idToDelete) {
                    userFields[USER_CRED_ISPARTICIPATING_IDX] = "0";
                    userFields[USER_CRED_PARTICIPELECID_IDX] = "NONE";
                    userLines[i] = adminConstructLine(userFields, USER_CRED_FIELD_COUNT, '-');
                    credentialsUpdated = true;
                }
            }
        }
        if (credentialsUpdated) {
            writeArrayToFile(USER_CREDENTIALS_FILE_PATH, userLines, userCount);
        }

    }
    else {
        displayMessage("Error: Failed to save changes after deleting.", true);
    }
}

void Admin::viewElectionResultsAdmin() {
    system("cls");
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t............. V I E W   E L E C T I O N   R E S U L T S ..............\n";
    cout << "\t\t\t\t\t\t......................................................................\n";
    listAllElections();
    cout << "\t\t\t\t\t\t......................................................................\n";

    string idToView;
    cout << "\t\t\t\t\t\tEnter Election ID to view results (must be Completed or Cancelled): ";
    getline(cin, idToView);

    ifstream electionsFile(ELECTIONS_FILE_PATH);
    if (!electionsFile.is_open()) {
        displayMessage("Error opening elections file.", true);
        return;
    }

    string line;
    string electionID, type, statusFromFile, endDateStr, scopeInfo;
    bool foundElection = false;
    Election* targetElection = NULL;

    while (getline(electionsFile, line)) {
        if (Election::parseElectionLine(line, electionID, type, statusFromFile, endDateStr, scopeInfo)) {
            if (electionID == idToView) {
                foundElection = true;
                if (statusFromFile == "Completed" || statusFromFile == "Cancelled") {
                    if (type == "National") targetElection = new NationalElection(electionID, statusFromFile);
                    else if (type == "Regional") targetElection = new RegionalElection(electionID, statusFromFile, scopeInfo);
                    else if (type == "Metropolitan") targetElection = new MetropolitanElection(electionID, statusFromFile, scopeInfo);
                    else if (type == "Local") {
                        size_t comma_pos = scopeInfo.find(',');
                        if (comma_pos != string::npos) targetElection = new LocalElection(electionID, statusFromFile, scopeInfo.substr(0, comma_pos), scopeInfo.substr(comma_pos + 1));
                    }
                    if (targetElection && endDateStr != "N/A") {
                        targetElection->setEndDateTime(Election::parseDateTimeString(endDateStr));
                    }
                }
                else {
                    displayMessage("Results can only be viewed for 'Completed' or 'Cancelled' elections. This one is '" + statusFromFile + "'.", false);
                }
                break;
            }
        }
    }
    electionsFile.close();

    if (!foundElection) {
        displayMessage("Election ID not found.", true);
        return;
    }

    if (targetElection) {
        targetElection->concludeAndPublishResults();
        delete targetElection;
    }
    else if (foundElection) {
    }
}

bool Admin::updateCredentialStatus(const string& targetCnic, const string& credFilePath, char newStatus) {
    string* lines = new string[MAX_FILE_LINES];
    if (lines == NULL) { return false; }
    int count = readFileToArray(credFilePath, lines, MAX_FILE_LINES);
    bool foundAndUpdated = false;

    for (int i = 0; i < count; ++i) {
        string currentFields[USER_CRED_FIELD_COUNT];
        if (adminParseLine(lines[i], '-', currentFields, USER_CRED_FIELD_COUNT) == USER_CRED_FIELD_COUNT) {
            if (currentFields[USER_CRED_CNIC_IDX] == targetCnic) {
                currentFields[USER_CRED_STATUS_IDX] = string(1, newStatus);
                lines[i] = adminConstructLine(currentFields, USER_CRED_FIELD_COUNT, '-');
                foundAndUpdated = true;
                break;
            }
        }
    }

    if (foundAndUpdated) {
        if (!writeArrayToFile(credFilePath, lines, count)) {
            foundAndUpdated = false;
        }
    }
    delete[] lines;
    return foundAndUpdated;
}


void Admin::accountApproval() {
    while (true) {
        system("cls");
        cout << "\t\t\t\t\t\t......................................................................\n";
        cout << "\t\t\t\t\t\t.....................A C C O U N T   A P P R O V A L..................\n";
        cout << "\t\t\t\t\t\t......................................................................\n";
        cout << "\t\t\t\t\t\t..                   Accounts Pending Approval                      ..\n";
        cout << "\t\t\t\t\t\t......................................................................\n";
        ifstream approvIn(APPROVAL_FILE); string line, pendingCnic; bool pendingExists = false; int count = 0;
        if (!approvIn.is_open() || isFileEmpty(APPROVAL_FILE)) { cout << "\t\t\t\t\t\t..                     No pending approvals.                        ..\n"; }
        else {
            while (getline(approvIn, line)) {
                size_t dashPos = line.find('-'); if (dashPos != string::npos) {
                    pendingCnic = line.substr(0, dashPos);
                    string countStr = to_string(++count);
                    string displayLine = "   " + countStr + ". " + pendingCnic;
                    cout << "\t\t\t\t\t\t.." << formatDisplayLine(displayLine) << "..\n";
                    pendingExists = true;
                }
            } approvIn.close();
            if (!pendingExists) { cout << "\t\t\t\t\t\t..                     No pending approvals found.                  ..\n"; }
        }
        cout << "\t\t\t\t\t\t......................................................................\n";
        cout << "\t\t\t\t\t\t..             Enter CNIC to Approve (or '0' to Go Back             ..\n";
        cout << "\t\t\t\t\t\t......................................................................\n";
        cout << "\t\t\t\t\t\t                       Enter CNIC here: ";
        string inputCnic; getline(cin, inputCnic, '\n');
        if (inputCnic == "0") { return; }
        if (!pendingExists && inputCnic != "0") { invalidEntry(); continue; }

        bool foundInApproval = false;
        approvIn.open(APPROVAL_FILE);
        if (approvIn.is_open()) {
            string currentLine;
            while (getline(approvIn, currentLine)) {
                size_t dashPos = currentLine.find('-');
                if (dashPos != string::npos && currentLine.substr(0, dashPos) == inputCnic) {
                    foundInApproval = true;
                    break;
                }
            }
            approvIn.close();
        }

        if (!foundInApproval) { displayMessage("CNIC (" + inputCnic + ") not found in pending list.", true); continue; }

        if (!updateCredentialStatus(inputCnic, USER_CREDENTIALS_FILE_PATH, ACTIVE_STATUS)) {
            displayMessage("Failed to update status for " + inputCnic, true);
            continue;
        }

        string* approvLinesLocal = new string[MAX_FILE_LINES];
        if (approvLinesLocal == NULL) { displayMessage("Memory error in approval.", true); return; }

        int approvCountLocal = readFileToArray(APPROVAL_FILE, approvLinesLocal, MAX_FILE_LINES);
        int approvDeleteIndexLocal = -1;
        bool removedFromApprovalFile = false;

        for (int i = 0; i < approvCountLocal; ++i) {
            size_t dashPos = approvLinesLocal[i].find('-');
            if (dashPos != string::npos) {
                string recordCnic = approvLinesLocal[i].substr(0, dashPos);
                if (recordCnic == inputCnic) {
                    approvDeleteIndexLocal = i;
                    break;
                }
            }
        }

        if (approvDeleteIndexLocal != -1) {
            for (int i = approvDeleteIndexLocal; i < approvCountLocal - 1; ++i) {
                approvLinesLocal[i] = approvLinesLocal[i + 1];
            }
            approvCountLocal--;
            if (writeArrayToFile(APPROVAL_FILE, approvLinesLocal, approvCountLocal)) {
                removedFromApprovalFile = true;
            }
            else {
                displayMessage("CRITICAL: Failed to update approval file for " + inputCnic + ".", true);
            }
        }
        delete[] approvLinesLocal;

        if (removedFromApprovalFile) { displayMessage("Account " + inputCnic + " approved successfully!", false); }
        else { displayMessage("Account " + inputCnic + " status updated, but failed to update approval list.", true); }
    }
}

void Admin::displayViewAdminProfileMenu() {
    system("cls");
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t..................A D M I N   P R O F I L E   M E N U.................\n";
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t.." << formatDisplayLine("              Logged In: " + adminProfile.getFullName() + " (Admin)") << "..\n";
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t..                                                                  ..\n";
    cout << "\t\t\t\t\t\t..                       '1' View My Profile                        ..\n";
    cout << "\t\t\t\t\t\t..                       '2' Edit My Profile                        ..\n";
    cout << "\t\t\t\t\t\t..                                                                  ..\n";
    cout << "\t\t\t\t\t\t..                       '0' Go Back                                ..\n";
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t                      Enter your choice here: ";
}

void Admin::viewAdminProfile() {
    string entry; int choice;
    while (true) {
        displayViewAdminProfileMenu();
        getline(cin, entry, '\n');
        if (entry.length() == 1 && isdigit(entry.at(0))) {
            try { choice = stoi(entry); }
            catch (...) { invalidEntry(); continue; }
            switch (choice) {
            case 1: displayCurrentAdminProfile(); break;
            case 2: editCurrentAdminProfile(); break;
            case 0: return;
            default: invalidEntry(); break;
            }
        }
        else { invalidEntry(); }
    }
}

void Admin::displayCurrentAdminProfile() {
    system("cls");
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t...................V I E W   A D M I N   P R O F I L E................\n";
    cout << "\t\t\t\t\t\t......................................................................\n";

    string* adminCredLineHolder = new string[1];
    string* adminDataLineHolder = new string[1];
    string adminCnicFromFile = "N/A";

    if (adminCredLineHolder == NULL || adminDataLineHolder == NULL) {
        displayMessage("Critical Error: Memory allocation failed.", true);
        if (adminCredLineHolder != NULL) delete[] adminCredLineHolder;
        if (adminDataLineHolder != NULL) delete[] adminDataLineHolder;
        return;
    }

    int adminCredCount = readFileToArray(ADMIN_CREDENTIALS_FILE_PATH, adminCredLineHolder, 1);
    int adminDataCount = readFileToArray(ADMIN_DATA_FILE_PATH, adminDataLineHolder, 1);

    if (adminCredCount > 0) {
        string currentAdminCredFields[ADMIN_CRED_FIELD_COUNT];
        if (adminParseLine(adminCredLineHolder[0], '-', currentAdminCredFields, ADMIN_CRED_FIELD_COUNT) >= 1) {
            adminCnicFromFile = currentAdminCredFields[ADMIN_CRED_CNIC_IDX];
            cout << "\t\t\t\t\t\t.." << formatDisplayLine("   Admin CNIC: " + adminCnicFromFile) << "..\n";
            if (adminParseLine(adminCredLineHolder[0], '-', currentAdminCredFields, ADMIN_CRED_FIELD_COUNT) == ADMIN_CRED_FIELD_COUNT) {
                cout << "\t\t\t\t\t\t.." << formatDisplayLine("   Full Name: " + currentAdminCredFields[ADMIN_CRED_FNAME_IDX] + " " + currentAdminCredFields[ADMIN_CRED_LNAME_IDX]) << "..\n";
                cout << "\t\t\t\t\t\t.." << formatDisplayLine("   Password: " + currentAdminCredFields[ADMIN_CRED_PASS_IDX]) << "..\n";
                cout << "\t\t\t\t\t\t.." << formatDisplayLine("   Account Status: Active (Admin)") << "..\n";
            }
            else {
                cout << "\t\t\t\t\t\t.." << formatDisplayLine("   Error parsing full admin credentials data.") << "..\n";
            }
        }
        else {
            cout << "\t\t\t\t\t\t.." << formatDisplayLine("   Error reading admin credentials.") << "..\n";
        }
    }
    else {
        cout << "\t\t\t\t\t\t.." << formatDisplayLine("   No admin credentials record found.") << "..\n";
    }
    cout << "\t\t\t\t\t\t......................................................................\n";
    if (adminDataCount > 0) {
        Profile tempAdminProfileDataOnly;
        tempAdminProfileDataOnly.parseDataLine(adminDataLineHolder[0]);

        if (adminCnicFromFile != "N/A" && tempAdminProfileDataOnly.getCnic() != adminCnicFromFile) {
            cout << "\t\t\t\t\t\t.." << formatDisplayLine("   Warning: CNIC mismatch between creds and data file.") << "..\n";
        }
        cout << "\t\t\t\t\t\t.." << formatDisplayLine("   Full Name: " + tempAdminProfileDataOnly.getFullName()) << "..\n";
        cout << "\t\t\t\t\t\t.." << formatDisplayLine("   Age: " + tempAdminProfileDataOnly.getAge() + " years") << "..\n";
        cout << "\t\t\t\t\t\t.." << formatDisplayLine("   Phone Number: " + tempAdminProfileDataOnly.getPhone()) << "..\n";
        cout << "\t\t\t\t\t\t.." << formatDisplayLine("   Gender: " + tempAdminProfileDataOnly.getGender()) << "..\n";
        cout << "\t\t\t\t\t\t.." << formatDisplayLine("   Address: " + tempAdminProfileDataOnly.getTown() + ", " + tempAdminProfileDataOnly.getCity() + ", " + tempAdminProfileDataOnly.getState()) << "..\n";
        cout << "\t\t\t\t\t\t.." << formatDisplayLine("   Postal Code: " + tempAdminProfileDataOnly.getPostalCode()) << "..\n";
        cout << "\t\t\t\t\t\t.." << formatDisplayLine("   Last Login/Record Update: " + tempAdminProfileDataOnly.getLastLoginTimestamp()) << "..\n";
    }
    else {
        cout << "\t\t\t\t\t\t.." << formatDisplayLine("   No admin profile data record found.") << "..\n";
    }
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\tPress Enter to continue...";
    string temp;
    getline(cin, temp);

    delete[] adminCredLineHolder;
    delete[] adminDataLineHolder;
}

void displayEditAdminFieldsMenu_helper() {
    cout << "\n\t\t\t\t\t\t.........................Which field to modify?.......................\n";
    cout << "\t\t\t\t\t\t.. 1. First Name                                                    ..\n";
    cout << "\t\t\t\t\t\t.. 2. Last Name                                                     ..\n";
    cout << "\t\t\t\t\t\t.. 3. Password                                                      ..\n";
    cout << "\t\t\t\t\t\t.. 4. Age                                                           ..\n";
    cout << "\t\t\t\t\t\t.. 5. Phone Number                                                  ..\n";
    cout << "\t\t\t\t\t\t.. 6. Gender                                                        ..\n";
    cout << "\t\t\t\t\t\t.. 7. State                                                         ..\n";
    cout << "\t\t\t\t\t\t.. 8. City                                                          ..\n";
    cout << "\t\t\t\t\t\t.. 9. Town                                                          ..\n";
    cout << "\t\t\t\t\t\t.. 10. Postal Code                                                  ..\n";
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t.. 99. Save Changes                                                 ..\n";
    cout << "\t\t\t\t\t\t.. 0. Discard Changes & Go Back                                     ..\n";
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t                      Enter your choice here: ";
}


void Admin::editCurrentAdminProfile() {
    system("cls");
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t...................E D I T   A D M I N   P R O F I L E................\n";
    cout << "\t\t\t\t\t\t......................................................................\n";

    string* adminCredLineHolder = new string[1];
    string* adminDataLineHolder = new string[1];

    if (adminCredLineHolder == NULL || adminDataLineHolder == NULL) {
        displayMessage("Critical Error: Memory allocation failed for edit.", true);
        if (adminCredLineHolder != NULL) delete[] adminCredLineHolder;
        if (adminDataLineHolder != NULL) delete[] adminDataLineHolder;
        return;
    }

    int adminCredCount = readFileToArray(ADMIN_CREDENTIALS_FILE_PATH, adminCredLineHolder, 1);
    int adminDataCount = readFileToArray(ADMIN_DATA_FILE_PATH, adminDataLineHolder, 1);

    if (adminCredCount == 0) {
        displayMessage("Admin credentials file is empty or not found. Cannot edit.", true);
        delete[] adminCredLineHolder; delete[] adminDataLineHolder;
        return;
    }

    string currentAdminCredFields[ADMIN_CRED_FIELD_COUNT];
    string currentAdminDataFields[DATA_FIELD_COUNT];
    bool credParsed = false;
    bool dataParsed = false;
    string adminCnicFromFile = "N/A";

    if (adminParseLine(adminCredLineHolder[0], '-', currentAdminCredFields, ADMIN_CRED_FIELD_COUNT) == ADMIN_CRED_FIELD_COUNT) {
        credParsed = true;
        adminCnicFromFile = currentAdminCredFields[ADMIN_CRED_CNIC_IDX];
    }

    if (adminDataCount > 0) {
        if (adminParseLine(adminDataLineHolder[0], '-', currentAdminDataFields, DATA_FIELD_COUNT) == DATA_FIELD_COUNT) {
            dataParsed = true;
            if (credParsed && adminCnicFromFile != currentAdminDataFields[DATA_CNIC_IDX]) {
                displayMessage("Warning: CNIC mismatch in admin files. Forcing data CNIC.", false);
                currentAdminDataFields[DATA_CNIC_IDX] = adminCnicFromFile;
            }
            else if (!credParsed && dataParsed) {
                adminCnicFromFile = currentAdminDataFields[DATA_CNIC_IDX];
            }
        }
    }
    else {
        displayMessage("Admin data record not found. New data record will be created.", false);
        if (credParsed) {
            currentAdminDataFields[DATA_CNIC_IDX] = adminCnicFromFile;
            currentAdminDataFields[DATA_FNAME_IDX] = currentAdminCredFields[ADMIN_CRED_FNAME_IDX];
            currentAdminDataFields[DATA_LNAME_IDX] = currentAdminCredFields[ADMIN_CRED_LNAME_IDX];
            currentAdminDataFields[DATA_AGE_IDX] = "0"; currentAdminDataFields[DATA_PHONE_IDX] = "";
            currentAdminDataFields[DATA_GENDER_IDX] = ""; currentAdminDataFields[DATA_STATE_IDX] = "";
            currentAdminDataFields[DATA_CITY_IDX] = ""; currentAdminDataFields[DATA_TOWN_IDX] = "";
            currentAdminDataFields[DATA_POSTAL_IDX] = "";
            dataParsed = true;
        }
    }

    if (!credParsed) {
        displayMessage("Error parsing admin credential data. Cannot edit.", true);
        delete[] adminCredLineHolder; delete[] adminDataLineHolder;
        return;
    }

    bool changesMade = false;
    string input, newValue;
    int choice;

    while (true) {
        system("cls");
        cout << "\t\t\t\t\t\t......................................................................\n";
        cout << "\t\t\t\t\t\t...................E D I T   A D M I N   P R O F I L E................\n";
        cout << "\t\t\t\t\t\t......................................................................\n";
        cout << "\t\t\t\t\t\t.." << formatDisplayLine("Editing Profile for Admin CNIC: " + adminCnicFromFile) << "..\n";
        cout << "\t\t\t\t\t\t.." << formatDisplayLine("Current FName: " + currentAdminCredFields[ADMIN_CRED_FNAME_IDX] + ", LName: " + currentAdminCredFields[ADMIN_CRED_LNAME_IDX]) << "..\n";
        if (dataParsed) {
            cout << "\t\t\t\t\t\t.." << formatDisplayLine("Current Age: " + currentAdminDataFields[DATA_AGE_IDX] + ", Phone: " + currentAdminDataFields[DATA_PHONE_IDX]) << "..\n";
        }
        else {
            cout << "\t\t\t\t\t\t.." << formatDisplayLine("(Admin data record fields may be blank or default)") << "..\n";
        }
        cout << "\t\t\t\t\t\t......................................................................\n";

        displayEditAdminFieldsMenu_helper();
        getline(cin, input);
        if (input.empty() || !containsOnlyDigits(input)) { invalidEntry(); continue; }
        try { choice = stoi(input); }
        catch (...) { invalidEntry(); continue; }

        switch (choice) {
        case 1:
            cout << "\t\t\t\t\t\tEnter new First Name: "; getline(cin, newValue);
            if (adminSimpleIsValidName(newValue)) {
                currentAdminCredFields[ADMIN_CRED_FNAME_IDX] = newValue;
                if (dataParsed) currentAdminDataFields[DATA_FNAME_IDX] = newValue;
                changesMade = true; displayMessage("First Name updated.", false);
            }
            else { displayMessage("Invalid First Name.", true); }
            break;
        case 2:
            cout << "\t\t\t\t\t\tEnter new Last Name: "; getline(cin, newValue);
            if (adminSimpleIsValidName(newValue)) {
                currentAdminCredFields[ADMIN_CRED_LNAME_IDX] = newValue;
                if (dataParsed) currentAdminDataFields[DATA_LNAME_IDX] = newValue;
                changesMade = true; displayMessage("Last Name updated.", false);
            }
            else { displayMessage("Invalid Last Name.", true); }
            break;
        case 3:
            cout << "\t\t\t\t\t\tEnter new Password: "; hidePasswordInput(newValue);
            if (adminSimpleIsValidPassword(newValue)) {
                currentAdminCredFields[ADMIN_CRED_PASS_IDX] = newValue;
                changesMade = true; displayMessage("Password updated.", false);
            }
            else { displayMessage("Invalid Password.", true); }
            break;
        case 4:
            cout << "\t\t\t\t\t\tEnter new Age: "; getline(cin, newValue); int tempAge;
            if (adminSimpleIsValidAge(newValue, tempAge)) {
                if (dataParsed) currentAdminDataFields[DATA_AGE_IDX] = newValue;
                changesMade = true; displayMessage("Age updated.", false);
            }
            else { displayMessage("Invalid Age.", true); }
            break;
        case 5:
            cout << "\t\t\t\t\t\tEnter new Phone (+92XXXXXXXXXX): "; getline(cin, newValue);
            if (adminSimpleIsValidPhone(newValue)) {
                if (dataParsed) currentAdminDataFields[DATA_PHONE_IDX] = newValue;
                changesMade = true; displayMessage("Phone updated.", false);
            }
            else { displayMessage("Invalid Phone Number.", true); }
            break;
        case 6:
            cout << "\t\t\t\t\t\tEnter new Gender: "; getline(cin, newValue);
            if (adminSimpleIsValidGender(newValue)) {
                if (dataParsed) currentAdminDataFields[DATA_GENDER_IDX] = newValue;
                changesMade = true; displayMessage("Gender updated.", false);
            }
            else { displayMessage("Invalid Gender.", true); }
            break;
        case 7:
            cout << "\t\t\t\t\t\tEnter new State: "; getline(cin, newValue);
            if (adminSimpleIsValidGenericText(newValue)) {
                if (dataParsed) currentAdminDataFields[DATA_STATE_IDX] = newValue;
                changesMade = true; displayMessage("State updated.", false);
            }
            else { displayMessage("Invalid State.", true); }
            break;
        case 8:
            cout << "\t\t\t\t\t\tEnter new City: "; getline(cin, newValue);
            if (adminSimpleIsValidGenericText(newValue)) {
                if (dataParsed) currentAdminDataFields[DATA_CITY_IDX] = newValue;
                changesMade = true; displayMessage("City updated.", false);
            }
            else { displayMessage("Invalid City.", true); }
            break;
        case 9:
            cout << "\t\t\t\t\t\tEnter new Town: "; getline(cin, newValue);
            if (adminSimpleIsValidGenericText(newValue)) {
                if (dataParsed) currentAdminDataFields[DATA_TOWN_IDX] = newValue;
                changesMade = true; displayMessage("Town updated.", false);
            }
            else { displayMessage("Invalid Town.", true); }
            break;
        case 10:
            cout << "\t\t\t\t\t\tEnter new Postal Code (5 digits): "; getline(cin, newValue);
            if (adminSimpleIsValidPostalCode(newValue)) {
                if (dataParsed) currentAdminDataFields[DATA_POSTAL_IDX] = newValue;
                changesMade = true; displayMessage("Postal Code updated.", false);
            }
            else { displayMessage("Invalid Postal Code.", true); }
            break;
        case 99:
            if (changesMade) {
                adminCredLineHolder[0] = adminConstructLine(currentAdminCredFields, ADMIN_CRED_FIELD_COUNT, '-');
                if (!writeArrayToFile(ADMIN_CREDENTIALS_FILE_PATH, adminCredLineHolder, 1)) {
                    displayMessage("Failed to save admin credentials file.", true);
                }
                else {
                    displayMessage("Admin credentials saved.", false);
                }
                currentAdminDataFields[DATA_TIMESTAMP_IDX] = DateTime::getCurrentDateTime().toString();
                adminDataLineHolder[0] = adminConstructLine(currentAdminDataFields, DATA_FIELD_COUNT, '-');
                if (!writeArrayToFile(ADMIN_DATA_FILE_PATH, adminDataLineHolder, 1)) {
                    displayMessage("Failed to save admin data file.", true);
                }
                else {
                    displayMessage("Admin profile data saved.", false);
                }
                adminProfile.loadFromFile(adminCnicFromFile, ADMIN_DATA_FILE_PATH);
                adminProfile.setFirstName(currentAdminCredFields[ADMIN_CRED_FNAME_IDX]);
                adminProfile.setLastName(currentAdminCredFields[ADMIN_CRED_LNAME_IDX]);
            }
            else {
                displayMessage("No changes were made.", false);
            }
            delete[] adminCredLineHolder; delete[] adminDataLineHolder;
            return;
        case 0:
            displayMessage("Changes discarded.", false);
            delete[] adminCredLineHolder; delete[] adminDataLineHolder;
            return;
        default:
            invalidEntry();
            break;
        }
    }
}

void Admin::displayPartyManagementMenu() {
    system("cls");
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t....................P A R T Y   M A N A G E M E N T...................\n";
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t..                                                                  ..\n";
    cout << "\t\t\t\t\t\t..                       '1' Create Party                           ..\n";
    cout << "\t\t\t\t\t\t..                       '2' Modify Party                           ..\n";
    cout << "\t\t\t\t\t\t..                       '3' Delete Party                           ..\n";
    cout << "\t\t\t\t\t\t..                       '4' List All Parties                       ..\n";
    cout << "\t\t\t\t\t\t..                       '0' Go Back (Admin Dashboard)              ..\n";
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t                      Enter your choice here: ";
}

void Admin::partyManagement() {
    string entry; int choice;
    while (true) {
        displayPartyManagementMenu();
        getline(cin, entry, '\n');
        if (entry.length() == 1 && isdigit(entry.at(0))) {
            try { choice = stoi(entry); }
            catch (...) { invalidEntry(); continue; }
            switch (choice) {
            case 1: createParty(); break;
            case 2: modifyParty(); break;
            case 3: deleteParty(); break;
            case 4: listAllParties(); break;
            case 0: return;
            default: invalidEntry(); break;
            }
        }
        else { invalidEntry(); }
    }
}

bool Admin::isPartyIDUnique(const string& partyID) {
    ifstream inFile(PARTIES_FILE_PATH);
    if (!inFile.is_open()) { return true; }
    string line;
    while (getline(inFile, line)) {
        if (line.empty()) continue;
        size_t firstDash = line.find('-');
        if (firstDash != string::npos) {
            if (line.substr(0, firstDash) == partyID) {
                inFile.close(); return false;
            }
        }
    }
    inFile.close(); return true;
}

bool Admin::isPartyNameUnique(const string& newPartyName) {
    ifstream inFile(PARTIES_FILE_PATH);
    if (!inFile.is_open()) { return true; }
    string line;
    string fields[PARTY_FIELD_COUNT];
    while (getline(inFile, line)) {
        if (line.empty()) continue;
        if (adminParseLine(line, '-', fields, PARTY_FIELD_COUNT) == PARTY_FIELD_COUNT) {
            if (fields[PARTY_NAME_IDX] == newPartyName) {
                inFile.close(); return false;
            }
        }
    }
    inFile.close(); return true;
}


string Admin::generateUniquePartyID() {
    srand(time(0));
    string newID;
    do {
        int num = 1000 + (rand() % 9000);
        newID = to_string(num);
    } while (!isPartyIDUnique(newID));
    return newID;
}

void Admin::createParty() {
    system("cls");
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t....................C R E A T E   N E W   P A R T Y...................\n";
    cout << "\t\t\t\t\t\t......................................................................\n";

    string partyName, presidentCnicStr, rankChoiceStr;
    PartyRank rank;
    bool partyNameValid = false, rankValid = false, presidentCnicValid = false, presidentChecksPassed = false;

    while (!partyNameValid) {
        cout << "\t\t\t\t\t\tEnter Party Name: "; getline(cin, partyName);
        if (!adminSimpleIsValidGenericText(partyName)) { displayMessage("Invalid Party Name.", true); }
        else if (!isPartyNameUnique(partyName)) { displayMessage("Party Name already exists.", true); }
        else { partyNameValid = true; }
    }

    while (!rankValid) {
        cout << "\t\t\t\t\t\tEnter Party Rank (0 Tier1, 1 Tier2, 2 Tier3): "; getline(cin, rankChoiceStr);
        if (rankChoiceStr == "0") { rank = TIER1; rankValid = true; }
        else if (rankChoiceStr == "1") { rank = TIER2; rankValid = true; }
        else if (rankChoiceStr == "2") { rank = TIER3; rankValid = true; }
        else { displayMessage("Invalid Rank choice.", true); }
    }

    string* credLines = new string[MAX_FILE_LINES];
    if (credLines == NULL) { displayMessage("Memory allocation error.", true); return; }

    while (!presidentChecksPassed) {
        presidentCnicValid = false;
        while (!presidentCnicValid) {
            cout << "\t\t\t\t\t\tEnter CNIC of Party President (13 digits): "; getline(cin, presidentCnicStr);
            if (!isValidCnicForDelete(presidentCnicStr)) { displayMessage("Invalid President CNIC format.", true); }
            else { presidentCnicValid = true; }
        }

        bool presidentExists = false;
        string presidentCredFields[USER_CRED_FIELD_COUNT];
        int presidentCredIndex = -1;
        int credCount = readFileToArray(USER_CREDENTIALS_FILE_PATH, credLines, MAX_FILE_LINES);

        for (int i = 0; i < credCount; ++i) {
            string fields[USER_CRED_FIELD_COUNT];
            if (adminParseLine(credLines[i], '-', fields, USER_CRED_FIELD_COUNT) == USER_CRED_FIELD_COUNT) {
                if (fields[USER_CRED_CNIC_IDX] == presidentCnicStr) {
                    presidentExists = true; presidentCredIndex = i;
                    for (int j = 0; j < USER_CRED_FIELD_COUNT; ++j) presidentCredFields[j] = fields[j];
                    break;
                }
            }
        }

        if (!presidentExists) { displayMessage("President CNIC not found in user records.", true); continue; }
        if (presidentCredFields[USER_CRED_PARTYID_IDX] != DEFAULT_PARTY_ID) { displayMessage("President already affiliated with party " + presidentCredFields[USER_CRED_PARTYID_IDX] + ".", true); continue; }

        presidentChecksPassed = true;
        string partyID = generateUniquePartyID();
        PoliticalParty newParty(partyID, partyName, rank, presidentCnicStr);

        ofstream outFile(PARTIES_FILE_PATH, ios::app);
        if (!outFile.is_open()) { displayMessage("Error: Could not open parties file.", true); delete[] credLines; return; }
        outFile << newParty.toString() << endl; outFile.close();

        if (presidentCredIndex != -1) {
            presidentCredFields[USER_CRED_PARTYID_IDX] = partyID;
            presidentCredFields[USER_CRED_ISPRESIDENT_IDX] = "1";
            presidentCredFields[USER_CRED_ISCANDIDATE_IDX] = string(1, IS_CANDIDATE_TRUE);
            presidentCredFields[USER_CRED_ISDEPENDENTCANDIDATE_IDX] = "1";
            credLines[presidentCredIndex] = adminConstructLine(presidentCredFields, USER_CRED_FIELD_COUNT, '-');
            if (!writeArrayToFile(USER_CREDENTIALS_FILE_PATH, credLines, credCount)) { displayMessage("Error updating president's credentials.", true); }
        }
        displayMessage("Party '" + partyName + "' created successfully with ID: " + partyID, false);
    }
    delete[] credLines;
}


void Admin::modifyParty() {
    system("cls");
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t......................M O D I F Y   P A R T Y.........................\n";
    cout << "\t\t\t\t\t\t......................................................................\n";
    string partyIDToModify;
    cout << "\t\t\t\t\t\tEnter 4-digit Party ID to modify: "; getline(cin, partyIDToModify);

    if (partyIDToModify.length() != 4 || !containsOnlyDigits(partyIDToModify)) { displayMessage("Invalid Party ID format.", true); return; }

    string* partyLines = new string[MAX_FILE_LINES];
    if (partyLines == NULL) { displayMessage("Memory allocation error.", true); return; }
    int partyCount = readFileToArray(PARTIES_FILE_PATH, partyLines, MAX_FILE_LINES);
    int partyModifyIndex = -1;

    for (int i = 0; i < partyCount; ++i) {
        if (partyLines[i].substr(0, partyLines[i].find('-')) == partyIDToModify) {
            partyModifyIndex = i; break;
        }
    }

    if (partyModifyIndex == -1) { displayMessage("Party ID not found.", true); delete[] partyLines; return; }

    PoliticalParty partyToModify;
    if (!partyToModify.fromString(partyLines[partyModifyIndex])) { displayMessage("Error parsing party data.", true); delete[] partyLines; return; }

    string oldPresidentCnic = partyToModify.getPartyPresidentCNIC();
    bool changesMade = false; string input, newValue; int choice;

    while (true) {
        system("cls");
        cout << "\t\t\t\t\t\tModifying Party ID: " << partyToModify.getPartyID() << " (" << partyToModify.getPartyName() << ")\n";
        cout << "\t\t\t\t\t\t1. Modify Party Name (Current: " << partyToModify.getPartyName() << ")\n";
        cout << "\t\t\t\t\t\t2. Modify Party Rank (Current: " << partyToModify.getPartyRankString() << ")\n";
        cout << "\t\t\t\t\t\t3. Modify Party President CNIC (Current: " << partyToModify.getPartyPresidentCNIC() << ")\n";
        cout << "\t\t\t\t\t\t99. Save Changes\n";
        cout << "\t\t\t\t\t\t0. Discard & Go Back\n";
        cout << "\t\t\t\t\t\tEnter choice: "; getline(cin, input);
        if (input.empty() || !containsOnlyDigits(input)) { invalidEntry(); continue; }
        try { choice = stoi(input); }
        catch (...) { invalidEntry(); continue; }

        switch (choice) {
        case 1:
            cout << "\t\t\t\t\t\tEnter new Party Name: "; getline(cin, newValue);
            if (adminSimpleIsValidGenericText(newValue)) {
                if (!isPartyNameUnique(newValue) && newValue != partyToModify.getPartyName()) { displayMessage("Party name already exists.", true); }
                else { partyToModify.setPartyName(newValue); changesMade = true; displayMessage("Party Name updated.", false); }
            }
            else { displayMessage("Invalid Party Name.", true); }
            break;
        case 2:
            cout << "\t\t\t\t\t\tEnter new Party Rank (0:Tier1, 1:Tier2, 2:Tier3): "; getline(cin, newValue);
            PartyRank newRank;
            if (newValue == "0") newRank = TIER1; else if (newValue == "1") newRank = TIER2;
            else if (newValue == "2") newRank = TIER3; else { displayMessage("Invalid rank choice.", true); continue; }
            partyToModify.setPartyRank(newRank); changesMade = true; displayMessage("Party Rank updated.", false);
            break;
        case 3:
            cout << "\t\t\t\t\t\tEnter new Party President CNIC: "; getline(cin, newValue);
            if (isValidCnicForDelete(newValue)) {
                bool newPresidentExists = false; string newPresidentPartyID = DEFAULT_PARTY_ID;
                string* tempCredLines = new string[MAX_FILE_LINES];
                if (tempCredLines == NULL) { displayMessage("Memory error.", true); break; }
                int tempCredCount = readFileToArray(USER_CREDENTIALS_FILE_PATH, tempCredLines, MAX_FILE_LINES);
                for (int k = 0; k < tempCredCount; ++k) {
                    string tempFields[USER_CRED_FIELD_COUNT];
                    if (adminParseLine(tempCredLines[k], '-', tempFields, USER_CRED_FIELD_COUNT) == USER_CRED_FIELD_COUNT) {
                        if (tempFields[USER_CRED_CNIC_IDX] == newValue) {
                            newPresidentExists = true; newPresidentPartyID = tempFields[USER_CRED_PARTYID_IDX]; break;
                        }
                    }
                } delete[] tempCredLines;
                if (!newPresidentExists) { displayMessage("New President CNIC not found.", true); break; }
                if (newPresidentPartyID != DEFAULT_PARTY_ID && newPresidentPartyID != partyToModify.getPartyID()) { displayMessage("New President already affiliated.", true); break; }
                partyToModify.setPartyPresidentCNIC(newValue); changesMade = true; displayMessage("Party President CNIC updated.", false);
            }
            else { displayMessage("Invalid CNIC.", true); }
            break;
        case 99:
            if (changesMade) {
                partyLines[partyModifyIndex] = partyToModify.toString();
                if (!writeArrayToFile(PARTIES_FILE_PATH, partyLines, partyCount)) { displayMessage("Failed to save parties file.", true); }
                else {
                    if (oldPresidentCnic != partyToModify.getPartyPresidentCNIC()) {
                        updateUserCredentialField(oldPresidentCnic, USER_CRED_ISPRESIDENT_IDX, "0");
                        updateUserCredentialField(oldPresidentCnic, USER_CRED_PARTYID_IDX, DEFAULT_PARTY_ID);
                        updateUserCredentialField(oldPresidentCnic, USER_CRED_ISDEPENDENTCANDIDATE_IDX, "0");
                        updateUserCredentialField(partyToModify.getPartyPresidentCNIC(), USER_CRED_ISPRESIDENT_IDX, "1");
                        updateUserCredentialField(partyToModify.getPartyPresidentCNIC(), USER_CRED_PARTYID_IDX, partyToModify.getPartyID());
                        updateUserCredentialField(partyToModify.getPartyPresidentCNIC(), USER_CRED_ISCANDIDATE_IDX, "1");
                        updateUserCredentialField(partyToModify.getPartyPresidentCNIC(), USER_CRED_ISDEPENDENTCANDIDATE_IDX, "1");
                    } displayMessage("Party changes saved successfully.", false);
                }
            }
            else { displayMessage("No changes made.", false); }
            delete[] partyLines; return;
        case 0:
            displayMessage("Changes discarded.", false); delete[] partyLines; return;
        default: invalidEntry();
        }
    }
}


void Admin::deleteParty() {
    system("cls");
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t........................D E L E T E   P A R T Y.......................\n";
    cout << "\t\t\t\t\t\t......................................................................\n";
    string partyIDToDelete;
    cout << "\t\t\t\t\t\tEnter 4-digit Party ID to delete: "; getline(cin, partyIDToDelete);
    if (partyIDToDelete.length() != 4 || !containsOnlyDigits(partyIDToDelete)) { displayMessage("Invalid Party ID format.", true); return; }

    string* partyLines = new string[MAX_FILE_LINES];
    if (partyLines == NULL) { displayMessage("Memory allocation error.", true); return; }
    int partyCount = readFileToArray(PARTIES_FILE_PATH, partyLines, MAX_FILE_LINES);
    int partyDeleteIndex = -1; string presidentCnicOfDeletedParty = "N/A";

    for (int i = 0; i < partyCount; ++i) {
        string fields[PARTY_FIELD_COUNT];
        if (adminParseLine(partyLines[i], '-', fields, PARTY_FIELD_COUNT) == PARTY_FIELD_COUNT) {
            if (fields[PARTY_ID_IDX] == partyIDToDelete) {
                partyDeleteIndex = i; presidentCnicOfDeletedParty = fields[PARTY_PRESIDENT_CNIC_IDX]; break;
            }
        }
    }

    if (partyDeleteIndex == -1) { displayMessage("Party ID not found.", true); }
    else {
        for (int i = partyDeleteIndex; i < partyCount - 1; ++i) { partyLines[i] = partyLines[i + 1]; }
        partyCount--;
        if (!writeArrayToFile(PARTIES_FILE_PATH, partyLines, partyCount)) { displayMessage("Failed to update parties file.", true); }
        else {
            if (presidentCnicOfDeletedParty != "N/A") {
                updateUserCredentialField(presidentCnicOfDeletedParty, USER_CRED_ISPRESIDENT_IDX, "0");
                updateUserCredentialField(presidentCnicOfDeletedParty, USER_CRED_PARTYID_IDX, DEFAULT_PARTY_ID);
                updateUserCredentialField(presidentCnicOfDeletedParty, USER_CRED_ISDEPENDENTCANDIDATE_IDX, "0");
            }
            string* userCredLines = new string[MAX_FILE_LINES];
            if (userCredLines == NULL) { displayMessage("Memory error updating users.", true); }
            else {
                int userCount = readFileToArray(USER_CREDENTIALS_FILE_PATH, userCredLines, MAX_FILE_LINES);
                for (int i = 0; i < userCount; ++i) {
                    string userFields[USER_CRED_FIELD_COUNT];
                    if (adminParseLine(userCredLines[i], '-', userFields, USER_CRED_FIELD_COUNT) == USER_CRED_FIELD_COUNT) {
                        if (userFields[USER_CRED_PARTYID_IDX] == partyIDToDelete) {
                            userFields[USER_CRED_PARTYID_IDX] = DEFAULT_PARTY_ID;
                            userFields[USER_CRED_ISPRESIDENT_IDX] = "0";
                            userFields[USER_CRED_ISDEPENDENTCANDIDATE_IDX] = "0";
                            userCredLines[i] = adminConstructLine(userFields, USER_CRED_FIELD_COUNT, '-');
                        }
                    }
                }
                writeArrayToFile(USER_CREDENTIALS_FILE_PATH, userCredLines, userCount);
                delete[] userCredLines;
            }
            displayMessage("Party " + partyIDToDelete + " deleted and user affiliations updated.", false);
        }
    }
    delete[] partyLines;
}

int Admin::getPartyMemberCount(const string& partyID) {
    int memberCount = 0;
    string* userCredLines = new string[MAX_FILE_LINES];
    if (userCredLines == NULL) return 0;
    int userCount = readFileToArray(USER_CREDENTIALS_FILE_PATH, userCredLines, MAX_FILE_LINES);
    for (int i = 0; i < userCount; ++i) {
        string userFields[USER_CRED_FIELD_COUNT];
        if (adminParseLine(userCredLines[i], '-', userFields, USER_CRED_FIELD_COUNT) == USER_CRED_FIELD_COUNT) {
            if (userFields[USER_CRED_PARTYID_IDX] == partyID) { memberCount++; }
        }
    }
    delete[] userCredLines; return memberCount;
}

int Admin::getPartyCandidateCount(const string& partyID) {
    int candidateCount = 0;
    string* userCredLines = new string[MAX_FILE_LINES];
    if (userCredLines == NULL) return 0;
    int userCount = readFileToArray(USER_CREDENTIALS_FILE_PATH, userCredLines, MAX_FILE_LINES);
    for (int i = 0; i < userCount; ++i) {
        string userFields[USER_CRED_FIELD_COUNT];
        if (adminParseLine(userCredLines[i], '-', userFields, USER_CRED_FIELD_COUNT) == USER_CRED_FIELD_COUNT) {
            if (userFields[USER_CRED_PARTYID_IDX] == partyID && userFields[USER_CRED_ISDEPENDENTCANDIDATE_IDX] == "1") { candidateCount++; }
        }
    }
    delete[] userCredLines; return candidateCount;
}


void Admin::listAllParties() {
    system("cls");
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t....................L I S T   A L L   P A R T I E S...................\n";
    cout << "\t\t\t\t\t\t......................................................................\n";

    string* partyLines = new string[MAX_FILE_LINES];
    if (partyLines == NULL) { displayMessage("Memory allocation error.", true); return; }
    int partyCount = readFileToArray(PARTIES_FILE_PATH, partyLines, MAX_FILE_LINES);

    cout << "\t\t\t\t\t\t.." << formatDisplayLine("                      Total Parties Found: " + to_string(partyCount)) << "..\n";
    cout << "\t\t\t\t\t\t......................................................................\n";

    if (partyCount == 0) {
        cout << "\t\t\t\t\t\t.." << formatDisplayLine("No political parties registered.") << "..\n";
    }
    else {
        for (int i = 0; i < partyCount; ++i) {
            PoliticalParty currentParty;
            if (currentParty.fromString(partyLines[i])) {
                cout << "\t\t\t\t\t\t.." << formatDisplayLine("Party " + to_string(i + 1) + ": ID - " + currentParty.getPartyID()) << "..\n";
                cout << "\t\t\t\t\t\t.." << formatDisplayLine("  Name: " + currentParty.getPartyName()) << "..\n";
                cout << "\t\t\t\t\t\t.." << formatDisplayLine("  Rank: " + currentParty.getPartyRankString()) << "..\n";
                cout << "\t\t\t\t\t\t.." << formatDisplayLine("  President CNIC: " + currentParty.getPartyPresidentCNIC()) << "..\n";
                cout << "\t\t\t\t\t\t.." << formatDisplayLine("  Members Limit: " + to_string(currentParty.getMembersLimit())) << "..\n";
                cout << "\t\t\t\t\t\t.." << formatDisplayLine("  Candidates Limit: " + to_string(currentParty.getCandidatesLimit())) << "..\n";
                cout << "\t\t\t\t\t\t.." << formatDisplayLine("  Current Members: " + to_string(getPartyMemberCount(currentParty.getPartyID()))) << "..\n";
                cout << "\t\t\t\t\t\t.." << formatDisplayLine("  Current Dependent Candidates: " + to_string(getPartyCandidateCount(currentParty.getPartyID()))) << "..\n";
                cout << "\t\t\t\t\t\t......................................................................\n";
            }
        }
    }
    delete[] partyLines;
    cout << "\t\t\t\t\t\tPress Enter to go Back...";
    string temp;
    getline(cin, temp);
}


bool Admin::updateUserCredentialField(const string& targetCnic, int fieldIndex, const string& newValue) {
    if (fieldIndex < 0 || fieldIndex >= USER_CRED_FIELD_COUNT) return false;

    string* lines = new string[MAX_FILE_LINES];
    if (lines == NULL) { return false; }
    int count = readFileToArray(USER_CREDENTIALS_FILE_PATH, lines, MAX_FILE_LINES);
    bool foundAndUpdated = false;

    for (int i = 0; i < count; ++i) {
        string currentFields[USER_CRED_FIELD_COUNT];
        if (adminParseLine(lines[i], '-', currentFields, USER_CRED_FIELD_COUNT) == USER_CRED_FIELD_COUNT) {
            if (currentFields[USER_CRED_CNIC_IDX] == targetCnic) {
                currentFields[fieldIndex] = newValue;
                lines[i] = adminConstructLine(currentFields, USER_CRED_FIELD_COUNT, '-');
                foundAndUpdated = true;
                break;
            }
        }
    }

    if (foundAndUpdated) {
        if (!writeArrayToFile(USER_CREDENTIALS_FILE_PATH, lines, count)) {
            foundAndUpdated = false;
        }
    }
    delete[] lines;
    return foundAndUpdated;
}