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

void init_analyzer(struct LogAnalyzer *analyzer) {
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

int parse_log_line(const char *line, struct LogEntry *entry) {
    char timestamp[32] = {0};
    char level[MAX_LEVEL_LENGTH] = {0};
    char message[256] = {0};
    time_t parsed_time;
    
    if (sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message) != 3) {
        return 0;
    }
    
    if (!parse_timestamp(timestamp, &parsed_time)) {
        return 0;
    }
    
    if (!validate_level(level)) {
        return 0;
    }
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    entry->level[sizeof(entry->level) - 1] = '\0';
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    if (strcmp(level, "ERROR") == 0) {
        entry->severity = 3;
    } else if (strcmp(level, "WARNING") == 0) {
        entry->severity = 2;
    } else {
        entry->severity = 1;
    }
    
    return 1;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *line) {
    if (analyzer->count >= MAX_ENTRIES) {
        return 0;
    }
    
    struct LogEntry *entry = &analyzer->entries[analyzer->count];
    if (!parse_log_line(line, entry)) {
        return 0;
    }
    
    time_t current_time;
    if (!parse_timestamp(entry->timestamp, &current_time)) {
        return 0;
    }
    
    if (analyzer->count == 0) {
        analyzer->start_time = current_time;
        analyzer->end_time = current_time;
    } else {
        if (current_time < analyzer->start_time) {
            analyzer->start_time = current_time;
        }
        if (current_time > analyzer->end_time) {
            analyzer->end_time = current_time;
        }
    }
    
    if (entry->severity == 3) {
        analyzer->error_count++;
    } else if (entry->severity == 2) {
        analyzer->warning_count++;
    } else {
        analyzer->info_count++;
    }
    
    analyzer->count++;
    return 1;
}

void generate_report(const struct LogAnalyzer *analyzer) {
    printf("Log Analysis Report\n");
    printf("===================\n");
    printf("Total entries: %d\n", analyzer->count);
    printf("Error count: %d\n", analyzer->error_count);
    printf("Warning count: %d\n", analyzer->warning_count);
    printf("Info count: %d\n", analyzer->info_count);
    printf("\n");
    
    if (analyzer->count > 0) {
        double error_rate = (double)analyzer->error_count / analyzer->count * 100.0;
        double warning_rate = (double)analyzer->warning_count / analyzer->count * 100.0;
        printf("Error rate: %.1f%%\n", error_rate);
        printf("Warning rate: %.1f%%\n", warning_rate);
        
        printf("\nMost recent entries:\n");
        int start = analyzer->count > 5 ? analyzer->count - 5 : 0;
        for (int i = start; i < analyzer->count; i++) {
            const struct LogEntry *entry = &analyzer->entries[i];
            printf("%s %s %s\n", entry->timestamp, entry->level, entry->message