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
    char line[MAX_LINE_LEN];
    int count = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Enter 'END' on a separate line to finish.\n");
    
    while (count < max_entries && fgets(line, sizeof(line), stdin) != NULL) {
        if (strncmp(line, "END", 3) == 0 && (line[3] == '\n' || line[3] == '\0')) {
            break;
        }
        
        line[strcspn(line, "\n")] = 0;
        
        if (strlen(line) == 0) continue;
        
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        
        if (parsed < 3) {
            printf("Invalid format. Skipping: %s\n", line);
            continue;
        }
        
        if (!is_valid_timestamp(timestamp)) {
            printf("Invalid timestamp. Skipping: %s\n", timestamp);
            continue;
        }
        
        int severity = parse_severity(level);
        if (severity == -1) {
            printf("Unknown log level. Skipping: %s\n", level);
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
    if (count == 0) {
        printf("No log entries to analyze.\n");
        return;
    }
    
    int severity_count[4] = {0};
    char* severity_names[] = {"DEBUG", "INFO", "WARN", "ERROR"};
    
    for (int i = 0; i < count; i++) {
        if (entries[i].severity >= 0 && entries[i].severity <= 3) {
            severity_count[entries[i].severity]++;
        }
    }
    
    printf("\nLog Analysis Summary:\n");
    printf("====================\n");
    printf("Total entries: %d\n", count);
    
    for (int i = 3; i >= 0; i--) {
        printf("%s: %d (%.1f%%)\n", severity_names[i], severity_count[i],
               count > 0 ? (severity_count[i] * 100.0) / count : 0.0);
    }
    
    int error_count = severity_count[3];
    if (error_count > 0) {
        printf("\nError entries:\n");
        for (int i = 0; i < count; i++) {
            if (entries[i].severity == 3) {
                printf("- %s: %s\n", entries[i].timestamp, entries[i].message);
            }
        }
    }
}

int main(void) {
    struct LogEntry* heap_entries = malloc(MAX_ENTRIES * sizeof(struct LogEntry));
    if (heap_entries == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    int entry_count = read_log_entries(heap_entries, MAX_ENTRIES);
    
    if (entry_count > 0) {
        struct LogEntry stack_entries[MAX_ENTRIES];
        if (entry_count <= MAX_ENTRIES) {
            memcpy(stack_entries, heap_entries, entry_count * sizeof(struct LogEntry));