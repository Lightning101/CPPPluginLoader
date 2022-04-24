#pragma once
// includes we need
#include <string>
#include <memory>

// Test to see if we are building a DLL.
// If we are, specify that we are exporting
// to the DLL, otherwise don't worry (we
// will manually import the functions).
#ifdef __cplusplus
#define EXPORT extern "C" __declspec (dllexport)
#else
#define EXPORT __declspec (dllexport)
#endif

// This is the base class for the class
// retrieved from the DLL. This is used simply
// so that I can show how various types should
// be retrieved from a DLL. This class is to
// show how derived classes can be taken from
// a DLL.
class DLLBase {
public:
    // Make sure we call the derived classes destructors
    virtual ~DLLBase() = default;

    // Pure virtual print function, effect specific to DLL
    virtual void print(void) = 0;

    // Pure virtual function to calculate something, 
    // according to an unknown set of rules.
    virtual double calc(double val) = 0;

};

EXPORT const char* getName(void);
EXPORT const DLLBase* getObj(void);

