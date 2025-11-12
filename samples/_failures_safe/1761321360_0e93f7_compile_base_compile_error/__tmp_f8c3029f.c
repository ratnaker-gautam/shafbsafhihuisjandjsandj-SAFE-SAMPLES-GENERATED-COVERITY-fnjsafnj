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
    
    if (strlen(timestamp) == 0 || strlen(level) == 0 || strlen(message) == 0) return 0;
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->severity = parse_severity(level);
    
    return 1;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *line) {
    if (analyzer == NULL || line == NULL) return 0;
    if (analyzer->count >= MAX_ENTRIES) return 0;
    
    struct LogEntry entry;
    if (!parse_log_line(line, &entry)) return 0;
    
    analyzer->entries[analyzer->count] = entry;
    analyzer->count++;
    
    if (entry.severity == 3) analyzer->error_count++;
    else if (entry.severity == 2) analyzer->warning_count++;
    else if (entry.severity == 1) analyzer->info_count++;
    
    return 1;
}

void generate_sample_logs(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    const char *sample_logs[] = {
        "2024-01-15T10:30:00 INFO System started successfully",
        "2024-01-15T10:31:15 WARN Disk usage above 80%",
        "2024-01-15T10:32:30 ERROR Database connection failed",
        "2024-01-15T10:33:45 INFO User login successful",
        "2024-01-15T10:34:20 WARN Memory usage high",
        "2024-01-15T10:35:10 ERROR File not found: config.txt",
        "2024-01-15T10:36:05 INFO Backup completed",
        "2024-01-15T10:37:30 WARN Network latency detected",
        "2024-01-15T10:38:15 INFO Cache cleared",
        "2024-01-15T10:39:00 ERROR Permission denied: /var/log"
    };
    
    int num_logs = sizeof(sample_logs) / sizeof(sample_logs[0]);
    for (int i = 0; i < num_logs; i++) {
        add_log_entry(analyzer, sample_logs[i]);
    }
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

void display_errors(const struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    printf("Error Entries:\n");
    for (int i = 0; i < analyzer->count; i++) {
        if (analyzer->entries[i].severity == 3) {
            printf("%s %s %s\n", analyzer->entries[i].timestamp, 
                   analyzer->entries[i].level, analyzer->entries[i].message);
        }
    }
    printf("\n");
}

void display_warnings(const struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    printf("Warning Entries:\n");
    for (int i = 0; i < analyzer->count; i++) {
        if (analyzer->entries[i].severity == 2) {
            printf("%s %s %s\n