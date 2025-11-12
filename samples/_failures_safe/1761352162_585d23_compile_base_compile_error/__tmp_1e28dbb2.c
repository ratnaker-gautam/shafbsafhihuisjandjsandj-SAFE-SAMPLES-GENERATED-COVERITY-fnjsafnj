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
    time_t start_time;
    time_t end_time;
};

void init_log_analyzer(struct LogAnalyzer *analyzer) {
    analyzer->count = 0;
    analyzer->error_count = 0;
    analyzer->warning_count = 0;
    analyzer->info_count = 0;
    analyzer->start_time = 0;
    analyzer->end_time = 0;
}

int parse_timestamp(const char *str, time_t *result) {
    struct tm tm = {0};
    if (strptime(str, "%Y-%m-%d %H:%M:%S", &tm) == NULL) {
        return 0;
    }
    *result = mktime(&tm);
    return 1;
}

int validate_level(const char *level) {
    if (strcmp(level, "ERROR") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 1;
    if (strcmp(level, "INFO") == 0) return 1;
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *timestamp, const char *level, const char *message) {
    if (analyzer->count >= MAX_ENTRIES) {
        return 0;
    }
    if (strlen(timestamp) >= 32 || strlen(level) >= MAX_LEVEL_LENGTH || strlen(message) >= 256) {
        return 0;
    }
    if (!validate_level(level)) {
        return 0;
    }
    
    struct LogEntry *entry = &analyzer->entries[analyzer->count];
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, MAX_LEVEL_LENGTH - 1);
    entry->level[MAX_LEVEL_LENGTH - 1] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    if (strcmp(level, "ERROR") == 0) {
        entry->severity = 3;
        analyzer->error_count++;
    } else if (strcmp(level, "WARNING") == 0) {
        entry->severity = 2;
        analyzer->warning_count++;
    } else {
        entry->severity = 1;
        analyzer->info_count++;
    }
    
    time_t entry_time;
    if (parse_timestamp(timestamp, &entry_time)) {
        if (analyzer->start_time == 0 || entry_time < analyzer->start_time) {
            analyzer->start_time = entry_time;
        }
        if (analyzer->end_time == 0 || entry_time > analyzer->end_time) {
            analyzer->end_time = entry_time;
        }
    }
    
    analyzer->count++;
    return 1;
}

void generate_report(const struct LogAnalyzer *analyzer) {
    printf("Log Analysis Report\n");
    printf("==================\n");
    printf("Total entries: %d\n", analyzer->count);
    printf("Errors: %d\n", analyzer->error_count);
    printf("Warnings: %d\n", analyzer->warning_count);
    printf("Info: %d\n", analyzer->info_count);
    
    if (analyzer->start_time != 0 && analyzer->end_time != 0) {
        double duration = difftime(analyzer->end_time, analyzer->start_time);
        printf("Time range: %.0f seconds\n", duration);
    }
    
    printf("\nRecent entries:\n");
    int show_count = analyzer->count > 5 ? 5 : analyzer->count;
    for (int i = analyzer->count - show_count; i < analyzer->count; i++) {
        const struct LogEntry *entry = &analyzer->entries[i];
        printf("[%s] %s: %s\n", entry->timestamp, entry->level, entry->message);
    }
}

int main(void) {
    struct LogAnalyzer analyzer;
    init_log_analyzer(&analyzer);
    
    add_log_entry(&analyzer, "2024-01-15 10:30:00", "INFO", "System started");
    add_log_entry(&analyzer, "2024-01-15 10:31:15", "WARNING", "High memory usage detected");
    add_log_entry(&analyzer, "2024-01-15 10:32:30", "ERROR", "Database connection failed");
    add_log_entry(&analyzer, "2024-01-15 10:33:45", "INFO", "Backup completed");
    add_log_entry(&analyzer, "2024-01-15 10:34:20", "WARNING", "CPU temperature high");
    add_log_entry(&analyzer, "2024-01-15 10:35:10", "INFO", "User login successful");
    add_log_entry(&analyzer, "2024-01-15