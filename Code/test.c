
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "MinPopVote.h"

bool test_totalEVs() {
    State aStates[10];
    int res;
    
    aStates[0].electoralVotes = 5;
    aStates[1].electoralVotes = 8;
    aStates[2].electoralVotes = 12;
    aStates[3].electoralVotes = 6;
    aStates[4].electoralVotes = 7;
    aStates[5].electoralVotes = 10;

    printf(" Checking totalEVs() for 5 States:\n");
    res = totalEVs(aStates,5);
    if (res != 38) {
        printf("  individual state EVs are 5, 8, 12, 6, 7\n");
        printf("  expected total EVs = 38, actual total EVs = %d\n",res);
        return false;
    }

    //--------------------------------------------------------   
    // TODO: Task 6 - perform additional testing to make this 
    //                test case for totalEVs() more robust
    //--------------------------------------------------------   
    
    State someSates[3];
    res = 0;

    int sample = 5;
    for (int i = 0; i < 3; i++) {
        sample = sample + 5;
        someSates[i].electoralVotes = sample; 
    }

    printf("Checking totalEVs() for 3 states...\n");
    res = totalEVs(someSates, 3);
    if (res != 45) {
        printf("  individual state EVs are 10, 15, 20\n");
        printf("  expected total EVs = 45, actual total EVs = %d\n", res);
        return false;
    }

    return true;
}

bool test_totalPVs() {
    //--------------------------------------------------------   
    // TODO: Task 6 - write your own test case for totalPVs();
    //                use test_totalEVs() as a sample;
    //                make sure to test all components
    //--------------------------------------------------------
    State someSates[3];
    int res = 0;

    int sample = 5000;
    for (int i = 0; i < 3; i++) {
        sample = sample + 5000;
        someSates[i].popularVotes = sample; 
    }

    printf("Checking totalPVs() for 3 states...\n");
    res = totalPVs(someSates, 3);
    if (res != 45000) {
        printf("  individual state PVs are 10k, 15k, 20k\n");
        printf("  expected total PVs = 45k, actual total PVs = %d\n", res);
        return false;
    }

    return true;
}

bool test_setSettings() {
    //-----------------------------------------------------------   
    // TODO: Task 6 - write your own test case for setSettings();
    //                use test_totalEVs() as a sample;
    //                make sure to test all components
    //-----------------------------------------------------------

    // Test Case 1: Valid arguments for year, fast mode, and quiet mode
    char* args1[] = { "program", "-y", "2000", "-f", "-q" };
    int argc1 = sizeof(args1) / sizeof(args1[0]);
    int year;
    bool fastMode, quietMode;
    bool result1 = setSettings(argc1, args1, &year, &fastMode, &quietMode);
    if (result1 == false || !fastMode || !quietMode || year != 2000) {
        printf("Test Case 1: FAIL\n");
        printf("  Expected: year = 2000, fastMode = true, quietMode = true\n");
        printf("  Actual: year = %d, fastMode = %s, quietMode = %s\n", year, fastMode ? "true" : "false", quietMode ? "true" : "false");
        return false;
    }

    // Test Case 2: Invalid year arg (outside valid range)
    char* args2[] = { "program", "-y", "1800" };
    int argc2 = sizeof(args2) / sizeof(args2[0]);
    bool result2 = setSettings(argc2, args2, &year, &fastMode, &quietMode);
    if (result2 == false) {
        printf("Test Case 2: FAIL\n");
        printf("  Expected: Invalid argument\n");
        return false;
    }

    return true;
}

bool test_inFilename() {
    //----------------------------------------------------------   
    // TODO: Task 6 - write your own test case for inFilename();
    //                use test_totalEVs() as a sample;
    //                make sure to test all components
    //----------------------------------------------------------
    
    char filename[40];

    // Test Case 1: Valid year, check if the filename is correct
    int year1 = 2022;
    inFilename(filename, year1);
    if (strcmp(filename, "data/2022.csv") != 0) {
        printf("Test Case 1: FAIL\n");
        printf("  Expected: data/2022.csv\n");
        printf("  Actual: %s\n", filename);
        return false;
    }

    // Test Case 2: Valid year, check if the filename is correct
    int year2 = 1828;
    inFilename(filename, year2);
    if (strcmp(filename, "data/1828.csv") != 0) {
        printf("Test Case 2: FAIL\n");
        printf("  Expected: data/1828.csv\n");
        printf("  Actual: %s\n", filename);
        return false;
    }

    return true;
}

bool test_outFilename() {
    //-----------------------------------------------------------   
    // TODO: Task 6 - write your own test case for outFilename();
    //                use test_totalEVs() as a sample;
    //                make sure to test all components
    //-----------------------------------------------------------
    
    char filename[40];

    // Test Case 1: Valid year, check if the filename is correct
    int year1 = 2022;
    outFilename(filename, year1);
    if (strcmp(filename, "toWin/2022_win.csv") != 0) {
        printf("Test Case 1: FAIL\n");
        printf("  Expected: toWin/2022_win.csv\n");
        printf("  Actual: %s\n", filename);
        return false;
    }

    // Test Case 2: Valid year, check if the filename is correct
    int year2 = 1828;
    outFilename(filename, year2);
    if (strcmp(filename, "toWin/1828_win.csv") != 0) {
        printf("Test Case 2: FAIL\n");
        printf("  Expected: toWin/1828_win.csv\n");
        printf("  Actual: %s\n", filename);
        return false;
    }

    return true;
}

