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

int parse_timestamp(const char *str, char *output) {
    if (str == NULL || output == NULL) return 0;
    size_t len = strlen(str);
    if (len < 19 || len >= 32) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (i == 10) {
            if (str[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (str[i] != ':') return 0;
        } else {
            if (!isdigit(str[i]) && str[i] != ' ') return 0;
        }
    }
    
    strncpy(output, str, 31);
    output[31] = '\0';
    return 1;
}

int validate_level(const char *level, int *severity) {
    if (level == NULL || severity == NULL) return 0;
    
    if (strcmp(level, "ERROR") == 0) {
        *severity = 3;
        return 1;
    } else if (strcmp(level, "WARNING") == 0) {
        *severity = 2;
        return 1;
    } else if (strcmp(level, "INFO") == 0) {
        *severity = 1;
        return 1;
    }
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *timestamp, const char *level, const char *message) {
    if (analyzer == NULL || timestamp == NULL || level == NULL || message == NULL) return 0;
    if (analyzer->count >= MAX_ENTRIES) return 0;
    
    struct LogEntry *entry = &analyzer->entries[analyzer->count];
    
    if (!parse_timestamp(timestamp, entry->timestamp)) return 0;
    
    int severity;
    if (!validate_level(level, &severity)) return 0;
    
    if (strlen(message) >= 256) return 0;
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    entry->severity = severity;
    
    analyzer->count++;
    
    switch (severity) {
        case 3: analyzer->error_count++; break;
        case 2: analyzer->warning_count++; break;
        case 1: analyzer->info_count++; break;
    }
    
    return 1;
}

void generate_sample_logs(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    add_log_entry(analyzer, "2024-01-15 10:30:25", "INFO", "System startup completed");
    add_log_entry(analyzer, "2024-01-15 10:35:42", "WARNING", "High memory usage detected");
    add_log_entry(analyzer, "2024-01-15 10:40:18", "ERROR", "Database connection failed");
    add_log_entry(analyzer, "2024-01-15 10:45:03", "INFO", "Backup process started");
    add_log_entry(analyzer, "2024-01-15 10:50:57", "WARNING", "CPU temperature above threshold");
}

void display_statistics(const struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    printf("Log Analysis Statistics:\n");
    printf("Total entries: %d\n", analyzer->count);
    printf("Errors: %d\n", analyzer->error_count);
    printf("Warnings: %d\n", analyzer->warning_count);
    printf("Info: %d\n", analyzer->info_count);
    
    if (analyzer->count > 0) {
        printf("Error rate: %.1f%%\n", (analyzer->error_count * 100.0) / analyzer->count);
        printf("Warning rate: %.1f%%\n", (analyzer->warning_count * 100.0) / analyzer->count);
    }
}

void display_entries_by_level(const struct LogAnalyzer *analyzer, const char *level) {
    if (analyzer == NULL || level == NULL) return;
    
    printf("\nEntries with level '%s':\n", level);
    int found = 0;
    
    for (int i = 0; i < analyzer->count; i++) {
        if (strcmp(analyzer->entries[i].level, level) == 0) {
            printf("%s [%s] %s\n", analyzer->entries[i].timestamp