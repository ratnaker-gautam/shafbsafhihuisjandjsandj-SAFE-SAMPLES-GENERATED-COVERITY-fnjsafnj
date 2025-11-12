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
    if (!analyzer) return;
    analyzer->count = 0;
    analyzer->error_count = 0;
    analyzer->warning_count = 0;
    analyzer->info_count = 0;
}

int parse_timestamp(const char *timestamp, struct tm *tm) {
    if (!timestamp || !tm) return 0;
    return sscanf(timestamp, "%d-%d-%d %d:%d:%d",
                  &tm->tm_year, &tm->tm_mon, &tm->tm_mday,
                  &tm->tm_hour, &tm->tm_min, &tm->tm_sec) == 6;
}

int validate_log_entry(const char *timestamp, const char *level, const char *message) {
    if (!timestamp || !level || !message) return 0;
    if (strlen(timestamp) >= 32 || strlen(level) >= 16 || strlen(message) >= 256) return 0;
    
    struct tm tm;
    if (!parse_timestamp(timestamp, &tm)) return 0;
    
    if (tm.tm_year < 2000 || tm.tm_year > 2100) return 0;
    if (tm.tm_mon < 1 || tm.tm_mon > 12) return 0;
    if (tm.tm_mday < 1 || tm.tm_mday > 31) return 0;
    if (tm.tm_hour < 0 || tm.tm_hour > 23) return 0;
    if (tm.tm_min < 0 || tm.tm_min > 59) return 0;
    if (tm.tm_sec < 0 || tm.tm_sec > 59) return 0;
    
    if (strcmp(level, "ERROR") != 0 && strcmp(level, "WARNING") != 0 && strcmp(level, "INFO") != 0) {
        return 0;
    }
    
    return 1;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *timestamp, const char *level, const char *message) {
    if (!analyzer || !timestamp || !level || !message) return 0;
    if (analyzer->count >= MAX_ENTRIES) return 0;
    if (!validate_log_entry(timestamp, level, message)) return 0;
    
    struct LogEntry *entry = &analyzer->entries[analyzer->count];
    
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
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
    
    analyzer->count++;
    return 1;
}

void generate_summary(const struct LogAnalyzer *analyzer) {
    if (!analyzer) return;
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", analyzer->count);
    printf("Errors: %d\n", analyzer->error_count);
    printf("Warnings: %d\n", analyzer->warning_count);
    printf("Info: %d\n", analyzer->info_count);
    
    if (analyzer->count > 0) {
        printf("Error rate: %.2f%%\n", (analyzer->error_count * 100.0) / analyzer->count);
    }
}

void display_recent_entries(const struct LogAnalyzer *analyzer, int count) {
    if (!analyzer || count <= 0) return;
    
    int start = analyzer->count - count;
    if (start < 0) start = 0;
    
    printf("\nRecent %d log entries:\n", count);
    for (int i = start; i < analyzer->count; i++) {
        const struct LogEntry *entry = &analyzer->entries[i];
        printf("[%s] %s: %s\n", entry->timestamp, entry->level, entry->message);
    }
}

int main(void) {
    struct LogAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    add_log_entry(&analyzer, "2024-03-15 10:30:45", "INFO", "System started successfully");
    add_log_entry(&analyzer, "2024-03-15 10:31:20", "WARNING", "High memory usage detected");
    add_log_entry(&analyzer, "2024-03-15 10:32:05", "INFO