//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strcpy(temp_line, line);
    
    char* timestamp_start = strchr(temp_line, '[');
    if (timestamp_start == NULL) return 0;
    timestamp_start++;
    
    char* timestamp_end = strchr(timestamp_start, ']');
    if (timestamp_end == NULL) return 0;
    *timestamp_end = '\0';
    
    if (strlen(timestamp_start) >= sizeof(entry->timestamp)) return 0;
    strcpy(entry->timestamp, timestamp_start);
    
    char* level_start = timestamp_end + 1;
    while (*level_start == ' ') level_start++;
    
    char* level_end = strchr(level_start, ' ');
    if (level_end == NULL) return 0;
    
    size_t level_len = level_end - level_start;
    if (level_len >= sizeof(entry->level)) return 0;
    strncpy(entry->level, level_start, level_len);
    entry->level[level_len] = '\0';
    
    char* message_start = level_end + 1;
    while (*message_start == ' ') message_start++;
    
    if (strlen(message_start) >= sizeof(entry->message)) return 0;
    strcpy(entry->message, message_start);
    
    return 1;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    char line[MAX_LINE_LENGTH];
    
    printf("Log Analyzer - Enter log lines (format: [timestamp] level message)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count] = entry;
            entry_count++;
            
            if (strcmp(entry.level, "ERROR") == 0) {
                error_count++;
            } else if (strcmp(entry.level, "WARNING") == 0) {
                warning_count++;
            } else if (strcmp(entry.level, "INFO") == 0) {
                info_count++;
            }
        }
    }
    
    printf("\n=== Log Analysis Results ===\n");
    printf("Total entries: %d\n", entry_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    if (entry_count > 0) {
        printf("\n=== Recent Entries ===\n");
        int display_count = (entry_count < 5) ? entry_count : 5;
        for (int i = entry_count - display_count; i < entry_count; i++) {
            printf("[%s] %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }
    
    return 0;
}