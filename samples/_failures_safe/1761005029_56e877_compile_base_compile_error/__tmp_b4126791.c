//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
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
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
        if (strlen(line) >= MAX_LINE_LEN - 1) continue;
        
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed != 3) continue;
        
        if (!validate_timestamp(timestamp)) continue;
        
        int severity = parse_log_level(level);
        if (severity == -1) continue;
        
        strncpy(entries[count].timestamp, timestamp, sizeof(entries[count].timestamp) - 1);
        strncpy(entries[count].level, level, sizeof(entries[count].level) - 1);
        strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
        entries[count].severity = severity;
        count++;
    }
    
    return count;
}

int compare_entries(const void* a, const void* b) {
    const LogEntry* entry_a = (const LogEntry*)a;
    const LogEntry* entry_b = (const LogEntry*)b;
    
    int time_cmp = strcmp(entry_a->timestamp, entry_b->timestamp);
    if (time_cmp != 0) return time_cmp;
    
    return entry_b->severity - entry_a->severity;
}

void analyze_logs(LogEntry* entries, int count) {
    if (count == 0) {
        printf("No valid log entries found.\n");
        return;
    }
    
    int severity_count[5] = {0};
    char* severity_names[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    
    for (int i = 0; i < count; i++) {
        if (entries[i].severity >= 0 && entries[i].severity < 5) {
            severity_count[entries[i].severity]++;
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    printf("Severity distribution:\n");
    for (int i = 0; i < 5; i++) {
        printf("  %s: %d (%.1f%%)\n", severity_names[i], severity_count[i], 
               count > 0 ? (severity_count[i] * 100.0 / count) : 0.0);
    }
    
    qsort(entries, count, sizeof(LogEntry), compare_entries);
    
    printf("\nRecent critical entries (last 5):\n");
    int critical_shown = 0;
    for (int i = count - 1; i >= 0 && critical_shown < 5; i--) {
        if (entries[i].severity >= 3) {
            printf("  [%s] %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            critical_shown++;
        }
    }
    
    if (critical_shown == 0) {
        printf("  No critical entries found.\n");
    }
}

int main(void) {
    LogEntry* entries = malloc(MAX_ENTRIES * sizeof(LogEntry));
    if (entries == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Press Ctrl+D (Unix) or Ctrl+Z (Windows) to finish input.\n");
    
    int count = read_log_entries(entries, MAX_