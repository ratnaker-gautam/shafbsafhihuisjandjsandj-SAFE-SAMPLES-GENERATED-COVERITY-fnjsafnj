//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000
#define MAX_LEVEL_LENGTH 16
#define MAX_MESSAGE_LENGTH 256

struct LogEntry {
    char timestamp[32];
    char level[MAX_LEVEL_LENGTH];
    char message[MAX_MESSAGE_LENGTH];
    int severity;
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count;
    int error_count;
    int warning_count;
    int info_count;
    int debug_count;
};

void initialize_analyzer(struct LogAnalyzer *analyzer) {
    analyzer->entry_count = 0;
    analyzer->error_count = 0;
    analyzer->warning_count = 0;
    analyzer->info_count = 0;
    analyzer->debug_count = 0;
}

int parse_timestamp(const char *timestamp, struct tm *tm) {
    return strptime(timestamp, "%Y-%m-%d %H:%M:%S", tm) != NULL;
}

int validate_level(const char *level) {
    if (strcmp(level, "ERROR") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 1;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "DEBUG") == 0) return 1;
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *timestamp, const char *level, const char *message) {
    if (analyzer->entry_count >= MAX_ENTRIES) return 0;
    if (strlen(timestamp) >= 32) return 0;
    if (strlen(level) >= MAX_LEVEL_LENGTH) return 0;
    if (strlen(message) >= MAX_MESSAGE_LENGTH) return 0;
    if (!validate_level(level)) return 0;
    
    struct tm tm;
    if (!parse_timestamp(timestamp, &tm)) return 0;
    
    struct LogEntry *entry = &analyzer->entries[analyzer->entry_count];
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, MAX_LEVEL_LENGTH - 1);
    entry->level[MAX_LEVEL_LENGTH - 1] = '\0';
    strncpy(entry->message, message, MAX_MESSAGE_LENGTH - 1);
    entry->message[MAX_MESSAGE_LENGTH - 1] = '\0';
    
    if (strcmp(level, "ERROR") == 0) {
        entry->severity = 4;
        analyzer->error_count++;
    } else if (strcmp(level, "WARNING") == 0) {
        entry->severity = 3;
        analyzer->warning_count++;
    } else if (strcmp(level, "INFO") == 0) {
        entry->severity = 2;
        analyzer->info_count++;
    } else {
        entry->severity = 1;
        analyzer->debug_count++;
    }
    
    analyzer->entry_count++;
    return 1;
}

void generate_sample_logs(struct LogAnalyzer *analyzer) {
    add_log_entry(analyzer, "2024-01-15 10:30:00", "INFO", "System started successfully");
    add_log_entry(analyzer, "2024-01-15 10:31:15", "DEBUG", "Initializing modules");
    add_log_entry(analyzer, "2024-01-15 10:32:30", "WARNING", "High memory usage detected");
    add_log_entry(analyzer, "2024-01-15 10:33:45", "ERROR", "Database connection failed");
    add_log_entry(analyzer, "2024-01-15 10:34:00", "INFO", "Retrying connection");
    add_log_entry(analyzer, "2024-01-15 10:35:20", "INFO", "Connection established");
    add_log_entry(analyzer, "2024-01-15 10:36:10", "DEBUG", "Cache populated");
}

void display_statistics(const struct LogAnalyzer *analyzer) {
    printf("Log Analysis Statistics:\n");
    printf("Total entries: %d\n", analyzer->entry_count);
    printf("ERROR entries: %d\n", analyzer->error_count);
    printf("WARNING entries: %d\n", analyzer->warning_count);
    printf("INFO entries: %d\n", analyzer->info_count);
    printf("DEBUG entries: %d\n", analyzer->debug_count);
    
    if (analyzer->entry_count > 0) {
        printf("Error percentage: %.2f%%\n", 
               (float)analyzer->error_count / analyzer->entry_count * 100);
    }
}

void display_entries_by_level(const struct LogAnalyzer *analyzer, const char *level) {
    printf("\nEntries with level %s:\n", level);
    for (int i = 0; i < analyzer->entry_count; i++) {
        if (strcmp(analyzer->entries[i].level, level) == 0) {
            printf("[%s] %s: %s\n", analyzer->entries[i].timestamp, 
                   analyzer->entries[i].level, analyzer->entries[i].