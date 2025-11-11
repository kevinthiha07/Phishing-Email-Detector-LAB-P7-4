#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STUDENTS 100

typedef struct {
    int id;
    char name[100];
    char programme[100];
    float marks;
} Student;

int checkDuplicateID(int newID);
void insertRecord();
void viewRecords();
void searchQuery();
void updateRecord();
void openFile();
void saveRecords();
void deleteRecord();
void showAllSorted(); // New function for sorted display

char currentFileName[100] = "Team_P7_4-CMS.txt";
char tempFileName[100] = "temp_workfile.txt";
int unsavedChanges = 0;

// Function to load students from file into array
int loadStudents(Student students[]) {
    FILE* file = fopen(tempFileName, "r");
    if (file == NULL) {
        return 0;
    }

    char line[200];
    int count = 0;

    // Skip header line
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file) != NULL && count < MAX_STUDENTS) {
        if (sscanf(line, "%d,%99[^,],%99[^,],%f",
            &students[count].id, students[count].name,
            students[count].programme, &students[count].marks) == 4) {
            count++;
        }
    }

    fclose(file);
    return count;
}

// Comparison functions for sorting
int compareIDAsc(const void* a, const void* b) {
    return ((Student*)a)->id - ((Student*)b)->id;
}

int compareIDDesc(const void* a, const void* b) {
    return ((Student*)b)->id - ((Student*)a)->id;
}

int compareMarksAsc(const void* a, const void* b) {
    float diff = ((Student*)a)->marks - ((Student*)b)->marks;
    return (diff > 0) - (diff < 0); // Returns -1, 0, or 1
}

int compareMarksDesc(const void* a, const void* b) {
    float diff = ((Student*)b)->marks - ((Student*)a)->marks;
    return (diff > 0) - (diff < 0); // Returns -1, 0, or 1
}

void showAllSorted() {
    Student students[MAX_STUDENTS];
    int count = loadStudents(students);

    if (count == 0) {
        printf("CMS: No student records found.\n");
        return;
    }

    printf("\nSort Options:\n");
    printf("1. Sort by ID (Ascending)\n");
    printf("2. Sort by ID (Descending)\n");
    printf("3. Sort by Marks (Ascending)\n");
    printf("4. Sort by Marks (Descending)\n");
    printf("Enter your choice (1-4): ");

    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("CMS: Invalid choice.\n");
        while (getchar() != '\n');
        return;
    }

    // Sort based on user choice
    switch (choice) {
    case 1:
        qsort(students, count, sizeof(Student), compareIDAsc);
        printf("\nStudent Records Sorted by ID (Ascending):\n");
        break;
    case 2:
        qsort(students, count, sizeof(Student), compareIDDesc);
        printf("\nStudent Records Sorted by ID (Descending):\n");
        break;
    case 3:
        qsort(students, count, sizeof(Student), compareMarksAsc);
        printf("\nStudent Records Sorted by Marks (Ascending):\n");
        break;
    case 4:
        qsort(students, count, sizeof(Student), compareMarksDesc);
        printf("\nStudent Records Sorted by Marks (Descending):\n");
        break;
    default:
        printf("CMS: Invalid choice. Displaying unsorted records.\n");
        break;
    }

    printf("ID         Name                 Programme               Marks\n");
    printf("-------------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        printf("%-10d %-20s %-23s %.1f\n",
            students[i].id, students[i].name, students[i].programme, students[i].marks);
    }
}

