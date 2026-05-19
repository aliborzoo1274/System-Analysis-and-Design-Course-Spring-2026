#include "Registration.hpp"

#include <algorithm>

using namespace std;

Registration::Registration() : id(0), studentId(0) {}

Registration::Registration(int id, int studentId) : id(id), studentId(studentId) {}

int Registration::getId() const {
    return id;
}

int Registration::getStudentId() const {
    return studentId;
}

const vector<int>& Registration::getOfferingIds() const {
    return offeringIds;
}

bool Registration::hasOffering(int offeringId) const {
    return find(offeringIds.begin(), offeringIds.end(), offeringId) != offeringIds.end();
}

void Registration::addOffering(int offeringId) {
    if (!hasOffering(offeringId)) {
        offeringIds.push_back(offeringId);
    }
}

void Registration::removeOffering(int offeringId) {
    offeringIds.erase(remove(offeringIds.begin(), offeringIds.end(), offeringId), offeringIds.end());
}
