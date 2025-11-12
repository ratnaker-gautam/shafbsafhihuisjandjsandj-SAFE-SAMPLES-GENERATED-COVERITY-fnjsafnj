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

#define INIT_STATS(s) do { \
    memset((s).counts, 0, sizeof((s).counts)); \
    (s).total = 0; \
    (s).first_timestamp[0] = '\0'; \
    (s).last_timestamp[0] = '\0'; \
} while(0)

#define PARSE_TIMESTAMP(dest, src) do { \
    if (strlen(src) >= TIMESTAMP_LEN - 1) { \
        strncpy(dest, src, TIMESTAMP_LEN - 1); \
        dest[TIMESTAMP_LEN - 1] = '\0'; \
    } else { \
        strcpy(dest, src); \
    } \
} while(0)

#define LEVEL_TO_STR(l) ( \
    (l) == LEVEL_ERROR ? "ERROR" : \
    (l) == LEVEL_WARN ? "WARN" : \
    (l) == LEVEL_INFO ? "INFO" : \
    (l) == LEVEL_DEBUG ? "DEBUG" : "UNKNOWN" \
)

int parse_log_level(const char* str) {
    if (str == NULL) return -1;
    if (strcasecmp(str, "ERROR") == 0) return LEVEL_ERROR;
    if (strcasecmp(str, "WARN") == 0) return LEVEL_WARN;
    if (strcasecmp(str, "INFO") == 0) return LEVEL_INFO;
    if (strcasecmp(str, "DEBUG") == 0) return LEVEL_DEBUG;
    return -1;
}

int parse_log_line(const char* line, struct log_entry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char level_str[16];
    int scanned = sscanf(line, "%19s %15s %1023[^\n]", 
                        entry->timestamp, level_str, entry->message);
    
    if (scanned != 3) return 0;
    
    entry->level = parse_log_level(level_str);
    if (!IS_VALID_LEVEL(entry->level)) return 0;
    
    return 1;
}

void update_stats(struct log_stats* stats, const struct log_entry* entry) {
    if (stats == NULL || entry == NULL) return;
    
    if (stats->total == 0) {
        PARSE_TIMESTAMP(stats->first_timestamp, entry->timestamp);
    }
    PARSE_TIMESTAMP(stats->last_timestamp, entry->timestamp);
    
    if (IS_VALID_LEVEL(entry->level)) {
        stats->counts[entry->level]++;
    }
    stats->total++;
}

void print_stats(const struct log_stats* stats) {
    if (stats == NULL) return;
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", stats->total);
    printf("Time range: %s to %s\n", stats->first_timestamp, stats->last_timestamp);
    printf("\nBreakdown by level:\n");
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        double percentage = stats->total > 0 ? 
            (100.0 * stats->counts[i]) / stats->total : 0.0;
        printf("%-6s: %d (%.1f%%)\n", 
               LEVEL_TO_STR(i), stats->counts[i], percentage);
    }
}

int main(void) {
    struct log_entry entries[MAX_ENTRIES];
    struct log_stats stats;
    INIT_STATS(stats);
    
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Supported levels: ERROR, WARN, INFO, DEBUG\n");
    printf("Enter empty line to finish.\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (line[0] == '\0') break;
        
        if (parse_log_line(line, &entries[entry_count])) {
            update_stats(&stats, &entries[entry_count]);
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    printf("\n");
    print_stats(&stats);
    return