int main() {
    int choice;

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

    // Initialize: copy original file to temp working file
    FILE* source = fopen(currentFileName, "r");
    FILE* dest = fopen(tempFileName, "w");
    if (source && dest) {
        char ch;
        while ((ch = fgetc(source)) != EOF) {
            fputc(ch, dest);
        }
        fclose(source);
        fclose(dest);
    }

    while (1) {
        printf("\nStudent Management System (Current File: %s)\n", currentFileName);
        if (unsavedChanges) {
            printf("*** UNSAVED CHANGES ***\n");
        }
        printf("1. Open File\n");
        printf("2. Show All\n");
        printf("3. Show All w/ Sort\n");
        printf("4. Insert\n");
        printf("5. Query\n");
        printf("6. Update\n");
        printf("7. Delete\n");
        printf("8. Save Records\n");
        printf("9. Exit\n\n");

        printf("Enter your choice (1-9): ");
        if (scanf("%d", &choice) != 1) {
            printf("CMS: Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
        case 1:
            openFile();
            break;
        case 2:
            viewRecords();
            break;
        case 3:
            showAllSorted();
            break;
        case 4:
            insertRecord();
            break;
        case 5:
            searchQuery();
            break;
        case 6:
            updateRecord();
            break;
        case 7:
            deleteRecord();
            break;
        case 8:
            saveRecords();
            break;
        case 9:
            if (unsavedChanges) {
                char confirm;
                printf("\nCMS: You have unsaved changes! Are you sure you want to exit? (y/n): ");
                scanf(" %c", &confirm);
                if (confirm == 'y' || confirm == 'Y') {
                    printf("CMS: Exiting without saving. All changes discarded.\n");
                    remove(tempFileName);
                }
                else {
                    continue;
                }
            }
            printf("CMS: Goodbye!\n");
            remove(tempFileName);
            return 0;
        default:
            printf("CMS: Invalid choice. Please enter a number between 1-9.\n");
            break;
        }
    }

    return 0;
}

// The rest of the functions remain the same as previous code...
// [viewRecords, saveRecords, openFile, checkDuplicateID, insertRecord, searchQuery, updateRecord, deleteRecord]
// ... (all other functions remain unchanged from the previous version)

void viewRecords() {
    FILE* file = fopen(tempFileName, "r");
    if (file == NULL) {
        printf("Error: Cannot open file '%s'\n", tempFileName);
        return;
    }

    printf("\nStudent Records from '%s':\n", currentFileName);
    if (unsavedChanges) {
        printf("*** Displaying UNSAVED CHANGES ***\n");
    }
    printf("ID         Name                 Programme               Marks\n");
    printf("-------------------------------------------------------------\n");

    char line[200];
    int id;
    char name[100];
    char programme[100];
    float marks;

    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file) != NULL) {
        if (sscanf(line, "%d,%99[^,],%99[^,],%f", &id, name, programme, &marks) == 4) {
            printf("%-10d %-20s %-23s %.1f\n", id, name, programme, marks);
        }
    }

    fclose(file);
}

void saveRecords() {
    if (!unsavedChanges) {
        printf("CMS: No changes to save.\n");
        return;
    }

    FILE* source = fopen(tempFileName, "r");
    FILE* dest = fopen(currentFileName, "w");

    if (source == NULL || dest == NULL) {
        printf("CMS: Error: Cannot save records to file.\n");
        return;
    }

    char ch;
    while ((ch = fgetc(source)) != EOF) {
        fputc(ch, dest);
    }

    fclose(source);
    fclose(dest);

    unsavedChanges = 0;
    printf("CMS: All changes successfully saved to '%s'\n", currentFileName);
}

void openFile() {
    if (unsavedChanges) {
        char confirm;
        printf("\nCMS: You have unsaved changes! Are you sure you want to open a new file? (y/n): ");
        scanf(" %c", &confirm);
        if (!(confirm == 'y' || confirm == 'Y')) {
            return;
        }
    }

    char filename[100];

    printf("\n--- Open File ---\n");
    printf("Enter the filename to open (e.g., students.txt): ");
    scanf("%99s", filename);

    FILE* testFile = fopen(filename, "r");
    if (testFile == NULL) {
        printf("CMS: Error: Cannot open file '%s'. File does not exist or cannot be accessed.\n", filename);
        return;
    }
    fclose(testFile);

    strcpy(currentFileName, filename);
    unsavedChanges = 0;

    FILE* source = fopen(currentFileName, "r");
    FILE* dest = fopen(tempFileName, "w");
    if (source && dest) {
        char ch;
        while ((ch = fgetc(source)) != EOF) {
            fputc(ch, dest);
        }
        fclose(source);
        fclose(dest);
    }

    printf("CMS: Successfully opened file '%s'\n", filename);
    viewRecords();
}

