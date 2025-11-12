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
    if (timestamp == NULL) return 0;
    
    char* level = strtok(NULL, " ");
    if (level == NULL) return 0;
    
    char* message = strtok(NULL, "\n");
    if (message == NULL) return 0;
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

int count_log_level(const LogEntry* entries, int count, const char* level) {
    int total = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, level) == 0) {
            total++;
        }
    }
    return total;
}

void analyze_log_patterns(const LogEntry* entries, int count) {
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strstr(entries[i].message, "error") != NULL ||
            strstr(entries[i].message, "failed") != NULL) {
            error_count++;
        } else if (strstr(entries[i].message, "warning") != NULL) {
            warning_count++;
        } else if (strstr(entries[i].message, "info") != NULL) {
            info_count++;
        }
    }
    
    printf("Pattern analysis:\n");
    printf("  Messages containing 'error' or 'failed': %d\n", error_count);
    printf("  Messages containing 'warning': %d\n", warning_count);
    printf("  Messages containing 'info': %d\n", info_count);
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Log Analyzer - Enter log lines (empty line to finish):\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        if (line[0] == '\n') {
            break;
        }
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format. Expected: TIMESTAMP LEVEL MESSAGE\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    printf("\nAnalysis Results:\n");
    printf("Total log entries: %d\n", entry_count);
    
    int error_count = count_log_level(entries, entry_count, "ERROR");
    int warning_count = count_log_level(entries, entry_count, "WARN");
    int info_count = count_log_level(entries, entry_count, "INFO");
    
    printf("ERROR entries: %d\n", error_count);
    printf("WARN entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("Error rate: %.2f%%\n", (double)error_count / entry_count * 100);
    
    analyze_log_patterns(entries, entry_count);
    
    clock_t start = clock();
    
    for (int i = 0; i < entry_count; i++) {
        for (int j = i + 1; j < entry_count; j++) {
            if (strcmp(entries[i].level, entries[j].level) > 0) {
                LogEntry temp = entries[i];
                entries[i] = entries[j];
                entries[j] = temp;
            }
        }
    }
    
    clock_t end = clock();
    double cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    printf("\nSorted by level (CPU time: %.6f seconds)\n", cpu_time);
    printf("First 5 entries after sorting:\n");
    for (int i = 0; i < 5 && i < entry_count; i++) {
        printf("  %s %s %s\n", entries[i].timestamp, entries[i].level,