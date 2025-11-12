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

struct LogStats {
    int error_count;
    int warning_count;
    int info_count;
    char most_frequent_level[16];
    int max_severity;
};

void trim_whitespace(char *str) {
    char *end;
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0) return;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    *(end + 1) = 0;
}

int parse_log_level(const char *level) {
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARN") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    return 0;
}

void analyze_logs(struct LogEntry *entries, int count, struct LogStats *stats) {
    int level_counts[4] = {0};
    stats->error_count = 0;
    stats->warning_count = 0;
    stats->info_count = 0;
    stats->max_severity = 0;

    for (int i = 0; i < count; i++) {
        switch (entries[i].severity) {
            case 3: stats->error_count++; break;
            case 2: stats->warning_count++; break;
            case 1: stats->info_count++; break;
        }
        if (entries[i].severity >= 0 && entries[i].severity < 4) {
            level_counts[entries[i].severity]++;
        }
        if (entries[i].severity > stats->max_severity) {
            stats->max_severity = entries[i].severity;
        }
    }

    int max_count = 0;
    int max_level = 0;
    for (int i = 1; i < 4; i++) {
        if (level_counts[i] > max_count) {
            max_count = level_counts[i];
            max_level = i;
        }
    }

    switch (max_level) {
        case 3: strcpy(stats->most_frequent_level, "ERROR"); break;
        case 2: strcpy(stats->most_frequent_level, "WARN"); break;
        case 1: strcpy(stats->most_frequent_level, "INFO"); break;
        default: strcpy(stats->most_frequent_level, "UNKNOWN"); break;
    }
}

int main(void) {
    struct LogEntry *entries = malloc(MAX_ENTRIES * sizeof(struct LogEntry));
    if (entries == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    char buffer[MAX_LINE_LEN];
    int entry_count = 0;

    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a separate line to finish input\n");

    while (entry_count < MAX_ENTRIES && fgets(buffer, sizeof(buffer), stdin) != NULL) {
        trim_whitespace(buffer);
        if (strcmp(buffer, "END") == 0) {
            break;
        }

        if (strlen(buffer) == 0) {
            continue;
        }

        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};

        int parsed = sscanf(buffer, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed < 2) {
            fprintf(stderr, "Invalid log format. Skipping: %s\n", buffer);
            continue;
        }

        if (strlen(timestamp) > 0 && strlen(level) > 0) {
            strncpy(entries[entry_count].timestamp, timestamp, sizeof(entries[entry_count].timestamp) - 1);
            entries[entry_count].timestamp[sizeof(entries[entry_count].timestamp) - 1] = '\0';
            strncpy(entries[entry_count].level, level, sizeof(entries[entry_count].level) - 1);
            entries[entry_count].level[sizeof(entries[entry_count].level) - 1] = '\0';
            if (strlen(message) > 0) {
                strncpy(entries[entry_count].message, message, sizeof(entries[entry_count].message) - 1);
                entries[entry_count].message[sizeof(entries[entry_count].message) - 1] = '\0';
            } else {
                strcpy(entries[entry_count].message, "");
            }
            entries[entry_count].severity = parse_log_level(level);
            entry_count++;
        }
    }

    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        free(entries);
        return 0;
    }

    struct LogStats stats;
    analyze_logs(entries, entry_count, &stats);

    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    printf("ERROR entries: %d\n