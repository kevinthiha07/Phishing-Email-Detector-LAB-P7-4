#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

int checkDuplicateID(int newID);
void insertRecord();
void viewRecords();
void searchQuery();
void updateRecord(); // Prototype for the update function

int main() {
    int searchId, found;
    int choice;
    char line[200];

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

    printf("Date: 25 November 2025\n\n");

    printf("=== Class Management System (CMS) ===\n");

    while (1) {
        printf("\nStudent Management System\n");
        printf("1. Show All\n");
        printf("2. Insert\n");
        printf("3. Query\n");
        printf("4. Update\n");
        printf("5. Delete\n");
        printf("6. Exit\n\n");

        printf("Enter your choice (1-6): ");
        if (scanf("%d", &choice) != 1) {
            printf("CMS: Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
        case 1: // Show All
            viewRecords();
            break;

        case 2: // Insert
            insertRecord();
            break;

        case 3: // Query
            searchQuery();
            break;

        case 4: // Update
            updateRecord();
            break;

        case 5: // Delete
            printf("CMS: Delete function - To be implemented\n");
            break;

        case 6: // Exit
            printf("CMS: Goodbye!\n");
            return 0;

        default:
            printf("CMS: Invalid choice. Please enter a number between 1-6.\n");
            break;
        }
    }

    return 0;
}

void viewRecords() {
    FILE* file = fopen("Team_P7_4-CMS.txt", "r");
    if (file == NULL) {
        printf("CMS: Error: Cannot open database file\n");
        return;
    }

    printf("\nCMS: Here are all the records found in the table \"StudentRecords\".\n");
    printf("ID\t\tName\t\tProgramme\t\tMarks\n");
    printf("------------------------------------------------------------\n");

    char line[200];
    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line);
    }

    fclose(file);
}

int checkDuplicateID(int newID) {
    FILE* file = fopen("Team_P7_4-CMS.txt", "r");
    if (file == NULL) {
        return 0;
    }

    char line[200];
    int existingID;
    char tempName[100], tempProgramme[100];
    float tempMarks;

    while (fgets(line, sizeof(line), file) != NULL) {
        if (sscanf(line, "%d,%99[^,],%99[^,],%f", &existingID, tempName, tempProgramme, &tempMarks) >= 1) {
            if (existingID == newID) {
                fclose(file);
                return 1; // duplicated ID found
            }
        }
    }

    fclose(file);
    return 0; // no duplicate ID 
}

void insertRecord() {
    int newID;
    char name[100];
    char programme[100];
    float marks;
    char input[500];

    printf("\n--- Insert New Student Record ---\n");
    printf("Enter Student Data: ");

    while (getchar() != '\n');

    // reads the entire input line
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("CMS: Error reading input!\n");
        return;
    }

    input[strcspn(input, "\n")] = 0;


    // parsing inputs of fields with quotation marks
    if (sscanf(input, "ID=\"%d\", Name=\"%99[^\"]\", Programme=\"%99[^\"]\", Marks=\"%f\"",
        &newID, name, programme, &marks) != 4) {
        printf("CMS: Error: Invalid input format!\n");
        printf("CMS: Expected: ID=\"number\", Name=\"name\", Programme=\"programme\", Marks=\"marks\"\n");
        return;
    }

    // checking if the ID exists already
    if (checkDuplicateID(newID)) {
        printf("CMS: Error: Student with ID %d already exists! Insertion cancelled.\n", newID);
        return;
    }

    // opening file to append new record
    FILE* file = fopen("Team_P7_4-CMS.txt", "a");
    if (file == NULL) {
        printf("CMS: Error: Cannot open database file for writing\n");
        return;
    }

    // write new record to file
    fprintf(file, "%d,%s,%s,%.1f\n", newID, name, programme, marks);
    fclose(file);

    printf("CMS: A new record with ID=%d was successfully inserted.\n", newID);

    // show all records after insertion
    viewRecords();
}

