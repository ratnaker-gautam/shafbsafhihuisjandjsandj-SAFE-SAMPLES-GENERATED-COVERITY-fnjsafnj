//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000
#define MAX_LEVEL_LENGTH 16

struct LogEntry {
    char timestamp[32];
    char level[MAX_LEVEL_LENGTH];
    char message[256];
    uint32_t line_number;
};

struct LogStats {
    uint32_t total_entries;
    uint32_t error_count;
    uint32_t warning_count;
    uint32_t info_count;
    uint32_t max_message_length;
    char most_frequent_level[MAX_LEVEL_LENGTH];
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
    analyzer->stats.max_message_length = 0;
    strncpy(analyzer->stats.most_frequent_level, "UNKNOWN", MAX_LEVEL_LENGTH - 1);
    analyzer->stats.most_frequent_level[MAX_LEVEL_LENGTH - 1] = '\0';
}

int validate_level(const char *level) {
    if (level == NULL) return 0;
    if (strcmp(level, "ERROR") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 1;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "DEBUG") == 0) return 1;
    return 0;
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char timestamp[32] = {0};
    char level[MAX_LEVEL_LENGTH] = {0};
    char message[256] = {0};
    
    int result = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    if (result != 3) return 0;
    
    if (!validate_level(level)) return 0;
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer, const struct LogEntry *entry) {
    if (analyzer == NULL || entry == NULL) return;
    
    analyzer->stats.total_entries++;
    
    size_t msg_len = strlen(entry->message);
    if (msg_len > analyzer->stats.max_message_length) {
        analyzer->stats.max_message_length = msg_len;
    }
    
    if (strcmp(entry->level, "ERROR") == 0) {
        analyzer->stats.error_count++;
    } else if (strcmp(entry->level, "WARNING") == 0) {
        analyzer->stats.warning_count++;
    } else if (strcmp(entry->level, "INFO") == 0) {
        analyzer->stats.info_count++;
    }
}

void calculate_most_frequent_level(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    uint32_t error_count = analyzer->stats.error_count;
    uint32_t warning_count = analyzer->stats.warning_count;
    uint32_t info_count = analyzer->stats.info_count;
    
    if (error_count >= warning_count && error_count >= info_count) {
        strncpy(analyzer->stats.most_frequent_level, "ERROR", MAX_LEVEL_LENGTH - 1);
        analyzer->stats.most_frequent_level[MAX_LEVEL_LENGTH - 1] = '\0';
    } else if (warning_count >= error_count && warning_count >= info_count) {
        strncpy(analyzer->stats.most_frequent_level, "WARNING", MAX_LEVEL_LENGTH - 1);
        analyzer->stats.most_frequent_level[MAX_LEVEL_LENGTH - 1] = '\0';
    } else {
        strncpy(analyzer->stats.most_frequent_level, "INFO", MAX_LEVEL_LENGTH - 1);
        analyzer->stats.most_frequent_level[MAX_LEVEL_LENGTH - 1] = '\0';
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
    
    if (strlen(filename) == 0) return