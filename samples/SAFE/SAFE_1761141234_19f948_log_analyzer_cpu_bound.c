//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 10000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
    int severity;
} LogEntry;

int parse_log_level(const char* level) {
    if (strcmp(level, "DEBUG") == 0) return 1;
    if (strcmp(level, "INFO") == 0) return 2;
    if (strcmp(level, "WARNING") == 0) return 3;
    if (strcmp(level, "ERROR") == 0) return 4;
    if (strcmp(level, "CRITICAL") == 0) return 5;
    return 0;
}

int validate_timestamp(const char* ts) {
    if (strlen(ts) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (ts[i] != '-') return 0;
        } else if (i == 10) {
            if (ts[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (ts[i] != ':') return 0;
        } else {
            if (!isdigit(ts[i])) return 0;
        }
    }
    return 1;
}

int parse_log_line(const char* line, LogEntry* entry) {
    char temp_line[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN - 1) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    if (!timestamp || !validate_timestamp(timestamp)) return 0;
    strcpy(entry->timestamp, timestamp);
    
    char* level = strtok(NULL, " ");
    if (!level) return 0;
    strcpy(entry->level, level);
    
    entry->severity = parse_log_level(level);
    if (entry->severity == 0) return 0;
    
    char* message = strtok(NULL, "\n");
    if (!message) return 0;
    if (strlen(message) >= 256) return 0;
    strcpy(entry->message, message);
    
    return 1;
}

void analyze_logs(LogEntry* logs, int count) {
    int level_counts[6] = {0};
    int total_severity = 0;
    
    for (int i = 0; i < count; i++) {
        if (logs[i].severity >= 1 && logs[i].severity <= 5) {
            level_counts[logs[i].severity]++;
            total_severity += logs[i].severity;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries processed: %d\n", count);
    printf("DEBUG: %d\n", level_counts[1]);
    printf("INFO: %d\n", level_counts[2]);
    printf("WARNING: %d\n", level_counts[3]);
    printf("ERROR: %d\n", level_counts[4]);
    printf("CRITICAL: %d\n", level_counts[5]);
    
    if (count > 0) {
        double avg_severity = (double)total_severity / count;
        printf("Average severity: %.2f\n", avg_severity);
    }
}

int main(void) {
    LogEntry logs[MAX_ENTRIES];
    int log_count = 0;
    
    char line[MAX_LINE_LEN];
    int line_num = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && log_count < MAX_ENTRIES) {
        line_num++;
        
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') {
            line[len-1] = '\0';
        }
        
        if (strlen(line) == 0) continue;
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            logs[log_count] = entry;
            log_count++;
        } else {
            fprintf(stderr, "Invalid log format at line %d\n", line_num);
        }
    }
    
    if (ferror(stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (log_count == 0) {
        printf("No valid log entries found\n");
        return 0;
    }
    
    analyze_logs(logs, log_count);
    
    return 0;
}