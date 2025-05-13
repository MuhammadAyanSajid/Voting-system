#include "Authorization.h"
#include "Profile.h"
#include "Admin.h"
#include "User.h"
#include "PoliticalParty.h"
#include "DateTime.h"


const string USER_CRED_FILE_PATH = "user_credentials.txt";


void invalidEntry() {
    system("cls");
    cout << "\t\t\t\t\t\t......................................................................" << endl;
    cout << "\t\t\t\t\t\t......................I N V A L I D   E N T R Y......................." << endl;
    cout << "\t\t\t\t\t\t......................................................................" << endl;
    cout << "\t\t\t\t\t\t..                   Please enter a valid choice.                   .." << endl;
    cout << "\t\t\t\t\t\t......................................................................" << endl;
    Sleep(1500);
}

void displayMessage(const string& msg, bool error) {
    system("cls");
    cout << "\t\t\t\t\t\t......................................................................\n";
    if (error) {
        cout << "\t\t\t\t\t\t................................ E R R O R ...........................\n";
    }
    else {
        cout << "\t\t\t\t\t\t............................. N O T I C E ............................\n";
    }
    cout << "\t\t\t\t\t\t......................................................................\n";
    int spaces_needed = 66;
    if (msg.length() < 66) {
        spaces_needed = 66 - msg.length();
    }
    else {
        spaces_needed = 0;
    }
    string spaces(spaces_needed / 2, ' ');
    cout << "\t\t\t\t\t\t.." << spaces << msg.substr(0, 64) << spaces << (spaces_needed % 2 != 0 ? " " : "") << "..\n";
    cout << "\t\t\t\t\t\t......................................................................\n";
    Sleep(2000);
}

void displayLoginError(const string& msg) {
    system("cls");
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t........................ L O G I N   F A I L E D .....................\n";
    cout << "\t\t\t\t\t\t......................................................................\n";
    int spaces_needed = 66;
    if (msg.length() < 66) {
        spaces_needed = 66 - msg.length();
    }
    else {
        spaces_needed = 0;
    }
    string spaces(spaces_needed / 2, ' ');
    cout << "\t\t\t\t\t\t.." << spaces << msg.substr(0, 64) << spaces << (spaces_needed % 2 != 0 ? " " : "") << "..\n";
    cout << "\t\t\t\t\t\t......................................................................\n";
    Sleep(2500);
}

bool containsOnlyDigits(const string& s) {
    if (s.empty()) return false;
    for (char c : s) { if (!isdigit(c)) return false; } return true;
}

bool containsOnlyAlpha(const string& s) {
    if (s.empty()) return false;
    for (char c : s) { if (!isalpha(c)) return false; } return true;
}

bool containsOnlyAlphaAndSpace(const string& s) {
    if (s.empty()) return false;
    for (char c : s) { if (!isalpha(c) && !isspace(c)) return false; } return true;
}

void displayMainPanel() {
    system("cls"); Sleep(250);
    cout << "\t\t\t\t\t\t......................................................................\n\t\t\t\t\t\t...............O N L I N E   V O T I N G   S Y S T E M................\n\t\t\t\t\t\t......................................................................\n\t\t\t\t\t\t..                     '1' Proceed to Access Panel                  ..\n\t\t\t\t\t\t..                     '2' View Developers                          ..\n\t\t\t\t\t\t..                     '0' Exit Application                         ..\n\t\t\t\t\t\t......................................................................\n\t\t\t\t\t\t                       Enter your choice here: ";
}

void developersPage() {
    string entry; int choice;
    while (true) {
        system("cls"); Sleep(250);
        cout << "\t\t\t\t\t\t......................................................................\n";
        cout << "\t\t\t\t\t\t..........................D E V E L O P E R S.........................\n";
        cout << "\t\t\t\t\t\t......................................................................\n";
        cout << "\t\t\t\t\t\t..                         Zain Khalid                              ..\n";
        cout << "\t\t\t\t\t\t..                         Minahil Ehsan                            ..\n";
        cout << "\t\t\t\t\t\t..                         Muhammad Ayan Sajid                      ..\n";
        cout << "\t\t\t\t\t\t......................................................................\n";
        cout << "\t\t\t\t\t\t..                          '0' Go Back                             ..\n";
        cout << "\t\t\t\t\t\t......................................................................\n";
        cout << "\t\t\t\t\t\t                       Enter your choice here: ";
        getline(cin, entry, '\n');
        if (entry.length() == 1 && isdigit(entry.at(0))) {
            try { choice = stoi(entry); }
            catch (...) { invalidEntry(); continue; }
            if (choice == 0) { mainPanel(); return; }
            else { invalidEntry(); }
        }
        else { invalidEntry(); }
    }
}

void mainPanel() {
    string entry; int choice;
    while (true) {
        displayMainPanel(); getline(cin, entry, '\n');
        if (entry.length() == 1 && isdigit(entry.at(0))) {
            try { choice = stoi(entry); }
            catch (...) { invalidEntry(); continue; }
            switch (choice) {
            case 1: accessPanel(); return;
            case 2: developersPage(); return;
            case 0: FreeConsole(); exit(0);
            default: invalidEntry(); break;
            }
        }
        else { invalidEntry(); }
    }
}

