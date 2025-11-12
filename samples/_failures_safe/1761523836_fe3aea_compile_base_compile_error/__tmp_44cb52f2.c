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
};

void init_log_analyzer(struct LogAnalyzer *analyzer) {
    analyzer->count = 0;
    analyzer->error_count = 0;
    analyzer->warning_count = 0;
    analyzer->info_count = 0;
}

int parse_timestamp(const char *str, char *output) {
    if (str == NULL || output == NULL) return 0;
    size_t len = strlen(str);
    if (len < 19 || len >= 32) return 0;
    
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (str[i] != '-') return 0;
        } else if (i == 10) {
            if (str[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (str[i] != ':') return 0;
        } else {
            if (!isdigit(str[i])) return 0;
        }
    }
    
    strncpy(output, str, 19);
    output[19] = '\0';
    return 1;
}

int parse_level(const char *str, char *output) {
    if (str == NULL || output == NULL) return 0;
    
    if (strncmp(str, "ERROR", 5) == 0) {
        strncpy(output, "ERROR", MAX_LEVEL_LENGTH - 1);
        return 3;
    } else if (strncmp(str, "WARNING", 7) == 0) {
        strncpy(output, "WARNING", MAX_LEVEL_LENGTH - 1);
        return 2;
    } else if (strncmp(str, "INFO", 4) == 0) {
        strncpy(output, "INFO", MAX_LEVEL_LENGTH - 1);
        return 1;
    }
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *timestamp, const char *level, const char *message) {
    if (analyzer == NULL || timestamp == NULL || level == NULL || message == NULL) return 0;
    if (analyzer->count >= MAX_ENTRIES) return 0;
    
    struct LogEntry *entry = &analyzer->entries[analyzer->count];
    
    if (!parse_timestamp(timestamp, entry->timestamp)) return 0;
    
    int severity = parse_level(level, entry->level);
    if (severity == 0) return 0;
    
    size_t msg_len = strlen(message);
    if (msg_len >= 256) return 0;
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    entry->severity = severity;
    
    if (severity == 3) analyzer->error_count++;
    else if (severity == 2) analyzer->warning_count++;
    else if (severity == 1) analyzer->info_count++;
    
    analyzer->count++;
    return 1;
}

void generate_sample_logs(struct LogAnalyzer *analyzer) {
    add_log_entry(analyzer, "2024-01-15 10:30:25", "INFO", "System startup completed");
    add_log_entry(analyzer, "2024-01-15 10:35:42", "WARNING", "High memory usage detected");
    add_log_entry(analyzer, "2024-01-15 10:40:18", "ERROR", "Database connection failed");
    add_log_entry(analyzer, "2024-01-15 10:45:03", "INFO", "Backup process started");
    add_log_entry(analyzer, "2024-01-15 10:50:29", "ERROR", "File system full");
    add_log_entry(analyzer, "2024-01-15 10:55:47", "WARNING", "Network latency increased");
    add_log_entry(analyzer, "2024-01-15 11:00:12", "INFO", "User login successful");
}

void display_statistics(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", analyzer->count);
    printf("Errors: %d\n", analyzer->error_count);
    printf("Warnings: %d\n", analyzer->warning_count);
    printf("Info: %d\n", analyzer->info_count);
    
    if (analyzer->count > 0) {
        printf("\nError rate: %.2f%%\n", 
               (float)analyzer->error_count / analyzer->count * 100);
    }
}

void display_entries_by_level(struct LogAnalyzer *analyzer, int severity) {
    if (analyzer == NULL) return;
    
    const char *level_name = "";
    if (severity == 3)