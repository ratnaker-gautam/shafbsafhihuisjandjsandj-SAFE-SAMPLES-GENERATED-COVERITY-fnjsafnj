//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LEVEL_LEN 16

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    char level[LEVEL_LEN];
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - LEVEL_LEN];
} LogEntry;

#define IS_VALID_LEVEL(l) (strcmp(l, "INFO") == 0 || strcmp(l, "WARNING") == 0 || strcmp(l, "ERROR") == 0)

int parse_log_entry(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "");
    
    if (!timestamp || !level || !message) return 0;
    if (strlen(timestamp) >= TIMESTAMP_LEN - 1) return 0;
    if (strlen(level) >= LEVEL_LEN - 1) return 0;
    if (strlen(message) >= sizeof(entry->message) - 1) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return IS_VALID_LEVEL(entry->level);
}

#define PRINT_STATS(counts) \
    printf("INFO: %d\n", counts[0]); \
    printf("WARNING: %d\n", counts[1]); \
    printf("ERROR: %d\n", counts[2])

int main() {
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
            if (strcmp(entries[entry_count].level, "INFO") == 0) level_counts[0]++;
            else if (strcmp(entries[entry_count].level, "WARNING") == 0) level_counts[1]++;
            else if (strcmp(entries[entry_count].level, "ERROR") == 0) level_counts[2]++;
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
    printf("Total entries: %d\n", entry_count);
    PRINT_STATS(level_counts);
    
    printf("\nRecent ERROR entries:\n");
    int error_count = 0;
    for (int i = entry_count - 1; i >= 0 && error_count < 5; i--) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            error_count++;
        }
    }
    
    if (error_count == 0) {
        printf("No ERROR entries found.\n");
    }
    
    return 0;
}