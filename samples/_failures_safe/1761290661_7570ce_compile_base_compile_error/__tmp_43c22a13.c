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

void init_log_analyzer(struct LogAnalyzer *analyzer) {
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
        } else if (i == 10) {
            if (str[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (str[i] != ':') return 0;
        } else {
            if (!isdigit(str[i])) return 0;
        }
        timestamp[i] = str[i];
    }
    timestamp[19] = '\0';
    return 1;
}

int parse_level(const char *str, char *level) {
    if (str == NULL || level == NULL) return 0;
    
    const char *levels[] = {"ERROR", "WARNING", "INFO"};
    for (int i = 0; i < 3; i++) {
        if (strncmp(str, levels[i], strlen(levels[i])) == 0) {
            strncpy(level, levels[i], MAX_LEVEL_LENGTH - 1);
            level[MAX_LEVEL_LENGTH - 1] = '\0';
            return 1;
        }
    }
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *line) {
    if (analyzer == NULL || line == NULL) return 0;
    if (analyzer->count >= MAX_ENTRIES) return 0;
    
    struct LogEntry *entry = &analyzer->entries[analyzer->count];
    
    if (!parse_timestamp(line, entry->timestamp)) return 0;
    
    const char *level_start = line + 20;
    if (!parse_level(level_start, entry->level)) return 0;
    
    const char *message_start = level_start;
    while (*message_start && !isspace(*message_start)) {
        message_start++;
    }
    while (*message_start && isspace(*message_start)) {
        message_start++;
    }
    
    if (strlen(message_start) == 0) return 0;
    size_t msg_len = strlen(message_start);
    if (msg_len >= 256) return 0;
    strncpy(entry->message, message_start, 255);
    entry->message[255] = '\0';
    
    if (strcmp(entry->level, "ERROR") == 0) {
        entry->severity = 3;
        analyzer->error_count++;
    } else if (strcmp(entry->level, "WARNING") == 0) {
        entry->severity = 2;
        analyzer->warning_count++;
    } else {
        entry->severity = 1;
        analyzer->info_count++;
    }
    
    analyzer->count++;
    return 1;
}

void generate_sample_logs(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    const char *sample_logs[] = {
        "2024-01-15 10:30:25 ERROR Database connection failed",
        "2024-01-15 10:31:00 WARNING High memory usage detected",
        "2024-01-15 10:32:15 INFO User login successful",
        "2024-01-15 10:33:45 ERROR File not found",
        "2024-01-15 10:34:20 INFO Backup completed",
        "2024-01-15 10:35:10 WARNING Slow response time",
        "2024-01-15 10:36:30 INFO Cache cleared",
        "2024-01-15 10:37:05 ERROR Permission denied",
        "2024-01-15 10:38:40 WARNING Disk space low",
        "2024-01-15 10:39:55 INFO System update available"
    };
    
    for (int i = 0; i < 10; i++) {
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

void display_entries_by