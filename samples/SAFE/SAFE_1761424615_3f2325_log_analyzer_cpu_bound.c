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
    
    char temp[MAX_LINE_LENGTH];
    strncpy(temp, line, MAX_LINE_LENGTH - 1);
    temp[MAX_LINE_LENGTH - 1] = '\0';
    
    char* parts[3];
    char* token = strtok(temp, "|");
    int part_count = 0;
    
    while (token != NULL && part_count < 3) {
        parts[part_count++] = token;
        token = strtok(NULL, "|");
    }
    
    if (part_count != 3) return 0;
    
    strncpy(entry->timestamp, parts[0], 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, parts[1], 15);
    entry->level[15] = '\0';
    strncpy(entry->message, parts[2], 255);
    entry->message[255] = '\0';
    
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
    
    printf("Log Analyzer - Enter log entries (format: timestamp|level|message)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (log_count < MAX_ENTRIES && fgets(line, MAX_LINE_LENGTH, stdin) != NULL) {
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
            printf("Invalid log format. Use: timestamp|level|message\n");
        }
    }
    
    if (log_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries processed: %d\n", log_count);
    
    int error_count = count_log_level(logs, log_count, "ERROR");
    int warning_count = count_log_level(logs, log_count, "WARNING");
    int info_count = count_log_level(logs, log_count, "INFO");
    
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("Error percentage: %.2f%%\n", (float)error_count / log_count * 100);
    printf("Warning percentage: %.2f%%\n", (float)warning_count / log_count * 100);
    
    analyze_log_patterns(logs, log_count);
    
    int consecutive_errors = 0;
    int max_consecutive = 0;
    for (int i = 0; i < log_count; i++) {
        if (strcmp(logs[i].level, "ERROR") == 0) {
            consecutive_errors++;
            if (consecutive_errors > max_consecutive) {
                max_consecutive = consecutive_errors;
            }
        } else {
            consecutive_errors = 0;
        }
    }
    
    printf("Maximum consecutive errors: %d\n", max_consecutive);
    
    return 0;
}