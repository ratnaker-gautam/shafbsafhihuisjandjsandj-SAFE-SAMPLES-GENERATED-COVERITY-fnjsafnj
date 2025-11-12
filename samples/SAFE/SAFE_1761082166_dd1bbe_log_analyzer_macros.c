//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: log_analyzer
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

#define IS_VALID_LEVEL(l) (strcmp(l, "INFO") == 0 || strcmp(l, "WARN") == 0 || strcmp(l, "ERROR") == 0)

int parse_log_entry(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char level_buf[MAX_LEVEL_LEN];
    char timestamp_buf[DATE_LEN];
    char message_buf[MAX_LINE_LEN];
    
    int result = sscanf(line, "%19s %9s %1023[^\n]", timestamp_buf, level_buf, message_buf);
    if (result != 3) return 0;
    
    if (!IS_VALID_LEVEL(level_buf)) return 0;
    
    strncpy(entry->timestamp, timestamp_buf, DATE_LEN - 1);
    entry->timestamp[DATE_LEN - 1] = '\0';
    strncpy(entry->level, level_buf, MAX_LEVEL_LEN - 1);
    entry->level[MAX_LEVEL_LEN - 1] = '\0';
    strncpy(entry->message, message_buf, MAX_LINE_LEN - 1);
    entry->message[MAX_LINE_LEN - 1] = '\0';
    
    return 1;
}

#define COUNT_LEVEL(entries, count, target) do { \
    for (size_t i = 0; i < count; i++) { \
        if (strcmp(entries[i].level, target) == 0) { \
            printf("  %s: %s - %s\n", entries[i].timestamp, entries[i].level, entries[i].message); \
        } \
    } \
} while(0)

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    size_t entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Supported levels: INFO, WARN, ERROR\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
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
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %zu\n", entry_count);
    
    size_t info_count = 0, warn_count = 0, error_count = 0;
    for (size_t i = 0; i < entry_count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        else if (strcmp(entries[i].level, "WARN") == 0) warn_count++;
        else if (strcmp(entries[i].level, "ERROR") == 0) error_count++;
    }
    
    printf("INFO entries: %zu\n", info_count);
    printf("WARN entries: %zu\n", warn_count);
    printf("ERROR entries: %zu\n", error_count);
    
    printf("\nERROR entries:\n");
    COUNT_LEVEL(entries, entry_count, "ERROR");
    
    printf("\nWARN entries:\n");
    COUNT_LEVEL(entries, entry_count, "WARN");
    
    return 0;
}