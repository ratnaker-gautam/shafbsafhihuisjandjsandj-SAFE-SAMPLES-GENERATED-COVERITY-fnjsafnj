//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 10000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
    int error_code;
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strcpy(temp_line, line);
    
    char* token = strtok(temp_line, " ");
    if (token == NULL) return 0;
    if (strlen(token) >= sizeof(entry->timestamp)) return 0;
    strcpy(entry->timestamp, token);
    
    token = strtok(NULL, " ");
    if (token == NULL) return 0;
    if (strlen(token) >= sizeof(entry->level)) return 0;
    strcpy(entry->level, token);
    
    token = strtok(NULL, " ");
    if (token == NULL) return 0;
    int code = atoi(token);
    if (code < 0 || code > 9999) return 0;
    entry->error_code = code;
    
    token = strtok(NULL, "\n");
    if (token == NULL) return 0;
    if (strlen(token) >= sizeof(entry->message)) return 0;
    strcpy(entry->message, token);
    
    return 1;
}

int is_valid_level(const char* level) {
    return strcmp(level, "INFO") == 0 || 
           strcmp(level, "WARN") == 0 || 
           strcmp(level, "ERROR") == 0 ||
           strcmp(level, "DEBUG") == 0;
}

void analyze_logs(LogEntry* entries, int count) {
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    int total_errors = 0;
    int max_error_code = 0;
    int min_error_code = 9999;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        else if (strcmp(entries[i].level, "WARN") == 0) warn_count++;
        else if (strcmp(entries[i].level, "ERROR") == 0) error_count++;
        else if (strcmp(entries[i].level, "DEBUG") == 0) debug_count++;
        
        if (entries[i].error_code > max_error_code) max_error_code = entries[i].error_code;
        if (entries[i].error_code < min_error_code) min_error_code = entries[i].error_code;
        total_errors += entries[i].error_code;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("INFO: %d, WARN: %d, ERROR: %d, DEBUG: %d\n", info_count, warn_count, error_count, debug_count);
    printf("Error code range: %d - %d\n", min_error_code, max_error_code);
    if (count > 0) {
        printf("Average error code: %.2f\n", (double)total_errors / count);
    }
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL ERROR_CODE MESSAGE)\n");
    printf("Example: 2024-01-15T10:30:00 INFO 100 System started\n");
    printf("Enter 'END' to finish input\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (is_valid_level(entry.level)) {
                entries[entry_count] = entry;
                entry_count++;
            } else {
                printf("Invalid log level in: %s\n", line);
            }
        } else {
            printf("Failed to parse: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    analyze_logs(entries, entry_count);
    
    printf("\nDetailed log entries:\n");
    for (int i = 0; i < entry_count; i++) {
        printf("%s %s %d %s\n", entries[i].timestamp, entries[i].level, 
               entries[i].error_code, entries[i].message);
    }
    
    return 0;
}