#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Players_data.h"


typedef struct PlayerNode {
    int id;
    char name[60];
    char teamName[50];
    char role[20];
    int totalRuns;
    float batAvg;
    float strikeRate;
    int wickets;
    float econRate;
    float perfIndex;
    struct PlayerNode* next;
} PlayerNode;

typedef struct {
    int teamId;
    char teamName[50];
    int playerCount;
    float avgStrikeRate;
    PlayerNode* head;
} Team;
Team teamsList[10];   

float getPerformanceIndex(const char* role, float avg, float sr, int wk, float eco)
{
    if (strcmp(role, "Batsman") == 0) {
        return (avg * sr) / 100.0f;
    }
    else if (strcmp(role, "Bowler") == 0) {
        return (wk * 2) + (100.0f - eco);
    }
    else { 
        return (avg * sr) / 100.0f + (wk * 2);
    }
}

void addPlayerToLinkedList(Team* t, PlayerNode* newNode)
{
    newNode->next = t->head;
    t->head = newNode;
    t->playerCount++;
}

Team* findTeamById(int id)
{
    int low = 0;
    int high = 9;

    while (low <= high) {
        int mid = (low + high) / 2;

        if (teamsList[mid].teamId == id)
            return &teamsList[mid];
        else if (teamsList[mid].teamId < id)
            low = mid + 1;
        else
            high = mid - 1;
    }

    return NULL; 
}

void loadInitialData()
{
    for (int i = 0; i < teamCount; i++) {
        teamsList[i].teamId = i + 1;
        strcpy(teamsList[i].teamName, teams[i]);
        teamsList[i].playerCount = 0;
        teamsList[i].avgStrikeRate = 0.0f;
        teamsList[i].head = NULL;
    }

    for (int i = 0; i < playerCount; i++) {

        PlayerNode* p = (PlayerNode*)malloc(sizeof(PlayerNode));

        if (!p) {
            printf("Memory allocation failed\n");
            return;
        }

        p->id = players[i].id;
        strcpy(p->name, players[i].name);
        strcpy(p->teamName, players[i].team);
        strcpy(p->role, players[i].role);
        p->totalRuns = players[i].totalRuns;
        p->batAvg = players[i].battingAverage;
        p->strikeRate = players[i].strikeRate;
        p->wickets = players[i].wickets;
        p->econRate = players[i].economyRate;

        p->perfIndex = getPerformanceIndex(p->role, p->batAvg, p->strikeRate,
                                           p->wickets, p->econRate);

        p->next = NULL;

        for (int t = 0; t < 10; t++) {
            if (strcmp(teamsList[t].teamName, p->teamName) == 0) {
                addPlayerToLinkedList(&teamsList[t], p);
                break;
            }
        }
    }
}

