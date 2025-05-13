#include "PoliticalParty.h"
#include <string> 
PoliticalParty::PoliticalParty()
    : partyID("0000"), partyName("Unknown"), rank(RANK_UNKNOWN),
    membersLimit(0), candidatesLimit(0), partyPresidentCNIC("N/A") {
}

PoliticalParty::PoliticalParty(const string& id, const string& name, PartyRank r, const string& presidentCnic)
    : partyID(id), partyName(name), rank(r), partyPresidentCNIC(presidentCnic) {
    setLimitsByRank();
}

void PoliticalParty::setLimitsByRank() {
    switch (rank) {
    case TIER1:
        membersLimit = 20;
        candidatesLimit = 10;
        break;
    case TIER2:
        membersLimit = 25;
        candidatesLimit = 15;
        break;
    case TIER3:
        membersLimit = 30;
        candidatesLimit = 20;
        break;
    default:
        membersLimit = 0;
        candidatesLimit = 0;
        break;
    }
}

void PoliticalParty::setPartyID(const string& id) { partyID = id; }
void PoliticalParty::setPartyName(const string& name) { partyName = name; }
void PoliticalParty::setPartyRank(PartyRank r) {
    rank = r;
    setLimitsByRank();
}
void PoliticalParty::setPartyPresidentCNIC(const string& cnic) { partyPresidentCNIC = cnic; }

string PoliticalParty::getPartyID() const { return partyID; }
string PoliticalParty::getPartyName() const { return partyName; }
PartyRank PoliticalParty::getPartyRank() const { return rank; }

string PoliticalParty::getPartyRankString() const {
    return PoliticalParty::partyRankToString(rank);
}

int PoliticalParty::getMembersLimit() const { return membersLimit; }
int PoliticalParty::getCandidatesLimit() const { return candidatesLimit; }
string PoliticalParty::getPartyPresidentCNIC() const { return partyPresidentCNIC; }

string PoliticalParty::toString(char delimiter) const {
    string rankStr = "3"; 
    if (rank == TIER1) rankStr = "0";
    else if (rank == TIER2) rankStr = "1";
    else if (rank == TIER3) rankStr = "2";

    return partyID + delimiter +
        partyName + delimiter +
        rankStr + delimiter +
        partyPresidentCNIC;
}

bool PoliticalParty::fromString(const string& partyDataString, char delimiter) {
    string fields[4];
    int count = 0;
    size_t start = 0;
    size_t end = partyDataString.find(delimiter);
    while (end != string::npos && count < 3) {
        fields[count++] = partyDataString.substr(start, end - start);
        start = end + 1;
        end = partyDataString.find(delimiter, start);
    }
    if (count < 4 && start <= partyDataString.length()) {
        fields[count++] = partyDataString.substr(start);
    }

    if (count == 4) {
        partyID = fields[0];
        partyName = fields[1];
        rank = stringToPartyRank(fields[2]);
        partyPresidentCNIC = fields[3];
        setLimitsByRank();
        return true;
    }
    return false;
}

PartyRank PoliticalParty::stringToPartyRank(const string& rankStr) {
    if (rankStr == "0") return TIER1;
    if (rankStr == "1") return TIER2;
    if (rankStr == "2") return TIER3;
    return RANK_UNKNOWN;
}

string PoliticalParty::partyRankToString(PartyRank r) {
    if (r == TIER1) return "Tier 1";
    if (r == TIER2) return "Tier 2";
    if (r == TIER3) return "Tier 3";
    return "Unknown Rank";
}