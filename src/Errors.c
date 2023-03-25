#include "Errors.h"

const char* ERRORS[] = {
        "",
        "Failed to open file",
        "Macro syntax error",
        "Label syntax error",
        "Multiple label definitions",
        "Data syntax error",
        "Extern missing argument",
        "Entry missing argument",
        "Command not found",
        "Invalid destination argument",
        "Invalid source argument",
        "Preserved keyword error",
        "Too many arguments",
        "Too few arguments",
        "Extern too many arguments",
        "Entry too many arguments",
        "Undefined label",
        "Multiple extern request for label",
        "Multiple entry calls for label",
        "Conflict local and external label",
        "String missing argument",
        "String syntax error",
        "Parameter value out of range",
        "Missing code after label",
        "String missing quote",
        "Data out of range",
        "Command or data instruction not found",
        "Memory overflow - file too large",
};

const char* WARNINGS[] = {
        "No file was specified",
        "Macro syntax warning",
        "Label definition before extern",
        "Label definition before entry"
};
