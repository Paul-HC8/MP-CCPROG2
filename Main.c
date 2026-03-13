    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    #include <Windows.h>
    #include <time.h>

    #define HOME_ADVANTAGE 3   

    
   

    // ANSI Color Codes
    #define COLOR_PRX       "\033[38;5;91m"  // Extended Purple
    #define COLOR_NRG       "\033[38;5;202m" // Extended Orange
    #define COLOR_G2        "\033[38;5;9m"   // Default Red
    #define COLOR_TL        "\033[38;5;27m"  // Extended Blue
    #define COLOR_RESET     "\033[0m"        // Reset

    //Global variables 
    char teamColors[4][20] = {COLOR_PRX, COLOR_G2, COLOR_NRG, COLOR_TL};
    char teamNames[4][50] = {"Paper_Rex", "G2", "NRG", "Team_Liquid"};

    //Function prototypes
    void simulateGame(struct Team teams[], struct GameRecord gameRecords[], int *gameCount, struct stats boxScorePlayers[]);
    void loadLeagueData(struct Team teams[4], int *gameCount);
    void saveAlltoFile (struct Team teams[4]);

    void resetData(struct Team teams[4], struct GameRecord gameRecords[100], int *gameCount);
    void updateGameHistory(int HomeTeamID, int AwayTeamID, int HomeScore, int AwayScore, int *gameCount);
    void standings(struct Team teams[4]);
    void averages(struct Team teams[4]);
    void acePlayerData(struct Team teams[4]);
    void scorebook(struct GameRecord gameRecords[100], int gameCount);
    void mvpRace(struct Team teams[4]); 
    void head2head(struct Team teams[4], struct GameRecord gameRecords[100], int gameCount);
    void viewBoxScore(struct GameRecord gameRecords[100], int gameCount);


    //Structures
    struct stats
    {
        int points;
        int rebounds;
        int assists;
    };

    struct Player 
    {
        char lastName[50];
        int jerseyNum;
        int totalPoints;
        int totalRebounds;
        int totalAssists;
        float avgPoints;
        float avgRebounds;
        float avgAssists;
        int gamesPlayed;
    };

    struct Team 
    {
        char teamName[50];
        struct Player MainPlayers[5];
        int wins;
        int losses;
    };

    struct GameRecord 
    {
        int gameID;
        char homeTeam[31];
        char awayTeam[31];
        int homeScore;
        int awayScore;
    };

    int main()
    {
        struct Team teams[4]; // Array to hold 4 teams in the league
        struct GameRecord gameRecords[100]; // Array to hold up to 100 game records
        struct stats boxScorePlayers[10]; // Array to hold stats for 10 players on the court
        int choice = 0;
        int gameCount = 0; // Counter for number of games played
        srand(time(NULL)); // Seed the random number generator
        // Load league data 
        loadLeagueData(teams, &gameCount);


        // Prints out the Menu/Interface for our Simulator
        while (choice != 9) 
        {
            system("cls");
            printf("=========================================\n");
            printf(">>>       VALORANT MASTERS DLSU       <<<\n");
            printf("=========================================\n");
            printf("[0] CREATE NEW LEAGUE\n");
            printf("[1] THE TIP-OFF (SIMULATE GAME)\n");
            printf("[2] THE ROAD TO NATIONALS (STANDINGS)\n");
            printf("[3] TEAM SCOUTING REPORT (AVERAGES)\n");
            printf("[4] ACE PLAYER DATA (PLAYER STATS)\n");
            printf("[5] TOURNAMENT SCOREBOOK (HISTORY)\n");
            printf("[6] MVP RACE CANDIDATES\n");
            printf("[7] RIVALRY CHECK (HEAD-TO-HEAD)\n");
            printf("[8] VIEW BOX SCORE\n");
            printf("[9] LEAVE THE COURT (EXIT)\n");
            
            printf("Enter choice: ");
            
            scanf("%d", &choice);

            switch (choice) 
            {
                case 0:
                    //set all data to 0 (Reset all data)
                    resetData(teams, gameRecords, &gameCount);
                    break;
                case 1:
                    simulateGame(teams, gameRecords, &gameCount, boxScorePlayers);
                    break;
                case 2:
                    standings(teams);
                    break;
                case 3:
                    averages(teams);
                    break;
                case 4:
                    acePlayerData(teams);
                    break;
                case 5:
                    scorebook(gameRecords, gameCount);
                    break;
                case 6:
                    mvpRace(teams);
                    break;
                case 7:
                    head2head(teams, gameRecords, gameCount);
                    break;  
                case 8:
                    // View Box Score (This will read the latest game from box_scores.txt and display it)
                    viewBoxScore(gameRecords, gameCount);
                    break;
                case 9:
                    // Exit the program
                    printf("Exiting the program. Goodbye!\n");
                    break;
                default:
                    printf("Invalid choice. Please try again.\n");
        
                    break;
            }

        }
        return 0;
    }

    // Phase 2: File Handling & Data Persistence
    //  The system must maintain two separate text files to ensure data is not lost when the program closes:
    //          ● league_data.txt (The Database): * Loading: At startup, read the team names, win/loss records, and all player attributes.
    //              o Saving: Every time a game concludes, overwrite this file with the updated cumulative statistics and averages.
    //          ● history.txt (The Scorebook): * Logging: Append a new line for every completed game, including
    //            a unique incrementing Game ID.
    //            o Synchronization: Implement a function that reads the entire history file at startup to
    //              recalculate each team's win/loss records, ensuring the standings match the recorded game
    //              results.
    //          ● box_scores.txt (New Feature): A detailed archive of individual player performances for every
    //            specific game. Every time a game is simulated, the stats for all 10 players on the court must be
    //            appended to this file, linked by the unique Game ID.

    void loadLeagueData(struct Team teams[4], int *gameCount)
    {
        FILE *fp = fopen("league_data.txt", "r");
        if (fp != NULL)
        {
            for (int i = 0; i < 4; i++)
            {
                // Read Team Name, Wins, Losses
                if (fscanf(fp, "%s %d %d", teams[i].teamName, &teams[i].wins, &teams[i].losses) != 3) break;
                
                for (int j = 0; j < 5; j++)
                {
                    // ADDED &teams[i].MainPlayers[j].jerseyNum to match your file
                    fscanf(fp, "%s %d %d %d %d %f %f %f %d", 
                        teams[i].MainPlayers[j].lastName,  
                        &teams[i].MainPlayers[j].jerseyNum,     // This was missing!
                        &teams[i].MainPlayers[j].totalPoints,
                        &teams[i].MainPlayers[j].totalRebounds,
                        &teams[i].MainPlayers[j].totalAssists,
                        &teams[i].MainPlayers[j].avgPoints,
                        &teams[i].MainPlayers[j].avgRebounds,
                        &teams[i].MainPlayers[j].avgAssists,
                        &teams[i].MainPlayers[j].gamesPlayed);
                }
            }
            fclose(fp);
            printf("Data loaded successfully!\n");
        }
        else
        {
            printf("File not found! Starting with empty records.\n");
        }
    }

    void saveAlltoFile (struct Team teams[4])
    {
        FILE *fp;
        fp = fopen("league_data.txt", "w");

        if (fp != NULL)
        {
            for (int i = 0; i < 4; i++) 
            {
                // Save Team Header: PRX 1 2
                fprintf(fp, "%s %d %d\n", teams[i].teamName, teams[i].wins, teams[i].losses);
                
                for (int j = 0; j < 5; j++) 
                {
                    // Save Player Data matching your file format
                    fprintf(fp, "%s %d %d %d %d %.6f %.6f %.6f %d\n", 
                            teams[i].MainPlayers[j].lastName, 
                            teams[i].MainPlayers[j].jerseyNum, // Added this
                            teams[i].MainPlayers[j].totalPoints, 
                            teams[i].MainPlayers[j].totalRebounds,
                            teams[i].MainPlayers[j].totalAssists,
                            teams[i].MainPlayers[j].avgPoints, 
                            teams[i].MainPlayers[j].avgRebounds, 
                            teams[i].MainPlayers[j].avgAssists, 
                            teams[i].MainPlayers[j].gamesPlayed);
                }
            }
            fclose(fp);
            printf("League data saved successfully!\n");
        }
        else
        {
            printf("Error: Could not open file for saving.\n");
        }
    }

    void updateGameHistory(int HomeTeamID, int AwayTeamID, int HomeScore, int AwayScore, int *gameCount)
    {
        FILE *fp;
        fp = fopen("history.txt", "a");
        if (fp != NULL)
        {
            fprintf(fp, "%d %s %s %d %d\n", *gameCount, teamNames[HomeTeamID-1], teamNames[AwayTeamID-1], HomeScore, AwayScore);
        }
        else
        {
            printf("Unable to open history file!\n");
        }
        fclose(fp);
    }

    void updateBoxScores(int gameID, struct Team teams[], int homeID, int awayID, struct stats boxScorePlayers[])
    {
        FILE *fp = fopen("box_scores.txt", "a"); // 'a' for append mode
        if (fp != NULL)
        {
            // Log Home Team Players (Indices 0-4 in boxScorePlayers)
            for (int i = 0; i < 5; i++)
            {
                fprintf(fp, "%d %s %s %d %d %d\n", 
                        gameID, 
                        teams[homeID-1].teamName, 
                        teams[homeID-1].MainPlayers[i].lastName, 
                        boxScorePlayers[i].points, 
                        boxScorePlayers[i].rebounds, 
                        boxScorePlayers[i].assists);
            }

            // Log Away Team Players (Indices 5-9 in boxScorePlayers)
            for (int i = 5; i < 10; i++)
            {
                fprintf(fp, "%d %s %s %d %d %d\n", 
                        gameID, 
                        teams[awayID-1].teamName, 
                        teams[awayID-1].MainPlayers[i-5].lastName, 
                        boxScorePlayers[i].points, 
                        boxScorePlayers[i].rebounds, 
                        boxScorePlayers[i].assists);
            }
            fclose(fp);
        }
        else
        {
            printf("Error: Could not open box_scores.txt for appending.\n");
        }
    }
    void resetData(struct Team teams[4], struct GameRecord gameRecords[100], int *gameCount)
    {
        char confirm;
        printf("WARNING: This will wipe all stats and history. Continue? (Y/N): ");
        scanf(" %c", &confirm);

        if (confirm == 'Y' || confirm == 'y') {
            *gameCount = 0;

            // 1. Manually define the starting names to ensure the file isn't "empty"
            char *defaultTeams[4] = {"Paper_Rex", "G2", "NRG", "Team_Liquid"};
            char *defaultPlayers[4][5] = {
                {"F0rsakeN", "Jinggg", "D4v41", "Something", "Invy"},
                {"Leaf", "Valyn", "Trent", "Jawgemo", "Babybay"},
                {"Ethan", "Mada", "Brawk", "Skuba", "Keiko"},
                {"NAts", "Kamo", "Purp0", "Wayne", "MiniBoo"}
            };
            int jerseyNumbers[4][5] = {
                {4, 7, 10, 11, 14}, {4, 5, 6, 10, 9}, 
                {4, 7, 13, 6, 5}, {4, 5, 6, 7, 8}
            };

            // 2. Clear internal memory
            for (int i = 0; i < 4; i++) {
                strcpy(teams[i].teamName, defaultTeams[i]);
                teams[i].wins = 0;
                teams[i].losses = 0;
                for (int j = 0; j < 5; j++) {
                    strcpy(teams[i].MainPlayers[j].lastName, defaultPlayers[i][j]);
                    teams[i].MainPlayers[j].jerseyNum = jerseyNumbers[i][j];
                    teams[i].MainPlayers[j].totalPoints = 0;
                    teams[i].MainPlayers[j].totalRebounds = 0;
                    teams[i].MainPlayers[j].totalAssists = 0;
                    teams[i].MainPlayers[j].avgPoints = 0.0;
                    teams[i].MainPlayers[j].avgRebounds = 0.0;
                    teams[i].MainPlayers[j].avgAssists = 0.0;
                    teams[i].MainPlayers[j].gamesPlayed = 0;
                }
            }

            // 3. Save this "Clean Slate" to the file immediately
            saveAlltoFile(teams);

            // 4. Wipe history files (these CAN be empty)
            FILE *fpHist = fopen("history.txt", "w");
            if (fpHist) fclose(fpHist);
            
            FILE *fpBox = fopen("box_scores.txt", "w");
            if (fpBox) fclose(fpBox);

            printf("\nLeague has been reset to factory defaults.\n");
        }
        system("pause");
    }

