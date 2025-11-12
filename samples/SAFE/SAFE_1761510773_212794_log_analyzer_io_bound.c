//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    if (timestamp == NULL) return 0;
    if (strlen(timestamp) >= sizeof(entry->timestamp)) return 0;
    strcpy(entry->timestamp, timestamp);
    
    char* level = strtok(NULL, " ");
    if (level == NULL) return 0;
    if (strlen(level) >= sizeof(entry->level)) return 0;
    strcpy(entry->level, level);
    
    char* message = strtok(NULL, "\n");
    if (message == NULL) return 0;
    if (strlen(message) >= sizeof(entry->message)) return 0;
    strcpy(entry->message, message);
    
    return 1;
}

int is_valid_level(const char* level) {
    if (level == NULL) return 0;
    return (strcmp(level, "INFO") == 0 || strcmp(level, "WARN") == 0 || 
            strcmp(level, "ERROR") == 0 || strcmp(level, "DEBUG") == 0);
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    
    printf("Log Analyzer - Enter log lines (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Supported levels: INFO, WARN, ERROR, DEBUG\n");
    printf("Enter 'END' on a separate line to finish input\n\n");
    
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
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (is_valid_level(entry.level)) {
                entries[entry_count] = entry;
                entry_count++;
                
                if (strcmp(entry.level, "INFO") == 0) info_count++;
                else if (strcmp(entry.level, "WARN") == 0) warn_count++;
                else if (strcmp(entry.level, "ERROR") == 0) error_count++;
                else if (strcmp(entry.level, "DEBUG") == 0) debug_count++;
                
                printf("Added: %s %s %s\n", entry.timestamp, entry.level, entry.message);
            } else {
                printf("Invalid log level. Supported: INFO, WARN, ERROR, DEBUG\n");
            }
        } else {
            printf("Invalid log format. Use: TIMESTAMP LEVEL MESSAGE\n");
        }
    }
    
    printf("\n=== Log Analysis Results ===\n");
    printf("Total entries: %d\n", entry_count);
    printf("INFO entries: %d\n", info_count);
    printf("WARN entries: %d\n", warn_count);
    printf("ERROR entries: %d\n", error_count);
    printf("DEBUG entries: %d\n", debug_count);
    
    if (entry_count > 0) {
        printf("\n=== Recent Entries ===\n");
        int display_count = (entry_count < 5) ? entry_count : 5;
        for (int i = entry_count - display_count; i < entry_count; i++) {
            printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }
    
    return 0;
}