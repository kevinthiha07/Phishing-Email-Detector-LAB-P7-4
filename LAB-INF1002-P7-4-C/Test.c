/***************************************************************
 * Declaration
 * SIT’s policy on copying does not allow the students to copy
 * source code as well as assessment solutions from another
 * person AI or other places. It is the students’ responsibility
 * to guarantee that their assessment solutions are their own
 * work. Meanwhile, the students must also ensure that their
 * work is not accessible by others. Where such plagiarism is
 * detected, both of the assessments involved will receive ZERO
 * mark.
 *
 * We hereby declare that:
 * • We fully understand and agree to the abovementioned plagiarism policy.
 * • We did not copy any code from others or from other places.
 * • We did not share our codes with others or upload to any other places for public access and will not do that in the future.
 * • We agree that our project will receive Zero mark if there is any plagiarism detected.
 * • We agree that we will not disclose any information or material of the group project to others or upload to any other places for public access.
 * • We agree that we did not copy any code directly from AI generated sources
 *
 * Declared by: P7_4
 * Team members:
 * 1. XXX
 * 2. XXX
 * 3. XXX
 * 4. XXX
 * 5. XXX
 * Date: 24 November 2025
 ***************************************************************/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef struct {
    int id;
    char name[100];
    char programme[100];
    float mark;
} Student;

Student records[100];
int count = 0;
int unsaved_changes = 0;
int file_opened = 0; // Track if file has been opened

// Helper function to convert a string to lowercase
void to_lowercase(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

// Helper function to trim leading spaces from a string
void trim_leading_spaces(char* str) {
    if (str == NULL || *str == '\0') {
        return;
    }
    char* start = str;
    while (*start == ' ') {
        start++;
    }
    if (start != str) {
        // Move the non-space characters to the start of the string
        memmove(str, start, strlen(start) + 1);
    }
}

/**
 * NEW HELPER FUNCTION: Checks if the first two digits of the ID are valid (21-25).
 */
int is_valid_id_prefix(const char* id_str) {
    if (strlen(id_str) < 2) {
        return 0; // Not enough digits
    }
    char prefix_str[3];
    strncpy(prefix_str, id_str, 2);
    prefix_str[2] = '\0';
    int prefix = atoi(prefix_str);

    // Check if the prefix is in the valid range [21, 25]
    if (prefix >= 21 && prefix <= 25) {
        return 1;
    }
    return 0;
}

void load_data_from_file() {
    FILE* file = fopen("Team_P7_4-CMS.txt", "r");
    if (!file) {
        printf("Error: Cannot open Team_P7_4-CMS.txt\n");
        return;
    }

    char line[500];

    // Skip header line
    if (fgets(line, sizeof(line), file)) {
        // Header found
    }

    // Read data lines
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        if (strlen(line) == 0) continue;

        int id;
        char name[100] = "";
        char programme[100] = "";
        float mark;

        // Parse CSV format
        char* token;
        int field = 0;
        char temp_line[500];
        strcpy(temp_line, line);

        token = strtok(temp_line, ",");
        while (token != NULL) {
            // Remove leading spaces
            while (*token == ' ') token++;
            // Remove trailing spaces
            char* end = token + strlen(token) - 1;
            while (end > token && *end == ' ') *end-- = '\0';

            switch (field) {
            case 0: id = atoi(token); break;
            case 1: strcpy(name, token); break;
            case 2: strcpy(programme, token); break;
            case 3: mark = atof(token); break;
            }

            token = strtok(NULL, ",");
            field++;
        }

        if (field == 4) {
            records[count].id = id;
            strcpy(records[count].name, name);
            strcpy(records[count].programme, programme);
            records[count].mark = mark;
            count++;
        }
    }

    fclose(file);
    file_opened = 1; // Mark file as opened
}

