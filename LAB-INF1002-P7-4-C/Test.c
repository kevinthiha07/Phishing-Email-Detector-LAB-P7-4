#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_STUDENTS 100

typedef struct {
    int id;
    char name[100];
    char programme[100];
    float marks;
} Student;

// Function prototypes
int checkDuplicateID(int newID);
void insertRecord();
void viewRecords();
void searchQuery();
void updateRecord();
void openFile();
void saveRecords();
void deleteRecord();
void showAllSorted();
void exportToCSV();
void initializeSystem();
void showMainMenu();

// Global variables
char currentFileName[100] = "";
char tempFileName[100] = "temp_workfile.txt";
int unsavedChanges = 0;
int fileLoaded = 0;

// Function to load students from file into array for sorting
int loadStudents(Student students[]) {
    FILE* file = fopen(tempFileName, "r");
    if (file == NULL) {
        return 0;
    }

    char line[200];
    int count = 0;

    // Skip header line
    fgets(line, sizeof(line), file);

    // Read student data
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

// --- Comparison functions for qsort ---
int compareIDDesc(const void* a, const void* b) {
    return ((Student*)b)->id - ((Student*)a)->id;
}

int compareMarksDesc(const void* a, const void* b) {
    float diff = ((Student*)b)->marks - ((Student*)a)->marks;
    return (diff > 0) - (diff < 0);
}

// --- Modified Sorting and Display Function with Text Input ---
void showAllSorted() {
    if (!fileLoaded) {
        printf("CMS: No file loaded. Please open a file first.\n");
        return;
    }

    Student students[MAX_STUDENTS];
    int count = loadStudents(students);

    if (count == 0) {
        printf("CMS: No student records found.\n");
        return;
    }

    printf("\nSort Options:\n");
    printf("Available sorts: 'ID' or 'Marks' (both descending)\n");
    printf("Enter your choice: ");

    char input[20];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("CMS: Error reading input.\n");
        return;
    }

    // Remove newline character
    input[strcspn(input, "\n")] = 0;

    // Convert to lowercase
    for (int i = 0; input[i]; i++) {
        input[i] = tolower(input[i]);
    }

    char sortField[30];
    int sorted = 1;

    if (strcmp(input, "id") == 0) {
        qsort(students, count, sizeof(Student), compareIDDesc);
        strcpy(sortField, "ID (Descending)");
    }
    else if (strcmp(input, "marks") == 0) {
        qsort(students, count, sizeof(Student), compareMarksDesc);
        strcpy(sortField, "Marks (Descending)");
    }
    else {
        printf("CMS: Invalid choice '%s'. Displaying unsorted records.\n", input);
        strcpy(sortField, "Unsorted");
        sorted = 0;
    }

    if (sorted) {
        printf("\nStudent Records Sorted by %s:\n", sortField);
    }
    else {
        printf("\nStudent Records (%s):\n", sortField);
    }

    printf("ID          Name                 Programme               Marks\n");
    printf("-------------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        printf("%-10d %-20s %-23s %.1f\n",
            students[i].id, students[i].name, students[i].programme, students[i].marks);
    }
}

// --- New Function: Initialize System ---
void initializeSystem() {
    remove(tempFileName);
    printf("=== Class Management System (CMS) ===\n");
    printf("System initialized. Please open a data file to begin.\n");
}

