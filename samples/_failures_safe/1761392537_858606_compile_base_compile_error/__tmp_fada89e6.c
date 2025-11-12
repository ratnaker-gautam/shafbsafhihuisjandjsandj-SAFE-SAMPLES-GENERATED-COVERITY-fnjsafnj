//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define MAX_LEVEL_LEN 10
#define MAX_MESSAGE_LEN 512

struct LogEntry {
    char timestamp[20];
    char level[MAX_LEVEL_LEN];
    char message[MAX_MESSAGE_LEN];
    uint32_t line_number;
};

struct LogStats {
    uint32_t total_entries;
    uint32_t error_count;
    uint32_t warning_count;
    uint32_t info_count;
    uint32_t debug_count;
    char most_frequent_level[MAX_LEVEL_LEN];
    uint32_t max_level_count;
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    uint32_t entry_count;
};

void init_log_analyzer(struct LogAnalyzer *analyzer) {
    memset(analyzer, 0, sizeof(struct LogAnalyzer));
    strcpy(analyzer->stats.most_frequent_level, "UNKNOWN");
}

int validate_timestamp(const char *timestamp) {
    if (strlen(timestamp) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 10) {
            if (timestamp[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    return 1;
}

int validate_level(const char *level) {
    if (strlen(level) == 0 || strlen(level) >= MAX_LEVEL_LEN) return 0;
    const char *valid_levels[] = {"ERROR", "WARN", "INFO", "DEBUG"};
    for (size_t i = 0; i < 4; i++) {
        if (strcmp(level, valid_levels[i]) == 0) return 1;
    }
    return 0;
}

int parse_log_line(const char *line, struct LogEntry *entry, uint32_t line_num) {
    if (line == NULL || entry == NULL) return 0;
    
    char timestamp[20], level[MAX_LEVEL_LEN], message[MAX_MESSAGE_LEN];
    int parsed = sscanf(line, "%19s %9s %511[^\n]", timestamp, level, message);
    
    if (parsed != 3) return 0;
    if (!validate_timestamp(timestamp)) return 0;
    if (!validate_level(level)) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    entry->line_number = line_num;
    
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer, const char *level) {
    analyzer->stats.total_entries++;
    
    if (strcmp(level, "ERROR") == 0) {
        analyzer->stats.error_count++;
    } else if (strcmp(level, "WARN") == 0) {
        analyzer->stats.warning_count++;
    } else if (strcmp(level, "INFO") == 0) {
        analyzer->stats.info_count++;
    } else if (strcmp(level, "DEBUG") == 0) {
        analyzer->stats.debug_count++;
    }
    
    uint32_t current_count = 0;
    if (strcmp(level, "ERROR") == 0) current_count = analyzer->stats.error_count;
    else if (strcmp(level, "WARN") == 0) current_count = analyzer->stats.warning_count;
    else if (strcmp(level, "INFO") == 0) current_count = analyzer->stats.info_count;
    else if (strcmp(level, "DEBUG") == 0) current_count = analyzer->stats.debug_count;
    
    if (current_count > analyzer->stats.max_level_count) {
        analyzer->stats.max_level_count = current_count;
        strcpy(analyzer->stats.most_frequent_level, level);
    }
}

void print_stats(const struct LogStats *stats) {
    printf("Log Analysis Results:\n");
    printf("Total entries: %u\n", stats->total_entries);
    printf("ERROR count: %u\n", stats->error_count);
    printf("WARN count: %u\n", stats->warning_count);
    printf("INFO count: %u\n", stats->info_count);
    printf("DEBUG count: %u\n", stats->debug_count);
    printf("Most frequent level: %s (%u occurrences)\n", 
           stats->most_frequent_level, stats->max_level_count);
}

int main(void) {
    struct LogAnalyzer analyzer;
    init_log_analyzer(&analyzer);
    
    char line[MAX_LINE_LEN];
    uint32_t line_number = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    while (fgets(line, sizeof(line), stdin) != NULL) {
        line_number++;
        
        if (line_number > MAX_ENT