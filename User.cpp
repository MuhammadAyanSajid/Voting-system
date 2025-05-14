#include "User.h"
#include "Authorization.h"
#include "Election.h"
#include "Profile.h"
#include "PoliticalParty.h"
#include <iostream>
#include <string>
#include <windows.h>
#include <cctype>
#include <fstream>

User::User() : isRegisteredCandidate_flag(false), partyID_val(DEFAULT_PARTY_ID),
isPresident_flag(false), isDependentCandidate_flag(false),
isParticipating_flag(false), participatingElectionID_val("NONE") {
}


Profile& User::getProfile() { return userProfile; }
const Profile& User::getProfile() const { return userProfile; }

bool User::loadUserProfile(const string& cnic, const string& dataFilePath) {
    if (userProfile.loadFromFile(cnic, dataFilePath)) {
        return true;
    }
    else {
        displayMessage("Failed to load user profile.", true);
        return false;
    }
}

bool User::updateLoginTimestamp(const string& dataFilePath) {
    string currentTimestamp = DateTime::getCurrentDateTime().toString();
    if (userProfile.updateTimestampInFile(userProfile.getCnic(), dataFilePath, currentTimestamp)) {
        userProfile.setLastLoginTimestamp(currentTimestamp);
        return true;
    }
    else {
        displayMessage("Failed to update login timestamp in file.", true);
        return false;
    }
}

void User::setCandidateStatus(bool status) { this->isRegisteredCandidate_flag = status; }
void User::setPartyID(const string& pid) { this->partyID_val = pid; }
void User::setIsPresident(bool status) { this->isPresident_flag = status; }
void User::setIsDependentCandidate(bool status) { this->isDependentCandidate_flag = status; }
void User::setIsParticipating(bool status) { this->isParticipating_flag = status; }
void User::setParticipatingElectionID(const string& eid) { this->participatingElectionID_val = eid; }

bool User::getIsRegisteredCandidate() const { return isRegisteredCandidate_flag; }
string User::getPartyID() const { return partyID_val; }
bool User::getIsPresident() const { return isPresident_flag; }
bool User::getIsDependentCandidate() const { return isDependentCandidate_flag; }
bool User::getIsParticipating() const { return isParticipating_flag; }
string User::getParticipatingElectionID() const { return participatingElectionID_val; }


void User::displayUserMenu() {
    system("cls");
    cout << "\t\t\t\t\t\t......................................................................\n";

    string dynamicTitle = "U S E R   D A S H B O A R D";
    if (isRegisteredCandidate_flag) {
        if (isDependentCandidate_flag) {
            if (isPresident_flag) dynamicTitle = "P A R T Y   P R E S I D E N T   D A S H B O A R D";
            else dynamicTitle = "D E P E N D E N T   C A N D I D A T E   D A S H B O A R D";
        }
        else {
            dynamicTitle = "I N D E P E N D E N T   C A N D I D A T E   D A S H B O A R D";
        }
    }
    int titleLength = dynamicTitle.length();
    int spaces_needed_title = 66 - titleLength;
    string spaces_title(spaces_needed_title / 2, ' ');
    string endSpaces_title = spaces_title;
    if (spaces_needed_title % 2 != 0 && titleLength < 66) endSpaces_title += " " ;
    cout << "\t\t\t\t\t\t" << spaces_title << dynamicTitle << endSpaces_title << "\n";

    cout << "\t\t\t\t\t\t......................................................................\n";
    string nameHeader = "Logged In: " + userProfile.getFullName();
    string roleSuffix = " (User)";
    if (isRegisteredCandidate_flag) {
        if (isDependentCandidate_flag) {
            if (isPresident_flag) roleSuffix = " (User, Party President)";
            else roleSuffix = " (User, Dependent Candidate)";
        }
        else {
            roleSuffix = " (User, Independent Candidate)";
        }
    }
    if (isParticipating_flag) {
        roleSuffix += ", Participating in Election: " + participatingElectionID_val;
    }
    nameHeader += roleSuffix;
    string timeHeader = "Last Login: " + userProfile.getLastLoginTimestamp();

    int nameHeaderLength = nameHeader.length();
    int spaces_name = 66 - nameHeaderLength; if (spaces_name < 0) spaces_name = 0;
    string pad_name(spaces_name / 2, ' ');
    string end_pad_name = pad_name;
    if (spaces_name % 2 != 0 && nameHeaderLength < 66) end_pad_name += " ";
    cout << "\t\t\t\t\t\t.." << pad_name << nameHeader << end_pad_name << "..\n";

    int timeHeaderLength = timeHeader.length();
    int spaces_time = 66 - timeHeaderLength; if (spaces_time < 0) spaces_time = 0;
    string pad_time(spaces_time / 2, ' ');
    string end_pad_time = pad_time;
    if (spaces_time % 2 != 0 && timeHeaderLength < 66) end_pad_time += " ";
    cout << "\t\t\t\t\t\t.." << pad_time << timeHeader << end_pad_time << "..\n";

    cout << "\t\t\t\t\t\t..                                                                  ..\n";
    cout << "\t\t\t\t\t\t..                     '1' View Election Schedule                   ..\n";
    cout << "\t\t\t\t\t\t..                     '2' Cast Vote                                ..\n";
    cout << "\t\t\t\t\t\t..                     '3' View/Edit Profile                        ..\n";
    cout << "\t\t\t\t\t\t..                     '4' View Election Results                    ..\n";

    int nextOption = 5;
    if (isRegisteredCandidate_flag) {
        cout << "\t\t\t\t\t\t..                     '" << nextOption++ << "' Election Management Menu                 ..\n";
    }
    if (isDependentCandidate_flag) {
        cout << "\t\t\t\t\t\t..                     '" << nextOption++ << "' Party Management Menu                    ..\n";
    }

    cout << "\t\t\t\t\t\t..                     '9' Log Out                                  ..\n";
    cout << "\t\t\t\t\t\t..                     '0' Main Menu                                ..\n";
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t                      Enter your choice here: ";
}

