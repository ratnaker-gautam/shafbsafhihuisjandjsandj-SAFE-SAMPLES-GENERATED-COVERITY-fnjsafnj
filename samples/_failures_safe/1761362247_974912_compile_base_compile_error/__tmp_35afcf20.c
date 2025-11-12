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

struct LogFile {
    char filename[MAX_FILENAME];
    struct LogEntry entries[MAX_ENTRIES];
    uint32_t entry_count;
    struct LogStats stats;
};

int validate_filename(const char* filename) {
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

int parse_log_line(const char* line, struct LogEntry* entry, uint32_t line_num) {
    if (line == NULL || entry == NULL) {
        return 0;
    }
    
    char level[16] = {0};
    char timestamp[32] = {0};
    char message[256] = {0};
    
    int result = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    if (result < 2) {
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
    entry->line_number = line_num;
    
    return 1;
}

void initialize_stats(struct LogStats* stats) {
    if (stats == NULL) return;
    stats->total_entries = 0;
    stats->error_count = 0;
    stats->warning_count = 0;
    stats->info_count = 0;
    stats->debug_count = 0;
    strcpy(stats->most_frequent_level, "UNKNOWN");
    stats->max_level_count = 0;
}

void update_stats(struct LogStats* stats, const char* level) {
    if (stats == NULL || level == NULL) return;
    
    stats->total_entries++;
    
    if (strcmp(level, "ERROR") == 0) {
        stats->error_count++;
        if (stats->error_count > stats->max_level_count) {
            stats->max_level_count = stats->error_count;
            strcpy(stats->most_frequent_level, "ERROR");
        }
    } else if (strcmp(level, "WARNING") == 0) {
        stats->warning_count++;
        if (stats->warning_count > stats->max_level_count) {
            stats->max_level_count = stats->warning_count;
            strcpy(stats->most_frequent_level, "WARNING");
        }
    } else if (strcmp(level, "INFO") == 0) {
        stats->info_count++;
        if (stats->info_count > stats->max_level_count) {
            stats->max_level_count = stats->info_count;
            strcpy(stats->most_frequent_level, "INFO");
        }
    } else if (strcmp(level, "DEBUG") == 0) {
        stats->debug_count++;
        if (stats->debug_count > stats->max_level_count) {
            stats->max_level_count = stats->debug_count;
            strcpy(stats->most_frequent_level, "DEBUG");
        }
    }
}

int process_log_file(struct LogFile* log_file) {
    if (log_file == NULL) {
        return 0;
    }
    
    FILE* file = fopen(log_file->filename, "r");
    if (file == NULL) {
        return 0;
    }
    
    char line[MAX_LINE_LENGTH];
    uint32_t line_count = 0;
    log_file->entry_count = 0;
    initialize_stats(&log_file->stats);
    
    while (fgets(line, sizeof(line), file) != NULL && log_file->entry_count < MAX_ENTRIES) {
        line_count++;
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry, line_count)) {
            log_file->entries[log_file->entry_count] = entry;
            update_stats(&log_file->stats, entry.level);
            log_file->entry_count++;
        }
    }
    
    fclose(file);
    return 1;
}

void display_statistics(const