

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <float.h> // Required for FLT_MAX and FLT_MIN

typedef struct {
    int id;
    char name[100];
    char programme[100];
    float mark;
} Student;

// --- GLOBAL VARIABLES (Dynamic) ---
Student* records = NULL; // Pointer for dynamic array
int count = 0;           // Current number of records
int capacity = 0;        // Current allocated size
int unsaved_changes = 0;
int file_opened = 0;

// --- MEMORY MANAGEMENT FUNCTIONS ---

/* Initialize memory using calloc */
void init_system() {
    capacity = 10; // Initial capacity
    records = (Student*)calloc(capacity, sizeof(Student));
    if (records == NULL) {
        printf("System Error: Memory allocation failed on startup.\n");
        exit(1);
    }
}

/* Resize memory using realloc if array is full */
void ensure_capacity() {
    if (count >= capacity) {
        int new_capacity = capacity * 2;
        Student* temp = (Student*)realloc(records, new_capacity * sizeof(Student));

        if (temp == NULL) {
            printf("System Error: Memory reallocation failed. Database full.\n");
            return;
        }

        records = temp;
        capacity = new_capacity;
    }
}

/* Free memory before exit */
void cleanup() {
    if (records != NULL) {
        free(records);
        records = NULL;
    }
    printf("System cleanup completed. Memory freed.\n");
}

// --- UTILITY FUNCTIONS ---

