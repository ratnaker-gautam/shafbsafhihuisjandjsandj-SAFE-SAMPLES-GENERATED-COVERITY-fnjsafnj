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
};

void init_analyzer(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    analyzer->count = 0;
    analyzer->error_count = 0;
    analyzer->warning_count = 0;
    analyzer->info_count = 0;
}

int parse_timestamp(const char *str, char *timestamp) {
    if (str == NULL || timestamp == NULL) return 0;
    if (strlen(str) < 19) return 0;
    
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (str[i] != '-') return 0;
        } else if (i == 13 || i == 16) {
            if (str[i] != ':') return 0;
        } else {
            if (!isdigit(str[i])) return 0;
        }
    }
    
    strncpy(timestamp, str, 19);
    timestamp[19] = '\0';
    return 1;
}

int parse_level(const char *str, char *level, int *severity) {
    if (str == NULL || level == NULL || severity == NULL) return 0;
    
    if (strncmp(str, "ERROR", 5) == 0) {
        strncpy(level, "ERROR", MAX_LEVEL_LENGTH - 1);
        level[MAX_LEVEL_LENGTH - 1] = '\0';
        *severity = 3;
        return 1;
    } else if (strncmp(str, "WARNING", 7) == 0) {
        strncpy(level, "WARNING", MAX_LEVEL_LENGTH - 1);
        level[MAX_LEVEL_LENGTH - 1] = '\0';
        *severity = 2;
        return 1;
    } else if (strncmp(str, "INFO", 4) == 0) {
        strncpy(level, "INFO", MAX_LEVEL_LENGTH - 1);
        level[MAX_LEVEL_LENGTH - 1] = '\0';
        *severity = 1;
        return 1;
    }
    return 0;
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strcpy(temp_line, line);
    
    char *timestamp_part = strtok(temp_line, " ");
    if (timestamp_part == NULL) return 0;
    
    if (!parse_timestamp(timestamp_part, entry->timestamp)) return 0;
    
    char *level_part = strtok(NULL, " ");
    if (level_part == NULL) return 0;
    
    if (!parse_level(level_part, entry->level, &entry->severity)) return 0;
    
    char *message_part = strtok(NULL, "\n");
    if (message_part == NULL) return 0;
    
    if (strlen(message_part) >= 255) return 0;
    strcpy(entry->message, message_part);
    
    return 1;
}

void add_entry(struct LogAnalyzer *analyzer, const struct LogEntry *entry) {
    if (analyzer == NULL || entry == NULL) return;
    if (analyzer->count >= MAX_ENTRIES) return;
    
    analyzer->entries[analyzer->count] = *entry;
    
    switch (entry->severity) {
        case 3: analyzer->error_count++; break;
        case 2: analyzer->warning_count++; break;
        case 1: analyzer->info_count++; break;
    }
    
    analyzer->count++;
}

void generate_sample_logs(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    struct LogEntry sample_entries[] = {
        {"2024-01-15 10:30:45", "INFO", "System started successfully", 1},
        {"2024-01-15 10:31:22", "WARNING", "High memory usage detected", 2},
        {"2024-01-15 10:32:10", "ERROR", "Database connection failed", 3},
        {"2024-01-15 10:33:05", "INFO", "Backup completed", 1},
        {"2024-01-15 10:34:18", "WARNING", "CPU temperature rising", 2}
    };
    
    for (int i = 0; i < 5; i++) {
        add_entry(analyzer, &sample_entries[i]);
    }
}

void display_summary(const struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", analyzer->count);
    printf("Errors: %d