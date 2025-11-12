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
    size_t len = strlen(str);
    if (len < 19 || len >= 32) return 0;
    
    for (size_t i = 0; i < len; i++) {
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
    
    strncpy(output, str, 31);
    output[31] = '\0';
    return 1;
}

int parse_level(const char *str, char *output, int *severity) {
    if (str == NULL || output == NULL || severity == NULL) return 0;
    
    if (strcmp(str, "ERROR") == 0) {
        strcpy(output, "ERROR");
        *severity = 3;
        return 1;
    } else if (strcmp(str, "WARNING") == 0) {
        strcpy(output, "WARNING");
        *severity = 2;
        return 1;
    } else if (strcmp(str, "INFO") == 0) {
        strcpy(output, "INFO");
        *severity = 1;
        return 1;
    }
    return 0;
}

int add_entry(struct LogAnalyzer *analyzer, const char *timestamp, const char *level, const char *message) {
    if (analyzer == NULL || timestamp == NULL || level == NULL || message == NULL) return 0;
    if (analyzer->count >= MAX_ENTRIES) return 0;
    
    struct LogEntry *entry = &analyzer->entries[analyzer->count];
    
    if (!parse_timestamp(timestamp, entry->timestamp)) return 0;
    if (!parse_level(level, entry->level, &entry->severity)) return 0;
    
    size_t msg_len = strlen(message);
    if (msg_len >= 256) return 0;
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    analyzer->count++;
    
    if (entry->severity == 3) analyzer->error_count++;
    else if (entry->severity == 2) analyzer->warning_count++;
    else if (entry->severity == 1) analyzer->info_count++;
    
    return 1;
}

void generate_sample_data(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    add_entry(analyzer, "2024-01-15 10:30:25", "INFO", "System startup completed");
    add_entry(analyzer, "2024-01-15 10:35:42", "WARNING", "High memory usage detected");
    add_entry(analyzer, "2024-01-15 10:40:18", "ERROR", "Database connection failed");
    add_entry(analyzer, "2024-01-15 10:45:03", "INFO", "Backup process started");
    add_entry(analyzer, "2024-01-15 10:50:27", "WARNING", "CPU temperature above threshold");
    add_entry(analyzer, "2024-01-15 10:55:14", "INFO", "User login successful");
    add_entry(analyzer, "2024-01-15 11:00:39", "ERROR", "File system corruption detected");
}

void display_summary(const struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    printf("Log Analysis Summary\n");
    printf("===================\n");
    printf("Total entries: %d\n", analyzer->count);
    printf("INFO entries: %d\n", analyzer->info_count);
    printf("WARNING entries: %d\n", analyzer->warning_count);
    printf("ERROR entries: %d\n", analyzer->error_count);
    printf("\n");
}

void display_entries_by_level(const struct LogAnalyzer *analyzer, const char *level) {
    if (analyzer == NULL || level == NULL) return;
    
    printf("Entries with level %s:\n", level);
    printf("=====================\n");
    
    for (int i = 0; i < analyzer->count; i++) {
        const struct LogEntry *entry = &analyzer->entries[i];
        if