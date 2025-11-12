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
    uint32_t min_line_length;
    double avg_line_length;
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    uint32_t entry_count;
    char filename[MAX_FILENAME];
};

void init_log_analyzer(struct LogAnalyzer *analyzer, const char *filename) {
    if (analyzer == NULL || filename == NULL) return;
    
    analyzer->entry_count = 0;
    analyzer->stats.total_entries = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    analyzer->stats.max_line_length = 0;
    analyzer->stats.min_line_length = UINT32_MAX;
    analyzer->stats.avg_line_length = 0.0;
    
    strncpy(analyzer->filename, filename, MAX_FILENAME - 1);
    analyzer->filename[MAX_FILENAME - 1] = '\0';
}

int parse_log_level(const char *level_str) {
    if (level_str == NULL) return -1;
    
    if (strcmp(level_str, "ERROR") == 0) return 0;
    if (strcmp(level_str, "WARNING") == 0) return 1;
    if (strcmp(level_str, "INFO") == 0) return 2;
    
    return -1;
}

int parse_log_line(const char *line, struct LogEntry *entry, uint32_t line_num) {
    if (line == NULL || entry == NULL) return 0;
    
    char timestamp[32] = {0};
    char level[16] = {0};
    char message[256] = {0};
    
    int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    if (parsed != 3) return 0;
    
    if (parse_log_level(level) == -1) return 0;
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    entry->line_number = line_num;
    return 1;
}

int load_log_file(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return 0;
    
    FILE *file = fopen(analyzer->filename, "r");
    if (file == NULL) return 0;
    
    char line[MAX_LINE_LENGTH];
    uint32_t line_count = 0;
    uint32_t total_length = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && analyzer->entry_count < MAX_ENTRIES) {
        line_count++;
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) continue;
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry, line_count)) {
            analyzer->entries[analyzer->entry_count] = entry;
            analyzer->entry_count++;
            
            analyzer->stats.total_entries++;
            
            int level = parse_log_level(entry.level);
            if (level == 0) analyzer->stats.error_count++;
            else if (level == 1) analyzer->stats.warning_count++;
            else if (level == 2) analyzer->stats.info_count++;
            
            if (len > analyzer->stats.max_line_length) {
                analyzer->stats.max_line_length = len;
            }
            if (len < analyzer->stats.min_line_length) {
                analyzer->stats.min_line_length = len;
            }
            total_length += len;
        }
    }
    
    fclose(file);
    
    if (analyzer->stats.total_entries > 0) {
        analyzer->stats.avg_line_length = (double)total_length / analyzer->stats.total_entries;
    }
    
    return analyzer->entry_count > 0;
}

void print_log_stats(const struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    printf("Log Analysis Results for: %s\n", analyzer->filename);
    printf("Total entries processed: %u\n", analyzer->stats.total_entries);
    printf("Error entries: %u\n", analyzer->stats.error_count);
    printf("Warning entries: %u\n", analyzer->stats.warning_count);
    printf("Info entries: %u\n", analyzer