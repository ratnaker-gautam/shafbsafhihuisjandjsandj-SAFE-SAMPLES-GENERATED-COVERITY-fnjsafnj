//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
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
    if (strstr(line, "ERROR") != NULL || strstr(line, "error") != NULL) {
        printf("Found error: %s\n", line);
    }
}

void analyze_timestamps(const char* line) {
    const char* timestamp_start = strchr(line, '[');
    if (timestamp_start != NULL) {
        const char* timestamp_end = strchr(timestamp_start, ']');
        if (timestamp_end != NULL && timestamp_end > timestamp_start) {
            printf("Timestamp found: ");
            for (const char* p = timestamp_start; p <= timestamp_end && *p != '\0'; p++) {
                putchar(*p);
            }
            printf("\n");
        }
    }
}

void process_log(LogData* log, LogProcessor processor) {
    if (log == NULL || processor == NULL) return;
    
    for (int i = 0; i < log->count; i++) {
        if (log->lines[i][0] != '\0') {
            processor(log->lines[i]);
        }
    }
}

int read_log_file(LogData* log) {
    if (log == NULL) return 0;
    
    const char* sample_logs[] = {
        "[2024-01-15 10:30:45] INFO: System started",
        "[2024-01-15 10:31:20] ERROR: Database connection failed",
        "[2024-01-15 10:32:10] WARNING: High memory usage",
        "[2024-01-15 10:33:05] INFO: User login successful",
        "[2024-01-15 10:34:30] ERROR: File not found",
        "[2024-01-15 10:35:15] INFO: Backup completed"
    };
    
    int sample_count = sizeof(sample_logs) / sizeof(sample_logs[0]);
    
    for (int i = 0; i < sample_count && log->count < MAX_LINES; i++) {
        size_t len = strlen(sample_logs[i]);
        if (len < MAX_LINE_LENGTH) {
            strncpy(log->lines[log->count], sample_logs[i], len);
            log->lines[log->count][len] = '\0';
            log->count++;
        }
    }
    
    return log->count;
}

void print_menu() {
    printf("\nLog Analyzer Menu:\n");
    printf("1. Count and display all lines\n");
    printf("2. Find error messages\n");
    printf("3. Extract timestamps\n");
    printf("4. Exit\n");
    printf("Enter your choice (1-4): ");
}

int get_valid_choice() {
    char input[10];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    if (strlen(input) == 0 || !isdigit(input[0])) {
        return -1;
    }
    
    int choice = atoi(input);
    if (choice >= 1 && choice <= 4) {
        return choice;
    }
    
    return -1;
}

int main() {
    LogData log = {0};
    
    if (read_log_file(&log) == 0) {
        printf("Failed to load log data\n");
        return 1;
    }
    
    printf("Loaded %d log entries\n", log.count);
    
    LogProcessor processors[] = {count_lines, find_errors, analyze_timestamps};
    int running = 1;
    
    while (running) {
        print_menu();
        int choice = get_valid_choice();
        
        switch (choice) {
            case 1:
                printf("\n=== Line Count Analysis ===\n");
                process_log(&log, processors[0]);
                break;
            case 2:
                printf("\n=== Error Detection ===\n");
                process_log(&log, processors[1]);
                break;
            case 3:
                printf("\n=== Timestamp Extraction ===\n");
                process_log(&log, processors[2]);
                break;
            case 4:
                running = 0;
                printf("Exiting log analyzer\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }
    
    return 0;
}