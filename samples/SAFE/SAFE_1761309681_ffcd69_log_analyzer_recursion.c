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
    
    char* parts[3];
    parts[0] = strtok(temp, " ");
    parts[1] = strtok(NULL, " ");
    parts[2] = strtok(NULL, "\n");
    
    if (!parts[0] || !parts[1] || !parts[2]) return 0;
    
    if (strlen(parts[0]) >= 20 || strlen(parts[1]) >= 10 || strlen(parts[2]) >= MAX_LINE_LENGTH) {
        return 0;
    }
    
    strncpy(entry->timestamp, parts[0], 19);
    entry->timestamp[19] = '\0';
    strncpy(entry->level, parts[1], 9);
    entry->level[9] = '\0';
    strncpy(entry->message, parts[2], MAX_LINE_LENGTH - 1);
    entry->message[MAX_LINE_LENGTH - 1] = '\0';
    
    return 1;
}

void analyze_log_recursive(LogEntry* logs, int count, int index, int* error_count, int* warning_count, int* info_count) {
    if (index >= count) return;
    
    if (strcmp(logs[index].level, "ERROR") == 0) {
        (*error_count)++;
    } else if (strcmp(logs[index].level, "WARNING") == 0) {
        (*warning_count)++;
    } else if (strcmp(logs[index].level, "INFO") == 0) {
        (*info_count)++;
    }
    
    analyze_log_recursive(logs, count, index + 1, error_count, warning_count, info_count);
}

void display_logs_recursive(LogEntry* logs, int count, int index, const char* filter) {
    if (index >= count) return;
    
    if (!filter || strcmp(logs[index].level, filter) == 0) {
        printf("%s %s %s\n", logs[index].timestamp, logs[index].level, logs[index].message);
    }
    
    display_logs_recursive(logs, count, index + 1, filter);
}

int main() {
    LogEntry logs[MAX_ENTRIES];
    int log_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a separate line to finish input.\n");
    
    char line[MAX_LINE_LENGTH];
    while (log_count < MAX_ENTRIES) {
        if (!fgets(line, sizeof(line), stdin)) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        if (parse_log_line(line, &logs[log_count])) {
            log_count++;
        } else {
            printf("Invalid log format. Skipping: %s\n", line);
        }
    }
    
    if (log_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 0;
    }
    
    int error_count = 0, warning_count = 0, info_count = 0;
    analyze_log_recursive(logs, log_count, 0, &error_count, &warning_count, &info_count);
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", log_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("\nEnter level to filter (ERROR/WARNING/INFO) or 'ALL' for all entries: ");
    char filter[20];
    if (fgets(filter, sizeof(filter), stdin)) {
        filter[strcspn(filter, "\n")] = '\0';
        
        printf("\nFiltered Log Entries:\n");
        if (strcmp(filter, "ALL") == 0) {
            display_logs_recursive(logs, log_count, 0, NULL);
        } else if (strcmp(filter, "ERROR") == 0 || strcmp(filter, "WARNING") == 0 || strcmp(filter, "INFO") == 0) {
            display_logs_recursive(logs, log_count, 0, filter);
        } else {
            printf("Invalid filter. Showing all entries:\n");
            display_logs_recursive(logs, log_count, 0, NULL);
        }
    }
    
    return 0;
}