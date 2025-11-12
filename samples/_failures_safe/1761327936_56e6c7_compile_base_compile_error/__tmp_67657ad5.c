//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
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

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    int count;
    int error_count;
    int warning_count;
    int info_count;
};

void init_analyzer(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    analyzer->count = 0;
    analyzer->error_count = 0;
    analyzer->warning_count = 0;
    analyzer->info_count = 0;
}

int parse_severity(const char *level) {
    if (level == NULL) return 0;
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARN") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    return 0;
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char timestamp[32] = {0};
    char level[16] = {0};
    char message[256] = {0};
    
    int result = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    if (result != 3) return 0;
    
    if (strlen(timestamp) > 0 && strlen(level) > 0 && strlen(message) > 0) {
        strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
        strncpy(entry->level, level, sizeof(entry->level) - 1);
        strncpy(entry->message, message, sizeof(entry->message) - 1);
        entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
        entry->level[sizeof(entry->level) - 1] = '\0';
        entry->message[sizeof(entry->message) - 1] = '\0';
        entry->severity = parse_severity(level);
        return 1;
    }
    
    return 0;
}

int add_entry(struct LogAnalyzer *analyzer, const struct LogEntry *entry) {
    if (analyzer == NULL || entry == NULL) return 0;
    if (analyzer->count >= MAX_ENTRIES) return 0;
    
    analyzer->entries[analyzer->count] = *entry;
    
    if (entry->severity == 3) analyzer->error_count++;
    else if (entry->severity == 2) analyzer->warning_count++;
    else if (entry->severity == 1) analyzer->info_count++;
    
    analyzer->count++;
    return 1;
}

void generate_sample_logs(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    const char *sample_logs[] = {
        "2024-01-15T10:30:00 INFO System started successfully",
        "2024-01-15T10:35:23 WARN Disk usage above 80%",
        "2024-01-15T11:20:15 ERROR Database connection failed",
        "2024-01-15T11:25:40 INFO Backup completed",
        "2024-01-15T12:00:00 WARN Memory usage high",
        "2024-01-15T12:30:45 ERROR Service unavailable",
        "2024-01-15T13:15:30 INFO User login successful",
        "2024-01-15T14:00:00 INFO Scheduled task completed"
    };
    
    int num_logs = sizeof(sample_logs) / sizeof(sample_logs[0]);
    
    for (int i = 0; i < num_logs && analyzer->count < MAX_ENTRIES; i++) {
        struct LogEntry entry;
        if (parse_log_line(sample_logs[i], &entry)) {
            add_entry(analyzer, &entry);
        }
    }
}

void display_summary(const struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", analyzer->count);
    printf("Errors: %d\n", analyzer->error_count);
    printf("Warnings: %d\n", analyzer->warning_count);
    printf("Info messages: %d\n", analyzer->info_count);
    printf("\n");
}

void display_entries_by_level(const struct LogAnalyzer *analyzer, const char *level) {
    if (analyzer == NULL || level == NULL) return;
    
    printf("Entries with level '%s':\n", level);
    int found = 0;
    
    for (int i = 0; i < analyzer->count; i++) {
        if (strcmp(analyzer->entries[i].level, level) == 0) {
            printf("%s %s %s\n", analyzer->entries[i].timestamp, 
                   analyzer->entries[i].level, analyzer->entries[i].message);
            found = 1;
        }
    }
    
    if (!found) {
        printf("No entries found for level '%s'\n",