void displayAccessPanel() {
    system("cls"); Sleep(250);
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t...............O N L I N E   V O T I N G   S Y S T E M................\n";
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t..                          '1' Login                               ..\n";
    cout << "\t\t\t\t\t\t..                          '2' Sign Up                             ..\n";
    cout << "\t\t\t\t\t\t..                          '0' Main Menu                           ..\n";
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t                        Enter your choice here: ";
}

void accessPanel() {
    string entry; int choice;
    while (true) {
        displayAccessPanel(); getline(cin, entry, '\n');
        if (entry.length() == 1 && isdigit(entry.at(0))) {
            try { choice = stoi(entry); }
            catch (...) { invalidEntry(); continue; }
            switch (choice) {
            case 1: loginPanel(); return;
            case 2: signUpPanel(); return;
            case 0: mainPanel(); return;
            default: invalidEntry(); break;
            }
        }
        else { invalidEntry(); }
    }
}

void displayLogin_SignUp() {
    system("cls"); Sleep(250);
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t........................W H O   A R E   Y O U.........................\n";
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t..                          '1' Admin                               ..\n";
    cout << "\t\t\t\t\t\t..                          '2' User                                ..\n";
    cout << "\t\t\t\t\t\t..                          '3' Go Back                             ..\n";
    cout << "\t\t\t\t\t\t..                          '0' Main Menu                           ..\n";
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t                       Enter your choice here: ";
}

void signUpPanel() {
    string entry; int choice;
    while (true) {
        system("cls"); Sleep(250);
        cout << "\t\t\t\t\t\t......................................................................\n";
        cout << "\t\t\t\t\t\t..........................S I G N   U P   A S.........................\n";
        cout << "\t\t\t\t\t\t......................................................................\n";
        cout << "\t\t\t\t\t\t..                          '1' User                                ..\n";
        cout << "\t\t\t\t\t\t..                          '2' Go Back                             ..\n";
        cout << "\t\t\t\t\t\t..                          '0' Main Menu                           ..\n";
        cout << "\t\t\t\t\t\t......................................................................\n";
        cout << "\t\t\t\t\t\t                       Enter your choice here: ";
        getline(cin, entry, '\n');
        if (entry.length() == 1 && isdigit(entry.at(0))) {
            try { choice = stoi(entry); }
            catch (...) { invalidEntry(); continue; }
            switch (choice) {
            case 1: UserSignUp().signUp(); accessPanel(); return;
            case 2: accessPanel(); return;
            case 0: mainPanel(); return;
            default: invalidEntry(); break;
            }
        }
        else { invalidEntry(); }
    }
}

void loginPanel() {
    string entry; int choice;
    while (true) {
        displayLogin_SignUp();
        getline(cin, entry, '\n');
        if (entry.length() == 1 && isdigit(entry.at(0))) {
            try { choice = stoi(entry); }
            catch (...) { invalidEntry(); continue; }
            switch (choice) {
            case 1: AdminLogin().login(); return;
            case 2: UserLogin().login(); return;
            case 3: accessPanel(); return;
            case 0: mainPanel(); return;
            default: invalidEntry(); break;
            }
        }
        else { invalidEntry(); }
    }
}

void hidePasswordInput(string& password) {
    char ch; password.clear();
    while ((ch = _getch()) != '\r') {
        if (ch == '\b') { if (!password.empty()) { password.pop_back(); cout << "\b \b"; } }
        else if (isprint(ch) && password.length() < 30) { password.push_back(ch); cout << "*"; }
    } cout << endl;
}

void getAdminLogin(string& cnic, string& password) {
    system("cls");
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t.........................A D M I N   L O G I N........................\n";
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\tEnter CNIC (13 digits, no dashes): "; getline(cin, cnic, '\n');
    cout << "\t\t\t\t\t\tEnter Password: "; hidePasswordInput(password);
}

void getUserLogin(string& cnic, string& password) {
    system("cls");
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t..........................U S E R   L O G I N.........................\n";
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\tEnter CNIC (13 digits, no dashes): "; getline(cin, cnic, '\n');
    cout << "\t\t\t\t\t\tEnter Password: "; hidePasswordInput(password);
}

void loadingScreen() {
    system("cls");
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t............................L O A D I N G.............................\n";
    cout << "\t\t\t\t\t\t";
    for (int i = 0; i < 70; ++i) { cout << "."; Sleep(20); }
    cout << endl; system("cls");
}

void invalidCredentials() {
    displayMessage("Invalid CNIC or Password.", true);
}

bool isFileEmpty(const string& filePath) {
    ifstream file(filePath);
    return !file.is_open() || file.peek() == ifstream::traits_type::eof();
}

void displayPasswordError(const string& message) {
    displayMessage("Password Error: " + message, true);
}