void save_data_to_file() {
    if (!file_opened) {
        printf("CMS: Error - No file opened. Use OPEN command first.\n");
        return;
    }

    FILE* file = fopen("Team_P7_4-CMS.txt", "w");
    if (!file) {
        printf("Error: Cannot save to Team_P7_4-CMS.txt\n");
        return;
    }

    fprintf(file, "ID,Name,Programme,Mark\n");
    for (int i = 0; i < count; i++) {
        fprintf(file, "%d,%s,%s,%.1f\n",
            records[i].id, records[i].name,
            records[i].programme, records[i].mark);
    }

    fclose(file);
    unsaved_changes = 0;
    printf("CMS: All changes saved successfully.\n");
}

void find_column_widths(int* id_width, int* name_width, int* prog_width, int* mark_width) {
    *id_width = 2;
    *name_width = 4;
    *prog_width = 8;
    *mark_width = 4;

    for (int i = 0; i < count; i++) {
        char temp[50];
        sprintf(temp, "%d", records[i].id);
        int id_len = strlen(temp);
        int name_len = strlen(records[i].name);
        int prog_len = strlen(records[i].programme);
        sprintf(temp, "%.1f", records[i].mark);
        int mark_len = strlen(temp);

        if (id_len > *id_width) *id_width = id_len;
        if (name_len > *name_width) *name_width = name_len;
        if (prog_len > *prog_width) *prog_width = prog_len;
        if (mark_len > *mark_width) *mark_width = mark_len;
    }

    *name_width += 2;
    *prog_width += 2;
}

void show_all() {
    if (!file_opened) {
        printf("CMS: Error - No file opened. Use OPEN command first.\n");
        return;
    }

    if (count == 0) {
        printf("CMS: No records found.\n");
        return;
    }

    printf("CMS: Here are all the records:\n\n");

    int id_width, name_width, prog_width, mark_width;
    find_column_widths(&id_width, &name_width, &prog_width, &mark_width);

    printf("%-*s  %-*s  %-*s  %s\n",
        id_width, "ID", name_width, "Name",
        prog_width, "Programme", "Mark");

    int total_width = id_width + name_width + prog_width + mark_width + 10;
    for (int i = 0; i < total_width; i++) printf("=");
    printf("\n");

    for (int i = 0; i < count; i++) {
        printf("%-*d  %-*s  %-*s  %.1f\n",
            id_width, records[i].id,
            name_width, records[i].name,
            prog_width, records[i].programme,
            records[i].mark);
    }

    printf("\nTotal records: %d\n", count);
    if (unsaved_changes) {
        printf("⚠️  Unsaved changes - Use 'SAVE' to save\n");
    }
}

void capitalize_words(char* str) {
    int capitalize_next = 1;
    for (int i = 0; str[i]; i++) {
        if (capitalize_next && isalpha(str[i])) {
            str[i] = toupper(str[i]);
            capitalize_next = 0;
        }
        else if (str[i] == ' ') {
            capitalize_next = 1;
        }
        else if (isalpha(str[i])) {
            str[i] = tolower(str[i]);
        }
    }
}

int find_student_index(int id) {
    for (int i = 0; i < count; i++) {
        if (records[i].id == id) return i;
    }
    return -1;
}

int is_duplicate_data(char* name, char* programme, float mark) {
    for (int i = 0; i < count; i++) {
        if (strcmp(records[i].name, name) == 0 &&
            strcmp(records[i].programme, programme) == 0 &&
            records[i].mark == mark) {
            return 1;
        }
    }
    return 0;
}

// Helper function to extract and clean values (used in insert_record)
void extract_and_clean(char* start_ptr, char* end_ptr, char* destination) {
    if (end_ptr == NULL) end_ptr = start_ptr + strlen(start_ptr); // Read to end if no terminator
    int len = end_ptr - start_ptr;
    if (len < 0) len = 0; // Safety check

    // Copy only the required length
    strncpy(destination, start_ptr, len);
    destination[len] = '\0';

    // Trim leading spaces
    trim_leading_spaces(destination);

    // Trim trailing spaces/commas (though comma shouldn't be an issue with key=value format)
    int i = strlen(destination) - 1;
    while (i >= 0 && (destination[i] == ' ' || destination[i] == ',')) {
        destination[i] = '\0';
        i--;
    }
}

