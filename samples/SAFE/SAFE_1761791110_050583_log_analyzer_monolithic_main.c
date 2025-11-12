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
    
    int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    if (parsed != 3) return 0;
    
    if (strlen(timestamp) > 31 || strlen(level) > 15 || strlen(message) > 255) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

int count_entries_by_level(struct LogEntry* entries, int count, const char* level) {
    int total = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, level) == 0) {
            total++;
        }
    }
    return total;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Log Analyzer - Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Supported levels: INFO, WARN, ERROR, DEBUG\n");
    printf("Enter 'END' on a new line to finish input\n\n");
    
    while (entry_count < MAX_ENTRIES) {
        printf("Log entry %d: ", entry_count + 1);
        
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        line[strcspn(line, "\n")] = 0;
        
        if (strlen(line) == 0) {
            continue;
        }
        
        if (strcmp(line, "END") == 0) {
            break;
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
    
    printf("\n=== Log Analysis Results ===\n");
    printf("Total entries processed: %d\n", entry_count);
    
    if (entry_count > 0) {
        printf("\nBreakdown by level:\n");
        printf("INFO:  %d entries\n", count_entries_by_level(entries, entry_count, "INFO"));
        printf("WARN:  %d entries\n", count_entries_by_level(entries, entry_count, "WARN"));
        printf("ERROR: %d entries\n", count_entries_by_level(entries, entry_count, "ERROR"));
        printf("DEBUG: %d entries\n", count_entries_by_level(entries, entry_count, "DEBUG"));
        
        printf("\nRecent entries:\n");
        int display_count = (entry_count < 5) ? entry_count : 5;
        for (int i = entry_count - display_count; i < entry_count; i++) {
            printf("%s [%s] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    } else {
        printf("No valid log entries to analyze.\n");
    }
    
    return 0;
}