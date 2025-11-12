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
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* tokens[3];
    char* token = strtok(temp_line, "|");
    int token_count = 0;
    
    while (token != NULL && token_count < 3) {
        tokens[token_count++] = token;
        token = strtok(NULL, "|");
    }
    
    if (token_count != 3) return 0;
    
    if (strlen(tokens[0]) >= 31) return 0;
    strncpy(entry->timestamp, tokens[0], 31);
    entry->timestamp[31] = '\0';
    
    if (strlen(tokens[1]) >= 15) return 0;
    strncpy(entry->level, tokens[1], 15);
    entry->level[15] = '\0';
    
    if (strlen(tokens[2]) >= 255) return 0;
    strncpy(entry->message, tokens[2], 255);
    entry->message[255] = '\0';
    
    entry->error_count = 0;
    
    return 1;
}

int is_error_level(const char* level) {
    if (level == NULL) return 0;
    return (strcmp(level, "ERROR") == 0 || strcmp(level, "FATAL") == 0);
}

void analyze_logs(LogEntry* entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    int total_errors = 0;
    int total_warnings = 0;
    int total_info = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            total_errors++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            total_warnings++;
        } else if (strcmp(entries[i].level, "INFO") == 0) {
            total_info++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries processed: %d\n", count);
    printf("INFO messages: %d\n", total_info);
    printf("WARNING messages: %d\n", total_warnings);
    printf("ERROR messages: %d\n", total_errors);
    
    if (count > 0) {
        printf("Error rate: %.2f%%\n", (total_errors * 100.0) / count);
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    const char* sample_logs[] = {
        "2024-01-15 10:30:00|INFO|System startup completed",
        "2024-01-15 10:31:15|WARNING|High memory usage detected",
        "2024-01-15 10:32:00|INFO|User login successful",
        "2024-01-15 10:33:45|ERROR|Database connection failed",
        "2024-01-15 10:34:20|INFO|Backup process started",
        "2024-01-15 10:35:10|ERROR|File system full",
        "2024-01-15 10:36:00|INFO|Cache cleared",
        "2024-01-15 10:37:30|WARNING|Network latency high",
        "2024-01-15 10:38:15|INFO|Task completed successfully",
        "2024-01-15 10:39:00|ERROR|Authentication service unavailable"
    };
    
    int sample_count = sizeof(sample_logs) / sizeof(sample_logs[0]);
    
    for (int i = 0; i < sample_count && entry_count < MAX_ENTRIES; i++) {
        if (parse_log_line(sample_logs[i], &entries[entry_count])) {
            entry_count++;
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    analyze_logs(entries, entry_count);
    
    printf("\nError messages found:\n");
    for (int i = 0; i < entry_count; i++) {
        if (is_error_level(entries[i].level)) {
            printf("- %s: %s\n", entries[i].timestamp, entries[i].message);
        }
    }
    
    return 0;
}