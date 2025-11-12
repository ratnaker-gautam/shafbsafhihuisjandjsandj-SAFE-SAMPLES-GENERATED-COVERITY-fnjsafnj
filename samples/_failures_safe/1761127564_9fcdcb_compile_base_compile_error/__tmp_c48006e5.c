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
    if (analyzer == NULL) return;
    analyzer->count = 0;
    analyzer->error_count = 0;
    analyzer->warning_count = 0;
    analyzer->info_count = 0;
    analyzer->start_time = 0;
    analyzer->end_time = 0;
}

int parse_timestamp(const char *str, struct tm *tm) {
    if (str == NULL || tm == NULL) return 0;
    if (strlen(str) != 19) return 0;
    if (sscanf(str, "%d-%d-%d %d:%d:%d", 
               &tm->tm_year, &tm->tm_mon, &tm->tm_mday,
               &tm->tm_hour, &tm->tm_min, &tm->tm_sec) != 6) return 0;
    tm->tm_year -= 1900;
    tm->tm_mon -= 1;
    return 1;
}

int validate_level(const char *level) {
    if (level == NULL) return 0;
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *timestamp, 
                  const char *level, const char *message) {
    if (analyzer == NULL || timestamp == NULL || level == NULL || message == NULL) return 0;
    if (analyzer->count >= MAX_ENTRIES) return 0;
    if (strlen(timestamp) >= 32) return 0;
    if (strlen(level) >= MAX_LEVEL_LENGTH) return 0;
    if (strlen(message) >= 256) return 0;
    
    struct tm tm;
    if (!parse_timestamp(timestamp, &tm)) return 0;
    
    int severity = validate_level(level);
    if (severity == 0) return 0;
    
    struct LogEntry *entry = &analyzer->entries[analyzer->count];
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, MAX_LEVEL_LENGTH - 1);
    entry->level[MAX_LEVEL_LENGTH - 1] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    entry->severity = severity;
    
    time_t entry_time = mktime(&tm);
    if (analyzer->count == 0) {
        analyzer->start_time = entry_time;
        analyzer->end_time = entry_time;
    } else {
        if (entry_time < analyzer->start_time) analyzer->start_time = entry_time;
        if (entry_time > analyzer->end_time) analyzer->end_time = entry_time;
    }
    
    if (severity == 3) analyzer->error_count++;
    else if (severity == 2) analyzer->warning_count++;
    else if (severity == 1) analyzer->info_count++;
    
    analyzer->count++;
    return 1;
}

void generate_sample_logs(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    add_log_entry(analyzer, "2024-01-15 10:30:00", "INFO", "System started successfully");
    add_log_entry(analyzer, "2024-01-15 10:35:22", "WARNING", "High memory usage detected");
    add_log_entry(analyzer, "2024-01-15 11:20:15", "ERROR", "Database connection failed");
    add_log_entry(analyzer, "2024-01-15 12:05:30", "INFO", "Backup completed");
    add_log_entry(analyzer, "2024-01-15 13:45:10", "ERROR", "Network timeout");
    add_log_entry(analyzer, "2024-01-15 14:20:05", "WARNING", "CPU temperature high");
    add_log_entry(analyzer, "2024-01-15 15:10:45", "INFO", "User login successful");
}

void print_statistics(const struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", analyzer->count);
    printf("Time range: ");
    if (analyzer->count > 0) {
        char start_buf[32], end_buf[32];
        struct tm tm_start, tm_end;
        struct tm *tm_start_ptr = localtime(&analyzer