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

int parse_timestamp(const char *str, struct tm *tm) {
    return sscanf(str, "%4d-%2d-%2d %2d:%2d:%2d",
                  &tm->tm_year, &tm->tm_mon, &tm->tm_mday,
                  &tm->tm_hour, &tm->tm_min, &tm->tm_sec) == 6;
}

int validate_timestamp(const struct tm *tm) {
    if (tm->tm_year < 2000 || tm->tm_year > 2100) return 0;
    if (tm->tm_mon < 1 || tm->tm_mon > 12) return 0;
    if (tm->tm_mday < 1 || tm->tm_mday > 31) return 0;
    if (tm->tm_hour < 0 || tm->tm_hour > 23) return 0;
    if (tm->tm_min < 0 || tm->tm_min > 59) return 0;
    if (tm->tm_sec < 0 || tm->tm_sec > 59) return 0;
    return 1;
}

int parse_log_line(const char *line, struct log_entry *entry) {
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    
    struct tm tm = {0};
    char level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 4];
    
    if (sscanf(line, "%19s [%c] %[^\n]", entry->timestamp, &level, message) != 3) {
        return 0;
    }
    
    if (!IS_VALID_LEVEL(level)) return 0;
    
    if (!parse_timestamp(entry->timestamp, &tm)) return 0;
    if (!validate_timestamp(&tm)) return 0;
    
    entry->level = level;
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void print_summary(const int counts[LOG_LEVELS]) {
    const char *level_names[] = {"ERROR", "WARN", "INFO", "DEBUG"};
    printf("Log Level Summary:\n");
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d\n", level_names[i], counts[i]);
    }
}

int main(void) {
    struct log_entry entries[MAX_ENTRIES];
    int entry_count = 0;
    int level_counts[LOG_LEVELS] = {0};
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS [LEVEL] message):\n");
    printf("Enter empty line to finish.\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin) != NULL) {
        if (line[0] == '\n' || line[0] == '\0') break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strlen(line) == 0) continue;
        
        if (parse_log_line(line, &entries[entry_count])) {
            int level_idx = LEVEL_TO_INDEX(entries[entry_count].level);
            level_counts[level_idx]++;
            entry_count++;
            printf("Added: %s\n", line);
        } else {
            printf("Invalid format: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    printf("\nAnalysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    print_summary(level_counts);
    
    printf("\nRecent entries:\n");
    int start = (entry_count > 5) ? entry_count - 5 : 0;
    for (int i = start; i < entry_count; i++) {
        printf("%s [%c] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
    
    return 0;
}