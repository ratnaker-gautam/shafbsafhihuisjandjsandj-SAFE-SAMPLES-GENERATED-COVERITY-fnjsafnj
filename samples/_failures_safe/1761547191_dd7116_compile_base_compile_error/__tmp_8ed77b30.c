//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LOG_LEVELS 4

#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_WARN 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_DEBUG 3

#define IS_VALID_LEVEL(l) ((l) >= LOG_LEVEL_ERROR && (l) <= LOG_LEVEL_DEBUG)
#define LEVEL_TO_STR(l) (level_strings[(l)])
#define PARSE_TIMESTAMP(ts, buf) parse_timestamp(ts, buf)

static const char *level_strings[] = {"ERROR", "WARN", "INFO", "DEBUG"};

struct log_entry {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
};

int parse_timestamp(const char *str, char *buf) {
    if (!str || !buf) return 0;
    size_t len = strlen(str);
    if (len < TIMESTAMP_LEN - 1) return 0;
    
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
        buf[i] = str[i];
    }
    buf[TIMESTAMP_LEN - 1] = '\0';
    return 1;
}

int parse_level(const char *str) {
    if (!str) return -1;
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strcmp(str, level_strings[i]) == 0) {
            return i;
        }
    }
    return -1;
}

int read_log_entries(struct log_entry *entries, int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
        if (strlen(line) == 0 || line[0] == '\n') continue;
        
        char timestamp[TIMESTAMP_LEN];
        char level_str[10];
        char message[MAX_LINE_LEN];
        
        if (PARSE_TIMESTAMP(line, timestamp)) {
            const char *rest = line + TIMESTAMP_LEN;
            
            if (sscanf(rest, " %9s", level_str) != 1) continue;
            
            int level = parse_level(level_str);
            if (!IS_VALID_LEVEL(level)) continue;
            
            const char *msg_start = strchr(rest, ' ');
            if (!msg_start) continue;
            msg_start = strchr(msg_start + 1, ' ');
            if (!msg_start) continue;
            msg_start++;
            
            strncpy(entries[count].timestamp, timestamp, TIMESTAMP_LEN - 1);
            entries[count].timestamp[TIMESTAMP_LEN - 1] = '\0';
            entries[count].level = level;
            strncpy(entries[count].message, msg_start, sizeof(entries[count].message) - 1);
            entries[count].message[sizeof(entries[count].message) - 1] = '\0';
            
            char *newline = strchr(entries[count].message, '\n');
            if (newline) *newline = '\0';
            
            count++;
        }
    }
    return count;
}

void analyze_logs(struct log_entry *entries, int count) {
    int level_counts[LOG_LEVELS] = {0};
    char earliest[TIMESTAMP_LEN] = "";
    char latest[TIMESTAMP_LEN] = "";
    
    for (int i = 0; i < count; i++) {
        if (IS_VALID_LEVEL(entries[i].level)) {
            level_counts[entries[i].level]++;
        }
        
        if (i == 0) {
            strncpy(earliest, entries[i].timestamp, TIMESTAMP_LEN - 1);
            earliest[TIMESTAMP_LEN - 1] = '\0';
            strncpy(latest, entries[i].timestamp, TIMESTAMP_LEN - 1);
            latest[TIMESTAMP_LEN - 1] = '\0';
        } else {
            if (strcmp(entries[i].timestamp, earliest) < 0) {
                strncpy(earliest, entries[i].timestamp, TIMESTAMP_LEN - 1);
                earliest[TIMESTAMP_LEN - 1] = '\0';
            }
            if (strcmp(entries[i].timestamp, latest) > 0) {
                strncpy(latest, entries[i].timestamp, TIMESTAMP_LEN - 1);
                latest[TIMESTAMP_LEN - 1] = '\0';
            }
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    printf("Time range: %s to %s\n", earliest, latest);
    printf("Level distribution:\n");
    for (int