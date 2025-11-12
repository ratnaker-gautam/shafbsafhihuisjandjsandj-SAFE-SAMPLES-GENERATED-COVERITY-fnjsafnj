//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
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
    
    char buffer[MAX_LINE_LENGTH];
    strncpy(buffer, line, MAX_LINE_LENGTH - 1);
    buffer[MAX_LINE_LENGTH - 1] = '\0';
    
    char* token = strtok(buffer, " ");
    if (token == NULL) return 0;
    
    strncpy(entry->timestamp, token, 31);
    entry->timestamp[31] = '\0';
    
    token = strtok(NULL, " ");
    if (token == NULL) return 0;
    
    strncpy(entry->level, token, 15);
    entry->level[15] = '\0';
    
    token = strtok(NULL, "\n");
    if (token == NULL) return 0;
    
    strncpy(entry->message, token, 255);
    entry->message[255] = '\0';
    
    entry->error_count = 0;
    if (strcmp(entry->level, "ERROR") == 0) {
        entry->error_count = 1;
    }
    
    return 1;
}

void analyze_logs(LogEntry* entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    char first_timestamp[32] = "";
    char last_timestamp[32] = "";
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        }
        
        if (i == 0) {
            strncpy(first_timestamp, entries[i].timestamp, 31);
            first_timestamp[31] = '\0';
        }
        if (i == count - 1) {
            strncpy(last_timestamp, entries[i].timestamp, 31);
            last_timestamp[31] = '\0';
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("Error entries: %d\n", error_count);
    printf("Warning entries: %d\n", warning_count);
    printf("Info entries: %d\n", info_count);
    printf("Time range: %s to %s\n", first_timestamp, last_timestamp);
    
    double error_rate = (double)error_count / count * 100.0;
    printf("Error rate: %.2f%%\n", error_rate);
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (one per line, format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    char line[MAX_LINE_LENGTH];
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, MAX_LINE_LENGTH, stdin) == NULL) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format. Skipping line: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    printf("\nProcessing %d log entries...\n", entry_count);
    
    clock_t start_time = clock();
    
    analyze_logs(entries, entry_count);
    
    clock_t end_time = clock();
    double cpu_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    printf("Analysis completed in %.6f seconds\n", cpu_time);
    
    return 0;
}