void simulateGame(struct Team teams[], struct GameRecord gameRecords[], int *gameCount, struct stats boxScorePlayers[])
{
    int homeTeamId, awayTeamId, selectedMode;
    int hometeamScore = 0, awayteamScore = 0;

    system("cls");
    printf("=========================================\n");
    printf(">>>           SIMULATING GAME         <<<\n");
    printf("=========================================\n\n");
    printf(">>> TIP OFF | SELECT THE TEAMS PLAYING <<<\n");

    // Display menu using global arrays
    for (int i = 0; i < 4; i++) {
        printf(" [%d] %s%s%s\n", i + 1, teamColors[i], teamNames[i], COLOR_RESET);
    }

    printf("\nHOME TEAM: ");
    scanf("%d", &homeTeamId);
    printf("AWAY TEAM: ");
    scanf("%d", &awayTeamId);

    // Validation
    if (awayTeamId < 1 || awayTeamId > 4 || homeTeamId < 1 || homeTeamId > 4 || awayTeamId == homeTeamId)
    {
        printf(">>> Invalid team selection. Please try again <<<\n");
        system("pause");
        return;
    }

    // Set Indices for easier access (Converting 1-4 input to 0-3 index)
    int hIdx = homeTeamId - 1;
    int aIdx = awayTeamId - 1;

    printf("\n>>>             CHOOSE MODE             <<<\n");
    printf(" [1] MANUAL ENTRY MODE (Box Score)\n");
    printf(" [2] QUICK SIM MODE (Randomized)\n");
    printf("Enter choice: ");
    scanf("%d", &selectedMode);

    if (selectedMode == 1) {    
        // Manual Entry Mode
        system("cls");
        printf("=========================================\n");
        printf(">>>          MANUAL ENTRY MODE        <<<\n");
        printf("=========================================\n\n");
        printf(">>> THE WHISTLE BLOWS: %s%s%s VS %s%s%s <<<\n\n", 
                teamColors[hIdx], teamNames[hIdx], COLOR_RESET, 
                teamColors[aIdx], teamNames[aIdx], COLOR_RESET);

        // INPUT HOME TEAM STATS
        printf(">>> ENTER %s%s%s PLAYER STATS:\n", teamColors[hIdx], teamNames[hIdx], COLOR_RESET);
        for (int i = 0; i < 5; i++) {
            printf("%s%s%s (Pts Reb Ast): ", teamColors[hIdx], teams[hIdx].MainPlayers[i].lastName, COLOR_RESET);
            scanf("%d %d %d", &boxScorePlayers[i].points, &boxScorePlayers[i].rebounds, &boxScorePlayers[i].assists);
            hometeamScore += boxScorePlayers[i].points;
        }

        printf("\n"); // Space between teams

        // INPUT AWAY TEAM STATS
        printf(">>> ENTER %s%s%s PLAYER STATS:\n", teamColors[aIdx], teamNames[aIdx], COLOR_RESET);
        for (int i = 5; i < 10; i++) {
            printf("%s%s%s (Pts Reb Ast): ", teamColors[aIdx], teams[aIdx].MainPlayers[i-5].lastName, COLOR_RESET);
            scanf("%d %d %d", &boxScorePlayers[i].points, &boxScorePlayers[i].rebounds, &boxScorePlayers[i].assists);
            awayteamScore += boxScorePlayers[i].points;
        }
    }
    else if (selectedMode == 2)
    {
        // Quick Sim Mode
        system("cls");
        printf("=========================================\n");
        printf(">>>            QUICK SIM MODE         <<<\n");
        printf("=========================================\n\n");
        printf(">>> THE WHISTLE BLOWS: %s%s%s VS %s%s%s <<<\n", 
                teamColors[hIdx], teamNames[hIdx], COLOR_RESET, 
                teamColors[aIdx], teamNames[aIdx], COLOR_RESET);

        // Simulate stats for home team
        for (int i = 0; i < 5; i++) {   
            boxScorePlayers[i].points = (rand() % 31) + HOME_ADVANTAGE;
            boxScorePlayers[i].rebounds = rand() % 16;
            boxScorePlayers[i].assists = rand() % 11;
            hometeamScore += boxScorePlayers[i].points;
        }

        // Simulate stats for away team
        for (int i = 5; i < 10; i++) {
            boxScorePlayers[i].points = rand() % 31;
            boxScorePlayers[i].rebounds = rand() % 16;
            boxScorePlayers[i].assists = rand() % 11;
            awayteamScore += boxScorePlayers[i].points;
        }
    }
    else {
        printf("Invalid mode selected.\n");
        return;
    }

    // --- FINAL RESULTS SECTION ---
    printf("\n--- FINAL BUZZER: %d - %d ---\n", hometeamScore, awayteamScore);
    printf("FINAL SCORE: %s%s %d%s - %s%s %d%s\n", 
            teamColors[hIdx], teamNames[hIdx], hometeamScore, COLOR_RESET, 
            teamColors[aIdx], teamNames[aIdx], awayteamScore, COLOR_RESET);

    if (hometeamScore > awayteamScore)
    {
        printf("\n>>> %s%s VICTORY!%s\n", teamColors[hIdx], teamNames[hIdx], COLOR_RESET);
        teams[hIdx].wins++;
        teams[aIdx].losses++;
    }
    else if (awayteamScore > hometeamScore)
    {
        printf("\n>>> %s%s VICTORY!%s\n", teamColors[aIdx], teamNames[aIdx], COLOR_RESET);
        teams[aIdx].wins++;
        teams[hIdx].losses++;
    }
    else
    {
        printf("\n>>> IT'S A TIE!!! <<<\n");
    }

    // Update player totals in struct
    for (int i = 0; i < 5; i++) {
        teams[hIdx].MainPlayers[i].totalPoints += boxScorePlayers[i].points;
        teams[hIdx].MainPlayers[i].totalRebounds += boxScorePlayers[i].rebounds;
        teams[hIdx].MainPlayers[i].totalAssists += boxScorePlayers[i].assists;
        teams[hIdx].MainPlayers[i].gamesPlayed++;
    }
    for (int i = 5; i < 10; i++) {
        teams[aIdx].MainPlayers[i-5].totalPoints += boxScorePlayers[i].points;
        teams[aIdx].MainPlayers[i-5].totalRebounds += boxScorePlayers[i].rebounds;
        teams[aIdx].MainPlayers[i-5].totalAssists += boxScorePlayers[i].assists;
        teams[aIdx].MainPlayers[i-5].gamesPlayed++;
    }

    // Update game history and save files
    (*gameCount)++;
    updateGameHistory(homeTeamId, awayTeamId, hometeamScore, awayteamScore, gameCount);
    updateBoxScores(*gameCount, teams, homeTeamId, awayTeamId, boxScorePlayers);
    saveAlltoFile(teams);
    
    printf("\n");
    system("pause");
}

    void standings(struct Team teams[4]) 
    {
        struct Team sortedTeams[4];
        struct Team tempTeam;

        // 1. Copy original data to the temporary array
        for (int i = 0; i < 4; i++) 
        {
            sortedTeams[i] = teams[i];
        }

        // 2. Sort by wins (Descending)
        for (int i = 0; i < 4 - 1; i++) 
        {
            for (int j = 0; j < 4 - i - 1; j++) 
            {
                if (sortedTeams[j].wins < sortedTeams[j + 1].wins) 
                {
                    tempTeam = sortedTeams[j];
                    sortedTeams[j] = sortedTeams[j + 1];
                    sortedTeams[j + 1] = tempTeam;
                }
            }
        }
        
        system("cls");
        printf("=====================================================\n");
        printf(">>>           THE ROAD TO NATIONALS               <<<\n");
        printf("=====================================================\n");
        printf("| RANK | TEAM              | WINS | LOSS | WIN %%    |\n");
        printf("+------+-------------------+------+------+----------+\n");

        // 3. Loop through exactly 4 teams
        for (int i = 0; i < 4; i++) 
        {   
            // Calculate Win Percentage inside the loop for each specific team
            int total = sortedTeams[i].wins + sortedTeams[i].losses;
            double WPercent = 0.0;
            
            if (total > 0) 
            {
                WPercent = ((double)sortedTeams[i].wins / total) * 100;
            }

            printf("| %-4d | %-17s | %-4d | %-4d | %-7.1f%% |\n", 
                i + 1, sortedTeams[i].teamName, sortedTeams[i].wins, sortedTeams[i].losses, WPercent);
        }
        printf("+------+-------------------+------+------+----------+\n");
        system("pause");
    }

    void averages(struct Team teams[4]) 
    {

        struct Team sortedTeams[4];
        int sortChoice;
        
        // 1. Copy original data to a temporary array for sorting
        for (int i = 0; i < 4; i++) {
            sortedTeams[i] = teams[i];
        }

        system("cls");
        printf("=========================================\n");
        printf(">>>       TEAM SCOUTING FILTERS       <<<\n");
        printf("=========================================\n");
        printf("[1] Sort by Average Points\n");
        printf("[2] Sort by Average Rebounds\n");
        printf("[3] Sort by Average Assists\n");
        printf("[4] Default (Alphabetical/ID)\n");
        printf("Enter choice: ");
        scanf("%d", &sortChoice);

        // 2. Sort based on user choice
        if (sortChoice == 1) 
        { // SORT BY POINTS
            for (int i = 0; i < 4 - 1; i++) {
                for (int j = 0; j < 4 - i - 1; j++) {
                    float avg1 = (sortedTeams[j].wins + sortedTeams[j].losses > 0) ? 
                        (float)(sortedTeams[j].MainPlayers[0].totalPoints + sortedTeams[j].MainPlayers[1].totalPoints + sortedTeams[j].MainPlayers[2].totalPoints + sortedTeams[j].MainPlayers[3].totalPoints + sortedTeams[j].MainPlayers[4].totalPoints) / (sortedTeams[j].wins + sortedTeams[j].losses) : 0;
                    float avg2 = (sortedTeams[j+1].wins + sortedTeams[j+1].losses > 0) ? 
                        (float)(sortedTeams[j+1].MainPlayers[0].totalPoints + sortedTeams[j+1].MainPlayers[1].totalPoints + sortedTeams[j+1].MainPlayers[2].totalPoints + sortedTeams[j+1].MainPlayers[3].totalPoints + sortedTeams[j+1].MainPlayers[4].totalPoints) / (sortedTeams[j+1].wins + sortedTeams[j+1].losses) : 0;
                    
                    if (avg1 < avg2) {
                        struct Team temp = sortedTeams[j];
                        sortedTeams[j] = sortedTeams[j + 1];
                        sortedTeams[j + 1] = temp;
                    }
                }
            }
        } 
        else if (sortChoice == 2) 
        { // SORT BY REBOUNDS
            for (int i = 0; i < 4 - 1; i++) {
                for (int j = 0; j < 4 - i - 1; j++) {
                    float avg1 = (sortedTeams[j].wins + sortedTeams[j].losses > 0) ? 
                        (float)(sortedTeams[j].MainPlayers[0].totalRebounds + sortedTeams[j].MainPlayers[1].totalRebounds + sortedTeams[j].MainPlayers[2].totalRebounds + sortedTeams[j].MainPlayers[3].totalRebounds + sortedTeams[j].MainPlayers[4].totalRebounds) / (sortedTeams[j].wins + sortedTeams[j].losses) : 0;
                    float avg2 = (sortedTeams[j+1].wins + sortedTeams[j+1].losses > 0) ? 
                        (float)(sortedTeams[j+1].MainPlayers[0].totalRebounds + sortedTeams[j+1].MainPlayers[1].totalRebounds + sortedTeams[j+1].MainPlayers[2].totalRebounds + sortedTeams[j+1].MainPlayers[3].totalRebounds + sortedTeams[j+1].MainPlayers[4].totalRebounds) / (sortedTeams[j+1].wins + sortedTeams[j+1].losses) : 0;
                    
                    if (avg1 < avg2) {
                        struct Team temp = sortedTeams[j];
                        sortedTeams[j] = sortedTeams[j + 1];
                        sortedTeams[j + 1] = temp;
                    }
                }
            }
        } 
        else if (sortChoice == 3) 
        { // SORT BY ASSISTS
            for (int i = 0; i < 4 - 1; i++) {
                for (int j = 0; j < 4 - i - 1; j++) {
                    float avg1 = (sortedTeams[j].wins + sortedTeams[j].losses > 0) ? 
                        (float)(sortedTeams[j].MainPlayers[0].totalAssists + sortedTeams[j].MainPlayers[1].totalAssists + sortedTeams[j].MainPlayers[2].totalAssists + sortedTeams[j].MainPlayers[3].totalAssists + sortedTeams[j].MainPlayers[4].totalAssists) / (sortedTeams[j].wins + sortedTeams[j].losses) : 0;
                    float avg2 = (sortedTeams[j+1].wins + sortedTeams[j+1].losses > 0) ? 
                        (float)(sortedTeams[j+1].MainPlayers[0].totalAssists + sortedTeams[j+1].MainPlayers[1].totalAssists + sortedTeams[j+1].MainPlayers[2].totalAssists + sortedTeams[j+1].MainPlayers[3].totalAssists + sortedTeams[j+1].MainPlayers[4].totalAssists) / (sortedTeams[j+1].wins + sortedTeams[j+1].losses) : 0;
                    
                    if (avg1 < avg2) {
                        struct Team temp = sortedTeams[j];
                        sortedTeams[j] = sortedTeams[j + 1];
                        sortedTeams[j + 1] = temp;
                    }
                }
            }
        }
        
        // 3. Display the sorted teams with their average stats
        // 3. Display the Table
        system("cls");
        printf("=================================================\n");
        printf("               TEAM SCOUTING REPORT              \n");
        printf("=================================================\n");
        printf("+-----------------+---------+---------+---------+\n");
        printf("| TEAM            | AVG PTS | AVG REB | AVG AST |\n");
        printf("+-----------------+---------+---------+---------+\n");

        for (int i = 0; i < 4; i++) {
            int games = sortedTeams[i].wins + sortedTeams[i].losses;
            float aP = 0, aR = 0, aA = 0;
            if (games > 0) {
                int tP = 0, tR = 0, tA = 0;
                for(int k=0; k<5; k++) {
                    tP += sortedTeams[i].MainPlayers[k].totalPoints;
                    tR += sortedTeams[i].MainPlayers[k].totalRebounds;
                    tA += sortedTeams[i].MainPlayers[k].totalAssists;
                }
                aP = (float)tP / games;
                aR = (float)tR / games;
                aA = (float)tA / games;
            }
            printf("| %-15s | %7.2f | %7.2f | %7.2f |\n", 
                sortedTeams[i].teamName, aP, aR, aA);
        }
        printf("+-----------------+---------+---------+---------+\n");
        system("pause");
        }
        

    void acePlayerData(struct Team teams[4]) 
    {
        // Arrays to hold pointers to the existing data
        struct Player *playerPtrs[20];
        char *playerTeamNames[20]; 
        int count = 0;
        int choice;

        // 1. Map pointers to existing data
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 5; j++) {
                playerPtrs[count] = &teams[i].MainPlayers[j];
                playerTeamNames[count] = teams[i].teamName;
                count++;
            }
        }

        system("cls");
        printf("==============================================\n");
        printf("           ACE PLAYER DATA FILTERS\n");
        printf("==============================================\n");
        printf("[1] Top 5 - Average Points\n");
        printf("[2] Top 5 - Average Rebounds\n");
        printf("[3] Top 5 - Average Assists\n");
        printf("[4] All Players - Alphabetical\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        // 2. Sorting Logic
        for (int i = 0; i < 20 - 1; i++) {
            for (int j = 0; j < 20 - i - 1; j++) {
                int swap = 0;
                
                // Calculate averages on the fly for accurate sorting
                float gp1 = (playerPtrs[j]->gamesPlayed > 0) ? (float)playerPtrs[j]->gamesPlayed : 1.0;
                float gp2 = (playerPtrs[j+1]->gamesPlayed > 0) ? (float)playerPtrs[j+1]->gamesPlayed : 1.0;

                if (choice == 1) {
                    if (((float)playerPtrs[j]->totalPoints / gp1) < ((float)playerPtrs[j+1]->totalPoints / gp2)) swap = 1;
                } else if (choice == 2) {
                    if (((float)playerPtrs[j]->totalRebounds / gp1) < ((float)playerPtrs[j+1]->totalRebounds / gp2)) swap = 1;
                } else if (choice == 3) {
                    if (((float)playerPtrs[j]->totalAssists / gp1) < ((float)playerPtrs[j+1]->totalAssists / gp2)) swap = 1;
                } else if (choice == 4) {
                    // FIXED: Alphabetical sort using strcmp on the Player lastName
                    if (strcmp(playerPtrs[j]->lastName, playerPtrs[j+1]->lastName) > 0) swap = 1;
                }

                if (swap) {
                    struct Player *tempPtr = playerPtrs[j];
                    playerPtrs[j] = playerPtrs[j+1];
                    playerPtrs[j+1] = tempPtr;

                    char *tempTeam = playerTeamNames[j];
                    playerTeamNames[j] = playerTeamNames[j+1];
                    playerTeamNames[j+1] = tempTeam;
                }
            }
        }

        // 3. Display with expanded spacing for long names
        system("cls");
        printf("===============================================================\n");
        printf("                        ACE PLAYER DATA\n");
        printf("===============================================================\n");
        printf("+-----------------+--------------+-------+-------+-------+\n");
        printf("| PLAYER          | TEAM         | PTS   | REB   | AST   |\n");
        printf("+-----------------+--------------+-------+-------+-------+\n");

        int limit = (choice == 4) ? 20 : 5;

        for (int i = 0; i < limit; i++) {
            float gp = (playerPtrs[i]->gamesPlayed > 0) ? (float)playerPtrs[i]->gamesPlayed : 1.0;
            
            // %-15s and %-12s provide the extra padding needed for long names
            printf("| %-15s | %-12s | %-5.1f | %-5.1f | %-5.1f |\n",
                playerPtrs[i]->lastName, 
                playerTeamNames[i], 
                (float)playerPtrs[i]->totalPoints / gp,
                (float)playerPtrs[i]->totalRebounds / gp,
                (float)playerPtrs[i]->totalAssists / gp);
            printf("+-----------------+--------------+-------+-------+-------+\n");
        }
        
        system("pause");
    }
