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
    if (!line || !entry) return 0;
    
    char temp[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strcpy(temp, line);
    
    char* parts[3];
    parts[0] = strtok(temp, "|");
    parts[1] = strtok(NULL, "|");
    parts[2] = strtok(NULL, "\n");
    
    if (!parts[0] || !parts[1] || !parts[2]) return 0;
    
    if (strlen(parts[0]) >= 32) return 0;
    if (strlen(parts[1]) >= 16) return 0;
    if (strlen(parts[2]) >= 256) return 0;
    
    strcpy(entry->timestamp, parts[0]);
    strcpy(entry->level, parts[1]);
    strcpy(entry->message, parts[2]);
    
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
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: timestamp|level|message), empty line to finish:\n");
    
    while (log_count < MAX_ENTRIES) {
        if (!fgets(line, MAX_LINE_LEN, stdin)) break;
        
        line[strcspn(line, "\n")] = 0;
        
        if (strlen(line) == 0) break;
        
        if (!parse_log_line(line, &logs[log_count])) {
            printf("Invalid log format. Skipping.\n");
            continue;
        }
        
        log_count++;
    }
    
    if (log_count == 0) {
        printf("No valid log entries provided.\n");
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
    
    analyze_log_patterns(logs, log_count);
    
    printf("\nProcessing complete.\n");
    return 0;
}