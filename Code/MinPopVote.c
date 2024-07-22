#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "MinPopVote.h"

bool setSettings(int argc, char** argv, int* year, bool* fastMode, bool* quietMode) {
    //------------------------------------------------   
    // TODO: Task 1 - write the setSettings() function
    //------------------------------------------------
    
    // initialize default values
    *year = 0; 
    *quietMode = false; 
    *fastMode = false; 
    char* year_str = "";
    int year_int = 0;

    //sample processing of command-line arg...
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-q") == 0) { // [-q] = quiet mode; if ON, do not print the full State list read-in
            *quietMode = true;
        } else if (strcmp(argv[i], "-f") == 0) { // [-f] = fast mode; if ON, use the "fast" version of the functions
            *fastMode = true;
        } else if (strcmp(argv[i], "-y") == 0 && i + 1 < argc) { // [-y yr] = sets the election year for the program
            // get the str year entered
            year_str = argv[i + 1]; // the next arg is the year
            // convert char array to int
            year_int = atoi(year_str);
            // check if valid year
            if (year_int >= 1828 && year_int <= 2020 && year_int % 4 == 0) {
                *year = year_int;
            }
        }
    }

    // Check if all command-line arguments are valid
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-q") != 0 && strcmp(argv[i], "-f") != 0 && strcmp(argv[i], "-y") != 0 && strcmp(argv[i], year_str) != 0) {
            return false; // Invalid argument found
        }
    }

    return true; // All command-line arguments are valid

}

void inFilename(char* filename, int year) {
    //------------------------------------------------   
    // TODO: Task 2 - write the inFilename() function
    //------------------------------------------------

    // print into the char array 'filename'
    sprintf(filename, "data/%d.csv", year);
    return;
}

void outFilename(char* filename, int year) {
    //------------------------------------------------   
    // TODO: Task 2 - write the outFilename() function
    //------------------------------------------------
    
    // print into the char array 'filename'
    sprintf(filename, "toWin/%d_win.csv", year);
    return;
}

bool parseLine(char* line, State* myState) {
    //------------------------------------------------   
    // TODO: Task 4 - write the parseLine() function
    //------------------------------------------------

    // check for new line char
    int len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
        line[len - 1] = '\0'; // replace with null terminator char
    }
    
    // scan all items
    int numItems = sscanf(line, "%20[^,],%2[^,],%d,%d", myState->name, myState->postalCode, &myState->electoralVotes, &myState->popularVotes);

    // make sure num items scanned is correct
    if (numItems == 4) {
        return true;
    } else {
        return false;
    }
}

bool readElectionData(char* filename, State* allStates, int* nStates) {
    *nStates = 0; //required initialization prior to incrementing. 
    //-----------------------------------------------------   
    // TODO: Task 5 - write the readElectionData() function
    //-----------------------------------------------------

    // Open file for reading check if file exists
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return false;
    }

    // Read the file line by line
    char line[40];
    while (fgets(line, sizeof(line), file) != NULL) {
        // check if end of file reached
        if (feof(file)) {
            break;
        }

        // fill the struct array
        State currSate;
        parseLine(line, &currSate);

        // add current state to array. max cap of 51
        if (*nStates <= 50) {
            allStates[*nStates] = currSate;
            (*nStates)++;
        }
    }

    // Close the file
    fclose(file);

    return true;
    
}

int totalEVs(State* states, int szStates) {
    //----------------------------------------------   
    // TODO: Task 6 - write the totalEVs() function;
    //                note test_totalEVs() is 
    //                provided in test.c to test
    //                the functionality of totalEVs()
    //                   >> make build_test
    //                   >> make run_test 
    //----------------------------------------------

    int total = 0;
    for (int i = 0; i < szStates; i++) {
       total += states[i].electoralVotes;
    }
    return total;
}

int totalPVs(State* states, int szStates) {
    //--------------------------------------------------   
    // TODO: Task 6 - write the totalPVs() function;
    //                then, write your own test function 
    //                test_totalPVs() in test.c
    //                   >> make build_test
    //                   >> make run_test 
    //--------------------------------------------------
    
    int total = 0;
    for (int i = 0; i < szStates; i++) {
       total += states[i].popularVotes;
    }
    return total;
}

MinInfo minPopVoteAtLeast(State* states, int szStates, int start, int EVs) {
    //----------------------------------------------
    // TODO: Task 7 - write minPopVoteAtLeast();
    //                a recursive helper function;
    //                returns MinInfo for the subset
    //                of [states] from index [start]
    //                to the end with the minimum
    //                popular vote total that has 
    //                sufficient [EVs] to win
    //---------------------------------------------- 

    // Base cases
    if (start == szStates) {
        MinInfo result;
        result.szSomeStates = 0;
        result.sufficientEVs = EVs <= 0;
        result.subsetPVs = result.sufficientEVs ? 0 : 1000;  // Initialize with a large value
        return result;
    }

    if (EVs <= 0) {
        // No additional states should be added to the subset
        MinInfo result;
        result.szSomeStates = 0;
        result.sufficientEVs = true;
        result.subsetPVs = 0;
        return result;
    }

    // Include the current state in the subset
    MinInfo withCurrent = minPopVoteAtLeast(states, szStates, start + 1, EVs - states[start].electoralVotes);
    withCurrent.someStates[withCurrent.szSomeStates++] = states[start];
    withCurrent.subsetPVs += states[start].popularVotes / 2 + 1;

    // Exclude the current state from the subset
    MinInfo withoutCurrent = minPopVoteAtLeast(states, szStates, start + 1, EVs);

    // Choose the subset with the minimum popular votes and sufficient EVs
    if (withCurrent.sufficientEVs && withoutCurrent.sufficientEVs) {
        if (withCurrent.subsetPVs < withoutCurrent.subsetPVs) {
            return withCurrent;
        } else {
            return withoutCurrent;
        }
    } else if (withCurrent.sufficientEVs) {
        return withCurrent;
    } else if (withoutCurrent.sufficientEVs) {
        return withoutCurrent;
    } else {
        // Neither including nor excluding the current state provides sufficient EVs
        MinInfo result;
        result.szSomeStates = 0;
        result.sufficientEVs = false;
        result.subsetPVs = 0;
        return result;
    }
}

