//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
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
    time_t timestamp;
    char level[MAX_LEVEL_LENGTH];
    char message[MAX_MESSAGE_LENGTH];
    int severity;
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    int count;
    int error_count;
    int warning_count;
    int info_count;
};

struct LogParser {
    char buffer[MAX_LINE_LENGTH];
    int line_number;
};

void init_log_analyzer(struct LogAnalyzer *analyzer) {
    analyzer->count = 0;
    analyzer->error_count = 0;
    analyzer->warning_count = 0;
    analyzer->info_count = 0;
}

int parse_timestamp(const char *str, time_t *timestamp) {
    struct tm tm = {0};
    if (sscanf(str, "%d-%d-%d %d:%d:%d", 
               &tm.tm_year, &tm.tm_mon, &tm.tm_mday,
               &tm.tm_hour, &tm.tm_min, &tm.tm_sec) != 6) {
        return 0;
    }
    tm.tm_year -= 1900;
    tm.tm_mon -= 1;
    *timestamp = mktime(&tm);
    return *timestamp != -1;
}

int parse_log_level(const char *level, int *severity) {
    if (strcmp(level, "ERROR") == 0) {
        *severity = 3;
        return 1;
    } else if (strcmp(level, "WARNING") == 0) {
        *severity = 2;
        return 1;
    } else if (strcmp(level, "INFO") == 0) {
        *severity = 1;
        return 1;
    }
    return 0;
}

int parse_log_line(struct LogParser *parser, struct LogEntry *entry) {
    char timestamp_str[32];
    char level_str[MAX_LEVEL_LENGTH];
    char message[MAX_MESSAGE_LENGTH];
    
    if (sscanf(parser->buffer, "%31s %15s %255[^\n]", 
               timestamp_str, level_str, message) != 3) {
        return 0;
    }
    
    if (!parse_timestamp(timestamp_str, &entry->timestamp)) {
        return 0;
    }
    
    if (!parse_log_level(level_str, &entry->severity)) {
        return 0;
    }
    
    strncpy(entry->level, level_str, MAX_LEVEL_LENGTH - 1);
    entry->level[MAX_LEVEL_LENGTH - 1] = '\0';
    strncpy(entry->message, message, MAX_MESSAGE_LENGTH - 1);
    entry->message[MAX_MESSAGE_LENGTH - 1] = '\0';
    
    return 1;
}

void add_log_entry(struct LogAnalyzer *analyzer, struct LogEntry *entry) {
    if (analyzer->count >= MAX_ENTRIES) {
        return;
    }
    
    analyzer->entries[analyzer->count] = *entry;
    analyzer->count++;
    
    switch (entry->severity) {
        case 3: analyzer->error_count++; break;
        case 2: analyzer->warning_count++; break;
        case 1: analyzer->info_count++; break;
    }
}

void generate_sample_logs(struct LogAnalyzer *analyzer) {
    struct LogEntry sample_entries[] = {
        {0, "INFO", "System started successfully", 1},
        {0, "WARNING", "High memory usage detected", 2},
        {0, "ERROR", "Database connection failed", 3},
        {0, "INFO", "User login successful", 1},
        {0, "WARNING", "Disk space running low", 2}
    };
    
    time_t base_time = time(NULL);
    for (int i = 0; i < 5; i++) {
        struct LogEntry entry = sample_entries[i];
        entry.timestamp = base_time - (4 - i) * 3600;
        add_log_entry(analyzer, &entry);
    }
}

void display_statistics(struct LogAnalyzer *analyzer) {
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", analyzer->count);
    printf("Errors: %d\n", analyzer->error_count);
    printf("Warnings: %d\n", analyzer->warning_count);
    printf("Info: %d\n", analyzer->info_count);
    
    if (analyzer->count > 0) {
        printf("\nRecent log entries:\n");
        int display_count = analyzer->count > 5 ? 5 : analyzer->count;
        for (int i = analyzer->count - display_count; i < analyzer->count; i++) {
            struct LogEntry *entry = &analyzer->entries[i];
            char time_str[32];
            struct tm *tm_info = localtime(&entry->timestamp);
            if (tm_info != NULL) {
                strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm_info);
                printf("%s %-8s %s\n", time_str, entry->level, entry->message);
            }
        }
    }
}

int