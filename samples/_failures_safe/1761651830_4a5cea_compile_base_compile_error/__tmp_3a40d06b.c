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
    uint32_t max_message_length;
    char earliest_timestamp[32];
    char latest_timestamp[32];
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    uint32_t entry_count;
};

void initialize_analyzer(struct LogAnalyzer *analyzer) {
    memset(analyzer, 0, sizeof(struct LogAnalyzer));
    analyzer->stats.max_message_length = 0;
    strcpy(analyzer->stats.earliest_timestamp, "9999-12-31 23:59:59");
    strcpy(analyzer->stats.latest_timestamp, "0000-01-01 00:00:00");
}

int is_valid_timestamp(const char *timestamp) {
    if (strlen(timestamp) != 19) return 0;
    if (timestamp[4] != '-' || timestamp[7] != '-' || timestamp[10] != ' ' ||
        timestamp[13] != ':' || timestamp[16] != ':') return 0;
    
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7 || i == 10 || i == 13 || i == 16) continue;
        if (!isdigit(timestamp[i])) return 0;
    }
    
    int year, month, day, hour, minute, second;
    if (sscanf(timestamp, "%4d-%2d-%2d %2d:%2d:%2d", 
               &year, &month, &day, &hour, &minute, &second) != 6) return 0;
    
    if (year < 2000 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;
    if (hour < 0 || hour > 23) return 0;
    if (minute < 0 || minute > 59) return 0;
    if (second < 0 || second > 59) return 0;
    
    return 1;
}

int is_valid_level(const char *level) {
    return (strcmp(level, "ERROR") == 0 || 
            strcmp(level, "WARNING") == 0 || 
            strcmp(level, "INFO") == 0);
}

int parse_log_line(const char *line, struct LogEntry *entry, uint32_t line_num) {
    if (!line || strlen(line) == 0) return 0;
    
    char timestamp[32], level[16], message[256];
    int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    
    if (parsed != 3) return 0;
    if (!is_valid_timestamp(timestamp)) return 0;
    if (!is_valid_level(level)) return 0;
    if (strlen(message) == 0) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    entry->line_number = line_num;
    
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer, const struct LogEntry *entry) {
    analyzer->stats.total_entries++;
    
    if (strcmp(entry->level, "ERROR") == 0) {
        analyzer->stats.error_count++;
    } else if (strcmp(entry->level, "WARNING") == 0) {
        analyzer->stats.warning_count++;
    } else if (strcmp(entry->level, "INFO") == 0) {
        analyzer->stats.info_count++;
    }
    
    size_t msg_len = strlen(entry->message);
    if (msg_len > analyzer->stats.max_message_length) {
        analyzer->stats.max_message_length = msg_len;
    }
    
    if (strcmp(entry->timestamp, analyzer->stats.earliest_timestamp) < 0) {
        strcpy(analyzer->stats.earliest_timestamp, entry->timestamp);
    }
    if (strcmp(entry->timestamp, analyzer->stats.latest_timestamp) > 0) {
        strcpy(analyzer->stats.latest_timestamp, entry->timestamp);
    }
}

int read_log_file(struct LogAnalyzer *analyzer, const char *filename) {
    if (!filename || strlen(filename) == 0) return 0;
    
    FILE *file = fopen(filename, "r");
    if (!file) return 0;
    
    char line[MAX_LINE_LENGTH];
    uint32_t line_number = 0;
    
    while (fgets(line, sizeof(line), file) && analyzer->entry_count < MAX_ENTRIES) {
        line_number++;
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') {