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

void process_log_lines(LogData* log, LogProcessor processor) {
    if (log == NULL || processor == NULL) return;
    
    for (int i = 0; i < log->count; i++) {
        if (log->lines[i][0] != '\0') {
            processor(log->lines[i]);
        }
    }
}

int read_log_file(LogData* log) {
    if (log == NULL) return 0;
    
    log->count = 0;
    
    printf("Enter log lines (empty line to finish, max %d lines):\n", MAX_LINES);
    
    for (int i = 0; i < MAX_LINES; i++) {
        if (fgets(log->lines[i], MAX_LINE_LENGTH, stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(log->lines[i]);
        if (len > 0 && log->lines[i][len - 1] == '\n') {
            log->lines[i][len - 1] = '\0';
        }
        
        if (log->lines[i][0] == '\0') {
            break;
        }
        
        log->count++;
        
        if (log->count >= MAX_LINES) {
            printf("Maximum line count reached.\n");
            break;
        }
    }
    
    return log->count;
}

void display_menu(void) {
    printf("\nLog Analyzer Menu:\n");
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
    LogData log;
    
    printf("Log Analyzer\n");
    printf("============\n");
    
    if (read_log_file(&log) == 0) {
        printf("No log data entered.\n");
        return 1;
    }
    
    printf("\nSuccessfully loaded %d log lines.\n", log.count);
    
    while (1) {
        display_menu();
        int choice = get_menu_choice();
        
        if (choice == -1) {
            printf("Invalid choice. Please try again.\n");
            continue;
        }
        
        switch (choice) {
            case 1:
                printf("\nCounting all lines:\n");
                process_log_lines(&log, count_lines);
                break;
            case 2:
                printf("\nFinding ERROR messages:\n");
                process_log_lines(&log, find_errors);
                break;
            case 3:
                printf("\nFinding WARNING messages:\n");
                process_log_lines(&log, find_warnings);
                break;
            case 4:
                printf("Exiting...\n");
                return 0;
        }
    }
    
    return 0;
}