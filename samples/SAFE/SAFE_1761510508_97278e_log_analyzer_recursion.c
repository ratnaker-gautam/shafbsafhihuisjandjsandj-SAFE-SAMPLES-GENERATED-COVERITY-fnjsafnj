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

int read_log_file(const char* filename, LogEntry* entries, int max_entries) {
    if (!filename || !entries) return 0;
    
    FILE* file = fopen(filename, "r");
    if (!file) return 0;
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, MAX_LINE_LENGTH, file) && count < max_entries) {
        line[MAX_LINE_LENGTH - 1] = '\0';
        if (parse_log_line(line, &entries[count])) {
            count++;
        }
    }
    
    fclose(file);
    return count;
}

void print_statistics(int* counts) {
    printf("Log Level Statistics:\n");
    printf("ERROR: %d\n", counts[0]);
    printf("WARN:  %d\n", counts[1]);
    printf("INFO:  %d\n", counts[2]);
    printf("DEBUG: %d\n", counts[3]);
    printf("OTHER: %d\n", counts[4]);
    
    int total = counts[0] + counts[1] + counts[2] + counts[3] + counts[4];
    if (total > 0) {
        printf("\nTotal entries: %d\n", total);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    LogEntry entries[MAX_ENTRIES];
    int level_counts[5] = {0};
    
    int entry_count = read_log_file(argv[1], entries, MAX_ENTRIES);
    if (entry_count == 0) {
        printf("No valid log entries found or file cannot be read.\n");
        return 1;
    }
    
    count_log_levels_recursive(entries, 0, entry_count, level_counts);
    print_statistics(level_counts);
    
    return 0;
}