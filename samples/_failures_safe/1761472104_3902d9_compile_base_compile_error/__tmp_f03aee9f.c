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
    strncpy(temp, line, MAX_LINE_LENGTH - 1);
    temp[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp_end = strchr(temp, ' ');
    if (timestamp_end == NULL) return 0;
    *timestamp_end = '\0';
    
    if (strlen(temp) >= sizeof(entry->timestamp)) return 0;
    strncpy(entry->timestamp, temp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    char* level_start = timestamp_end + 1;
    char* level_end = strchr(level_start, ' ');
    if (level_end == NULL) return 0;
    *level_end = '\0';
    
    if (strlen(level_start) >= sizeof(entry->level)) return 0;
    strncpy(entry->level, level_start, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    char* message_start = level_end + 1;
    if (strlen(message_start) >= sizeof(entry->message)) return 0;
    strncpy(entry->message, message_start, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
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

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a separate line to finish input.\n");
    
    char line[MAX_LINE_LENGTH];
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format. Please use: TIMESTAMP LEVEL MESSAGE\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    int level_counts[4] = {0};
    count_log_levels_recursive(entries, 0, entry_count - 1, level_counts);
    
    printf("\nLog Level Summary:\n");
    printf("ERROR: %d\n", level_counts[0]);
    printf("WARN:  %d\n", level_counts[1]);
    printf("INFO:  %d\n", level_counts[2]);
    printf("DEBUG: %d\n", level_counts[3]);
    
    printf("\nEnter log level to filter (ERROR/WARN/INFO/DEBUG or ALL): ");
    char filter[10];
    if (fgets(filter, sizeof(filter), stdin) == NULL) {
        printf("Input error.\n");
        return 1;
    }
    filter[strcspn(filter, "\n")] = '\0';
    
    printf("\nFiltered Log Entries:\n");
    if (strcmp(filter, "ALL") == 0) {
        display_logs_recursive(entries, 0, entry_count - 1, NULL);
    } else if (strcmp(filter, "ERROR") == 0 || strcmp(filter, "WARN") == 0 || 
               strcmp(filter, "INFO") == 0 || strcmp(filter, "DEBUG