//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[MAX_LINE_LENGTH - 48];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strcpy(temp, line);
    
    char* timestamp_end = strchr(temp, ' ');
    if (timestamp_end == NULL) return 0;
    *timestamp_end = '\0';
    if (strlen(temp) >= sizeof(entry->timestamp)) return 0;
    strcpy(entry->timestamp, temp);
    
    char* level_start = timestamp_end + 1;
    char* level_end = strchr(level_start, ' ');
    if (level_end == NULL) return 0;
    *level_end = '\0';
    if (strlen(level_start) >= sizeof(entry->level)) return 0;
    strcpy(entry->level, level_start);
    
    char* message_start = level_end + 1;
    if (strlen(message_start) >= sizeof(entry->message)) return 0;
    strcpy(entry->message, message_start);
    
    return 1;
}

int is_valid_level(const char* level) {
    return strcmp(level, "INFO") == 0 || 
           strcmp(level, "WARN") == 0 || 
           strcmp(level, "ERROR") == 0;
}

int main() {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int info_count = 0, warn_count = 0, error_count = 0;
    
    printf("Log Analyzer - Enter log lines (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Supported levels: INFO, WARN, ERROR\n");
    printf("Enter 'END' on a new line to finish input\n\n");
    
    char line[MAX_LINE_LENGTH];
    while (entry_count < MAX_ENTRIES) {
        printf("Log entry %d: ", entry_count + 1);
        
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
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
                
                printf("  Added: %s %s %s\n", entry.timestamp, entry.level, entry.message);
            } else {
                printf("  Invalid log level. Supported: INFO, WARN, ERROR\n");
            }
        } else {
            printf("  Invalid format. Use: TIMESTAMP LEVEL MESSAGE\n");
        }
    }
    
    printf("\n=== Log Analysis Results ===\n");
    printf("Total entries processed: %d\n", entry_count);
    printf("INFO entries: %d\n", info_count);
    printf("WARN entries: %d\n", warn_count);
    printf("ERROR entries: %d\n", error_count);
    
    if (entry_count > 0) {
        printf("\nRecent entries:\n");
        int start = (entry_count > 5) ? entry_count - 5 : 0;
        for (int i = start; i < entry_count; i++) {
            printf("%d. %s %s: %s\n", i + 1, entries[i].timestamp, 
                   entries[i].level, entries[i].message);
        }
    }
    
    return 0;
}