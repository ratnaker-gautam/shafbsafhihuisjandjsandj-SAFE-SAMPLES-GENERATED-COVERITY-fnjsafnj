//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
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

int read_log_entries(struct LogEntry* entries, int max_entries) {
    char buffer[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(buffer, sizeof(buffer), stdin) != NULL && count < max_entries) {
        if (strlen(buffer) >= sizeof(buffer) - 1) {
            continue;
        }
        
        char ts[32], level[16], msg[256];
        int parsed = sscanf(buffer, "%31s %15s %255[^\n]", ts, level, msg);
        
        if (parsed == 3 && is_valid_timestamp(ts)) {
            strncpy(entries[count].timestamp, ts, sizeof(entries[count].timestamp) - 1);
            entries[count].timestamp[sizeof(entries[count].timestamp) - 1] = '\0';
            
            strncpy(entries[count].level, level, sizeof(entries[count].level) - 1);
            entries[count].level[sizeof(entries[count].level) - 1] = '\0';
            
            strncpy(entries[count].message, msg, sizeof(entries[count].message) - 1);
            entries[count].message[sizeof(entries[count].message) - 1] = '\0';
            
            entries[count].severity = parse_severity(level);
            
            if (entries[count].severity > 0) {
                count++;
            }
        }
    }
    
    return count;
}

void analyze_logs(struct LogEntry* entries, int count) {
    int severity_count[6] = {0};
    int total_severity = 0;
    
    for (int i = 0; i < count; i++) {
        if (entries[i].severity >= 1 && entries[i].severity <= 5) {
            severity_count[entries[i].severity]++;
            total_severity += entries[i].severity;
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    printf("DEBUG: %d\n", severity_count[1]);
    printf("INFO: %d\n", severity_count[2]);
    printf("WARNING: %d\n", severity_count[3]);
    printf("ERROR: %d\n", severity_count[4]);
    printf("CRITICAL: %d\n", severity_count[5]);
    
    if (count > 0) {
        double avg_severity = (double)total_severity / count;
        printf("Average severity: %.2f\n", avg_severity);
    }
}

int compare_by_severity(const void* a, const void* b) {
    const struct LogEntry* entry_a = (const struct LogEntry*)a;
    const struct LogEntry* entry_b = (const struct LogEntry*)b;
    return entry_b->severity - entry_a->severity;
}

void display_top_entries(struct LogEntry* entries, int count, int top_n) {
    if (top_n <= 0 || count <= 0) return;
    
    if (top_n > count) top_n = count;
    
    struct LogEntry* sorted_entries = malloc(sizeof(struct LogEntry) * count);
    if (sorted_entries == NULL) return;
    
    memcpy(sorted_entries, entries, sizeof(struct LogEntry) * count);
    qsort(sorted_entries, count, sizeof(struct LogEntry), compare_by_severity);
    
    printf("\nTop %d most severe entries:\n", top_n);
    for (int i = 0; i < top_n; i++) {
        printf("%s %s %s\n", sorted_entries[i].timestamp, 
               sorted_entries[i].level, sorted_entries[i].message);
    }
    
    free(sorted_entries);
}

int main(void) {
    struct LogEntry entries_stack[MAX_ENTRIES];
    int entry_count;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE):\n");
    printf("Press Ctrl+D (