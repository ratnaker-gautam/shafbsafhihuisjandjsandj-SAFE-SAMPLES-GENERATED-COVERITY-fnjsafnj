//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000
#define MAX_FILENAME 256

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
    uint32_t line_number;
};

struct LogStats {
    uint32_t total_entries;
    uint32_t error_count;
    uint32_t warning_count;
    uint32_t info_count;
    char most_frequent_level[16];
    uint32_t max_level_count;
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    uint32_t entry_count;
};

void initialize_analyzer(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    analyzer->entry_count = 0;
    analyzer->stats.total_entries = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    strncpy(analyzer->stats.most_frequent_level, "UNKNOWN", sizeof(analyzer->stats.most_frequent_level) - 1);
    analyzer->stats.most_frequent_level[sizeof(analyzer->stats.most_frequent_level) - 1] = '\0';
    analyzer->stats.max_level_count = 0;
}

int is_valid_level(const char *level) {
    if (level == NULL) return 0;
    if (strcmp(level, "ERROR") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 1;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "DEBUG") == 0) return 1;
    return 0;
}

int parse_log_line(const char *line, struct LogEntry *entry, uint32_t line_num) {
    if (line == NULL || entry == NULL) return 0;
    
    char timestamp[32] = {0};
    char level[16] = {0};
    char message[256] = {0};
    
    int result = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    if (result != 3) return 0;
    
    if (!is_valid_level(level)) return 0;
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    entry->line_number = line_num;
    
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer, const char *level) {
    if (analyzer == NULL || level == NULL) return;
    
    analyzer->stats.total_entries++;
    
    if (strcmp(level, "ERROR") == 0) {
        analyzer->stats.error_count++;
    } else if (strcmp(level, "WARNING") == 0) {
        analyzer->stats.warning_count++;
    } else if (strcmp(level, "INFO") == 0) {
        analyzer->stats.info_count++;
    }
}

void calculate_frequent_level(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    uint32_t error_count = analyzer->stats.error_count;
    uint32_t warning_count = analyzer->stats.warning_count;
    uint32_t info_count = analyzer->stats.info_count;
    
    if (error_count >= warning_count && error_count >= info_count) {
        strncpy(analyzer->stats.most_frequent_level, "ERROR", sizeof(analyzer->stats.most_frequent_level) - 1);
        analyzer->stats.most_frequent_level[sizeof(analyzer->stats.most_frequent_level) - 1] = '\0';
        analyzer->stats.max_level_count = error_count;
    } else if (warning_count >= error_count && warning_count >= info_count) {
        strncpy(analyzer->stats.most_frequent_level, "WARNING", sizeof(analyzer->stats.most_frequent_level) - 1);
        analyzer->stats.most_frequent_level[sizeof(analyzer->stats.most_frequent_level) - 1] = '\0';
        analyzer->stats.max_level_count = warning_count;
    } else {
        strncpy(analyzer->stats.most_frequent_level, "INFO", sizeof(analyzer->stats.most_frequent_level) - 1);
        analyzer->stats.most_frequent_level[sizeof(analyzer->stats.most_frequent_level) - 1] = '\0';
        analyzer->stats.max_level_count = info_count;
    }
}

int read_log_file(struct LogAnalyzer *analyzer, const char *filename) {
    if (analyzer == NULL || filename == NULL) return 0;
    
    FILE *file = fopen(filename, "r");
    if (file == NULL) return 0;
    
    char line[MAX_LINE_LENGTH];
    uint