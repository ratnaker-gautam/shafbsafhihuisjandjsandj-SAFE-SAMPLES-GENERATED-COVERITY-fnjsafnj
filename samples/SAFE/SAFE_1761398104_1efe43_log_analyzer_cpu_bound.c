//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

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
    
    if (strlen(timestamp) >= sizeof(entry->timestamp)) return 0;
    if (strlen(level) >= sizeof(entry->level)) return 0;
    if (strlen(message) >= sizeof(entry->message)) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    entry->error_count = 0;
    if (strcmp(level, "ERROR") == 0) {
        entry->error_count = 1;
    }
    
    return 1;
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int total_errors = 0;
    int total_warnings = 0;
    
    char line[MAX_LINE_LENGTH];
    int line_count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && entry_count < MAX_ENTRIES) {
        line_count++;
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (len >= MAX_LINE_LENGTH - 1) {
            continue;
        }
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count] = entry;
            entry_count++;
            
            if (strcmp(entry.level, "ERROR") == 0) {
                total_errors++;
            } else if (strcmp(entry.level, "WARNING") == 0) {
                total_warnings++;
            }
        }
    }
    
    if (ferror(stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    clock_t start_time = clock();
    
    for (int i = 0; i < entry_count; i++) {
        for (int j = i + 1; j < entry_count; j++) {
            if (strcmp(entries[i].level, entries[j].level) > 0) {
                LogEntry temp = entries[i];
                entries[i] = entries[j];
                entries[j] = temp;
            }
        }
    }
    
    int error_distribution[24] = {0};
    for (int i = 0; i < entry_count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            int hour;
            if (sscanf(entries[i].timestamp, "%*[^:]:%*d:%*d:%d", &hour) == 1) {
                if (hour >= 0 && hour < 24) {
                    error_distribution[hour]++;
                }
            }
        }
    }
    
    clock_t end_time = clock();
    double cpu_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    printf("Log Analysis Results:\n");
    printf("Total entries processed: %d\n", entry_count);
    printf("Total errors: %d\n", total_errors);
    printf("Total warnings: %d\n", total_warnings);
    printf("Processing time: %.6f seconds\n", cpu_time);
    
    printf("\nError distribution by hour:\n");
    for (int i = 0; i < 24; i++) {
        printf("Hour %02d: %d errors\n", i, error_distribution[i]);
    }
    
    printf("\nTop 5 error messages:\n");
    int printed = 0;
    for (int i = 0; i < entry_count && printed < 5; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            printf("%s: %s\n", entries[i].timestamp, entries[i].message);
            printed++;
        }
    }
    
    return 0;
}