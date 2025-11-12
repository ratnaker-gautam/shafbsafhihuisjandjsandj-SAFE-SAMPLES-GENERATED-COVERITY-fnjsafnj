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
    analyzer->count = 0;
    analyzer->error_count = 0;
    analyzer->warning_count = 0;
    analyzer->info_count = 0;
    analyzer->debug_count = 0;
}

int validate_timestamp(const char *timestamp) {
    if (strlen(timestamp) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 10) {
            if (timestamp[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    return 1;
}

int validate_level(const char *level) {
    if (strcmp(level, "ERROR") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 1;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "DEBUG") == 0) return 1;
    return 0;
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    strcpy(temp_line, line);
    
    char *timestamp = strtok(temp_line, " ");
    if (!timestamp || !validate_timestamp(timestamp)) return 0;
    strcpy(entry->timestamp, timestamp);
    
    char *level = strtok(NULL, " ");
    if (!level || !validate_level(level)) return 0;
    strcpy(entry->level, level);
    
    char *message = strtok(NULL, "\n");
    if (!message) return 0;
    if (strlen(message) >= 256) return 0;
    strcpy(entry->message, message);
    
    if (strcmp(level, "ERROR") == 0) entry->severity = 4;
    else if (strcmp(level, "WARNING") == 0) entry->severity = 3;
    else if (strcmp(level, "INFO") == 0) entry->severity = 2;
    else entry->severity = 1;
    
    return 1;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *line) {
    if (analyzer->count >= MAX_ENTRIES) return 0;
    
    struct LogEntry *entry = &analyzer->entries[analyzer->count];
    if (!parse_log_line(line, entry)) return 0;
    
    if (strcmp(entry->level, "ERROR") == 0) analyzer->error_count++;
    else if (strcmp(entry->level, "WARNING") == 0) analyzer->warning_count++;
    else if (strcmp(entry->level, "INFO") == 0) analyzer->info_count++;
    else analyzer->debug_count++;
    
    analyzer->count++;
    return 1;
}

void generate_sample_logs(struct LogAnalyzer *analyzer) {
    const char *sample_logs[] = {
        "2024-01-15 10:30:45 INFO Application started successfully",
        "2024-01-15 10:31:02 DEBUG Loading configuration file",
        "2024-01-15 10:31:15 WARNING Configuration value missing, using default",
        "2024-01-15 10:32:30 ERROR Database connection failed",
        "2024-01-15 10:33:45 INFO Retrying database connection",
        "2024-01-15 10:34:10 DEBUG Connection parameters: host=localhost, port=5432",
        "2024-01-15 10:35:20 INFO Database connection established",
        "2024-01-15 10:36:05 WARNING High memory usage detected",
        "2024-01-15 10:37:30 DEBUG Memory usage: 85%",
        "2024-01-15 10:38:15 INFO Cleaning up temporary files"
    };
    
    int num_samples = sizeof(sample_logs) / sizeof(sample_logs[0]);
    for (int i = 0; i < num_samples; i++) {
        add_log_entry(analyzer, sample_logs[i]);
    }
}

void display_statistics(const struct LogAnalyzer *analyzer) {
    printf("Log Analysis Statistics:\n");
    printf("Total entries: %d\n", analyzer->count);
    printf("ERROR entries: %d\n", analyzer->error_count);
    printf("WARNING entries: %d\n