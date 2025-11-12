//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
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
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void count_log_levels_recursive(LogEntry* entries, int count, int index, int* info_count, int* warn_count, int* error_count) {
    if (index >= count) return;
    
    if (strcmp(entries[index].level, "INFO") == 0) {
        (*info_count)++;
    } else if (strcmp(entries[index].level, "WARN") == 0) {
        (*warn_count)++;
    } else if (strcmp(entries[index].level, "ERROR") == 0) {
        (*error_count)++;
    }
    
    count_log_levels_recursive(entries, count, index + 1, info_count, warn_count, error_count);
}

void print_logs_by_level_recursive(LogEntry* entries, int count, int index, const char* target_level) {
    if (index >= count) return;
    
    if (strcmp(entries[index].level, target_level) == 0) {
        printf("%s %s %s\n", entries[index].timestamp, entries[index].level, entries[index].message);
    }
    
    print_logs_by_level_recursive(entries, count, index + 1, target_level);
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE). Enter empty line to finish:\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(line, sizeof(line), stdin)) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) break;
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format. Use: TIMESTAMP LEVEL MESSAGE\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    int info_count = 0, warn_count = 0, error_count = 0;
    count_log_levels_recursive(entries, entry_count, 0, &info_count, &warn_count, &error_count);
    
    printf("\nLog Statistics:\n");
    printf("INFO entries: %d\n", info_count);
    printf("WARN entries: %d\n", warn_count);
    printf("ERROR entries: %d\n", error_count);
    printf("Total entries: %d\n", entry_count);
    
    printf("\nEnter log level to filter (INFO/WARN/ERROR): ");
    char filter_level[16];
    if (fgets(filter_level, sizeof(filter_level), stdin)) {
        filter_level[strcspn(filter_level, "\n")] = '\0';
        
        if (strcmp(filter_level, "INFO") == 0 || strcmp(filter_level, "WARN") == 0 || strcmp(filter_level, "ERROR") == 0) {
            printf("\n%s entries:\n", filter_level);
            print_logs_by_level_recursive(entries, entry_count, 0, filter_level);
        } else {
            printf("Invalid log level specified.\n");
        }
    }
    
    return 0;
}