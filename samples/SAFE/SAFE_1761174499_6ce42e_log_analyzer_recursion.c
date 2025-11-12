//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH];
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp[MAX_LINE_LENGTH];
    strncpy(temp, line, MAX_LINE_LENGTH - 1);
    temp[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp = strtok(temp, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    if (strlen(timestamp) >= 19) return 0;
    if (strlen(level) >= 9) return 0;
    if (strlen(message) >= MAX_LINE_LENGTH) return 0;
    
    strncpy(entry->timestamp, timestamp, 19);
    entry->timestamp[19] = '\0';
    strncpy(entry->level, level, 9);
    entry->level[9] = '\0';
    strncpy(entry->message, message, MAX_LINE_LENGTH - 1);
    entry->message[MAX_LINE_LENGTH - 1] = '\0';
    
    return 1;
}

void count_log_levels_recursive(LogEntry* entries, int index, int total, int* counts) {
    if (index >= total) return;
    
    if (strcmp(entries[index].level, "ERROR") == 0) {
        counts[0]++;
    } else if (strcmp(entries[index].level, "WARN") == 0) {
        counts[1]++;
    } else if (strcmp(entries[index].level, "INFO") == 0) {
        counts[2]++;
    } else if (strcmp(entries[index].level, "DEBUG") == 0) {
        counts[3]++;
    } else {
        counts[4]++;
    }
    
    count_log_levels_recursive(entries, index + 1, total, counts);
}

void display_entries_recursive(LogEntry* entries, int index, int total, const char* filter) {
    if (index >= total) return;
    
    if (!filter || strcmp(entries[index].level, filter) == 0) {
        printf("%s %s %s\n", entries[index].timestamp, entries[index].level, entries[index].message);
    }
    
    display_entries_recursive(entries, index + 1, total, filter);
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE), empty line to finish:\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(line, MAX_LINE_LENGTH, stdin)) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) break;
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format. Use: YYYY-MM-DD HH:MM:SS LEVEL message\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    int level_counts[5] = {0};
    count_log_levels_recursive(entries, 0, entry_count, level_counts);
    
    printf("\nLog Level Summary:\n");
    printf("ERROR: %d\n", level_counts[0]);
    printf("WARN: %d\n", level_counts[1]);
    printf("INFO: %d\n", level_counts[2]);
    printf("DEBUG: %d\n", level_counts[3]);
    printf("OTHER: %d\n", level_counts[4]);
    
    printf("\nEnter level to filter (ERROR/WARN/INFO/DEBUG/ALL): ");
    char filter[10];
    if (!fgets(filter, sizeof(filter), stdin)) {
        printf("Input error.\n");
        return 1;
    }
    filter[strcspn(filter, "\n")] = '\0';
    
    printf("\nFiltered Log Entries:\n");
    if (strcmp(filter, "ALL") == 0) {
        display_entries_recursive(entries, 0, entry_count, NULL);
    } else {
        display_entries_recursive(entries, 0, entry_count, filter);
    }
    
    return 0;
}