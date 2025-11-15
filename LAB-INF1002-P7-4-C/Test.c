#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define INITIAL_CAPACITY 10

typedef struct {
    int id;
    char name[100];
    char programme[100];
    float marks;
} Student;

// Function declarations
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
void showSummaryStatistics();
void cleanup();
void loadGlobalStudents();
void resyncTempFile();
int checkDuplicateID(int newID);

// Global variables
char currentFileName[100] = "";
char tempFileName[100] = "temp_workfile.txt";
int unsavedChanges = 0;
int fileLoaded = 0;

Student* g_students = NULL;
int g_studentCount = 0;
int g_studentCapacity = 0;

// Comparison functions for sorting
int compareIDDesc(const void* a, const void* b) {
    return ((Student*)b)->id - ((Student*)a)->id;
}

int compareMarksDesc(const void* a, const void* b) {
    float diff = ((Student*)b)->marks - ((Student*)a)->marks;
    return (diff > 0) - (diff < 0);
}

void cleanup() {
    if (g_students != NULL) {
        free(g_students);
        g_students = NULL;
    }
    remove(tempFileName);
    g_studentCount = 0;
    g_studentCapacity = 0;
    fileLoaded = 0;
    unsavedChanges = 0;
    strcpy(currentFileName, "");
}

void loadGlobalStudents() {
    if (g_students != NULL) {
        free(g_students);
        g_students = NULL;
        g_studentCount = 0;
        g_studentCapacity = 0;
    }

    FILE* file = fopen(tempFileName, "r");
    if (file == NULL) {
        return;
    }

    char line[200];
    fgets(line, sizeof(line), file); // Skip header

    while (fgets(line, sizeof(line), file) != NULL) {
        if (g_studentCount == g_studentCapacity) {
            if (g_studentCapacity == 0) {
                g_studentCapacity = INITIAL_CAPACITY;
                g_students = (Student*)calloc(g_studentCapacity, sizeof(Student));
            }
            else {
                g_studentCapacity *= 2;
                Student* temp = (Student*)realloc(g_students, g_studentCapacity * sizeof(Student));
                if (temp == NULL) {
                    printf("CMS: Memory allocation failed!\n");
                    fclose(file);
                    return;
                }
                g_students = temp;
            }
        }

        if (sscanf(line, "%d,%99[^,],%99[^,],%f",
            &g_students[g_studentCount].id,
            g_students[g_studentCount].name,
            g_students[g_studentCount].programme,
            &g_students[g_studentCount].marks) == 4) {
            g_studentCount++;
        }
    }
    fclose(file);
}

void resyncTempFile() {
    FILE* file = fopen(tempFileName, "w");
    if (file == NULL) return;

    fprintf(file, "ID,Name,Programme,Mark\n");
    for (int i = 0; i < g_studentCount; i++) {
        fprintf(file, "%d,%s,%s,%.1f\n",
            g_students[i].id,
            g_students[i].name,
            g_students[i].programme,
            g_students[i].marks);
    }
    fclose(file);
}

