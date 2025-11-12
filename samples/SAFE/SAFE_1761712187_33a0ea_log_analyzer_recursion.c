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
    
    if (strlen(timestamp) >= 19 && strlen(level) >= 2 && strlen(message) >= 1) {
        strncpy(entry->timestamp, timestamp, 19);
        entry->timestamp[19] = '\0';
        strncpy(entry->level, level, 9);
        entry->level[9] = '\0';
        strncpy(entry->message, message, MAX_LINE_LENGTH - 1);
        entry->message[MAX_LINE_LENGTH - 1] = '\0';
        return 1;
    }
    
    return 0;
}

void count_log_levels_recursive(LogEntry* entries, int index, int total, int* error_count, int* warning_count, int* info_count) {
    if (index >= total) return;
    
    if (strcmp(entries[index].level, "ERROR") == 0) {
        (*error_count)++;
    } else if (strcmp(entries[index].level, "WARNING") == 0) {
        (*warning_count)++;
    } else if (strcmp(entries[index].level, "INFO") == 0) {
        (*info_count)++;
    }
    
    count_log_levels_recursive(entries, index + 1, total, error_count, warning_count, info_count);
}

void print_logs_by_level_recursive(LogEntry* entries, int index, int total, const char* target_level) {
    if (index >= total) return;
    
    if (strcmp(entries[index].level, target_level) == 0) {
        printf("%s %s %s\n", entries[index].timestamp, entries[index].level, entries[index].message);
    }
    
    print_logs_by_level_recursive(entries, index + 1, total, target_level);
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE). Enter 'END' to finish:\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, MAX_LINE_LENGTH, stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) > 0) {
            LogEntry entry;
            if (parse_log_line(line, &entry)) {
                entries[entry_count] = entry;
                entry_count++;
            } else {
                printf("Invalid log format. Please use: TIMESTAMP LEVEL MESSAGE\n");
            }
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    int error_count = 0, warning_count = 0, info_count = 0;
    count_log_levels_recursive(entries, 0, entry_count, &error_count, &warning_count, &info_count);
    
    printf("\nLog Statistics:\n");
    printf("Total entries: %d\n", entry_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("\nEnter log level to filter (ERROR/WARNING/INFO): ");
    char filter_level[10];
    if (fgets(filter_level, sizeof(filter_level), stdin) != NULL) {
        filter_level[strcspn(filter_level, "\n")] = '\0';
        
        if (strcmp(filter_level, "ERROR") == 0 || 
            strcmp(filter_level, "WARNING") == 0 || 
            strcmp(filter_level, "INFO") == 0) {
            
            printf("\nLogs with level %s:\n", filter_level);
            print_logs_by_level_recursive(entries, 0, entry_count, filter_level);
        } else {
            printf("Invalid log level specified.\n");
        }
    }
    
    return 0;
}