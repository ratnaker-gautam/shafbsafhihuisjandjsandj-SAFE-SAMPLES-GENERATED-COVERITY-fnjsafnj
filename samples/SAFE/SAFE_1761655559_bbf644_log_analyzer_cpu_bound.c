//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 10000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
    int error_count;
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char buffer[MAX_LINE_LENGTH];
    strncpy(buffer, line, MAX_LINE_LENGTH - 1);
    buffer[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp = strtok(buffer, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    entry->error_count = 0;
    if (strcmp(level, "ERROR") == 0) {
        entry->error_count = 1;
    }
    
    return 1;
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int total_errors = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Log Analyzer - Processing simulated log data...\n");
    
    for (int i = 0; i < 5000 && entry_count < MAX_ENTRIES; i++) {
        char timestamp[32];
        time_t t = time(NULL) - (rand() % 86400);
        struct tm* tm_info = localtime(&t);
        strftime(timestamp, 32, "%Y-%m-%d %H:%M:%S", tm_info);
        
        const char* levels[] = {"INFO", "WARNING", "ERROR"};
        const char* messages[] = {
            "User login successful",
            "Database connection established",
            "File not found",
            "Memory allocation failed",
            "Network timeout",
            "Configuration loaded",
            "Backup completed",
            "Security alert detected"
        };
        
        const char* level = levels[rand() % 3];
        const char* message = messages[rand() % 8];
        
        snprintf(line, MAX_LINE_LENGTH, "%s %s %s\n", timestamp, level, message);
        
        if (parse_log_line(line, &entries[entry_count])) {
            total_errors += entries[entry_count].error_count;
            entry_count++;
        }
    }
    
    printf("Analysis Results:\n");
    printf("Total log entries processed: %d\n", entry_count);
    printf("Total errors found: %d\n", total_errors);
    printf("Error rate: %.2f%%\n", entry_count > 0 ? (total_errors * 100.0) / entry_count : 0.0);
    
    int warning_count = 0;
    for (int i = 0; i < entry_count; i++) {
        if (strcmp(entries[i].level, "WARNING") == 0) {
            warning_count++;
        }
    }
    
    printf("Warning count: %d\n", warning_count);
    printf("Info messages: %d\n", entry_count - total_errors - warning_count);
    
    if (total_errors > 0) {
        printf("\nRecent error messages:\n");
        int printed = 0;
        for (int i = entry_count - 1; i >= 0 && printed < 5; i--) {
            if (entries[i].error_count > 0) {
                printf("- %s: %s\n", entries[i].timestamp, entries[i].message);
                printed++;
            }
        }
    }
    
    return 0;
}