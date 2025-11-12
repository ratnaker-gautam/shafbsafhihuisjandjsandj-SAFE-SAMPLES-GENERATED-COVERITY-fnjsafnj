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
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp[MAX_LINE_LEN];
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
    char* token = strtok(temp, " ");
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
            strstr(entries[i].message, "ERROR") != NULL) {
            error_count++;
        }
        if (strstr(entries[i].message, "warning") != NULL ||
            strstr(entries[i].message, "WARNING") != NULL) {
            warning_count++;
        }
        if (strstr(entries[i].message, "info") != NULL ||
            strstr(entries[i].message, "INFO") != NULL) {
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
    char line[MAX_LINE_LEN];
    
    printf("Log Analyzer - Enter log lines (format: timestamp level message)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, MAX_LINE_LEN, stdin) == NULL) {
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
            printf("Invalid log format: %s\n", line);
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
    int debug_count = count_log_level(entries, entry_count, "DEBUG");
    
    printf("ERROR entries: %d\n", error_count);
    printf("WARN entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    printf("DEBUG entries: %d\n", debug_count);
    
    printf("\nLevel distribution:\n");
    if (entry_count > 0) {
        printf("ERROR: %.2f%%\n", (error_count * 100.0) / entry_count);
        printf("WARN: %.2f%%\n", (warning_count * 100.0) / entry_count);
        printf("INFO: %.2f%%\n", (info_count * 100.0) / entry_count);
        printf("DEBUG: %.2f%%\n", (debug_count * 100.0) / entry_count);
    }
    
    analyze_log_patterns(entries, entry_count);
    
    return 0;
}