void User::Dashboard() {
    string entry; int choice;
    while (true) {
        displayUserMenu();
        getline(cin, entry, '\n');

        if (entry.length() == 1 && isdigit(entry.at(0))) {
            try { choice = stoi(entry); }
            catch (...) { invalidEntry(); continue; }

            int currentDynamicOption = 5;
            int electionMenuChoice = -1;
            int partyMenuChoice = -1;

            if (isRegisteredCandidate_flag) {
                electionMenuChoice = currentDynamicOption++;
            }
            if (isDependentCandidate_flag) {
                partyMenuChoice = currentDynamicOption++;
            }

            switch (choice) {
            case 1: viewElectionSchedule(); break;
            case 2: castVote(); break;
            case 3: viewEditProfile(); break;
            case 4: viewElectionResults(); break;
            case 9: loginPanel(); return;
            case 0: mainPanel(); return;
            default:
                if (isRegisteredCandidate_flag && choice == electionMenuChoice) {
                    electionManagementMenu();
                }
                else if (isDependentCandidate_flag && choice == partyMenuChoice) {
                    partyManagementMenu();
                }
                else {
                    invalidEntry();
                }
                break;
            }
        }
        else { invalidEntry(); }
    }
}

void User::viewElectionSchedule() {
    system("cls");
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t..................E L E C T I O N   S C H E D U L E...................\n";
    cout << "\t\t\t\t\t\t...........(Showing Upcoming and Ongoing Elections for You)...........\n";
    cout << "\t\t\t\t\t\t......................................................................\n";

    ifstream electionsFile(ELECTIONS_FILE_PATH);
    if (!electionsFile.is_open()) {
        displayMessage("Error: Elections file not found.", true); Sleep(1000); return;
    }
    string line;
    bool foundEligibleElection = false;
    int electionCount = 0;

    cout << "\t\t\t\t\t\t..                        Eligible Elections                        ..\n";
    cout << "\t\t\t\t\t\t......................................................................\n";

    string id, type, statusFile, endDateStr, scope;

    while (getline(electionsFile, line)) {
        if (line.empty()) continue;
        if (!Election::parseElectionLine(line, id, type, statusFile, endDateStr, scope)) continue;

        if (statusFile == "Completed" || statusFile == "Cancelled") continue;

        Election* currentElection = NULL;
        if (type == "National") {
            currentElection = new NationalElection(id, statusFile);
            if (endDateStr != "N/A") currentElection->setEndDateTime(Election::parseDateTimeString(endDateStr));
        }
        else if (type == "Regional" && !scope.empty()) {
            currentElection = new RegionalElection(id, statusFile, scope);
            if (endDateStr != "N/A") currentElection->setEndDateTime(Election::parseDateTimeString(endDateStr));
        }
        else if (type == "Metropolitan" && !scope.empty()) {
            currentElection = new MetropolitanElection(id, statusFile, scope);
            if (endDateStr != "N/A") currentElection->setEndDateTime(Election::parseDateTimeString(endDateStr));
        }
        else if (type == "Local" && scope.find(',') != string::npos) {
            size_t comma_pos = scope.find(',');
            if (comma_pos != string::npos) {
                string town = scope.substr(0, comma_pos);
                string city_ctx = scope.substr(comma_pos + 1);
                currentElection = new LocalElection(id, statusFile, town, city_ctx);
                if (endDateStr != "N/A") currentElection->setEndDateTime(Election::parseDateTimeString(endDateStr));
            }
        }

        if (currentElection) {
            if (currentElection->isUserEligibleToVote(*this, this->userProfile))
            {
                electionCount++;
                cout << "\t\t\t\t\t\tElection " << electionCount << ":\n";
                currentElection->displayElectionDetails();
                cout << "\t\t\t\t\t\t......................................................................\n";
                foundEligibleElection = true;
            }
            delete currentElection;
            currentElection = NULL;
        }
    }
    electionsFile.close();

    if (!foundEligibleElection) {
        cout << "\t\t\t\t\t\t..      No upcoming or ongoing elections found for your scope.      ..\n";
        cout << "\t\t\t\t\t\t......................................................................\n";
    }

    cout << "\t\t\t\t\t\tPress Enter to go back...";
    string temp_input;
    getline(cin, temp_input);
}


