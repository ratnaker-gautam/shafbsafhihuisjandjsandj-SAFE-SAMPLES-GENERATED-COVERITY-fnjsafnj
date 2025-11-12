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

void process_log(LogData* data, LogProcessor processor) {
    if (data == NULL || processor == NULL) return;
    
    for (int i = 0; i < data->count; i++) {
        if (data->lines[i][0] != '\0') {
            processor(data->lines[i]);
        }
    }
}

int read_log_file(LogData* data) {
    if (data == NULL) return 0;
    
    data->count = 0;
    char buffer[MAX_LINE_LENGTH];
    
    printf("Enter log lines (empty line to finish, max %d lines):\n", MAX_LINES);
    
    while (data->count < MAX_LINES) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        if (buffer[0] == '\0') {
            break;
        }
        
        if (len >= MAX_LINE_LENGTH) {
            printf("Line too long, skipping.\n");
            continue;
        }
        
        strncpy(data->lines[data->count], buffer, MAX_LINE_LENGTH - 1);
        data->lines[data->count][MAX_LINE_LENGTH - 1] = '\0';
        data->count++;
    }
    
    return data->count;
}

void display_menu(void) {
    printf("\nLog Analysis Menu:\n");
    printf("1. Count all lines\n");
    printf("2. Find ERROR messages\n");
    printf("3. Find WARNING messages\n");
    printf("4. Exit\n");
    printf("Choose an option: ");
}

int get_menu_choice(void) {
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    char* endptr;
    long choice = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n') {
        return -1;
    }
    
    if (choice < 1 || choice > 4) {
        return -1;
    }
    
    return (int)choice;
}

int main(void) {
    LogData log_data;
    
    printf("Log Analyzer\n");
    printf("============\n");
    
    if (read_log_file(&log_data) == 0) {
        printf("No log data entered.\n");
        return 1;
    }
    
    printf("\nRead %d log lines.\n", log_data.count);
    
    while (1) {
        display_menu();
        int choice = get_menu_choice();
        
        if (choice == -1) {
            printf("Invalid choice. Please try again.\n");
            continue;
        }
        
        switch (choice) {
            case 1:
                process_log(&log_data, count_lines);
                break;
            case 2:
                process_log(&log_data, find_errors);
                break;
            case 3:
                process_log(&log_data, find_warnings);
                break;
            case 4:
                printf("Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice.\n");
                break;
        }
    }
    
    return 0;
}