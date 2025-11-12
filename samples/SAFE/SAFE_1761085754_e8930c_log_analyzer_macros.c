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

int parse_timestamp(const char *str, struct tm *tm) {
    return sscanf(str, "%4d-%2d-%2d %2d:%2d:%2d",
                  &tm->tm_year, &tm->tm_mon, &tm->tm_mday,
                  &tm->tm_hour, &tm->tm_min, &tm->tm_sec) == 6;
}

int validate_log_line(const char *line) {
    if (strlen(line) >= MAX_LINE_LEN - 1) return 0;
    if (strlen(line) < TIMESTAMP_LEN + 5) return 0;
    
    struct tm tm;
    if (!parse_timestamp(line, &tm)) return 0;
    
    if (line[TIMESTAMP_LEN] != ' ' || line[TIMESTAMP_LEN + 1] != '[') return 0;
    if (!IS_VALID_LEVEL(line[TIMESTAMP_LEN + 2])) return 0;
    if (line[TIMESTAMP_LEN + 3] != ']' || line[TIMESTAMP_LEN + 4] != ' ') return 0;
    
    return 1;
}

int parse_log_line(const char *line, struct log_entry *entry) {
    if (!validate_log_line(line)) return 0;
    
    strncpy(entry->timestamp, line, TIMESTAMP_LEN - 1);
    entry->timestamp[TIMESTAMP_LEN - 1] = '\0';
    
    entry->level = line[TIMESTAMP_LEN + 2];
    
    const char *msg_start = line + TIMESTAMP_LEN + 5;
    size_t msg_len = strlen(msg_start);
    if (msg_len >= sizeof(entry->message)) return 0;
    
    strncpy(entry->message, msg_start, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

int main(void) {
    struct log_entry entries[MAX_ENTRIES];
    int entry_count = 0;
    int level_counts[LOG_LEVELS] = {0};
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS [L] message):\n");
    printf("Press Ctrl+D (Unix) or Ctrl+Z (Windows) to finish.\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin)) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strlen(line) == 0) continue;
        
        if (parse_log_line(line, &entries[entry_count])) {
            int level_idx = LEVEL_TO_INDEX(entries[entry_count].level);
            level_counts[level_idx]++;
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (ferror(stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    printf("\nLog Analysis Summary:\n");
    printf("Total entries: %d\n", entry_count);
    printf("Error: %d\n", level_counts[LOG_LEVEL_ERROR]);
    printf("Warning: %d\n", level_counts[LOG_LEVEL_WARN]);
    printf("Info: %d\n", level_counts[LOG_LEVEL_INFO]);
    printf("Debug: %d\n", level_counts[LOG_LEVEL_DEBUG]);
    
    if (entry_count > 0) {
        printf("\nLast 5 entries:\n");
        int start = entry_count > 5 ? entry_count - 5 : 0;
        for (int i = start; i < entry_count; i++) {
            printf("%s [%c] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }
    
    return 0;
}