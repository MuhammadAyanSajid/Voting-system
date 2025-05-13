#ifndef ADMIN_H
#define ADMIN_H

#include "Profile.h"
#include <string>

using namespace std;

class PoliticalParty;

class Admin {
private:
    Profile adminProfile;

    void displayAdminMenu();

    void userManagement();
    void partyManagement();
    void electionManagement();
    void accountApproval();
    void viewAdminProfile();

    bool updateCredentialStatus(const string& targetCnic, const string& credFilePath, char newStatus);
    bool updateUserCredentialField(const string& targetCnic, int fieldIndex, const string& newValue);


    void displayUserManagementMenu();
    void addUser();
    void deleteUser();
    void modifyUser();
    void searchUser();
    void listAllUsers();

    void displayPartyManagementMenu();
    void createParty();
    void modifyParty();
    void deleteParty();
    void listAllParties();
    string generateUniquePartyID();
    bool isPartyIDUnique(const string& partyID);
    bool isPartyNameUnique(const string& partyName);
    int getPartyMemberCount(const string& partyID);
    int getPartyCandidateCount(const string& partyID);

    void displayElectionManagementMenu();
    void createElection();
    void modifyElection();
    void deleteElection();
    void listAllElections();
    void manageElectionLifecycle();
    void viewElectionResultsAdmin();


    void displayViewAdminProfileMenu();
    void displayCurrentAdminProfile();
    void editCurrentAdminProfile();


public:
    Admin() = default;
    ~Admin() = default;

    void displayDashboard();
    void Dashboard();

    Profile& getProfile();
    const Profile& getProfile() const;

    bool loadAdminProfile(const string& cnic, const string& dataFilePath);
    bool updateLoginTimestamp(const string& dataFilePath);
};

#endif