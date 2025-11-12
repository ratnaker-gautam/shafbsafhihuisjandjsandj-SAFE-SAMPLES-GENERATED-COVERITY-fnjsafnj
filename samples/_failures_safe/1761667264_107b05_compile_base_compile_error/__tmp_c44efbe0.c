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

struct log_entry {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
};

static const char *level_names[] = {"ERROR", "WARN", "INFO", "DEBUG"};

int parse_log_level(const char *str) {
    if (str == NULL) return -1;
    if (strcmp(str, "ERROR") == 0) return LOG_LEVEL_ERROR;
    if (strcmp(str, "WARN") == 0) return LOG_LEVEL_WARN;
    if (strcmp(str, "INFO") == 0) return LOG_LEVEL_INFO;
    if (strcmp(str, "DEBUG") == 0) return LOG_LEVEL_DEBUG;
    return -1;
}

int parse_timestamp(const char *str, char *out) {
    if (str == NULL || out == NULL) return 0;
    if (strlen(str) < TIMESTAMP_LEN - 1) return 0;
    
    for (int i = 0; i < TIMESTAMP_LEN - 1; i++) {
        if (str[i] == ' ') break;
        out[i] = str[i];
    }
    out[TIMESTAMP_LEN - 1] = '\0';
    return 1;
}

int read_log_entries(struct log_entry *entries, int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
        if (strlen(line) == 0) continue;
        
        char timestamp[TIMESTAMP_LEN];
        char level_str[16];
        char message[MAX_LINE_LEN];
        
        if (parse_timestamp(line, timestamp) &&
            sscanf(line + TIMESTAMP_LEN, "%15s", level_str) == 1) {
            
            int level = parse_log_level(level_str);
            if (IS_VALID_LEVEL(level)) {
                const char *msg_start = strstr(line, level_str);
                if (msg_start != NULL) {
                    msg_start += strlen(level_str);
                    while (*msg_start == ' ') msg_start++;
                    
                    strncpy(entries[count].message, msg_start, 
                           sizeof(entries[count].message) - 1);
                    entries[count].message[sizeof(entries[count].message) - 1] = '\0';
                    
                    char *newline = strchr(entries[count].message, '\n');
                    if (newline != NULL) *newline = '\0';
                    
                    strcpy(entries[count].timestamp, timestamp);
                    entries[count].level = level;
                    count++;
                }
            }
        }
    }
    return count;
}

void generate_summary(const struct log_entry *entries, int count) {
    int level_counts[LOG_LEVELS] = {0};
    
    for (int i = 0; i < count; i++) {
        if (IS_VALID_LEVEL(entries[i].level)) {
            level_counts[entries[i].level]++;
        }
    }
    
    printf("Log Summary:\n");
    printf("Total entries: %d\n", count);
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d\n", level_names[i], level_counts[i]);
    }
}

void show_entries_by_level(const struct log_entry *entries, int count, int target_level) {
    if (!IS_VALID_LEVEL(target_level)) return;
    
    printf("\n%s entries:\n", level_names[target_level]);
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (entries[i].level == target_level) {
            printf("%s %s %s\n", entries[i].timestamp, level_names[target_level], entries[i].message);
            found++;
        }
    }
    if (found == 0) {
        printf("No %s entries found.\n", level_names[target_level]);
    }
}

int main(void) {
    struct log_entry entries[MAX_ENTRIES];
    
    printf("Enter log entries (timestamp LEVEL message), one per line.\n");
    printf("Example: 2024-01-15T10:30:00 INFO System started\n");
    printf("Press Ctrl+D (Unix) or Ctrl+Z (Windows) to finish input.\n\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    if (count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    generate_summary(entries, count);
    
    char input[16];
    while (1) {
        printf("\nEnter level to filter (ERROR/WARN/INFO/DEBUG) or 'quit': ");
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        
        size_t