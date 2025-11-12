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

int parse_timestamp(const char* str, char* timestamp) {
    if (!str || !timestamp) return 0;
    if (strlen(str) < TIMESTAMP_LEN - 1) return 0;
    for (int i = 0; i < TIMESTAMP_LEN - 1; i++) {
        if (i == 4 || i == 7) {
            if (str[i] != '-') return 0;
        } else if (i == 10) {
            if (str[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (str[i] != ':') return 0;
        } else {
            if (!isdigit((unsigned char)str[i])) return 0;
        }
        timestamp[i] = str[i];
    }
    timestamp[TIMESTAMP_LEN - 1] = '\0';
    return 1;
}

int parse_level(const char* str) {
    if (!str) return -1;
    if (strncmp(str, "ERROR", 5) == 0) return LEVEL_ERROR;
    if (strncmp(str, "WARN", 4) == 0) return LEVEL_WARN;
    if (strncmp(str, "INFO", 4) == 0) return LEVEL_INFO;
    if (strncmp(str, "DEBUG", 5) == 0) return LEVEL_DEBUG;
    return -1;
}

int read_log_entries(struct log_entry* entries, int max_entries) {
    if (!entries || max_entries <= 0) return 0;
    int count = 0;
    char line[MAX_LINE_LEN];
    
    while (count < max_entries && fgets(line, sizeof(line), stdin) != NULL) {
        if (strlen(line) == 0 || line[0] == '\n') continue;
        
        struct log_entry* entry = &entries[count];
        
        if (!parse_timestamp(line, entry->timestamp)) {
            continue;
        }
        
        const char* level_start = line + TIMESTAMP_LEN - 1;
        if (*level_start != ' ') continue;
        level_start++;
        
        int level = parse_level(level_start);
        if (!IS_VALID_LEVEL(level)) {
            continue;
        }
        entry->level = level;
        
        const char* msg_start = level_start;
        while (*msg_start && *msg_start != ' ') msg_start++;
        while (*msg_start && *msg_start == ' ') msg_start++;
        
        if (strlen(msg_start) > 0) {
            size_t msg_len = strlen(msg_start);
            if (msg_len >= sizeof(entry->message)) {
                msg_len = sizeof(entry->message) - 1;
            }
            strncpy(entry->message, msg_start, msg_len);
            entry->message[msg_len] = '\0';
            
            char* newline = strchr(entry->message, '\n');
            if (newline) *newline = '\0';
        } else {
            entry->message[0] = '\0';
        }
        
        count++;
    }
    
    if (ferror(stdin)) {
        return -1;
    }
    
    return count;
}

void analyze_logs(struct log_entry* entries, int count) {
    if (!entries || count <= 0) return;
    int level_counts[LOG_LEVELS] = {0};
    const char* level_names[] = {"ERROR", "WARN", "INFO", "DEBUG"};
    
    for (int i = 0; i < count; i++) {
        if (IS_VALID_LEVEL(entries[i].level)) {
            level_counts[entries[i].level]++;
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("====================\n");
    printf("Total entries: %d\n", count);
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d (%.1f%%)\n", level_names[i], level_counts[i],
               count > 0 ? (level_counts[i] * 100.0 / count) : 0.0);
    }
    
    if (count > 0) {
        printf("\nRecent entries:\n");
        int show_count = count > 5 ? 5 : count;
        for (int i = count - show_count; i < count; i++) {
            printf("[%s] %s: %s\n", entries[i].timestamp, 
                   level_names[entries[i].level], entries[i].message);
        }
    }
}

int main(void) {
    struct log_entry entries[MAX_ENTRIES];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Supported levels: ERROR, W