void populateApprovalFile() {
    ofstream approvFile(APPROVAL_FILE, ios::trunc);
    if (!approvFile.is_open()) {
        displayMessage("Error: Cannot create/clear approval file.", true);
        return;
    }
    string credFiles[] = { USER_CRED_FILE_PATH };
    string line;
    for (const string& credFile : credFiles) {
        ifstream inFile(credFile);
        if (inFile.is_open()) {
            string fields[USER_CRED_FIELD_COUNT];
            while (getline(inFile, line)) {
                if (adminParseLine(line, '-', fields, USER_CRED_FIELD_COUNT) == USER_CRED_FIELD_COUNT) {
                    if (fields[USER_CRED_STATUS_IDX].length() > 0 && fields[USER_CRED_STATUS_IDX][0] == PENDING_STATUS) {
                        approvFile << fields[USER_CRED_CNIC_IDX] << "-" << PENDING_STATUS << endl;
                    }
                }
            } inFile.close();
        }
    } approvFile.close();
}

int AdminLogin::checkLogin(const string& cnic, const string& password, ifstream& logFile) {
    logFile.clear(); logFile.seekg(0, ios::beg);
    string line;
    string adminCredFields[ADMIN_CRED_FIELD_COUNT];
    while (getline(logFile, line)) {
        if (adminParseLine(line, '-', adminCredFields, ADMIN_CRED_FIELD_COUNT) == ADMIN_CRED_FIELD_COUNT) {
            if (cnic == adminCredFields[ADMIN_CRED_CNIC_IDX] && password == adminCredFields[ADMIN_CRED_PASS_IDX]) {
                this->userCNIC = adminCredFields[ADMIN_CRED_CNIC_IDX];
                this->name = adminCredFields[ADMIN_CRED_FNAME_IDX] + " " + adminCredFields[ADMIN_CRED_LNAME_IDX];
                return 1;
            }
        }
    }
    return -1;
}

void AdminLogin::login() {
    string credFile = ADMIN_CREDENTIALS_FILE_PATH;
    string dataFile = ADMIN_DATA_FILE_PATH;
    ifstream logFile(credFile);
    if (!logFile.is_open()) { displayMessage("Admin credentials file inaccessible.", true); displayMessage("Admin login unavailable.", false); accessPanel(); return; }
    ofstream checkData(dataFile, ios::app); checkData.close();
    if (isFileEmpty(credFile)) { displayMessage("No Admin account configured.", true); displayMessage("Admin login unavailable.", false); logFile.close(); accessPanel(); return; }
    string cnic, password;
    while (true) {
        getAdminLogin(cnic, password);
        int loginStatus = checkLogin(cnic, password, logFile);
        if (loginStatus == 1) {
            logFile.close();
            loadingScreen();
            Admin adminUser;
            if (adminUser.loadAdminProfile(this->userCNIC, dataFile)) {
                adminUser.updateLoginTimestamp(dataFile);
                adminUser.Dashboard();
            }
            else {
                displayMessage("Failed to load admin profile data.", true);
                loginPanel();
            }
            return;
        }
        else {
            invalidCredentials();
        }
    }
}

int UserLogin::checkLogin(const string& cnic, const string& password, ifstream& logFile) {
    logFile.clear(); logFile.seekg(0, ios::beg);
    string line;
    string userCredFields[USER_CRED_FIELD_COUNT];
    while (getline(logFile, line)) {
        if (adminParseLine(line, '-', userCredFields, USER_CRED_FIELD_COUNT) == USER_CRED_FIELD_COUNT) {
            if (cnic == userCredFields[USER_CRED_CNIC_IDX] && password == userCredFields[USER_CRED_PASS_IDX]) {
                if (userCredFields[USER_CRED_STATUS_IDX].length() > 0 && userCredFields[USER_CRED_STATUS_IDX][0] == ACTIVE_STATUS) {
                    this->userCNIC = userCredFields[USER_CRED_CNIC_IDX];
                    this->name = userCredFields[USER_CRED_FNAME_IDX] + " " + userCredFields[USER_CRED_LNAME_IDX];
                    this->isAlsoCandidate = (userCredFields[USER_CRED_ISCANDIDATE_IDX] == "1");
                    this->partyID = userCredFields[USER_CRED_PARTYID_IDX];
                    this->isPresident = (userCredFields[USER_CRED_ISPRESIDENT_IDX] == "1");
                    this->isDependentCandidate = (userCredFields[USER_CRED_ISDEPENDENTCANDIDATE_IDX] == "1");
                    this->isParticipating = (userCredFields[USER_CRED_ISPARTICIPATING_IDX] == "1");
                    this->participatingElectionID = userCredFields[USER_CRED_PARTICIPELECID_IDX];
                    return 1;
                }
                else {
                    return 0;
                }
            }
        }
    }
    return -1;
}

