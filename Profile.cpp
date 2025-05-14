#include "Profile.h"
#include <iostream>

Profile::Profile() : age("0") {}

void Profile::setCnic(const string& c) { cnic = c; }
void Profile::setFirstName(const string& fn) { firstName = fn; }
void Profile::setLastName(const string& ln) { lastName = ln; }
void Profile::setAge(const string& a) { age = a; }
void Profile::setPhone(const string& p) { phone = p; }
void Profile::setGender(const string& g) { gender = g; }
void Profile::setState(const string& s) { state = s; }
void Profile::setCity(const string& c) { city = c; }
void Profile::setTown(const string& t) { town = t; }
void Profile::setPostalCode(const string& pc) { postalCode = pc; }
void Profile::setLastLoginTimestamp(const string& ts) { lastLoginTimestamp = ts; }
void Profile::setSignupTimestamp(const string& ts) { signupTimestamp = ts; }


string Profile::getCnic() const { return cnic; }
string Profile::getFirstName() const { return firstName; }
string Profile::getLastName() const { return lastName; }
string Profile::getFullName() const { return firstName + " " + lastName; }
string Profile::getAge() const { return age; }
string Profile::getPhone() const { return phone; }
string Profile::getGender() const { return gender; }
string Profile::getState() const { return state; }
string Profile::getCity() const { return city; }
string Profile::getTown() const { return town; }
string Profile::getPostalCode() const { return postalCode; }
string Profile::getLastLoginTimestamp() const { return lastLoginTimestamp; }
string Profile::getSignupTimestamp() const { return signupTimestamp; }


void Profile::parseDataLine(const string& line) {
    size_t startPos = 0;
    size_t endPos = 0;
    string field;
    int fieldCount = 0;
    endPos = line.find('-', startPos);
    if (endPos == string::npos) { setCnic(line.substr(startPos)); return; }
    setCnic(line.substr(startPos, endPos - startPos));
    startPos = endPos + 1;
    fieldCount++;
    endPos = line.find('-', startPos);
    if (endPos == string::npos) { setFirstName(line.substr(startPos)); return; }
    setFirstName(line.substr(startPos, endPos - startPos));
    startPos = endPos + 1;
    fieldCount++;
    endPos = line.find('-', startPos);
    if (endPos == string::npos) { setLastName(line.substr(startPos)); return; }
    setLastName(line.substr(startPos, endPos - startPos));
    startPos = endPos + 1;
    fieldCount++;
    endPos = line.find('-', startPos);
    if (endPos == string::npos) { setAge(line.substr(startPos)); return; }
    setAge(line.substr(startPos, endPos - startPos));
    startPos = endPos + 1;
    fieldCount++;
    endPos = line.find('-', startPos);
    if (endPos == string::npos) { setPhone(line.substr(startPos)); return; }
    setPhone(line.substr(startPos, endPos - startPos));
    startPos = endPos + 1;
    fieldCount++;
    endPos = line.find('-', startPos);
    if (endPos == string::npos) { setGender(line.substr(startPos)); return; }
    setGender(line.substr(startPos, endPos - startPos));
    startPos = endPos + 1;
    fieldCount++;
    endPos = line.find('-', startPos);
    if (endPos == string::npos) { setState(line.substr(startPos)); return; }
    setState(line.substr(startPos, endPos - startPos));
    startPos = endPos + 1;
    fieldCount++;
    endPos = line.find('-', startPos);
    if (endPos == string::npos) { setCity(line.substr(startPos)); return; }
    setCity(line.substr(startPos, endPos - startPos));
    startPos = endPos + 1;
    fieldCount++;
    endPos = line.find('-', startPos);
    if (endPos == string::npos) { setTown(line.substr(startPos)); return; }
    setTown(line.substr(startPos, endPos - startPos));
    startPos = endPos + 1;
    fieldCount++;
    endPos = line.find('-', startPos);
    if (endPos == string::npos) {
        setPostalCode(line.substr(startPos));
        setLastLoginTimestamp("Unavailable");
        setSignupTimestamp("Unavailable");
        return;
    }
    setPostalCode(line.substr(startPos, endPos - startPos));
    startPos = endPos + 1;
    fieldCount++;
    if (startPos < line.length()) {
        string timestampField = line.substr(startPos);
        setLastLoginTimestamp(timestampField);
        setSignupTimestamp(timestampField);
    }
    else {
        setLastLoginTimestamp("Unavailable");
        setSignupTimestamp("Unavailable");
    }
}


bool Profile::loadFromFile(const string& userCnicToLoad, const string& dataFilePath) {
    ifstream dataFileIn(dataFilePath);
    if (!dataFileIn.is_open()) {
        return false;
    }

    string line;
    bool found = false;
    while (getline(dataFileIn, line)) {
        if (line.empty()) continue;
        size_t firstDash = line.find('-');
        if (firstDash == string::npos || firstDash == 0) continue;
        string recordCnic = line.substr(0, firstDash);

        if (recordCnic == userCnicToLoad) {
            parseDataLine(line);
            found = true;
            break;
        }
    }
    dataFileIn.close();
    return found;
}

bool Profile::updateTimestampInFile(const string& userCnicToUpdate, const string& dataFilePath, const string& newTimestamp) {
    ifstream dataFileIn(dataFilePath);
    if (!dataFileIn.is_open()) {
        ofstream createFile(dataFilePath, ios::app);
        if (!createFile.is_open()) return false;
        createFile.close();
        return true;
    }

    string fullFileContent = "";
    string line;
    bool foundUser = false;
    bool updateOccurred = false;

    while (getline(dataFileIn, line)) {
        if (line.empty()) { fullFileContent += "\n"; continue; }
        size_t firstDash = line.find('-');
        if (firstDash == string::npos || firstDash == 0) { fullFileContent += line + "\n"; continue; }
        string recordCnic = line.substr(0, firstDash);

        if (recordCnic == userCnicToUpdate) {
            foundUser = true;
            size_t timestampStartPos = 0;
            int dashCount = 0;
            for (size_t i = 0; i < line.length(); ++i) {
                if (line[i] == '-') {
                    dashCount++;
                    if (dashCount == 10) {
                        timestampStartPos = i + 1;
                        break;
                    }
                }
            }

            if (timestampStartPos > 0 && timestampStartPos < line.length()) {
                string mainDataPart = line.substr(0, timestampStartPos);
                string newLine = mainDataPart + newTimestamp;
                fullFileContent += newLine + "\n";
                updateOccurred = true;
            }
            else if (dashCount == 9 && line.back() != '-') {
                fullFileContent += line + "-" + newTimestamp + "\n";
                updateOccurred = true;
            }
            else if (dashCount == 9 && line.back() == '-') {
                fullFileContent += line + newTimestamp + "\n";
                updateOccurred = true;
            }
            else { 
                fullFileContent += line + "\n";
            }
        }
        else {
            fullFileContent += line + "\n";
        }
    }
    dataFileIn.close();

    ofstream dataFileOut(dataFilePath, ios::trunc);
    if (!dataFileOut.is_open()) {
        return false;
    }
    dataFileOut << fullFileContent;
    dataFileOut.close();

    return true;
}