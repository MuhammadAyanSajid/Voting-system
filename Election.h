#ifndef ELECTION_H
#define ELECTION_H

#include <string>
#include "DateTime.h"
#include "PoliticalParty.h"

using namespace std;

class User;
class Profile;

class Election {
protected:
    string electionID;
    string electionType;
    string statusFromFile;
    DateTime endDateTime;
    string scopeInfo;
    int maxCandidates;

    string winnerCNIC;     
    int winnerVoteCount;    

    void setScopeInfo(const string& scope);

public:
    Election(const string& id = "0000",
        const string& type = "Unknown",
        const string& statusFile = "Upcoming",
        const string& scope = "Unknown",
        int maxC = 0);
    virtual ~Election();

    string getElectionID() const;
    string getElectionType() const;
    string getStatusFromFile() const;
    DateTime getEndDateTime() const;
    string getScopeInfo() const;
    string getDynamicStatus() const;
    string getWinnerCNIC() const;          
    int getWinnerVoteCount() const;        

    void setElectionID(const string& id);
    void setElectionType(const string& type);
    void setStatusFromFile(const string& status);
    void setEndDateTime(const DateTime& dt);
    void setWinner(const string& cnic, int votes); 

    virtual bool isUserEligibleToVote(const User& user, const Profile& userProfile) const = 0;
    virtual bool isCandidateEligibleToParticipate(const User& candidateUser, const Profile& candidateProfile, const PoliticalParty& party) const = 0;
    virtual int getCandidateLimit() const = 0;
    virtual string getGeographicScope() const = 0;

    virtual void displayElectionDetails(bool showResults = false) const; 
    void showCandidatesAndCastVote(User& voter, Profile& voterProfile);
    void concludeAndPublishResults();

    static bool hasUserVoted(const string& voterCnic, const string& electionId);
    static bool recordUserVote(const string& voterCnic, const string& electionId);
    static bool updateVoteCount(const string& electionId, const string& electionType, const string& scope, const string& candidateCnic);
    static DateTime parseDateTimeString(const string& dateTimeStr);
    static bool parseElectionLine(const string& line,
        string& id,
        string& type,
        string& statusFromFile,
        string& endDateTimeStr,
        string& scope);
};


class NationalElection : public Election {
public:
    NationalElection(const string& id = "NE00", const string& statusFile = "Upcoming");
    bool isUserEligibleToVote(const User& user, const Profile& userProfile) const override;
    bool isCandidateEligibleToParticipate(const User& candidateUser, const Profile& candidateProfile, const PoliticalParty& party) const override;
    int getCandidateLimit() const override;
    string getGeographicScope() const override;
    void displayElectionDetails(bool showResults = false) const override;
};

class RegionalElection : public Election {
public:
    RegionalElection(const string& id = "RE00", const string& statusFile = "Upcoming", const string& region = "UnknownRegion");
    bool isUserEligibleToVote(const User& user, const Profile& userProfile) const override;
    bool isCandidateEligibleToParticipate(const User& candidateUser, const Profile& candidateProfile, const PoliticalParty& party) const override;
    int getCandidateLimit() const override;
    string getGeographicScope() const override;
    void displayElectionDetails(bool showResults = false) const override;
};

class MetropolitanElection : public Election {
public:
    MetropolitanElection(const string& id = "ME00", const string& statusFile = "Upcoming", const string& city = "UnknownCity");
    bool isUserEligibleToVote(const User& user, const Profile& userProfile) const override;
    bool isCandidateEligibleToParticipate(const User& candidateUser, const Profile& candidateProfile, const PoliticalParty& party) const override;
    int getCandidateLimit() const override;
    string getGeographicScope() const override;
    void displayElectionDetails(bool showResults = false) const override;
};

class LocalElection : public Election {
public:
    LocalElection(const string& id = "LE00", const string& statusFile = "Upcoming", const string& town = "UnknownTown", const string& city = "UnknownCity");
    bool isUserEligibleToVote(const User& user, const Profile& userProfile) const override;
    bool isCandidateEligibleToParticipate(const User& candidateUser, const Profile& candidateProfile, const PoliticalParty& party) const override;
    int getCandidateLimit() const override;
    string getGeographicScope() const override;
    void displayElectionDetails(bool showResults = false) const override;
};

bool loadPartyById(const string& partyIdToLoad, PoliticalParty& party);

#endif