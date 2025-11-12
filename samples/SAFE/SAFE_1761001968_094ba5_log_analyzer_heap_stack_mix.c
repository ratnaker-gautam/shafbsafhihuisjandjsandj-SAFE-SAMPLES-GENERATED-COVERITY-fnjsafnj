//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
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
    int count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\\n");
    printf("Enter empty line to finish.\\n");
    
    while (count < max_entries && fgets(line, sizeof(line), stdin)) {
        if (line[0] == '\\n' || line[0] == '\\0') break;
        
        line[strcspn(line, "\\n")] = 0;
        
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        int parsed = sscanf(line, "%31s %15s %255[^\\n]", timestamp, level, message);
        if (parsed != 3) {
            printf("Invalid format. Skipping.\\n");
            continue;
        }
        
        if (!is_valid_timestamp(timestamp)) {
            printf("Invalid timestamp format. Skipping.\\n");
            continue;
        }
        
        int severity = parse_severity(level);
        if (severity == -1) {
            printf("Unknown log level. Skipping.\\n");
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

int compare_entries(const void* a, const void* b) {
    const struct LogEntry* entry_a = (const struct LogEntry*)a;
    const struct LogEntry* entry_b = (const struct LogEntry*)b;
    
    int time_cmp = strcmp(entry_a->timestamp, entry_b->timestamp);
    if (time_cmp != 0) return time_cmp;
    
    return entry_b->severity - entry_a->severity;
}

void analyze_logs(struct LogEntry* entries, int count) {
    if (count == 0) {
        printf("No log entries to analyze.\\n");
        return;
    }
    
    qsort(entries, count, sizeof(struct LogEntry), compare_entries);
    
    int severity_count[4] = {0};
    for (int i = 0; i < count; i++) {
        if (entries[i].severity >= 0 && entries[i].severity <= 3) {
            severity_count[entries[i].severity]++;
        }
    }
    
    printf("\\nLog Analysis Summary:\\n");
    printf("Total entries: %d\\n", count);
    printf("DEBUG: %d\\n", severity_count[0]);
    printf("INFO: %d\\n", severity_count[1]);
    printf("WARN: %d\\n", severity_count[2]);
    printf("ERROR: %d\\n", severity_count[3]);
    
    printf("\\nSorted Log Entries:\\n");
    for (int i = 0; i < count; i++) {
        printf("%s %s %s\\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
}

int main() {
    struct LogEntry* heap_entries = malloc(MAX_ENTRIES * sizeof(struct LogEntry));
    if (!heap_entries) {
        fprintf(stderr, "Memory allocation failed\\n");
        return 1;
    }
    
    int count = read_log_entries(heap_entries, MAX_ENTRIES);
    analyze_logs(heap_entries, count);
    
    free(heap_entries);
    return 0;
}