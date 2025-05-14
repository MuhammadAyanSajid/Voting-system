#ifndef USER_H
#define USER_H

#include "Profile.h"
#include "Authorization.h"
#include <string>

using namespace std;

class Election;

class User {
protected:
    Profile userProfile;
    bool isRegisteredCandidate_flag;
    string partyID_val;
    bool isPresident_flag;
    bool isDependentCandidate_flag;
    bool isParticipating_flag;
    string participatingElectionID_val;

    void displayUserMenu();

    void viewElectionSchedule();
    void castVote();
    void viewEditProfile();
    void viewElectionResults();

    void electionManagementMenu();
    void registerInElection();
    void unregisterFromElection();

    void partyManagementMenu();
    void registerInParty();
    void unregisterFromParty();
    void viewPartyDetails();
    void viewPartyMembers();

    void modifyPartyDetails();
    void addMemberToParty();
    void removeMemberFromParty();


public:
    User();
    virtual ~User() = default;

    void setCandidateStatus(bool status);
    void setPartyID(const string& pid);
    void setIsPresident(bool status);
    void setIsDependentCandidate(bool status);
    void setIsParticipating(bool status);
    void setParticipatingElectionID(const string& eid);

    bool getIsRegisteredCandidate() const;
    string getPartyID() const;
    bool getIsPresident() const;
    bool getIsDependentCandidate() const;
    bool getIsParticipating() const;
    string getParticipatingElectionID() const;

    void Dashboard();

    Profile& getProfile();
    const Profile& getProfile() const;

    bool loadUserProfile(const string& cnic, const string& dataFilePath);
    bool updateLoginTimestamp(const string& dataFilePath);
};

#endif