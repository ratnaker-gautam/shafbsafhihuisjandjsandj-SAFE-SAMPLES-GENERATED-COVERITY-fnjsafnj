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

#define IS_VALID_LEVEL(l) ((l) >= LOG_LEVEL_ERROR && (l) <= LOG_LEVEL_DEBUG)

struct log_entry {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
};

int parse_log_level(const char* str) {
    if (strncmp(str, "ERROR", 5) == 0) return LOG_LEVEL_ERROR;
    if (strncmp(str, "WARN", 4) == 0) return LOG_LEVEL_WARN;
    if (strncmp(str, "INFO", 4) == 0) return LOG_LEVEL_INFO;
    if (strncmp(str, "DEBUG", 5) == 0) return LOG_LEVEL_DEBUG;
    return -1;
}

const char* level_to_string(int level) {
    switch (level) {
        case LOG_LEVEL_ERROR: return "ERROR";
        case LOG_LEVEL_WARN: return "WARN";
        case LOG_LEVEL_INFO: return "INFO";
        case LOG_LEVEL_DEBUG: return "DEBUG";
        default: return "UNKNOWN";
    }
}

int parse_timestamp(const char* str, char* timestamp) {
    if (strlen(str) < TIMESTAMP_LEN - 1) return 0;
    
    for (int i = 0; i < TIMESTAMP_LEN - 1; i++) {
        char c = str[i];
        if (i == 4 || i == 7) {
            if (c != '-') return 0;
        } else if (i == 10) {
            if (c != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (c != ':') return 0;
        } else {
            if (!isdigit(c)) return 0;
        }
        timestamp[i] = c;
    }
    timestamp[TIMESTAMP_LEN - 1] = '\0';
    return 1;
}

int read_log_file(const char* filename, struct log_entry* entries, int max_entries) {
    FILE* file = fopen(filename, "r");
    if (!file) return -1;
    
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) && count < max_entries) {
        if (strlen(line) == 0) continue;
        
        char timestamp[TIMESTAMP_LEN];
        if (!parse_timestamp(line, timestamp)) continue;
        
        const char* level_start = line + TIMESTAMP_LEN;
        while (*level_start == ' ') level_start++;
        
        int level = parse_log_level(level_start);
        if (!IS_VALID_LEVEL(level)) continue;
        
        const char* message_start = level_start;
        while (*message_start && *message_start != ' ') message_start++;
        while (*message_start == ' ') message_start++;
        
        strncpy(entries[count].timestamp, timestamp, TIMESTAMP_LEN - 1);
        entries[count].timestamp[TIMESTAMP_LEN - 1] = '\0';
        entries[count].level = level;
        
        size_t msg_len = strlen(message_start);
        if (msg_len > 0 && message_start[msg_len - 1] == '\n') {
            msg_len--;
        }
        if (msg_len >= sizeof(entries[count].message)) {
            msg_len = sizeof(entries[count].message) - 1;
        }
        strncpy(entries[count].message, message_start, msg_len);
        entries[count].message[msg_len] = '\0';
        
        count++;
    }
    
    fclose(file);
    return count;
}

void generate_summary(struct log_entry* entries, int count) {
    int level_counts[LOG_LEVELS] = {0};
    
    for (int i = 0; i < count; i++) {
        if (IS_VALID_LEVEL(entries[i].level)) {
            level_counts[entries[i].level]++;
        }
    }
    
    printf("Log Summary:\n");
    printf("Total entries: %d\n", count);
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d\n", level_to_string(i), level_counts[i]);
    }
    printf("\n");
}

void show_entries_by_level(struct log_entry* entries, int count, int level) {
    if (!IS_VALID_LEVEL(level)) return;
    
    printf("%s entries:\n", level_to_string(level));
    int found = 0;
    
    for (int i = 0; i < count; i++) {
        if (entries[i].level == level) {
            printf("[%s] %s\n", entries[i].timestamp, entries[i].message);
            found++;
            if (found >= 10) break;
        }
    }
    
    if (found == 0) {
        printf("No entries found.\n");
    }
    printf("\n");
}

int main(void) {
    struct