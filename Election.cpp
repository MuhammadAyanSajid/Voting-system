#include "Election.h"
#include "Authorization.h"
#include "User.h"
#include "Profile.h"
#include "PoliticalParty.h"
#include <fstream>
#include <iostream>


Election::Election(const string& id, const string& type, const string& statusFile, const string& scope, int maxC)
    : electionID(id), electionType(type), statusFromFile(statusFile),
    scopeInfo(scope), maxCandidates(maxC), endDateTime(),
    winnerCNIC("N/A"), winnerVoteCount(0)
{
}

Election::~Election() {}

string Election::getElectionID() const { return electionID; }
string Election::getElectionType() const { return electionType; }
string Election::getStatusFromFile() const { return statusFromFile; }
DateTime Election::getEndDateTime() const { return endDateTime; }
string Election::getScopeInfo() const { return scopeInfo; }
string Election::getWinnerCNIC() const { return winnerCNIC; }
int Election::getWinnerVoteCount() const { return winnerVoteCount; }


string Election::getDynamicStatus() const {
    DateTime now = DateTime::getCurrentDateTime();

    if (statusFromFile == "Cancelled" || statusFromFile == "Completed") {
        return statusFromFile;
    }
    if (statusFromFile == "Upcoming") {
        return "Upcoming";
    }
    if (statusFromFile == "Ongoing") {
        if (endDateTime.isObjectValid() && now >= endDateTime) {
            return "Completed";
        }
        return "Ongoing";
    }
    return "Unknown Status";
}

void Election::setElectionID(const string& id) { electionID = id; }
void Election::setElectionType(const string& type) {
    if (statusFromFile == "Upcoming") {
        electionType = type;
    }
}
void Election::setStatusFromFile(const string& status) { statusFromFile = status; }
void Election::setEndDateTime(const DateTime& dt) { endDateTime = dt; }
void Election::setScopeInfo(const string& scope) {
    if (statusFromFile == "Upcoming") {
        scopeInfo = scope;
    }
}
void Election::setWinner(const string& cnic, int votes) {
    winnerCNIC = cnic;
    winnerVoteCount = votes;
}


bool Election::parseElectionLine(const string& line,
    string& id,
    string& type,
    string& statusFromFile,
    string& endDateTimeStr,
    string& scope) {
    id.clear(); type.clear(); statusFromFile.clear(); endDateTimeStr.clear(); scope.clear();

    string tempFields[5];
    int fieldIndex = 0;
    size_t startPos = 0;
    size_t endPos = 0;

    for (fieldIndex = 0; fieldIndex < 5; ++fieldIndex) {
        endPos = line.find('-', startPos);
        if (endPos != string::npos) {
            tempFields[fieldIndex] = line.substr(startPos, endPos - startPos);
            startPos = endPos + 1;
        }
        else {
            if (startPos < line.length()) {
                tempFields[fieldIndex] = line.substr(startPos);
            }
            else {
                tempFields[fieldIndex] = "";
            }
            fieldIndex++;
            break;
        }
    }

    if (fieldIndex == 5) {
        id = tempFields[0];
        type = tempFields[1];
        statusFromFile = tempFields[2];
        endDateTimeStr = tempFields[3];
        scope = tempFields[4];

        if (id.empty() || type.empty() || statusFromFile.empty() || endDateTimeStr.empty() || scope.empty()) {
            return false;
        }
        return true;
    }
    return false;
}