void scorebook(struct GameRecord gameRecords[100], int gameCount) {
    FILE *fp = fopen("history.txt", "r");
    
    system("cls");
    printf("=========================================================================\n");
    printf("                              GAME HISTORY                               \n");
    printf("=========================================================================\n");
    printf("+------+-----------------+-----------------+-------------+-----------------+\n");
    // Increased widths to 15 to handle "Paper Rex" and "Team Liquid"
    printf("| ID   | HOME            | AWAY            | FINAL SCORE | WINNER          |\n");
    printf("+------+-----------------+-----------------+-------------+-----------------+\n");

    if (fp == NULL) {
        printf("|                        NO GAMES RECORDED YET                          |\n");
        printf("+------+-----------------+-----------------+-------------+-----------------+\n");
    } else {
        int id, hScore, aScore;
        char hTeam[50], aTeam[50], winner[50];

        // Note: This assumes names are saved with underscores or 
        // that you've updated the save format to handle spaces.
        while (fscanf(fp, "%d %s %s %d %d", &id, hTeam, aTeam, &hScore, &aScore) == 5) {
            
            if (hScore > aScore) {
                strcpy(winner, hTeam);
            } else if (aScore > hScore) {
                strcpy(winner, aTeam);
            } else {
                strcpy(winner, "TIE");
            }

            // %-15s provides left-aligned padding for longer team names
            printf("| %-4d | %-15s | %-15s |   %3d - %-3d | %-15s |\n", 
                   id, hTeam, aTeam, hScore, aScore, winner);
            printf("+------+-----------------+-----------------+-------------+-----------------+\n");
        }
        fclose(fp);
    }
    
    printf("\n");
    system("pause");
}

    void mvpRace(struct Team teams[4]) {
    // Parallel arrays to hold the "flattened" league data
    char pNames[20][50];
    char pTeams[20][50];
    float pSP[20]; // To hold the calculated Stat Points

    int count = 0;

    // 1. Calculate SP for every player in the league
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 5; j++) {
            struct Player *p = &teams[i].MainPlayers[j];
            
            // Get Games Played (handle division by zero if no games played yet)
            float gp = (p->gamesPlayed > 0) ? (float)p->gamesPlayed : 1.0;
            
            // Calculate Averages
            float avgPts = (float)p->totalPoints / gp;
            float avgReb = (float)p->totalRebounds / gp;
            float avgAst = (float)p->totalAssists / gp;

            // Apply the SP Formula: 10*AvgPts + 5*AvgReb + 3*AvgAst
            float sp = (10.0 * avgPts) + (5.0 * avgReb) + (3.0 * avgAst);

            // Store in parallel arrays
            strcpy(pNames[count], p->lastName);
            strcpy(pTeams[count], teams[i].teamName);
            pSP[count] = sp;
            
            count++;
        }
    }

    // 2. Sort players based on SP (Descending - Highest to Lowest)
    for (int i = 0; i < 20 - 1; i++) {
        for (int j = 0; j < 20 - i - 1; j++) {
            if (pSP[j] < pSP[j + 1]) {
                // Swap SP
                float tempSP = pSP[j];
                pSP[j] = pSP[j + 1];
                pSP[j + 1] = tempSP;

                // Swap Name
                char tempName[50];
                strcpy(tempName, pNames[j]);
                strcpy(pNames[j], pNames[j + 1]);
                strcpy(pNames[j + 1], tempName);

                // Swap Team
                char tempTeam[50];
                strcpy(tempTeam, pTeams[j]);
                strcpy(pTeams[j], pTeams[j + 1]);
                strcpy(pTeams[j + 1], tempTeam);
            }
        }
    }

    // 3. Display the Top 5 Contenders
    system("cls");
    printf("========================================================\n");
    printf("                        MVP RACE\n");
    printf("========================================================\n");
    // Adjusted widths: Name (15), Team (12) to accommodate Team_Liquid/Paper_Rex
    printf("+------+-----------------+--------------+------------------+\n");
    printf("| RANK | PLAYER          | TEAM         | STAT POINTS (SP) |\n");
    printf("+------+-----------------+--------------+------------------+\n");

    for (int i = 0; i < 5; i++) {
        printf("| %-4d | %-15s | %-12s | %-16.2f |\n", 
               i + 1, pNames[i], pTeams[i], pSP[i]);
        printf("+------+-----------------+--------------+------------------+\n");
    }

    system("pause");
}   

