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
    
    char temp[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strcpy(temp, line);
    
    char* timestamp = strtok(temp, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= 19 || strlen(level) >= 9 || strlen(message) >= MAX_LINE_LENGTH - 1) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

void count_log_levels_recursive(LogEntry* entries, int index, int total, int* counts) {
    if (index >= total) return;
    
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

void display_logs_recursive(LogEntry* entries, int index, int total, const char* filter_level) {
    if (index >= total) return;
    
    if (filter_level == NULL || strcmp(entries[index].level, filter_level) == 0) {
        printf("%s %s %s\n", entries[index].timestamp, entries[index].level, entries[index].message);
    }
    
    display_logs_recursive(entries, index + 1, total, filter_level);
}

int read_log_file(const char* filename, LogEntry* entries, int max_entries) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) return 0;
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && count < max_entries) {
        line[strcspn(line, "\n")] = 0;
        
        if (parse_log_line(line, &entries[count])) {
            count++;
        }
    }
    
    fclose(file);
    return count;
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    char filename[100];
    
    printf("Enter log file name: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    filename[strcspn(filename, "\n")] = 0;
    
    if (strlen(filename) == 0) {
        printf("Invalid filename\n");
        return 1;
    }
    
    int entry_count = read_log_file(filename, entries, MAX_ENTRIES);
    if (entry_count == 0) {
        printf("No valid log entries found or file not accessible\n");
        return 1;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    
    int level_counts[4] = {0};
    count_log_levels_recursive(entries, 0, entry_count, level_counts);
    
    printf("INFO: %d\n", level_counts[0]);
    printf("WARN: %d\n", level_counts[1]);
    printf("ERROR: %d\n", level_counts[2]);
    printf("OTHER: %d\n", level_counts[3]);
    
    printf("\nDisplay logs for level (or 'all'): ");
    char level_filter[10];
    if (fgets(level_filter, sizeof(level_filter), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    level_filter[strcspn(level_filter, "\n")] = 0;
    
    printf("\nFiltered Log Entries:\n");
    if (strcmp(level_filter, "all") == 0) {
        display_logs_recursive(entries, 0, entry_count, NULL);
    } else {
        display_logs_recursive(entries, 0, entry_count, level_filter);
    }
    
    return 0;
}