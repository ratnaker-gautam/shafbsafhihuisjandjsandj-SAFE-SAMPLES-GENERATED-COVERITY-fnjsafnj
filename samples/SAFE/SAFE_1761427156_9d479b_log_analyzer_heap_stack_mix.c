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

int read_log_entries(struct LogEntry* entries, int max_entries) {
    char buffer[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(buffer, sizeof(buffer), stdin) != NULL && count < max_entries) {
        if (buffer[0] == '\n' || buffer[0] == '\0') continue;
        
        char ts[32] = {0};
        char level[16] = {0};
        char msg[256] = {0};
        
        int parsed = sscanf(buffer, "%31s %15s %255[^\n]", ts, level, msg);
        if (parsed != 3) continue;
        
        if (!is_valid_timestamp(ts)) continue;
        
        int sev = parse_severity(level);
        if (sev == -1) continue;
        
        strncpy(entries[count].timestamp, ts, sizeof(entries[count].timestamp) - 1);
        strncpy(entries[count].level, level, sizeof(entries[count].level) - 1);
        strncpy(entries[count].message, msg, sizeof(entries[count].message) - 1);
        entries[count].severity = sev;
        count++;
    }
    
    return count;
}

int compare_entries(const void* a, const void* b) {
    const struct LogEntry* ea = (const struct LogEntry*)a;
    const struct LogEntry* eb = (const struct LogEntry*)b;
    
    int time_cmp = strcmp(ea->timestamp, eb->timestamp);
    if (time_cmp != 0) return time_cmp;
    return eb->severity - ea->severity;
}

void print_statistics(struct LogEntry* entries, int count) {
    int severity_count[5] = {0};
    const char* level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    
    for (int i = 0; i < count; i++) {
        if (entries[i].severity >= 0 && entries[i].severity < 5) {
            severity_count[entries[i].severity]++;
        }
    }
    
    printf("\nLog Statistics:\n");
    printf("Total entries: %d\n", count);
    for (int i = 0; i < 5; i++) {
        printf("%s: %d\n", level_names[i], severity_count[i]);
    }
}

int main(void) {
    struct LogEntry* entries = malloc(MAX_ENTRIES * sizeof(struct LogEntry));
    if (entries == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Press Ctrl+D to finish input\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    if (count == 0) {
        printf("No valid log entries found\n");
        free(entries);
        return 0;
    }
    
    qsort(entries, count, sizeof(struct LogEntry), compare_entries);
    
    printf("\nSorted Log Entries:\n");
    for (int i = 0; i < count; i++) {
        printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
    
    print_statistics(entries, count);
    
    free(entries);
    return 0;
}