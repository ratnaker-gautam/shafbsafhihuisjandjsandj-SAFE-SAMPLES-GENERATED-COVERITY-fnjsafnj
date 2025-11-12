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
    uint32_t debug_count;
    char earliest_timestamp[32];
    char latest_timestamp[32];
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    uint32_t entry_count;
};

void initialize_analyzer(struct LogAnalyzer *analyzer) {
    if (!analyzer) return;
    memset(analyzer, 0, sizeof(struct LogAnalyzer));
    analyzer->stats.earliest_timestamp[0] = '\0';
    analyzer->stats.latest_timestamp[0] = '\0';
}

int parse_timestamp(const char *str, char *timestamp) {
    if (!str || !timestamp) return 0;
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
    
    strncpy(timestamp, str, 31);
    timestamp[31] = '\0';
    return 1;
}

int parse_log_level(const char *str, char *level) {
    if (!str || !level) return 0;
    
    const char *levels[] = {"ERROR", "WARN", "INFO", "DEBUG"};
    for (int i = 0; i < 4; i++) {
        if (strncmp(str, levels[i], strlen(levels[i])) == 0) {
            strncpy(level, levels[i], 15);
            level[15] = '\0';
            return 1;
        }
    }
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *line, uint32_t line_num) {
    if (!analyzer || !line || analyzer->entry_count >= MAX_ENTRIES) return 0;
    
    struct LogEntry *entry = &analyzer->entries[analyzer->entry_count];
    char temp_timestamp[32];
    char temp_level[16];
    
    if (sscanf(line, "%31s %15s", temp_timestamp, temp_level) != 2) return 0;
    
    if (!parse_timestamp(temp_timestamp, entry->timestamp)) return 0;
    if (!parse_log_level(temp_level, entry->level)) return 0;
    
    const char *message_start = strchr(line, ' ');
    if (message_start) {
        message_start = strchr(message_start + 1, ' ');
    }
    
    if (message_start) {
        strncpy(entry->message, message_start + 1, 255);
        entry->message[255] = '\0';
    } else {
        entry->message[0] = '\0';
    }
    
    entry->line_number = line_num;
    analyzer->entry_count++;
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer) {
    if (!analyzer || analyzer->entry_count == 0) return;
    
    memset(&analyzer->stats, 0, sizeof(struct LogStats));
    analyzer->stats.total_entries = analyzer->entry_count;
    
    strcpy(analyzer->stats.earliest_timestamp, analyzer->entries[0].timestamp);
    strcpy(analyzer->stats.latest_timestamp, analyzer->entries[0].timestamp);
    
    for (uint32_t i = 0; i < analyzer->entry_count; i++) {
        struct LogEntry *entry = &analyzer->entries[i];
        
        if (strcmp(entry->level, "ERROR") == 0) analyzer->stats.error_count++;
        else if (strcmp(entry->level, "WARN") == 0) analyzer->stats.warning_count++;
        else if (strcmp(entry->level, "INFO") == 0) analyzer->stats.info_count++;
        else if (strcmp(entry->level, "DEBUG") == 0) analyzer->stats.debug_count++;
        
        if (strcmp(entry->timestamp, analyzer->stats.earliest_timestamp) < 0) {
            strcpy(analyzer->stats.earliest_timestamp, entry->timestamp);
        }
        if (strcmp(entry->timestamp, analyzer->stats.latest_timestamp) > 0) {
            strcpy(analyzer->stats.latest_timestamp, entry->timestamp);
        }
    }
}

void print_analysis(const struct LogAnalyzer *analyzer) {
    if (!analyzer) return;
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %u\n", analyzer->stats.total_