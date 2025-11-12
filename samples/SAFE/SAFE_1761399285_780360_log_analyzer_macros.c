//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 256
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LEVEL_LEN 10

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    char level[LEVEL_LEN];
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - LEVEL_LEN - 3];
} LogEntry;

#define VALID_LEVEL(level) (strcmp(level, "INFO") == 0 || strcmp(level, "WARN") == 0 || strcmp(level, "ERROR") == 0)

int parse_log_entry(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char timestamp[TIMESTAMP_LEN];
    char level[LEVEL_LEN];
    char message[MAX_LINE_LEN];
    
    int result = sscanf(line, "%19s %9s %255[^\n]", timestamp, level, message);
    if (result != 3) return 0;
    
    if (strlen(timestamp) != 19 || timestamp[4] != '-' || timestamp[7] != '-' || 
        timestamp[10] != ' ' || timestamp[13] != ':' || timestamp[16] != ':') return 0;
    
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7 || i == 10 || i == 13 || i == 16) continue;
        if (!isdigit(timestamp[i])) return 0;
    }
    
    if (!VALID_LEVEL(level)) return 0;
    
    strncpy(entry->timestamp, timestamp, TIMESTAMP_LEN - 1);
    entry->timestamp[TIMESTAMP_LEN - 1] = '\0';
    strncpy(entry->level, level, LEVEL_LEN - 1);
    entry->level[LEVEL_LEN - 1] = '\0';
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

#define COUNT_LEVEL(entries, count, target_level) \
    do { \
        for (size_t i = 0; i < count; i++) { \
            if (strcmp(entries[i].level, target_level) == 0) { \
                printf("  %s: %s\n", entries[i].timestamp, entries[i].message); \
            } \
        } \
    } while(0)

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    size_t entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin) != NULL) {
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
        printf("No valid log entries found.\n");
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
    
    printf("INFO entries: %d\n", info_count);
    printf("WARN entries: %d\n", warn_count);
    printf("ERROR entries: %d\n", error_count);
    
    if (error_count > 0) {
        printf("\nERROR entries:\n");
        COUNT_LEVEL(entries, entry_count, "ERROR");
    }
    
    if (warn_count > 0) {
        printf("\nWARN entries:\n");
        COUNT_LEVEL(entries, entry_count, "WARN");
    }
    
    return 0;
}