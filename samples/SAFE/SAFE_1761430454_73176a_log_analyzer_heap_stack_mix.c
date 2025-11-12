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
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\\n");
    printf("Enter 'END' on a separate line to finish.\\n");
    
    while (count < max_entries && fgets(line, sizeof(line), stdin)) {
        if (strncmp(line, "END", 3) == 0) break;
        
        char ts[32], level[16], msg[256];
        int parsed = sscanf(line, "%31s %15s %255[^\\n]", ts, level, msg);
        
        if (parsed != 3) {
            printf("Invalid format. Skipping.\\n");
            continue;
        }
        
        if (!is_valid_timestamp(ts)) {
            printf("Invalid timestamp format. Skipping.\\n");
            continue;
        }
        
        int sev = parse_severity(level);
        if (sev == -1) {
            printf("Unknown log level. Skipping.\\n");
            continue;
        }
        
        strncpy(entries[count].timestamp, ts, sizeof(entries[count].timestamp) - 1);
        strncpy(entries[count].level, level, sizeof(entries[count].level) - 1);
        strncpy(entries[count].message, msg, sizeof(entries[count].message) - 1);
        entries[count].severity = sev;
        entries[count].timestamp[31] = '\\0';
        entries[count].level[15] = '\\0';
        entries[count].message[255] = '\\0';
        
        count++;
    }
    
    return count;
}

void analyze_logs(struct LogEntry* entries, int count) {
    if (count == 0) {
        printf("No log entries to analyze.\\n");
        return;
    }
    
    int severity_count[4] = {0};
    int total_severity = 0;
    
    for (int i = 0; i < count; i++) {
        if (entries[i].severity >= 0 && entries[i].severity <= 3) {
            severity_count[entries[i].severity]++;
            total_severity += entries[i].severity;
        }
    }
    
    printf("\\nLog Analysis Summary:\\n");
    printf("Total entries: %d\\n", count);
    printf("DEBUG: %d\\n", severity_count[0]);
    printf("INFO: %d\\n", severity_count[1]);
    printf("WARN: %d\\n", severity_count[2]);
    printf("ERROR: %d\\n", severity_count[3]);
    
    if (count > 0) {
        double avg_severity = (double)total_severity / count;
        printf("Average severity: %.2f\\n", avg_severity);
    }
    
    printf("\\nRecent high severity entries (ERROR only):\\n");
    int printed = 0;
    for (int i = count - 1; i >= 0 && printed < 3; i--) {
        if (entries[i].severity == 3) {
            printf("- %s: %s\\n", entries[i].timestamp, entries[i].message);
            printed++;
        }
    }
    if (printed == 0) {
        printf("No ERROR entries found.\\n");
    }
}

int main(void) {
    struct LogEntry* heap_entries = malloc(MAX_ENTRIES * sizeof(struct LogEntry));
    if (!heap_entries) {
        fprintf(stderr, "Memory allocation failed\\n");
        return 1;
    }
    
    int entry_count = read_log_entries(heap_entries, MAX_ENTRIES);
    analyze_logs(heap_entries, entry_count);
    
    free(heap_entries);
    return 0;
}