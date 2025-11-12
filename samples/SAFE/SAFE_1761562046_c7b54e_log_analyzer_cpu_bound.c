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
    
    return 1;
}

int is_valid_level(const char* level) {
    return strcmp(level, "INFO") == 0 || 
           strcmp(level, "WARN") == 0 || 
           strcmp(level, "ERROR") == 0;
}

void analyze_logs(LogEntry* entries, int count) {
    int info_count = 0, warn_count = 0, error_count = 0;
    char first_timestamp[32] = "";
    char last_timestamp[32] = "";
    
    if (count > 0) {
        strcpy(first_timestamp, entries[0].timestamp);
        strcpy(last_timestamp, entries[count-1].timestamp);
    }
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        else if (strcmp(entries[i].level, "WARN") == 0) warn_count++;
        else if (strcmp(entries[i].level, "ERROR") == 0) error_count++;
    }
    
    printf("Log Analysis Results:\n");
    printf("Time range: %s to %s\n", first_timestamp, last_timestamp);
    printf("Total entries: %d\n", count);
    printf("INFO: %d\n", info_count);
    printf("WARN: %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    printf("INFO percentage: %.2f%%\n", count > 0 ? (info_count * 100.0) / count : 0.0);
    printf("WARN percentage: %.2f%%\n", count > 0 ? (warn_count * 100.0) / count : 0.0);
    printf("ERROR percentage: %.2f%%\n", count > 0 ? (error_count * 100.0) / count : 0.0);
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (one per line, format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Example: 2024-01-15T10:30:00 INFO System started\n");
    printf("Type 'END' on a separate line to finish input.\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin) != NULL) {
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) continue;
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (is_valid_level(entry.level)) {
                entries[entry_count] = entry;
                entry_count++;
            } else {
                printf("Invalid log level in: %s\n", line);
            }
        } else {
            printf("Failed to parse: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    clock_t start_time = clock();
    
    for (int i = 0; i < entry_count - 1; i++) {
        for (int j = 0; j < entry_count - i - 1; j++) {
            if (strcmp(entries[j].timestamp, entries[j+1].timestamp) > 0) {
                LogEntry temp = entries[j];
                entries[j] = entries[j+1];
                entries[j+1] = temp;
            }
        }
    }
    
    analyze_logs(entries, entry_count);
    
    clock_t end_time = clock();
    double cpu_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Analysis completed in %.6f seconds\n", cpu_time);
    
    return 0;
}