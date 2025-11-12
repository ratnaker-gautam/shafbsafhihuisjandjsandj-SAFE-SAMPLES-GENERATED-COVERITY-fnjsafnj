//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
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
    strcpy(analyzer->stats.most_frequent_level, "NONE");
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
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
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
    } else if (strcmp(entry->level, "DEBUG") == 0) {
        analyzer->stats.debug_count++;
    }
}

void calculate_final_stats(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    uint32_t counts[] = {
        analyzer->stats.error_count,
        analyzer->stats.warning_count,
        analyzer->stats.info_count,
        analyzer->stats.debug_count
    };
    
    const char *levels[] = {"ERROR", "WARNING", "INFO", "DEBUG"};
    
    uint32_t max_count = 0;
    int max_index = -1;
    
    for (int i = 0; i < 4; i++) {
        if (counts[i] > max_count) {
            max_count = counts[i];
            max_index = i;
        }
    }
    
    if (max_index >= 0 && max_count > 0) {
        strncpy(analyzer->stats.most_frequent_level, levels[max_index], 15);
        analyzer->stats.most_frequent_level[15] = '\0';
        analyzer->stats.max_level_count = max_count;
    }
}

int load_log_file(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return 0;
    
    FILE *file = fopen(analyzer->filename, "r");
    if (file == NULL) return 0;
    
    char line[MAX_LINE_LENGTH];
    uint32_t line_num = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && analyzer->entry_count < MAX_ENTRIES) {
        line_num++;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strlen(line) == 0) continue;
        
        struct LogEntry entry;
        entry.line_number = line_num;
        
        if (parse_log_line(line, &entry)) {
            analyzer->entries[analyzer->entry_count] = entry;
            analyzer->entry_count++;
            update_stats(analyzer, &entry);
        }
    }
    
    fclose(file);
    calculate_final_stats(analyzer);
    return 1;
}

void display_statistics(const struct LogAnalyzer *analyzer) {
    if (analyzer ==