// Helper function to find the next valid field start point after a given key
char* find_next_key(char* current_key_start, char* base_input) {
    char* next_key = NULL;
    char* name_end = strstr(current_key_start + 1, "Name=");
    char* prog_end = strstr(current_key_start + 1, "Programme=");
    char* mark_end = strstr(current_key_start + 1, "Mark=");
    char* id_end = strstr(current_key_start + 1, "ID=");

    // Find the earliest occurrence of another key after the current key's starting position
    if (name_end != NULL) next_key = (next_key == NULL || name_end < next_key) ? name_end : next_key;
    if (prog_end != NULL) next_key = (next_key == NULL || prog_end < next_key) ? prog_end : next_key;
    if (mark_end != NULL) next_key = (next_key == NULL || mark_end < next_key) ? mark_end : next_key;
    if (id_end != NULL) next_key = (next_key == NULL || id_end < next_key) ? id_end : next_key;

    return next_key;
}

/**
 * FIX: Modified to accept the ID from the command line argument string and remove the prompt.
 * Format: "QUERY ID=2401234"
 */
void query_record(char* input) {
    if (!file_opened) {
        printf("CMS: Error - No file opened. Use OPEN command first.\n");
        return;
    }

    // Check if ID was provided
    if (strlen(input) == 0) {
        printf("CMS: Error - Missing ID. Format: QUERY ID=7-digits\n");
        return;
    }

    char temp_input[300];
    strcpy(temp_input, input);

    // --- Extract ID ---
    char* id_ptr = strstr(temp_input, "ID=");
    if (!id_ptr) {
        printf("CMS: Error - Format must include 'ID=7-digits'.\n");
        return;
    }
    id_ptr += 3; // Move past "ID="

    char id_str[8] = "";
    // Extract exactly 7 digits
    for (int i = 0; i < 7 && isdigit(id_ptr[i]); i++) {
        id_str[i] = id_ptr[i];
    }
    id_str[7] = '\0';

    if (strlen(id_str) != 7) {
        printf("CMS: Error - ID must be 7 digits.\n");
        return;
    }

    // Check for non-digit characters immediately following the 7 digits (only if ID is not the last argument)
    if (strlen(id_ptr) > 7 && !isspace(id_ptr[7]) && id_ptr[7] != '\0') {
        printf("CMS: Error - ID must be digits only.\n");
        return;
    }


    int id = atoi(id_str);
    int index = find_student_index(id);

    if (index == -1) {
        printf("CMS: Record with ID=%d not found.\n", id);
        return;
    }

    printf("CMS: Record found:\n");
    printf("ID: %d\n", records[index].id);
    printf("Name: %s\n", records[index].name);
    printf("Programme: %s\n", records[index].programme);
    printf("Mark: %.1f\n", records[index].mark);
}

/**
 * FIX: Modified to accept the ID from the command line argument string and remove the prompt.
 * Format: "DELETE ID=2401234"
 */
void delete_record(char* input) {
    if (!file_opened) {
        printf("CMS: Error - No file opened. Use OPEN command first.\n");
        return;
    }

    // Check if ID was provided
    if (strlen(input) == 0) {
        printf("CMS: Error - Missing ID. Format: DELETE ID=7-digits\n");
        return;
    }

    char temp_input[300];
    strcpy(temp_input, input);

    // --- Extract ID ---
    char* id_ptr = strstr(temp_input, "ID=");
    if (!id_ptr) {
        printf("CMS: Error - Format must include 'ID=7-digits'.\n");
        return;
    }
    id_ptr += 3; // Move past "ID="

    char id_str[8] = "";
    // Extract exactly 7 digits
    for (int i = 0; i < 7 && isdigit(id_ptr[i]); i++) {
        id_str[i] = id_ptr[i];
    }
    id_str[7] = '\0';

    if (strlen(id_str) != 7) {
        printf("CMS: Error - ID must be 7 digits.\n");
        return;
    }

    // Check for non-digit characters immediately following the 7 digits (only if ID is not the last argument)
    if (strlen(id_ptr) > 7 && !isspace(id_ptr[7]) && id_ptr[7] != '\0') {
        printf("CMS: Error - ID must be digits only.\n");
        return;
    }

    int id = atoi(id_str);
    int index = find_student_index(id);

    if (index == -1) {
        printf("CMS: Error - ID %d not found.\n", id);
        return;
    }

    printf("\nRecord to delete:\n");
    printf("ID: %d, Name: %s, Programme: %s, Mark: %.1f\n",
        records[index].id, records[index].name,
        records[index].programme, records[index].mark);

    printf("Confirm deletion? (yes/no): ");
    char response[10];
    fgets(response, sizeof(response), stdin);
    response[strcspn(response, "\n")] = 0;

    for (int i = 0; response[i]; i++) {
        response[i] = tolower(response[i]);
    }

    if (strcmp(response, "yes") == 0 || strcmp(response, "y") == 0) {
        for (int i = index; i < count - 1; i++) {
            records[i] = records[i + 1];
        }
        count--;
        unsaved_changes = 1;
        printf("CMS: Record deleted successfully.\n");
    }
    else {
        printf("CMS: Deletion cancelled.\n");
    }
}