void head2head(struct Team teams[4]) 
{
    int team1Idx, team2Idx;
    int t1Wins = 0, t2Wins = 0;
    int foundMatch = 0;

    system("cls");
    printf("*** SELECT TEAMS FOR H2H ANALYSIS ***\n");
    for (int i = 0; i < 4; i++) {
        printf("  [%d] %s\n", i, teams[i].teamName);
    }

    printf("\nSelect First Team Index: ");
    scanf("%d", &team1Idx);
    printf("Select Second Team Index: ");
    scanf("%d", &team2Idx);

    // Validation to ensure they don't pick the same team
    if (team1Idx == team2Idx || team1Idx < 0 || team1Idx > 3 || team2Idx < 0 || team2Idx > 3) {
        printf("\nInvalid selection. Please choose two different valid teams.\n");
        system("pause");
        return;
    }

    FILE *fp = fopen("history.txt", "r");
    if (fp == NULL) {
        printf("\nNo tournament history found.\n");
    } else {
        printf("\n       --- %s VS %s HISTORY ---\n", teams[team1Idx].teamName, teams[team2Idx].teamName);
        printf("+----+-----------------+-----------------+-------------+\n");
        printf("| ID | HOME            | AWAY            | SCORE       |\n");
        printf("+----+-----------------+-----------------+-------------+\n");

        int id, hScore, aScore;
        char hTeam[50], aTeam[50];

        // Search history.txt for matchups between Team A and Team B
        while (fscanf(fp, "%d %s %s %d %d", &id, hTeam, aTeam, &hScore, &aScore) == 5) {
            
            // Check if team1 and team2 are the participants in this specific game
            int isMatch = (strcmp(hTeam, teams[team1Idx].teamName) == 0 && strcmp(aTeam, teams[team2Idx].teamName) == 0) ||
                          (strcmp(hTeam, teams[team2Idx].teamName) == 0 && strcmp(aTeam, teams[team1Idx].teamName) == 0);

            if (isMatch) {
                foundMatch = 1;
                printf("| %-2d | %-15s | %-15s |   %3d - %-3d   |\n", id, hTeam, aTeam, hScore, aScore);

                // Tally the wins
                if (hScore > aScore) {
                    if (strcmp(hTeam, teams[team1Idx].teamName) == 0) t1Wins++; else t2Wins++;
                } else if (aScore > hScore) {
                    if (strcmp(aTeam, teams[team1Idx].teamName) == 0) t1Wins++; else t2Wins++;
                }
            }
        }
        fclose(fp);

        if (!foundMatch) {
            printf("|             NO MATCHES FOUND BETWEEN THESE TEAMS             |\n");
        }
        printf("+----+-----------------+-----------------+-------------+\n");

        printf("\nSUMMARY:\n");
        printf(" %-15s Wins: %d\n", teams[team1Idx].teamName, t1Wins);
        printf(" %-15s Wins: %d\n", teams[team2Idx].teamName, t2Wins);
    }

    printf("\n");
    system("pause");
}

