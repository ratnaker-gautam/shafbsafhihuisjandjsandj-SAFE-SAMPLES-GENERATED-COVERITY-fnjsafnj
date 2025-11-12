//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LOG_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
} LogEntry;

typedef void (*LogProcessor)(LogEntry*);

void count_entries(LogEntry* entry) {
    static int count = 0;
    count++;
    printf("Processed %d entries\n", count);
}

void print_errors(LogEntry* entry) {
    if (strcmp(entry->level, "ERROR") == 0) {
        printf("ERROR: %s - %s\n", entry->timestamp, entry->message);
    }
}

void print_warnings(LogEntry* entry) {
    if (strcmp(entry->level, "WARN") == 0) {
        printf("WARNING: %s - %s\n", entry->timestamp, entry->message);
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
    
    if (strlen(timestamp) >= 31) return 0;
    if (strlen(level) >= 15) return 0;
    if (strlen(message) >= 255) return 0;
    
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
    
    while (fgets(line, sizeof(line), file) != NULL && line_count < MAX_LOG_ENTRIES) {
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
    printf("1. Count all entries\n");
    printf("2. Show errors only\n");
    printf("3. Show warnings only\n");
    printf("Enter choice (1-3): ");
    
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    int choice;
    if (sscanf(input, "%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    LogProcessor processors[] = {count_entries, print_errors, print_warnings};
    const char* processor_names[] = {"count", "errors", "warnings"};
    
    if (choice < 1 || choice > 3) {
        printf("Invalid choice\n");
        return 1;
    }
    
    printf("Processing log with %s processor...\n", processor_names[choice - 1]);
    
    FILE* log_file = fopen("sample.log", "r");
    if (log_file == NULL) {
        printf("Cannot open sample.log\n");
        printf("Creating sample log data...\n");
        
        FILE* sample = fopen("sample.log", "w");
        if (sample) {
            fprintf(sample, "2024-01-15T10:30:00 INFO System started\n");
            fprintf(sample, "2024-01-15T10:31:15 WARN Low disk space\n");
            fprintf(sample, "2024-01-15T10:32:00 ERROR Database connection failed\n");
            fprintf(sample, "2024-01-15T10:33:45 INFO User login successful\n");
            fprintf(sample, "2024-01-15T10:34:20 ERROR File not found\n");
            fprintf(sample, "2024-01-15T10:35:10 WARN High memory usage\n");
            fclose(sample);
            
            log_file = fopen("sample.log", "r");
        }
    }
    
    if (log_file) {
        process_log_file(log_file, processors[choice - 1]);
        fclose(log_file);
    } else {
        printf("Failed to process log file\n");
        return 1;
    }
    
    return 0;
}