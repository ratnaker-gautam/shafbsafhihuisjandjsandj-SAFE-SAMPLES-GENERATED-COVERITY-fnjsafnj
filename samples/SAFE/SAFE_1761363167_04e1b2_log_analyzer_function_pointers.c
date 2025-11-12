//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
        printf("Error #%d: %s - %s\n", error_count, entry->timestamp, entry->message);
    }
}

void print_warnings(LogEntry* entry) {
    if (strcmp(entry->level, "WARNING") == 0) {
        printf("Warning: %s - %s\n", entry->timestamp, entry->message);
    }
}

void process_log_level(LogEntry* entry, const char* target_level, LogProcessor processor) {
    if (strcmp(entry->level, target_level) == 0) {
        processor(entry);
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

void validate_log_level(const char* level) {
    const char* valid_levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    int valid = 0;
    for (int i = 0; i < 4; i++) {
        if (strcmp(level, valid_levels[i]) == 0) {
            valid = 1;
            break;
        }
    }
    if (!valid) {
        printf("Invalid log level: %s\n", level);
    }
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    const char* sample_logs[] = {
        "2024-01-15T10:30:00 INFO System started successfully",
        "2024-01-15T10:35:23 WARNING High memory usage detected",
        "2024-01-15T10:40:15 ERROR Database connection failed",
        "2024-01-15T10:42:00 INFO Backup completed",
        "2024-01-15T10:45:30 ERROR File not found: config.xml",
        "2024-01-15T10:50:00 DEBUG Processing user request",
        "2024-01-15T10:55:45 WARNING Slow response time",
        "2024-01-15T11:00:00 INFO User login successful"
    };
    
    int sample_count = sizeof(sample_logs) / sizeof(sample_logs[0]);
    
    for (int i = 0; i < sample_count && entry_count < MAX_ENTRIES; i++) {
        if (parse_log_line(sample_logs[i], &entries[entry_count])) {
            validate_log_level(entries[entry_count].level);
            entry_count++;
        }
    }
    
    printf("\n=== ERROR ANALYSIS ===\n");
    for (int i = 0; i < entry_count; i++) {
        process_log_level(&entries[i], "ERROR", count_errors);
    }
    
    printf("\n=== WARNING ANALYSIS ===\n");
    for (int i = 0; i < entry_count; i++) {
        process_log_level(&entries[i], "WARNING", print_warnings);
    }
    
    printf("\n=== SUMMARY ===\n");
    int info_count = 0, warning_count = 0, error_count = 0, debug_count = 0;
    
    for (int i = 0; i < entry_count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        else if (strcmp(entries[i].level, "WARNING") == 0) warning_count++;
        else if (strcmp(entries[i].level, "ERROR") == 0) error_count++;
        else if (strcmp(entries[i].level, "DEBUG") == 0) debug_count++;
    }
    
    printf("Total entries: %d\n", entry_count);
    printf("INFO: %d, WARNING: %d, ERROR: %d, DEBUG: %d\n", 
           info_count, warning_count, error_count, debug_count);
    
    return 0;
}