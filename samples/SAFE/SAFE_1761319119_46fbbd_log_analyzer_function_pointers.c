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
    printf("Processed %d lines\n", line_count);
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

void analyze_log(LogData* log, LogProcessor processor) {
    if (log == NULL || processor == NULL) return;
    
    for (int i = 0; i < log->count; i++) {
        processor(log->lines[i]);
    }
}

int read_log_file(LogData* log) {
    if (log == NULL) return 0;
    
    printf("Enter log data (empty line to finish, max %d lines):\n", MAX_LINES);
    
    log->count = 0;
    char buffer[MAX_LINE_LENGTH];
    
    while (log->count < MAX_LINES) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        if (strlen(buffer) == 0) {
            break;
        }
        
        if (strlen(buffer) >= MAX_LINE_LENGTH - 1) {
            printf("Line too long, skipping\n");
            continue;
        }
        
        strncpy(log->lines[log->count], buffer, MAX_LINE_LENGTH - 1);
        log->lines[log->count][MAX_LINE_LENGTH - 1] = '\0';
        log->count++;
    }
    
    return log->count;
}

void print_menu() {
    printf("\nLog Analysis Options:\n");
    printf("1. Count all lines\n");
    printf("2. Find ERROR messages\n");
    printf("3. Find WARNING messages\n");
    printf("4. Exit\n");
    printf("Choose option (1-4): ");
}

int get_choice() {
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    if (strlen(input) == 0 || !isdigit(input[0])) {
        return -1;
    }
    
    int choice = atoi(input);
    if (choice < 1 || choice > 4) {
        return -1;
    }
    
    return choice;
}

int main() {
    LogData log;
    
    printf("Log Analyzer\n");
    printf("============\n");
    
    if (read_log_file(&log) == 0) {
        printf("No log data entered.\n");
        return 1;
    }
    
    printf("\nSuccessfully loaded %d log lines.\n", log.count);
    
    LogProcessor processors[] = {
        count_lines,
        find_errors,
        find_warnings,
        NULL
    };
    
    while (1) {
        print_menu();
        int choice = get_choice();
        
        if (choice == -1) {
            printf("Invalid choice. Please try again.\n");
            continue;
        }
        
        if (choice == 4) {
            printf("Exiting...\n");
            break;
        }
        
        if (choice >= 1 && choice <= 3) {
            analyze_log(&log, processors[choice - 1]);
        }
    }
    
    return 0;
}