void User::castVote() {
    system("cls");
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t..................... C H O O S E   E L E C T I O N ..................\n";
    cout << "\t\t\t\t\t\t......................................................................\n";

    ifstream electionsFile(ELECTIONS_FILE_PATH);
    if (!electionsFile.is_open()) {
        displayMessage("Error: Elections file not found.", true); Sleep(1000); return;
    }
    string line;

    const int MAX_ELIGIBLE_ELECTIONS = 50;
    Election* eligibleElections[MAX_ELIGIBLE_ELECTIONS];
    int eligibleCount = 0;

    cout << "\t\t\t\t\t\t..      Select an ongoing election you are eligible to vote in:     ..\n";
    cout << "\t\t\t\t\t\t......................................................................\n";

    string id, type, statusFile, endDateStr, scope;

    while (getline(electionsFile, line) && eligibleCount < MAX_ELIGIBLE_ELECTIONS) {
        if (line.empty()) continue;
        if (!Election::parseElectionLine(line, id, type, statusFile, endDateStr, scope)) continue;

        if (statusFile != "Ongoing") continue;

        Election* currentElection = NULL;
        if (type == "National") {
            currentElection = new NationalElection(id, statusFile);
            if (endDateStr != "N/A") currentElection->setEndDateTime(Election::parseDateTimeString(endDateStr));
        }
        else if (type == "Regional" && !scope.empty()) {
            currentElection = new RegionalElection(id, statusFile, scope);
            if (endDateStr != "N/A") currentElection->setEndDateTime(Election::parseDateTimeString(endDateStr));
        }
        else if (type == "Metropolitan" && !scope.empty()) {
            currentElection = new MetropolitanElection(id, statusFile, scope);
            if (endDateStr != "N/A") currentElection->setEndDateTime(Election::parseDateTimeString(endDateStr));
        }
        else if (type == "Local" && scope.find(',') != string::npos) {
            size_t comma_pos = scope.find(',');
            if (comma_pos != string::npos) {
                string town = scope.substr(0, comma_pos);
                string city_ctx = scope.substr(comma_pos + 1);
                currentElection = new LocalElection(id, statusFile, town, city_ctx);
                if (endDateStr != "N/A") currentElection->setEndDateTime(Election::parseDateTimeString(endDateStr));
            }
        }

        if (currentElection) {
            if (currentElection->getDynamicStatus() == "Ongoing" &&
                currentElection->isUserEligibleToVote(*this, this->userProfile))
            {
                eligibleElections[eligibleCount] = currentElection;
                eligibleCount++;
            }
            else {
                delete currentElection;
                currentElection = NULL;
            }
        }
    }
    electionsFile.close();

    if (eligibleCount == 0) {
        displayMessage("No ongoing elections available for you to vote in.", false);
        return;
    }

    cout << "\t\t\t\t\t\tAvailable Elections to Vote In:\n";
    for (int i = 0; i < eligibleCount; ++i) {
        cout << "\t\t\t\t\t\t" << (i + 1) << ". ID: " << eligibleElections[i]->getElectionID()
            << " (" << eligibleElections[i]->getElectionType();
        string geoScope = eligibleElections[i]->getGeographicScope();
        if (eligibleElections[i]->getElectionType() != "National" && !geoScope.empty()) {
            if (eligibleElections[i]->getElectionType() == "Local") cout << " - Scope: Town=" << geoScope.substr(0, geoScope.find(',')) << ", City=" << geoScope.substr(geoScope.find(',') + 1);
            else cout << " - Scope: " << geoScope;
        }
        else if (eligibleElections[i]->getElectionType() == "National") cout << " - Scope: Nationwide";
        cout << ")\n";
    }

    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\tEnter the number of the election (or '0' to cancel): ";
    string choiceStr;
    getline(cin, choiceStr);
    int choiceNum = -1;

    if (choiceStr.length() > 0 && containsOnlyDigits(choiceStr)) {
        try { choiceNum = stoi(choiceStr); }
        catch (...) { choiceNum = -1; }
    }

    if (choiceNum == 0) {
        displayMessage("Vote casting cancelled.", false);
    }
    else if (choiceNum > 0 && choiceNum <= eligibleCount) {
        eligibleElections[choiceNum - 1]->showCandidatesAndCastVote(*this, this->userProfile);
    }
    else {
        invalidEntry();
    }

    for (int i = 0; i < eligibleCount; ++i) {
        if (eligibleElections[i]) delete eligibleElections[i];
    }
}


