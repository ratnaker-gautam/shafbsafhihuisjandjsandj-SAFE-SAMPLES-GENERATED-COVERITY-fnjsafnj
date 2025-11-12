//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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
    uint32_t max_line_length;
    time_t earliest_time;
    time_t latest_time;
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    uint32_t entry_count;
};

void init_log_analyzer(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    memset(analyzer, 0, sizeof(struct LogAnalyzer));
    analyzer->stats.earliest_time = 0;
    analyzer->stats.latest_time = 0;
}

int parse_timestamp(const char *timestamp_str, time_t *result) {
    if (timestamp_str == NULL || result == NULL) return 0;
    struct tm tm = {0};
    if (strptime(timestamp_str, "%Y-%m-%d %H:%M:%S", &tm) == NULL) {
        return 0;
    }
    *result = mktime(&tm);
    return 1;
}

int is_valid_log_level(const char *level) {
    if (level == NULL) return 0;
    return (strcmp(level, "ERROR") == 0 || 
            strcmp(level, "WARNING") == 0 || 
            strcmp(level, "INFO") == 0);
}

int parse_log_line(const char *line, struct LogEntry *entry, uint32_t line_num) {
    if (line == NULL || entry == NULL) return 0;
    
    char timestamp[32], level[16], message[256];
    int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    
    if (parsed != 3) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp) ||
        strlen(level) >= sizeof(entry->level) ||
        strlen(message) >= sizeof(entry->message)) {
        return 0;
    }
    
    if (!is_valid_log_level(level)) return 0;
    
    time_t entry_time;
    if (!parse_timestamp(timestamp, &entry_time)) return 0;
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->line_number = line_num;
    
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    entry->level[sizeof(entry->level) - 1] = '\0';
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer, const struct LogEntry *entry) {
    if (analyzer == NULL || entry == NULL) return;
    
    analyzer->stats.total_entries++;
    
    if (strcmp(entry->level, "ERROR") == 0) {
        analyzer->stats.error_count++;
    } else if (strcmp(entry->level, "WARNING") == 0) {
        analyzer->stats.warning_count++;
    } else if (strcmp(entry->level, "INFO") == 0) {
        analyzer->stats.info_count++;
    }
    
    time_t entry_time;
    if (parse_timestamp(entry->timestamp, &entry_time)) {
        if (analyzer->stats.earliest_time == 0 || entry_time < analyzer->stats.earliest_time) {
            analyzer->stats.earliest_time = entry_time;
        }
        if (analyzer->stats.latest_time == 0 || entry_time > analyzer->stats.latest_time) {
            analyzer->stats.latest_time = entry_time;
        }
    }
    
    size_t line_len = strlen(entry->timestamp) + strlen(entry->level) + strlen(entry->message) + 3;
    if (line_len > analyzer->stats.max_line_length) {
        analyzer->stats.max_line_length = line_len;
    }
}

int process_log_file(struct LogAnalyzer *analyzer, const char *filename) {
    if (analyzer == NULL || filename == NULL) return 0;
    
    FILE *file = fopen(filename, "r");
    if (file == NULL) return 0;
    
    char line[MAX_LINE_LENGTH];
    uint32_t line_number = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && analyzer->entry_count < MAX_ENTRIES) {
        line_number++;
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (len >= sizeof(line) - 1) {
            continue;
        }
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry, line_number)) {
            if (analyzer->