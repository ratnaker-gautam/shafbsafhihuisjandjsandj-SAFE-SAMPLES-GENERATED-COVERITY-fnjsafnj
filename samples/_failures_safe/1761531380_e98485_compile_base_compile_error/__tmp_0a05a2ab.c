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

#define LEVEL_ERROR 0
#define LEVEL_WARN 1
#define LEVEL_INFO 2
#define LEVEL_DEBUG 3

#define IS_VALID_LEVEL(l) ((l) >= LEVEL_ERROR && (l) <= LEVEL_DEBUG)

struct log_entry {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
};

struct log_stats {
    int counts[LOG_LEVELS];
    int total;
    char first_timestamp[TIMESTAMP_LEN];
    char last_timestamp[TIMESTAMP_LEN];
};

#define LEVEL_NAME(l) (\
    (l) == LEVEL_ERROR ? "ERROR" : \
    (l) == LEVEL_WARN ? "WARN" : \
    (l) == LEVEL_INFO ? "INFO" : \
    (l) == LEVEL_DEBUG ? "DEBUG" : "UNKNOWN"\
)

#define PARSE_TIMESTAMP(dest, src) do { \
    if (strlen(src) >= TIMESTAMP_LEN - 1) { \
        strncpy(dest, src, TIMESTAMP_LEN - 1); \
        dest[TIMESTAMP_LEN - 1] = '\0'; \
    } else { \
        strcpy(dest, src); \
    } \
} while(0)

#define PARSE_LEVEL(str) (\
    strcmp(str, "ERROR") == 0 ? LEVEL_ERROR : \
    strcmp(str, "WARN") == 0 ? LEVEL_WARN : \
    strcmp(str, "INFO") == 0 ? LEVEL_INFO : \
    strcmp(str, "DEBUG") == 0 ? LEVEL_DEBUG : -1\
)

int parse_log_line(const char* line, struct log_entry* entry) {
    char level_str[16];
    char timestamp[TIMESTAMP_LEN];
    
    if (strlen(line) >= MAX_LINE_LEN - 1) {
        return 0;
    }
    
    if (sscanf(line, "%19s %15s", timestamp, level_str) != 2) {
        return 0;
    }
    
    int level = PARSE_LEVEL(level_str);
    if (!IS_VALID_LEVEL(level)) {
        return 0;
    }
    
    const char* message_start = strchr(line, ']');
    if (!message_start || *(message_start + 1) != ' ') {
        return 0;
    }
    message_start += 2;
    
    PARSE_TIMESTAMP(entry->timestamp, timestamp);
    entry->level = level;
    
    size_t msg_len = strlen(message_start);
    if (msg_len >= sizeof(entry->message)) {
        strncpy(entry->message, message_start, sizeof(entry->message) - 1);
        entry->message[sizeof(entry->message) - 1] = '\0';
    } else {
        strcpy(entry->message, message_start);
    }
    
    return 1;
}

void init_stats(struct log_stats* stats) {
    memset(stats, 0, sizeof(*stats));
    for (int i = 0; i < LOG_LEVELS; i++) {
        stats->counts[i] = 0;
    }
    stats->total = 0;
    stats->first_timestamp[0] = '\0';
    stats->last_timestamp[0] = '\0';
}

void update_stats(struct log_stats* stats, const struct log_entry* entry) {
    if (stats->total == 0) {
        strcpy(stats->first_timestamp, entry->timestamp);
    }
    strcpy(stats->last_timestamp, entry->timestamp);
    
    if (IS_VALID_LEVEL(entry->level)) {
        stats->counts[entry->level]++;
    }
    stats->total++;
}

void print_stats(const struct log_stats* stats) {
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", stats->total);
    printf("Time range: %s to %s\n", stats->first_timestamp, stats->last_timestamp);
    printf("\nBreakdown by level:\n");
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        int count = stats->counts[i];
        double percentage = (stats->total > 0) ? (count * 100.0 / stats->total) : 0.0;
        printf("%-6s: %3d (%5.1f%%)\n", LEVEL_NAME(i), count, percentage);
    }
}

int main(void) {
    struct log_entry entries[MAX_ENTRIES];
    struct log_stats stats;
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    init_stats(&stats);
    
    printf("Enter log entries (format: [TIMESTAMP] LEVEL message)\n");
    printf("Example: [2024-01-15T10:30:00] INFO System started\n");
    printf("Enter empty line to finish.\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin)) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0