#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <string>
using namespace std;

// Exceptions
class expectedToken: public std::exception {
    private: string message;

    public: expectedToken(string);
    string what();
};

class catchableExpectedToken: public std::exception {
    private: string message;

    public: catchableExpectedToken(string);
    string what();
};

class staticSemanticsException: public std::exception {
    private: string message;

    public: staticSemanticsException(string);
    string what();
};

class InvalidLabel : public std::exception {
public:
  char *what();
};

class InvalidSymbol : public std::exception {
public:
  char *what();
};

// Error printing functions
string errorMessage(string, string, string);
string staticSemanticsErrorMessage(string);

#endif