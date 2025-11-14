#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h> //used for malloc, realloc, calloc, free, exit
#include <ctype.h>

#define MAX_STUDENTS 100 //only used for showAllSorted's local array
#define INITIAL_CAPACITY 10 //Initial size for our dynamic array

typedef struct {
    int id;
    char name[100];
    char programme[100];
    float marks;
} Student;

//functions
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



char currentFileName[100] = "";
char tempFileName[100] = "temp_workfile.txt";
int unsavedChanges = 0;
int fileLoaded = 0;


Student* g_students = NULL; // Pointer to our dynamic array of students
int g_studentCount = 0;     // How many students are *in* the array
int g_studentCapacity = 0;  // How much space is *allocated* for the array


// cleanup function frees all dynamic memory and removes temp files.
void cleanup() {
    if (g_students != NULL) {
        free(g_students);
        g_students = NULL;
    }
    remove(tempFileName); // Remove the temporary work file

    // Reset all global state
    g_studentCount = 0;
    g_studentCapacity = 0;
    fileLoaded = 0;
    unsavedChanges = 0;
    strcpy(currentFileName, "");
    printf("CMS: System cleaned up.\n");
}

//function loadgloabalstudents
void loadGlobalStudents() {
    // If memory is already allocated (e.g., from a previous file), free it first.
    if (g_students != NULL) {
        free(g_students);
        g_students = NULL;
        g_studentCount = 0;
        g_studentCapacity = 0;
    }

    FILE* file = fopen(tempFileName, "r");
    if (file == NULL) {
        printf("CMS: Error: Cannot open temp file to load students.\n");
        return;
    }

    char line[200];

    // Skip header line
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file) != NULL) {
        // Check if we need more memory
        if (g_studentCount == g_studentCapacity) {
            if (g_studentCapacity == 0) {
                // First allocation: Use calloc to get zero-initialized memory
                g_studentCapacity = INITIAL_CAPACITY;
                g_students = (Student*)calloc(g_studentCapacity, sizeof(Student));
            }
            else {
                // Double the capacity: Use realloc
                g_studentCapacity *= 2;
                Student* temp = (Student*)realloc(g_students, g_studentCapacity * sizeof(Student));
                if (temp == NULL) {
                    printf("CMS: CRITICAL ERROR: Could not reallocate memory! Aborting load.\n");
                    fclose(file);
                    cleanup();
                    exit(1); // Exit on memory failure
                }
                g_students = temp; // Point g_students to the new, larger memory block
            }
        }

        // Parse the line and load data into the g_students array
        if (sscanf(line, "%d,%99[^,],%99[^,],%f",
            &g_students[g_studentCount].id,
            g_students[g_studentCount].name,
            g_students[g_studentCount].programme,
            &g_students[g_studentCount].marks) == 4)
        {
            g_studentCount++; // Only increment if sscanf was successful
        }
    }

    fclose(file);
    printf("CMS: Loaded %d student records into memory.\n", g_studentCount);
}

//function resyncTempfile
void resyncTempFile() {
    FILE* file = fopen(tempFileName, "w"); // "w" mode to overwrite
    if (file == NULL) {
        printf("CMS: Error: Cannot rewrite temporary file.\n");
        return;
    }

    // Write the header
    fprintf(file, "ID,Name,Programme,Mark\n");

    // Write all students from memory back to the file
    for (int i = 0; i < g_studentCount; i++) {
        fprintf(file, "%d,%s,%s,%.1f\n",
            g_students[i].id,
            g_students[i].name,
            g_students[i].programme,
            g_students[i].marks);
    }

    fclose(file);
}



int compareIDDesc(const void* a, const void* b) {
    return ((Student*)b)->id - ((Student*)a)->id;
}

int compareMarksDesc(const void* a, const void* b) {
    float diff = ((Student*)b)->marks - ((Student*)a)->marks;
    return (diff > 0) - (diff < 0);
}


//sort function
void showAllSorted() {
    if (!fileLoaded) {
        printf("CMS: No file loaded. Please open a file first.\n");
        return;
    }

    if (g_studentCount == 0) {
        printf("CMS: No student records found.\n");
        return;
    }

    
    //allocate a temporary array to hold the copy
    Student* sortedArray = (Student*)malloc(g_studentCount * sizeof(Student));
    if (sortedArray == NULL) {
        printf("CMS: Error: Could not allocate memory for sorting.\n");
        return;
    }

    //Copy data from the global array to the temporary array
    memcpy(sortedArray, g_students, g_studentCount * sizeof(Student));

    printf("\nSort Options:\n");
    printf("Available sorts: 'ID' or 'Marks' (both descending)\n");
    printf("Enter your choice: ");

    char input[20];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("CMS: Error reading input.\n");
        free(sortedArray); 
        return;
    }

    input[strcspn(input, "\n")] = 0;
    for (int i = 0; input[i]; i++) {
        input[i] = tolower(input[i]);
    }

    char sortField[30];
    int sorted = 1;

    if (strcmp(input, "id") == 0) {
        qsort(sortedArray, g_studentCount, sizeof(Student), compareIDDesc);
        strcpy(sortField, "ID (Descending)");
    }
    else if (strcmp(input, "marks") == 0) {
        qsort(sortedArray, g_studentCount, sizeof(Student), compareMarksDesc);
        strcpy(sortField, "Marks (Descending)");
    }
    else {
        printf("CMS: Invalid choice '%s'. Displaying unsorted records.\n", input);
        strcpy(sortField, "Unsorted (Original Load Order)");
        sorted = 0;
        //'sortedArray' is already in the original load order
    }

    if (sorted) {
        printf("\nStudent Records Sorted by %s:\n", sortField);
    }
    else {
        printf("\nStudent Records (%s):\n", sortField);
    }

    printf("ID          Name                 Programme                Marks\n");
    printf("-------------------------------------------------------------\n");

    for (int i = 0; i < g_studentCount; i++) {
        printf("%-10d %-20s %-23s %.1f\n",
            sortedArray[i].id, sortedArray[i].name, sortedArray[i].programme, sortedArray[i].marks);
    }

    // 3. Free the temporary array
    free(sortedArray);
}