/**
 * FIX: Modified to accept the arguments from the command line and remove all prompts.
 * Format: "ID=2401234 Name=Michelle Lee Programme=Information Security Mark=73.2"
 */
void insert_record(char* input) {
    if (!file_opened) {
        printf("CMS: Error - No file opened. Use OPEN command first.\n");
        return;
    }

    if (count >= 100) {
        printf("CMS: Error - Database full.\n");
        return;
    }

    // Check if input arguments were provided
    if (strlen(input) == 0) {
        printf("CMS: Error - Missing insert data. Format: INSERT ID=... Name=... Programme=... Mark=...\n");
        return;
    }

    char temp_input[300];
    strcpy(temp_input, input);

    char name[100] = "", programme[100] = "";
    float mark = -1.0;
    int id = 0;

    // --- 1. Extract ID ---
    char* id_ptr = strstr(temp_input, "ID=");
    if (!id_ptr) {
        printf("CMS: Error - Missing required field: ID.\n");
        return;
    }
    id_ptr += 3; // Move past "ID="

    char id_str[8] = "";
    // Loop to extract ID characters (stop at first non-digit or end of string)
    int i = 0;
    while (i < 7 && isdigit(id_ptr[i])) {
        id_str[i] = id_ptr[i];
        i++;
    }
    id_str[i] = '\0'; // Null-terminate at the end of the digits found

    if (strlen(id_str) != 7) {
        printf("CMS: Error - ID must be 7 digits.\n");
        return;
    }

    // **NEW VALIDATION: Check ID Prefix**
    if (!is_valid_id_prefix(id_str)) {
        printf("CMS: Error - ID prefix must be 21, 22, 23, 24, or 25.\n");
        return;
    }
    // **END NEW VALIDATION**

    id = atoi(id_str);

    if (find_student_index(id) != -1) {
        printf("CMS: Error - ID %d already exists.\n", id);
        return;
    }

    // --- 2. Extract Name, Programme, Marks ---
    char* name_start = strstr(temp_input, "Name=");
    char* prog_start = strstr(temp_input, "Programme=");
    char* mark_start = strstr(temp_input, "Mark=");

    if (!name_start || !prog_start || !mark_start) {
        printf("CMS: Error - Missing one or more required fields (Name, Programme, Mark).\n");
        return;
    }

    // 1. Extract Name
    if (name_start) {
        char* name_val_start = name_start + 5;
        char* name_stop = find_next_key(name_start, temp_input);
        extract_and_clean(name_val_start, name_stop, name);
    }

    // 2. Extract Programme
    if (prog_start) {
        char* prog_val_start = prog_start + 10;
        char* prog_stop = find_next_key(prog_start, temp_input);
        extract_and_clean(prog_val_start, prog_stop, programme);
    }

    // 3. Extract Marks
    // Mark= is 5 characters
    if (mark_start && sscanf(mark_start + 5, "%f", &mark) != 1) {
        printf("CMS: Error - Mark field missing or invalid.\n");
        return;
    }

    // Final checks
    if (strlen(name) == 0) {
        printf("CMS: Error - Name value missing.\n");
        return;
    }
    if (strlen(programme) == 0) {
        printf("CMS: Error - Programme value missing.\n");
        return;
    }
    if (mark < 0.0 || mark > 100.0) {
        printf("CMS: Error - Marks must be 0-100.\n");
        return;
    }

    // --- Capitalization ---
    capitalize_words(name);
    capitalize_words(programme);

    // --- Validation and Insertion ---
    if (is_duplicate_data(name, programme, mark)) {
        printf("CMS: Error - Duplicate record exists.\n");
        return;
    }

    records[count].id = id;
    strcpy(records[count].name, name);
    strcpy(records[count].programme, programme);
    records[count].mark = mark;
    count++;
    unsaved_changes = 1;
    printf("CMS: Record inserted successfully.\n");
}


