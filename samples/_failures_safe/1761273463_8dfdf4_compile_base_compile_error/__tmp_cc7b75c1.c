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

int validate_level(const char *level) {
    if (level == NULL) return 0;
    if (strcmp(level, "ERROR") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 1;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "DEBUG") == 0) return 1;
    return 0;
}

int parse_severity(const char *level) {
    if (level == NULL) return 0;
    if (strcmp(level, "ERROR") == 0) return 4;
    if (strcmp(level, "WARNING") == 0) return 3;
    if (strcmp(level, "INFO") == 0) return 2;
    if (strcmp(level, "DEBUG") == 0) return 1;
    return 0;
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char level_buf[MAX_LEVEL_LENGTH];
    char timestamp_buf[20];
    char message_buf[256];
    
    int result = sscanf(line, "%19s %9s %255[^\n]", timestamp_buf, level_buf, message_buf);
    
    if (result != 3) return 0;
    
    if (!validate_level(level_buf)) return 0;
    
    if (strlen(timestamp_buf) != 19) return 0;
    if (timestamp_buf[4] != '-' || timestamp_buf[7] != '-' || 
        timestamp_buf[10] != ' ' || timestamp_buf[13] != ':' || timestamp_buf[16] != ':') return 0;
    
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7 || i == 10 || i == 13 || i == 16) continue;
        if (!isdigit(timestamp_buf[i])) return 0;
    }
    
    strncpy(entry->timestamp, timestamp_buf, 19);
    entry->timestamp[19] = '\0';
    strncpy(entry->level, level_buf, MAX_LEVEL_LENGTH - 1);
    entry->level[MAX_LEVEL_LENGTH - 1] = '\0';
    strncpy(entry->message, message_buf, 255);
    entry->message[255] = '\0';
    entry->severity = parse_severity(level_buf);
    
    return 1;
}

void add_entry(struct LogAnalyzer *analyzer, const struct LogEntry *entry) {
    if (analyzer == NULL || entry == NULL) return;
    if (analyzer->count >= MAX_ENTRIES) return;
    
    analyzer->entries[analyzer->count] = *entry;
    
    if (strcmp(entry->level, "ERROR") == 0) analyzer->error_count++;
    else if (strcmp(entry->level, "WARNING") == 0) analyzer->warning_count++;
    else if (strcmp(entry->level, "INFO") == 0) analyzer->info_count++;
    else if (strcmp(entry->level, "DEBUG") == 0) analyzer->debug_count++;
    
    analyzer->count++;
}

void generate_sample_logs(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    const char *sample_logs[] = {
        "2024-01-15 10:30:25 INFO System started successfully",
        "2024-01-15 10:31:10 DEBUG Initializing modules",
        "2024-01-15 10:32:45 WARNING Low disk space detected",
        "2024-01-15 10:33:20 ERROR Database connection failed",
        "2024-01-15 10:34:05 INFO User login successful",
        "2024-01-15 10:35:30 DEBUG Processing request ID 12345",
        "2024-01-15 10:36:15 WARNING High memory usage",
        "2024-01-15 10:37:00 INFO Backup completed",
        "2024-01-15 10:38:22 ERROR File not found: config.xml",
        "2024-01-15 10:39:10 DEBUG Cache cleared"
    };
    
    int sample_count = sizeof(sample_logs) / sizeof(sample_logs[0]);
    
    for