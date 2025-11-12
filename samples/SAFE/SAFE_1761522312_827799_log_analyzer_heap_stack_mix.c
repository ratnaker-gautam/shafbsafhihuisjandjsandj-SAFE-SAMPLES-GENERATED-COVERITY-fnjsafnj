//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARN") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "DEBUG") == 0) return 0;
    return -1;
}

int read_log_entries(struct LogEntry* entries, int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
        if (strlen(line) == 0 || line[0] == '\n') continue;
        
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed < 3) continue;
        
        int severity = parse_severity(level);
        if (severity == -1) continue;
        
        strncpy(entries[count].timestamp, timestamp, sizeof(entries[count].timestamp) - 1);
        strncpy(entries[count].level, level, sizeof(entries[count].level) - 1);
        strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
        entries[count].severity = severity;
        count++;
    }
    
    return count;
}

void analyze_logs(struct LogEntry* entries, int count) {
    if (count <= 0) return;
    
    int severity_count[4] = {0};
    int total_severity = 0;
    
    for (int i = 0; i < count; i++) {
        if (entries[i].severity >= 0 && entries[i].severity < 4) {
            severity_count[entries[i].severity]++;
            total_severity += entries[i].severity;
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    printf("DEBUG: %d\n", severity_count[0]);
    printf("INFO: %d\n", severity_count[1]);
    printf("WARN: %d\n", severity_count[2]);
    printf("ERROR: %d\n", severity_count[3]);
    
    if (count > 0) {
        double avg_severity = (double)total_severity / count;
        printf("Average severity: %.2f\n", avg_severity);
    }
}

struct LogEntry* filter_by_severity(struct LogEntry* entries, int count, int min_severity, int* result_count) {
    if (entries == NULL || count <= 0 || min_severity < 0 || min_severity > 3) {
        *result_count = 0;
        return NULL;
    }
    
    int filtered_count = 0;
    for (int i = 0; i < count; i++) {
        if (entries[i].severity >= min_severity) {
            filtered_count++;
        }
    }
    
    if (filtered_count == 0) {
        *result_count = 0;
        return NULL;
    }
    
    struct LogEntry* filtered = malloc(filtered_count * sizeof(struct LogEntry));
    if (filtered == NULL) {
        *result_count = 0;
        return NULL;
    }
    
    int idx = 0;
    for (int i = 0; i < count; i++) {
        if (entries[i].severity >= min_severity) {
            filtered[idx] = entries[i];
            idx++;
        }
    }
    
    *result_count = filtered_count;
    return filtered;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Press Ctrl+D (Linux) to finish input\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    if (count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    analyze_logs(entries, count);
    
    int filtered_count;
    struct LogEntry* high_severity = filter_by_severity(entries, count, 2, &filtered_count);
    
    if (high_severity != NULL && filtered_count > 0) {
        printf("\nHigh severity entries (WARN/ERROR): %d\n", filtered_count);
        for (int i = 0; i < filtered_count; i++) {
            printf("%s %s %s\n", high_severity[i].timestamp, high_severity[i].level, high_severity[i].message);
        }
        free(high_severity);
    }
    
    return 0;
}