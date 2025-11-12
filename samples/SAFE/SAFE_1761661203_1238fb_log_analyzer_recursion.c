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
    
    if (strlen(timestamp) >= 19 && strlen(level) < 10) {
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

void count_log_levels_recursive(LogEntry* entries, int index, int count, int* level_counts) {
    if (index >= count) return;
    
    if (strcmp(entries[index].level, "ERROR") == 0) {
        level_counts[0]++;
    } else if (strcmp(entries[index].level, "WARN") == 0) {
        level_counts[1]++;
    } else if (strcmp(entries[index].level, "INFO") == 0) {
        level_counts[2]++;
    } else if (strcmp(entries[index].level, "DEBUG") == 0) {
        level_counts[3]++;
    } else {
        level_counts[4]++;
    }
    
    count_log_levels_recursive(entries, index + 1, count, level_counts);
}

void display_logs_recursive(LogEntry* entries, int index, int count, const char* filter_level) {
    if (index >= count) return;
    
    if (filter_level == NULL || strcmp(entries[index].level, filter_level) == 0) {
        printf("%s %s %s\n", entries[index].timestamp, entries[index].level, entries[index].message);
    }
    
    display_logs_recursive(entries, index + 1, count, filter_level);
}

int read_log_file(const char* filename, LogEntry* entries, int max_entries) {
    FILE* file = fopen(filename, "r");
    if (!file) return 0;
    
    char line[MAX_LINE_LENGTH];
    int entry_count = 0;
    
    while (fgets(line, MAX_LINE_LENGTH, file) && entry_count < max_entries) {
        line[MAX_LINE_LENGTH - 1] = '\0';
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        }
    }
    
    fclose(file);
    return entry_count;
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    char filename[256];
    
    printf("Enter log file name: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    filename[strcspn(filename, "\n")] = '\0';
    
    if (strlen(filename) == 0) {
        printf("Invalid filename\n");
        return 1;
    }
    
    int entry_count = read_log_file(filename, entries, MAX_ENTRIES);
    
    if (entry_count == 0) {
        printf("No valid log entries found or file not accessible\n");
        return 1;
    }
    
    printf("\nFound %d log entries\n", entry_count);
    
    int level_counts[5] = {0};
    count_log_levels_recursive(entries, 0, entry_count, level_counts);
    
    printf("\nLog level summary:\n");
    printf("ERROR: %d\n", level_counts[0]);
    printf("WARN:  %d\n", level_counts[1]);
    printf("INFO:  %d\n", level_counts[2]);
    printf("DEBUG: %d\n", level_counts[3]);
    printf("OTHER: %d\n", level_counts[4]);
    
    printf("\nEnter log level to filter (or press Enter for all): ");
    char filter[20];
    if (fgets(filter, sizeof(filter), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    filter[strcspn(filter, "\n")] = '\0';
    
    printf("\nFiltered log entries:\n");
    if (strlen(filter) == 0) {
        display_logs_recursive(entries, 0, entry_count, NULL);
    } else {
        display_logs_recursive(entries, 0, entry_count, filter);
    }
    
    return 0;
}