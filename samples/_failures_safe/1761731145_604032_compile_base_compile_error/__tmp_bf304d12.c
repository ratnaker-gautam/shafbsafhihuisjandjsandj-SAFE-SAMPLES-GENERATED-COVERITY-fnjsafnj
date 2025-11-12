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
    uint32_t debug_count;
    uint32_t max_line_length;
    uint32_t min_line_length;
    double avg_line_length;
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    uint32_t entry_count;
};

void initialize_analyzer(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    memset(analyzer, 0, sizeof(struct LogAnalyzer));
    analyzer->stats.min_line_length = UINT32_MAX;
}

int parse_log_level(const char *level_str, char *level_out) {
    if (level_str == NULL || level_out == NULL) return 0;
    
    size_t len = strlen(level_str);
    if (len == 0 || len >= MAX_LEVEL_LENGTH) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (!isalpha(level_str[i])) return 0;
    }
    
    strncpy(level_out, level_str, MAX_LEVEL_LENGTH - 1);
    level_out[MAX_LEVEL_LENGTH - 1] = '\0';
    return 1;
}

int parse_timestamp(const char *timestamp_str, char *timestamp_out) {
    if (timestamp_str == NULL || timestamp_out == NULL) return 0;
    
    size_t len = strlen(timestamp_str);
    if (len == 0 || len >= 32) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (!isdigit(timestamp_str[i]) && timestamp_str[i] != '-' && 
            timestamp_str[i] != ':' && timestamp_str[i] != '.' && 
            timestamp_str[i] != 'T' && timestamp_str[i] != 'Z') {
            return 0;
        }
    }
    
    strncpy(timestamp_out, timestamp_str, 31);
    timestamp_out[31] = '\0';
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer, const char *level, size_t msg_len);

int add_log_entry(struct LogAnalyzer *analyzer, const char *timestamp, 
                  const char *level, const char *message, uint32_t line_num) {
    if (analyzer == NULL || timestamp == NULL || level == NULL || message == NULL) return 0;
    if (analyzer->entry_count >= MAX_ENTRIES) return 0;
    
    struct LogEntry *entry = &analyzer->entries[analyzer->entry_count];
    
    if (!parse_timestamp(timestamp, entry->timestamp)) return 0;
    if (!parse_log_level(level, entry->level)) return 0;
    
    size_t msg_len = strlen(message);
    if (msg_len == 0 || msg_len >= 256) return 0;
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    entry->line_number = line_num;
    analyzer->entry_count++;
    
    update_stats(analyzer, level, msg_len);
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer, const char *level, size_t msg_len) {
    if (analyzer == NULL || level == NULL) return;
    
    analyzer->stats.total_entries++;
    
    if (strcmp(level, "ERROR") == 0) analyzer->stats.error_count++;
    else if (strcmp(level, "WARNING") == 0) analyzer->stats.warning_count++;
    else if (strcmp(level, "INFO") == 0) analyzer->stats.info_count++;
    else if (strcmp(level, "DEBUG") == 0) analyzer->stats.debug_count++;
    
    if (msg_len > analyzer->stats.max_line_length) {
        analyzer->stats.max_line_length = msg_len;
    }
    if (msg_len < analyzer->stats.min_line_length) {
        analyzer->stats.min_line_length = msg_len;
    }
    
    double total_length = analyzer->stats.avg_line_length * (analyzer->stats.total_entries - 1);
    total_length += msg_len;
    analyzer->stats.avg_line_length = total_length / analyzer->stats.total_entries;
}

void generate_sample_logs(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    const char *timestamps[] = {
        "2024-01-15T10:30:00Z", "2024-01-15T10:31:15Z", "2024-01-15T10:32:30Z",
        "2024-01-15T10:33:45Z", "2024-01-15T10:34:20Z"
    };
    const char *levels[] = {"INFO", "DEBUG", "WARNING", "ERROR", "INFO"};
    const char *messages[] = {
        "Application started successfully",
        "Processing user request ID 12345