void initializeSystem() {
    printf("=== Class Management System (CMS) ===\n");
    printf("System initialized. Please open a data file to begin.\n");
}

//Shows main menu
void showMainMenu() {
    int choice;

    while (1) {
        printf("\nStudent Management System (Current File: %s | In Memory: %d)\n", currentFileName, g_studentCount);
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
        printf("9. Show Summary Statistics\n"); 
        printf("10. Exit\n\n");                 

        printf("Enter your choice (1-10): "); 
        if (scanf("%d", &choice) != 1) {
            printf("CMS: Invalid input. Please enter a number.\n");
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
                printf("\nCMS: You have unsaved changes! Are you sure you want to exit? (y/n): ");
                scanf(" %c", &confirm);
                while (getchar() != '\n');
                if (confirm == 'y' || confirm == 'Y') {
                    printf("CMS: Exiting without saving. All changes discarded.\n");
                }
                else {
                    continue; //go back to menu
                }
            }
            printf("CMS: Goodbye!\n");
            cleanup(); 
            exit(0);
        default:
            printf("CMS: Invalid choice. Please enter a number between 1-10.\n"); 
            break;
        }
    }
}


//main function
int main() {
    
    printf("Declaration\n");
    printf("Declared by: P7_4\n");
    printf("Date: 25 November 2025\n\n");

    initializeSystem();

    int choice;
    while (!fileLoaded) {
        printf("\n--- Main Menu ---\n");
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
                showMainMenu(); // This function now has its own exit path
            }
            break;
        case 2:
            printf("CMS: Goodbye!\n");
            cleanup();//Calls cleanup before exiting
            return 0;
        default:
            printf("CMS: Invalid choice. Please enter 1 or 2.\n");
            break;
        }
    }

    // This part should not be reached, but just in case:
    cleanup();
    return 0;
}

//function view record.
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

    // Loop through the in-memory array
    for (int i = 0; i < g_studentCount; i++) {
        printf("%-10d %-20s %-23s %.1f\n",
            g_students[i].id,
            g_students[i].name,
            g_students[i].programme,
            g_students[i].marks);
    }
}

//function save.

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


//function open file
void openFile() {
    if (unsavedChanges && fileLoaded) {
        char confirm;
        printf("\nCMS: You have unsaved changes! Are you sure you want to open a new file? (y/n): ");
        scanf(" %c", &confirm);
        while (getchar() != '\n');
        if (!(confirm == 'y' || confirm == 'Y')) {
            return;
        }
        // If 'y', proceed to open, which will call cleanup()
    }

   
    if (fileLoaded) {
        cleanup(); //Free old g_students remove old temp file
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


    loadGlobalStudents();

    printf("CMS: Successfully opened file '%s'\n", filename);
    viewRecords(); //Show records from memory
}

//function check duplicate.
int checkDuplicateID(int newID) {
    if (!fileLoaded) {
        return 0;
    }

    // Loop through the in-memory array
    for (int i = 0; i < g_studentCount; i++) {
        if (g_students[i].id == newID) {
            return 1; // Found
        }
    }

    return 0; // Not found
}

//Function insert.
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


    //Check if we need to reallocate
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
                return; // Don't exit, just fail the insertion
            }
            g_students = temp;
        }
    }

    //Add the new student to the in-memory array
    g_students[g_studentCount].id = newID;
    strcpy(g_students[g_studentCount].name, name);
    strcpy(g_students[g_studentCount].programme, programme);
    g_students[g_studentCount].marks = marks;
    g_studentCount++; // Increment the count

    
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
    viewRecords(); // Show updated list from memory
}

//Function query.
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
   //Loop through the in-memory array
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
            break;//Stop searching once found
        }
    }

    if (!found) {
        printf("CMS: The record with ID=%d does not exist.\n", searchId);
    }
}

//function update.
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

