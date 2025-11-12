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

int parse_log_level(const char* level) {
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARN") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "DEBUG") == 0) return 0;
    return -1;
}

int validate_timestamp(const char* ts) {
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

int main(void) {
    struct LogEntry* entries = malloc(MAX_ENTRIES * sizeof(struct LogEntry));
    if (entries == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    int entry_count = 0;
    char buffer[MAX_LINE_LEN];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Enter 'END' on a separate line to finish input.\n");

    while (entry_count < MAX_ENTRIES) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        buffer[strcspn(buffer, "\n")] = 0;
        
        if (strcmp(buffer, "END") == 0) {
            break;
        }
        
        if (strlen(buffer) == 0) {
            continue;
        }

        char timestamp[32], level[16], message[256];
        int parsed = sscanf(buffer, "%31s %15s %255[^\n]", timestamp, level, message);
        
        if (parsed != 3) {
            fprintf(stderr, "Invalid log format: %s\n", buffer);
            continue;
        }
        
        if (!validate_timestamp(timestamp)) {
            fprintf(stderr, "Invalid timestamp: %s\n", timestamp);
            continue;
        }
        
        int severity = parse_log_level(level);
        if (severity == -1) {
            fprintf(stderr, "Invalid log level: %s\n", level);
            continue;
        }

        struct LogEntry* entry = &entries[entry_count];
        strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
        entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
        strncpy(entry->level, level, sizeof(entry->level) - 1);
        entry->level[sizeof(entry->level) - 1] = '\0';
        strncpy(entry->message, message, sizeof(entry->message) - 1);
        entry->message[sizeof(entry->message) - 1] = '\0';
        entry->severity = severity;
        
        entry_count++;
    }

    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        free(entries);
        return 0;
    }

    int severity_counts[4] = {0};
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].severity >= 0 && entries[i].severity <= 3) {
            severity_counts[entries[i].severity]++;
        }
    }

    printf("\nLog Analysis Summary:\n");
    printf("Total entries: %d\n", entry_count);
    printf("DEBUG: %d\n", severity_counts[0]);
    printf("INFO: %d\n", severity_counts[1]);
    printf("WARN: %d\n", severity_counts[2]);
    printf("ERROR: %d\n", severity_counts[3]);

    printf("\nRecent entries (last 5):\n");
    int start = (entry_count > 5) ? entry_count - 5 : 0;
    for (int i = start; i < entry_count; i++) {
        printf("%s [%s] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }

    free(entries);
    return 0;
}