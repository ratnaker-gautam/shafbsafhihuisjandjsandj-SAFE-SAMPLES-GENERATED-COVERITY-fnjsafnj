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
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp[MAX_LINE_LENGTH];
    strncpy(temp, line, MAX_LINE_LENGTH - 1);
    temp[MAX_LINE_LENGTH - 1] = '\0';
    
    char* token = strtok(temp, " ");
    if (token == NULL) return 0;
    strncpy(entry->timestamp, token, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    token = strtok(NULL, " ");
    if (token == NULL) return 0;
    strncpy(entry->level, token, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    token = strtok(NULL, "\n");
    if (token == NULL) return 0;
    strncpy(entry->message, token, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

int count_log_level(const LogEntry* logs, int count, const char* level) {
    int total = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(logs[i].level, level) == 0) {
            total++;
        }
    }
    return total;
}

void analyze_log_patterns(const LogEntry* logs, int count) {
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strstr(logs[i].message, "error") != NULL) {
            error_count++;
        }
        if (strstr(logs[i].message, "warning") != NULL) {
            warning_count++;
        }
        if (strstr(logs[i].message, "info") != NULL) {
            info_count++;
        }
    }
    
    printf("Pattern analysis:\n");
    printf("  Messages containing 'error': %d\n", error_count);
    printf("  Messages containing 'warning': %d\n", warning_count);
    printf("  Messages containing 'info': %d\n", info_count);
}

int main(void) {
    LogEntry logs[MAX_ENTRIES];
    int log_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Log Analyzer - Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (log_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        if (parse_log_line(line, &logs[log_count])) {
            log_count++;
        } else {
            printf("Invalid log format. Skipping line.\n");
        }
    }
    
    if (log_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    printf("\nAnalysis Results:\n");
    printf("Total log entries: %d\n", log_count);
    
    int error_count = count_log_level(logs, log_count, "ERROR");
    int warning_count = count_log_level(logs, log_count, "WARNING");
    int info_count = count_log_level(logs, log_count, "INFO");
    
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("Error percentage: %.2f%%\n", (double)error_count / log_count * 100);
    printf("Warning percentage: %.2f%%\n", (double)warning_count / log_count * 100);
    
    analyze_log_patterns(logs, log_count);
    
    clock_t start_time = clock();
    
    for (int i = 0; i < 1000; i++) {
        for (int j = 0; j < log_count; j++) {
            if (strcmp(logs[j].level, "ERROR") == 0) {
                volatile int dummy = i + j;
            }
        }
    }
    
    clock_t end_time = clock();
    double cpu_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("CPU-intensive analysis completed in %.6f seconds\n", cpu_time);
    
    return 0;
}