int checkDuplicateID(int newID) {
    FILE* file = fopen(tempFileName, "r");
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
                return 1;
            }
        }
    }

    fclose(file);
    return 0;
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

    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("CMS: Error reading input!\n");
        return;
    }

    input[strcspn(input, "\n")] = 0;

    if (sscanf(input, "ID=\"%d\", Name=\"%99[^\"]\", Programme=\"%99[^\"]\", Marks=\"%f\"",
        &newID, name, programme, &marks) != 4) {
        printf("CMS: Error: Invalid input format!\n");
        printf("CMS: Expected: ID=\"number\", Name=\"name\", Programme=\"programme\", Marks=\"marks\"\n");
        return;
    }

    if (checkDuplicateID(newID)) {
        printf("CMS: Error: Student with ID %d already exists! Insertion cancelled.\n", newID);
        return;
    }

    FILE* file = fopen(tempFileName, "a");
    if (file == NULL) {
        printf("CMS: Error: Cannot open database file for writing\n");
        return;
    }

    fprintf(file, "%d,%s,%s,%.1f\n", newID, name, programme, marks);
    fclose(file);

    unsavedChanges = 1;
    printf("CMS: A new record with ID=%d was successfully inserted.\n", newID);
    printf("CMS: Use 'Save Records' to make changes permanent.\n");
    viewRecords();
}

void searchQuery() {
    int searchId;

    printf("Enter student ID to search: ");
    if (scanf("%d", &searchId) != 1) {
        printf("CMS: Invalid ID format.\n");
        while (getchar() != '\n');
        return;
    }

    FILE* file = fopen(tempFileName, "r");
    if (file == NULL) {
        printf("CMS: Error: Cannot open database file\n");
        return;
    }

    char line[200];
    int found = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        int currentId;
        char currentName[100], currentProgramme[100];
        float currentMarks;

        if (sscanf(line, "%d,%99[^,],%99[^,],%f", &currentId, currentName, currentProgramme, &currentMarks) == 4) {
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

void updateRecord() {
    int updateId, currentId;
    char name[100], programme[100];
    float marks;
    char originalName[100], originalProgramme[100];
    float originalMarks;
    char line[200];
    int found = 0;

    FILE* file_in = fopen(tempFileName, "r");
    FILE* file_out = fopen("temp_update.txt", "w");

    if (file_in == NULL || file_out == NULL) {
        printf("CMS: Error: Cannot open database file(s).\n");
        if (file_in) fclose(file_in);
        if (file_out) fclose(file_out);
        return;
    }

    printf("Enter student ID to update: ");
    if (scanf("%d", &updateId) != 1) {
        printf("CMS: Invalid ID format.\n");
        while (getchar() != '\n');
        fclose(file_in);
        fclose(file_out);
        remove("temp_update.txt");
        return;
    }
    while (getchar() != '\n');

    while (fgets(line, sizeof(line), file_in) != NULL) {
        if (sscanf(line, "%d,%99[^,],%99[^,],%f", &currentId, originalName, originalProgramme, &originalMarks) == 4) {
            if (currentId == updateId) {
                found = 1;
                char confirmation;

                printf("Record found. Enter new data for ID %d:\n", updateId);

                printf("Enter new Name: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;

                printf("Enter new Programme: ");
                fgets(programme, sizeof(programme), stdin);
                programme[strcspn(programme, "\n")] = 0;

                printf("Enter new Marks: ");
                scanf("%f", &marks);
                while (getchar() != '\n');

                printf("\n--- Review Changes ---\n");
                printf("OLD: %s, %s, %.1f\n", originalName, originalProgramme, originalMarks);
                printf("NEW: %s, %s, %.1f\n", name, programme, marks);
                printf("Are you sure you want to save these changes? (Y/N): ");

                scanf(" %c", &confirmation);
                while (getchar() != '\n');

                if (confirmation == 'Y' || confirmation == 'y') {
                    fprintf(file_out, "%d,%s,%s,%.1f\n", updateId, name, programme, marks);
                    printf("CMS: Record for ID=%d was successfully updated.\n", updateId);
                    unsavedChanges = 1;
                    printf("CMS: Use 'Save Records' to make changes permanent.\n");
                }
                else {
                    fprintf(file_out, "%d,%s,%s,%.1f\n", updateId, originalName, originalProgramme, originalMarks);
                    printf("CMS: Update cancelled. Original record for ID=%d was kept.\n", updateId);
                }
            }
            else {
                fputs(line, file_out);
            }
        }
        else {
            fputs(line, file_out);
        }
    }

    fclose(file_in);
    fclose(file_out);

    if (found) {
        remove(tempFileName);
        rename("temp_update.txt", tempFileName);
        viewRecords();
    }
    else {
        remove("temp_update.txt");
        printf("CMS: The record with ID=%d does not exist.\n", updateId);
    }
}

void deleteRecord() {
    printf("CMS: Delete function - To be implemented\n");
}