void UserLogin::login() {
    string credFile = USER_CRED_FILE_PATH;
    string dataFile = USER_DATA_FILE_PATH;
    ifstream logFile(credFile);
    if (!logFile.is_open()) { displayMessage("User credentials file inaccessible.", true); accessPanel(); return; }
    ofstream checkData(dataFile, ios::app); checkData.close();
    if (isFileEmpty(credFile)) { displayMessage("No User accounts exist. Please Sign Up.", false); logFile.close(); UserSignUp().signUp(); accessPanel(); return; }
    string cnic, password;
    while (true) {
        getUserLogin(cnic, password);
        int loginStatus = checkLogin(cnic, password, logFile);
        if (loginStatus == 1) {
            logFile.close();
            loadingScreen();
            User userObject;
            if (userObject.loadUserProfile(this->userCNIC, dataFile)) {
                userObject.updateLoginTimestamp(dataFile);
                userObject.setCandidateStatus(this->isAlsoCandidate);
                userObject.setPartyID(this->partyID);
                userObject.setIsPresident(this->isPresident);
                userObject.setIsDependentCandidate(this->isDependentCandidate);
                userObject.setIsParticipating(this->isParticipating);
                userObject.setParticipatingElectionID(this->participatingElectionID);
                userObject.Dashboard();
            }
            else {
                displayMessage("Failed to load user profile data.", true);
                loginPanel();
            }
            return;
        }
        else if (loginStatus == 0) {
            displayLoginError("Account requires admin approval."); loginPanel(); return;
        }
        else {
            invalidCredentials();
        }
    }
}

void SignUpDashboard::displaySignupHeader(const string& role, int page) {
    system("cls");
    cout << "\t\t\t\t\t\t......................................................................\n";
    string title = role + "   S I G N   U P   (Page " + to_string(page) + "/4)";
    int spaces_needed = 66 - title.length(); string spaces(spaces_needed / 2, ' ');
    cout << "\t\t\t\t\t\t.." << spaces << title << spaces << (spaces_needed % 2 != 0 ? " " : "") << "..\n";
    cout << "\t\t\t\t\t\t......................................................................\n";
}

void SignUpDashboard::getBasicInfo(string& fName, string& lName, string& ageStr, bool& fNameValid, bool& lNameValid, bool& ageValid) {
    if (!fNameValid) { cout << "\t\t\t\t\t\tEnter First Name: "; getline(cin, fName, '\n'); }
    else { cout << "\t\t\t\t\t\tFirst Name: " << fName << " (OK)\n"; }
    if (!lNameValid) { cout << "\t\t\t\t\t\tEnter Last Name: "; getline(cin, lName, '\n'); }
    else { cout << "\t\t\t\t\t\tLast Name: " << lName << " (OK)\n"; }
    if (!ageValid) { cout << "\t\t\t\t\t\tEnter Age: "; getline(cin, ageStr, '\n'); }
    else { cout << "\t\t\t\t\t\tAge: " << ageStr << " (OK)\n"; }
    cout << "\t\t\t\t\t\t......................................................................\n";
}

void SignUpDashboard::getDetailedInfo(string& cnic, string& phone, string& gender, string& state, string& city, string& town, string& postalCode, bool& cnicValid, bool& phoneValid, bool& genderValid, bool& stateValid, bool& cityValid, bool& townValid, bool& postalValid) {
    if (!cnicValid) { cout << "\t\t\t\t\t\tEnter CNIC (13 digits, no dashes): "; getline(cin, cnic, '\n'); }
    else { cout << "\t\t\t\t\t\tCNIC: " << cnic << " (OK)\n"; }
    if (!phoneValid) { cout << "\t\t\t\t\t\tEnter Phone (+92XXXXXXXXXX): "; getline(cin, phone, '\n'); }
    else { cout << "\t\t\t\t\t\tPhone: " << phone << " (OK)\n"; }
    if (!genderValid) { cout << "\t\t\t\t\t\tEnter Gender (e.g., Male, Female): "; getline(cin, gender, '\n'); }
    else { cout << "\t\t\t\t\t\tGender: " << gender << " (OK)\n"; }
    if (!stateValid) { cout << "\t\t\t\t\t\tEnter State/Region: "; getline(cin, state, '\n'); }
    else { cout << "\t\t\t\t\t\tState/Region: " << state << " (OK)\n"; }
    if (!cityValid) { cout << "\t\t\t\t\t\tEnter City: "; getline(cin, city, '\n'); }
    else { cout << "\t\t\t\t\t\tCity: " << city << " (OK)\n"; }
    if (!townValid) { cout << "\t\t\t\t\t\tEnter Town/Area: "; getline(cin, town, '\n'); }
    else { cout << "\t\t\t\t\t\tTown/Area: " << town << " (OK)\n"; }
    if (!postalValid) { cout << "\t\t\t\t\t\tEnter Postal Code (5 digits): "; getline(cin, postalCode, '\n'); }
    else { cout << "\t\t\t\t\t\tPostal Code: " << postalCode << " (OK)\n"; }
    cout << "\t\t\t\t\t\t......................................................................\n";
}

void SignUpDashboard::getPasswordInfo(string& pass1, string& pass2, bool& passValid, bool& passMatch) {
    cout << "\t\t\t\t\t\tEnter Password: "; hidePasswordInput(pass1);
    cout << "\t\t\t\t\t\tRe-enter Password: "; hidePasswordInput(pass2);
    cout << "\t\t\t\t\t\t......................................................................\n";
}