void addNewPlayer()
{
    int teamId, roleOption;
    char newName[60], newRole[20];

    printf("Enter Team ID: ");
    scanf("%d", &teamId);

    Team* teamPtr = findTeamById(teamId);

    if (!teamPtr) {
        printf("Team not found!\n");
        return;
    }

    PlayerNode* p = (PlayerNode*)malloc(sizeof(PlayerNode));
    if (!p) {
        printf("Memory error\n");
        return;
    }

    printf("Player ID: ");
    scanf("%d", &p->id);

    printf("Name: ");
    getchar(); 
    fgets(newName, 60, stdin);
    newName[strcspn(newName, "\n")] = 0;
    strcpy(p->name, newName);

    printf("Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    scanf("%d", &roleOption);

    if (roleOption == 1) strcpy(newRole, "Batsman");
    else if (roleOption == 2) strcpy(newRole, "Bowler");
    else strcpy(newRole, "All-rounder");

    strcpy(p->role, newRole);
    strcpy(p->teamName, teamPtr->teamName);

    printf("Total Runs: ");
    scanf("%d", &p->totalRuns);

    printf("Batting Average: ");
    scanf("%f", &p->batAvg);

    printf("Strike Rate: ");
    scanf("%f", &p->strikeRate);

    printf("Wickets: ");
    scanf("%d", &p->wickets);

    printf("Economy Rate: ");
    scanf("%f", &p->econRate);

    p->perfIndex = getPerformanceIndex(p->role, p->batAvg, p->strikeRate,p->wickets, p->econRate);
    p->next = NULL;

    addPlayerToLinkedList(teamPtr, p);

    printf("Player added successfully to %s!\n", teamPtr->teamName);
}

void showTeamPlayers()
{
    int id;
    printf("Enter Team ID: ");
    scanf("%d", &id);

    Team* t = findTeamById(id);

    if (!t) {
        printf("No team found with that ID.\n");
        return;
    }

    printf("\nPlayers of %s:\n", t->teamName);
    printf("------------------------------------------------------------\n");
    printf("ID   Name                 Role        PI\n");
    printf("------------------------------------------------------------\n");

    float sumSR = 0;
    int srCount = 0;

    PlayerNode* temp = t->head;
    while (temp) {
        printf("%-4d %-20s %-12s %.2f\n",
               temp->id, temp->name, temp->role, temp->perfIndex);

        if (strcmp(temp->role, "Batsman") == 0 ||
            strcmp(temp->role, "All-rounder") == 0)
        {
            sumSR += temp->strikeRate;
            srCount++;
        }

        temp = temp->next;
    }

    if (srCount > 0)
        t->avgStrikeRate = sumSR / srCount;
    else
        t->avgStrikeRate = 0;

    printf("------------------------------------------------------------\n");
    printf("Total Players: %d\n", t->playerCount);
    printf("Avg Batting SR: %.2f\n", t->avgStrikeRate);
}

void sortTeamsByAvgSR()
{
    for (int i = 0; i < 10; i++) {
        float totalSR = 0;
        int count = 0;

        PlayerNode* p = teamsList[i].head;

        while (p) {
            if (strcmp(p->role, "Batsman") == 0 ||
                strcmp(p->role, "All-rounder") == 0)
            {
                totalSR += p->strikeRate;
                count++;
            }
            p = p->next;
        }

        teamsList[i].avgStrikeRate = (count > 0) ? totalSR / count : 0;
    }

    for (int i = 0; i < 10; i++) {
        for (int j = i + 1; j < 10; j++) {
            if (teamsList[i].avgStrikeRate < teamsList[j].avgStrikeRate) {
                Team temp = teamsList[i];
                teamsList[i] = teamsList[j];
                teamsList[j] = temp;
            }
        }
    }

    printf("\nTeams by Average Batting Strike Rate:\n");
    printf("----------------------------------------------\n");
    printf("ID   Team                Avg SR   Players\n");
    printf("----------------------------------------------\n");

    for (int i = 0; i < 10; i++) {
        printf("%-4d %-18s %-7.2f %-5d\n",
               teamsList[i].teamId, teamsList[i].teamName,
               teamsList[i].avgStrikeRate, teamsList[i].playerCount);
    }
}


void showTopKOfTeam()
{
    int tid, roleOpt, K;
    char chosenRole[20];

    printf("Enter Team ID: ");
    scanf("%d", &tid);

    Team* t = findTeamById(tid);
    if (!t) {
        printf("Team not found.\n");
        return;
    }

    printf("Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    scanf("%d", &roleOpt);

    if (roleOpt == 1) strcpy(chosenRole, "Batsman");
    else if (roleOpt == 2) strcpy(chosenRole, "Bowler");
    else strcpy(chosenRole, "All-rounder");

    printf("Enter K: ");
    scanf("%d", &K);

    PlayerNode* arr[200];
    int cnt = 0;

    PlayerNode* p = t->head;

    while (p) {
        if (strcmp(p->role, chosenRole) == 0) {
            arr[cnt++] = p;
        }
        p = p->next;
    }

    for (int i = 0; i < cnt; i++) {
        for (int j = i + 1; j < cnt; j++) {
            if (arr[i]->perfIndex < arr[j]->perfIndex) {
                PlayerNode* temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }

    printf("\nTop %d %s of %s:\n", K, chosenRole, t->teamName);
    printf("--------------------------------------------\n");

    for (int i = 0; i < K && i < cnt; i++) {
        printf("%d  %s  (PI: %.2f)\n",
               arr[i]->id, arr[i]->name, arr[i]->perfIndex);
    }
}

void showRoleAcrossTeams()
{
    int opt;
    char role[20];

    printf("Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    scanf("%d", &opt);

    if (opt == 1) strcpy(role, "Batsman");
    else if (opt == 2) strcpy(role, "Bowler");
    else strcpy(role, "All-rounder");

    PlayerNode* list[300];
    int count = 0;

    for (int i = 0; i < 10; i++) {
        PlayerNode* p = teamsList[i].head;

        while (p) {
            if (strcmp(p->role, role) == 0)
                list[count++] = p;

            p = p->next;
        }
    }

    for (int i = 0; i < count; i++) {
        for (int j = i + 1; j < count; j++) {
            if (list[i]->perfIndex < list[j]->perfIndex) {
                PlayerNode* tmp = list[i];
                list[i] = list[j];
                list[j] = tmp;
            }
        }
    }

    printf("\nPlayers of role: %s\n", role);
    printf("------------------------------------\n");

    for (int i = 0; i < count; i++) {
        printf("%-4d %-20s %-15s PI: %.2f\n",
               list[i]->id, list[i]->name, list[i]->teamName, list[i]->perfIndex);
    }
}

int main()
{
    loadInitialData();

    int choice;

    while (1) {
        printf("\n===== ICC ODI Player Performance Analyzer =====\n");
        printf("1. Add Player to Team\n");
        printf("2. Display Players of a Team\n");
        printf("3. Display Teams by Avg Strike Rate\n");
        printf("4. Display Top K Players of a Team\n");
        printf("5. Display All Players of a Role (All Teams)\n");
        printf("6. Exit\n");
        printf("------------------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1: addNewPlayer(); break;
            case 2: showTeamPlayers(); break;
            case 3: sortTeamsByAvgSR(); break;
            case 4: showTopKOfTeam(); break;
            case 5: showRoleAcrossTeams(); break;
            case 6: printf("Exiting...\n"); return 0;
            default: printf("Invalid option. Try again.\n");
        }
    }

    return 0;
}