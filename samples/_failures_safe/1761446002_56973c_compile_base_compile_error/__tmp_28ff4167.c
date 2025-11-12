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
    uint32_t debug_count;
    char earliest_timestamp[32];
    char latest_timestamp[32];
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    uint32_t entry_count;
};

void init_log_analyzer(struct LogAnalyzer *analyzer) {
    if (!analyzer) return;
    memset(analyzer, 0, sizeof(struct LogAnalyzer));
    analyzer->stats.earliest_timestamp[0] = '\0';
    analyzer->stats.latest_timestamp[0] = '\0';
}

int validate_timestamp(const char *timestamp) {
    if (!timestamp) return 0;
    size_t len = strlen(timestamp);
    if (len != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 10) {
            if (timestamp[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit((unsigned char)timestamp[i])) return 0;
        }
    }
    return 1;
}

int validate_level(const char *level) {
    if (!level) return 0;
    return (strcmp(level, "ERROR") == 0 || strcmp(level, "WARNING") == 0 ||
            strcmp(level, "INFO") == 0 || strcmp(level, "DEBUG") == 0);
}

int parse_log_line(const char *line, struct LogEntry *entry, uint32_t line_num) {
    if (!line || !entry || strlen(line) == 0) return 0;
    
    char timestamp[32], level[16], message[256];
    int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    
    if (parsed != 3) return 0;
    if (!validate_timestamp(timestamp)) return 0;
    if (!validate_level(level)) return 0;
    
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    entry->line_number = line_num;
    
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer, const struct LogEntry *entry) {
    if (!analyzer || !entry) return;
    
    analyzer->stats.total_entries++;
    
    if (strcmp(entry->level, "ERROR") == 0) {
        analyzer->stats.error_count++;
    } else if (strcmp(entry->level, "WARNING") == 0) {
        analyzer->stats.warning_count++;
    } else if (strcmp(entry->level, "INFO") == 0) {
        analyzer->stats.info_count++;
    } else if (strcmp(entry->level, "DEBUG") == 0) {
        analyzer->stats.debug_count++;
    }
    
    if (analyzer->stats.earliest_timestamp[0] == '\0' || 
        strcmp(entry->timestamp, analyzer->stats.earliest_timestamp) < 0) {
        strncpy(analyzer->stats.earliest_timestamp, entry->timestamp, 31);
        analyzer->stats.earliest_timestamp[31] = '\0';
    }
    
    if (analyzer->stats.latest_timestamp[0] == '\0' || 
        strcmp(entry->timestamp, analyzer->stats.latest_timestamp) > 0) {
        strncpy(analyzer->stats.latest_timestamp, entry->timestamp, 31);
        analyzer->stats.latest_timestamp[31] = '\0';
    }
}

int load_log_file(struct LogAnalyzer *analyzer, const char *filename) {
    if (!analyzer || !filename || strlen(filename) == 0) return 0;
    
    FILE *file = fopen(filename, "r");
    if (!file) return 0;
    
    char line[MAX_LINE_LENGTH];
    uint32_t line_number = 0;
    
    while (fgets(line, sizeof(line), file) && analyzer->entry_count < MAX_ENTRIES) {
        line_number++;
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry, line_number)) {
            analyzer->entries[analyzer->entry_count] = entry;
            update_stats(analyzer,