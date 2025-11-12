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

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char *timestamp = strtok(temp_line, " ");
    char *level = strtok(NULL, " ");
    char *message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) {
        return 0;
    }
    
    if (strlen(timestamp) >= sizeof(entry->timestamp) ||
        strlen(level) >= sizeof(entry->level) ||
        strlen(message) >= sizeof(entry->message)) {
        return 0;
    }
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    for (char *p = entry->level; *p; p++) {
        *p = toupper(*p);
    }
    
    return 1;
}

int load_log_file(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return 0;
    
    FILE *file = fopen(analyzer->filename, "r");
    if (file == NULL) {
        return 0;
    }
    
    char line[MAX_LINE_LENGTH];
    uint32_t line_num = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && analyzer->entry_count < MAX_ENTRIES) {
        line_num++;
        if (strlen(line) > 0 && line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }
        
        if (strlen(line) == 0) continue;
        
        struct LogEntry entry;
        entry.line_number = line_num;
        
        if (parse_log_line(line, &entry)) {
            analyzer->entries[analyzer->entry_count] = entry;
            analyzer->entry_count++;
        }
    }
    
    fclose(file);
    return 1;
}

void analyze_logs(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    analyzer->stats.total_entries = analyzer->entry_count;
    
    uint32_t level_counts[4] = {0};
    const char *levels[] = {"ERROR", "WARNING", "INFO", "DEBUG"};
    
    for (uint32_t i = 0; i < analyzer->entry_count; i++) {
        const char *level = analyzer->entries[i].level;
        
        if (strcmp(level, "ERROR") == 0) {
            analyzer->stats.error_count++;
            level_counts[0]++;
        } else if (strcmp(level, "WARNING") == 0) {
            analyzer->stats.warning_count++;
            level_counts[1]++;
        } else if (strcmp(level, "INFO") == 0) {
            analyzer->stats.info_count++;
            level_counts[2]++;
        } else if (strcmp(level, "DEBUG") == 0) {
            analyzer->stats.debug_count++;
            level_counts[3]++;
        }
    }
    
    uint32_t max_count = 0;
    uint32_t max_index = 0;
    
    for (int i = 0; i < 4; i++) {
        if (level_counts[i] > max_count) {
            max_count = level_counts[i];
            max_index = i;
        }
    }
    
    if (max_count > 0) {
        strcpy(analyzer->stats.most_frequent_level, levels[max_index]);
        analyzer->stats.max_level_count = max_count;
    }
}

void display_statistics(const struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    printf