void SignUpDashboard::getPartyAffiliation(string& partyID) {
    string choice;
    while (true) {
        cout << "\t\t\t\t\t\tDo you want to affiliate with a political party? (y/n): ";
        getline(cin, choice);
        if (choice == "y" || choice == "Y") {
            while (true) {
                cout << "\t\t\t\t\t\tEnter the 4-digit Party ID to join (or 'NONE' for no party): ";
                string enteredPartyID;
                getline(cin, enteredPartyID);

                PoliticalParty tempParty;
                if (isValidPartyIDForSignup(enteredPartyID, tempParty)) {
                    partyID = enteredPartyID;
                    cout << "\t\t\t\t\t\tParty Affiliation set to: " << partyID << endl;
                    cout << "\t\t\t\t\t\t......................................................................\n";
                    return;
                }
                else {
                    displayMessage("Invalid Party ID or Party does not exist. Try again or 'NONE'.", true);
                }
            }
        }
        else if (choice == "n" || choice == "N") {
            partyID = DEFAULT_PARTY_ID;
            cout << "\t\t\t\t\t\tParty Affiliation set to: " << partyID << endl;
            cout << "\t\t\t\t\t\t......................................................................\n";
            return;
        }
        else {
            displayMessage("Invalid choice. Please enter 'y' or 'n'.", true);
        }
    }
}

bool SignUpDashboard::isValidName(const string& name) {
    if (name.empty()) { displayMessage("Name cannot be empty.", true); return false; }
    if (!containsOnlyAlphaAndSpace(name) || name.find('-') != string::npos) { displayMessage("Name must contain only letters and spaces.", true); return false; }
    if (name.length() > 50) { displayMessage("Name is too long (max 50 characters).", true); return false; }
    if (name.front() == ' ' || name.back() == ' ') { displayMessage("Name cannot start or end with a space.", true); return false; }
    if (name.find("  ") != string::npos) { displayMessage("Name cannot contain multiple spaces together.", true); return false; }
    return true;
}

bool SignUpDashboard::isValidAge(const string& ageStr, int& age) {
    if (!containsOnlyDigits(ageStr)) { displayMessage("Age must contain only digits.", true); return false; }
    try { age = stoi(ageStr); }
    catch (...) { displayMessage("Invalid age format.", true); return false; }
    if (age <= 0 || age > 120) { displayMessage("Age must be between 1 and 120.", true); return false; }
    return true;
}

bool SignUpDashboard::isEligible(int age) { return age >= 18; }

bool SignUpDashboard::isValidCNIC(const string& cnic) {
    if (!containsOnlyDigits(cnic)) { displayMessage("CNIC must contain only digits.", true); return false; }
    if (cnic.length() != 13) { displayMessage("CNIC must be exactly 13 digits.", true); return false; }
    return true;
}

bool SignUpDashboard::isCNICUnique(const string& cnic) {
    string files[] = { USER_CRED_FILE_PATH, ADMIN_CREDENTIALS_FILE_PATH };
    for (const string& file : files) {
        ifstream logFile(file);
        if (logFile.is_open()) {
            string line, storedCnic;
            while (getline(logFile, line)) {
                size_t d1 = line.find('-'); if (d1 != string::npos) {
                    storedCnic = line.substr(0, d1); if (cnic == storedCnic) { logFile.close(); displayMessage("CNIC (" + cnic + ") is already registered.", true); return false; }
                }
            } logFile.close();
        }
    } return true;
}

bool SignUpDashboard::isPhoneUnique(const string& phone) {
    string files[] = { USER_DATA_FILE_PATH, ADMIN_DATA_FILE_PATH };
    for (const string& file : files) {
        ifstream dataFile(file);
        if (dataFile.is_open()) {
            string line;
            string dataFields[DATA_FIELD_COUNT];
            while (getline(dataFile, line)) {
                if (adminParseLine(line, '-', dataFields, DATA_FIELD_COUNT) == DATA_FIELD_COUNT) {
                    if (phone == dataFields[DATA_PHONE_IDX]) {
                        dataFile.close();
                        displayMessage("Phone number (" + phone + ") already registered.", true);
                        return false;
                    }
                }
            } dataFile.close();
        }
    } return true;
}

bool SignUpDashboard::isValidPhoneNumber(const string& phone) {
    if (phone.length() != 13) { displayMessage("Phone number must be 13 chars (+92XXXXXXXXXX).", true); return false; }
    if (phone.substr(0, 3) != "+92") { displayMessage("Phone number must start with '+92'.", true); return false; }
    if (!containsOnlyDigits(phone.substr(3))) { displayMessage("Phone number must contain only digits after '+92'.", true); return false; }
    return true;
}

bool SignUpDashboard::isValidPostalCode(const string& code) {
    if (!containsOnlyDigits(code)) { displayMessage("Postal code must contain only digits.", true); return false; }
    if (code.length() != 5) { displayMessage("Postal code must be exactly 5 digits.", true); return false; }
    return true;
}

bool SignUpDashboard::isValidGender(const string& gender) {
    if (gender.empty()) { displayMessage("Gender cannot be empty.", true); return false; }
    if (!containsOnlyAlpha(gender)) { displayMessage("Gender must contain only letters (e.g., Male, Female).", true); return false; }
    if (gender.length() > 20) { displayMessage("Gender is too long (max 20 characters).", true); return false; }
    return true;
}

