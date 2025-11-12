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
    analyzer->count = 0;
    analyzer->error_count = 0;
    analyzer->warning_count = 0;
    analyzer->info_count = 0;
}

int parse_timestamp(const char *timestamp, struct tm *tm) {
    return sscanf(timestamp, "%d-%d-%d %d:%d:%d",
                  &tm->tm_year, &tm->tm_mon, &tm->tm_mday,
                  &tm->tm_hour, &tm->tm_min, &tm->tm_sec) == 6;
}

int validate_log_level(const char *level) {
    if (strcmp(level, "ERROR") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 1;
    if (strcmp(level, "INFO") == 0) return 1;
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *timestamp, 
                  const char *level, const char *message) {
    if (analyzer->count >= MAX_ENTRIES) return 0;
    
    struct tm tm;
    if (!parse_timestamp(timestamp, &tm)) return 0;
    
    if (!validate_log_level(level)) return 0;
    
    if (strlen(message) >= 256) return 0;
    
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

void generate_sample_logs(struct LogAnalyzer *analyzer) {
    add_log_entry(analyzer, "2024-01-15 10:30:45", "INFO", "System started successfully");
    add_log_entry(analyzer, "2024-01-15 10:35:22", "WARNING", "High memory usage detected");
    add_log_entry(analyzer, "2024-01-15 11:20:15", "ERROR", "Database connection failed");
    add_log_entry(analyzer, "2024-01-15 11:25:30", "INFO", "Backup process completed");
    add_log_entry(analyzer, "2024-01-15 12:00:00", "WARNING", "CPU temperature above threshold");
}

void display_summary(const struct LogAnalyzer *analyzer) {
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", analyzer->count);
    printf("Errors: %d\n", analyzer->error_count);
    printf("Warnings: %d\n", analyzer->warning_count);
    printf("Info: %d\n", analyzer->info_count);
    
    if (analyzer->count > 0) {
        float error_percent = (float)analyzer->error_count / analyzer->count * 100;
        float warning_percent = (float)analyzer->warning_count / analyzer->count * 100;
        printf("Error rate: %.1f%%\n", error_percent);
        printf("Warning rate: %.1f%%\n", warning_percent);
    }
}

void display_entries_by_level(const struct LogAnalyzer *analyzer, const char *level) {
    printf("\n%s entries:\n", level);
    int found = 0;
    
    for (int i = 0; i < analyzer->count; i++) {
        if (strcmp(analyzer->entries[i].level, level) == 0) {
            printf("%s - %s: %s\n", analyzer->entries[i].timestamp, 
                   analyzer->entries[i].level, analyzer->entries[i].message);
            found = 1;
        }
    }
    
    if (!found) {
        printf("No %s entries found.\n", level);
    }
}

int main() {
    struct LogAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    printf("Log Analyzer\n");
    printf("Generating sample log data...\n");
    
    generate_sample_logs(&analyzer);
    
    int choice;
    do {
        printf("\nMenu:\n");
        printf("1. Display summary\n");
        printf("2. Show ERROR entries\n");
        printf("3. Show WARNING entries\n");
        printf("