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

int parse_timestamp(const char *str, struct tm *tm) {
    if (strlen(str) != 19) return 0;
    if (sscanf(str, "%4d-%2d-%2d %2d:%2d:%2d",
               &tm->tm_year, &tm->tm_mon, &tm->tm_mday,
               &tm->tm_hour, &tm->tm_min, &tm->tm_sec) != 6) return 0;
    tm->tm_year -= 1900;
    tm->tm_mon -= 1;
    return 1;
}

int validate_level(const char *level) {
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *timestamp, const char *level, const char *message) {
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

void generate_report(const struct LogAnalyzer *analyzer) {
    printf("Log Analysis Report\n");
    printf("==================\n");
    printf("Total entries: %d\n", analyzer->count);
    printf("Time range: %s", ctime(&analyzer->start_time));
    printf("           to %s", ctime(&analyzer->end_time));
    printf("Error count: %d\n", analyzer->error_count);
    printf("Warning count: %d\n", analyzer->warning_count);
    printf("Info count: %d\n", analyzer->info_count);
    
    if (analyzer->count > 0) {
        double error_rate = (double)analyzer->error_count / analyzer->count * 100.0;
        printf("Error rate: %.2f%%\n", error_rate);
    }
    
    printf("\nRecent entries:\n");
    int start = analyzer->count > 5 ? analyzer->count - 5 : 0;
    for (int i = start; i < analyzer->count; i++) {
        const struct LogEntry *entry = &analyzer->entries[i];
        printf("[%s] %s: %s\n", entry->timestamp, entry->level, entry->message);
    }
}

int main(void) {
    struct LogAnalyzer analyzer;
    init_log_analyzer(&analyzer);
    
    printf("Enter log entries (timestamp level message). Format: YYYY-MM-DD HH:MM:SS LEVEL message\n");
    printf("Example: 2024-01-15 14:30:00 INFO System started\n");
    printf("Enter 'quit' to finish.\n");
    
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), stdin) != NULL) {
        if (strncmp(line, "quit", 4) ==