void deleteRecord() {
    if (!fileLoaded) {
        printf("CMS: No file loaded. Please open a file first.\n");
        return;
    }

    int deleteId;
    printf("Enter student ID to delete: ");
    if (scanf("%d", &deleteId) != 1) {
        printf("CMS: Invalid ID format.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    int foundIndex = -1;
    for (int i = 0; i < g_studentCount; i++) {
        if (g_students[i].id == deleteId) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1) {
        printf("CMS: Record with ID=%d does not exist.\n", deleteId);
        return;
    }

    printf("Record to delete: %s, %s, %.1f\n",
        g_students[foundIndex].name,
        g_students[foundIndex].programme,
        g_students[foundIndex].marks);

    char confirmation;
    printf("Are you sure you want to delete this record? (Y/N): ");
    scanf(" %c", &confirmation);
    while (getchar() != '\n');

    if (confirmation == 'Y' || confirmation == 'y') {
        for (int i = foundIndex; i < g_studentCount - 1; i++) {
            g_students[i] = g_students[i + 1];
        }
        g_studentCount--;

        resyncTempFile();
        unsavedChanges = 1;
        printf("CMS: Record with ID=%d was successfully deleted.\n", deleteId);
    }
    else {
        printf("CMS: Deletion cancelled.\n");
    }
}

void showAllSorted() {
    if (!fileLoaded) {
        printf("CMS: No file loaded. Please open a file first.\n");
        return;
    }

    if (g_studentCount == 0) {
        printf("CMS: No student records found.\n");
        return;
    }

    Student* sortedArray = (Student*)malloc(g_studentCount * sizeof(Student));
    if (sortedArray == NULL) {
        printf("CMS: Memory allocation failed!\n");
        return;
    }

    memcpy(sortedArray, g_students, g_studentCount * sizeof(Student));

    printf("\nSort Options: 'ID' or 'Marks'\nEnter your choice: ");

    char input[20];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        free(sortedArray);
        return;
    }
    input[strcspn(input, "\n")] = 0;
    for (int i = 0; input[i]; i++) {
        input[i] = tolower(input[i]);
    }

    if (strcmp(input, "id") == 0) {
        qsort(sortedArray, g_studentCount, sizeof(Student), compareIDDesc);
        printf("\nStudent Records Sorted by ID (Descending):\n");
    }
    else if (strcmp(input, "marks") == 0) {
        qsort(sortedArray, g_studentCount, sizeof(Student), compareMarksDesc);
        printf("\nStudent Records Sorted by Marks (Descending):\n");
    }
    else {
        printf("CMS: Invalid choice. Displaying unsorted records.\n");
    }

    printf("ID          Name                 Programme                Marks\n");
    printf("-------------------------------------------------------------\n");
    for (int i = 0; i < g_studentCount; i++) {
        printf("%-10d %-20s %-23s %.1f\n",
            sortedArray[i].id, sortedArray[i].name, sortedArray[i].programme, sortedArray[i].marks);
    }

    free(sortedArray);
}

void initializeSystem() {
    printf("=== Class Management System (CMS) ===\n");
}

void showMainMenu() {
    int choice;
    while (1) {
        printf("\nStudent Management System (File: %s | Records: %d)\n",
            currentFileName, g_studentCount);
        if (unsavedChanges) printf("*** UNSAVED CHANGES ***\n");

        printf("1. Show All\n2. Show All w/ Sort\n3. Insert\n4. Query\n5. Update\n");
        printf("6. Delete\n7. Save Records\n8. Export to CSV\n9. Show Summary Statistics\n10. Exit\n");
        printf("Enter your choice (1-10): ");

        if (scanf("%d", &choice) != 1) {
            printf("CMS: Invalid input.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        switch (choice) {
        case 1: viewRecords(); break;
        case 2: showAllSorted(); break;
        case 3: insertRecord(); break;
        case 4: searchQuery(); break;
        case 5: updateRecord(); break;
        case 6: deleteRecord(); break;
        case 7: saveRecords(); break;
        case 8: exportToCSV(); break;
        case 9: showSummaryStatistics(); break;
        case 10:
            if (unsavedChanges) {
                char confirm;
                printf("CMS: Unsaved changes! Exit anyway? (y/n): ");
                scanf(" %c", &confirm);
                while (getchar() != '\n');
                if (confirm != 'y' && confirm != 'Y') continue;
            }
            printf("CMS: Goodbye!\n");
            cleanup();
            exit(0);
        default: printf("CMS: Invalid choice.\n");
        }
    }
}

int main() {
    printf("Declaration\nDeclared by: P7_4\nDate: 25 November 2025\n\n");
    initializeSystem();

    int choice;
    while (1) {
        if (!fileLoaded) {
            printf("\n--- Main Menu ---\n1. Open File\n2. Exit\nEnter your choice (1-2): ");

            if (scanf("%d", &choice) != 1) {
                printf("CMS: Invalid input.\n");
                while (getchar() != '\n');
                continue;
            }
            while (getchar() != '\n');

            switch (choice) {
            case 1:
                openFile();
                if (fileLoaded) {
                    showMainMenu(); // This is the key line that was missing!
                }
                break;
            case 2:
                printf("CMS: Goodbye!\n");
                cleanup();
                return 0;
            default:
                printf("CMS: Invalid choice.\n");
            }
        }
        else {
            showMainMenu(); // If file is already loaded, show main menu directly
        }
    }
    return 0;
}

void viewRecords() {
    if (!fileLoaded) {
        printf("CMS: No file loaded. Please open a file first.\n");
        return;
    }

    printf("\nStudent Records from '%s':\n", currentFileName);
    if (unsavedChanges) {
        printf("*** Displaying UNSAVED CHANGES (from memory) ***\n");
    }

    if (g_studentCount == 0) {
        printf("CMS: No student records found.\n");
        return;
    }

    printf("ID          Name                 Programme                Marks\n");
    printf("-------------------------------------------------------------\n");

    for (int i = 0; i < g_studentCount; i++) {
        printf("%-10d %-20s %-23s %.1f\n",
            g_students[i].id,
            g_students[i].name,
            g_students[i].programme,
            g_students[i].marks);
    }
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

    if (fileLoaded) {
        cleanup();
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

    loadGlobalStudents();
    printf("CMS: Successfully opened file '%s'\n", filename);
    viewRecords();
}

int checkDuplicateID(int newID) {
    if (!fileLoaded) {
        return 0;
    }

    for (int i = 0; i < g_studentCount; i++) {
        if (g_students[i].id == newID) {
            return 1;
        }
    }
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
    input[strcspn(input, "\n")] = 0;

    char tempName[100], tempProgramme[100];
    int success = 0;
    char* id_ptr = strstr(input, "ID=");
    char* name_ptr = strstr(input, "Name=");
    char* prog_ptr = strstr(input, "Programme=");
    char* mark_ptr = strstr(input, "Mark=");

    if (id_ptr && name_ptr && prog_ptr && mark_ptr && id_ptr < name_ptr && name_ptr < prog_ptr && prog_ptr < mark_ptr) {
        id_ptr += 3;
        name_ptr += 5;
        prog_ptr += 10;
        mark_ptr += 5;
        if (sscanf(id_ptr, "%d", &newID) != 1) goto parse_error;
        char* name_end = prog_ptr - 10;
        if (name_end <= name_ptr) goto parse_error;
        strncpy(name, name_ptr, name_end - name_ptr);
        name[name_end - name_ptr - 1] = '\0';
        char* prog_end = mark_ptr - 5;
        if (prog_end <= prog_ptr) goto parse_error;
        strncpy(programme, prog_ptr, prog_end - prog_ptr);
        programme[prog_end - prog_ptr - 1] = '\0';
        if (sscanf(mark_ptr, "%f", &marks) != 1) goto parse_error;
        success = 1;
    }
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
        printf("CMS: Example: ID=1 Name=GuoLai Programme=Grab Food Mark=100.0\n");
        return;
    }

    if (marks < 0 || marks > 100) {
        printf("CMS: Error: Marks must be between 0 and 100!\n");
        return;
    }

    if (checkDuplicateID(newID)) {
        printf("CMS: Error: Student with ID %d already exists! Insertion cancelled.\n", newID);
        return;
    }

    if (g_studentCount == g_studentCapacity) {
        if (g_studentCapacity == 0) {
            g_studentCapacity = INITIAL_CAPACITY;
            g_students = (Student*)calloc(g_studentCapacity, sizeof(Student));
        }
        else {
            g_studentCapacity *= 2;
            Student* temp = (Student*)realloc(g_students, g_studentCapacity * sizeof(Student));
            if (temp == NULL) {
                printf("CMS: CRITICAL ERROR: Could not reallocate memory! Insertion failed.\n");
                return;
            }
            g_students = temp;
        }
    }

    g_students[g_studentCount].id = newID;
    strcpy(g_students[g_studentCount].name, name);
    strcpy(g_students[g_studentCount].programme, programme);
    g_students[g_studentCount].marks = marks;
    g_studentCount++;

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

    int found = 0;
    for (int i = 0; i < g_studentCount; i++) {
        if (g_students[i].id == searchId) {
            printf("CMS: The record with ID=%d is found in the data table.\n", searchId);
            printf("ID\t\tName\t\tProgramme\t\tMark\n");
            printf("------------------------------------------------------------\n");
            printf("%d\t\t%s\t\t%s\t\t%.1f\n",
                g_students[i].id,
                g_students[i].name,
                g_students[i].programme,
                g_students[i].marks);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("CMS: The record with ID=%d does not exist.\n", searchId);
    }
}

void updateRecord() {
    if (!fileLoaded) {
        printf("CMS: No file loaded. Please open a file first.\n");
        return;
    }

    int updateId;
    printf("Enter student ID to update: ");
    if (scanf("%d", &updateId) != 1) {
        printf("CMS: Invalid ID format.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    int foundIndex = -1;
    for (int i = 0; i < g_studentCount; i++) {
        if (g_students[i].id == updateId) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1) {
        printf("CMS: Record with ID=%d does not exist.\n", updateId);
        return;
    }

    printf("Record found: %s, %s, %.1f\n",
        g_students[foundIndex].name,
        g_students[foundIndex].programme,
        g_students[foundIndex].marks);

    char* input = (char*)malloc(500 * sizeof(char));
    if (!input) {
        printf("CMS: Memory allocation failed!\n");
        return;
    }

    printf("Enter new data (Name=name Programme=programme Mark=marks) or Enter to cancel: ");

    if (!fgets(input, 500, stdin)) {
        printf("CMS: Error reading input!\n");
        free(input);
        return;
    }
    input[strcspn(input, "\n")] = 0;

    if (strlen(input) == 0) {
        printf("CMS: Update cancelled.\n");
        free(input);
        return;
    }

    char name[100], programme[100];
    float marks;
    int success = 0;

    char* name_ptr = strstr(input, "Name=");
    char* prog_ptr = strstr(input, "Programme=");
    char* mark_ptr = strstr(input, "Mark=");

    if (name_ptr && prog_ptr && mark_ptr) {
        name_ptr += 5;
        prog_ptr += 10;
        mark_ptr += 5;

        char* name_end = strstr(name_ptr, " Programme=");
        if (name_end) {
            strncpy(name, name_ptr, name_end - name_ptr);
            name[name_end - name_ptr] = '\0';

            char* prog_end = strstr(prog_ptr, " Mark=");
            if (prog_end) {
                strncpy(programme, prog_ptr, prog_end - prog_ptr);
                programme[prog_end - prog_ptr] = '\0';

                char marks_str[50];
                strncpy(marks_str, mark_ptr, sizeof(marks_str) - 1);
                marks_str[sizeof(marks_str) - 1] = '\0';

                if (sscanf(marks_str, "%f", &marks) == 1) {
                    success = 1;
                }
            }
        }
    }

    if (!success) {
        printf("CMS: Invalid format! Use: Name=full name Programme=programme name Mark=marks\n");
        printf("CMS: Example: Name=John Smith Programme=Computer Science Mark=85.5\n");
        free(input);
        return;
    }

    free(input);

    if (marks < 0 || marks > 100) {
        printf("CMS: Marks must be 0-100!\n");
        return;
    }

    char confirmation;
    printf("\nOLD: %s, %s, %.1f\nNEW: %s, %s, %.1f\nSave changes? (Y/N): ",
        g_students[foundIndex].name, g_students[foundIndex].programme, g_students[foundIndex].marks,
        name, programme, marks);

    scanf(" %c", &confirmation);
    while (getchar() != '\n');

    if (confirmation == 'Y' || confirmation == 'y') {
        strcpy(g_students[foundIndex].name, name);
        strcpy(g_students[foundIndex].programme, programme);
        g_students[foundIndex].marks = marks;

        resyncTempFile();
        printf("CMS: Record updated. Use 'Save Records' to make permanent.\n");
        unsavedChanges = 1;
    }
    else {
        printf("CMS: Update cancelled.\n");
    }
}

void showSummaryStatistics() {
    if (!fileLoaded) {
        printf("CMS: No file loaded. Please open a file first.\n");
        return;
    }

    if (g_studentCount == 0) {
        printf("CMS: No student records found. Cannot generate summary.\n");
        return;
    }

    double totalMarks = 0.0;
    float highestMark = g_students[0].marks;
    float lowestMark = g_students[0].marks;
    int highestIndex = 0;
    int lowestIndex = 0;

    for (int i = 0; i < g_studentCount; i++) {
        totalMarks += g_students[i].marks;

        if (g_students[i].marks > highestMark) {
            highestMark = g_students[i].marks;
            highestIndex = i;
        }

        if (g_students[i].marks < lowestMark) {
            lowestMark = g_students[i].marks;
            lowestIndex = i;
        }
    }

    double averageMark = totalMarks / g_studentCount;

    printf("\n---------------- Summary Statistics ----------------------\n");
    printf("Total number of students: %d\n", g_studentCount);
    printf("Average mark:             %.2f\n", averageMark);
    printf("Highest mark:             %.1f (Student: %s, ID: %d)\n",
        g_students[highestIndex].marks,
        g_students[highestIndex].name,
        g_students[highestIndex].id);
    printf("Lowest mark:              %.1f (Student: %s, ID: %d)\n",
        g_students[lowestIndex].marks,
        g_students[lowestIndex].name,
        g_students[lowestIndex].id);
    printf("-----------------------------------------------------------\n");
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