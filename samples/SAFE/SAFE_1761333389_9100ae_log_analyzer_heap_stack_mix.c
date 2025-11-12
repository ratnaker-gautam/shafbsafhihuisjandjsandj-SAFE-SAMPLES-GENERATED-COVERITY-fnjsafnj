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

int read_log_entries(struct LogEntry* entries, int max_entries) {
    char buffer[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(buffer, sizeof(buffer), stdin) != NULL && count < max_entries) {
        if (buffer[0] == '\n' || buffer[0] == '\0') continue;
        
        char ts[32], level[16], msg[256];
        int parsed = sscanf(buffer, "%31s %15s %255[^\n]", ts, level, msg);
        
        if (parsed == 3 && is_valid_timestamp(ts)) {
            int sev = parse_severity(level);
            if (sev >= 0) {
                strncpy(entries[count].timestamp, ts, sizeof(entries[count].timestamp) - 1);
                strncpy(entries[count].level, level, sizeof(entries[count].level) - 1);
                strncpy(entries[count].message, msg, sizeof(entries[count].message) - 1);
                entries[count].severity = sev;
                entries[count].timestamp[31] = '\0';
                entries[count].level[15] = '\0';
                entries[count].message[255] = '\0';
                count++;
            }
        }
    }
    
    return count;
}

void analyze_logs(struct LogEntry* entries, int count) {
    int severity_count[5] = {0};
    char* severity_names[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    
    for (int i = 0; i < count; i++) {
        if (entries[i].severity >= 0 && entries[i].severity < 5) {
            severity_count[entries[i].severity]++;
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    for (int i = 0; i < 5; i++) {
        printf("%s: %d\n", severity_names[i], severity_count[i]);
    }
    
    if (count > 0) {
        printf("\nRecent entries:\n");
        int display_count = count > 5 ? 5 : count;
        for (int i = count - display_count; i < count; i++) {
            printf("%s [%s] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }
}

int main(void) {
    struct LogEntry* heap_entries = malloc(MAX_ENTRIES * sizeof(struct LogEntry));
    if (heap_entries == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Press Ctrl+D (Linux) or Ctrl+Z (Windows) to finish input\n");
    
    int entry_count = read_log_entries(heap_entries, MAX_ENTRIES);
    
    if (entry_count > 0) {
        analyze_logs(heap_entries, entry_count);
    } else {
        printf("No valid log entries found\n");
    }
    
    free(heap_entries);
    return 0;
}