//Find the student in the in-memory array
    int foundIndex = -1;
    for (int i = 0; i < g_studentCount; i++) {
        if (g_students[i].id == updateId) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1) {
        printf("CMS: The record with ID=%d does not exist.\n", updateId);
        return;
    }

 //Get new data
    char name[100], programme[100];
    float marks;
    char confirmation;
    char input[500];

    printf("Record found. ID %d: %s, %s, %.1f\n", updateId,
        g_students[foundIndex].name,
        g_students[foundIndex].programme,
        g_students[foundIndex].marks);
    printf("Enter new data (Format: Name=\"Name\", Programme=\"Prog\", Marks=\"0.0\") or press Enter to cancel: ");

    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("CMS: Error reading input!\n");
        return;
    }
    input[strcspn(input, "\n")] = 0;

    if (strlen(input) == 0) {
        printf("CMS: Update cancelled. Original record for ID=%d was kept.\n", updateId);
        return;
    }

    if (sscanf(input, "Name=\"%99[^\"]\", Programme=\"%99[^\"]\", Marks=\"%f\"",
        name, programme, &marks) != 3) {
        printf("CMS: Error: Invalid input format!\n");
        printf("CMS: Update cancelled. Original record for ID=%d was kept.\n", updateId);
        return;
    }

    printf("\n--- Review Changes ---\n");
    printf("OLD: %s, %s, %.1f\n",
        g_students[foundIndex].name,
        g_students[foundIndex].programme,
        g_students[foundIndex].marks);
    printf("NEW: %s, %s, %.1f\n", name, programme, marks);
    printf("Are you sure you want to save these changes? (Y/N): ");

    scanf(" %c", &confirmation);
    while (getchar() != '\n');

  //Update in memory and resync file
    if (confirmation == 'Y' || confirmation == 'y') {
        // Update the in-memory array
        strcpy(g_students[foundIndex].name, name);
        strcpy(g_students[foundIndex].programme, programme);
        g_students[foundIndex].marks = marks;

      // Rewrite the entire temp file from memory
        resyncTempFile();

        printf("CMS: Record for ID=%d was successfully updated.\n", updateId);
        unsavedChanges = 1;
        printf("CMS: Use 'Save Records' to make changes permanent.\n");
        viewRecords();
    }
    else {
        printf("CMS: Update cancelled. Original record for ID=%d was kept.\n", updateId);
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

  //Find the student in the in-memory array
    int foundIndex = -1;
    for (int i = 0; i < g_studentCount; i++) {
        if (g_students[i].id == targetID) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1) {
        printf("CMS: The record with ID=%d does not exist.\n", targetID);
        return;
    }

   //Confirm deletion
    printf("CMS: Are you sure you want to delete record with ID=%d? ", targetID);
    printf("Type \"Y\" to Confirm or type \"N\" to cancel.\n");
    printf("P1_1: ");

    char confirm;
    if (scanf(" %c", &confirm) != 1) {
        confirm = 'N'; //Default to 'N' on bad input
    }
    while (getchar() != '\n');

    //Delete from memory and resync file
    if (confirm == 'Y' || confirm == 'y') {
    //Delete from memory by shifting all subsequent elements left
        for (int i = foundIndex; i < g_studentCount - 1; i++) {
            g_students[i] = g_students[i + 1]; //Struct copy
        }
        g_studentCount--; //Reduce the count

        resyncTempFile();

        printf("CMS: The record with ID=%d is successfully deleted.\n", targetID);
        unsavedChanges = 1;
        printf("CMS: Use 'Save Records' to make changes permanent.\n");
        viewRecords();
    }
    else {
        printf("CMS: The deletion is cancelled.\n");
    }
}
//Summary Function
void showSummaryStatistics() {
    if (!fileLoaded) {
        printf("CMS: No file loaded. Please open a file first.\n");
        return;
    }

    // Guard clause: Check if there are any students
    if (g_studentCount == 0) {
        printf("CMS: No student records found. Cannot generate summary.\n");
        return;
    }

    // --- 1. Initialize variables ---
    double totalMarks = 0.0;

    // Start by assuming the first student has the highest/lowest mark
    float highestMark = g_students[0].marks;
    float lowestMark = g_students[0].marks;

    // Store the *index* of the student, which is safer
    int highestIndex = 0;
    int lowestIndex = 0;

    //Loop through the in memory array
    for (int i = 0; i < g_studentCount; i++) {
        // Add to total for average
        totalMarks += g_students[i].marks;

        // Check for new highest
        if (g_students[i].marks > highestMark) {
            highestMark = g_students[i].marks;
            highestIndex = i;
        }

        // Check for new lowest
        if (g_students[i].marks < lowestMark) {
            lowestMark = g_students[i].marks;
            lowestIndex = i;
        }
    }

    //calculate then display the result.
    double averageMark = totalMarks / g_studentCount;

    printf("\n---------------- Summary Statistics ----------------------\n");
    printf("Total number of students: %d\n", g_studentCount);
    printf("Average mark:             %.2f\n", averageMark); // Show 2 decimal places for average
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


// --- Export to CSV (Unchanged) ---
void exportToCSV() {
    if (!fileLoaded) {
        printf("CMS: No file loaded. Please open a file first.\n");
        return;
    }
    // ... (rest of function is unchanged and correct) ...

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