/**
 * update_record function signature:
 * Accepts the entire command line *after* the "UPDATE" keyword.
 * Example: "ID=2401234 Mark=69.8"
 */
void update_record(char* input) {
    if (!file_opened) {
        printf("CMS: Error - No file opened. Use OPEN command first.\n");
        return;
    }

    char temp_input[300];
    strcpy(temp_input, input);

    // 1. Extract ID
    char* id_ptr = strstr(temp_input, "ID=");
    if (!id_ptr) {
        printf("CMS: Error - Format must include 'ID=7-digits' and an updated field.\n");
        return;
    }
    id_ptr += 3; // Move past "ID="

    char id_str[8] = "";
    // Extract exactly 7 digits
    for (int i = 0; i < 7 && isdigit(id_ptr[i]); i++) {
        id_str[i] = id_ptr[i];
    }
    id_str[7] = '\0';

    if (strlen(id_str) != 7) {
        printf("CMS: Error - ID must be 7 digits.\n");
        return;
    }

    // **NEW VALIDATION: Check ID Prefix**
    // NOTE: For UPDATE, we only need to check the prefix if we were allowing ID to be updated,
    // but the ID field itself is used for locating the record and is typically not updated.
    // However, if the logic were to allow an ID update (which is not implemented here), 
    // the check would be necessary. We skip the prefix check here since the ID is only
    // used to find an existing record.
    // We will leave the prefix check out of UPDATE as per common database logic (ID is key), 
    // but if the intent was to ensure IDs *already in the system* satisfy the rule (which they should 
    // if INSERT enforces it), that would be a separate validation on load.
    // For now, we only apply the requested check to the INSERT command.

    int id = atoi(id_str);
    int index = find_student_index(id);

    if (index == -1) {
        printf("CMS: The record with ID=%d does not exist.\n", id);
        return;
    }

    // 2. Identify and extract the updated field
    char* mark_ptr = strstr(temp_input, "Mark=");
    char* name_ptr = strstr(temp_input, "Name=");
    char* prog_ptr = strstr(temp_input, "Programme=");

    if (!mark_ptr && !name_ptr && !prog_ptr) {
        printf("CMS: Error - Must specify a field to update (Mark, Name, or Programme).\n");
        return;
    }

    // Preserve old data for duplicate check and error revert
    char new_name[100], new_prog[100];
    float new_mark;

    strcpy(new_name, records[index].name);
    strcpy(new_prog, records[index].programme);
    new_mark = records[index].mark;

    int update_made = 0;

    // --- Update Mark ---
    if (mark_ptr) {
        mark_ptr += 5; // Move past "Mark="
        float mark_val;
        if (sscanf(mark_ptr, "%f", &mark_val) == 1) {
            if (mark_val >= 0 && mark_val <= 100) {
                new_mark = mark_val;
                update_made = 1;
                printf("CMS: The record with ID=%d is successfully updated.\n", id);
            }
            else {
                printf("CMS: Error - Mark must be between 0 and 100.\n");
                return;
            }
        }
        else {
            printf("CMS: Error - Invalid Mark value.\n");
            return;
        }
    }

    // --- Update Programme ---
    if (prog_ptr) {
        prog_ptr += 10; // Move past "Programme="
        char* prog_val_start = prog_ptr;
        char temp_prog[100];

        char* prog_stop = find_next_key(prog_ptr - 10, temp_input); // find_next_key needs the start of the key-value pair, -10 moves back to "Programme="
        extract_and_clean(prog_val_start, prog_stop, temp_prog);

        if (strlen(temp_prog) > 0) {
            capitalize_words(temp_prog);
            strcpy(new_prog, temp_prog);
            update_made = 1;
            printf("CMS: The record with ID=%d is successfully updated.\n", id);
        }
        else {
            printf("CMS: Error - Invalid Programme value.\n");
            return;
        }
    }

    // --- Update Name ---
    if (name_ptr) {
        name_ptr += 5; // Move past "Name="
        char* name_val_start = name_ptr;
        char temp_name[100];

        char* name_stop = find_next_key(name_ptr - 5, temp_input); // find_next_key needs the start of the key-value pair, -5 moves back to "Name="
        extract_and_clean(name_val_start, name_stop, temp_name);

        if (strlen(temp_name) > 0) {
            capitalize_words(temp_name);
            strcpy(new_name, temp_name);
            update_made = 1;
            printf("CMS: The record with ID=%d is successfully updated.\n", id);
        }
        else {
            printf("CMS: Error - Invalid Name value.\n");
            return;
        }
    }

    if (!update_made) {
        // This case should be mostly covered by the earlier checks, but as a fallback:
        printf("CMS: No valid fields were provided for update.\n");
        return;
    }

    // 3. Final Validation (Duplicate Check)
    // Temporarily apply the update for duplicate check
    Student original_record = records[index]; // Save original
    strcpy(records[index].name, new_name);
    strcpy(records[index].programme, new_prog);
    records[index].mark = new_mark;

    // Check for duplicates (excluding the current record's original data)
    int is_dup = 0;
    for (int i = 0; i < count; i++) {
        if (i != index && strcmp(records[i].name, new_name) == 0 &&
            strcmp(records[i].programme, new_prog) == 0 &&
            records[i].mark == new_mark) {
            is_dup = 1;
            break;
        }
    }

    if (is_dup) {
        // Revert changes if duplicate found
        records[index] = original_record;
        printf("CMS: Error - Duplicate record exists after update.\n");
        return;
    }

    // The update is applied; set the flag
    unsaved_changes = 1;
}


