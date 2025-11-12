//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

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
    char most_frequent_level[16];
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    uint32_t entry_count;
};

void init_analyzer(struct LogAnalyzer *analyzer) {
    memset(analyzer, 0, sizeof(struct LogAnalyzer));
    strcpy(analyzer->stats.most_frequent_level, "UNKNOWN");
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char *timestamp = strtok(temp_line, " ");
    char *level = strtok(NULL, " ");
    char *message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp) ||
        strlen(level) >= sizeof(entry->level) ||
        strlen(message) >= sizeof(entry->message)) {
        return 0;
    }
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer, const struct LogEntry *entry) {
    if (!analyzer || !entry) return;
    
    analyzer->stats.total_entries++;
    
    size_t msg_len = strlen(entry->message);
    if (msg_len > analyzer->stats.max_message_length) {
        analyzer->stats.max_message_length = msg_len;
    }
    
    if (strcmp(entry->level, "ERROR") == 0) {
        analyzer->stats.error_count++;
    } else if (strcmp(entry->level, "WARNING") == 0) {
        analyzer->stats.warning_count++;
    } else if (strcmp(entry->level, "INFO") == 0) {
        analyzer->stats.info_count++;
    }
}

void calculate_final_stats(struct LogAnalyzer *analyzer) {
    if (!analyzer) return;
    
    uint32_t max_count = analyzer->stats.info_count;
    strcpy(analyzer->stats.most_frequent_level, "INFO");
    
    if (analyzer->stats.warning_count > max_count) {
        max_count = analyzer->stats.warning_count;
        strcpy(analyzer->stats.most_frequent_level, "WARNING");
    }
    
    if (analyzer->stats.error_count > max_count) {
        strcpy(analyzer->stats.most_frequent_level, "ERROR");
    }
}

int read_log_file(struct LogAnalyzer *analyzer) {
    char filename[256];
    printf("Enter log file name: ");
    
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        return 0;
    }
    
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file '%s'\n", filename);
        return 0;
    }
    
    char line[MAX_LINE_LENGTH];
    uint32_t line_num = 0;
    
    while (fgets(line, sizeof(line), file) && analyzer->entry_count < MAX_ENTRIES) {
        line_num++;
        
        if (strlen(line) > 0 && line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }
        
        struct LogEntry entry;
        entry.line_number = line_num;
        
        if (parse_log_line(line, &entry)) {
            analyzer->entries[analyzer->entry_count] = entry;
            update_stats(analyzer, &entry);
            analyzer->entry_count++;
        }
    }
    
    fclose(file);
    calculate_final_stats(analyzer);
    return 1;
}

void display_stats(const struct LogAnalyzer *analyzer) {
    if (!analyzer) return;
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %u\n", analyzer->stats.total_entries);
    printf("INFO entries: %u\n", analyzer->stats.info_count);
    printf("WARNING entries: %u\n", analyzer->stats.warning_count);
    printf("ERROR entries: %u\n", analyzer->stats.error_count);
    printf("Most frequent level: %s\n", analyzer->stats.most_frequent_level);
    printf("Longest message: %u characters\n", analyzer->stats.max_message_length);
}

void display_entries_by_level(const struct LogAnalyzer *