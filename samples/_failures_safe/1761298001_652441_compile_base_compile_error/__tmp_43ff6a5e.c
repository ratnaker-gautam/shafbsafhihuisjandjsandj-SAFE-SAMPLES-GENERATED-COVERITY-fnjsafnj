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
    char most_frequent_level[16];
    uint32_t max_level_count;
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    uint32_t entry_count;
    char filename[MAX_FILENAME];
};

void initialize_analyzer(struct LogAnalyzer *analyzer, const char *filename) {
    if (analyzer == NULL || filename == NULL) return;
    
    analyzer->entry_count = 0;
    strncpy(analyzer->filename, filename, MAX_FILENAME - 1);
    analyzer->filename[MAX_FILENAME - 1] = '\0';
    
    analyzer->stats.total_entries = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    analyzer->stats.debug_count = 0;
    strcpy(analyzer->stats.most_frequent_level, "UNKNOWN");
    analyzer->stats.max_level_count = 0;
}

int parse_log_level(const char *level_str) {
    if (level_str == NULL) return -1;
    
    if (strcmp(level_str, "ERROR") == 0) return 0;
    if (strcmp(level_str, "WARNING") == 0) return 1;
    if (strcmp(level_str, "INFO") == 0) return 2;
    if (strcmp(level_str, "DEBUG") == 0) return 3;
    
    return -1;
}

int parse_log_line(const char *line, struct LogEntry *entry, uint32_t line_num) {
    if (line == NULL || entry == NULL) return 0;
    
    char timestamp[32] = {0};
    char level[16] = {0};
    char message[256] = {0};
    
    int result = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    if (result < 3) return 0;
    
    if (parse_log_level(level) == -1) return 0;
    
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    entry->line_number = line_num;
    
    return 1;
}

int load_log_file(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return 0;
    
    FILE *file = fopen(analyzer->filename, "r");
    if (file == NULL) return 0;
    
    char line[MAX_LINE_LENGTH];
    uint32_t line_count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && analyzer->entry_count < MAX_ENTRIES) {
        line_count++;
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (parse_log_line(line, &analyzer->entries[analyzer->entry_count], line_count)) {
            analyzer->entry_count++;
        }
    }
    
    fclose(file);
    return 1;
}

void calculate_statistics(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    analyzer->stats.total_entries = analyzer->entry_count;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    analyzer->stats.debug_count = 0;
    
    for (uint32_t i = 0; i < analyzer->entry_count; i++) {
        const char *level = analyzer->entries[i].level;
        
        if (strcmp(level, "ERROR") == 0) analyzer->stats.error_count++;
        else if (strcmp(level, "WARNING") == 0) analyzer->stats.warning_count++;
        else if (strcmp(level, "INFO") == 0) analyzer->stats.info_count++;
        else if (strcmp(level, "DEBUG") == 0) analyzer->stats.debug_count++;
    }
    
    const char *levels[] = {"ERROR", "WARNING", "INFO", "DEBUG"};
    uint32_t counts[] = {analyzer->stats.error_count, analyzer->stats.warning_count, 
                        analyzer->stats.info_count, analyzer->stats.debug_count};
    
    uint32_t max_count = 0;
    uint32_t max_index = 0;
    
    for (int i = 0; i < 4; i++) {
        if (counts[i] > max_count) {
            max_count = counts[i];
            max_index = i;
        }
    }
    
    if