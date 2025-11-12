//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000
#define MAX_LEVEL_LENGTH 10

struct LogEntry {
    char timestamp[20];
    char level[MAX_LEVEL_LENGTH];
    char message[256];
    int severity;
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    int count;
    int error_count;
    int warning_count;
    int info_count;
};

void init_analyzer(struct LogAnalyzer *analyzer) {
    analyzer->count = 0;
    analyzer->error_count = 0;
    analyzer->warning_count = 0;
    analyzer->info_count = 0;
}

int validate_timestamp(const char *timestamp) {
    if (strlen(timestamp) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    return 1;
}

int validate_level(const char *level) {
    if (strcmp(level, "ERROR") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 1;
    if (strcmp(level, "INFO") == 0) return 1;
    return 0;
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (line == NULL || strlen(line) >= MAX_LINE_LENGTH) return 0;
    
    char timestamp[20], level[MAX_LEVEL_LENGTH], message[256];
    
    int result = sscanf(line, "%19s %9s %255[^\n]", timestamp, level, message);
    if (result != 3) return 0;
    
    if (!validate_timestamp(timestamp)) return 0;
    if (!validate_level(level)) return 0;
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    if (strcmp(level, "ERROR") == 0) entry->severity = 3;
    else if (strcmp(level, "WARNING") == 0) entry->severity = 2;
    else entry->severity = 1;
    
    return 1;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *line) {
    if (analyzer->count >= MAX_ENTRIES) return 0;
    
    struct LogEntry entry;
    if (!parse_log_line(line, &entry)) return 0;
    
    analyzer->entries[analyzer->count] = entry;
    analyzer->count++;
    
    if (entry.severity == 3) analyzer->error_count++;
    else if (entry.severity == 2) analyzer->warning_count++;
    else analyzer->info_count++;
    
    return 1;
}

void generate_sample_logs(struct LogAnalyzer *analyzer) {
    const char *sample_logs[] = {
        "2024-01-15 10:30:25 INFO System started successfully",
        "2024-01-15 10:35:42 WARNING High memory usage detected",
        "2024-01-15 10:40:18 ERROR Database connection failed",
        "2024-01-15 10:45:33 INFO Backup completed",
        "2024-01-15 10:50:07 WARNING Disk space running low",
        "2024-01-15 10:55:51 ERROR Service timeout occurred",
        "2024-01-15 11:00:12 INFO User login successful"
    };
    
    int sample_count = sizeof(sample_logs) / sizeof(sample_logs[0]);
    for (int i = 0; i < sample_count; i++) {
        add_log_entry(analyzer, sample_logs[i]);
    }
}

void display_summary(const struct LogAnalyzer *analyzer) {
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", analyzer->count);
    printf("ERROR entries: %d\n", analyzer->error_count);
    printf("WARNING entries: %d\n", analyzer->warning_count);
    printf("INFO entries: %d\n", analyzer->info_count);
    printf("\n");
}

void display_entries_by_level(const struct LogAnalyzer *analyzer, const char *level) {
    printf("Entries with level %s:\n", level);
    for (int i = 0; i < analyzer->count; i++) {
        if (strcmp(analyzer->entries[i].level, level) == 0) {
            printf("%s %s %s\n", analyzer->entries[i].timestamp, analyzer->