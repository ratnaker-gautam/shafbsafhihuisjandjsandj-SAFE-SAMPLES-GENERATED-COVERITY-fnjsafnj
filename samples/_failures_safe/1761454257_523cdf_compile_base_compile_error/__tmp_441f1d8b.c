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

int parse_log_level(const char *level) {
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARN") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *timestamp, const char *level, const char *message) {
    if (analyzer->count >= MAX_ENTRIES) return 0;
    if (strlen(timestamp) >= 32 || strlen(level) >= 16 || strlen(message) >= 256) return 0;
    
    struct LogEntry *entry = &analyzer->entries[analyzer->count];
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    entry->severity = parse_log_level(level);
    
    if (entry->severity == 3) analyzer->error_count++;
    else if (entry->severity == 2) analyzer->warning_count++;
    else if (entry->severity == 1) analyzer->info_count++;
    
    analyzer->count++;
    return 1;
}

void generate_sample_logs(struct LogAnalyzer *analyzer) {
    add_log_entry(analyzer, "2024-01-15 10:30:25", "INFO", "System startup completed");
    add_log_entry(analyzer, "2024-01-15 10:35:42", "WARN", "High memory usage detected");
    add_log_entry(analyzer, "2024-01-15 10:40:18", "ERROR", "Database connection failed");
    add_log_entry(analyzer, "2024-01-15 10:45:03", "INFO", "Backup process started");
    add_log_entry(analyzer, "2024-01-15 10:50:27", "WARN", "CPU temperature above threshold");
    add_log_entry(analyzer, "2024-01-15 10:55:14", "INFO", "User login successful");
    add_log_entry(analyzer, "2024-01-15 11:00:45", "ERROR", "File system corruption detected");
}

void display_statistics(const struct LogAnalyzer *analyzer) {
    printf("Log Analysis Statistics:\n");
    printf("Total entries: %d\n", analyzer->count);
    printf("Errors: %d\n", analyzer->error_count);
    printf("Warnings: %d\n", analyzer->warning_count);
    printf("Info messages: %d\n", analyzer->info_count);
    
    if (analyzer->count > 0) {
        printf("Error rate: %.1f%%\n", (analyzer->error_count * 100.0) / analyzer->count);
        printf("Warning rate: %.1f%%\n", (analyzer->warning_count * 100.0) / analyzer->count);
    }
}

void display_entries_by_level(const struct LogAnalyzer *analyzer, const char *level) {
    int target_severity = parse_log_level(level);
    printf("Entries with level '%s':\n", level);
    
    int found = 0;
    for (int i = 0; i < analyzer->count; i++) {
        if (analyzer->entries[i].severity == target_severity) {
            printf("%s [%s] %s\n", analyzer->entries[i].timestamp, 
                   analyzer->entries[i].level, analyzer->entries[i].message);
            found = 1;
        }
    }
    
    if (!found) {
        printf("No entries found with level '%s'\n", level);
    }
}

int validate_input(const char *input, int max_length) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > (size_t)max_length) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i])) return 0;
    }
    return 1;
}

int main() {
    struct LogAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    generate_sample_logs(&analyzer);
    
    printf("Log Analyzer\n");
    printf("============\n\n");
    
    display_statistics(&analyzer);
    printf("\n");
    
    char input[64];
    int running = 1;
    
    while (running)