bool SignUpDashboard::isValidGenericText(const string& text) {
    if (text.empty()) { displayMessage("Field cannot be empty.", true); return false; }
    if (text.find('-') != string::npos) { displayMessage("Field cannot contain hyphens (-).", true); return false; }
    if (text.length() > 100) { displayMessage("Field is too long (max 100 characters).", true); return false; }
    for (char c : text) { if (!isalnum(c) && !isspace(c) && c != '/' && c != ',' && c != '.') { displayMessage("Field contains invalid characters.", true); return false; } }
    if (text.front() == ' ' || text.back() == ' ') { displayMessage("Field cannot start or end with a space.", true); return false; }
    return true;
}

bool SignUpDashboard::isValidPassword(const string& password) {
    if (password.empty()) { displayPasswordError("Password cannot be empty."); return false; }
    if (password.find(' ') != string::npos || password.find('-') != string::npos) { displayPasswordError("Password cannot contain spaces or hyphens."); return false; }
    if (password.length() < 8) { displayPasswordError("Password must be at least 8 characters."); return false; }
    if (password.length() > 30) { displayPasswordError("Password is too long (max 30 characters)."); return false; }
    bool hasUpper = false, hasLower = false, hasDigit = false;
    for (char c : password) {
        if (isupper(c)) hasUpper = true; else if (islower(c)) hasLower = true; else if (isdigit(c)) hasDigit = true;
        else if (!isalnum(c)) { displayPasswordError("Password cannot contain special characters or symbols."); return false; }
    }
    if (!hasUpper) { displayPasswordError("Password must contain at least one uppercase letter."); return false; }
    if (!hasLower) { displayPasswordError("Password must contain at least one lowercase letter."); return false; }
    if (!hasDigit) { displayPasswordError("Password must contain at least one digit."); return false; }
    return true;
}

bool SignUpDashboard::isValidPartyIDForSignup(const string& partyID, PoliticalParty& foundParty) {
    if (partyID == DEFAULT_PARTY_ID) {
        return true;
    }
    if (partyID.length() != 4 || !containsOnlyDigits(partyID)) {
        return false;
    }
    ifstream inFile(PARTIES_FILE_PATH);
    if (!inFile.is_open()) {
        return false;
    }
    string line;
    while (getline(inFile, line)) {
        PoliticalParty temp;
        if (temp.fromString(line)) {
            if (temp.getPartyID() == partyID) {
                foundParty = temp;
                inFile.close();
                return true;
            }
        }
    }
    inFile.close();
    return false;
}


bool SignUpDashboard::saveCredentials(const string& filePath, const string& cnic, const string& fName, const string& lName, const string& password,
    char status, char isCandidateOverall, const string& partyId, char isPresident, char isDependentCandidate) {
    ofstream outputFile(filePath, ios::app);
    if (outputFile.is_open()) {
        outputFile << cnic << "-" << fName << "-" << lName << "-" << password << "-"
            << status << "-" << isCandidateOverall << "-" << partyId << "-"
            << isPresident << "-" << isDependentCandidate << "-"
            << IS_CANDIDATE_FALSE << "-" << DEFAULT_PARTY_ID << endl;
        outputFile.close(); return true;
    }
    else { displayMessage("Error opening credentials file for writing: " + filePath, true); return false; }
}

bool SignUpDashboard::addPendingApproval(const string& cnic) {
    ofstream approvFile(APPROVAL_FILE, ios::app); if (!approvFile.is_open()) { displayMessage("Error opening approval file.", true); return false; }
    approvFile << cnic << "-" << PENDING_STATUS << endl; approvFile.close(); return true;
}

bool SignUpDashboard::saveData(const string& filePath, const string& cnic, const string& fName, const string& lName, const string& age, const string& phone, const string& gender, const string& state, const string& city, const string& town, const string& postalCode, const string& signupTime) {
    ofstream dataFile(filePath, ios::app);
    if (dataFile.is_open()) { dataFile << cnic << "-" << fName << "-" << lName << "-" << age << "-" << phone << "-" << gender << "-" << state << "-" << city << "-" << town << "-" << postalCode << "-" << signupTime << endl; dataFile.close(); return true; }
    else { displayMessage("Error opening data file for writing: " + filePath, true); return false; }
}

