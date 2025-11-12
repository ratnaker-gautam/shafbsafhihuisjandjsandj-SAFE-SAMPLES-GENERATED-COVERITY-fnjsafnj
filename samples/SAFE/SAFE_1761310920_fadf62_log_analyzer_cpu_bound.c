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
    parts[2] = strtok(NULL, "|");
    
    if (!parts[0] || !parts[1] || !parts[2]) return 0;
    
    if (strlen(parts[0]) >= sizeof(entry->timestamp)) return 0;
    if (strlen(parts[1]) >= sizeof(entry->level)) return 0;
    if (strlen(parts[2]) >= sizeof(entry->message)) return 0;
    
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

int main() {
    LogEntry logs[MAX_ENTRIES];
    int log_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Log Analyzer - Enter log entries (format: timestamp|level|message)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (log_count < MAX_ENTRIES) {
        if (!fgets(line, sizeof(line), stdin)) {
            break;
        }
        
        line[strcspn(line, "\n")] = 0;
        
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
    
    printf("\nAnalysis Results:\n");
    printf("Total log entries: %d\n", log_count);
    
    int error_count = count_log_level(logs, log_count, "ERROR");
    int warning_count = count_log_level(logs, log_count, "WARNING");
    int info_count = count_log_level(logs, log_count, "INFO");
    
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("\nLevel distribution:\n");
    if (log_count > 0) {
        printf("ERROR: %.1f%%\n", (error_count * 100.0) / log_count);
        printf("WARNING: %.1f%%\n", (warning_count * 100.0) / log_count);
        printf("INFO: %.1f%%\n", (info_count * 100.0) / log_count);
    }
    
    analyze_log_patterns(logs, log_count);
    
    return 0;
}