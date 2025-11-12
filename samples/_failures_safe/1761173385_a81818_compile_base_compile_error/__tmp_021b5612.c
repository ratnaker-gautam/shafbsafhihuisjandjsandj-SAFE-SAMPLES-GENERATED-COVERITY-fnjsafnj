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

int read_log_file(const char* filename, struct LogEntry* entries, int max_entries) {
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
        if (severity == -1) continue;
        
        strncpy(entries[count].timestamp, timestamp, sizeof(entries[count].timestamp) - 1);
        strncpy(entries[count].level, level, sizeof(entries[count].level) - 1);
        strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
        entries[count].timestamp[sizeof(entries[count].timestamp) - 1] = '\0';
        entries[count].level[sizeof(entries[count].level) - 1] = '\0';
        entries[count].message[sizeof(entries[count].message) - 1] = '\0';
        entries[count].severity = severity;
        count++;
    }
    
    fclose(file);
    return count;
}

void analyze_logs(struct LogEntry* entries, int count) {
    if (count <= 0) return;
    
    int severity_count[5] = {0};
    char earliest[32] = {0};
    char latest[32] = {0};
    
    strncpy(earliest, entries[0].timestamp, sizeof(earliest) - 1);
    strncpy(latest, entries[0].timestamp, sizeof(latest) - 1);
    earliest[sizeof(earliest) - 1] = '\0';
    latest[sizeof(latest) - 1] = '\0';
    
    for (int i = 0; i < count; i++) {
        if (entries[i].severity >= 0 && entries[i].severity < 5) {
            severity_count[entries[i].severity]++;
        }
        
        if (strcmp(entries[i].timestamp, earliest) < 0) {
            strncpy(earliest, entries[i].timestamp, sizeof(earliest) - 1);
            earliest[sizeof(earliest) - 1] = '\0';
        }
        if (strcmp(entries[i].timestamp, latest) > 0) {
            strncpy(latest, entries[i].timestamp, sizeof(latest) - 1);
            latest[sizeof(latest) - 1] = '\0';
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("Time range: %s to %s\n", earliest, latest);
    printf("Severity breakdown:\n");
    printf("  DEBUG: %d\n", severity_count[0]);
    printf("  INFO: %d\n", severity_count[1]);
    printf("  WARNING: %d\n", severity_count[2]);
    printf("  ERROR: %d\n", severity_count[3]);
    printf("  CRITICAL: %d\n", severity_count[4]);
}

int main() {
    struct LogEntry* entries = malloc(MAX_ENTRIES * sizeof(struct LogEntry));
    if (!entries) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    printf("Enter log filename: ");
    char filename[256