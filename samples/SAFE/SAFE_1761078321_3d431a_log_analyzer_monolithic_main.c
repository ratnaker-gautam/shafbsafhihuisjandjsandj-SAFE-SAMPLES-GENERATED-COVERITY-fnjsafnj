//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[64];
    char level[16];
    char message[256];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char level[16];
    char message[256];
    char timestamp[64];
    
    int result = sscanf(line, "%63s %15s %255[^\n]", timestamp, level, message);
    if (result != 3) return 0;
    
    if (strlen(timestamp) > 63 || strlen(level) > 15 || strlen(message) > 255) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

int is_valid_level(const char* level) {
    return strcmp(level, "INFO") == 0 || strcmp(level, "WARN") == 0 || 
           strcmp(level, "ERROR") == 0 || strcmp(level, "DEBUG") == 0;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    
    char line[MAX_LINE_LENGTH];
    
    printf("Log Analyzer - Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Supported levels: INFO, WARN, ERROR, DEBUG\n");
    printf("Enter 'END' on a separate line to finish input\n\n");
    
    while (entry_count < MAX_ENTRIES) {
        printf("Log entry %d: ", entry_count + 1);
        
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
        
        if (strlen(line) >= MAX_LINE_LENGTH - 1) {
            printf("Error: Line too long. Maximum %d characters allowed.\n", MAX_LINE_LENGTH - 1);
            continue;
        }
        
        struct LogEntry entry;
        if (!parse_log_line(line, &entry)) {
            printf("Error: Invalid log format. Use: TIMESTAMP LEVEL MESSAGE\n");
            continue;
        }
        
        if (!is_valid_level(entry.level)) {
            printf("Error: Invalid log level. Use INFO, WARN, ERROR, or DEBUG.\n");
            continue;
        }
        
        entries[entry_count] = entry;
        entry_count++;
        
        if (strcmp(entry.level, "INFO") == 0) info_count++;
        else if (strcmp(entry.level, "WARN") == 0) warn_count++;
        else if (strcmp(entry.level, "ERROR") == 0) error_count++;
        else if (strcmp(entry.level, "DEBUG") == 0) debug_count++;
    }
    
    printf("\n=== Log Analysis Results ===\n");
    printf("Total entries processed: %d\n", entry_count);
    printf("INFO entries: %d\n", info_count);
    printf("WARN entries: %d\n", warn_count);
    printf("ERROR entries: %d\n", error_count);
    printf("DEBUG entries: %d\n", debug_count);
    
    if (entry_count > 0) {
        printf("\n=== Recent Log Entries ===\n");
        int display_count = (entry_count < 5) ? entry_count : 5;
        for (int i = entry_count - display_count; i < entry_count; i++) {
            printf("%s [%s] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
        
        printf("\n=== Error Summary ===\n");
        if (error_count > 0) {
            printf("Error entries found:\n");
            for (int i = 0; i < entry_count; i++) {
                if (strcmp(entries[i].level, "ERROR") == 0) {
                    printf("- %s: %s\n", entries[i].timestamp, entries[i].message);
                }
            }
        } else {
            printf("No error entries found.\n");
        }
    }
    
    return 0;
}