void to_lowercase(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

void trim_leading_spaces(char* str) {
    if (str == NULL || *str == '\0') return;
    char* start = str;
    while (*start == ' ') start++;
    if (start != str) memmove(str, start, strlen(start) + 1);
}

int is_valid_id_prefix(const char* id_str) {
    if (strlen(id_str) < 2) return 0;
    char prefix_str[3];
    strncpy(prefix_str, id_str, 2);
    prefix_str[2] = '\0';
    int prefix = atoi(prefix_str);
    if (prefix >= 21 && prefix <= 25) return 1;
    return 0;
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

void extract_and_clean(char* start_ptr, char* end_ptr, char* destination) {
    if (end_ptr == NULL) end_ptr = start_ptr + strlen(start_ptr);
    int len = end_ptr - start_ptr;
    if (len < 0) len = 0;
    strncpy(destination, start_ptr, len);
    destination[len] = '\0';
    trim_leading_spaces(destination);
    int i = strlen(destination) - 1;
    while (i >= 0 && (destination[i] == ' ' || destination[i] == ',')) {
        destination[i] = '\0';
        i--;
    }
}

char* find_next_key(char* current_key_start, char* base_input) {
    char* next_key = NULL;
    char* name_end = strstr(current_key_start + 1, "Name=");
    char* prog_end = strstr(current_key_start + 1, "Programme=");
    char* mark_end = strstr(current_key_start + 1, "Mark=");
    char* id_end = strstr(current_key_start + 1, "ID=");

    if (name_end != NULL) next_key = (next_key == NULL || name_end < next_key) ? name_end : next_key;
    if (prog_end != NULL) next_key = (next_key == NULL || prog_end < next_key) ? prog_end : next_key;
    if (mark_end != NULL) next_key = (next_key == NULL || mark_end < next_key) ? mark_end : next_key;
    if (id_end != NULL) next_key = (next_key == NULL || id_end < next_key) ? id_end : next_key;

    return next_key;
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

// --- CORE OPERATIONS ---

void load_data_from_file() {
    // If existing data, free and reset
    count = 0;

    FILE* file = fopen("Team_P7_4-CMS.txt", "r");
    if (!file) {
        printf("Error: Cannot open Team_P7_4-CMS.txt\n");
        return;
    }

    char line[500];
    if (fgets(line, sizeof(line), file)) { /* Skip header */ }

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        if (strlen(line) == 0) continue;

        // Ensure Dynamic Capacity
        ensure_capacity();

        int id;
        char name[100] = "";
        char programme[100] = "";
        float mark;

        char* token;
        int field = 0;
        char temp_line[500];
        strcpy(temp_line, line);

        token = strtok(temp_line, ",");
        while (token != NULL) {
            while (*token == ' ') token++;
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
    file_opened = 1;
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

/**
 * NEW FEATURE: SHOW SUMMARY
 * Implementation of Summary Statistics [cite: 41]
 * Displays:
 * 1. Total number of students
 * 2. Average mark
 * 3. Highest and lowest mark with student names
 */
void show_summary() {
    if (!file_opened) {
        printf("CMS: Error - No file opened. Use OPEN command first.\n");
        return;
    }

    if (count == 0) {
        printf("CMS: No records available for summary.\n");
        return;
    }

    float total_marks = 0.0;
    float max_mark = -1.0;
    float min_mark = 101.0; // Higher than max possible mark

    // Indices to track who has the highest/lowest
    int max_indices[100]; // Store multiple students if they share the same mark
    int min_indices[100];
    int max_count = 0;
    int min_count = 0;

    // First Pass: Calculate Sum and find Min/Max values
    for (int i = 0; i < count; i++) {
        total_marks += records[i].mark;

        if (records[i].mark > max_mark) {
            max_mark = records[i].mark;
        }
        if (records[i].mark < min_mark) {
            min_mark = records[i].mark;
        }
    }

    // Second Pass: Identify all students with those Min/Max values
    for (int i = 0; i < count; i++) {
        if (records[i].mark == max_mark) {
            max_indices[max_count++] = i;
        }
        if (records[i].mark == min_mark) {
            min_indices[min_count++] = i;
        }
    }

    float average = total_marks / count;

    printf("\n=== CLASS SUMMARY STATISTICS ===\n");
    printf("Total Students: %d\n", count);
    printf("Average Mark  : %.2f\n", average);

    printf("--------------------------------\n");
    printf("Highest Mark  : %.2f\n", max_mark);
    printf("Achieved by   : ");
    for (int i = 0; i < max_count; i++) {
        printf("%s", records[max_indices[i]].name);
        if (i < max_count - 1) printf(", ");
    }
    printf("\n");

    printf("Lowest Mark   : %.2f\n", min_mark);
    printf("Achieved by   : ");
    for (int i = 0; i < min_count; i++) {
        printf("%s", records[min_indices[i]].name);
        if (i < min_count - 1) printf(", ");
    }
    printf("\n================================\n");
}

void query_record(char* input) {
    if (!file_opened) {
        printf("CMS: Error - No file opened. Use OPEN command first.\n");
        return;
    }
    if (strlen(input) == 0) {
        printf("CMS: Error - Missing ID. Format: QUERY ID=7-digits\n");
        return;
    }
    char temp_input[300];
    strcpy(temp_input, input);
    char* id_ptr = strstr(temp_input, "ID=");
    if (!id_ptr) {
        printf("CMS: Error - Format must include 'ID=7-digits'.\n");
        return;
    }
    id_ptr += 3;
    char id_str[8] = "";
    for (int i = 0; i < 7 && isdigit(id_ptr[i]); i++) id_str[i] = id_ptr[i];
    id_str[7] = '\0';

    if (strlen(id_str) != 7) {
        printf("CMS: Error - ID must be 7 digits.\n");
        return;
    }
    int id = atoi(id_str);
    int index = find_student_index(id);
    if (index == -1) {
        printf("CMS: Record with ID=%d not found.\n", id);
        return;
    }
    printf("CMS: Record found:\n");
    printf("ID: %d\nName: %s\nProgramme: %s\nMark: %.1f\n",
        records[index].id, records[index].name, records[index].programme, records[index].mark);
}

void delete_record(char* input) {
    if (!file_opened) {
        printf("CMS: Error - No file opened. Use OPEN command first.\n");
        return;
    }
    if (strlen(input) == 0) {
        printf("CMS: Error - Missing ID. Format: DELETE ID=7-digits\n");
        return;
    }
    char temp_input[300];
    strcpy(temp_input, input);
    char* id_ptr = strstr(temp_input, "ID=");
    if (!id_ptr) {
        printf("CMS: Error - Format must include 'ID=7-digits'.\n");
        return;
    }
    id_ptr += 3;
    char id_str[8] = "";
    for (int i = 0; i < 7 && isdigit(id_ptr[i]); i++) id_str[i] = id_ptr[i];
    id_str[7] = '\0';
    if (strlen(id_str) != 7) {
        printf("CMS: Error - ID must be 7 digits.\n");
        return;
    }
    int id = atoi(id_str);
    int index = find_student_index(id);
    if (index == -1) {
        printf("CMS: Error - ID %d not found.\n", id);
        return;
    }
    printf("\nRecord to delete:\nID: %d, Name: %s, Mark: %.1f\n",
        records[index].id, records[index].name, records[index].mark);
    printf("Confirm deletion? (yes/no): ");
    char response[10];
    fgets(response, sizeof(response), stdin);
    response[strcspn(response, "\n")] = 0;
    for (int i = 0; response[i]; i++) response[i] = tolower(response[i]);

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

void insert_record(char* input) {
    if (!file_opened) {
        printf("CMS: Error - No file opened. Use OPEN command first.\n");
        return;
    }

    // Dynamic Capacity Check
    ensure_capacity();

    if (strlen(input) == 0) {
        printf("CMS: Error - Missing insert data.\n");
        return;
    }
    char temp_input[300];
    strcpy(temp_input, input);
    char name[100] = "", programme[100] = "";
    float mark = -1.0;
    int id = 0;

    char* id_ptr = strstr(temp_input, "ID=");
    if (!id_ptr) { printf("CMS: Error - Missing ID.\n"); return; }
    id_ptr += 3;
    char id_str[8] = "";
    int i = 0;
    while (i < 7 && isdigit(id_ptr[i])) { id_str[i] = id_ptr[i]; i++; }
    id_str[i] = '\0';
    if (strlen(id_str) != 7) { printf("CMS: Error - ID must be 7 digits.\n"); return; }
    if (!is_valid_id_prefix(id_str)) { printf("CMS: Error - ID prefix must be 21-25.\n"); return; }

    id = atoi(id_str);
    if (find_student_index(id) != -1) { printf("CMS: Error - ID %d already exists.\n", id); return; }

    char* name_start = strstr(temp_input, "Name=");
    char* prog_start = strstr(temp_input, "Programme=");
    char* mark_start = strstr(temp_input, "Mark=");

    if (!name_start || !prog_start || !mark_start) {
        printf("CMS: Error - Missing Name, Programme, or Mark.\n");
        return;
    }

    if (name_start) {
        char* name_stop = find_next_key(name_start, temp_input);
        extract_and_clean(name_start + 5, name_stop, name);
    }
    if (prog_start) {
        char* prog_stop = find_next_key(prog_start, temp_input);
        extract_and_clean(prog_start + 10, prog_stop, programme);
    }
    if (mark_start && sscanf(mark_start + 5, "%f", &mark) != 1) {
        printf("CMS: Error - Invalid Mark.\n"); return;
    }

    if (strlen(name) == 0 || strlen(programme) == 0) { printf("CMS: Error - Empty fields.\n"); return; }
    if (mark < 0.0 || mark > 100.0) { printf("CMS: Error - Marks must be 0-100.\n"); return; }

    capitalize_words(name);
    capitalize_words(programme);

    if (is_duplicate_data(name, programme, mark)) { printf("CMS: Error - Duplicate record.\n"); return; }

    records[count].id = id;
    strcpy(records[count].name, name);
    strcpy(records[count].programme, programme);
    records[count].mark = mark;
    count++;
    unsaved_changes = 1;
    printf("CMS: Record inserted successfully.\n");
}

void update_record(char* input) {
    if (!file_opened) { printf("CMS: Error - No file opened.\n"); return; }
    char temp_input[300];
    strcpy(temp_input, input);
    char* id_ptr = strstr(temp_input, "ID=");
    if (!id_ptr) { printf("CMS: Error - Missing ID.\n"); return; }
    id_ptr += 3;
    char id_str[8] = "";
    for (int i = 0; i < 7 && isdigit(id_ptr[i]); i++) id_str[i] = id_ptr[i];
    id_str[7] = '\0';
    if (strlen(id_str) != 7) { printf("CMS: Error - ID must be 7 digits.\n"); return; }
    int id = atoi(id_str);
    int index = find_student_index(id);
    if (index == -1) { printf("CMS: Record %d not found.\n", id); return; }

    char* mark_ptr = strstr(temp_input, "Mark=");
    char* name_ptr = strstr(temp_input, "Name=");
    char* prog_ptr = strstr(temp_input, "Programme=");

    char new_name[100], new_prog[100];
    float new_mark;
    strcpy(new_name, records[index].name);
    strcpy(new_prog, records[index].programme);
    new_mark = records[index].mark;
    int update_made = 0;

    if (mark_ptr) {
        float mark_val;
        if (sscanf(mark_ptr + 5, "%f", &mark_val) == 1 && mark_val >= 0 && mark_val <= 100) {
            new_mark = mark_val; update_made = 1;
        }
        else { printf("CMS: Error - Invalid Mark.\n"); return; }
    }
    if (prog_ptr) {
        char temp_prog[100];
        char* stop = find_next_key(prog_ptr, temp_input);
        extract_and_clean(prog_ptr + 10, stop, temp_prog);
        if (strlen(temp_prog) > 0) {
            capitalize_words(temp_prog); strcpy(new_prog, temp_prog); update_made = 1;
        }
    }
    if (name_ptr) {
        char temp_name[100];
        char* stop = find_next_key(name_ptr, temp_input);
        extract_and_clean(name_ptr + 5, stop, temp_name);
        if (strlen(temp_name) > 0) {
            capitalize_words(temp_name); strcpy(new_name, temp_name); update_made = 1;
        }
    }

    if (update_made) {
        strcpy(records[index].name, new_name);
        strcpy(records[index].programme, new_prog);
        records[index].mark = new_mark;
        unsaved_changes = 1;
        printf("CMS: Record updated successfully.\n");
    }
    else {
        printf("CMS: No valid fields to update.\n");
    }
}

void sort_by_id() {
    if (!file_opened) { printf("CMS: Error - No file opened.\n"); return; }
    if (count == 0) { printf("No records.\n"); return; }
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (records[j].id > records[j + 1].id) {
                Student temp = records[j]; records[j] = records[j + 1]; records[j + 1] = temp;
            }
        }
    }
    unsaved_changes = 1;
    printf("Records sorted by ID.\n");
}

void sort_by_marks() {
    if (!file_opened) { printf("CMS: Error - No file opened.\n"); return; }
    if (count == 0) { printf("No records.\n"); return; }
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (records[j].mark < records[j + 1].mark) {
                Student temp = records[j]; records[j] = records[j + 1]; records[j + 1] = temp;
            }
        }
    }
    unsaved_changes = 1;
    printf("Records sorted by marks.\n");
}

void show_help() {
    printf("\n=== Available Commands ===\n");
    if (!file_opened) printf("OPEN        - Open the student records file\n");
    printf("SHOW ALL    - Display all records\n");
    printf("SHOW SUMMARY- Display statistics (Total, Avg, Min, Max)\n");
    printf("QUERY ID=.. - Search record\n");
    printf("INSERT ...  - Add record\n");
    printf("UPDATE ...  - Update record\n");
    printf("DELETE ...  - Delete record\n");
    printf("SORT ID/MARKS - Sort records\n");
    printf("SAVE        - Save changes\n");
    printf("EXIT        - Quit\n");
    printf("==========================\n\n");
}

int confirm_exit() {
    if (unsaved_changes) {
        printf("\n⚠️  Unsaved changes! Exit without saving? (yes/no): ");
        char response[10];
        fgets(response, sizeof(response), stdin);
        response[strcspn(response, "\n")] = 0;
        if (strcmp(response, "yes") == 0 || strcmp(response, "y") == 0) return 1;
        return 0;
    }
    return 1;
}

void open_file() {
    if (file_opened) { printf("CMS: File already opened.\n"); return; }
    load_data_from_file();
    if (file_opened) {
        printf("CMS: Team_P7_4-CMS.txt opened successfully.\n");
        printf("CMS: %d records loaded.\n", count);
    }
}

void print_declaration() {
    printf("***************************************************************\n");
    printf("Declaration\n");
    printf("SIT's policy on copying does not allow the students to copy\n");
    printf("source code as well as assessment solutions from another\n");
    printf("person AI or other places. It is the students' responsibility\n");
    printf("to guarantee that their assessment solutions are their own\n");
    printf("work. Meanwhile, the students must also ensure that their\n");
    printf("work is not accessible by others. Where such plagiarism is\n");
    printf("detected, both of the assessments involved will receive ZERO\n");
    printf("mark.\n\n");
    printf("We hereby declare that:\n");
    printf("- We fully understand and agree to the abovementioned plagiarism policy.\n");
    printf("- We did not copy any code from others or from other places.\n");
    printf("- We did not share our codes with others or upload to any other places for public access and will not do that in the future.\n");
    printf("- We agree that our project will receive Zero mark if there is any plagiarism detected.\n");
    printf("- We agree that we will not disclose any information or material of the group project to others or upload to any other places for public access.\n");
    printf("- We agree that we did not copy any code directly from AI generated sources\n\n");
    printf("Declared by: P7_4\n");
    printf("Team members:\n");
    printf("1. Hermann Phua\n");
    printf("2. Thiha Thein Kevin\n");
    printf("3. Sharlene Teo\n");
    printf("4. Glenda Teo\n");
    printf("5. Oh Rui Cheng\n");
    printf("Date: 24 November 2025\n");
    printf("***************************************************************\n\n");
}

int main() {
    // --- INITIALIZE DYNAMIC MEMORY ---
    init_system();
	print_declaration();

    char command_line[300];
    char command[50];
    char lower_command[50];

    printf("Student Records System started.\n\n");
    printf("Type HELP for a list of available commands\n\n");

    while (1) {
        printf("P7_4: ");
        fgets(command_line, sizeof(command_line), stdin);
        command_line[strcspn(command_line, "\n")] = 0;

        char temp_line[300];
        strcpy(temp_line, command_line);
        char* token = strtok(temp_line, " ");
        if (token == NULL) { printf("\n"); continue; }

        strcpy(command, token);
        strcpy(lower_command, command);
        to_lowercase(lower_command);

        char* args_start = command_line + strlen(command);
        while (*args_start == ' ') args_start++;

        if (strcmp(lower_command, "open") == 0) open_file();
        else if (strcmp(lower_command, "show") == 0) {
            char lower_args[50];
            strcpy(lower_args, args_start);
            to_lowercase(lower_args);

            if (strcmp(lower_args, "all") == 0) show_all();
            else if (strcmp(lower_args, "summary") == 0) show_summary();
            else printf("Unknown command. Use 'SHOW ALL' or 'SHOW SUMMARY'.\n");
        }
        else if (strcmp(lower_command, "query") == 0) query_record(args_start);
        else if (strcmp(lower_command, "insert") == 0) insert_record(args_start);
        else if (strcmp(lower_command, "update") == 0) update_record(args_start);
        else if (strcmp(lower_command, "delete") == 0) delete_record(args_start);
        else if (strcmp(lower_command, "sort") == 0) {
            char sort_key[10];
            if (sscanf(args_start, "%s", sort_key) == 1) {
                to_lowercase(sort_key);
                if (strcmp(sort_key, "id") == 0) sort_by_id();
                else if (strcmp(sort_key, "marks") == 0) sort_by_marks();
                else printf("Unknown sort key.\n");
            }
            else printf("Unknown sort key.\n");
        }
        else if (strcmp(lower_command, "save") == 0) save_data_to_file();
        else if (strcmp(lower_command, "help") == 0) show_help();
        else if (strcmp(lower_command, "exit") == 0) {
            if (confirm_exit()) {
                cleanup(); // Free dynamic memory
                printf("Goodbye!\n");
                break;
            }
        }
        else printf("Unknown command.\n");
        printf("\n");
    }
    return 0;
}




