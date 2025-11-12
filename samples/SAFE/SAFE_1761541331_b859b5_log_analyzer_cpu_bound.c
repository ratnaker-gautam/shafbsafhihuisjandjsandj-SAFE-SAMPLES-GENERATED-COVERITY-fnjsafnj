//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 10000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
    int error_count;
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp)) return 0;
    if (strlen(level) >= sizeof(entry->level)) return 0;
    if (strlen(message) >= sizeof(entry->message)) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    entry->error_count = (strcmp(level, "ERROR") == 0) ? 1 : 0;
    
    return 1;
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int total_errors = 0;
    int total_warnings = 0;
    char line[MAX_LINE_LEN];
    
    printf("Log Analyzer - Processing log entries...\n");
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a new line to finish input.\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(line, sizeof(line), stdin)) {
            break;
        }
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
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
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    printf("\n=== Log Analysis Results ===\n");
    printf("Total entries processed: %d\n", entry_count);
    printf("Total errors: %d\n", total_errors);
    printf("Total warnings: %d\n", total_warnings);
    printf("Error rate: %.2f%%\n", entry_count > 0 ? (total_errors * 100.0) / entry_count : 0.0);
    
    printf("\n=== Error Summary ===\n");
    for (int i = 0; i < entry_count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            printf("%s: %s\n", entries[i].timestamp, entries[i].message);
        }
    }
    
    printf("\n=== Performance Statistics ===\n");
    clock_t start_time = clock();
    
    int processed_count = 0;
    for (int i = 0; i < entry_count; i++) {
        for (int j = 0; j < 1000; j++) {
            processed_count++;
        }
    }
    
    clock_t end_time = clock();
    double cpu_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    printf("CPU processing time: %.6f seconds\n", cpu_time);
    printf("Entries processed per second: %.0f\n", 
           cpu_time > 0 ? processed_count / cpu_time : 0);
    
    return 0;
}