bool test_parseLine() {
    //---------------------------------------------------------   
    // TODO: Task 6 - write your own test case for parseLine();
    //                use test_totalEVs() as a sample;
    //                make sure to test all components
    //---------------------------------------------------------

    char line1[] = "Alabama,AL,9,100000";
    char line2[] = "Invalid Line";

    State state1, state2;

    // Test Case 1: Valid line, check if the State struct is filled correctly
    bool result1 = parseLine(line1, &state1);
    if (result1 == false) {
        printf("Test Case 1: FAIL\n");
        printf("  Expected: name = Alabama, postalCode = AL, electoralVotes = 9, popularVotes = 100000\n");
        printf("  Actual: name = %s, postalCode = %s, electoralVotes = %d, popularVotes = %d\n", state1.name, state1.postalCode, state1.electoralVotes, state1.popularVotes);
        return false;
    }
    
    // Test Case 2: Valid line, check if the State struct is filled correctly
    bool result2 = parseLine(line2, &state2);
    if (result2 == true) {
        printf("Test Case 2: FAIL\n");
        printf("  Expected: Invalid line, return false\n");
        return false;
    }

    return true;
}

bool test_readElectionData() {
    //----------------------------------------------------------------   
    // TODO: Task 6 - write your own test case for readElectionData();
    //                make sure to test all components
    //----------------------------------------------------------------

    State someSates[51];
    int nStates = 0;

    if (!readElectionData("toWin/1828_win.csv", someSates, &nStates) && nStates == 15) {
        printf("Test Case: FAIL\n");
        printf("  Expected: Successful read, nStates = 15\n");
        printf("  Actual: Failed to read, nStates = %d\n", nStates);
        return false;
    }

    return true;
}

bool test_minPVsSlow() {
    //----------------------------------------------------------------- 
    // TODO: Task 7 - write your own test case for minPopVoteAtLeast();
    //                make sure to test all components
    //-----------------------------------------------------------------
    
    // Sample data
    State allStates[4] = {
        {"StateA", "AA", 5, 60},
        {"StateB", "BB", 2, 20},
        {"StateC", "CC", 8, 70},
        {"StateD", "DD", 3, 30}
    };
    int szStates = 4;

    // Test minPopVoteToWin
    MinInfo result = minPopVoteToWin(allStates, szStates);
    if (result.sufficientEVs == false || result.szSomeStates != 2 || result.subsetPVs != 47) {
        printf("  Expected: Winning subset with 47 PVs\n");
        printf("  Actual: %s, PVs: %d, EVs: %d\n", result.sufficientEVs ? "Success" : "Fail", result.subsetPVs, totalEVs(result.someStates, result.szSomeStates));
        
        // Output the states for verification
        printf("  States in the subset:\n");
        for (int i = 0; i < result.szSomeStates; i++) {
            printf("    %s, %s, EVs: %d, PVs: %d\n", result.someStates[i].name, result.someStates[i].postalCode, result.someStates[i].electoralVotes, result.someStates[i].popularVotes);
        }
        
        return false;
    }

    return true;
}

bool test_minPVsFast() {
    //--------------------------------------------------------------------- 
    // TODO: Task 8 - write your own test case for minPopVoteAtLeastFast();
    //                make sure to test all components
    //---------------------------------------------------------------------
    
    // Sample data
    State allStates[4] = {
        {"StateA", "AA", 5, 60},
        {"StateB", "BB", 2, 20},
        {"StateC", "CC", 8, 70},
        {"StateD", "DD", 3, 30}
    };
    int szStates = 4;

    // Test minPopVoteToWin
    MinInfo result = minPopVoteToWinFast(allStates, szStates);
    if (result.sufficientEVs != true || result.szSomeStates != 2 || result.subsetPVs != 47) {
        printf("  Expected: Winning subset with 47 PVs\n");
        printf("  Actual: %s, PVs: %d, EVs: %d\n", result.sufficientEVs ? "Success" : "Fail", result.subsetPVs, totalEVs(result.someStates, result.szSomeStates));
        
        // Output the states for verification
        printf("  States in the subset:\n");
        for (int i = 0; i < result.szSomeStates; i++) {
            printf("    %s, %s, EVs: %d, PVs: %d\n", result.someStates[i].name, result.someStates[i].postalCode, result.someStates[i].electoralVotes, result.someStates[i].popularVotes);
        }
        
        return false;
    }

    return true;
}

int main() {
    printf("Welcome to the Popular Vote Minimizer Testing Suite!\n\n");
    
    printf("Testing totalEVs()...\n"); 
    if (test_totalEVs()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing totalPVs()...\n"); 
    if (test_totalPVs()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing setSettings()...\n"); 
    if (test_setSettings()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }
    
    printf("Testing inFilename()...\n"); 
    if (test_inFilename()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing outFilename()...\n"); 
    if (test_outFilename()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing parseLine()...\n"); 
    if (test_parseLine()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing readElectionData()...\n"); 
    if (test_readElectionData()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }
    
    printf("Testing minPopVoteToWin()...\n"); 
    if (test_minPVsSlow()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing minPopVoteToWinFast()...\n"); 
    if (test_minPVsFast()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    return 0;
}