DateTime Election::parseDateTimeString(const string& dateTimeStr) {
    if (dateTimeStr.length() != 19) {
        return DateTime(0, 0, 0, 0, 0, 0);
    }
    string yearStr, monthStr, dayStr, hourStr, minuteStr, secondStr;
    if (dateTimeStr[4] != '-' || dateTimeStr[7] != '-' || dateTimeStr[10] != ' ' ||
        dateTimeStr[13] != ':' || dateTimeStr[16] != ':') {
        return DateTime(0, 0, 0, 0, 0, 0);
    }
    yearStr = dateTimeStr.substr(0, 4);
    monthStr = dateTimeStr.substr(5, 2);
    dayStr = dateTimeStr.substr(8, 2);
    hourStr = dateTimeStr.substr(11, 2);
    minuteStr = dateTimeStr.substr(14, 2);
    secondStr = dateTimeStr.substr(17, 2);
    int year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0;
    try {
        if (containsOnlyDigits(yearStr)) year = stoi(yearStr); else return DateTime(0, 0, 0, 0, 0, 0);
        if (containsOnlyDigits(monthStr)) month = stoi(monthStr); else return DateTime(0, 0, 0, 0, 0, 0);
        if (containsOnlyDigits(dayStr)) day = stoi(dayStr); else return DateTime(0, 0, 0, 0, 0, 0);
        if (containsOnlyDigits(hourStr)) hour = stoi(hourStr); else return DateTime(0, 0, 0, 0, 0, 0);
        if (containsOnlyDigits(minuteStr)) minute = stoi(minuteStr); else return DateTime(0, 0, 0, 0, 0, 0);
        if (containsOnlyDigits(secondStr)) second = stoi(secondStr); else return DateTime(0, 0, 0, 0, 0, 0);
    }
    catch (...) {
        return DateTime(0, 0, 0, 0, 0, 0);
    }
    return DateTime(year, month, day, hour, minute, second);
}


void Election::displayElectionDetails(bool showResults) const {
    cout << "\t\t\t\t\t\tElection ID: " << electionID << "\n";
    cout << "\t\t\t\t\t\tType: " << electionType << "\n";
    cout << "\t\t\t\t\t\tStatus (File): " << statusFromFile << "\n";
    cout << "\t\t\t\t\t\tStatus (Actual): " << getDynamicStatus() << "\n";
    if (endDateTime.isObjectValid()) {
        cout << "\t\t\t\t\t\tEnds:   " << endDateTime.toString() << "\n";
    }
    else {
        cout << "\t\t\t\t\t\tEnds:   Not Yet Started\n";
    }
    if (!scopeInfo.empty()) {
        if (electionType == "National") cout << "\t\t\t\t\t\tScope: Nationwide\n";
        else if (electionType == "Local" && scopeInfo.find(',') != string::npos) cout << "\t\t\t\t\t\tScope: Town=" << scopeInfo.substr(0, scopeInfo.find(',')) << ", City=" << scopeInfo.substr(scopeInfo.find(',') + 1) << "\n";
        else cout << "\t\t\t\t\t\tScope: " << scopeInfo << "\n";
    }
    cout << "\t\t\t\t\t\tCandidate Limit: " << getCandidateLimit() << "\n";

    if (showResults && (statusFromFile == "Completed" || getDynamicStatus() == "Completed")) {
        if (winnerCNIC != "N/A" && winnerCNIC != "") {
            Profile winnerProf;
            winnerProf.loadFromFile(winnerCNIC, USER_DATA_FILE_PATH);
            cout << "\t\t\t\t\t\tWINNER: " << winnerProf.getFullName() << " (CNIC: " << winnerCNIC << ") with " << winnerVoteCount << " votes.\n";
        }
        else if (statusFromFile == "Completed" || getDynamicStatus() == "Completed") {
            cout << "\t\t\t\t\t\tResults: No winner declared or tie (manual check needed if votes present).\n";
        }
    }
}

bool loadPartyById(const string& partyIdToLoad, PoliticalParty& party) {
    if (partyIdToLoad == DEFAULT_PARTY_ID || partyIdToLoad.empty() || partyIdToLoad == "0000") {
        party = PoliticalParty();
        return false;
    }
    ifstream partiesFile(PARTIES_FILE_PATH);
    if (!partiesFile.is_open()) {
        party = PoliticalParty();
        return false;
    }
    string line;
    bool found = false;
    while (getline(partiesFile, line)) {
        PoliticalParty tempParty;
        if (tempParty.fromString(line)) {
            if (tempParty.getPartyID() == partyIdToLoad) {
                party = tempParty;
                found = true;
                break;
            }
        }
    }
    partiesFile.close();
    if (!found) {
        party = PoliticalParty();
    }
    return found;
}