MinInfo minPopVoteToWin(State* states, int szStates) {
    int totEVs = totalEVs(states,szStates);
    int reqEVs = totEVs/2 + 1; // required EVs to win election
    return minPopVoteAtLeast(states, szStates, 0, reqEVs);
}

MinInfo minPopVoteAtLeastFast(State* states, int szStates, int start, int EVs, MinInfo** memo) {
    //----------------------------------------------   
    // TODO: Task 8 - write minPopVoteAtLeastFast();
    //                start by copying in fully
    //                functioning code from 
    //                minPopVoteAtLeast() and make
    //                additions for memoization
    //---------------------------------------------- 
    
    // Base cases
    if (start == szStates) {
        MinInfo result;
        result.szSomeStates = 0;
        result.sufficientEVs = EVs <= 0;
        result.subsetPVs = result.sufficientEVs ? 0 : 1000;  // Initialize with a large value
        return result;
    }

    if (EVs <= 0) {
        // No additional states should be added to the subset
        MinInfo result;
        result.szSomeStates = 0;
        result.sufficientEVs = true;
        result.subsetPVs = 0;
        return result;
    }

    if (memo[start][EVs].subsetPVs != -1) {
        return memo[start][EVs];
    }

    // Include the current state in the subset
    MinInfo withCurrent = minPopVoteAtLeastFast(states, szStates, start + 1, EVs - states[start].electoralVotes, memo);
    withCurrent.someStates[withCurrent.szSomeStates++] = states[start];
    withCurrent.subsetPVs += states[start].popularVotes / 2 + 1;

    // Exclude the current state from the subset
    MinInfo withoutCurrent = minPopVoteAtLeastFast(states, szStates, start + 1, EVs, memo);

    // Choose the subset with the minimum popular votes and sufficient EVs
    if (withCurrent.sufficientEVs && withoutCurrent.sufficientEVs) {
        if (withCurrent.subsetPVs < withoutCurrent.subsetPVs) {
            return memo[start][EVs] = withCurrent;
        } else {
            return memo[start][EVs] = withoutCurrent;
        }
    } else if (withCurrent.sufficientEVs) {
        return memo[start][EVs] = withCurrent;
    } else if (withoutCurrent.sufficientEVs) {
        return memo[start][EVs] = withoutCurrent;
    } else {
        // Neither including nor excluding the current state provides sufficient EVs
        MinInfo result;
        result.szSomeStates = 0;
        result.sufficientEVs = false;
        result.subsetPVs = 0;
        return memo[start][EVs] = result;
    }
}

MinInfo minPopVoteToWinFast(State* states, int szStates) {
    int totEVs = totalEVs(states,szStates);
    int reqEVs = totEVs/2 + 1; // required EVs to win election

    MinInfo** memo = (MinInfo**)malloc((szStates+1)*sizeof(MinInfo*));
    for (int i = 0; i < szStates+1; ++i) {
        memo[i] = (MinInfo*)malloc((reqEVs+1)*sizeof(MinInfo));
        for (int j = 0; j < reqEVs+1; ++j) {
            memo[i][j].subsetPVs = -1;
        }
    }
    MinInfo res = minPopVoteAtLeastFast(states, szStates, 0, reqEVs, memo);
    
    //----------------------------------------------   
    // TODO: Task 8 - [memo] will go out of scope 
    //                upon return, so free all
    //                heap-allocated memory for 
    //                [memo] before return 
    //---------------------------------------------- 

    // Free up all heap-allocated memory for memo
    for (int i = 0; i < szStates + 1; ++i) {
        free(memo[i]);
    }
    free(memo);

    return res;
}

bool writeSubsetData(char* filenameW, int totEVs, int totPVs, int wonEVs, MinInfo toWin) {
    //-----------------------------------------------------   
    // TODO: Task 9 - write the writeSubsetData() function
    //-----------------------------------------------------
    
    // Open the file for writing
    FILE* file = fopen(filenameW, "w");
    
    // Check if the file could be opened
    if (file == NULL) {
        return false;
    }

    // Write the first line with the summary information
    fprintf(file, "%d,%d,%d,%d\n", totEVs, totPVs, wonEVs, toWin.subsetPVs);

    // Write the individual State details
    for (int i = toWin.szSomeStates - 1; i >= 0; i--) {
        State state = toWin.someStates[i];
        fprintf(file, "%s,%s,%d,%d\n", state.name, state.postalCode, state.electoralVotes, state.popularVotes / 2 + 1);
    }

    // Close the file
    fclose(file);

    return true;
}
