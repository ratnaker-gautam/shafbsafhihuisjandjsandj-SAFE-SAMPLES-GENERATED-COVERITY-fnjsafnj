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
    if (analyzer == NULL) return;
    analyzer->count = 0;
    analyzer->error_count = 0;
    analyzer->warning_count = 0;
    analyzer->info_count = 0;
}

int parse_timestamp(const char *str, char *timestamp) {
    if (str == NULL || timestamp == NULL) return 0;
    if (strlen(str) < 19) return 0;
    
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (str[i] != '-') return 0;
        } else if (i == 13 || i == 16) {
            if (str[i] != ':') return 0;
        } else {
            if (!isdigit(str[i])) return 0;
        }
    }
    
    strncpy(timestamp, str, 19);
    timestamp[19] = '\0';
    return 1;
}

int parse_level(const char *str, char *level, int *severity) {
    if (str == NULL || level == NULL || severity == NULL) return 0;
    
    if (strncmp(str, "ERROR", 5) == 0) {
        strncpy(level, "ERROR", MAX_LEVEL_LENGTH - 1);
        level[MAX_LEVEL_LENGTH - 1] = '\0';
        *severity = 3;
        return 1;
    } else if (strncmp(str, "WARNING", 7) == 0) {
        strncpy(level, "WARNING", MAX_LEVEL_LENGTH - 1);
        level[MAX_LEVEL_LENGTH - 1] = '\0';
        *severity = 2;
        return 1;
    } else if (strncmp(str, "INFO", 4) == 0) {
        strncpy(level, "INFO", MAX_LEVEL_LENGTH - 1);
        level[MAX_LEVEL_LENGTH - 1] = '\0';
        *severity = 1;
        return 1;
    }
    
    return 0;
}

int add_entry(struct LogAnalyzer *analyzer, const char *timestamp, const char *level, const char *message) {
    if (analyzer == NULL || timestamp == NULL || level == NULL || message == NULL) return 0;
    if (analyzer->count >= MAX_ENTRIES) return 0;
    
    struct LogEntry *entry = &analyzer->entries[analyzer->count];
    
    if (!parse_timestamp(timestamp, entry->timestamp)) return 0;
    
    int severity;
    if (!parse_level(level, entry->level, &severity)) return 0;
    
    if (strlen(message) >= 256) return 0;
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    entry->severity = severity;
    
    if (severity == 3) analyzer->error_count++;
    else if (severity == 2) analyzer->warning_count++;
    else if (severity == 1) analyzer->info_count++;
    
    analyzer->count++;
    return 1;
}

void generate_sample_data(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    add_entry(analyzer, "2024-01-15 10:30:45", "INFO", "System started successfully");
    add_entry(analyzer, "2024-01-15 10:35:22", "WARNING", "High memory usage detected");
    add_entry(analyzer, "2024-01-15 10:40:15", "ERROR", "Database connection failed");
    add_entry(analyzer, "2024-01-15 10:45:33", "INFO", "Backup completed");
    add_entry(analyzer, "2024-01-15 10:50:01", "WARNING", "CPU temperature above threshold");
}

void display_summary(const struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", analyzer->count);
    printf("Errors: %d\n", analyzer->error_count);
    printf("Warnings: %d\n", analyzer->warning_count);
    printf("Info: %d\n", analyzer->info_count);
    printf("\n");
}

void display_entries_by_level(const struct LogAnalyzer *analyzer, const char *level) {
    if (analyzer == NULL || level == NULL) return;
    
    printf("Entries with level %s:\n", level);
    for (int i = 0; i < analyzer->count; i++) {
        if (strcmp(analyzer->entries[i].level, level) == 0) {
            printf("%s [%s