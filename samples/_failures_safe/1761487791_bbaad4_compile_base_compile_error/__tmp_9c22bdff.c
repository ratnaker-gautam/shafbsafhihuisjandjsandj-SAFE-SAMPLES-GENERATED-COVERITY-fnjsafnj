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
    int debug_count;
};

void init_log_analyzer(struct LogAnalyzer *analyzer) {
    analyzer->count = 0;
    analyzer->error_count = 0;
    analyzer->warning_count = 0;
    analyzer->info_count = 0;
    analyzer->debug_count = 0;
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
    if (strcmp(level, "DEBUG") == 0) return 1;
    return 0;
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (line == NULL || strlen(line) >= MAX_LINE_LENGTH) return 0;
    
    char timestamp[20];
    char level[MAX_LEVEL_LENGTH];
    char message[256];
    
    int result = sscanf(line, "%19s %9s %255[^\n]", timestamp, level, message);
    if (result != 3) return 0;
    
    if (!validate_timestamp(timestamp)) return 0;
    if (!validate_level(level)) return 0;
    
    strncpy(entry->timestamp, timestamp, 19);
    entry->timestamp[19] = '\0';
    strncpy(entry->level, level, MAX_LEVEL_LENGTH - 1);
    entry->level[MAX_LEVEL_LENGTH - 1] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    if (strcmp(level, "ERROR") == 0) entry->severity = 4;
    else if (strcmp(level, "WARNING") == 0) entry->severity = 3;
    else if (strcmp(level, "INFO") == 0) entry->severity = 2;
    else entry->severity = 1;
    
    return 1;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *line) {
    if (analyzer->count >= MAX_ENTRIES) return 0;
    
    struct LogEntry entry;
    if (!parse_log_line(line, &entry)) return 0;
    
    analyzer->entries[analyzer->count] = entry;
    analyzer->count++;
    
    if (entry.severity == 4) analyzer->error_count++;
    else if (entry.severity == 3) analyzer->warning_count++;
    else if (entry.severity == 2) analyzer->info_count++;
    else analyzer->debug_count++;
    
    return 1;
}

void generate_sample_logs(struct LogAnalyzer *analyzer) {
    const char *sample_logs[] = {
        "2024-01-15 10:30:25 INFO Application started successfully",
        "2024-01-15 10:31:10 DEBUG Processing user request",
        "2024-01-15 10:32:45 WARNING Disk space running low",
        "2024-01-15 10:33:20 ERROR Database connection failed",
        "2024-01-15 10:34:05 INFO User login successful",
        "2024-01-15 10:35:30 DEBUG Cache updated",
        "2024-01-15 10:36:15 WARNING High memory usage detected",
        "2024-01-15 10:37:00 INFO Backup completed",
        "2024-01-15 10:38:25 ERROR File not found",
        "2024-01-15 10:39:10 DEBUG Request processed"
    };
    
    int sample_count = sizeof(sample_logs) / sizeof(sample_logs[0]);
    for (int i = 0; i < sample_count; i++) {
        add_log_entry(analyzer, sample_logs[i]);
    }
}

void print_analysis(const struct LogAnalyzer *analyzer) {
    printf("Log Analysis Report\n");
    printf("==================\n");
    printf("Total entries: %d\n", analyzer->count);
    printf("ERROR entries: %d\n", analyzer->error_count);
    printf("WARNING entries: %d\n", analyzer->warning_count);
    printf("INFO entries: %