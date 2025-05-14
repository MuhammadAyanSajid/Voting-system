#ifndef PROFILE_H
#define PROFILE_H

#include <string>
#include <fstream>
#include "DateTime.h"

using namespace std;

class Profile {
private:
    string cnic;
    string firstName;
    string lastName;
    string age;
    string phone;
    string gender;
    string state;
    string city;
    string town;
    string postalCode;
    string lastLoginTimestamp;
    string signupTimestamp;

    

public:
    Profile();
    ~Profile() = default;

    void parseDataLine(const string& line);

    void setCnic(const string& c);
    void setFirstName(const string& fn);
    void setLastName(const string& ln);
    void setAge(const string& a);
    void setPhone(const string& p);
    void setGender(const string& g);
    void setState(const string& s);
    void setCity(const string& c);
    void setTown(const string& t);
    void setPostalCode(const string& pc);
    void setLastLoginTimestamp(const string& ts);
    void setSignupTimestamp(const string& ts);


    string getCnic() const;
    string getFirstName() const;
    string getLastName() const;
    string getFullName() const;
    string getAge() const;
    string getPhone() const;
    string getGender() const;
    string getState() const;
    string getCity() const;
    string getTown() const;
    string getPostalCode() const;
    string getLastLoginTimestamp() const;
    string getSignupTimestamp() const;

    bool loadFromFile(const string& userCnicToLoad, const string& dataFilePath);
    bool updateTimestampInFile(const string& userCnicToUpdate, const string& dataFilePath, const string& newTimestamp);
};

#endif // PROFILE_H