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

#define CHECK_NULL(ptr) if ((ptr) == NULL) return EXIT_FAILURE
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) return EXIT_FAILURE

struct log_entry {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
};

int parse_timestamp(const char* str, char* timestamp) {
    if (str == NULL || timestamp == NULL) return -1;
    size_t len = strlen(str);
    if (len < TIMESTAMP_LEN - 1) return -1;
    
    for (int i = 0; i < TIMESTAMP_LEN - 1; i++) {
        char c = str[i];
        if (i == 4 || i == 7) {
            if (c != '-') return -1;
        } else if (i == 10) {
            if (c != ' ') return -1;
        } else if (i == 13 || i == 16) {
            if (c != ':') return -1;
        } else {
            if (!isdigit(c)) return -1;
        }
        timestamp[i] = c;
    }
    timestamp[TIMESTAMP_LEN - 1] = '\0';
    return 0;
}

int parse_level(const char* str) {
    if (str == NULL) return -1;
    
    if (strncmp(str, "ERROR", 5) == 0) return LEVEL_ERROR;
    if (strncmp(str, "WARN", 4) == 0) return LEVEL_WARN;
    if (strncmp(str, "INFO", 4) == 0) return LEVEL_INFO;
    if (strncmp(str, "DEBUG", 5) == 0) return LEVEL_DEBUG;
    
    return -1;
}

int read_log_entries(struct log_entry* entries, int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
        if (strlen(line) == 0) continue;
        
        char* timestamp_start = line;
        char* level_start = strchr(line, ' ');
        if (level_start == NULL) continue;
        *level_start = '\0';
        level_start++;
        
        char* message_start = strchr(level_start, ' ');
        if (message_start == NULL) continue;
        *message_start = '\0';
        message_start++;
        
        if (parse_timestamp(timestamp_start, entries[count].timestamp) != 0) continue;
        
        int level = parse_level(level_start);
        if (level == -1) continue;
        entries[count].level = level;
        
        size_t msg_len = strlen(message_start);
        if (msg_len > 0 && message_start[msg_len - 1] == '\n') {
            message_start[msg_len - 1] = '\0';
        }
        
        strncpy(entries[count].message, message_start, sizeof(entries[count].message) - 1);
        entries[count].message[sizeof(entries[count].message) - 1] = '\0';
        
        count++;
    }
    
    return count;
}

void print_summary(const struct log_entry* entries, int count) {
    int level_counts[LOG_LEVELS] = {0};
    const char* level_names[] = {"ERROR", "WARN", "INFO", "DEBUG"};
    
    for (int i = 0; i < count; i++) {
        if (entries[i].level >= LOG_LEVELS || entries[i].level < 0) continue;
        level_counts[entries[i].level]++;
    }
    
    printf("Log Summary:\n");
    printf("Total entries: %d\n", count);
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d\n", level_names[i], level_counts[i]);
    }
}

int main(void) {
    struct log_entry entries[MAX_ENTRIES];
    
    printf("Enter log entries (timestamp level message):\n");
    printf("Format: YYYY-MM-DD HH:MM:SS LEVEL message\\n\n");
    printf("Press Ctrl+D to finish input.\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    if (count == 0) {
        printf("No valid log entries found.\n");
        return EXIT_FAILURE;
    }
    
    print_summary(entries, count);
    
    printf("\nRecent entries:\n");
    int display_count = (count < 5) ? count : 5;
    const char* level_names[] = {"ERROR", "WARN", "INFO", "DEBUG"};
    for (int i = count - display_count; i < count; i++) {
        if (entries[i].level >= LOG_LEVELS || entries[i].level < 0) continue;
        printf("%s [%s] %s\n", entries[i].timestamp, level_names[entries[i].level