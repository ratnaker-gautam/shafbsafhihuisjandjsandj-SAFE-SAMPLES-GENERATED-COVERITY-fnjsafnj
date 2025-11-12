//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH - 30];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= 19 || strlen(level) >= 9 || strlen(message) >= (MAX_LINE_LENGTH - 30)) return 0;
    
    strncpy(entry->timestamp, timestamp, 19);
    entry->timestamp[19] = '\0';
    strncpy(entry->level, level, 9);
    entry->level[9] = '\0';
    strncpy(entry->message, message, MAX_LINE_LENGTH - 30 - 1);
    entry->message[MAX_LINE_LENGTH - 30 - 1] = '\0';
    
    return 1;
}

void count_log_levels(struct LogEntry* entries, int count, int* level_counts) {
    if (entries == NULL || level_counts == NULL) return;
    
    for (int i = 0; i < 4; i++) level_counts[i] = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) level_counts[0]++;
        else if (strcmp(entries[i].level, "WARN") == 0) level_counts[1]++;
        else if (strcmp(entries[i].level, "ERROR") == 0) level_counts[2]++;
        else level_counts[3]++;
    }
}

void display_summary(struct LogEntry* entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    int level_counts[4];
    count_log_levels(entries, count, level_counts);
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    printf("INFO: %d\n", level_counts[0]);
    printf("WARN: %d\n", level_counts[1]);
    printf("ERROR: %d\n", level_counts[2]);
    printf("OTHER: %d\n", level_counts[3]);
    
    printf("\nRecent entries:\n");
    int display_count = (count < 5) ? count : 5;
    for (int i = 0; i < display_count; i++) {
        printf("%s [%s] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
}

int read_log_file(struct LogEntry* entries, int max_entries) {
    if (entries == NULL || max_entries <= 0) return 0;
    
    printf("Enter log data (empty line to finish):\n");
    
    char line[MAX_LINE_LENGTH];
    int entry_count = 0;
    
    while (entry_count < max_entries) {
        if (fgets(line, MAX_LINE_LENGTH, stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) break;
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    return entry_count;
}

int main() {
    struct LogEntry entries[MAX_ENTRIES];
    
    printf("Log Analyzer\n");
    printf("Enter log lines in format: TIMESTAMP LEVEL MESSAGE\n");
    printf("Example: 2024-01-15T10:30:00 INFO User logged in\n");
    
    int entry_count = read_log_file(entries, MAX_ENTRIES);
    
    if (entry_count > 0) {
        display_summary(entries, entry_count);
    } else {
        printf("No valid log entries found.\n");
    }
    
    return 0;
}