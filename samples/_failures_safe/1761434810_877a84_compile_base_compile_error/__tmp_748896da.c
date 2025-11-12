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
    if (analyzer == NULL) return;
    analyzer->count = 0;
    analyzer->error_count = 0;
    analyzer->warning_count = 0;
    analyzer->info_count = 0;
}

int parse_timestamp(const char *str, char *output) {
    if (str == NULL || output == NULL) return 0;
    
    int year, month, day, hour, min, sec;
    if (sscanf(str, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &min, &sec) != 6) {
        return 0;
    }
    
    if (year < 2000 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;
    if (hour < 0 || hour > 23) return 0;
    if (min < 0 || min > 59) return 0;
    if (sec < 0 || sec > 59) return 0;
    
    int result = snprintf(output, 32, "%04d-%02d-%02d %02d:%02d:%02d", year, month, day, hour, min, sec);
    return (result > 0 && result < 32) ? 1 : 0;
}

int parse_log_level(const char *str, char *output, int *severity) {
    if (str == NULL || output == NULL || severity == NULL) return 0;
    
    if (strncasecmp(str, "ERROR", 5) == 0) {
        strncpy(output, "ERROR", 16);
        output[15] = '\0';
        *severity = 3;
        return 1;
    } else if (strncasecmp(str, "WARNING", 7) == 0) {
        strncpy(output, "WARNING", 16);
        output[15] = '\0';
        *severity = 2;
        return 1;
    } else if (strncasecmp(str, "INFO", 4) == 0) {
        strncpy(output, "INFO", 16);
        output[15] = '\0';
        *severity = 1;
        return 1;
    }
    
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *timestamp, const char *level, const char *message) {
    if (analyzer == NULL || timestamp == NULL || level == NULL || message == NULL) return 0;
    if (analyzer->count >= MAX_ENTRIES) return 0;
    
    struct LogEntry *entry = &analyzer->entries[analyzer->count];
    
    if (!parse_timestamp(timestamp, entry->timestamp)) return 0;
    if (!parse_log_level(level, entry->level, &entry->severity)) return 0;
    
    if (strlen(message) >= 256) return 0;
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    analyzer->count++;
    
    if (entry->severity == 3) analyzer->error_count++;
    else if (entry->severity == 2) analyzer->warning_count++;
    else if (entry->severity == 1) analyzer->info_count++;
    
    return 1;
}

void generate_sample_logs(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    add_log_entry(analyzer, "2024-03-15 10:30:45", "INFO", "System started successfully");
    add_log_entry(analyzer, "2024-03-15 10:31:22", "WARNING", "High memory usage detected");
    add_log_entry(analyzer, "2024-03-15 10:32:10", "ERROR", "Database connection failed");
    add_log_entry(analyzer, "2024-03-15 10:33:05", "INFO", "Backup process initiated");
    add_log_entry(analyzer, "2024-03-15 10:34:30", "ERROR", "File system corruption detected");
    add_log_entry(analyzer, "2024-03-15 10:35:12", "WARNING", "Network latency above threshold");
    add_log_entry(analyzer, "2024-03-15 10:36:45", "INFO", "User login successful");
}

void display_statistics(const struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    printf("Log Analysis Statistics:\n");
    printf("Total entries: %d\n", analyzer->count);
    printf("Errors: %d\n", analyzer->error_count);
    printf("Warnings: %d\n", analyzer->warning_count