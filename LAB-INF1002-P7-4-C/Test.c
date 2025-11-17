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
            // Remove spaces
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
}

void save_data_to_file() {
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

void query_record() {
    printf("Enter ID to query: ");
    char id_input[20];
    fgets(id_input, sizeof(id_input), stdin);
    id_input[strcspn(id_input, "\n")] = 0;

    if (strlen(id_input) != 7) {
        printf("CMS: Error - ID must be 7 digits.\n");
        return;
    }

    for (int i = 0; i < 7; i++) {
        if (!isdigit(id_input[i])) {
            printf("CMS: Error - ID must be digits only.\n");
            return;
        }
    }

    int id = atoi(id_input);
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

void delete_record() {
    printf("Enter ID to delete: ");
    char id_input[20];
    fgets(id_input, sizeof(id_input), stdin);
    id_input[strcspn(id_input, "\n")] = 0;

    if (strlen(id_input) != 7) {
        printf("CMS: Error - ID must be 7 digits.\n");
        return;
    }

    for (int i = 0; i < 7; i++) {
        if (!isdigit(id_input[i])) {
            printf("CMS: Error - ID must be digits only.\n");
            return;
        }
    }

    int id = atoi(id_input);
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

void insert_record() {
    if (count >= 100) {
        printf("CMS: Error - Database full.\n");
        return;
    }

    printf("Enter ID (7 digits): ");
    char id_input[20];
    fgets(id_input, sizeof(id_input), stdin);
    id_input[strcspn(id_input, "\n")] = 0;

    if (strlen(id_input) != 7) {
        printf("CMS: Error - ID must be 7 digits.\n");
        return;
    }

    for (int i = 0; i < 7; i++) {
        if (!isdigit(id_input[i])) {
            printf("CMS: Error - ID must be digits only.\n");
            return;
        }
    }

    int id = atoi(id_input);

    if (find_student_index(id) != -1) {
        printf("CMS: Error - ID %d already exists.\n", id);
        return;
    }

    printf("Enter Name, Programme, Marks (Format: Name=Full Name, Programme=Programme Name, Marks=XX.X): ");
    char data_input[300];
    fgets(data_input, sizeof(data_input), stdin);
    data_input[strcspn(data_input, "\n")] = 0;

    char name[100] = "", programme[100] = "";
    float mark;

    // Simple parsing
    char* name_ptr = strstr(data_input, "Name=");
    char* prog_ptr = strstr(data_input, "Programme=");
    char* mark_ptr = strstr(data_input, "Marks=");

    if (!name_ptr || !prog_ptr || !mark_ptr) {
        printf("CMS: Error - Invalid format.\n");
        return;
    }

    // Extract name
    name_ptr += 5;
    char* name_end = strstr(name_ptr, ", Programme=");
    if (name_end) {
        strncpy(name, name_ptr, name_end - name_ptr);
        name[name_end - name_ptr] = '\0';
    }

    // Extract programme
    prog_ptr += 10;
    char* prog_end = strstr(prog_ptr, ", Marks=");
    if (prog_end) {
        strncpy(programme, prog_ptr, prog_end - prog_ptr);
        programme[prog_end - prog_ptr] = '\0';
    }

    // Extract marks
    mark_ptr += 6;
    if (sscanf(mark_ptr, "%f", &mark) != 1) {
        printf("CMS: Error - Invalid marks.\n");
        return;
    }

    // Trim spaces
    for (int i = strlen(name) - 1; i >= 0 && name[i] == ' '; i--) name[i] = '\0';
    for (int i = strlen(programme) - 1; i >= 0 && programme[i] == ' '; i--) programme[i] = '\0';

    capitalize_words(name);
    capitalize_words(programme);

    if (mark < 0 || mark > 100) {
        printf("CMS: Error - Marks must be 0-100.\n");
        return;
    }

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

void update_record() {
    printf("Enter ID to update: ");
    char id_input[20];
    fgets(id_input, sizeof(id_input), stdin);
    id_input[strcspn(id_input, "\n")] = 0;

    if (strlen(id_input) != 7) {
        printf("CMS: Error - ID must be 7 digits.\n");
        return;
    }

    for (int i = 0; i < 7; i++) {
        if (!isdigit(id_input[i])) {
            printf("CMS: Error - ID must be digits only.\n");
            return;
        }
    }

    int id = atoi(id_input);
    int index = find_student_index(id);

    if (index == -1) {
        printf("CMS: Error - ID %d not found.\n", id);
        return;
    }

    printf("\nCurrent record:\n");
    printf("ID: %d, Name: %s, Programme: %s, Mark: %.1f\n\n",
        records[index].id, records[index].name,
        records[index].programme, records[index].mark);

    printf("Enter updated Name, Programme, Marks: ");
    char data_input[300];
    fgets(data_input, sizeof(data_input), stdin);
    data_input[strcspn(data_input, "\n")] = 0;

    char name[100] = "", programme[100] = "";
    float mark;

    char* name_ptr = strstr(data_input, "Name=");
    char* prog_ptr = strstr(data_input, "Programme=");
    char* mark_ptr = strstr(data_input, "Marks=");

    if (!name_ptr || !prog_ptr || !mark_ptr) {
        printf("CMS: Error - Invalid format.\n");
        return;
    }

    // Extract data (same as insert)
    name_ptr += 5;
    char* name_end = strstr(name_ptr, ", Programme=");
    if (name_end) {
        strncpy(name, name_ptr, name_end - name_ptr);
        name[name_end - name_ptr] = '\0';
    }

    prog_ptr += 10;
    char* prog_end = strstr(prog_ptr, ", Marks=");
    if (prog_end) {
        strncpy(programme, prog_ptr, prog_end - prog_ptr);
        programme[prog_end - prog_ptr] = '\0';
    }

    mark_ptr += 6;
    if (sscanf(mark_ptr, "%f", &mark) != 1) {
        printf("CMS: Error - Invalid marks.\n");
        return;
    }

    for (int i = strlen(name) - 1; i >= 0 && name[i] == ' '; i--) name[i] = '\0';
    for (int i = strlen(programme) - 1; i >= 0 && programme[i] == ' '; i--) programme[i] = '\0';

    capitalize_words(name);
    capitalize_words(programme);

    if (mark < 0 || mark > 100) {
        printf("CMS: Error - Marks must be 0-100.\n");
        return;
    }

    // Check duplicate
    char old_name[100], old_prog[100];
    float old_mark = records[index].mark;
    strcpy(old_name, records[index].name);
    strcpy(old_prog, records[index].programme);

    strcpy(records[index].name, name);
    strcpy(records[index].programme, programme);
    records[index].mark = mark;

    if (is_duplicate_data(name, programme, mark)) {
        strcpy(records[index].name, old_name);
        strcpy(records[index].programme, old_prog);
        records[index].mark = old_mark;
        printf("CMS: Error - Duplicate record exists.\n");
        return;
    }

    unsaved_changes = 1;
    printf("CMS: Record updated successfully.\n");
}

void sort_by_id() {
    if (count == 0) {
        printf("No records to sort.\n");
        return;
    }

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
    if (count == 0) {
        printf("No records to sort.\n");
        return;
    }

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
    printf("SHOW ALL    - Display all student records\n");
    printf("QUERY       - Search for a record by ID\n");
    printf("INSERT      - Add a new student record\n");
    printf("UPDATE      - Update an existing record\n");
    printf("DELETE      - Delete a record by ID\n");
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

void to_lowercase(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

int main() {
    char command[50];
    char lower_command[50];

    load_data_from_file();

    printf("Student Records System started.\n");
    printf("Team_P7_4-CMS.txt loaded.\n\n");
    printf("Type HELP for a list of available commands\n\n");

    while (1) {
        printf("Enter command: ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0;

        strcpy(lower_command, command);
        to_lowercase(lower_command);

        if (strcmp(lower_command, "show all") == 0) {
            show_all();
        }
        else if (strcmp(lower_command, "query") == 0) {
            query_record();
        }
        else if (strcmp(lower_command, "insert") == 0) {
            insert_record();
        }
        else if (strcmp(lower_command, "update") == 0) {
            update_record();
        }
        else if (strcmp(lower_command, "delete") == 0) {
            delete_record();
        }
        else if (strcmp(lower_command, "sort id") == 0) {
            sort_by_id();
        }
        else if (strcmp(lower_command, "sort marks") == 0) {
            sort_by_marks();
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