// --- New Function: Show Main Menu ---
void showMainMenu() {
    int choice;

    while (1) {
        printf("\nStudent Management System (Current File: %s)\n", currentFileName);
        if (unsavedChanges) {
            printf("*** UNSAVED CHANGES ***\n");
        }
      
        printf("1. Show All\n");
        printf("2. Show All w/ Sort\n");
        printf("3. Insert\n");
        printf("4. Query\n");
        printf("5. Update\n");
        printf("6. Delete\n");
        printf("7. Save Records\n");
        printf("8. Export to CSV\n");
        printf("9. Exit\n\n");

        printf("Enter your choice (1-10): ");
        if (scanf("%d", &choice) != 1) {
            printf("CMS: Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        switch (choice) {
        case 1:
            viewRecords();
            break;
        case 2:
            showAllSorted();
            break;
        case 3:
            insertRecord();
            break;
        case 4:
            searchQuery();
            break;
        case 5:
            updateRecord();
            break;
        case 6:
            deleteRecord();
            break;
        case 7:
            saveRecords();
            break;
        case 8:
            exportToCSV();
            break;
        case 9:
            if (unsavedChanges) {
                char confirm;
                printf("\nCMS: You have unsaved changes! Are you sure you want to exit? (y/n): ");
                scanf(" %c", &confirm);
                while (getchar() != '\n');
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
            exit(0);
        default:
            printf("CMS: Invalid choice. Please enter a number between 1-10.\n");
            break;
        }
    }
}

// --- Modified Main Function ---
int main() {
    // --- Declaration Print ---
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

    // Initialize system
    initializeSystem();

    // Initially, only show open file option
    int choice;
    while (!fileLoaded) {

        printf("1. Open File\n");
        printf("2. Exit\n");
        printf("\nEnter your choice (1-2): ");

        if (scanf("%d", &choice) != 1) {
            printf("CMS: Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        switch (choice) {
        case 1:
            openFile();
            if (fileLoaded) {
                showMainMenu();
            }
            break;
        case 2:
            printf("CMS: Goodbye!\n");
            remove(tempFileName);
            return 0;
        default:
            printf("CMS: Invalid choice. Please enter 1 or 2.\n");
            break;
        }
    }

    return 0;
}

// --- Other Function Implementations ---

void viewRecords() {
    if (!fileLoaded) {
        printf("CMS: No file loaded. Please open a file first.\n");
        return;
    }

    FILE* file = fopen(tempFileName, "r");
    if (file == NULL) {
        printf("Error: Cannot open file '%s'\n", tempFileName);
        return;
    }

    printf("\nStudent Records from '%s':\n", currentFileName);
    if (unsavedChanges) {
        printf("*** Displaying UNSAVED CHANGES ***\n");
    }
    printf("ID          Name                 Programme               Marks\n");
    printf("-------------------------------------------------------------\n");

    char line[200];
    int id;
    char name[100];
    char programme[100];
    float marks;

    fgets(line, sizeof(line), file); // Skip header line

    int recordsFound = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        if (sscanf(line, "%d,%99[^,],%99[^,],%f", &id, name, programme, &marks) == 4) {
            printf("%-10d %-20s %-23s %.1f\n", id, name, programme, marks);
            recordsFound = 1;
        }
    }

    if (!recordsFound) {
        printf("CMS: No student records found.\n");
    }

    fclose(file);
}

void saveRecords() {
    if (!fileLoaded) {
        printf("CMS: No file loaded. Please open a file first.\n");
        return;
    }

    if (!unsavedChanges) {
        printf("CMS: No changes to save.\n");
        return;
    }

    FILE* source = fopen(tempFileName, "r");
    FILE* dest = fopen(currentFileName, "w");

    if (source == NULL || dest == NULL) {
        printf("CMS: Error: Cannot save records to file.\n");
        if (source) fclose(source);
        if (dest) fclose(dest);
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

// --- NEW OPEN FILE FUNCTION ---
void openFile() {
    if (unsavedChanges && fileLoaded) {
        char confirm;
        printf("\nCMS: You have unsaved changes! Are you sure you want to open a new file? (y/n): ");
        scanf(" %c", &confirm);
        while (getchar() != '\n');
        if (!(confirm == 'y' || confirm == 'Y')) {
            return;
        }
    }

    char filename[100];

    printf("\n--- Open File ---\n");
    printf("Enter the filename to open (e.g., students.txt): ");
    scanf("%99s", filename);
    while (getchar() != '\n');

    FILE* testFile = fopen(filename, "r");
    if (testFile == NULL) {
        printf("CMS: Error: Cannot open file '%s'. File does not exist or cannot be accessed.\n", filename);
        return;
    }

    // File exists, now copy it to temp working file
    FILE* dest = fopen(tempFileName, "w");
    if (dest == NULL) {
        printf("CMS: Error: Could not create temp work file.\n");
        fclose(testFile);
        return;
    }

    char ch;
    while ((ch = fgetc(testFile)) != EOF) {
        fputc(ch, dest);
    }

    fclose(testFile);
    fclose(dest);

    strcpy(currentFileName, filename);
    unsavedChanges = 0;
    fileLoaded = 1;

    printf("CMS: Successfully opened file '%s'\n", filename);
    viewRecords();
}

int checkDuplicateID(int newID) {
    if (!fileLoaded) {
        return 0;
    }

    FILE* file = fopen(tempFileName, "r");
    if (file == NULL) {
        return 0;
    }

    char line[200];
    int existingID;
    char tempName[100], tempProgramme[100];
    float tempMarks;

    fgets(line, sizeof(line), file); // Skip header

    while (fgets(line, sizeof(line), file) != NULL) {
        if (sscanf(line, "%d,%99[^,],%99[^,],%f", &existingID, tempName, tempProgramme, &tempMarks) == 4) {
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
    if (!fileLoaded) {
        printf("CMS: No file loaded. Please open a file first.\n");
        return;
    }

    int newID;
    char name[100];
    char programme[100];
    float marks;
    char input[500];

    printf("\n--- Insert New Student Record ---\n");
    printf("Enter Student Data (Format: ID=number Name=name Programme=programme Mark=marks): ");

    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("CMS: Error reading input!\n");
        return;
    }

    input[strcspn(input, "\n")] = 0; // Remove trailing newline

    // --- Start of Corrected/Simplified Parsing ---
    // Use a single, robust sscanf attempt to handle spaces in names/programmes properly,
    // assuming the format is exactly: ID=... Name=... Programme=... Mark=...
    char tempName[100], tempProgramme[100];
    int success = 0;

    // Look for the required fields using delimiters to handle spaces in Name
    char* id_ptr = strstr(input, "ID=");
    char* name_ptr = strstr(input, "Name=");
    char* prog_ptr = strstr(input, "Programme=");
    char* mark_ptr = strstr(input, "Mark=");

    if (id_ptr && name_ptr && prog_ptr && mark_ptr && id_ptr < name_ptr && name_ptr < prog_ptr && prog_ptr < mark_ptr) {
        id_ptr += 3;
        name_ptr += 5;
        prog_ptr += 10;
        mark_ptr += 5;

        // 1. Extract ID
        if (sscanf(id_ptr, "%d", &newID) != 1) goto parse_error;

        // 2. Extract Name (between "Name=" and " Programme=")
        char* name_end = prog_ptr - 10;
        if (name_end <= name_ptr) goto parse_error;
        strncpy(name, name_ptr, name_end - name_ptr);
        name[name_end - name_ptr - 1] = '\0'; // -1 to skip the space before Programme

        // 3. Extract Programme (between "Programme=" and " Mark=")
        char* prog_end = mark_ptr - 5;
        if (prog_end <= prog_ptr) goto parse_error;
        strncpy(programme, prog_ptr, prog_end - prog_ptr);
        programme[prog_end - prog_ptr - 1] = '\0'; // -1 to skip the space before Mark

        // 4. Extract Mark
        if (sscanf(mark_ptr, "%f", &marks) != 1) goto parse_error;

        success = 1;
    }

    // Fallback/Simpler parsing attempt (if names/programmes don't contain spaces)
    if (!success) {
        if (sscanf(input, "ID=%d Name=%99s Programme=%99s Mark=%f",
            &newID, tempName, tempProgramme, &marks) == 4) {
            strcpy(name, tempName);
            strcpy(programme, tempProgramme);
            success = 1;
        }
    }

    if (!success) {
    parse_error:
        printf("CMS: Error: Invalid input format!\n");
        printf("CMS: Expected: ID=number Name=name Programme=programme Mark=marks\n");
        return;
    }

    // --- End of Corrected/Simplified Parsing ---

    // Validate mark range
    if (marks < 0 || marks > 100) {
        printf("CMS: Error: Marks must be between 0 and 100!\n");
        return;
    }

    if (checkDuplicateID(newID)) {
        printf("CMS: Error: Student with ID %d already exists! Insertion cancelled.\n", newID);
        return;
    }

    // Use append mode 'a' to add the new record to the working file
    FILE* file = fopen(tempFileName, "a");
    if (file == NULL) {
        printf("CMS: Error: Cannot open database file for writing\n");
        return;
    }

    // Write the new record in CSV format
    fprintf(file, "%d,%s,%s,%.1f\n", newID, name, programme, marks);
    fclose(file); // CRITICAL: Close the file to ensure data is written to disk

    unsavedChanges = 1;
    printf("CMS: A new record with ID=%d was successfully inserted.\n", newID);
    printf("CMS: Use 'Save Records' to make changes permanent.\n");
    viewRecords();
}

void searchQuery() {
    if (!fileLoaded) {
        printf("CMS: No file loaded. Please open a file first.\n");
        return;
    }

    int searchId;

    printf("Enter student ID to search: ");
    if (scanf("%d", &searchId) != 1) {
        printf("CMS: Invalid ID format.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    FILE* file = fopen(tempFileName, "r");
    if (file == NULL) {
        printf("CMS: Error: Cannot open database file\n");
        return;
    }

    char line[200];
    int found = 0;

    fgets(line, sizeof(line), file); // Skip header

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
    if (!fileLoaded) {
        printf("CMS: No file loaded. Please open a file first.\n");
        return;
    }

    int updateId, currentId;
    char name[100], programme[100];
    float marks;
    char originalName[100], originalProgramme[100];
    float originalMarks;
    char line[200];
    int found = 0;
    char temp_update_file[] = "temp_update.txt";

    FILE* file_in = fopen(tempFileName, "r");
    FILE* file_out = fopen(temp_update_file, "w");

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
        remove(temp_update_file);
        return;
    }
    while (getchar() != '\n');

    while (fgets(line, sizeof(line), file_in) != NULL) {
        if (sscanf(line, "%d,%99[^,],%99[^,],%f", &currentId, originalName, originalProgramme, &originalMarks) == 4) {
            if (currentId == updateId) {
                found = 1;
                char confirmation;
                char input[500];

                printf("Record found. ID %d: %s, %s, %.1f\n", updateId, originalName, originalProgramme, originalMarks);
                printf("Enter new data (Format: Name=\"Name\", Programme=\"Prog\", Marks=\"0.0\") or press Enter to cancel: ");

                if (fgets(input, sizeof(input), stdin) == NULL) {
                    printf("CMS: Error reading input!\n");
                    fputs(line, file_out);
                    break;
                }
                input[strcspn(input, "\n")] = 0;

                if (strlen(input) == 0) {
                    printf("CMS: Update cancelled. Original record for ID=%d was kept.\n", updateId);
                    fputs(line, file_out);
                    continue;
                }

                if (sscanf(input, "Name=\"%99[^\"]\", Programme=\"%99[^\"]\", Marks=\"%f\"",
                    name, programme, &marks) != 3) {
                    printf("CMS: Error: Invalid input format!\n");
                    printf("CMS: Update cancelled. Original record for ID=%d was kept.\n", updateId);
                    fputs(line, file_out);
                    continue;
                }

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
        if (remove(tempFileName) != 0) {
            printf("CMS: Error: Could not remove old temp file.\n");
        }
        if (rename(temp_update_file, tempFileName) != 0) {
            printf("CMS: Error: Could not rename new temp file.\n");
        }
        if (unsavedChanges) {
            viewRecords();
        }
    }
    else {
        remove(temp_update_file);
        printf("CMS: The record with ID=%d does not exist.\n", updateId);
    }
}

void deleteRecord() {
    if (!fileLoaded) {
        printf("CMS: No file loaded. Please open a file first.\n");
        return;
    }

    int targetID;
    printf("P1_1: DELETE ID=");
    if (scanf("%d", &targetID) != 1) {
        printf("CMS: Invalid ID format.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    FILE* file_in = fopen(tempFileName, "r");
    if (file_in == NULL) {
        printf("CMS: Error: Cannot open database file '%s'.\n", tempFileName);
        return;
    }

    char temp_delete_file[] = "temp_delete.txt";
    FILE* file_out = fopen(temp_delete_file, "w");
    if (file_out == NULL) {
        printf("CMS: Error: Cannot create temporary file for delete.\n");
        fclose(file_in);
        return;
    }

    char line[200];
    int found = 0;

    if (fgets(line, sizeof(line), file_in) != NULL) {
        fputs(line, file_out);
    }

    int id;
    char name[100];
    char programme[100];
    float marks;

    while (fgets(line, sizeof(line), file_in) != NULL) {
        if (sscanf(line, "%d,%99[^,],%99[^,],%f", &id, name, programme, &marks) == 4) {
            if (id == targetID) {
                found = 1;

                printf("CMS: Are you sure you want to delete record with ID=%d? ", targetID);
                printf("Type \"Y\" to Confirm or type \"N\" to cancel.\n");

                printf("P1_1: ");
                char confirm;
                if (scanf(" %c", &confirm) != 1) {
                    confirm = 'N';
                }
                while (getchar() != '\n');

                if (confirm == 'Y' || confirm == 'y') {
                    printf("CMS: The record with ID=%d is successfully deleted.\n", targetID);
                    unsavedChanges = 1;
                    printf("CMS: Use 'Save Records' to make changes permanent.\n");
                    continue;
                }
                else {
                    printf("CMS: The deletion is cancelled.\n");
                    fputs(line, file_out);
                    continue;
                }
            }
        }
        fputs(line, file_out);
    }

    fclose(file_in);
    fclose(file_out);

    if (!found) {
        printf("CMS: The record with ID=%d does not exist.\n", targetID);
        remove(temp_delete_file);
        return;
    }

    if (remove(tempFileName) != 0) {
        printf("CMS: Error: Could not remove old temp file.\n");
    }
    else if (rename(temp_delete_file, tempFileName) != 0) {
        printf("CMS: Error: Could not rename new temp file.\n");
    }
    else {
        if (unsavedChanges) {
            viewRecords();
        }
    }
}

void exportToCSV() {
    if (!fileLoaded) {
        printf("CMS: No file loaded. Please open a file first.\n");
        return;
    }

    char exportFileName[120];

    printf("\n--- Export Records to CSV ---\n");
    if (unsavedChanges) {
        printf("CMS: Note: The CSV will include UNSAVED CHANGES from the current session.\n");
    }

    printf("Enter CSV filename to export to (e.g., records_export.csv): ");
    if (scanf("%119s", exportFileName) != 1) {
        printf("CMS: Invalid filename input.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    FILE* source = fopen(tempFileName, "r");
    if (source == NULL) {
        printf("CMS: Error: Cannot open temporary database file '%s'.\n", tempFileName);
        return;
    }

    FILE* dest = fopen(exportFileName, "w");
    if (dest == NULL) {
        printf("CMS: Error: Cannot create CSV file '%s'.\n", exportFileName);
        fclose(source);
        return;
    }

    char ch;
    while ((ch = fgetc(source)) != EOF) {
        fputc(ch, dest);
    }

    fclose(source);
    fclose(dest);

    printf("CMS: Records successfully exported to '%s'.\n", exportFileName);
    printf("CMS: You can open this file with Excel or other spreadsheet software.\n");
}