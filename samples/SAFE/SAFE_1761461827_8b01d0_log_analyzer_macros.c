//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define MAX_LEVEL_LEN 10
#define DATE_LEN 20

typedef struct {
    char timestamp[DATE_LEN];
    char level[MAX_LEVEL_LEN];
    char message[MAX_LINE_LEN];
} LogEntry;

#define IS_VALID_LEVEL(lvl) (strcmp(lvl, "INFO") == 0 || strcmp(lvl, "WARN") == 0 || strcmp(lvl, "ERROR") == 0)

int parse_log_entry(const char* line, LogEntry* entry) {
    char level_buf[MAX_LEVEL_LEN];
    char timestamp_buf[DATE_LEN];
    int scanned = sscanf(line, "%19s %9s", timestamp_buf, level_buf);
    if (scanned != 2) return 0;
    
    if (!IS_VALID_LEVEL(level_buf)) return 0;
    
    strncpy(entry->timestamp, timestamp_buf, DATE_LEN - 1);
    entry->timestamp[DATE_LEN - 1] = '\0';
    strncpy(entry->level, level_buf, MAX_LEVEL_LEN - 1);
    entry->level[MAX_LEVEL_LEN - 1] = '\0';
    
    const char* msg_start = strchr(line, ']');
    if (!msg_start) return 0;
    msg_start++;
    while (*msg_start && isspace(*msg_start)) msg_start++;
    
    strncpy(entry->message, msg_start, MAX_LINE_LEN - 1);
    entry->message[MAX_LINE_LEN - 1] = '\0';
    
    return 1;
}

#define COUNT_LEVEL(entries, count, target) \
    do { \
        for (size_t i = 0; i < count; i++) { \
            if (strcmp(entries[i].level, target) == 0) { \
                printf("  %s: %s - %s\n", entries[i].timestamp, entries[i].level, entries[i].message); \
            } \
        } \
    } while(0)

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    size_t entry_count = 0;
    
    printf("Enter log entries (format: [TIMESTAMP] LEVEL Message). Empty line to finish:\n");
    
    char line[MAX_LINE_LEN];
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(line, sizeof(line), stdin)) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (line[0] == '\0') break;
        
        if (parse_log_entry(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 0;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %zu\n", entry_count);
    
    int info_count = 0, warn_count = 0, error_count = 0;
    for (size_t i = 0; i < entry_count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        else if (strcmp(entries[i].level, "WARN") == 0) warn_count++;
        else if (strcmp(entries[i].level, "ERROR") == 0) error_count++;
    }
    
    printf("INFO: %d\n", info_count);
    printf("WARN: %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    
    printf("\nERROR entries:\n");
    COUNT_LEVEL(entries, entry_count, "ERROR");
    
    printf("\nWARN entries:\n");
    COUNT_LEVEL(entries, entry_count, "WARN");
    
    return 0;
}