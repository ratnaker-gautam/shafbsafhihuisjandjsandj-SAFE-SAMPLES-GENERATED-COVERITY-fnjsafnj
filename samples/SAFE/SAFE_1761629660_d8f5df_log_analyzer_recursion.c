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
    if (line == NULL || entry == NULL) return 0;
    
    size_t line_len = strlen(line);
    if (line_len >= MAX_LINE_LENGTH || line_len == 0) return 0;
    
    char temp[MAX_LINE_LENGTH];
    strncpy(temp, line, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';
    
    char* timestamp = strtok(temp, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= 19 || strlen(level) >= 9 || strlen(message) >= MAX_LINE_LENGTH - 1) {
        return 0;
    }
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    return 1;
}

void count_log_levels_recursive(LogEntry* entries, int index, int total, int* counts) {
    if (index >= total || entries == NULL || counts == NULL) return;
    
    if (strcmp(entries[index].level, "INFO") == 0) {
        counts[0]++;
    } else if (strcmp(entries[index].level, "WARN") == 0) {
        counts[1]++;
    } else if (strcmp(entries[index].level, "ERROR") == 0) {
        counts[2]++;
    } else {
        counts[3]++;
    }
    
    count_log_levels_recursive(entries, index + 1, total, counts);
}

void print_entries_by_level_recursive(LogEntry* entries, int index, int total, const char* target_level) {
    if (index >= total || entries == NULL || target_level == NULL) return;
    
    if (strcmp(entries[index].level, target_level) == 0) {
        printf("%s %s %s\n", entries[index].timestamp, entries[index].level, entries[index].message);
    }
    
    print_entries_by_level_recursive(entries, index + 1, total, target_level);
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE). Enter empty line to finish:\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strlen(line) == 0) break;
        
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
    
    int counts[4] = {0};
    count_log_levels_recursive(entries, 0, entry_count, counts);
    
    printf("\nLog Level Summary:\n");
    printf("INFO: %d\n", counts[0]);
    printf("WARN: %d\n", counts[1]);
    printf("ERROR: %d\n", counts[2]);
    printf("OTHER: %d\n", counts[3]);
    
    printf("\nEnter level to filter (INFO/WARN/ERROR/OTHER): ");
    char filter_level[10];
    if (fgets(filter_level, sizeof(filter_level), stdin) == NULL) {
        return 1;
    }
    
    size_t filter_len = strlen(filter_level);
    if (filter_len > 0 && filter_level[filter_len - 1] == '\n') {
        filter_level[filter_len - 1] = '\0';
    }
    
    printf("\nEntries with level '%s':\n", filter_level);
    print_entries_by_level_recursive(entries, 0, entry_count, filter_level);
    
    return 0;
}