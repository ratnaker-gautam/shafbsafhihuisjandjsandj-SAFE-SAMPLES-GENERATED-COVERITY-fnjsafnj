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
    if (!line || !entry) return 0;
    
    char temp[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN - 1) return 0;
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
    char* timestamp = strtok(temp, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "");
    
    if (!timestamp || !level || !message) return 0;
    
    if (strlen(timestamp) >= TIMESTAMP_LEN - 1) return 0;
    if (strlen(level) >= LEVEL_LEN - 1) return 0;
    if (strlen(message) >= sizeof(entry->message) - 1) return 0;
    
    if (!VALID_LEVEL(level)) return 0;
    
    strncpy(entry->timestamp, timestamp, TIMESTAMP_LEN - 1);
    entry->timestamp[TIMESTAMP_LEN - 1] = '\0';
    strncpy(entry->level, level, LEVEL_LEN - 1);
    entry->level[LEVEL_LEN - 1] = '\0';
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

#define PRINT_STATS(counts) \
    printf("INFO: %d\n", counts[0]); \
    printf("WARN: %d\n", counts[1]); \
    printf("ERROR: %d\n", counts[2]); \
    printf("TOTAL: %d\n", counts[0] + counts[1] + counts[2])

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int level_counts[3] = {0};
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (empty line to finish):\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin)) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strlen(line) == 0) {
            break;
        }
        
        if (parse_log_entry(line, &entries[entry_count])) {
            if (strcmp(entries[entry_count].level, "INFO") == 0) {
                level_counts[0]++;
            } else if (strcmp(entries[entry_count].level, "WARN") == 0) {
                level_counts[1]++;
            } else if (strcmp(entries[entry_count].level, "ERROR") == 0) {
                level_counts[2]++;
            }
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    PRINT_STATS(level_counts);
    
    printf("\nRecent entries:\n");
    int display_count = (entry_count < 5) ? entry_count : 5;
    for (int i = entry_count - display_count; i < entry_count; i++) {
        printf("%s [%s] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
    
    return 0;
}