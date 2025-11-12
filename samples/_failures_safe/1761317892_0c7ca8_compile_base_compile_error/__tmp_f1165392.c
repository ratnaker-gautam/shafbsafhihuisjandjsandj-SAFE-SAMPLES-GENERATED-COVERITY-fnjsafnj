//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000
#define MAX_LEVEL_LENGTH 16

struct LogEntry {
    char timestamp[32];
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

int parse_severity(const char *level) {
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    return 0;
}

int validate_timestamp(const char *timestamp) {
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

int add_log_entry(struct LogAnalyzer *analyzer, const char *timestamp, const char *level, const char *message) {
    if (analyzer->count >= MAX_ENTRIES) return 0;
    if (!validate_timestamp(timestamp)) return 0;
    if (strlen(level) >= MAX_LEVEL_LENGTH) return 0;
    if (strlen(message) >= 256) return 0;
    
    struct LogEntry *entry = &analyzer->entries[analyzer->count];
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, MAX_LEVEL_LENGTH - 1);
    entry->level[MAX_LEVEL_LENGTH - 1] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    entry->severity = parse_severity(level);
    if (entry->severity == 3) analyzer->error_count++;
    else if (entry->severity == 2) analyzer->warning_count++;
    else if (entry->severity == 1) analyzer->info_count++;
    
    analyzer->count++;
    return 1;
}

void generate_sample_logs(struct LogAnalyzer *analyzer) {
    add_log_entry(analyzer, "2024-01-15 10:30:25", "INFO", "System startup completed");
    add_log_entry(analyzer, "2024-01-15 10:35:42", "WARNING", "High memory usage detected");
    add_log_entry(analyzer, "2024-01-15 10:40:18", "ERROR", "Database connection failed");
    add_log_entry(analyzer, "2024-01-15 10:45:33", "INFO", "Backup process started");
    add_log_entry(analyzer, "2024-01-15 10:50:07", "WARNING", "CPU temperature above threshold");
    add_log_entry(analyzer, "2024-01-15 10:55:21", "INFO", "User login successful");
    add_log_entry(analyzer, "2024-01-15 11:00:15", "ERROR", "File system corruption detected");
}

void display_statistics(const struct LogAnalyzer *analyzer) {
    printf("Log Analysis Statistics:\n");
    printf("Total entries: %d\n", analyzer->count);
    printf("Errors: %d\n", analyzer->error_count);
    printf("Warnings: %d\n", analyzer->warning_count);
    printf("Info messages: %d\n", analyzer->info_count);
    
    if (analyzer->count > 0) {
        printf("Error rate: %.2f%%\n", (analyzer->error_count * 100.0) / analyzer->count);
        printf("Warning rate: %.2f%%\n", (analyzer->warning_count * 100.0) / analyzer->count);
    }
}

void display_entries_by_level(const struct LogAnalyzer *analyzer, const char *level) {
    printf("\nEntries with level '%s':\n", level);
    int found = 0;
    for (int i = 0; i < analyzer->count; i++) {
        if (strcmp(analyzer->entries[i].level, level) == 0) {
            printf("%s [%s] %s\n", analyzer->entries[i].timestamp, 
                   analyzer->entries[i].level, analyzer->entries[i].message);
            found = 1;
        }
    }
    if (!found) {
        printf("No entries found for level '%