void searchQuery() {
    int searchId;

    printf("Enter student ID to search: ");
    if (scanf("%d", &searchId) != 1) {  // read and check the student id input
        printf("CMS: Invalid ID format.\n");  // display error message when fail to read an int
        while (getchar() != '\n'); // clear input buffer to remove any remaining invalid characters
        return; // exit since we have invalid input
    }

    FILE* file = fopen("Team_P7_4-CMS.txt", "r"); // open database file in read mode
    if (file == NULL) { // check if file is opened successfully
        printf("CMS: Error: Cannot open database file\n");
        return;
    }

    char line[200];
    int found = 0; // initialize flag to track if student record is found

    while (fgets(line, sizeof(line), file) != NULL) { // read each line from the file
        int currentId;
        char currentName[100], currentProgramme[100];
        float currentMarks;

        if (sscanf(line, "%d,%99[^,],%99[^,],%f", // parse the line to extract student details
            &currentId, currentName, currentProgramme, &currentMarks) == 4) {
            if (currentId == searchId) {
                printf("CMS: The record with ID=%d is found in the data table.\n", searchId);
                printf("ID\t\tName\t\tProgramme\t\tMark\n");
                printf("------------------------------------------------------------\n");
                printf("%d\t\t%s\t\t%s\t\t%.1f\n", currentId, currentName, currentProgramme, currentMarks);
                found = 1; // set found flag to indicate successful search 
                break;
            }
        }
    }

    if (!found) {
        printf("CMS: The record with ID=%d does not exist.\n", searchId);
    }

    fclose(file);
}

