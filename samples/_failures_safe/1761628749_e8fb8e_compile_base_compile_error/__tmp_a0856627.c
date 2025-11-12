//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
    int severity;
} LogEntry;

typedef struct {
    LogEntry entries[MAX_ENTRIES];
    int count;
} LogBuffer;

int parse_severity(const char* level) {
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

int read_log_file(const char* filename, LogBuffer* buffer) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file %s\n", filename);
        return 0;
    }

    char line[MAX_LINE_LENGTH];
    buffer->count = 0;

    while (fgets(line, sizeof(line), file) && buffer->count < MAX_ENTRIES) {
        if (strlen(line) == 0) continue;

        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};

        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed != 3) continue;

        if (!validate_timestamp(timestamp)) continue;

        int severity = parse_severity(level);
        if (severity == -1) continue;

        LogEntry* entry = &buffer->entries[buffer->count];
        strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
        strncpy(entry->level, level, sizeof(entry->level) - 1);
        strncpy(entry->message, message, sizeof(entry->message) - 1);
        entry->severity = severity;
        buffer->count++;
    }

    fclose(file);
    return 1;
}

void analyze_logs(LogBuffer* buffer) {
    if (buffer->count == 0) {
        printf("No valid log entries found.\n");
        return;
    }

    int severity_count[5] = {0};
    char* severity_names[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};

    for (int i = 0; i < buffer->count; i++) {
        if (buffer->entries[i].severity >= 0 && buffer->entries[i].severity < 5) {
            severity_count[buffer->entries[i].severity]++;
        }
    }

    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", buffer->count);
    printf("Severity breakdown:\n");
    for (int i = 0; i < 5; i++) {
        printf("  %s: %d (%.1f%%)\n", severity_names[i], severity_count[i],
               buffer->count > 0 ? (100.0 * severity_count[i] / buffer->count) : 0.0);
    }

    int error_count = severity_count[3] + severity_count[4];
    printf("Total errors (ERROR + CRITICAL): %d\n", error_count);
    printf("Error rate: %.1f%%\n", buffer->count > 0 ? (100.0 * error_count / buffer->count) : 0.0);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <logfile>\n", argv[0]);
        return 1;
    }

    LogBuffer* buffer = malloc(sizeof(LogBuffer));
    if (!buffer) {
        printf("Error: Memory allocation failed\n");
        return 1;
    }

    if (!read_log_file(argv[1], buffer)) {
        free(buffer);
        return 1;
    }

    analyze_logs(buffer);

    LogEntry* top_errors = malloc(10 * sizeof(LogEntry));
    if (top_errors) {
        int error_count = 0;
        for (int i = 0; i < buffer->count && error_count < 10; i++) {
            if (buffer->entries[i].severity >= 3) {
                top_errors[error_count] = buffer->entries[i];
                error_count++;
            }
        }

        if (error_count > 0) {
            printf("\