//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH];
} LogEntry;

typedef void (*LogProcessor)(LogEntry*);

void count_entries(LogEntry* entry) {
    static int count = 0;
    count++;
    printf("Processed %d entries\n", count);
}

void find_errors(LogEntry* entry) {
    if (strcmp(entry->level, "ERROR") == 0) {
        printf("ERROR found: %s - %s\n", entry->timestamp, entry->message);
    }
}

void find_warnings(LogEntry* entry) {
    if (strcmp(entry->level, "WARN") == 0) {
        printf("WARNING found: %s - %s\n", entry->timestamp, entry->message);
    }
}

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= 19) return 0;
    if (strlen(level) >= 9) return 0;
    if (strlen(message) >= MAX_LINE_LENGTH - 1) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

void process_log_file(FILE* file, LogProcessor processor) {
    if (file == NULL || processor == NULL) return;
    
    char line[MAX_LINE_LENGTH];
    LogEntry entry;
    int line_count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && line_count < MAX_ENTRIES) {
        line_count++;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (parse_log_line(line, &entry)) {
            processor(&entry);
        }
    }
}

int main() {
    printf("Log Analyzer\n");
    printf("Available operations:\n");
    printf("1. Count all log entries\n");
    printf("2. Find ERROR entries\n");
    printf("3. Find WARNING entries\n");
    printf("Enter choice (1-3): ");
    
    char input[10];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    int choice;
    if (sscanf(input, "%d", &choice) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    LogProcessor processor = NULL;
    
    switch (choice) {
        case 1:
            processor = count_entries;
            break;
        case 2:
            processor = find_errors;
            break;
        case 3:
            processor = find_warnings;
            break;
        default:
            fprintf(stderr, "Invalid choice\n");
            return 1;
    }
    
    FILE* log_file = fopen("sample.log", "r");
    if (log_file == NULL) {
        fprintf(stderr, "Cannot open sample.log\n");
        printf("Creating sample log data...\n");
        
        log_file = fopen("sample.log", "w");
        if (log_file == NULL) {
            fprintf(stderr, "Cannot create sample.log\n");
            return 1;
        }
        
        fprintf(log_file, "2024-01-15T10:30:00 INFO System started\n");
        fprintf(log_file, "2024-01-15T10:31:15 WARN High memory usage\n");
        fprintf(log_file, "2024-01-15T10:32:00 ERROR Disk full\n");
        fprintf(log_file, "2024-01-15T10:33:45 INFO Backup completed\n");
        fprintf(log_file, "2024-01-15T10:34:20 WARN Network latency\n");
        fprintf(log_file, "2024-01-15T10:35:00 ERROR Service unavailable\n");
        
        fclose(log_file);
        log_file = fopen("sample.log", "r");
        if (log_file == NULL) {
            fprintf(stderr, "Cannot reopen sample.log\n");
            return 1;
        }
    }
    
    printf("Processing log file...\n");
    process_log_file(log_file, processor);
    
    fclose(log_file);
    return 0;
}