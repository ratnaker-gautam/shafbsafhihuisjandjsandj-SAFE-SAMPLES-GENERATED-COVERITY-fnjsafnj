//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARN") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "DEBUG") == 0) return 0;
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
    int count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'END' on a separate line to finish.\n");
    
    while (count < max_entries) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) break;
        if (strlen(line) == 0) continue;
        
        char timestamp[32], level[16], message[256];
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        
        if (parsed != 3) {
            printf("Invalid format. Use: YYYY-MM-DD HH:MM:SS LEVEL message\n");
            continue;
        }
        
        if (!validate_timestamp(timestamp)) {
            printf("Invalid timestamp format: %s\n", timestamp);
            continue;
        }
        
        int severity = parse_severity(level);
        if (severity == -1) {
            printf("Invalid log level: %s\n", level);
            continue;
        }
        
        struct LogEntry* entry = &entries[count];
        strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
        strncpy(entry->level, level, sizeof(entry->level) - 1);
        strncpy(entry->message, message, sizeof(entry->message) - 1);
        entry->severity = severity;
        
        entries[count].timestamp[sizeof(entry->timestamp) - 1] = '\0';
        entries[count].level[sizeof(entry->level) - 1] = '\0';
        entries[count].message[sizeof(entry->message) - 1] = '\0';
        
        count++;
    }
    
    return count;
}

int compare_entries(const void* a, const void* b) {
    const struct LogEntry* entry_a = (const struct LogEntry*)a;
    const struct LogEntry* entry_b = (const struct LogEntry*)b;
    
    int time_cmp = strcmp(entry_a->timestamp, entry_b->timestamp);
    if (time_cmp != 0) return time_cmp;
    
    return entry_b->severity - entry_a->severity;
}

void analyze_logs(struct LogEntry* entries, int count) {
    if (count == 0) {
        printf("No log entries to analyze.\n");
        return;
    }
    
    int* severity_counts = malloc(4 * sizeof(int));
    if (severity_counts == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }
    
    for (int i = 0; i < 4; i++) {
        severity_counts[i] = 0;
    }
    
    for (int i = 0; i < count; i++) {
        if (entries[i].severity >= 0 && entries[i].severity < 4) {
            severity_counts[entries[i].severity]++;
        }
    }
    
    printf("\nLog Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    printf("DEBUG: %d\n", severity_counts[0]);
    printf("INFO: %d\n", severity_counts[1]);
    printf("WARN: %d\n", severity_counts[2]);
    printf("ERROR: %d\n", severity_counts[3]);
    
    free(severity_counts);
    
    qsort(entries, count, sizeof(struct LogEntry), compare_entries);
    
    printf("\nSorted log entries (by timestamp, then severity):\n");
    for (int i = 0; i < count; i++) {
        printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
}

int main()