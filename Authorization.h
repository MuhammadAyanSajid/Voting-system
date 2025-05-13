#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <conio.h>
#include <cctype>

#include "DateTime.h"
#include "PoliticalParty.h"

using namespace std;

const int MAX_FILE_LINES = 200;
const int MAX_CANDIDATES_DISPLAY = 100;

const string USER_CREDENTIALS_FILE_PATH = "user_credentials.txt";
const string USER_DATA_FILE_PATH = "user_data.txt";
const string ADMIN_CREDENTIALS_FILE_PATH = "admin_credentials.txt";
const string ADMIN_DATA_FILE_PATH = "admin_data.txt";
const string PARTIES_FILE_PATH = "parties.txt";
const string APPROVAL_FILE = "accountApprov.txt";

const char PENDING_STATUS = '0';
const char ACTIVE_STATUS = '1';
const char IS_CANDIDATE_TRUE = '1';
const char IS_CANDIDATE_FALSE = '0';
const string DEFAULT_PARTY_ID = "NONE";

const int USER_CRED_CNIC_IDX = 0;
const int USER_CRED_FNAME_IDX = 1;
const int USER_CRED_LNAME_IDX = 2;
const int USER_CRED_PASS_IDX = 3;
const int USER_CRED_STATUS_IDX = 4;
const int USER_CRED_ISCANDIDATE_IDX = 5;
const int USER_CRED_PARTYID_IDX = 6;
const int USER_CRED_ISPRESIDENT_IDX = 7;
const int USER_CRED_ISDEPENDENTCANDIDATE_IDX = 8;
const int USER_CRED_ISPARTICIPATING_IDX = 9;
const int USER_CRED_PARTICIPELECID_IDX = 10;
const int USER_CRED_FIELD_COUNT = 11;

const int ADMIN_CRED_CNIC_IDX = 0;
const int ADMIN_CRED_FNAME_IDX = 1;
const int ADMIN_CRED_LNAME_IDX = 2;
const int ADMIN_CRED_PASS_IDX = 3;
const int ADMIN_CRED_FIELD_COUNT = 4;

const int DATA_CNIC_IDX = 0;
const int DATA_FNAME_IDX = 1;
const int DATA_LNAME_IDX = 2;
const int DATA_AGE_IDX = 3;
const int DATA_PHONE_IDX = 4;
const int DATA_GENDER_IDX = 5;
const int DATA_STATE_IDX = 6;
const int DATA_CITY_IDX = 7;
const int DATA_TOWN_IDX = 8;
const int DATA_POSTAL_IDX = 9;
const int DATA_TIMESTAMP_IDX = 10;
const int DATA_FIELD_COUNT = 11;

const int PARTY_ID_IDX = 0;
const int PARTY_NAME_IDX = 1;
const int PARTY_RANK_IDX = 2;
const int PARTY_PRESIDENT_CNIC_IDX = 3;
const int PARTY_FIELD_COUNT = 4;

const string ELECTIONS_FILE_PATH = "elections.txt";
const string VOTES_FILE_PATH = "Votes.txt";
const string VOTES_CAST_FILE_PATH = "votes_cast.txt";

class LoginDashboard;
class AdminLogin;
class UserLogin;
class SignUpDashboard;
class UserSignUp;

void displayMessage(const string& msg, bool error = true);
void displayLoginError(const string& msg);
void invalidEntry();
void displayMainPanel();
void developersPage();
void mainPanel();
void displayAccessPanel();
void accessPanel();
void displayLogin_SignUp();
void signUpPanel();
void loginPanel();
void hidePasswordInput(string& password);
void getAdminLogin(string& cnic, string& password);
void getUserLogin(string& cnic, string& password);
void loadingScreen();
void invalidCredentials();
bool isFileEmpty(const string& filePath);
void displayPasswordError(const string& message);
bool containsOnlyDigits(const string& s);
bool containsOnlyAlpha(const string& s);
bool containsOnlyAlphaAndSpace(const string& s);
void populateApprovalFile();
int adminParseLine(const string& line, char delimiter, string fields[], int maxFields);

int readFileToArray(const string& filePath, string linesArray[], int maxLines);
bool writeArrayToFile(const string& filePath, const string linesArray[], int count);
int adminParseLine(const string& line, char delimiter, string fields[], int maxFields);
string adminConstructLine(const string fields[], int count, char delimiter);


class LoginDashboard {
public:
    virtual int checkLogin(const string& cnic, const string& password, ifstream& logfile) = 0;
    virtual void login() = 0;
    virtual ~LoginDashboard() {}
protected:
    LoginDashboard() {}
    string name;
    string userCNIC;
};

class AdminLogin : public LoginDashboard {
public:
    AdminLogin() : LoginDashboard() {}
    ~AdminLogin() {}
    int checkLogin(const string& cnic, const string& password, ifstream& logfile) override;
    void login() override;
};

class UserLogin : public LoginDashboard {
public:
    UserLogin() : LoginDashboard(), isAlsoCandidate(false), isPresident(false),
        isDependentCandidate(false), partyID(DEFAULT_PARTY_ID),
        isParticipating(false), participatingElectionID(DEFAULT_PARTY_ID) {
    }
    ~UserLogin() {}
    int checkLogin(const string& cnic, const string& password, ifstream& logfile) override;
    void login() override;

protected:
    bool isAlsoCandidate;
    string partyID;
    bool isPresident;
    bool isDependentCandidate;
    bool isParticipating;
    string participatingElectionID;
};

class SignUpDashboard {
public:
    virtual void signUp() = 0;
    virtual ~SignUpDashboard() {}
protected:
    SignUpDashboard() {}
    bool isValidName(const string& name);
    bool isValidAge(const string& ageStr, int& age);
    bool isEligible(int age);
    bool isValidCNIC(const string& cnic);
    bool isCNICUnique(const string& cnic);
    bool isPhoneUnique(const string& phone);
    bool isValidPhoneNumber(const string& phone);
    bool isValidPostalCode(const string& code);
    bool isValidGender(const string& gender);
    bool isValidGenericText(const string& text);
    bool isValidPassword(const string& password);
    bool isValidPartyIDForSignup(const string& partyID, PoliticalParty& foundParty);


    void displaySignupHeader(const string& title, int page);
    void getBasicInfo(string& fName, string& lName, string& ageStr, bool& fNameValid, bool& lNameValid, bool& ageValid);
    void getDetailedInfo(string& cnic, string& phone, string& gender, string& state, string& city, string& town, string& postalCode,
        bool& cnicValid, bool& phoneValid, bool& genderValid, bool& stateValid, bool& cityValid, bool& townValid, bool& postalValid);
    void getPasswordInfo(string& pass1, string& pass2, bool& passValid, bool& passMatch);
    void getPartyAffiliation(string& partyID);


    void signUpHelper(const string& role);

    bool saveCredentials(const string& filePath, const string& cnic, const string& fName, const string& lName, const string& password,
        char status, char isCandidateOverall, const string& partyId, char isPresident, char isDependentCandidate);
    bool saveData(const string& filePath, const string& cnic, const string& fName, const string& lName, const string& age,
        const string& phone, const string& gender, const string& state, const string& city, const string& town, const string& postalCode,
        const string& signupTime);

    bool addPendingApproval(const string& cnic);
};

class UserSignUp : public SignUpDashboard {
public:
    UserSignUp() : SignUpDashboard() {}
    ~UserSignUp() {}
    void signUp() override;
};

#endif