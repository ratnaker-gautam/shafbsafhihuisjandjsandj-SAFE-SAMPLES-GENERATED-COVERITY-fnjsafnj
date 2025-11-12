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

void init_analyzer(struct LogAnalyzer *analyzer) {
    analyzer->count = 0;
    analyzer->error_count = 0;
    analyzer->warning_count = 0;
    analyzer->info_count = 0;
}

int parse_timestamp(const char *str, char *timestamp) {
    if (strlen(str) < 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (str[i] != '-') return 0;
        } else if (i == 10) {
            if (str[i] != ' ') return 0;
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

int parse_log_level(const char *str, char *level, int *severity) {
    if (strncmp(str, "ERROR", 5) == 0) {
        strncpy(level, "ERROR", 6);
        *severity = 3;
        return 1;
    } else if (strncmp(str, "WARN", 4) == 0) {
        strncpy(level, "WARN", 5);
        *severity = 2;
        return 1;
    } else if (strncmp(str, "INFO", 4) == 0) {
        strncpy(level, "INFO", 5);
        *severity = 1;
        return 1;
    }
    return 0;
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    char temp[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strncpy(temp, line, MAX_LINE_LENGTH - 1);
    temp[MAX_LINE_LENGTH - 1] = '\0';

    char *timestamp_part = strtok(temp, " ");
    if (!timestamp_part) return 0;
    if (!parse_timestamp(timestamp_part, entry->timestamp)) return 0;

    char *level_part = strtok(NULL, " ");
    if (!level_part) return 0;
    if (!parse_log_level(level_part, entry->level, &entry->severity)) return 0;

    char *message_part = strtok(NULL, "\n");
    if (!message_part) return 0;
    if (strlen(message_part) >= 256) return 0;
    strncpy(entry->message, message_part, 255);
    entry->message[255] = '\0';

    return 1;
}

void add_entry(struct LogAnalyzer *analyzer, const char *line) {
    if (analyzer->count >= MAX_ENTRIES) return;
    if (!parse_log_line(line, &analyzer->entries[analyzer->count])) return;
    
    struct LogEntry *entry = &analyzer->entries[analyzer->count];
    if (entry->severity == 3) analyzer->error_count++;
    else if (entry->severity == 2) analyzer->warning_count++;
    else if (entry->severity == 1) analyzer->info_count++;
    
    analyzer->count++;
}

void generate_sample_logs(struct LogAnalyzer *analyzer) {
    add_entry(analyzer, "2024-01-15 10:30:25 INFO System started successfully");
    add_entry(analyzer, "2024-01-15 10:31:10 WARN High memory usage detected");
    add_entry(analyzer, "2024-01-15 10:32:45 ERROR Database connection failed");
    add_entry(analyzer, "2024-01-15 10:33:20 INFO User login successful");
    add_entry(analyzer, "2024-01-15 10:34:05 WARN Slow response time");
    add_entry(analyzer, "2024-01-15 10:35:30 ERROR File not found");
    add_entry(analyzer, "2024-01-15 10:36:15 INFO Backup completed");
}

void display_summary(const struct LogAnalyzer *analyzer) {
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", analyzer->count);
    printf("Errors: %d\n", analyzer->error_count);
    printf("Warnings: %d\n", analyzer->warning_count);
    printf("Info: %d\n", analyzer->info_count);
    printf("\n");
}

void display_entries_by_level(const struct LogAnalyzer *analyzer, int severity) {
    const char *level_name = "";
    if (severity == 3) level_name = "ERROR";
    else if (severity == 2) level_name = "WARN