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
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= 32 || strlen(level) >= 16 || strlen(message) >= 256) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    entry->error_count = 0;
    if (strcmp(level, "ERROR") == 0) {
        entry->error_count = 1;
    }
    
    return 1;
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int total_errors = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Log Analyzer - Processing simulated log data\n");
    printf("Enter log lines (format: TIMESTAMP LEVEL MESSAGE), empty line to finish:\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') line[len-1] = '\0';
        
        if (strlen(line) == 0) break;
        
        if (parse_log_line(line, &entries[entry_count])) {
            total_errors += entries[entry_count].error_count;
            entry_count++;
        } else {
            printf("Invalid log format. Skipping line.\n");
        }
    }
    
    printf("\nAnalysis Results:\n");
    printf("Total log entries processed: %d\n", entry_count);
    printf("Total ERROR entries: %d\n", total_errors);
    
    if (entry_count > 0) {
        double error_rate = (double)total_errors / entry_count * 100.0;
        printf("Error rate: %.2f%%\n", error_rate);
        
        int warning_count = 0;
        int info_count = 0;
        
        for (int i = 0; i < entry_count; i++) {
            if (strcmp(entries[i].level, "WARNING") == 0) warning_count++;
            if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        }
        
        printf("WARNING entries: %d\n", warning_count);
        printf("INFO entries: %d\n", info_count);
        
        printf("\nMost recent ERROR messages:\n");
        int printed = 0;
        for (int i = entry_count - 1; i >= 0 && printed < 3; i--) {
            if (entries[i].error_count > 0) {
                printf("- %s: %s\n", entries[i].timestamp, entries[i].message);
                printed++;
            }
        }
        if (printed == 0) {
            printf("No ERROR messages found.\n");
        }
    }
    
    return 0;
}