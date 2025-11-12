//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
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
    int debug_count;
};

void init_analyzer(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    analyzer->count = 0;
    analyzer->error_count = 0;
    analyzer->warning_count = 0;
    analyzer->info_count = 0;
    analyzer->debug_count = 0;
}

int parse_timestamp(const char *str, char *output) {
    if (str == NULL || output == NULL) return 0;
    
    int year, month, day, hour, minute, second;
    if (sscanf(str, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second) != 6) {
        return 0;
    }
    
    if (year < 2000 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;
    if (hour < 0 || hour > 23) return 0;
    if (minute < 0 || minute > 59) return 0;
    if (second < 0 || second > 59) return 0;
    
    int result = snprintf(output, 32, "%04d-%02d-%02d %02d:%02d:%02d", year, month, day, hour, minute, second);
    return (result > 0 && result < 32) ? 1 : 0;
}

int validate_level(const char *level) {
    if (level == NULL) return 0;
    
    if (strcmp(level, "ERROR") == 0) return 4;
    if (strcmp(level, "WARNING") == 0) return 3;
    if (strcmp(level, "INFO") == 0) return 2;
    if (strcmp(level, "DEBUG") == 0) return 1;
    
    return 0;
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char timestamp[32];
    char level[MAX_LEVEL_LENGTH];
    char message[256];
    
    if (sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message) != 3) {
        return 0;
    }
    
    if (!parse_timestamp(timestamp, entry->timestamp)) {
        return 0;
    }
    
    entry->severity = validate_level(level);
    if (entry->severity == 0) {
        return 0;
    }
    
    strncpy(entry->level, level, MAX_LEVEL_LENGTH - 1);
    entry->level[MAX_LEVEL_LENGTH - 1] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    return 1;
}

void add_entry(struct LogAnalyzer *analyzer, const struct LogEntry *entry) {
    if (analyzer == NULL || entry == NULL) return;
    if (analyzer->count >= MAX_ENTRIES) return;
    
    analyzer->entries[analyzer->count] = *entry;
    
    switch (entry->severity) {
        case 4: analyzer->error_count++; break;
        case 3: analyzer->warning_count++; break;
        case 2: analyzer->info_count++; break;
        case 1: analyzer->debug_count++; break;
    }
    
    analyzer->count++;
}

void generate_sample_logs(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    const char *sample_logs[] = {
        "2024-03-15 10:30:45 INFO System started successfully",
        "2024-03-15 10:31:02 DEBUG Initializing modules",
        "2024-03-15 10:31:15 WARNING Low disk space on /var",
        "2024-03-15 10:32:30 ERROR Database connection failed",
        "2024-03-15 10:33:45 INFO User login successful",
        "2024-03-15 10:34:12 DEBUG Processing request ID 12345",
        "2024-03-15 10:35:20 WARNING High memory usage detected",
        "2024-03-15 10:36:05 INFO Backup completed",
        "2024-03-15 10:37:30 ERROR File not found: config.xml",
        "2024-03-15 10:38:15 DEBUG Cache cleared"
    };
    
    int sample_count = sizeof(sample_logs) / sizeof(sample_logs[0]);
    
    for (int i = 0; i < sample_count; i++) {
        struct LogEntry entry;
        if (parse_log_line(s