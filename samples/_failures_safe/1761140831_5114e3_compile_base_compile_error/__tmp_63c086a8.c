//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

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

int read_log_entries(LogEntry* entries, int max_entries) {
    char buffer[MAX_LINE_LENGTH];
    int count = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Enter empty line to finish.\n");
    
    while (count < max_entries) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') buffer[len-1] = '\0';
        if (strlen(buffer) == 0) break;
        
        char timestamp[32], level[16], message[256];
        int parsed = sscanf(buffer, "%31s %15s %255[^\n]", timestamp, level, message);
        
        if (parsed != 3) {
            printf("Invalid format. Skipping.\n");
            continue;
        }
        
        if (!validate_timestamp(timestamp)) {
            printf("Invalid timestamp. Skipping.\n");
            continue;
        }
        
        int severity = parse_log_level(level);
        if (severity == -1) {
            printf("Invalid log level. Skipping.\n");
            continue;
        }
        
        strncpy(entries[count].timestamp, timestamp, sizeof(entries[count].timestamp)-1);
        strncpy(entries[count].level, level, sizeof(entries[count].level)-1);
        strncpy(entries[count].message, message, sizeof(entries[count].message)-1);
        entries[count].severity = severity;
        entries[count].timestamp[31] = '\0';
        entries[count].level[15] = '\0';
        entries[count].message[255] = '\0';
        
        count++;
    }
    
    return count;
}

void analyze_logs(LogEntry* entries, int count) {
    if (count == 0) {
        printf("No log entries to analyze.\n");
        return;
    }
    
    int severity_count[5] = {0};
    char* severity_names[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    
    for (int i = 0; i < count; i++) {
        if (entries[i].severity >= 0 && entries[i].severity < 5) {
            severity_count[entries[i].severity]++;
        }
    }
    
    printf("\nLog Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    for (int i = 0; i < 5; i++) {
        printf("%s: %d (%.1f%%)\n", severity_names[i], severity_count[i], 
               count > 0 ? (severity_count[i] * 100.0 / count) : 0.0);
    }
    
    int max_severity = 0;
    for (int i = 0; i < count; i++) {
        if (entries[i].severity > max_severity) {
            max_severity = entries[i].severity;
        }
    }
    
    if (max_severity >= 3) {
        printf("\nHigh severity entries found:\n");
        for (int i = 0; i < count; i++) {
            if (entries[i].severity >= 3) {
                printf("- %s %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            }
        }
    }
}

int compare_by_severity(const void* a, const void* b) {
    const LogEntry* entry_a = (const LogEntry*)a;
    const LogEntry* entry_b = (const LogEntry*)b;
    return entry_b->severity - entry_a->severity;
}

void sort_entries_by_severity(LogEntry* entries, int count) {
    if (count <=