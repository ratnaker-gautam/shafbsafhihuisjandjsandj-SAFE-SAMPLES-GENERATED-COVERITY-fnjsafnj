//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
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

int main(void) {
    struct log_entry entries[MAX_ENTRIES];
    int entry_count = 0;
    int level_counts[LOG_LEVELS] = {0};
    
    printf("Enter log entries (one per line, format: YYYY-MM-DD HH:MM:SS L message):\n");
    printf("Press Ctrl+D (Linux) or Ctrl+Z (Windows) to finish.\n");
    
    char line[MAX_LINE_LEN];
    while (fgets(line, sizeof(line), stdin) != NULL && entry_count < MAX_ENTRIES) {
        if (strlen(line) == 0 || line[0] == '\n') continue;
        
        line[strcspn(line, "\n")] = '\0';
        
        struct log_entry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count] = entry;
            level_counts[LEVEL_TO_INDEX(entry.level)]++;
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    printf("\nLog Analysis Summary:\n");
    printf("Total entries: %d\n", entry_count);
    printf("Error level: %d\n", level_counts[LOG_LEVEL_ERROR]);
    printf("Warning level: %d\n", level_counts[LOG_LEVEL_WARN]);
    printf("Info level: %d\n", level_counts[LOG_LEVEL_INFO]);
    printf("Debug level: %d\n", level_counts[LOG_LEVEL_DEBUG]);
    
    printf("\nRecent entries (last 5):\n");
    int start = (entry_count > 5) ? entry_count - 5 : 0;
    for (int i = start; i < entry_count; i++) {
        printf("%s %c %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
    
    int error_percentage = (level_counts[LOG_LEVEL_ERROR] * 100) / entry_count;
    printf("\nError percentage: %d%%\n", error_percentage);
    
    if (error_percentage > 10) {
        printf("High error rate detected!\n");
    }
    
    return 0;
}