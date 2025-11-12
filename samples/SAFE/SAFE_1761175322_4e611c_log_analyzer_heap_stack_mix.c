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

        struct LogEntry entry;
        int parsed = sscanf(buffer, "%31s %15s %255[^\n]", 
                           entry.timestamp, entry.level, entry.message);
        
        if (parsed != 3) {
            fprintf(stderr, "Invalid log format: %s\n", buffer);
            continue;
        }
        
        if (!validate_timestamp(entry.timestamp)) {
            fprintf(stderr, "Invalid timestamp: %s\n", entry.timestamp);
            continue;
        }
        
        entry.severity = parse_log_level(entry.level);
        if (entry.severity == -1) {
            fprintf(stderr, "Invalid log level: %s\n", entry.level);
            continue;
        }
        
        if (entry_count < MAX_ENTRIES) {
            entries[entry_count] = entry;
            entry_count++;
        }
    }

    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    printf("Total entries: %d\n", entry_count);
    
    int level_counts[5] = {0};
    const char* level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].severity >= 0 && entries[i].severity < 5) {
            level_counts[entries[i].severity]++;
        }
    }
    
    printf("\nBreakdown by level:\n");
    for (int i = 0; i < 5; i++) {
        printf("%-8s: %d\n", level_names[i], level_counts[i]);
    }
    
    if (entry_count > 0) {
        printf("\nMost recent entries:\n");
        int start = (entry_count > 5) ? entry_count - 5 : 0;
        for (int i = start; i < entry_count; i++) {
            printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }

    free(entries);
    return 0;
}