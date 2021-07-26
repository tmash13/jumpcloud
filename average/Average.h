//
//  Average.hpp
//  average
//
//  Created by Tamer Mash on 7/24/21.
//
//  This class takes in json strings in the format:
//      {"action":"theAction", "time":100 }
//  Once done adding the actions, the GetStats() function can be called to get the average time of each action
//
//  Notes:
//      - This class uses the JSON parser from https://github.com/nlohmann/json
//

#pragma once

#include <stdio.h>
#include <string>
#include <sstream>
#include <map>

class Average
{
public:
    Average();
    virtual ~Average();

// public interface
public:
    // adds an action
    bool AddAction(const std::string action);
    // returns the actions average times
    std::string GetStats();
    // returns the error string
    std::string GetError()          { return _errorStr.str();   }
    // runs a test
    static void Test();
    // removes all the added actions
    void ClearActions();

// helper functions
protected:
    static void CheckTestStats(const std::string jsString);
    
// member variables
protected:
    // holds the total times for aceraging
    std::map<std::string, float> _averagesMap;
    // holds the number of entries for each action
    std::map<std::string, int> _averagesCount;
    // error string
    std::stringstream _errorStr;
};
