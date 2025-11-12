//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
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
    return strptime(timestamp, "%Y-%m-%d %H:%M:%S", tm) != NULL;
}

int validate_log_level(const char *level) {
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *line) {
    if (analyzer->count >= MAX_ENTRIES) return 0;
    
    struct LogEntry *entry = &analyzer->entries[analyzer->count];
    struct tm time_struct;
    
    char timestamp[32], level[16], message[256];
    
    if (sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message) != 3) {
        return 0;
    }
    
    if (!parse_timestamp(timestamp, &time_struct)) {
        return 0;
    }
    
    int severity = validate_log_level(level);
    if (severity == 0) {
        return 0;
    }
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    entry->severity = severity;
    
    if (severity == 3) analyzer->error_count++;
    else if (severity == 2) analyzer->warning_count++;
    else if (severity == 1) analyzer->info_count++;
    
    analyzer->count++;
    return 1;
}

void generate_sample_logs(struct LogAnalyzer *analyzer) {
    const char *sample_logs[] = {
        "2024-01-15 10:30:00 INFO System started successfully",
        "2024-01-15 10:35:23 WARNING High memory usage detected",
        "2024-01-15 10:40:15 ERROR Database connection failed",
        "2024-01-15 10:45:00 INFO Backup completed",
        "2024-01-15 10:50:33 ERROR File not found: config.txt",
        "2024-01-15 10:55:10 WARNING Slow response time",
        "2024-01-15 11:00:00 INFO User login successful"
    };
    
    int num_samples = sizeof(sample_logs) / sizeof(sample_logs[0]);
    for (int i = 0; i < num_samples && analyzer->count < MAX_ENTRIES; i++) {
        add_log_entry(analyzer, sample_logs[i]);
    }
}

void display_statistics(const struct LogAnalyzer *analyzer) {
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", analyzer->count);
    printf("Errors: %d\n", analyzer->error_count);
    printf("Warnings: %d\n", analyzer->warning_count);
    printf("Info: %d\n", analyzer->info_count);
    
    if (analyzer->count > 0) {
        printf("Error rate: %.1f%%\n", 
               (analyzer->error_count * 100.0) / analyzer->count);
    }
}

void display_errors(const struct LogAnalyzer *analyzer) {
    printf("\nError entries:\n");
    for (int i = 0; i < analyzer->count; i++) {
        if (analyzer->entries[i].severity == 3) {
            printf("%s - %s\n", analyzer->entries[i].timestamp, 
                   analyzer->entries[i].message);
        }
    }
}

int main() {
    struct LogAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    printf("Log Analyzer\n");
    printf("Generating sample log data...\n");
    
    generate_sample_logs(&analyzer);
    
    if (analyzer.count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    display_statistics(&analyzer);
    display_errors(&analyzer);
    
    return 0;
}