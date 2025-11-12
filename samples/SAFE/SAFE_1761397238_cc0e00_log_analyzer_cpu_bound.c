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
    if (!line || !entry) return 0;
    
    char temp[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strcpy(temp, line);
    
    char* timestamp = strtok(temp, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    if (strlen(timestamp) >= 32 || strlen(level) >= 16 || strlen(message) >= 256) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
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
        if (strstr(entries[i].message, "error") != NULL) {
            error_count++;
        }
        if (strstr(entries[i].message, "warning") != NULL) {
            warning_count++;
        }
        if (strstr(entries[i].message, "info") != NULL) {
            info_count++;
        }
    }
    
    printf("Pattern analysis:\n");
    printf("  Messages containing 'error': %d\n", error_count);
    printf("  Messages containing 'warning': %d\n", warning_count);
    printf("  Messages containing 'info': %d\n", info_count);
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Log Analyzer - Enter log lines (empty line to finish):\n");
    
    char line[MAX_LINE_LENGTH];
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(line, sizeof(line), stdin)) {
            break;
        }
        
        line[strcspn(line, "\n")] = 0;
        
        if (strlen(line) == 0) {
            break;
        }
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format, skipping: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    printf("\nAnalysis Results:\n");
    printf("Total log entries: %d\n", entry_count);
    
    int error_count = count_log_level(entries, entry_count, "ERROR");
    int warning_count = count_log_level(entries, entry_count, "WARNING");
    int info_count = count_log_level(entries, entry_count, "INFO");
    int debug_count = count_log_level(entries, entry_count, "DEBUG");
    
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    printf("DEBUG entries: %d\n", debug_count);
    
    printf("\nLevel distribution:\n");
    if (entry_count > 0) {
        printf("ERROR: %.1f%%\n", (error_count * 100.0) / entry_count);
        printf("WARNING: %.1f%%\n", (warning_count * 100.0) / entry_count);
        printf("INFO: %.1f%%\n", (info_count * 100.0) / entry_count);
        printf("DEBUG: %.1f%%\n", (debug_count * 100.0) / entry_count);
    }
    
    analyze_log_patterns(entries, entry_count);
    
    clock_t start = clock();
    
    int total_chars = 0;
    for (int i = 0; i < entry_count; i++) {
        total_chars += strlen(entries[i].timestamp);
        total_chars += strlen(entries[i].level);
        total_chars += strlen(entries[i].message);
    }
    
    clock_t end = clock();
    double cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    printf("\nPerformance metrics:\n");
    printf("Total characters processed: %d\n", total_chars);
    printf("CPU time for analysis: %.6f seconds\n", cpu_time);
    
    return 0;
}