void sort_by_id() {
    if (!file_opened) {
        printf("CMS: Error - No file opened. Use OPEN command first.\n");
        return;
    }

    if (count == 0) {
        printf("No records to sort.\n");
        return;
    }

    // Bubble sort by ID (ascending)
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (records[j].id > records[j + 1].id) {
                Student temp = records[j];
                records[j] = records[j + 1];
                records[j + 1] = temp;
            }
        }
    }
    unsaved_changes = 1;
    printf("Records sorted by ID.\n");
}

void sort_by_marks() {
    if (!file_opened) {
        printf("CMS: Error - No file opened. Use OPEN command first.\n");
        return;
    }

    if (count == 0) {
        printf("No records to sort.\n");
        return;
    }

    // Bubble sort by marks (descending)
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (records[j].mark < records[j + 1].mark) {
                Student temp = records[j];
                records[j] = records[j + 1];
                records[j + 1] = temp;
            }
        }
    }
    unsaved_changes = 1;
    printf("Records sorted by marks.\n");
}

void show_help() {
    printf("\n=== Available Commands ===\n");
    if (!file_opened) {
        printf("OPEN          - Open the student records file\n");
    }
    printf("SHOW ALL    - Display all student records\n");
    printf("QUERY ID=...  - Search for a record by ID\n");
    printf("INSERT ID=... Name=... Programme=... Mark=... - Add a new record\n");
    printf("UPDATE ID=... Mark/Name/Programme=... - Update an existing record\n");
    printf("DELETE ID=... - Delete a record by ID (prompts for confirmation)\n");
    printf("SORT ID     - Sort records by ID (ascending)\n");
    printf("SORT MARKS  - Sort records by marks (descending)\n");
    printf("SAVE        - Save all changes to file\n");
    printf("HELP        - Show this help message\n");
    printf("EXIT        - Quit the program\n");
    printf("==========================\n\n");
}

