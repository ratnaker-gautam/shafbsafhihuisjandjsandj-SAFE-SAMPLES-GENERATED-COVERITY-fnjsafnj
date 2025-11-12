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
    int error_count;
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LEN];
    strncpy(temp_line, line, MAX_LINE_LEN - 1);
    temp_line[MAX_LINE_LEN - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    entry->error_count = 0;
    if (strcmp(level, "ERROR") == 0) {
        entry->error_count = 1;
    }
    
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
    int total_messages = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        else if (strcmp(entries[i].level, "WARN") == 0) warn_count++;
        else if (strcmp(entries[i].level, "ERROR") == 0) error_count++;
        else if (strcmp(entries[i].level, "DEBUG") == 0) debug_count++;
        total_messages++;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", total_messages);
    printf("INFO: %d (%.1f%%)\n", info_count, total_messages > 0 ? (info_count * 100.0) / total_messages : 0.0);
    printf("WARN: %d (%.1f%%)\n", warn_count, total_messages > 0 ? (warn_count * 100.0) / total_messages : 0.0);
    printf("ERROR: %d (%.1f%%)\n", error_count, total_messages > 0 ? (error_count * 100.0) / total_messages : 0.0);
    printf("DEBUG: %d (%.1f%%)\n", debug_count, total_messages > 0 ? (debug_count * 100.0) / total_messages : 0.0);
    
    if (error_count > 0) {
        printf("\nError messages:\n");
        for (int i = 0; i < count; i++) {
            if (strcmp(entries[i].level, "ERROR") == 0) {
                printf("- %s: %s\n", entries[i].timestamp, entries[i].message);
            }
        }
    }
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    char line[MAX_LINE_LEN];
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (is_valid_level(entry.level)) {
                entries[entry_count] = entry;
                entry_count++;
            } else {
                printf("Invalid log level in: %s\n", line);
            }
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    analyze_logs(entries, entry_count);
    
    return 0;
}