void Election::showCandidatesAndCastVote(User& voter, Profile& voterProfile) {
    system("cls");
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t..................... C A S T   Y O U R   V O T E ....................\n";
    cout << "\t\t\t\t\t\t......................................................................\n";
    this->displayElectionDetails();
    cout << "\t\t\t\t\t\t......................................................................\n";

    if (this->statusFromFile != "Ongoing") {
        displayMessage("This election is not currently ongoing (Status: " + this->statusFromFile + "). Cannot cast vote.", false);
        return;
    }
    if (this->getDynamicStatus() != "Ongoing") {
        displayMessage("This election voting period has ended. Cannot cast vote.", false);
        return;
    }

    if (hasUserVoted(voterProfile.getCnic(), this->electionID)) {
        displayMessage("You have already cast your vote in this election.", false);
        return;
    }

    cout << "\t\t\t\t\t\tEligible Candidates for " << this->electionType << " Election (ID: " << this->electionID << "):\n";

    ifstream credFile(USER_CREDENTIALS_FILE_PATH);
    if (!credFile.is_open()) {
        displayMessage("Error: Cannot open user credentials file.", true);
        return;
    }

    string line;
    const int MAX_CANDIDATES_DISPLAY = 100;
    string candidateCnicList[MAX_CANDIDATES_DISPLAY];
    string candidateNameList[MAX_CANDIDATES_DISPLAY];
    int displayedCandidateCount = 0;

    string userCredFields[USER_CRED_FIELD_COUNT];

    while (getline(credFile, line) && displayedCandidateCount < MAX_CANDIDATES_DISPLAY) {
        if (adminParseLine(line, '-', userCredFields, USER_CRED_FIELD_COUNT) == USER_CRED_FIELD_COUNT) {

            if (userCredFields[USER_CRED_ISPARTICIPATING_IDX] == "1" &&
                userCredFields[USER_CRED_PARTICIPELECID_IDX] == this->electionID)
            {
                User tempCandidateUser;
                Profile tempCandidateProfile;
                PoliticalParty tempParty;

                if (!tempCandidateProfile.loadFromFile(userCredFields[USER_CRED_CNIC_IDX], USER_DATA_FILE_PATH)) {
                    continue;
                }
                loadPartyById(userCredFields[USER_CRED_PARTYID_IDX], tempParty);

                cout << "\t\t\t\t\t\t" << (displayedCandidateCount + 1) << ". " << tempCandidateProfile.getFullName()
                    << " (CNIC: " << tempCandidateProfile.getCnic() << ")"
                    << (tempParty.getPartyID() != "0000" && tempParty.getPartyID() != DEFAULT_PARTY_ID && !tempParty.getPartyID().empty() ? " - Party: " + tempParty.getPartyName() : " - Independent") << "\n";

                candidateCnicList[displayedCandidateCount] = tempCandidateProfile.getCnic();
                candidateNameList[displayedCandidateCount] = tempCandidateProfile.getFullName();
                displayedCandidateCount++;

                if (displayedCandidateCount >= this->getCandidateLimit()) {
                    break;
                }
            }
        }
    }
    credFile.close();

    if (displayedCandidateCount == 0) {
        displayMessage("No candidates found participating in this election.", false);
        return;
    }

    cout << "\t\t\t\t\t\t......................................................................\n";
    string chosenCandidateCnic;
    bool validChoice = false;

    while (!validChoice) {
        cout << "\t\t\t\t\t\tEnter the CNIC of the candidate to vote for (or '0' to cancel): ";
        getline(cin, chosenCandidateCnic);

        if (chosenCandidateCnic == "0") {
            displayMessage("Vote cancelled.", false);
            return;
        }

        if (!containsOnlyDigits(chosenCandidateCnic) || chosenCandidateCnic.length() != 13) {
            displayMessage("Invalid CNIC format.", true);
            continue;
        }
        if (chosenCandidateCnic == voterProfile.getCnic()) {
            displayMessage("You cannot vote for yourself.", true);
            continue;
        }

        for (int i = 0; i < displayedCandidateCount; ++i) {
            if (candidateCnicList[i] == chosenCandidateCnic) {
                validChoice = true;
                break;
            }
        }

        if (!validChoice) {
            displayMessage("CNIC does not match listed candidates.", true);
        }
    }

    if (updateVoteCount(this->electionID, this->electionType, this->getGeographicScope(), chosenCandidateCnic)) {
        if (recordUserVote(voterProfile.getCnic(), this->electionID)) {
            displayMessage("Vote cast successfully for CNIC: " + chosenCandidateCnic, false);
        }
        else {
            displayMessage("Vote counted, but failed to record your vote status.", true);
        }
    }
    else {
        displayMessage("Failed to update vote count.", true);
    }
}