void User::viewEditProfile() {
    displayMessage("User: View/Edit Profile - Under Development.", false);
}
void User::viewElectionResults() {
    system("cls");
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t............. V I E W   E L E C T I O N   R E S U L T S ..............\n";
    cout << "\t\t\t\t\t\t......................................................................\n";

    ifstream electionsFile(ELECTIONS_FILE_PATH);
    if (!electionsFile.is_open()) {
        displayMessage("Error: Elections file not found.", true);
        Sleep(1000); return;
    }

    const int MAX_VIEWABLE_ELECTIONS = 50;
    Election* viewableElections[MAX_VIEWABLE_ELECTIONS];
    int viewableCount = 0;
    string line;
    string id, type, statusFile, endDateStr, scope;

    cout << "\t\t\t\t\t\t..          Select a 'Completed' election to view results           ..\n";
    cout << "\t\t\t\t\t\t......................................................................\n";

    while (getline(electionsFile, line) && viewableCount < MAX_VIEWABLE_ELECTIONS) {
        if (line.empty()) continue;
        if (!Election::parseElectionLine(line, id, type, statusFile, endDateStr, scope)) continue;

        if (statusFile == "Completed" ) {
            Election* currentElection = NULL;
            if (type == "National") currentElection = new NationalElection(id, statusFile);
            else if (type == "Regional") currentElection = new RegionalElection(id, statusFile, scope);
            else if (type == "Metropolitan") currentElection = new MetropolitanElection(id, statusFile, scope);
            else if (type == "Local") {
                size_t comma_pos = scope.find(',');
                if (comma_pos != string::npos) currentElection = new LocalElection(id, statusFile, scope.substr(0, comma_pos), scope.substr(comma_pos + 1));
            }

            if (currentElection) {
                if (endDateStr != "N/A") currentElection->setEndDateTime(Election::parseDateTimeString(endDateStr));
                if (currentElection->isUserEligibleToVote(*this, this->userProfile)) {
                    viewableElections[viewableCount] = currentElection;
                    viewableCount++;
                }
                else {
                    delete currentElection; 
                }
            }
        }
    }
    electionsFile.close();

    if (viewableCount == 0) {
        displayMessage("No completed or cancelled elections found for your scope.", false);
        return;
    }

    cout << "\t\t\t\t\t\tAvailable Elections to View Results For:\n";
    for (int i = 0; i < viewableCount; ++i) {
        cout << "\t\t\t\t\t\t" << (i + 1) << ". ID: " << viewableElections[i]->getElectionID()
            << " (" << viewableElections[i]->getElectionType() << " - " << viewableElections[i]->getStatusFromFile() << ")\n";
    }

    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\tEnter the number of the election (or '0' to cancel): ";
    string choiceStr;
    getline(cin, choiceStr);
    int choiceNum = -1;

    if (choiceStr.length() > 0 && containsOnlyDigits(choiceStr)) {
        try { choiceNum = stoi(choiceStr); }
        catch (...) { choiceNum = -1; }
    }

    if (choiceNum > 0 && choiceNum <= viewableCount) {
        viewableElections[choiceNum - 1]->concludeAndPublishResults();
    }
    else if (choiceNum == 0) {
        displayMessage("Action cancelled.", false);
    }
    else {
        invalidEntry();
    }

    for (int i = 0; i < viewableCount; ++i) {
        if (viewableElections[i]) delete viewableElections[i];
    }
}

