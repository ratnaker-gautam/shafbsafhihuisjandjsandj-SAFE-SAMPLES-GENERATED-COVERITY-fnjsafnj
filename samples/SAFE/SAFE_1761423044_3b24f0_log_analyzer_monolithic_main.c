//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char level[16];
    char timestamp[32];
    char message[256];
    
    int result = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    if (result != 3) return 0;
    
    if (strlen(timestamp) > 31 || strlen(level) > 15 || strlen(message) > 255) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

int main() {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Log Analyzer - Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Supported levels: INFO, WARN, ERROR, DEBUG\n");
    printf("Enter 'END' on a new line to finish input\n");
    
    while (entry_count < MAX_ENTRIES) {
        printf("Log entry %d: ", entry_count + 1);
        
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        if (strlen(line) >= MAX_LINE_LENGTH - 1) {
            printf("Error: Line too long. Maximum length is %d characters.\n", MAX_LINE_LENGTH - 2);
            continue;
        }
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count] = entry;
            entry_count++;
        } else {
            printf("Error: Invalid log format. Use: TIMESTAMP LEVEL MESSAGE\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 0;
    }
    
    printf("\n=== Log Analysis Results ===\n");
    printf("Total entries: %d\n", entry_count);
    
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0, other_count = 0;
    
    for (int i = 0; i < entry_count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        } else if (strcmp(entries[i].level, "WARN") == 0) {
            warn_count++;
        } else if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entries[i].level, "DEBUG") == 0) {
            debug_count++;
        } else {
            other_count++;
        }
    }
    
    printf("INFO: %d (%.1f%%)\n", info_count, (float)info_count / entry_count * 100);
    printf("WARN: %d (%.1f%%)\n", warn_count, (float)warn_count / entry_count * 100);
    printf("ERROR: %d (%.1f%%)\n", error_count, (float)error_count / entry_count * 100);
    printf("DEBUG: %d (%.1f%%)\n", debug_count, (float)debug_count / entry_count * 100);
    printf("OTHER: %d (%.1f%%)\n", other_count, (float)other_count / entry_count * 100);
    
    printf("\nRecent ERROR entries:\n");
    int error_shown = 0;
    for (int i = entry_count - 1; i >= 0 && error_shown < 5; i--) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            printf("- %s: %s\n", entries[i].timestamp, entries[i].message);
            error_shown++;
        }
    }
    
    if (error_shown == 0) {
        printf("No ERROR entries found.\n");
    }
    
    return 0;
}