void Election::concludeAndPublishResults() {
    if (this->statusFromFile != "Completed" && this->getDynamicStatus() != "Completed") {
        displayMessage("Election " + electionID + " is not yet completed. Results cannot be published.", false);
        return;
    }

    system("cls");
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t........... E L E C T I O N   R E S U L T S (ID: " << this->electionID << ") ...........\n";
    cout << "\t\t\t\t\t\t......................................................................\n";
    this->displayElectionDetails(false);
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\t                      V O T E   C O U N T S                           \n";
    cout << "\t\t\t\t\t\t......................................................................\n";

    ifstream votesFile(VOTES_FILE_PATH);
    if (!votesFile.is_open()) {
        displayMessage("Error: Could not open votes file to calculate results.", true);
        return;
    }

    string line;
    string candCNICs[MAX_CANDIDATES_DISPLAY];
    int candVotes[MAX_CANDIDATES_DISPLAY] = { 0 };
    int uniqueCandidatesFound = 0;

    string fields[5];
    while (getline(votesFile, line)) {
        if (adminParseLine(line, '-', fields, 5) == 5) {
            if (fields[0] == this->electionID && fields[1] == this->electionType && fields[2] == this->getGeographicScope()) {
                string currentCandCNIC = fields[3];
                int currentCandVoteCount = 0;
                try { if (containsOnlyDigits(fields[4])) currentCandVoteCount = stoi(fields[4]); }
                catch (...) {}

                bool foundInList = false;
                for (int i = 0; i < uniqueCandidatesFound; ++i) {
                    if (candCNICs[i] == currentCandCNIC) {
                        candVotes[i] = currentCandVoteCount;
                        foundInList = true;
                        break;
                    }
                }
                if (!foundInList && uniqueCandidatesFound < MAX_CANDIDATES_DISPLAY) {
                    candCNICs[uniqueCandidatesFound] = currentCandCNIC;
                    candVotes[uniqueCandidatesFound] = currentCandVoteCount;
                    uniqueCandidatesFound++;
                }
            }
        }
    }
    votesFile.close();

    if (uniqueCandidatesFound == 0) {
        cout << "\t\t\t\t\t\t..                No votes recorded for this election.              ..\n";
        this->setWinner("N/A", 0);
    }
    else {
        string tempWinnerCNIC = candCNICs[0];
        int tempMaxVotes = candVotes[0];
        bool tie = false;

        for (int i = 0; i < uniqueCandidatesFound; ++i) {
            Profile candProf;
            candProf.loadFromFile(candCNICs[i], USER_DATA_FILE_PATH);
            PoliticalParty candParty;
            string userCredFields[USER_CRED_FIELD_COUNT];
            ifstream credFile(USER_CREDENTIALS_FILE_PATH);
            string credLine;
            while (getline(credFile, credLine)) {
                if (adminParseLine(credLine, '-', userCredFields, USER_CRED_FIELD_COUNT) == USER_CRED_FIELD_COUNT) {
                    if (userCredFields[USER_CRED_CNIC_IDX] == candCNICs[i]) {
                        loadPartyById(userCredFields[USER_CRED_PARTYID_IDX], candParty);
                        break;
                    }
                }
            }
            credFile.close();

            cout << "\t\t\t\t\t\t  " << candProf.getFullName() << " (CNIC: " << candCNICs[i] << ")";
            if (candParty.getPartyID() != "0000" && candParty.getPartyID() != DEFAULT_PARTY_ID && !candParty.getPartyID().empty()) {
                cout << " - Party: " << candParty.getPartyName();
            }
            else {
                cout << " - Independent";
            }
            cout << ": " << candVotes[i] << " votes\n";

            if (i > 0) {
                if (candVotes[i] > tempMaxVotes) {
                    tempMaxVotes = candVotes[i];
                    tempWinnerCNIC = candCNICs[i];
                    tie = false;
                }
                else if (candVotes[i] == tempMaxVotes && tempMaxVotes > 0) {
                    tie = true;
                }
            }
        }
        cout << "\t\t\t\t\t\t......................................................................\n";
        if (tie) {
            cout << "\t\t\t\t\t\t..                       T I E   D E T E C T E D                ..\n";
            this->setWinner("TIE", tempMaxVotes);
        }
        else {
            Profile winnerProfile;
            winnerProfile.loadFromFile(tempWinnerCNIC, USER_DATA_FILE_PATH);
            cout << "\t\t\t\t\t\tWINNER: " << winnerProfile.getFullName() << " (CNIC: " << tempWinnerCNIC << ") with " << tempMaxVotes << " votes!\n";
            this->setWinner(tempWinnerCNIC, tempMaxVotes);
        }
    }
    cout << "\t\t\t\t\t\t......................................................................\n";
    cout << "\t\t\t\t\t\tPress Enter to continue...";
    string temp;
    getline(cin, temp);
}


