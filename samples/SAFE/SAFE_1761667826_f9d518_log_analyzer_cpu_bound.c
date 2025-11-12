//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 10000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
    int error_count;
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    entry->error_count = 0;
    if (strstr(message, "error") != NULL || strstr(message, "ERROR") != NULL) {
        entry->error_count = 1;
    }
    
    return 1;
}

int compare_entries(const void* a, const void* b) {
    const LogEntry* entry_a = (const LogEntry*)a;
    const LogEntry* entry_b = (const LogEntry*)b;
    return strcmp(entry_a->timestamp, entry_b->timestamp);
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int total_errors = 0;
    
    char line[MAX_LINE_LENGTH];
    int line_count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && entry_count < MAX_ENTRIES) {
        line_count++;
        if (strlen(line) == 0 || line[0] == '\n') continue;
        
        if (parse_log_line(line, &entries[entry_count])) {
            total_errors += entries[entry_count].error_count;
            entry_count++;
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    qsort(entries, entry_count, sizeof(LogEntry), compare_entries);
    
    printf("Log Analysis Results:\n");
    printf("Total entries processed: %d\n", entry_count);
    printf("Total lines read: %d\n", line_count);
    printf("Total errors detected: %d\n", total_errors);
    printf("\n");
    
    printf("First 5 entries:\n");
    for (int i = 0; i < 5 && i < entry_count; i++) {
        printf("%s [%s] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
    
    printf("\nLast 5 entries:\n");
    for (int i = (entry_count > 5 ? entry_count - 5 : 0); i < entry_count; i++) {
        printf("%s [%s] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
    
    printf("\nError summary by level:\n");
    int error_levels[4] = {0};
    char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    
    for (int i = 0; i < entry_count; i++) {
        for (int j = 0; j < 4; j++) {
            if (strcmp(entries[i].level, levels[j]) == 0) {
                error_levels[j] += entries[i].error_count;
                break;
            }
        }
    }
    
    for (int i = 0; i < 4; i++) {
        printf("%s: %d errors\n", levels[i], error_levels[i]);
    }
    
    return 0;
}