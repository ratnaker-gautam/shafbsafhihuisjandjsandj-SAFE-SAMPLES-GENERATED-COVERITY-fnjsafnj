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

struct log_stats {
    int counts[LOG_LEVELS];
    int total;
    time_t start_time;
    time_t end_time;
};

int parse_timestamp(const char *str, struct tm *tm) {
    return sscanf(str, "%4d-%2d-%2d %2d:%2d:%2d",
                  &tm->tm_year, &tm->tm_mon, &tm->tm_mday,
                  &tm->tm_hour, &tm->tm_min, &tm->tm_sec) == 6;
}

int validate_log_line(const char *line) {
    size_t len = strlen(line);
    if (len >= MAX_LINE_LEN - 1) return 0;
    if (len < TIMESTAMP_LEN + 3) return 0;
    
    struct tm tm = {0};
    if (!parse_timestamp(line, &tm)) return 0;
    
    if (tm.tm_year < 2000 || tm.tm_year > 2100) return 0;
    if (tm.tm_mon < 1 || tm.tm_mon > 12) return 0;
    if (tm.tm_mday < 1 || tm.tm_mday > 31) return 0;
    if (tm.tm_hour < 0 || tm.tm_hour > 23) return 0;
    if (tm.tm_min < 0 || tm.tm_min > 59) return 0;
    if (tm.tm_sec < 0 || tm.tm_sec > 59) return 0;
    
    if (line[TIMESTAMP_LEN] != ' ') return 0;
    if (line[TIMESTAMP_LEN + 1] != '[') return 0;
    if (!IS_VALID_LEVEL(line[TIMESTAMP_LEN + 2])) return 0;
    if (line[TIMESTAMP_LEN + 3] != ']') return 0;
    if (line[TIMESTAMP_LEN + 4] != ' ') return 0;
    
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

void init_stats(struct log_stats *stats) {
    memset(stats, 0, sizeof(*stats));
    stats->start_time = 0;
    stats->end_time = 0;
}

void update_stats(struct log_stats *stats, const struct log_entry *entry) {
    struct tm tm = {0};
    if (parse_timestamp(entry->timestamp, &tm)) {
        tm.tm_year -= 1900;
        tm.tm_mon -= 1;
        time_t entry_time = mktime(&tm);
        
        if (stats->start_time == 0 || entry_time < stats->start_time) {
            stats->start_time = entry_time;
        }
        if (entry_time > stats->end_time) {
            stats->end_time = entry_time;
        }
    }
    
    int level_idx = LEVEL_TO_INDEX(entry->level);
    if (level_idx >= 0 && level_idx < LOG_LEVELS) {
        stats->counts[level_idx]++;
        stats->total++;
    }
}

void print_stats(const struct log_stats *stats) {
    const char *level_names[] = {"ERROR", "WARN", "INFO", "DEBUG"};
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", stats->total);
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (stats->counts[i] > 0) {
            double percentage = (double)stats->counts[i] / stats->total * 100.0;
            printf("%s: %d (%.1f%%)\n", level_names[i], stats->counts[i], percentage);
        }
    }