//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LOG_LEVELS 4

#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_WARN 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_DEBUG 3

#define IS_VALID_LEVEL(c) ((c) == 'E' || (c) == 'W' || (c) == 'I' || (c) == 'D')
#define LEVEL_TO_INDEX(c) ((c) == 'E' ? 0 : (c) == 'W' ? 1 : (c) == 'I' ? 2 : 3)

struct log_entry {
    char timestamp[TIMESTAMP_LEN];
    char level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 4];
};

int parse_timestamp(const char *str, char *timestamp) {
    if (strlen(str) < TIMESTAMP_LEN - 1) return 0;
    for (int i = 0; i < TIMESTAMP_LEN - 1; i++) {
        if (i == 4 || i == 7) {
            if (str[i] != '-') return 0;
        } else if (i == 10) {
            if (str[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (str[i] != ':') return 0;
        } else {
            if (!isdigit(str[i])) return 0;
        }
        timestamp[i] = str[i];
    }
    timestamp[TIMESTAMP_LEN - 1] = '\0';
    return 1;
}

int parse_log_line(const char *line, struct log_entry *entry) {
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    
    char temp[MAX_LINE_LEN];
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
    char *timestamp_part = strtok(temp, " ");
    if (!timestamp_part) return 0;
    
    if (!parse_timestamp(timestamp_part, entry->timestamp)) return 0;
    
    char *level_part = strtok(NULL, " ");
    if (!level_part || strlen(level_part) != 1) return 0;
    
    if (!IS_VALID_LEVEL(level_part[0])) return 0;
    entry->level = level_part[0];
    
    char *message_part = strtok(NULL, "\n");
    if (!message_part) return 0;
    
    strncpy(entry->message, message_part, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void analyze_logs(struct log_entry *entries, int count) {
    int level_counts[LOG_LEVELS] = {0};
    int total_messages = 0;
    
    for (int i = 0; i < count; i++) {
        int level_idx = LEVEL_TO_INDEX(entries[i].level);
        level_counts[level_idx]++;
        total_messages++;
    }
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", total_messages);
    printf("Error: %d (%.1f%%)\n", level_counts[0], total_messages > 0 ? (level_counts[0] * 100.0) / total_messages : 0.0);
    printf("Warning: %d (%.1f%%)\n", level_counts[1], total_messages > 0 ? (level_counts[1] * 100.0) / total_messages : 0.0);
    printf("Info: %d (%.1f%%)\n", level_counts[2], total_messages > 0 ? (level_counts[2] * 100.0) / total_messages : 0.0);
    printf("Debug: %d (%.1f%%)\n", level_counts[3], total_messages > 0 ? (level_counts[3] * 100.0) / total_messages : 0.0);
}

int main(void) {
    struct log_entry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS L message):\n");
    printf("L = E(Error), W(Warning), I(Info), D(Debug)\n");
    printf("Enter empty line to finish.\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(line, sizeof(line), stdin)) break;
        
        if (line[0] == '\n' || line[0] == '\0') break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) continue;
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (entry_count > 0) {
        analyze_logs(entries, entry_count);
        
        printf("\nRecent entries