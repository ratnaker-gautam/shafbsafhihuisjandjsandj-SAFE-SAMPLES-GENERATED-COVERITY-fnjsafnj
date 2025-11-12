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

int is_valid_timestamp(const char* timestamp) {
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

int read_log_file(const char* filename, struct LogEntry** entries, int* count) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        return 0;
    }

    char line[MAX_LINE_LENGTH];
    *count = 0;
    *entries = malloc(MAX_ENTRIES * sizeof(struct LogEntry));
    if (!*entries) {
        fclose(file);
        return 0;
    }

    while (fgets(line, sizeof(line), file) && *count < MAX_ENTRIES) {
        if (strlen(line) < 2) continue;

        char timestamp[32], level[16], message[256];
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        
        if (parsed == 3 && is_valid_timestamp(timestamp)) {
            int severity = parse_severity(level);
            if (severity >= 0) {
                struct LogEntry* entry = &(*entries)[*count];
                strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
                strncpy(entry->level, level, sizeof(entry->level) - 1);
                strncpy(entry->message, message, sizeof(entry->message) - 1);
                entry->severity = severity;
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

    int severity_count[5] = {0};
    char* severity_names[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};

    for (int i = 0; i < count; i++) {
        if (entries[i].severity >= 0 && entries[i].severity < 5) {
            severity_count[entries[i].severity]++;
        }
    }

    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    for (int i = 0; i < 5; i++) {
        printf("%s: %d\n", severity_names[i], severity_count[i]);
    }

    int error_count = severity_count[3] + severity_count[4];
    if (error_count > 0) {
        printf("\nError entries found:\n");
        for (int i = 0; i < count; i++) {
            if (entries[i].severity >= 3) {
                printf("%s %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <logfile>\n", argv[0]);
        return 1;
    }

    struct LogEntry* entries = NULL;
    int count = 0;

    if (!read_log_file(argv[1], &entries, &count)) {
        return 1;
    }

    analyze_logs(entries, count);

    free(entries);
    return 0;
}