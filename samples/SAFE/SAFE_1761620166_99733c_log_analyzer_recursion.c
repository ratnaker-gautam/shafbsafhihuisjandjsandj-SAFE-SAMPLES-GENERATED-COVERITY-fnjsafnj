//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp[MAX_LINE_LENGTH];
    strncpy(temp, line, MAX_LINE_LENGTH - 1);
    temp[MAX_LINE_LENGTH - 1] = '\0';
    
    char* token = strtok(temp, " ");
    if (!token) return 0;
    
    strncpy(entry->timestamp, token, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    token = strtok(NULL, " ");
    if (!token) return 0;
    
    strncpy(entry->level, token, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    token = strtok(NULL, "\n");
    if (!token) return 0;
    
    strncpy(entry->message, token, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void count_log_levels_recursive(LogEntry* entries, int index, int total, int* counts) {
    if (index >= total) return;
    
    if (strcmp(entries[index].level, "INFO") == 0) {
        counts[0]++;
    } else if (strcmp(entries[index].level, "WARNING") == 0) {
        counts[1]++;
    } else if (strcmp(entries[index].level, "ERROR") == 0) {
        counts[2]++;
    }
    
    count_log_levels_recursive(entries, index + 1, total, counts);
}

void print_entries_by_level_recursive(LogEntry* entries, int index, int total, const char* level) {
    if (index >= total) return;
    
    if (strcmp(entries[index].level, level) == 0) {
        printf("%s %s %s\n", entries[index].timestamp, entries[index].level, entries[index].message);
    }
    
    print_entries_by_level_recursive(entries, index + 1, total, level);
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter 'END' on a new line to finish input.\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(line, sizeof(line), stdin)) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format. Skipping: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    int level_counts[3] = {0};
    count_log_levels_recursive(entries, 0, entry_count, level_counts);
    
    printf("\nLog Analysis Summary:\n");
    printf("Total entries: %d\n", entry_count);
    printf("INFO entries: %d\n", level_counts[0]);
    printf("WARNING entries: %d\n", level_counts[1]);
    printf("ERROR entries: %d\n", level_counts[2]);
    
    printf("\nINFO entries:\n");
    print_entries_by_level_recursive(entries, 0, entry_count, "INFO");
    
    printf("\nWARNING entries:\n");
    print_entries_by_level_recursive(entries, 0, entry_count, "WARNING");
    
    printf("\nERROR entries:\n");
    print_entries_by_level_recursive(entries, 0, entry_count, "ERROR");
    
    return 0;
}