void SignUpDashboard::signUpHelper(const string& role) {
    string credFilePath = USER_CREDENTIALS_FILE_PATH;
    string dataFilePath = USER_DATA_FILE_PATH;

    ofstream checkData(dataFilePath, ios::app); checkData.close();
    string fName, lName, ageStr, cnic, phone, gender, state, city, town, postalCode, pass1, pass2, partyID = DEFAULT_PARTY_ID;
    int currentAge = 0; int page = 1;
    bool fNameValid = false, lNameValid = false, ageValid = false, cnicValid = false, phoneValid = false, genderValid = false, stateValid = false, cityValid = false, townValid = false, postalValid = false, candidateChoiceValid = false, partyAffiliationTypeValid = false, partyIDValid = false;
    bool eligible = false, uniqueCNIC = false, uniquePhone = false, passValid = false, passMatch = false;
    bool allPage1Valid = false, allPage2Valid = false, allPage3Valid = false, allPage4Valid = false;
    char wantsToBeCandidateOverall = IS_CANDIDATE_FALSE;
    char isPresidentChar = IS_CANDIDATE_FALSE;
    char isDependentCandidateChar = IS_CANDIDATE_FALSE;


    while (page > 0 && page <= 4) {
        if (page == 1) {
            while (true) {
                displaySignupHeader(role, page);
                getBasicInfo(fName, lName, ageStr, fNameValid, lNameValid, ageValid);
                cout << "\t\t\t\t\t\t..                          '1' Proceed                             ..\n";
                cout << "\t\t\t\t\t\t..                          '0' Go Back (Cancel Signup)             ..\n";
                cout << "\t\t\t\t\t\t......................................................................\n";
                cout << "\t\t\t\t\t\t                       Enter your choice here: ";
                string entry; int choice = -1; getline(cin, entry, '\n');
                if (entry.length() == 1 && (entry == "0" || entry == "1")) { try { choice = stoi(entry); } catch (...) { choice = -1; } }
                if (choice == 0) { page = 0; break; }
                if (choice == 1) {
                    bool cFName = isValidName(fName); if (!cFName) Sleep(1500); else fNameValid = true;
                    bool cLName = isValidName(lName); if (!cLName) Sleep(1500); else lNameValid = true;
                    bool cAge = isValidAge(ageStr, currentAge); if (!cAge) Sleep(1500); else ageValid = true;
                    allPage1Valid = cFName && cLName && cAge;
                    if (allPage1Valid) {
                        eligible = isEligible(currentAge); if (!eligible) { displayMessage("You must be 18 or older to register.", true); fNameValid = lNameValid = ageValid = false; Sleep(2000); break; } page = 2; break;
                    }
                    else { displayMessage("Please correct the invalid fields on Page 1.", true); if (!cFName) fNameValid = false; if (!cLName) lNameValid = false; if (!cAge) ageValid = false; Sleep(2000); break; }
                }
                else { invalidEntry(); break; }
            } if (page == 0) break; if (page != 2) continue;
        }
        else if (page == 2) {
            while (true) {
                displaySignupHeader(role, page);
                getDetailedInfo(cnic, phone, gender, state, city, town, postalCode, cnicValid, phoneValid, genderValid, stateValid, cityValid, townValid, postalValid);
                cout << "\t\t\t\t\t\t..                          '1' Proceed                             ..\n";
                cout << "\t\t\t\t\t\t..                          '0' Go Back (to Page 1)                 ..\n";
                cout << "\t\t\t\t\t\t......................................................................\n";
                cout << "\t\t\t\t\t\t                       Enter your choice here: ";
                string entry; int choice = -1; getline(cin, entry, '\n');
                if (entry.length() == 1 && (entry == "0" || entry == "1")) { try { choice = stoi(entry); } catch (...) { choice = -1; } }
                if (choice == 0) { page = 1; break; }
                if (choice == 1) {
                    bool cCNIC = isValidCNIC(cnic); if (!cCNIC) Sleep(1500); else cnicValid = true;
                    uniqueCNIC = cCNIC ? isCNICUnique(cnic) : false; if (cCNIC && !uniqueCNIC) Sleep(1500);
                    bool cPhone = isValidPhoneNumber(phone); if (!cPhone) Sleep(1500); else phoneValid = true;
                    uniquePhone = cPhone ? isPhoneUnique(phone) : false; if (cPhone && !uniquePhone) Sleep(1500);
                    bool cGender = isValidGender(gender); if (!cGender) Sleep(1500); else genderValid = true;
                    bool cState = isValidGenericText(state); if (!cState) Sleep(1500); else stateValid = true;
                    bool cCity = isValidGenericText(city); if (!cCity) Sleep(1500); else cityValid = true;
                    bool cTown = isValidGenericText(town); if (!cTown) Sleep(1500); else townValid = true;
                    bool cPostal = isValidPostalCode(postalCode); if (!cPostal) Sleep(1500); else postalValid = true;
                    allPage2Valid = cCNIC && uniqueCNIC && cPhone && uniquePhone && cGender && cState && cCity && cTown && cPostal;
                    if (allPage2Valid) { page = 3; break; }
                    else {
                        displayMessage("Please correct the invalid fields on Page 2.", true);
                        if (!cCNIC || !uniqueCNIC) cnicValid = false; if (!cPhone || !uniquePhone) phoneValid = false;
                        if (!cGender) genderValid = false; if (!cState) stateValid = false; if (!cCity) cityValid = false;
                        if (!cTown) townValid = false; if (!cPostal) postalValid = false;
                        Sleep(2000); break;
                    }
                }
                else { invalidEntry(); break; }
            } if (page == 1) continue; if (page != 3) continue;
        }
        else if (page == 3) {
            while (true) {
                displaySignupHeader(role, page);
                cout << "\t\t\t\t\t\tDo you want to register as a Candidate overall? (y/n): ";
                string candidateInput;
                getline(cin, candidateInput);
                candidateChoiceValid = (candidateInput == "y" || candidateInput == "Y" || candidateInput == "n" || candidateInput == "N");

                if (candidateChoiceValid) {
                    if (candidateInput == "y" || candidateInput == "Y") {
                        wantsToBeCandidateOverall = IS_CANDIDATE_TRUE;
                        getPartyAffiliation(partyID);
                        partyIDValid = (partyID != "");
                        if (partyID != DEFAULT_PARTY_ID) isDependentCandidateChar = IS_CANDIDATE_TRUE;
                        else isDependentCandidateChar = IS_CANDIDATE_FALSE;
                    }
                    else {
                        wantsToBeCandidateOverall = IS_CANDIDATE_FALSE;
                        partyID = DEFAULT_PARTY_ID;
                        isDependentCandidateChar = IS_CANDIDATE_FALSE;
                        isPresidentChar = IS_CANDIDATE_FALSE;
                        partyIDValid = true;
                    }
                }
                else {
                    displayMessage("Invalid choice. Please enter 'y' or 'n'.", true);
                    Sleep(1500); continue;
                }
                cout << "\t\t\t\t\t\t......................................................................\n";
                cout << "\t\t\t\t\t\t..                          '1' Proceed                             ..\n";
                cout << "\t\t\t\t\t\t..                          '0' Go Back (to Page 2)                 ..\n";
                cout << "\t\t\t\t\t\t......................................................................\n";
                cout << "\t\t\t\t\t\t                       Enter your choice here: ";
                string entry; int choice = -1; getline(cin, entry, '\n');
                if (entry.length() == 1 && (entry == "0" || entry == "1")) { try { choice = stoi(entry); } catch (...) { choice = -1; } }

                if (choice == 0) { page = 2; break; }
                if (choice == 1) {
                    allPage3Valid = candidateChoiceValid && partyIDValid;
                    if (allPage3Valid) { page = 4; break; }
                    else { displayMessage("Please correct candidacy/party details.", true); Sleep(2000); break; }
                }
                else { invalidEntry(); break; }
            } if (page == 2) continue; if (page != 4) continue;
        }
        else if (page == 4) {
            while (true) {
                displaySignupHeader(role, page); pass1 = ""; pass2 = "";
                getPasswordInfo(pass1, pass2, passValid, passMatch);
                cout << "\t\t\t\t\t\t......................................................................\n";
                cout << "\t\t\t\t\t\t..                          '1' Finish Sign Up                      ..\n";
                cout << "\t\t\t\t\t\t..                          '0' Go Back (to Page 3)                 ..\n";
                cout << "\t\t\t\t\t\t......................................................................\n";
                cout << "\t\t\t\t\t\t                       Enter your choice here: ";
                string entry; int choice = -1; getline(cin, entry, '\n');
                if (entry.length() == 1 && (entry == "0" || entry == "1")) { try { choice = stoi(entry); } catch (...) { choice = -1; } }
                if (choice == 0) { page = 3; break; }
                if (choice == 1) {
                    passMatch = (pass1 == pass2);
                    if (!passMatch) { displayPasswordError("Passwords do not match."); Sleep(1500); passValid = false; }
                    else { passValid = isValidPassword(pass1); if (!passValid) Sleep(1500); }
                    allPage4Valid = passMatch && passValid;
                    if (allPage4Valid) {
                        string signupTimestamp = DateTime::getCurrentDateTime().toString();
                        bool credSaved = saveCredentials(credFilePath, cnic, fName, lName, pass1, PENDING_STATUS, wantsToBeCandidateOverall, partyID, isPresidentChar, isDependentCandidateChar);
                        bool dataSaved = saveData(dataFilePath, cnic, fName, lName, ageStr, phone, gender, state, city, town, postalCode, signupTimestamp);
                        bool approvalAdded = false;
                        if (credSaved && dataSaved) { approvalAdded = addPendingApproval(cnic); }
                        if (credSaved && dataSaved && approvalAdded) { displayMessage("Account created. Requires admin approval.", false); Sleep(2500); }
                        else { displayMessage("Account creation failed.", true); if (credSaved && (!dataSaved || !approvalAdded)) { displayMessage("Incomplete signup data saved.", true); } Sleep(2500); }
                        page = 0; break;
                    }
                    else { displayMessage("Please correct the password fields on Page 4.", true); Sleep(2000); break; }
                }
                else { invalidEntry(); break; }
            } if (page == 3) continue; if (page != 0) continue;
        }
    }
}

void UserSignUp::signUp() { signUpHelper("USER"); }

int adminParseLine(const string& line, char delimiter, string fields[], int maxFields) {
    int count = 0;
    size_t start = 0;
    size_t end = line.find(delimiter);
    while (end != string::npos && count < maxFields - 1) {
        fields[count++] = line.substr(start, end - start);
        start = end + 1;
        end = line.find(delimiter, start);
    }
    if (count < maxFields && start <= line.length()) {
        fields[count++] = line.substr(start);
    }
    return count;
}