void User::electionManagementMenu() {
    string entry; int choice;
    while (true) {
        system("cls");
        cout << "\t\t\t\t\t\t......................................................................\n";
        cout << "\t\t\t\t\t\t............E L E C T I O N   M A N A G E M E N T   M E N U...........\n";
        cout << "\t\t\t\t\t\t......................................................................\n";
        cout << "\t\t\t\t\t\t..                     '1' Register as Candidate in Election        ..\n";
        cout << "\t\t\t\t\t\t..                     '2' Unregister as Candidate from Election    ..\n";
        cout << "\t\t\t\t\t\t..                     '0' Go Back (User Dashboard)                 ..\n";
        cout << "\t\t\t\t\t\t......................................................................\n";
        cout << "\t\t\t\t\t\t                      Enter your choice here: ";
        getline(cin, entry, '\n');
        if (entry.length() == 1 && isdigit(entry.at(0))) {
            try { choice = stoi(entry); }
            catch (...) { invalidEntry(); continue; }
            switch (choice) {
            case 1: registerInElection(); break;
            case 2: unregisterFromElection(); break;
            case 0: return;
            default: invalidEntry(); break;
            }
        }
        else { invalidEntry(); }
    }
}

void User::registerInElection() {
    if (!isRegisteredCandidate_flag) {
        displayMessage("You must be a general candidate to register for an election.", true);
        return;
    }
    if (isParticipating_flag) {
        displayMessage("You are already participating in election: " + participatingElectionID_val, false);
        return;
    }

    system("cls");
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t.............. R E G I S T E R   F O R   E L E C T I O N ..............\n";
    cout << "\t\t\t\t\t\t......................................................................\n";

    ifstream electionsFile(ELECTIONS_FILE_PATH);
    if (!electionsFile.is_open()) {
        displayMessage("Error: Elections file not found.", true); Sleep(1000); return;
    }

    const int MAX_REG_ELECTIONS = 50;
    Election* availableElections[MAX_REG_ELECTIONS];
    int availableCount = 0;
    string line;
    string id, type, statusFile, endDateStr, scope;

    cout << "\t\t\t\t\t\t..       Select an 'Upcoming' election you are eligible for:        ..\n";
    cout << "\t\t\t\t\t\t......................................................................\n";

    while (getline(electionsFile, line) && availableCount < MAX_REG_ELECTIONS) {
        if (line.empty()) continue;
        if (!Election::parseElectionLine(line, id, type, statusFile, endDateStr, scope)) continue;

        if (statusFile != "Upcoming") continue;

        Election* currentElection = NULL;
        if (type == "National") currentElection = new NationalElection(id, statusFile);
        else if (type == "Regional" && !scope.empty()) currentElection = new RegionalElection(id, statusFile, scope);
        else if (type == "Metropolitan" && !scope.empty()) currentElection = new MetropolitanElection(id, statusFile, scope);
        else if (type == "Local" && scope.find(',') != string::npos) {
            size_t comma_pos = scope.find(',');
            if (comma_pos != string::npos) currentElection = new LocalElection(id, statusFile, scope.substr(0, comma_pos), scope.substr(comma_pos + 1));
        }

        if (currentElection) {
            PoliticalParty tempParty;
            loadPartyById(this->partyID_val, tempParty);

            if (currentElection->isCandidateEligibleToParticipate(*this, this->userProfile, tempParty)) {
                availableElections[availableCount] = currentElection;
                availableCount++;
            }
            else {
                delete currentElection;
                currentElection = NULL;
            }
        }
    }
    electionsFile.close();

    if (availableCount == 0) {
        displayMessage("No upcoming elections available for you to register in.", false);
        return;
    }

    cout << "\t\t\t\t\t\tAvailable Elections to Register In:\n";
    for (int i = 0; i < availableCount; ++i) {
        cout << "\t\t\t\t\t\t" << (i + 1) << ". ID: " << availableElections[i]->getElectionID()
            << " (" << availableElections[i]->getElectionType();
        string geoScope = availableElections[i]->getGeographicScope();
        if (availableElections[i]->getElectionType() != "National" && !geoScope.empty()) cout << " - Scope: " << geoScope;
        else if (availableElections[i]->getElectionType() == "National") cout << " - Scope: Nationwide";
        cout << ")\n";
    }

    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\tEnter election number to register for (or '0' to cancel): ";
    string choiceStr;
    getline(cin, choiceStr);
    int choiceNum = -1;

    if (choiceStr.length() > 0 && containsOnlyDigits(choiceStr)) {
        try { choiceNum = stoi(choiceStr); }
        catch (...) { choiceNum = -1; }
    }

    string chosenElectionID = "NONE";
    if (choiceNum > 0 && choiceNum <= availableCount) {
        chosenElectionID = availableElections[choiceNum - 1]->getElectionID();
    }
    else if (choiceNum != 0) {
        invalidEntry();
    }

    if (chosenElectionID != "NONE") {
        string userLines[MAX_FILE_LINES];
        int userCount = readFileToArray(USER_CREDENTIALS_FILE_PATH, userLines, MAX_FILE_LINES);
        int userIndex = -1;
        for (int i = 0; i < userCount; ++i) {
            if (!userLines[i].empty() && userLines[i].substr(0, userLines[i].find('-')) == userProfile.getCnic()) {
                userIndex = i;
                break;
            }
        }

        if (userIndex != -1) {
            string userFields[USER_CRED_FIELD_COUNT];
            if (adminParseLine(userLines[userIndex], '-', userFields, USER_CRED_FIELD_COUNT) == USER_CRED_FIELD_COUNT) {
                userFields[USER_CRED_ISPARTICIPATING_IDX] = "1";
                userFields[USER_CRED_PARTICIPELECID_IDX] = chosenElectionID;
                userLines[userIndex] = adminConstructLine(userFields, USER_CRED_FIELD_COUNT, '-');

                if (writeArrayToFile(USER_CREDENTIALS_FILE_PATH, userLines, userCount)) {
                    this->isParticipating_flag = true;
                    this->participatingElectionID_val = chosenElectionID;
                    displayMessage("Successfully registered for election ID: " + chosenElectionID, false);
                }
                else {
                    displayMessage("Error: Failed to update your registration status.", true);
                }
            }
            else {
                displayMessage("Error: Could not parse your user data.", true);
            }
        }
        else {
            displayMessage("Error: Could not find your user record.", true);
        }
    }
    else if (choiceNum == 0) {
        displayMessage("Registration cancelled.", false);
    }

    for (int i = 0; i < availableCount; ++i) {
        if (availableElections[i]) delete availableElections[i];
    }
}


