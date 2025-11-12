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
    int severity;
} LogEntry;

int parse_log_level(const char* level) {
    if (strcmp(level, "DEBUG") == 0) return 0;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "CRITICAL") == 0) return 4;
    return -1;
}

int validate_timestamp(const char* timestamp) {
    if (strlen(timestamp) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 10) {
            if (timestamp[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    return 1;
}

int parse_log_line(const char* line, LogEntry* entry) {
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    if (!timestamp || !validate_timestamp(timestamp)) return 0;
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    
    char* level = strtok(NULL, " ");
    if (!level) return 0;
    int severity = parse_log_level(level);
    if (severity == -1) return 0;
    entry->severity = severity;
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    
    char* message = strtok(NULL, "\n");
    if (!message) return 0;
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    return 1;
}

void analyze_logs(LogEntry* logs, int count) {
    int severity_count[5] = {0};
    char* severity_names[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    
    for (int i = 0; i < count; i++) {
        if (logs[i].severity >= 0 && logs[i].severity < 5) {
            severity_count[logs[i].severity]++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("====================\n");
    for (int i = 0; i < 5; i++) {
        printf("%-8s: %d entries\n", severity_names[i], severity_count[i]);
    }
    
    int total = 0;
    for (int i = 0; i < 5; i++) {
        total += severity_count[i];
    }
    printf("Total   : %d entries\n", total);
    
    if (total > 0) {
        printf("\nSeverity Distribution:\n");
        for (int i = 0; i < 5; i++) {
            double percentage = (double)severity_count[i] / total * 100.0;
            printf("%-8s: %.1f%%\n", severity_names[i], percentage);
        }
    }
}

int main(void) {
    LogEntry logs[MAX_ENTRIES];
    int log_count = 0;
    
    printf("Log Analyzer - Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    char line[MAX_LINE_LENGTH];
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
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            logs[log_count] = entry;
            log_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (log_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    analyze_logs(logs, log_count);
    
    return 0;
}