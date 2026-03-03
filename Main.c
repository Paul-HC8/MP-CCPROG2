#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>

// Struct declaration for basketball player stats (points, rebounds, assists)
struct Stats{
    int points;
    int rebounds;
    int assists;
};


// Struct declaration for basketball player stats AVERAGE (points, rebounds, assists)
struct Average{
    float pointsave;
    float reboundsave;
    float assistsave;
};


// struct declaration for basketball player information (lastname, jersey number, games played)
struct Player{
    char lastname[50];
    int jerseynumber;
    int gamesplayed;
}; 

// struct declaration for basketball team information (team name, array of 5 players)
struct Team{
    char teamname[50];
    struct Player players[5];
};

//Game record struct declaration (Match History, Game ID, Both names of the Team, final scores)
struct GameRecord{
    // match history
    int gameID;
    char team1[50];
    char team2[50];
    int finalscore1;

};

int main()
{
    int choice;

    // Prints out the Menu/Interface for our Simulator
    while (choice != 4) 
    {
    	system("cls");
        printf("=========================================\n");
        printf("===  ---  VALORANT MASTERS DLSU  ---  ===\n");
        printf("=========================================\n");
        printf("[1] THE TIP-OFF (SIMULATE GAME)\n");
        printf("[2] THE ROAD TO NATIONALS (STANDINGS)\n");
        printf("[3] TEAM SCOUTING REPORT (AVERAGES)\n");
        printf("[4] ACE PLAYER DATA (PLAYER STATS)\n");
        printf("[5] TOURNAMENT SCOREBOOK (HISTORY)\n");
        printf("[6] MVP RACE CANDIDATES\n");
        printf("[7] RIVALRY CHECK (HEAD-TO-HEAD)\n");
        printf("[7] RIVALRY CHECK (HEAD-TO-HEAD)\n");
        printf("[8] VIEW BOX SCORE\n");
        printf("[9] LEAVE THE COURT (EXIT)\n");
        
        printf("Enter choice: ");
        
        scanf("%d", &choice);

        if (choice == 1) 
        {
            addUser(users, &userCount); 
        }
        else if (choice == 2) 
        {
            authenticate(users, userCount); 
        }
        else if (choice == 3) 
        {
            updateUser(users, userCount); 
        }
        system("pause");
    }
    return 0;
}
