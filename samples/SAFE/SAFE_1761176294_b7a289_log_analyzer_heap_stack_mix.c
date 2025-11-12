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
    
    while (count < max_entries && fgets(line, sizeof(line), stdin) != NULL) {
        if (strlen(line) >= sizeof(line) - 1) {
            continue;
        }
        
        char ts[32], level[16], msg[256];
        int parsed = sscanf(line, "%31s %15s %255[^\n]", ts, level, msg);
        
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
    if (count <= 0) {
        printf("No valid log entries found.\n");
        return;
    }
    
    int severity_count[4] = {0};
    char* severity_names[] = {"DEBUG", "INFO", "WARN", "ERROR"};
    
    for (int i = 0; i < count; i++) {
        if (entries[i].severity >= 0 && entries[i].severity < 4) {
            severity_count[entries[i].severity]++;
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    for (int i = 3; i >= 0; i--) {
        printf("%s: %d (%.1f%%)\n", severity_names[i], severity_count[i], 
               count > 0 ? (severity_count[i] * 100.0 / count) : 0.0);
    }
    
    int error_count = 0;
    printf("\nRecent ERROR entries:\n");
    for (int i = count - 1; i >= 0 && error_count < 5; i--) {
        if (entries[i].severity == 3) {
            printf("- %s: %s\n", entries[i].timestamp, entries[i].message);
            error_count++;
        }
    }
    if (error_count == 0) {
        printf("No ERROR entries found.\n");
    }
}

int main(void) {
    struct LogEntry* entries = malloc(MAX_ENTRIES * sizeof(struct LogEntry));
    if (entries == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Press Ctrl+D (Unix) or Ctrl+Z (Windows) to finish input.\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    if (count > 0) {
        analyze_logs(entries, count);
    } else {
        printf("No valid log entries processed.\n");
    }
    
    free(entries);
    return 0;
}