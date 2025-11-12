//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

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
    if (!entries || !info_count || !warn_count || !error_count) return;
    
    if (strcmp(entries[index].level, "INFO") == 0) {
        if (*info_count < INT_MAX) (*info_count)++;
    } else if (strcmp(entries[index].level, "WARN") == 0) {
        if (*warn_count < INT_MAX) (*warn_count)++;
    } else if (strcmp(entries[index].level, "ERROR") == 0) {
        if (*error_count < INT_MAX) (*error_count)++;
    }
    
    count_log_levels_recursive(entries, count, index + 1, info_count, warn_count, error_count);
}

void print_entries_recursive(LogEntry* entries, int count, int index, const char* filter_level) {
    if (index >= count) return;
    if (!entries) return;
    
    if (filter_level == NULL || strcmp(entries[index].level, filter_level) == 0) {
        printf("%s %s %s\n", entries[index].timestamp, entries[index].level, entries[index].message);
    }
    
    print_entries_recursive(entries, count, index + 1, filter_level);
}

int read_log_file(const char* filename, LogEntry* entries, int max_entries) {
    if (!filename || !entries || max_entries <= 0) return 0;
    
    FILE* file = fopen(filename, "r");
    if (!file) return 0;
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) && count < max_entries) {
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) > 0) {
            if (parse_log_line(line, &entries[count])) {
                count++;
            }
        }
    }
    
    fclose(file);
    return count;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    LogEntry entries[MAX_ENTRIES];
    int entry_count = read_log_file(argv[1], entries, MAX_ENTRIES);
    
    if (entry_count == 0) {
        printf("No valid log entries found or file cannot be read.\n");
        return 1;
    }
    
    int info_count = 0, warn_count = 0, error_count = 0;
    count_log_levels_recursive(entries, entry_count, 0, &info_count, &warn_count, &error_count);
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    printf("INFO: %d\n", info_count);
    printf("WARN: %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    
    printf("\nAll ERROR entries:\n");
    print_entries_recursive(entries, entry_count, 0, "ERROR");
    
    return 0;
}