void User::unregisterFromElection() {
    if (!isParticipating_flag) {
        displayMessage("You are not currently registered in any election.", false);
        return;
    }

    string currentElectionID = participatingElectionID_val;
    string electionStatus = "Unknown";

    ifstream electionsFile(ELECTIONS_FILE_PATH);
    if (electionsFile.is_open()) {
        string line;
        string id, type, statusFile, endDateStr, scope;
        while (getline(electionsFile, line)) {
            if (Election::parseElectionLine(line, id, type, statusFile, endDateStr, scope)) {
                if (id == currentElectionID) {
                    electionStatus = statusFile;
                    break;
                }
            }
        }
        electionsFile.close();
    }
    else {
        displayMessage("Error: Cannot verify election status.", true);
        return;
    }

    if (electionStatus != "Upcoming") {
        displayMessage("Cannot unregister from election ID " + currentElectionID + " (Status: " + electionStatus + ").", true);
        return;
    }

    string userLines[MAX_FILE_LINES];
    int userCount = readFileToArray(USER_CREDENTIALS_FILE_PATH, userLines, MAX_FILE_LINES);
    int userIndex = -1;
    for (int i = 0; i < userCount; ++i) {
        if (!userLines[i].empty() && userLines[i].substr(0, userLines[i].find('-')) == userProfile.getCnic()) {
            userIndex = i;
            break;
        }
    }

    if (userIndex != -1) {
        string userFields[USER_CRED_FIELD_COUNT];
        if (adminParseLine(userLines[userIndex], '-', userFields, USER_CRED_FIELD_COUNT) == USER_CRED_FIELD_COUNT) {
            userFields[USER_CRED_ISPARTICIPATING_IDX] = "0";
            userFields[USER_CRED_PARTICIPELECID_IDX] = "NONE";
            userLines[userIndex] = adminConstructLine(userFields, USER_CRED_FIELD_COUNT, '-');

            if (writeArrayToFile(USER_CREDENTIALS_FILE_PATH, userLines, userCount)) {
                this->isParticipating_flag = false;
                this->participatingElectionID_val = "NONE";
                displayMessage("Successfully unregistered from election ID: " + currentElectionID, false);
            }
            else {
                displayMessage("Error: Failed to update your registration status.", true);
            }
        }
        else {
            displayMessage("Error: Could not parse your user data.", true);
        }
    }
    else {
        displayMessage("Error: Could not find your user record.", true);
    }
}


