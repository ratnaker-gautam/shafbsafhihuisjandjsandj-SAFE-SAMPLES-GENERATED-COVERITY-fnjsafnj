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
    static const char* levels[] = {"ERROR", "WARN", "INFO", "DEBUG"};
    if (IS_VALID_LEVEL(level)) return levels[level];
    return "UNKNOWN";
}

int parse_timestamp(const char* str, char* output) {
    if (strlen(str) < TIMESTAMP_LEN - 1) return 0;
    for (int i = 0; i < TIMESTAMP_LEN - 1; i++) {
        if (str[i] == ' ') break;
        output[i] = str[i];
    }
    output[TIMESTAMP_LEN - 1] = '\0';
    return 1;
}

int read_log_entries(struct log_entry* entries, int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
        if (strlen(line) == 0 || line[0] == '\n') continue;
        
        char timestamp[TIMESTAMP_LEN];
        if (!parse_timestamp(line, timestamp)) continue;
        
        const char* level_start = strchr(line, ' ');
        if (level_start == NULL) continue;
        level_start++;
        
        int level = parse_log_level(level_start);
        if (level == -1) continue;
        
        const char* message_start = strchr(level_start, ' ');
        if (message_start == NULL) continue;
        message_start++;
        
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
    
    return count;
}

void analyze_logs(struct log_entry* entries, int count) {
    int level_counts[LOG_LEVELS] = {0};
    
    for (int i = 0; i < count; i++) {
        if (IS_VALID_LEVEL(entries[i].level)) {
            level_counts[entries[i].level]++;
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d\n", level_to_string(i), level_counts[i]);
    }
    
    if (count > 0) {
        printf("\nRecent entries:\n");
        int show_count = count > 5 ? 5 : count;
        for (int i = count - show_count; i < count; i++) {
            printf("[%s] %s: %s\n", entries[i].timestamp, 
                   level_to_string(entries[i].level), entries[i].message);
        }
    }
}

int main(void) {
    struct log_entry entries[MAX_ENTRIES];
    
    printf("Enter log entries (timestamp LEVEL message), Ctrl+D to end:\n");
    printf("Example: 2024-01-15T10:30:00 INFO User logged in\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    if (count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    analyze_logs(entries, count);
    
    return 0;
}