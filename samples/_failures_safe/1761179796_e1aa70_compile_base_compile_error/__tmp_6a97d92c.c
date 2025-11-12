//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
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

int read_log_entries(struct LogEntry* entries, int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
        if (strlen(line) >= MAX_LINE_LEN - 1) {
            continue;
        }
        
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed != 3) {
            continue;
        }
        
        if (!validate_timestamp(timestamp)) {
            continue;
        }
        
        int severity = parse_severity(level);
        if (severity == -1) {
            continue;
        }
        
        strncpy(entries[count].timestamp, timestamp, sizeof(entries[count].timestamp) - 1);
        strncpy(entries[count].level, level, sizeof(entries[count].level) - 1);
        strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
        entries[count].severity = severity;
        count++;
    }
    
    return count;
}

void analyze_logs(struct LogEntry* entries, int count) {
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
        printf("%s: %d\n", severity_names[i], severity_count[i]);
    }
    
    if (count > 0) {
        int error_count = severity_count[3] + severity_count[4];
        double error_percentage = (double)error_count / count * 100.0;
        printf("Error rate: %.2f%%\n", error_percentage);
    }
}

int compare_by_severity(const void* a, const void* b) {
    const struct LogEntry* entry_a = (const struct LogEntry*)a;
    const struct LogEntry* entry_b = (const struct LogEntry*)b;
    return entry_b->severity - entry_a->severity;
}

void show_high_severity_logs(struct LogEntry* entries, int count, int threshold) {
    if (threshold < 0 || threshold > 4) {
        return;
    }
    
    struct LogEntry* filtered = malloc(count * sizeof(struct LogEntry));
    if (filtered == NULL) {
        return;
    }
    
    int filtered_count = 0;
    for (int i = 0; i < count; i++) {
        if (entries[i].severity >= threshold) {
            filtered[filtered_count] = entries[i];
            filtered_count++;
        }
    }
    
    if (filtered_count > 0) {
        qsort(filtered, filtered_count, sizeof(struct LogEntry), compare_by_severity);
        
        printf("\nHigh Severity Logs (>=%s):\n", 
               threshold == 0 ? "DEBUG" : 
               threshold == 1 ? "INFO" : 
               threshold == 2 ? "WARNING" : 
               threshold == 3 ? "ERROR" : "CRITICAL");
        
        for (int i = 0; i < filtered_count && i < 10; i++) {
            printf("%s [%s] %s\n", filtered[i].timestamp, filtered[i].level, filtered