bool Election::hasUserVoted(const string& voterCnic, const string& electionId) {
    ifstream votesCastFile(VOTES_CAST_FILE_PATH);
    if (!votesCastFile.is_open()) {
        return false;
    }
    string line;
    string fields[2];
    while (getline(votesCastFile, line)) {
        if (adminParseLine(line, '-', fields, 2) == 2) {
            if (fields[0] == voterCnic && fields[1] == electionId) {
                votesCastFile.close();
                return true;
            }
        }
    }
    votesCastFile.close();
    return false;
}

bool Election::recordUserVote(const string& voterCnic, const string& electionId) {
    ofstream votesCastFile(VOTES_CAST_FILE_PATH, ios::app);
    if (!votesCastFile.is_open()) {
        displayMessage("Error: Cannot open votes cast file for writing.", true);
        return false;
    }
    votesCastFile << voterCnic << "-" << electionId << endl;
    votesCastFile.close();
    return true;
}


bool Election::updateVoteCount(const string& electionId, const string& electionType, const string& scope, const string& candidateCnic) {
    const int MAX_VOTE_LINES = MAX_FILE_LINES * 10;
    string lines[MAX_VOTE_LINES];
    int lineCount = 0;
    ifstream votesFileIn(VOTES_FILE_PATH);
    bool foundCandidateVoteEntry = false;

    if (votesFileIn.is_open()) {
        string line;
        while (getline(votesFileIn, line) && lineCount < MAX_VOTE_LINES) {
            if (line.empty()) continue;
            lines[lineCount] = line;
            lineCount++;
        }
        votesFileIn.close();
    }

    for (int i = 0; i < lineCount; ++i) {
        string fields[5];
        if (adminParseLine(lines[i], '-', fields, 5) == 5) {
            if (fields[0] == electionId && fields[1] == electionType && fields[2] == scope && fields[3] == candidateCnic) {
                int currentVotes = 0;
                try {
                    if (containsOnlyDigits(fields[4])) {
                        currentVotes = stoi(fields[4]);
                    }
                }
                catch (...) {
                    currentVotes = 0;
                }
                currentVotes++;
                lines[i] = electionId + "-" + electionType + "-" + scope + "-" + candidateCnic + "-" + to_string(currentVotes);
                foundCandidateVoteEntry = true;
                break;
            }
        }
    }

    if (!foundCandidateVoteEntry) {
        if (lineCount < MAX_VOTE_LINES) {
            lines[lineCount] = electionId + "-" + electionType + "-" + scope + "-" + candidateCnic + "-1";
            lineCount++;
        }
        else {
            displayMessage("Error: Vote storage file is full.", true);
            return false;
        }
    }

    ofstream votesFileOut(VOTES_FILE_PATH, ios::trunc);
    if (!votesFileOut.is_open()) {
        displayMessage("Error: Cannot open votes file for writing.", true);
        return false;
    }
    for (int i = 0; i < lineCount; ++i) {
        votesFileOut << lines[i] << endl;
    }
    votesFileOut.close();

    return true;
}


