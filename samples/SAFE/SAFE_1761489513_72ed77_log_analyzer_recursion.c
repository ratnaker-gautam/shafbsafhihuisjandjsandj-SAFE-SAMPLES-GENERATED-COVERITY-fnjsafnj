//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 256
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
    
    if (strlen(timestamp) >= 19 || strlen(level) >= 9 || strlen(message) >= MAX_LINE_LENGTH - 1) {
        return 0;
    }
    
    strncpy(entry->timestamp, timestamp, 19);
    entry->timestamp[19] = '\0';
    strncpy(entry->level, level, 9);
    entry->level[9] = '\0';
    strncpy(entry->message, message, MAX_LINE_LENGTH - 1);
    entry->message[MAX_LINE_LENGTH - 1] = '\0';
    
    return 1;
}

void count_log_levels_recursive(LogEntry* entries, int start, int end, int* counts) {
    if (start > end) return;
    
    if (strcmp(entries[start].level, "ERROR") == 0) {
        counts[0]++;
    } else if (strcmp(entries[start].level, "WARN") == 0) {
        counts[1]++;
    } else if (strcmp(entries[start].level, "INFO") == 0) {
        counts[2]++;
    } else if (strcmp(entries[start].level, "DEBUG") == 0) {
        counts[3]++;
    }
    
    count_log_levels_recursive(entries, start + 1, end, counts);
}

void display_logs_recursive(LogEntry* entries, int start, int end, const char* filter_level) {
    if (start > end) return;
    
    if (filter_level == NULL || strcmp(entries[start].level, filter_level) == 0) {
        printf("%s %s %s\n", entries[start].timestamp, entries[start].level, entries[start].message);
    }
    
    display_logs_recursive(entries, start + 1, end, filter_level);
}

int read_log_file(const char* filename, LogEntry* entries, int max_entries) {
    FILE* file = fopen(filename, "r");
    if (!file) return -1;
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, MAX_LINE_LENGTH, file) && count < max_entries) {
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

int main() {
    LogEntry entries[MAX_ENTRIES];
    char filename[100];
    
    printf("Enter log filename: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error reading filename\n");
        return 1;
    }
    filename[strcspn(filename, "\n")] = '\0';
    
    if (strlen(filename) == 0) {
        printf("Invalid filename\n");
        return 1;
    }
    
    int entry_count = read_log_file(filename, entries, MAX_ENTRIES);
    if (entry_count <= 0) {
        printf("Error reading log file or file empty\n");
        return 1;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    
    int level_counts[4] = {0};
    count_log_levels_recursive(entries, 0, entry_count - 1, level_counts);
    
    printf("ERROR: %d\n", level_counts[0]);
    printf("WARN: %d\n", level_counts[1]);
    printf("INFO: %d\n", level_counts[2]);
    printf("DEBUG: %d\n", level_counts[3]);
    
    printf("\nEnter level to filter (or press Enter for all): ");
    char filter[20];
    if (fgets(filter, sizeof(filter), stdin) == NULL) {
        printf("Error reading filter\n");
        return 1;
    }
    filter[strcspn(filter, "\n")] = '\0';
    
    printf("\nFiltered Log Entries:\n");
    if (strlen(filter) == 0) {
        display_logs_recursive(entries, 0, entry_count - 1, NULL);
    } else {
        display_logs_recursive(entries, 0, entry_count - 1, filter);
    }
    
    return 0;
}