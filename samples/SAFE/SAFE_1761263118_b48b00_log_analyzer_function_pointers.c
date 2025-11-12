//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LINES 1000

typedef struct {
    char lines[MAX_LINES][MAX_LINE_LENGTH];
    int count;
} LogData;

typedef void (*LogProcessor)(const char* line);

void count_lines(const char* line) {
    static int line_count = 0;
    line_count++;
    printf("Processed line %d: %s\n", line_count, line);
}

void find_errors(const char* line) {
    if (strstr(line, "ERROR") != NULL) {
        printf("Found ERROR: %s\n", line);
    }
}

void find_warnings(const char* line) {
    if (strstr(line, "WARNING") != NULL) {
        printf("Found WARNING: %s\n", line);
    }
}

void process_log_lines(LogData* data, LogProcessor processor) {
    if (data == NULL || processor == NULL) return;
    
    for (int i = 0; i < data->count; i++) {
        if (data->lines[i][0] != '\0') {
            processor(data->lines[i]);
        }
    }
}

int read_log_file(LogData* data, const char* filename) {
    if (data == NULL || filename == NULL) return 0;
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) return 0;
    
    data->count = 0;
    char buffer[MAX_LINE_LENGTH];
    
    while (data->count < MAX_LINES && fgets(buffer, sizeof(buffer), file) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        if (len < sizeof(data->lines[0])) {
            strncpy(data->lines[data->count], buffer, sizeof(data->lines[0]) - 1);
            data->lines[data->count][sizeof(data->lines[0]) - 1] = '\0';
            data->count++;
        }
    }
    
    fclose(file);
    return 1;
}

void print_menu(void) {
    printf("\nLog Analyzer Menu:\n");
    printf("1. Count all lines\n");
    printf("2. Find ERROR messages\n");
    printf("3. Find WARNING messages\n");
    printf("4. Exit\n");
    printf("Choose an option: ");
}

int get_user_choice(void) {
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) return -1;
    
    for (char* p = input; *p; p++) {
        if (!isdigit((unsigned char)*p) && *p != '\n') return -1;
    }
    
    int choice = atoi(input);
    return (choice >= 1 && choice <= 4) ? choice : -1;
}

int main(void) {
    LogData log_data;
    const char* test_log[] = {
        "INFO: System started successfully",
        "WARNING: Disk space running low",
        "ERROR: Database connection failed",
        "INFO: User login successful",
        "WARNING: High memory usage detected",
        "ERROR: File not found",
        "INFO: Backup completed",
        NULL
    };
    
    for (int i = 0; test_log[i] != NULL && i < MAX_LINES; i++) {
        strncpy(log_data.lines[i], test_log[i], MAX_LINE_LENGTH - 1);
        log_data.lines[i][MAX_LINE_LENGTH - 1] = '\0';
    }
    log_data.count = 7;
    
    LogProcessor processors[] = {count_lines, find_errors, find_warnings};
    
    while (1) {
        print_menu();
        int choice = get_user_choice();
        
        if (choice == -1) {
            printf("Invalid choice. Please try again.\n");
            continue;
        }
        
        if (choice == 4) {
            printf("Exiting log analyzer.\n");
            break;
        }
        
        if (choice >= 1 && choice <= 3) {
            printf("\nProcessing log...\n");
            process_log_lines(&log_data, processors[choice - 1]);
        }
    }
    
    return 0;
}