#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

int main() {
    FILE* file;
    char line[200];
    int id;
    char name[100];
    char programme[100];
    float marks;
    char command[50]; //query
    int searchId, found; //query

    // Display declaration at startup
    printf("Declaration\n");
    printf("SIT's policy on copying does not allow the students to copy source code as well as assessment solutions\n");
    printf("from another person AI or other places. It is the students' responsibility to guarantee that their\n");
    printf("assessment solutions are their own work. Meanwhile, the students must also ensure that their work is\n");
    printf("not accessible by others. Where such plagiarism is detected, both of the assessments involved will\n");
    printf("receive ZERO mark.\n\n");

    printf("We hereby declare that:\n");
    printf("- We fully understand and agree to the abovementioned plagiarism policy.\n");
    printf("- We did not copy any code from others or from other places.\n");
    printf("- We did not share our codes with others or upload to any other places for public access and will\n");
    printf("  not do that in the future.\n");
    printf("- We agree that our project will receive Zero mark if there is any plagiarism detected.\n");
    printf("- We agree that we will not disclose any information or material of the group project to others or\n");
    printf("  upload to any other places for public access.\n");
    printf("- We agree that we did not copy any code directly from AI generated sources\n\n");

    printf("Declared by: P7_4\n");
    printf("Team members:\n");
    printf("1. Thiha (Kevin)\n");
    printf("2. Hermann Phua\n");
    printf("3. Sharlene Teo\n");
    printf("4. Glenda Teo\n");
    printf("5. Oh Rui Cheng\n\n");

    printf("Date: 25 November 2024\n\n");

    printf("=== Class Management System (CMS) ===\n");
    printf("Available commands: OPEN, SHOW ALL, QUERY, EXIT\n\n");

    while (1) {
        printf("P7_4: ");
        scanf("%s", command);

        if (strcmp(command, "OPEN") == 0) {
            file = fopen("Team_P7_4-CMS.txt", "r");
            if (file == NULL) {
                printf("CMS: Error: Cannot open database file\n");
                continue;
            }
            printf("CMS: The database file \"Team_P7_4-CMS.txt\" is successfully opened.\n");
            fclose(file);
        }
        else if (strcmp(command, "SHOW") == 0) {
            char next[10];
            scanf("%s", next);
            if (strcmp(next, "ALL") == 0) {
                file = fopen("Team_P7_4-CMS.txt", "r");
                if (file == NULL) {
                    printf("CMS: Error: Cannot open database file\n");
                    continue;
                }

                printf("CMS: Here are all the records found in the table \"StudentRecords\".\n");
                printf("ID\t\tName\t\tProgramme\t\tMark\n");
                printf("------------------------------------------------------------\n");

                while (fgets(line, sizeof(line), file) != NULL) {
                    printf("%s", line);
                }
                fclose(file);
            }
        }
        else if (strcmp(command, "QUERY") == 0) {
            char idStr[20];
            scanf("%s", idStr);

            // Extract ID from "ID=2301234" format
            if (sscanf(idStr, "ID=%d", &searchId) == 1) {
                file = fopen("Team_P7_4-CMS.txt", "r");
                if (file == NULL) {
                    printf("CMS: Error: Cannot open database file\n");
                    continue;
                }

                found = 0;
                // Skip header line if exists
                fgets(line, sizeof(line), file);

                while (fgets(line, sizeof(line), file) != NULL) {
                    // Parse each line to check if ID matches
                    int currentId;
                    char currentName[100], currentProgramme[100];
                    float currentMarks;

                    if (sscanf(line, "%d %99[^\t] %99[^\t] %f",
                        &currentId, currentName, currentProgramme, &currentMarks) == 4) {
                        if (currentId == searchId) {
                            printf("CMS: The record with ID=%d is found in the data table.\n", searchId);
                            printf("ID\t\tName\t\tProgramme\t\tMark\n");
                            printf("------------------------------------------------------------\n");
                            printf("%d\t\t%s\t\t%s\t\t%.1f\n", currentId, currentName, currentProgramme, currentMarks);
                            found = 1;
                            break;
                        }
                    }
                }

                if (!found) {
                    printf("CMS: The record with ID=%d does not exist.\n", searchId);
                }

                fclose(file);
            }
            else {
                printf("CMS: Invalid query format. Use: QUERY ID=2301234\n");
            }
        }
        else if (strcmp(command, "EXIT") == 0) {
            printf("CMS: Goodbye!\n");
            break;
        }
        else {
            printf("CMS: Unknown command. Available commands: OPEN, SHOW ALL, QUERY, EXIT\n");
            // Clear any remaining input
            while (getchar() != '\n');
        }
    }

    return 0;
}