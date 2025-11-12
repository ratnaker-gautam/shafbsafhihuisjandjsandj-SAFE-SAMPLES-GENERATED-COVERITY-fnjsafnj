//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[64];
    char level[16];
    char message[256];
} LogEntry;

typedef void (*LogProcessor)(LogEntry*);

void count_errors(LogEntry* entry) {
    static int error_count = 0;
    if (strcmp(entry->level, "ERROR") == 0) {
        error_count++;
        printf("Error count: %d\n", error_count);
    }
}

void print_warnings(LogEntry* entry) {
    if (strcmp(entry->level, "WARNING") == 0) {
        printf("Warning: %s\n", entry->message);
    }
}

void find_keyword(LogEntry* entry) {
    static const char* keyword = "failed";
    if (strstr(entry->message, keyword) != NULL) {
        printf("Found '%s' in: %s\n", keyword, entry->message);
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
    
    if (strlen(timestamp) >= 64 || strlen(level) >= 16 || strlen(message) >= 256) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

void process_log_file(const char* filename, LogProcessor processor) {
    if (filename == NULL || processor == NULL) return;
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Cannot open file: %s\n", filename);
        return;
    }
    
    char line[MAX_LINE_LENGTH];
    LogEntry entry;
    
    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = 0;
        
        if (parse_log_line(line, &entry)) {
            processor(&entry);
        }
    }
    
    fclose(file);
}

int main() {
    const char* log_filename = "sample.log";
    
    FILE* test_file = fopen(log_filename, "w");
    if (test_file != NULL) {
        fprintf(test_file, "2023-10-01T10:00:00 INFO System started\n");
        fprintf(test_file, "2023-10-01T10:01:00 WARNING High memory usage\n");
        fprintf(test_file, "2023-10-01T10:02:00 ERROR Database connection failed\n");
        fprintf(test_file, "2023-10-01T10:03:00 INFO User login successful\n");
        fprintf(test_file, "2023-10-01T10:04:00 WARNING Disk space low\n");
        fprintf(test_file, "2023-10-01T10:05:00 ERROR Authentication failed\n");
        fclose(test_file);
    }
    
    printf("=== Counting Errors ===\n");
    process_log_file(log_filename, count_errors);
    
    printf("\n=== Listing Warnings ===\n");
    process_log_file(log_filename, print_warnings);
    
    printf("\n=== Finding 'failed' keyword ===\n");
    process_log_file(log_filename, find_keyword);
    
    remove(log_filename);
    
    return 0;
}