// Function to update a record
void updateRecord() {
    int updateId, currentId;
    char name[100], programme[100];
    float marks;
    // Add variables to store original data
    char originalName[100], originalProgramme[100];
    float originalMarks;
    char line[200];
    int found = 0;

    /* // --- These variables were moved for C89 compatibility in the *other* version ---
    char confirmation;
    char marksInput[50];
    */

    // Open the original file for reading and a temp file for writing
    FILE* file_in = fopen("Team_P7_4-CMS.txt", "r");
    FILE* file_out = fopen("temp.txt", "w");

    if (file_in == NULL || file_out == NULL) {
        printf("CMS: Error: Cannot open database file(s).\n");
        if (file_in) fclose(file_in);
        if (file_out) fclose(file_out);
        return;
    }

    printf("Enter student ID to update: ");
    if (scanf("%d", &updateId) != 1) {
        printf("CMS: Invalid ID format.\n");
        while (getchar() != '\n'); // clear buffer
        fclose(file_in);
        fclose(file_out);
        remove("temp.txt"); // clean up temp file
        return;
    }
    while (getchar() != '\n'); // clear buffer after successful scanf

    // Read from original, write to temp
    while (fgets(line, sizeof(line), file_in) != NULL) {
        // Try to parse the ID and data from the current line
        // We use == 4 to ensure we're only checking valid data lines
        if (sscanf(line, "%d,%99[^,],%99[^,],%f", &currentId, originalName, originalProgramme, &originalMarks) == 4) {

            if (currentId == updateId) {
                found = 1;

                // --- 'confirmation' declared here (C99-style) ---
                char confirmation;

                // Record found, get new data from user
                printf("Record found. Enter new data for ID %d (or type 'cancel' to exit at any prompt):\n", updateId);

                printf("Enter new Name: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0; // remove trailing newline

                // --- START CANCEL CHECK ---
                if (strcmp(name, "cancel") == 0) {
                    fputs(line, file_out); // Write original line
                    printf("CMS: Update cancelled. Original record for ID=%d was kept.\n", updateId);
                    continue; // Skip to the next line in the file
                }
                // --- END CANCEL CHECK ---

                printf("Enter new Programme: ");
                fgets(programme, sizeof(programme), stdin);
                programme[strcspn(programme, "\n")] = 0; // remove trailing newline

                // --- START CANCEL CHECK ---
                if (strcmp(programme, "cancel") == 0) {
                    fputs(line, file_out); // Write original line
                    printf("CMS: Update cancelled. Original record for ID=%d was kept.\n", updateId);
                    continue; // Skip to the next line in the file
                }
                // --- END CANCEL CHECK ---

                // --- MODIFIED MARKS INPUT ---
                // --- 'marksInput' declared here (C99-style) ---
                char marksInput[50]; // Read marks as a string first
                while (1) {
                    printf("Enter new Marks: ");
                    fgets(marksInput, sizeof(marksInput), stdin);
                    marksInput[strcspn(marksInput, "\n")] = 0; // remove newline

                    // Check for cancel first
                    if (strcmp(marksInput, "cancel") == 0) {
                        break; // Exit the while(1) loop
                    }

                    // Try to parse the string as a float
                    if (sscanf(marksInput, "%f", &marks) == 1) {
                        break; // Valid float, exit the while(1) loop
                    }
                    else {
                        printf("CMS: Invalid marks format. Please enter a number (or 'cancel'): ");
                    }
                }

                // Check if the loop was broken by 'cancel'
                if (strcmp(marksInput, "cancel") == 0) {
                    fputs(line, file_out); // Write original line
                    printf("CMS: Update cancelled. Original record for ID=%d was kept.\n", updateId);
                    continue; // Skip to the next line in the file
                }
                // --- END MODIFIED MARKS INPUT ---


                // --- START CONFIRMATION ---
                printf("\n--- Review Changes ---\n");
                printf("OLD: %s, %s, %.1f\n", originalName, originalProgramme, originalMarks);
                printf("NEW: %s, %s, %.1f\n", name, programme, marks);
                printf("Are you sure you want to save these changes? (Y/N): ");

                // Read the confirmation, clearing buffer
                while (scanf(" %c", &confirmation) != 1) {
                    while (getchar() != '\n');
                }
                while (getchar() != '\n'); // always clear buffer after scanf

                if (confirmation == 'Y' || confirmation == 'y') {
                    // Write the *updated* record to the temp file
                    fprintf(file_out, "%d,%s,%s,%.1f\n", updateId, name, programme, marks);
                    printf("CMS: Record for ID=%d was successfully updated.\n", updateId);
                }
                else {
                    // Write the *original* record back to the temp file
                    fprintf(file_out, "%d,%s,%s,%.1f\n", updateId, originalName, originalProgramme, originalMarks);
                    printf("CMS: Update cancelled. Original record for ID=%d was kept.\n", updateId);
                }
                // --- END CONFIRMATION ---

            }
            else {
                // Not the ID we're looking for, write the *original* line to temp file
                fputs(line, file_out);
            }
        }
        else {
            // Line format might be incorrect, or not a data line, just preserve it
            fputs(line, file_out);
        }
    }

    // Close both files
    fclose(file_in);
    fclose(file_out);

    // Check if we found the record
    if (found) {
        // We found and updated the record (or user cancelled), so replace old file with new

        // --- START ROBUST FILE HANDLING ---
        // Check if remove() fails
        if (remove("Team_P7_4-CMS.txt") != 0) {
            printf("CMS: CRITICAL ERROR: Could not delete old database file. Changes not saved.\n");
            remove("temp.txt"); // Clean up the temp file
            return;
        }

        // Check if rename() fails
        if (rename("temp.txt", "Team_P7_4-CMS.txt") != 0) {
            printf("CMS: CRITICAL ERROR: Could not rename temp file. Database may be in an unstable state.\n");
            return;
        }
        // --- END ROBUST FILE HANDLING ---

        // Show all records after update, like insertRecord does
        viewRecords();
    }
    else {
        // Record was not found, so no changes were made. Delete the temp file.
        remove("temp.txt");
        printf("CMS: The record with ID=%d does not exist.\n", updateId);
    }
}