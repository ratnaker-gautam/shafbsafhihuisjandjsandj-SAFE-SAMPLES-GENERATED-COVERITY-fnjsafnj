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

int parse_level(const char *str) {
    if (strncmp(str, "ERROR", 5) == 0) return LEVEL_ERROR;
    if (strncmp(str, "WARN", 4) == 0) return LEVEL_WARN;
    if (strncmp(str, "INFO", 4) == 0) return LEVEL_INFO;
    if (strncmp(str, "DEBUG", 5) == 0) return LEVEL_DEBUG;
    return -1;
}

int parse_log_line(const char *line, struct log_entry *entry) {
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    
    char temp[MAX_LINE_LEN];
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
    if (!parse_timestamp(temp, entry->timestamp)) return 0;
    
    const char *level_start = temp + TIMESTAMP_LEN;
    if (*level_start != ' ') return 0;
    level_start++;
    
    int level = parse_level(level_start);
    if (!IS_VALID_LEVEL(level)) return 0;
    entry->level = level;
    
    const char *message_start = level_start;
    while (*message_start && *message_start != ' ') message_start++;
    if (*message_start != ' ') return 0;
    message_start++;
    
    if (strlen(message_start) >= sizeof(entry->message)) return 0;
    strncpy(entry->message, message_start, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    char *newline = strchr(entry->message, '\n');
    if (newline) *newline = '\0';
    
    return 1;
}

void print_summary(int counts[LOG_LEVELS]) {
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
    
    printf("Enter log entries (empty line to finish):\n");
    
    char line[MAX_LINE_LEN];
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin)) {
        if (line[0] == '\n') break;
        
        if (parse_log_line(line, &entries[entry_count])) {
            level_counts[entries[entry_count].level]++;
            entry_count++;
        } else {
            printf("Invalid log format: %s", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    printf("\nParsed %d log entries:\n", entry_count);
    for (int i = 0; i < entry_count; i++) {
        const char *level_names[] = {"ERROR", "WARN", "INFO", "DEBUG"};
        printf("[%s] %s: %s\n", entries[i].timestamp, level_names[entries[i].level], entries[i].message);
    }
    
    printf("\n");
    print_summary(level_counts);
    
    return 0;
}