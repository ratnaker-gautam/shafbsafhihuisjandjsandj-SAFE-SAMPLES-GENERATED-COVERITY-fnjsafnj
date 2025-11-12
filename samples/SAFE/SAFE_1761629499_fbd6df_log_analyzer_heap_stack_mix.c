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

int read_log_file(const char* filename, struct LogEntry** entries, int* count) {
    FILE* file = fopen(filename, "r");
    if (!file) return 0;
    
    char line[MAX_LINE_LENGTH];
    *count = 0;
    *entries = malloc(MAX_ENTRIES * sizeof(struct LogEntry));
    if (!*entries) {
        fclose(file);
        return 0;
    }
    
    while (fgets(line, sizeof(line), file) && *count < MAX_ENTRIES) {
        char ts[32], lvl[16], msg[256];
        if (sscanf(line, "%31s %15s %255[^\n]", ts, lvl, msg) == 3) {
            if (is_valid_timestamp(ts) && parse_severity(lvl) > 0) {
                struct LogEntry* entry = &(*entries)[*count];
                strncpy(entry->timestamp, ts, sizeof(entry->timestamp) - 1);
                strncpy(entry->level, lvl, sizeof(entry->level) - 1);
                strncpy(entry->message, msg, sizeof(entry->message) - 1);
                entry->severity = parse_severity(lvl);
                (*count)++;
            }
        }
    }
    
    fclose(file);
    return 1;
}

void analyze_logs(struct LogEntry* entries, int count) {
    if (count == 0) {
        printf("No valid log entries found.\n");
        return;
    }
    
    int severity_count[6] = {0};
    char latest_timestamp[32] = "";
    
    for (int i = 0; i < count; i++) {
        if (entries[i].severity >= 1 && entries[i].severity <= 5) {
            severity_count[entries[i].severity]++;
        }
        if (strcmp(entries[i].timestamp, latest_timestamp) > 0) {
            strncpy(latest_timestamp, entries[i].timestamp, sizeof(latest_timestamp) - 1);
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    printf("Latest timestamp: %s\n", latest_timestamp);
    printf("Severity breakdown:\n");
    printf("  DEBUG: %d\n", severity_count[1]);
    printf("  INFO: %d\n", severity_count[2]);
    printf("  WARNING: %d\n", severity_count[3]);
    printf("  ERROR: %d\n", severity_count[4]);
    printf("  CRITICAL: %d\n", severity_count[5]);
    
    int error_count = severity_count[4] + severity_count[5];
    if (error_count > 0) {
        printf("\nHigh severity entries found:\n");
        for (int i = 0; i < count; i++) {
            if (entries[i].severity >= 4) {
                printf("  %s %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            }
        }
    }
}

int main() {
    char filename[256];
    printf("Enter log file name: ");
    if (scanf("%255s", filename) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    struct LogEntry* entries = NULL;
    int count = 0;
    
    if (!read_log_file(filename, &entries, &count)) {
        printf("Failed to read log file or no valid entries found.\n");
        free(entries);
        return 1;
    }
    
    analyze_logs(entries, count);
    free(entries);
    
    return 0;
}