NationalElection::NationalElection(const string& id, const string& statusFile)
    : Election(id, "National", statusFile, "NATIONAL", 10) {
}
bool NationalElection::isUserEligibleToVote(const User& user, const Profile& userProfile) const { return true; }
bool NationalElection::isCandidateEligibleToParticipate(const User& candidateUser, const Profile& candidateProfile, const PoliticalParty& party) const {
    if (!candidateUser.getIsRegisteredCandidate()) return false;
    if (!candidateUser.getIsDependentCandidate()) return false;
    if (party.getPartyID() == DEFAULT_PARTY_ID || party.getPartyID() == "0000" || party.getPartyID().empty()) return false;
    return party.getPartyRank() == TIER3;
}
int NationalElection::getCandidateLimit() const { return 10; }
string NationalElection::getGeographicScope() const { return "NATIONAL"; }
void NationalElection::displayElectionDetails(bool showResults) const {
    Election::displayElectionDetails(showResults);
}


RegionalElection::RegionalElection(const string& id, const string& statusFile, const string& region)
    : Election(id, "Regional", statusFile, region, 15) {
}
bool RegionalElection::isUserEligibleToVote(const User& user, const Profile& userProfile) const { return userProfile.getState() == this->scopeInfo; }
bool RegionalElection::isCandidateEligibleToParticipate(const User& candidateUser, const Profile& candidateProfile, const PoliticalParty& party) const {
    if (!candidateUser.getIsRegisteredCandidate()) return false;
    if (candidateProfile.getState() != this->scopeInfo) return false;
    if (!candidateUser.getIsDependentCandidate()) return false;
    if (party.getPartyID() == DEFAULT_PARTY_ID || party.getPartyID() == "0000" || party.getPartyID().empty()) return false;
    return party.getPartyRank() == TIER2 || party.getPartyRank() == TIER3;
}
int RegionalElection::getCandidateLimit() const { return 15; }
string RegionalElection::getGeographicScope() const { return this->scopeInfo; }
void RegionalElection::displayElectionDetails(bool showResults) const {
    Election::displayElectionDetails(showResults);
}


MetropolitanElection::MetropolitanElection(const string& id, const string& statusFile, const string& city)
    : Election(id, "Metropolitan", statusFile, city, 20) {
}
bool MetropolitanElection::isUserEligibleToVote(const User& user, const Profile& userProfile) const { return userProfile.getCity() == this->scopeInfo; }
bool MetropolitanElection::isCandidateEligibleToParticipate(const User& candidateUser, const Profile& candidateProfile, const PoliticalParty& party) const {
    if (!candidateUser.getIsRegisteredCandidate()) return false;
    if (candidateProfile.getCity() != this->scopeInfo) return false;
    return true;
}
int MetropolitanElection::getCandidateLimit() const { return 20; }
string MetropolitanElection::getGeographicScope() const { return this->scopeInfo; }
void MetropolitanElection::displayElectionDetails(bool showResults) const {
    Election::displayElectionDetails(showResults);
}


LocalElection::LocalElection(const string& id, const string& statusFile, const string& town, const string& city)
    : Election(id, "Local", statusFile, town + "," + city, 20) {
}
bool LocalElection::isUserEligibleToVote(const User& user, const Profile& userProfile) const {
    size_t comma_pos = this->scopeInfo.find(',');
    if (comma_pos == string::npos) return false;
    string town = this->scopeInfo.substr(0, comma_pos);
    string city = this->scopeInfo.substr(comma_pos + 1);
    return userProfile.getTown() == town && userProfile.getCity() == city;
}
bool LocalElection::isCandidateEligibleToParticipate(const User& candidateUser, const Profile& candidateProfile, const PoliticalParty& party) const {
    if (!candidateUser.getIsRegisteredCandidate()) return false;
    size_t comma_pos = this->scopeInfo.find(',');
    if (comma_pos == string::npos) return false;
    string town = this->scopeInfo.substr(0, comma_pos);
    string city = this->scopeInfo.substr(comma_pos + 1);
    if (candidateProfile.getTown() != town || candidateProfile.getCity() != city) return false;
    return true;
}
int LocalElection::getCandidateLimit() const { return 20; }
string LocalElection::getGeographicScope() const { return this->scopeInfo; }
void LocalElection::displayElectionDetails(bool showResults) const {
    Election::displayElectionDetails(showResults);
}