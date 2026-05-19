#pragma once

#include <vector>

using namespace std;

class Registration {
public:
    Registration();
    Registration(int id, int studentId);

    int getId() const;
    int getStudentId() const;
    const vector<int>& getOfferingIds() const;

    bool hasOffering(int offeringId) const;
    void addOffering(int offeringId);
    void removeOffering(int offeringId);
    
private:
    int id;
    int studentId;
    vector<int> offeringIds;

};
