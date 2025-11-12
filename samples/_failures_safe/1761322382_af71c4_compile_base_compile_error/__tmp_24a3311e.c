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
};

void initialize_analyzer(struct LogAnalyzer *analyzer) {
    memset(analyzer, 0, sizeof(struct LogAnalyzer));
    strncpy(analyzer->stats.most_frequent_level, "NONE", 15);
}

int validate_filename(const char *filename) {
    if (filename == NULL || strlen(filename) == 0 || strlen(filename) >= MAX_FILENAME) {
        return 0;
    }
    for (size_t i = 0; i < strlen(filename); i++) {
        if (!isalnum((unsigned char)filename[i]) && filename[i] != '.' && filename[i] != '-' && filename[i] != '_') {
            return 0;
        }
    }
    return 1;
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL || strlen(line) >= MAX_LINE_LENGTH) {
        return 0;
    }
    
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
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    entry->level[sizeof(entry->level) - 1] = '\0';
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer, const char *level) {
    if (analyzer == NULL || level == NULL) {
        return;
    }
    
    analyzer->stats.total_entries++;
    
    if (strcmp(level, "ERROR") == 0) {
        analyzer->stats.error_count++;
    } else if (strcmp(level, "WARNING") == 0) {
        analyzer->stats.warning_count++;
    } else if (strcmp(level, "INFO") == 0) {
        analyzer->stats.info_count++;
    } else if (strcmp(level, "DEBUG") == 0) {
        analyzer->stats.debug_count++;
    }
}

void calculate_frequent_level(struct LogAnalyzer *analyzer) {
    uint32_t counts[4] = {0};
    const char *levels[] = {"ERROR", "WARNING", "INFO", "DEBUG"};
    
    counts[0] = analyzer->stats.error_count;
    counts[1] = analyzer->stats.warning_count;
    counts[2] = analyzer->stats.info_count;
    counts[3] = analyzer->stats.debug_count;
    
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

int process_log_file(struct LogAnalyzer *analyzer, const char *filename) {
    if (analyzer == NULL || !validate_filename(filename)) {
        return 0;
    }
    
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return 0;
    }
    
    char line[MAX_LINE_LENGTH];
    uint32_t line_num = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && analyzer->entry_count < MAX_ENTRIES) {
        line_num++;
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) {
            continue;
        }
        
        struct Log