void viewBoxScore(struct GameRecord gameRecords[100], int gameCount) {
    int searchID;
    int found = 0;
    
    // 1. Show the history first so the user can see the IDs
    scorebook(gameRecords, gameCount);

    printf("\nEnter Game ID to view detailed Box Score: ");
    scanf("%d", &searchID);

    FILE *fp = fopen("box_scores.txt", "r");
    if (fp == NULL) {
        printf("Error: Could not open box_scores.txt\n");
        system("pause");
        return;
    }

    system("cls");
    
    int gID, pts, reb, ast;
    char tName[50], pName[50];
    char team1[50] = "", team2[50] = "";
    int t1Pts = 0, t1Reb = 0, t1Ast = 0;
    int t2Pts = 0, t2Reb = 0, t2Ast = 0;
    int t1TotalPts = 0, t1TotalReb = 0, t1TotalAst = 0;
    int t2TotalPts = 0, t2TotalReb = 0, t2TotalAst = 0;

    printf("--- BOX SCORE: GAME #%d ---\n", searchID);

    // 2. First pass: Find the two team names and their total scores for the header
    while (fscanf(fp, "%d %s %s %d %d %d", &gID, tName, pName, &pts, &reb, &ast) == 6) {
        if (gID == searchID) {
            if (strlen(team1) == 0) {
                strcpy(team1, tName);
            } else if (strlen(team2) == 0 && strcmp(tName, team1) != 0) {
                strcpy(team2, tName);
            }

            if (strcmp(tName, team1) == 0) {
                t1TotalPts += pts;
            } else {
                t2TotalPts += pts;
            }
            found = 1;
        }
    }

    if (!found) {
        printf("No records found for Game ID %d.\n", searchID);
        fclose(fp);
        system("pause");
        return;
    }

    printf("%s (%d) vs %s (%d)\n\n", team1, t1TotalPts, team2, t2TotalPts);

    // 3. Second pass: Print Team 1 Table
    rewind(fp);
    printf("+-----------------+-----+-----+-----+\n");
    printf("| %-15s | PTS | REB | AST |\n", team1);
    printf("+-----------------+-----+-----+-----+\n");
    while (fscanf(fp, "%d %s %s %d %d %d", &gID, tName, pName, &pts, &reb, &ast) == 6) {
        if (gID == searchID && strcmp(tName, team1) == 0) {
            printf("| %-15s | %-3d | %-3d | %-3d |\n", pName, pts, reb, ast);
            t1TotalReb += reb;
            t1TotalAst += ast;
        }
    }
    printf("+-----------------+-----+-----+-----+\n");
    printf("| TEAM TOTALS     | %-3d | %-3d | %-3d |\n", t1TotalPts, t1TotalReb, t1TotalAst);
    printf("+-----------------+-----+-----+-----+\n\n");

    // 4. Third pass: Print Team 2 Table
    rewind(fp);
    printf("+-----------------+-----+-----+-----+\n");
    printf("| %-15s | PTS | REB | AST |\n", team2);
    printf("+-----------------+-----+-----+-----+\n");
    while (fscanf(fp, "%d %s %s %d %d %d", &gID, tName, pName, &pts, &reb, &ast) == 6) {
        if (gID == searchID && strcmp(tName, team2) == 0) {
            printf("| %-15s | %-3d | %-3d | %-3d |\n", pName, pts, reb, ast);
            t2TotalReb += reb;
            t2TotalAst += ast;
        }
    }
    printf("+-----------------+-----+-----+-----+\n");
    printf("| TEAM TOTALS     | %-3d | %-3d | %-3d |\n", t2TotalPts, t2TotalReb, t2TotalAst);
    printf("+-----------------+-----+-----+-----+\n");

    fclose(fp);
    printf("\n");
    system("pause");
}
