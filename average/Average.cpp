//
//  Average.cpp
//  average
//
//  Created by Tamer Mash on 7/24/21.
//
//  See header file comments for more details
//

#include <iostream>

#include "nlohmann/json.hpp"

#include "Average.h"

using nlohmann::json;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Const/Dest
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Average::Average()
{
    
}
Average::~Average()
{
    
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This functions add the time for the given action in the
// passed in json string. Format is: {"action":"theAction", "time":time }
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Average::AddAction(const std::string action)
{
    bool success = true;
    json j;
    
    // parse the given json into an object
    try
    {
        j = json::parse(action);
    }
    catch (json::parse_error& ex)
    {
        _errorStr << "parse error at byte " << ex.byte << " for action \"" << action << "\"" << std::endl;
        success = false;
    }
    
    if (success)
    {
        // add the time to the action
        if (j.find("action") != j.end())
        {
            if (j.find("time") != j.end())
            {
                _averagesMap[j["action"]] += (int)j["time"];
                // increment number of entries for averaging later
                _averagesCount[j["action"]] += 1;
            }
            else
            {
                _errorStr << "  Missing time key in item:" << std::endl << "    " << j.dump() << std::endl;
                success = false;
            }
        }
        else
        {
            _errorStr << "  Missing action key in item:" << std::endl << "    " << j.dump() << std::endl;
            success = false;
        }
    }
                   
    return success;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This function returns the average times for each action
// as a json string in the format:
//      {"action":"action1", "avg":time1},
//      {"action":"actions", "avg":time2}
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::string Average::GetStats()
{
    json js;
    // map that holds the action name and it's average time
    std::map<std::string, float> aveMap;
    
    // go through all the actions and calculate the averages
    for (auto item : _averagesMap)
    {
        aveMap[item.first] = (_averagesMap[item.first] / _averagesCount[item.first]);
    }
    // fill a json object with the results
    for (auto item : aveMap)
    {
        js[item.first] = item.second;
    }
    // return the json string
    return js.dump();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Clears all the actions that were added previously
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Average::ClearActions()
{
    // clear the maps
    _averagesMap.clear();
    _averagesCount.clear();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This function tests the AddAction() and GetStats() functions
//
// Note:
//      Not using any test framework like gtest to limit
//      the number of external libraries as per requirement
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Average::Test()
{
    // Test 1 (fail case)
    Average ave;
    bool success = true;
    
    std::cout << "Running test 1...";
    
    // add some values
    success = ave.AddAction("{\"action\":\"jump\", \"time\":100}");
    success &= ave.AddAction("{\"action\":\"run\", \"time\":75}");
    success &= ave.AddAction("{\"action\":\"jump\", \"time\":300}");

    if (!success)
    {
        std::cerr << "FAILED" << std::endl << ave.GetError();
    }
    else
    {
        // check that the result is as expected
        std::string jsString = ave.GetStats();
        CheckTestStats(jsString);
    }
    
    
    // Test 2 (failure case)
    ave.ClearActions();
    success = true;
        
    std::cout << std::endl << "Running test 2...";
    
    // add some values
    success = ave.AddAction("{\"Action\":\"jump\", \"time\":100}");
    success &= ave.AddAction("{\"action\":\"run\", \"time\":75}");
    success &= ave.AddAction("{\"action\":\"jump\", \"time\":300}");

    if (!success)
    {
        std::cerr << "FAILED" << std::endl << ave.GetError();
    }
    else
    {
        // check that the result is as expected
        std::string jsString = ave.GetStats();
        CheckTestStats(jsString);
    }
    
    
    // Test 3 (success case)
    ave.ClearActions();
    success = true;
    
    std::cout << std::endl << "Running test 3...";
    
    // add some values
    success = ave.AddAction("{\"action\":\"jump\", \"time\":100}");
    success &= ave.AddAction("{\"action\":\"run\", \"time\":75}");
    success &= ave.AddAction("{\"action\":\"jump\", \"time\":200}");

    if (!success)
    {
        std::cerr << "FAILED" << std::endl << ave.GetError();
    }
    else
    {
        // check that the result is as expected
        std::string jsString = ave.GetStats();
        CheckTestStats(jsString);
    }
    
    // Test 4 (invalid json)
    ave.ClearActions();
    success = true;
    
    std::cout << std::endl << "Running test 4...";
    
    // add some values
    success = ave.AddAction("{\"action\"s:\"jump\", \"time\":100}");
    success &= ave.AddAction("{\"action\":\"run\", w\"time\":75}");
    success &= ave.AddAction("{\"action\":\"jump\", \"time\":200}");

    if (!success)
    {
        std::cerr << "FAILED" << std::endl << ave.GetError();
    }
    else
    {
        // check that the result is as expected
        std::string jsString = ave.GetStats();
        CheckTestStats(jsString);
    }

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Helper function that checks the stats averages
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Average::CheckTestStats(const std::string jsString)
{
    bool success = true;
    // parse into json to inspect the items
    json js;
    try
    {
        js = json::parse(jsString);
    }
    catch (json::parse_error& ex)
    {
        std::cerr << "FAILED" << std::endl << "  parse error at byte " << ex.byte << std::endl;
        success = false;
    }
    if (success)
    {
        // check the averages
        if (js["jump"] != 150)
        {
            std::cerr << "FAILED" << std::endl << "  Invalid average for jump. Expected is 150, actual is " << js["jump"] << std::endl;
            success = false;
        }
        if (js["run"] != 75)
        {
            std::cerr << "FAILED" << std::endl << "  Invalid average for run. Expected is 75, actual is " << js["run"] << std::endl;
            success = false;
        }

        if (success)
        {
            std::cout << "SUCCESS" << std::endl << "averages are:" << std::endl;
            std::cout << jsString << std::endl;
        }
    }
}
