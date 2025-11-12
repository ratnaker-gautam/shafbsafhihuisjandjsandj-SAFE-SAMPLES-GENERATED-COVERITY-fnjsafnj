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
    
    if (strlen(timestamp) >= 19 && strlen(level) >= 9) return 0;
    
    strncpy(entry->timestamp, timestamp, 19);
    entry->timestamp[19] = '\0';
    strncpy(entry->level, level, 9);
    entry->level[9] = '\0';
    strncpy(entry->message, message, MAX_LINE_LENGTH - 1);
    entry->message[MAX_LINE_LENGTH - 1] = '\0';
    
    return 1;
}

void print_log_entry(const LogEntry* entry) {
    if (!entry) return;
    printf("%s %s %s\n", entry->timestamp, entry->level, entry->message);
}

void analyze_logs_recursive(LogEntry* logs, int count, int index, const char* target_level) {
    if (index >= count || !logs || !target_level) return;
    
    if (strcmp(logs[index].level, target_level) == 0) {
        print_log_entry(&logs[index]);
    }
    
    analyze_logs_recursive(logs, count, index + 1, target_level);
}

int count_logs_by_level_recursive(LogEntry* logs, int count, int index, const char* target_level) {
    if (index >= count || !logs || !target_level) return 0;
    
    int current = (strcmp(logs[index].level, target_level) == 0) ? 1 : 0;
    return current + count_logs_by_level_recursive(logs, count, index + 1, target_level);
}

int main() {
    LogEntry logs[MAX_ENTRIES];
    int log_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    while (log_count < MAX_ENTRIES && fgets(line, MAX_LINE_LENGTH, stdin)) {
        if (strncmp(line, "END", 3) == 0 && (line[3] == '\n' || line[3] == '\0')) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) continue;
        
        if (parse_log_line(line, &logs[log_count])) {
            log_count++;
        } else {
            printf("Invalid log format. Skipping: %s\n", line);
        }
    }
    
    if (log_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", log_count);
    
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    int level_count = sizeof(levels) / sizeof(levels[0]);
    
    for (int i = 0; i < level_count; i++) {
        int count = count_logs_by_level_recursive(logs, log_count, 0, levels[i]);
        printf("%s: %d entries\n", levels[i], count);
    }
    
    printf("\nERROR entries:\n");
    analyze_logs_recursive(logs, log_count, 0, "ERROR");
    
    printf("\nWARN entries:\n");
    analyze_logs_recursive(logs, log_count, 0, "WARN");
    
    return 0;
}