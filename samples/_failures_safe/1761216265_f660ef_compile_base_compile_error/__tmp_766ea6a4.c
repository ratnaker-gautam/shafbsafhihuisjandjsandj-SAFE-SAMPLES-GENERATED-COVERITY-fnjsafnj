//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[20];
    char level[10];
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

void init_log_analyzer(struct LogAnalyzer *analyzer) {
    analyzer->count = 0;
    analyzer->error_count = 0;
    analyzer->warning_count = 0;
    analyzer->info_count = 0;
}

int parse_severity(const char *level) {
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARN") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    return 0;
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (line == NULL || strlen(line) == 0) return 0;
    
    char timestamp[20], level[10], message[256];
    int parsed = sscanf(line, "%19s %9s %255[^\n]", timestamp, level, message);
    
    if (parsed != 3) return 0;
    
    if (strlen(timestamp) != 19 || timestamp[4] != '-' || timestamp[7] != '-' ||
        timestamp[10] != ' ' || timestamp[13] != ':' || timestamp[16] != ':') {
        return 0;
    }
    
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7 || i == 10 || i == 13 || i == 16) continue;
        if (!isdigit(timestamp[i])) return 0;
    }
    
    strncpy(entry->timestamp, timestamp, 19);
    entry->timestamp[19] = '\0';
    strncpy(entry->level, level, 9);
    entry->level[9] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    entry->severity = parse_severity(level);
    
    return 1;
}

void add_log_entry(struct LogAnalyzer *analyzer, const struct LogEntry *entry) {
    if (analyzer->count >= MAX_ENTRIES) return;
    
    analyzer->entries[analyzer->count] = *entry;
    analyzer->count++;
    
    switch (entry->severity) {
        case 3: analyzer->error_count++; break;
        case 2: analyzer->warning_count++; break;
        case 1: analyzer->info_count++; break;
    }
}

void generate_sample_logs(struct LogAnalyzer *analyzer) {
    const char *sample_logs[] = {
        "2024-01-15 10:30:45 INFO System started successfully",
        "2024-01-15 10:31:02 WARN High memory usage detected",
        "2024-01-15 10:32:15 ERROR Database connection failed",
        "2024-01-15 10:33:40 INFO User login successful",
        "2024-01-15 10:34:55 WARN Disk space running low",
        "2024-01-15 10:35:20 ERROR File not found",
        "2024-01-15 10:36:30 INFO Backup completed",
        "2024-01-15 10:37:45 INFO Cache cleared",
        "2024-01-15 10:38:10 WARN Network latency high",
        "2024-01-15 10:39:25 ERROR Permission denied"
    };
    
    int sample_count = sizeof(sample_logs) / sizeof(sample_logs[0]);
    
    for (int i = 0; i < sample_count && analyzer->count < MAX_ENTRIES; i++) {
        struct LogEntry entry;
        if (parse_log_line(sample_logs[i], &entry)) {
            add_log_entry(analyzer, &entry);
        }
    }
}

void display_summary(const struct LogAnalyzer *analyzer) {
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", analyzer->count);
    printf("Errors: %d\n", analyzer->error_count);
    printf("Warnings: %d\n", analyzer->warning_count);
    printf("Info: %d\n", analyzer->info_count);
    printf("\n");
}

void display_errors(const struct LogAnalyzer *analyzer) {
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
    printf("Warning Entries:\n");
    for (int i = 0; i < analyzer->count; i++) {
        if (analyzer->entries[i].severity == 2) {
            printf("%s %s