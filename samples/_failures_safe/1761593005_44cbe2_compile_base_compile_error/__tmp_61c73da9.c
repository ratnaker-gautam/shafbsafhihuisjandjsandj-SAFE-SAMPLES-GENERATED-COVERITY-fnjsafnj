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

int parse_log_level(const char* level) {
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

int read_log_entries(struct LogEntry* entries, int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'END' on a separate line to finish.\n");
    
    while (count < max_entries && fgets(line, sizeof(line), stdin)) {
        if (strncmp(line, "END", 3) == 0) break;
        
        char ts[32], level[16], msg[256];
        int parsed = sscanf(line, "%31s %15s %255[^\n]", ts, level, msg);
        
        if (parsed != 3) {
            printf("Invalid format. Skipping.\n");
            continue;
        }
        
        if (!is_valid_timestamp(ts)) {
            printf("Invalid timestamp format. Skipping.\n");
            continue;
        }
        
        int sev = parse_log_level(level);
        if (sev == 0) {
            printf("Unknown log level. Skipping.\n");
            continue;
        }
        
        strncpy(entries[count].timestamp, ts, sizeof(entries[count].timestamp) - 1);
        strncpy(entries[count].level, level, sizeof(entries[count].level) - 1);
        strncpy(entries[count].message, msg, sizeof(entries[count].message) - 1);
        entries[count].severity = sev;
        entries[count].timestamp[31] = '\0';
        entries[count].level[15] = '\0';
        entries[count].message[255] = '\0';
        
        count++;
    }
    
    return count;
}

void analyze_logs(struct LogEntry* entries, int count) {
    if (count == 0) {
        printf("No log entries to analyze.\n");
        return;
    }
    
    int severity_count[6] = {0};
    char* level_names[] = {"UNKNOWN", "DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    
    for (int i = 0; i < count; i++) {
        if (entries[i].severity >= 1 && entries[i].severity <= 5) {
            severity_count[entries[i].severity]++;
        }
    }
    
    printf("\nLog Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    printf("\nSeverity breakdown:\n");
    for (int i = 1; i <= 5; i++) {
        printf("%-8s: %d (%.1f%%)\n", level_names[i], severity_count[i], 
               (float)severity_count[i] / count * 100);
    }
    
    int max_sev = 0;
    for (int i = 0; i < count; i++) {
        if (entries[i].severity > max_sev) {
            max_sev = entries[i].severity;
        }
    }
    
    if (max_sev >= 4) {
        printf("\nHigh severity entries found:\n");
        for (int i = 0; i < count; i++) {
            if (entries[i].severity >= 4) {
                printf("- [%s] %s: %s\n", entries[i].timestamp, 
                       entries[i].level, entries[i].message);
            }
        }
    }
}

int main(void) {
    struct LogEntry* heap_entries = malloc(MAX_ENTRIES * sizeof(struct LogEntry));
    if (!heap_entries) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    struct LogEntry stack_entries[100];
    int heap_count = 0;
    int stack_count = 0;
    
    printf("Reading heap entries:\n");
    heap_count = read_log_entries