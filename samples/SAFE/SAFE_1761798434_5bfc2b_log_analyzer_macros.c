//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define MAX_LEVEL_LEN 10
#define TIMESTAMP_LEN 20

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    char level[MAX_LEVEL_LEN];
    char message[MAX_LINE_LEN];
} LogEntry;

#define VALID_LEVEL(level) (strcmp(level, "INFO") == 0 || strcmp(level, "WARN") == 0 || strcmp(level, "ERROR") == 0)
#define IS_VALID_TIMESTAMP(ts) (strlen(ts) == 19 && ts[4] == '-' && ts[7] == '-' && ts[10] == ' ' && ts[13] == ':' && ts[16] == ':')

int parse_log_entry(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
    if (strlen(temp) < TIMESTAMP_LEN + MAX_LEVEL_LEN + 3) return 0;
    
    strncpy(entry->timestamp, temp, TIMESTAMP_LEN - 1);
    entry->timestamp[TIMESTAMP_LEN - 1] = '\0';
    
    if (!IS_VALID_TIMESTAMP(entry->timestamp)) return 0;
    
    const char* level_start = temp + TIMESTAMP_LEN + 1;
    if (*level_start != '[') return 0;
    
    level_start++;
    const char* level_end = strchr(level_start, ']');
    if (!level_end) return 0;
    
    size_t level_len = level_end - level_start;
    if (level_len >= MAX_LEVEL_LEN) return 0;
    
    strncpy(entry->level, level_start, level_len);
    entry->level[level_len] = '\0';
    
    if (!VALID_LEVEL(entry->level)) return 0;
    
    const char* message_start = level_end + 2;
    if (message_start >= temp + strlen(temp)) return 0;
    
    size_t message_len = strlen(message_start);
    if (message_len >= MAX_LINE_LEN) return 0;
    
    strncpy(entry->message, message_start, MAX_LINE_LEN - 1);
    entry->message[MAX_LINE_LEN - 1] = '\0';
    
    return 1;
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS [LEVEL] message):\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin)) {
        if (strncmp(line, "END", 3) == 0 && (line[3] == '\n' || line[3] == '\0')) {
            break;
        }
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (len <= 1) continue;
        
        LogEntry entry;
        if (parse_log_entry(line, &entry)) {
            entries[entry_count] = entry;
            entry_count++;
        } else {
            printf("Invalid log entry format: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 0;
    }
    
    int info_count = 0, warn_count = 0, error_count = 0;
    
    for (int i = 0; i < entry_count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        else if (strcmp(entries[i].level, "WARN") == 0) warn_count++;
        else if (strcmp(entries[i].level, "ERROR") == 0) error_count++;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    printf("INFO: %d\n", info_count);
    printf("WARN: %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    
    if (error_count > 0) {
        printf("\nError entries:\n");
        for (int i = 0; i < entry_count; i++) {
            if (strcmp(entries[i].level, "ERROR") == 0) {
                printf("%s %s\n", entries[i].timestamp, entries[i].message);
            }
        }
    }
    
    return 0;
}