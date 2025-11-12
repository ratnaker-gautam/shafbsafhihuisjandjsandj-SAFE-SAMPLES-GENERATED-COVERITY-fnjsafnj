//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
    int severity;
};

int parse_severity(const char* level) {
    if (strcmp(level, "DEBUG") == 0) return 1;
    if (strcmp(level, "INFO") == 0) return 2;
    if (strcmp(level, "WARNING") == 0) return 3;
    if (strcmp(level, "ERROR") == 0) return 4;
    if (strcmp(level, "CRITICAL") == 0) return 5;
    return 0;
}

int is_valid_timestamp(const char* ts) {
    if (strlen(ts) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (ts[i] != '-') return 0;
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
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) && count < max_entries) {
        if (strlen(line) >= MAX_LINE_LENGTH - 1) continue;
        
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed != 3) continue;
        
        if (!is_valid_timestamp(timestamp)) continue;
        
        int severity = parse_severity(level);
        if (severity == 0) continue;
        
        strncpy(entries[count].timestamp, timestamp, sizeof(entries[count].timestamp) - 1);
        strncpy(entries[count].level, level, sizeof(entries[count].level) - 1);
        strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
        entries[count].severity = severity;
        count++;
    }
    
    fclose(file);
    return count;
}

void analyze_logs(struct LogEntry* entries, int count) {
    if (count <= 0) return;
    
    int severity_count[6] = {0};
    char* max_severity_levels[] = {"", "DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    
    for (int i = 0; i < count; i++) {
        if (entries[i].severity >= 1 && entries[i].severity <= 5) {
            severity_count[entries[i].severity]++;
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    printf("Severity breakdown:\n");
    for (int i = 1; i <= 5; i++) {
        printf("  %s: %d\n", max_severity_levels[i], severity_count[i]);
    }
    
    int max_severity = 0;
    for (int i = 0; i < count; i++) {
        if (entries[i].severity > max_severity) {
            max_severity = entries[i].severity;
        }
    }
    
    if (max_severity > 0) {
        printf("Highest severity found: %s\n", max_severity_levels[max_severity]);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    struct LogEntry* entries = malloc(MAX_ENTRIES * sizeof(struct LogEntry));
    if (!entries) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    int count = read_log_entries(argv[1], entries, MAX_ENTRIES);
    if (count < 0) {
        fprintf(stderr, "Error reading log file\n");
        free(entries);
        return 1;
    }
    
    if (count == 0) {
        printf("No valid log entries found\n");
        free(entries);
        return 0;
    }
    
    analyze_logs(entries, count);
    
    struct LogEntry high_severity_stack[100];
    int high_count = 0;
    
    for (int i = 0; i < count && high_count < 100; i++) {
        if (entries[i].severity >= 4) {
            strncpy(high_severity_stack[high_count].timestamp, entries[i].timestamp, 31);
            strncpy(high_severity_stack[high_count].level, entries[i].level,