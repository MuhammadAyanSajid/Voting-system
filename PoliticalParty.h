#ifndef POLITICALPARTY_H
#define POLITICALPARTY_H

#include <string>

using namespace std;

enum PartyRank {
    TIER1,
    TIER2,
    TIER3,
    RANK_UNKNOWN
};

class PoliticalParty {
private:
    string partyID;          // 4-digit unique
    string partyName;
    PartyRank rank;
    int membersLimit;
    int candidatesLimit;
    string partyPresidentCNIC; // CNIC of the party president

    void setLimitsByRank();

public:
    PoliticalParty();
    PoliticalParty(const string& id, const string& name, PartyRank r, const string& presidentCnic);

    void setPartyID(const string& id);
    void setPartyName(const string& name);
    void setPartyRank(PartyRank r);
    void setPartyPresidentCNIC(const string& cnic);

    string getPartyID() const;
    string getPartyName() const;
    PartyRank getPartyRank() const;
    string getPartyRankString() const;
    int getMembersLimit() const;
    int getCandidatesLimit() const;
    string getPartyPresidentCNIC() const;

    string toString(char delimiter = '-') const;
    bool fromString(const string& partyDataString, char delimiter = '-');

    static PartyRank stringToPartyRank(const string& rankStr);
    static string partyRankToString(PartyRank r);
};

#endif // POLITICALPARTY_H