int confirm_exit() {
    if (unsaved_changes) {
        printf("\n⚠️  Unsaved changes! Exit without saving? (yes/no): ");
        char response[10];
        fgets(response, sizeof(response), stdin);
        response[strcspn(response, "\n")] = 0;

        for (int i = 0; response[i]; i++) {
            response[i] = tolower(response[i]);
        }

        if (strcmp(response, "yes") == 0 || strcmp(response, "y") == 0) {
            return 1;
        }
        else {
            printf("Exit cancelled.\n");
            return 0;
        }
    }
    return 1;
}

void open_file() {
    if (file_opened) {
        printf("CMS: File is already opened.\n");
        return;
    }

    load_data_from_file();
    if (file_opened) {
        printf("CMS: Team_P7_4-CMS.txt opened successfully.\n");
        printf("CMS: %d records loaded.\n", count);
    }
}

int main() {
    char command_line[300];
    char command[50];
    char lower_command[50];

    printf("Student Records System started.\n\n");
    printf("Type HELP for a list of available commands\n\n");

    while (1) {
        printf("P7_4: ");
        // Read the entire line of input
        fgets(command_line, sizeof(command_line), stdin);
        command_line[strcspn(command_line, "\n")] = 0;

        // Use a temporary copy for tokenizing (since strtok modifies the string)
        char temp_line[300];
        strcpy(temp_line, command_line);

        // --- 1. Extract the Command Token ---
        char* token = strtok(temp_line, " ");
        if (token == NULL) {
            printf("\n");
            continue; // Empty line
        }

        // Save the command and convert to lowercase
        strcpy(command, token);
        strcpy(lower_command, command);
        to_lowercase(lower_command);

        // --- 2. Find the start of the Arguments in the original line ---
        char* args_start = command_line + strlen(command);
        // Skip any immediate spaces after the command word
        while (*args_start == ' ') {
            args_start++;
        }

        // --- 3. Command Handling ---
        if (strcmp(lower_command, "open") == 0) {
            open_file();
        }
        else if (strcmp(lower_command, "show") == 0) {
            if (strcmp(args_start, "all") == 0) {
                show_all();
            }
            else {
                printf("Unknown command. Type HELP for available commands.\n");
            }
        }
        else if (strcmp(lower_command, "query") == 0) {
            // Pass arguments directly to query_record
            query_record(args_start);
        }
        else if (strcmp(lower_command, "insert") == 0) {
            // Pass arguments directly to insert_record
            insert_record(args_start);
        }
        else if (strcmp(lower_command, "update") == 0) {
            if (strlen(args_start) > 0) {
                update_record(args_start);
            }
            else {
                printf("CMS: Error - Usage: UPDATE ID=... Mark/Name/Programme=...\n");
            }
        }
        else if (strcmp(lower_command, "delete") == 0) {
            // Pass arguments directly to delete_record
            delete_record(args_start);
        }
        else if (strcmp(lower_command, "sort") == 0) {
            char sort_key[10];
            if (sscanf(args_start, "%s", sort_key) == 1) {
                to_lowercase(sort_key);

                if (strcmp(sort_key, "id") == 0) {
                    sort_by_id();
                }
                else if (strcmp(sort_key, "marks") == 0) {
                    sort_by_marks();
                }
                else {
                    printf("Unknown sort key. Use 'SORT ID' or 'SORT MARKS'.\n");
                }
            }
            else {
                printf("Unknown sort key. Use 'SORT ID' or 'SORT MARKS'.\n");
            }
        }
        else if (strcmp(lower_command, "save") == 0) {
            save_data_to_file();
        }
        else if (strcmp(lower_command, "help") == 0) {
            show_help();
        }
        else if (strcmp(lower_command, "exit") == 0) {
            if (confirm_exit()) {
                printf("Goodbye!\n");
                break;
            }
        }
        else {
            printf("Unknown command. Type HELP for available commands.\n");
        }

        printf("\n");
    }

    return 0;
}