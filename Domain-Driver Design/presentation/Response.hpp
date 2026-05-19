#pragma once

#include <string>

using namespace std;

enum class ResponseStatus {
    Success,
    Failure
};

template <typename T>
struct Response {
    ResponseStatus status;
    T body;
    string message;

    static Response<T> success(const T& body, const string& message = "") {
        return Response<T>{ResponseStatus::Success, body, message};
    }

    static Response<T> failure(const T& body, const string& message) {
        return Response<T>{ResponseStatus::Failure, body, message};
    }

    static Response<T> failure(const string& message) {
        return Response<T>{ResponseStatus::Failure, T(), message};
    }
};