void User::partyManagementMenu() {
    string entry; int choice;
    while (true) {
        system("cls");
        cout << "\t\t\t\t\t\t......................................................................\n";
        cout << "\t\t\t\t\t\t..............P A R T Y   M A N A G E M E N T   M E N U...............\n";
        cout << "\t\t\t\t\t\t......................................................................\n";

        int currentPartyOption = 1;
        cout << "\t\t\t\t\t\t..                     '" << currentPartyOption++ << "' Register in Party                      ..\n";
        cout << "\t\t\t\t\t\t..                     '" << currentPartyOption++ << "' Unregister from Party                  ..\n";
        cout << "\t\t\t\t\t\t..                     '" << currentPartyOption++ << "' View Party Details                     ..\n";
        cout << "\t\t\t\t\t\t..                     '" << currentPartyOption++ << "' View Party Members                     ..\n";

        int modifyPartyDetailsOption = -1;
        int addMemberOption = -1;
        int removeMemberOption = -1;

        if (isPresident_flag) {
            modifyPartyDetailsOption = currentPartyOption++;
            addMemberOption = currentPartyOption++;
            removeMemberOption = currentPartyOption++;
            cout << "\t\t\t\t\t\t..                     '" << modifyPartyDetailsOption << "' Modify Party Details                 ..\n";
            cout << "\t\t\t\t\t\t..                     '" << addMemberOption << "' Add Member to Party                  ..\n";
            cout << "\t\t\t\t\t\t..                     '" << removeMemberOption << "' Remove Member from Party             ..\n";
        }

        cout << "\t\t\t\t\t\t..                     '0' Go Back (User Dashboard)                 ..\n";
        cout << "\t\t\t\t\t\t......................................................................\n";
        cout << "\t\t\t\t\t\t                      Enter your choice here: ";
        getline(cin, entry, '\n');

        if (entry.length() == 1 && isdigit(entry.at(0))) {
            try { choice = stoi(entry); }
            catch (...) { invalidEntry(); continue; }

            if (choice == 0) return;

            switch (choice) {
            case 1: registerInParty(); break;
            case 2: unregisterFromParty(); break;
            case 3: viewPartyDetails(); break;
            case 4: viewPartyMembers(); break;
            default:
                if (isPresident_flag) {
                    if (choice == modifyPartyDetailsOption) modifyPartyDetails();
                    else if (choice == addMemberOption) addMemberToParty();
                    else if (choice == removeMemberOption) removeMemberFromParty();
                    else invalidEntry();
                }
                else {
                    invalidEntry();
                }
                break;
            }
        }
        else { invalidEntry(); }
    }
}

void User::registerInParty() {
    if (partyID_val != DEFAULT_PARTY_ID && !partyID_val.empty()) {
        displayMessage("You are already registered with party ID: " + partyID_val, false);
        return;
    }
    if (!isRegisteredCandidate_flag) {
        displayMessage("You must be a registered candidate to join a party.", true);
        return;
    }

    string targetPartyID;
    cout << "\t\t\t\t\t\tEnter the 4-digit ID of the party you wish to join: ";
    getline(cin, targetPartyID);

    if (targetPartyID.length() != 4 || !containsOnlyDigits(targetPartyID)) {
        displayMessage("Invalid Party ID format. Must be 4 digits.", true);
        return;
    }

    bool partyExists = false;
    ifstream partiesFile(PARTIES_FILE_PATH);
    if (!partiesFile.is_open()) {
        displayMessage("Error: Cannot access parties file.", true);
        return;
    }
    string pLine;
    while (getline(partiesFile, pLine)) {
        if (!pLine.empty() && pLine.substr(0, pLine.find('-')) == targetPartyID) {
            partyExists = true;
            break;
        }
    }
    partiesFile.close();

    if (!partyExists) {
        displayMessage("Party with ID '" + targetPartyID + "' not found.", true);
        return;
    }

    string lines[MAX_FILE_LINES];
    ifstream inFile(USER_CREDENTIALS_FILE_PATH);
    if (!inFile.is_open()) {
        displayMessage("Error: Cannot access user credentials.", true);
        return;
    }
    int count = 0;
    string line;
    bool userFound = false;
    int userLineIndex = -1;

    while (getline(inFile, line) && count < MAX_FILE_LINES) {
        lines[count] = line;
        if (!line.empty() && line.substr(0, line.find('-')) == userProfile.getCnic()) {
            userFound = true;
            userLineIndex = count;
        }
        count++;
    }
    inFile.close();

    if (!userFound) {
        displayMessage("Error: Your user record not found in credentials.", true);
        return;
    }

    string userFields[USER_CRED_FIELD_COUNT];
    if (adminParseLine(lines[userLineIndex], '-', userFields, USER_CRED_FIELD_COUNT) == USER_CRED_FIELD_COUNT) {
        userFields[USER_CRED_PARTYID_IDX] = targetPartyID;
        userFields[USER_CRED_ISDEPENDENTCANDIDATE_IDX] = string(1, IS_CANDIDATE_TRUE);
        userFields[USER_CRED_ISPRESIDENT_IDX] = string(1, IS_CANDIDATE_FALSE);

        string newLine = "";
        for (int i = 0; i < USER_CRED_FIELD_COUNT; ++i) {
            newLine += userFields[i] + (i == USER_CRED_FIELD_COUNT - 1 ? "" : "-");
        }
        lines[userLineIndex] = newLine;
    }
    else {
        displayMessage("Error parsing your credential data.", true);
        return;
    }

    ofstream outFile(USER_CREDENTIALS_FILE_PATH, ios::trunc);
    if (!outFile.is_open()) {
        displayMessage("Error: Cannot update user credentials.", true);
        return;
    }
    for (int i = 0; i < count; ++i) {
        outFile << lines[i] << endl;
    }
    outFile.close();

    partyID_val = targetPartyID;
    isDependentCandidate_flag = true;
    isPresident_flag = false;
    displayMessage("Successfully registered with party ID: " + targetPartyID, false);
}

