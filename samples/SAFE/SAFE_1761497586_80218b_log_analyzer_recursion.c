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
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    strcpy(temp, line);
    
    char* timestamp = strtok(temp, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= 19 || strlen(level) >= 9 || strlen(message) >= MAX_LINE_LENGTH) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

void count_log_levels_recursive(LogEntry* entries, int index, int total, int* counts) {
    if (index >= total) return;
    
    if (strcmp(entries[index].level, "INFO") == 0) counts[0]++;
    else if (strcmp(entries[index].level, "WARN") == 0) counts[1]++;
    else if (strcmp(entries[index].level, "ERROR") == 0) counts[2]++;
    else counts[3]++;
    
    count_log_levels_recursive(entries, index + 1, total, counts);
}

void display_logs_recursive(LogEntry* entries, int index, int total, const char* filter) {
    if (index >= total) return;
    
    if (filter == NULL || strcmp(entries[index].level, filter) == 0) {
        printf("%s %s %s\n", entries[index].timestamp, entries[index].level, entries[index].message);
    }
    
    display_logs_recursive(entries, index + 1, total, filter);
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE), one per line.\n");
    printf("Enter 'END' on a separate line to finish.\n");
    
    char line[MAX_LINE_LENGTH];
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
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
    
    printf("\nAll log entries:\n");
    display_logs_recursive(entries, 0, entry_count, NULL);
    
    printf("\nERROR entries only:\n");
    display_logs_recursive(entries, 0, entry_count, "ERROR");
    
    return 0;
}