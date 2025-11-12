//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000
#define MAX_LEVEL_LENGTH 16
#define MAX_MESSAGE_LENGTH 256

struct LogEntry {
    time_t timestamp;
    char level[MAX_LEVEL_LENGTH];
    char message[MAX_MESSAGE_LENGTH];
    int severity;
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    int count;
    int error_count;
    int warning_count;
    int info_count;
};

struct LogParser {
    char buffer[MAX_LINE_LENGTH];
    int line_number;
};

void init_log_analyzer(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    analyzer->count = 0;
    analyzer->error_count = 0;
    analyzer->warning_count = 0;
    analyzer->info_count = 0;
}

int parse_timestamp(const char *str, time_t *timestamp) {
    if (str == NULL || timestamp == NULL) return 0;
    struct tm tm = {0};
    if (strptime(str, "%Y-%m-%d %H:%M:%S", &tm) == NULL) {
        return 0;
    }
    *timestamp = mktime(&tm);
    return 1;
}

int parse_log_level(const char *str, char *level, int *severity) {
    if (str == NULL || level == NULL || severity == NULL) return 0;
    
    if (strncasecmp(str, "ERROR", 5) == 0) {
        strncpy(level, "ERROR", MAX_LEVEL_LENGTH - 1);
        level[MAX_LEVEL_LENGTH - 1] = '\0';
        *severity = 3;
        return 1;
    } else if (strncasecmp(str, "WARNING", 7) == 0) {
        strncpy(level, "WARNING", MAX_LEVEL_LENGTH - 1);
        level[MAX_LEVEL_LENGTH - 1] = '\0';
        *severity = 2;
        return 1;
    } else if (strncasecmp(str, "INFO", 4) == 0) {
        strncpy(level, "INFO", MAX_LEVEL_LENGTH - 1);
        level[MAX_LEVEL_LENGTH - 1] = '\0';
        *severity = 1;
        return 1;
    }
    return 0;
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char timestamp_str[32];
    char level_str[MAX_LEVEL_LENGTH];
    char message[MAX_MESSAGE_LENGTH];
    
    int result = sscanf(line, "%31s %15s %255[^\n]", 
                       timestamp_str, level_str, message);
    
    if (result != 3) return 0;
    
    if (!parse_timestamp(timestamp_str, &entry->timestamp)) return 0;
    if (!parse_log_level(level_str, entry->level, &entry->severity)) return 0;
    
    strncpy(entry->message, message, MAX_MESSAGE_LENGTH - 1);
    entry->message[MAX_MESSAGE_LENGTH - 1] = '\0';
    
    return 1;
}

int add_log_entry(struct LogAnalyzer *analyzer, const struct LogEntry *entry) {
    if (analyzer == NULL || entry == NULL) return 0;
    if (analyzer->count >= MAX_ENTRIES) return 0;
    
    analyzer->entries[analyzer->count] = *entry;
    
    if (entry->severity == 3) analyzer->error_count++;
    else if (entry->severity == 2) analyzer->warning_count++;
    else if (entry->severity == 1) analyzer->info_count++;
    
    analyzer->count++;
    return 1;
}

void print_statistics(const struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", analyzer->count);
    printf("Errors: %d\n", analyzer->error_count);
    printf("Warnings: %d\n", analyzer->warning_count);
    printf("Info: %d\n", analyzer->info_count);
    
    if (analyzer->count > 0) {
        printf("Error rate: %.2f%%\n", 
               (float)analyzer->error_count / analyzer->count * 100);
    }
}

int read_log_file(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return 0;
    
    char filename[256];
    printf("Enter log file name: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) return 0;
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    if (strlen(filename) == 0) return 0;
    
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Cannot open file: %s\n", filename);
        return 0;
    }
    
    struct LogParser parser;
    parser.line_number = 0;
    
    while (fgets(parser.buffer, sizeof(