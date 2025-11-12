//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
    int severity;
};

int parse_severity(const char* level) {
    if (strcmp(level, "DEBUG") == 0) return 0;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "CRITICAL") == 0) return 4;
    return -1;
}

int is_valid_timestamp(const char* ts) {
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

int read_log_entries(const char* filename, struct LogEntry* entries, int max_entries) {
    FILE* file = fopen(filename, "r");
    if (!file) return -1;
    
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) && count < max_entries) {
        if (strlen(line) >= sizeof(line) - 1) continue;
        
        char timestamp[32], level[16], message[256];
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        
        if (parsed == 3 && is_valid_timestamp(timestamp)) {
            int severity = parse_severity(level);
            if (severity >= 0) {
                strncpy(entries[count].timestamp, timestamp, sizeof(entries[count].timestamp) - 1);
                strncpy(entries[count].level, level, sizeof(entries[count].level) - 1);
                strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
                entries[count].severity = severity;
                entries[count].timestamp[31] = '\0';
                entries[count].level[15] = '\0';
                entries[count].message[255] = '\0';
                count++;
            }
        }
    }
    
    fclose(file);
    return count;
}

void analyze_logs(struct LogEntry* entries, int count) {
    if (count <= 0) return;
    
    int severity_count[5] = {0};
    char* severity_names[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    
    for (int i = 0; i < count; i++) {
        if (entries[i].severity >= 0 && entries[i].severity < 5) {
            severity_count[entries[i].severity]++;
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    for (int i = 0; i < 5; i++) {
        printf("%s: %d (%.1f%%)\n", severity_names[i], severity_count[i], 
               count > 0 ? (severity_count[i] * 100.0) / count : 0.0);
    }
    
    int error_count = severity_count[3] + severity_count[4];
    if (error_count > count * 0.1) {
        printf("WARNING: High error rate detected!\n");
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    struct LogEntry* entries = malloc(MAX_ENTRIES * sizeof(struct LogEntry));
    if (!entries) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    int count = read_log_entries(argv[1], entries, MAX_ENTRIES);
    if (count < 0) {
        printf("Error reading log file: %s\n", argv[1]);
        free(entries);
        return 1;
    }
    
    if (count == 0) {
        printf("No valid log entries found\n");
        free(entries);
        return 0;
    }
    
    analyze_logs(entries, count);
    
    struct LogEntry recent_errors[10];
    int error_count = 0;
    
    for (int i = count - 1; i >= 0 && error_count < 10; i--) {
        if (entries[i].severity >= 3) {
            recent_errors[error_count] = entries[i];
            error_count++;
        }
    }
    
    if (error_count > 0) {
        printf("\nRecent errors (%d