void User::unregisterFromParty() {
    if (partyID_val == DEFAULT_PARTY_ID || partyID_val.empty()) {
        displayMessage("You are not currently registered with any party.", false);
        return;
    }
    if (isPresident_flag) {
        displayMessage("Party presidents cannot unregister. Contact Admin.", true);
        return;
    }

    string lines[MAX_FILE_LINES];
    ifstream inFile(USER_CREDENTIALS_FILE_PATH);
    if (!inFile.is_open()) {
        displayMessage("Error: Cannot access user credentials.", true);
        return;
    }
    int count = 0;
    string line;
    bool userFound = false;
    int userLineIndex = -1;

    while (getline(inFile, line) && count < MAX_FILE_LINES) {
        lines[count] = line;
        if (!line.empty() && line.substr(0, line.find('-')) == userProfile.getCnic()) {
            userFound = true;
            userLineIndex = count;
        }
        count++;
    }
    inFile.close();

    if (!userFound) {
        displayMessage("Error: Your user record not found in credentials.", true);
        return;
    }

    string oldPartyIDCurrent = partyID_val;

    string userFields[USER_CRED_FIELD_COUNT];
    if (adminParseLine(lines[userLineIndex], '-', userFields, USER_CRED_FIELD_COUNT) == USER_CRED_FIELD_COUNT) {
        userFields[USER_CRED_PARTYID_IDX] = DEFAULT_PARTY_ID;
        userFields[USER_CRED_ISPRESIDENT_IDX] = string(1, IS_CANDIDATE_FALSE);
        userFields[USER_CRED_ISDEPENDENTCANDIDATE_IDX] = string(1, IS_CANDIDATE_FALSE);

        string newLine = "";
        for (int i = 0; i < USER_CRED_FIELD_COUNT; ++i) {
            newLine += userFields[i] + (i == USER_CRED_FIELD_COUNT - 1 ? "" : "-");
        }
        lines[userLineIndex] = newLine;
    }
    else {
        displayMessage("Error parsing your credential data.", true);
        return;
    }

    ofstream outFile(USER_CREDENTIALS_FILE_PATH, ios::trunc);
    if (!outFile.is_open()) {
        displayMessage("Error: Cannot update user credentials.", true);
        return;
    }
    for (int i = 0; i < count; ++i) {
        outFile << lines[i] << endl;
    }
    outFile.close();

    displayMessage("Successfully unregistered from party ID: " + oldPartyIDCurrent, false);
    partyID_val = DEFAULT_PARTY_ID;
    isPresident_flag = false;
    isDependentCandidate_flag = false;
}


void User::viewPartyDetails() {
    displayMessage("Party Feature: View Party Details - Under Development.", false);
}
void User::viewPartyMembers() {
    displayMessage("Party Feature: View Party Members - Under Development.", false);
}

void User::modifyPartyDetails() {
    displayMessage("President Feature: Modify Party Details - Under Development.", false);
}
void User::addMemberToParty() {
    displayMessage("President Feature: Add Member to Party - Under Development.", false);
}
void User::removeMemberFromParty() {
    displayMessage("President Feature: Remove Member from Party - Under Development.", false);
}