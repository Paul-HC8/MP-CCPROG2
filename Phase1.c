#include <stdio.h>

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
    

    return 0;
}