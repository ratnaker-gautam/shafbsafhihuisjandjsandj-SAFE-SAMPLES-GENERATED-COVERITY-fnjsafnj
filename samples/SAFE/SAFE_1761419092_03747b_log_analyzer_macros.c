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

#define IS_VALID_LEVEL(l) ((l) >= 0 && (l) < LOG_LEVELS)
#define LEVEL_NAME(l) (level_names[(l)])
#define IS_TIMESTAMP_CHAR(c) (isdigit((c)) || (c) == '-' || (c) == ':' || (c) == ' ')

static const char *level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR"};

typedef struct {
    char timestamp[TIMESTAMP_LEN + 1];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
} LogEntry;

int parse_timestamp(const char *str, char *timestamp) {
    if (!str || !timestamp) return 0;
    
    int i = 0;
    while (i < TIMESTAMP_LEN && str[i] != '\0' && IS_TIMESTAMP_CHAR(str[i])) {
        timestamp[i] = str[i];
        i++;
    }
    timestamp[i] = '\0';
    
    return (i > 0);
}

int parse_level(const char *str) {
    if (!str) return -1;
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strstr(str, LEVEL_NAME(i)) != NULL) {
            return i;
        }
    }
    return -1;
}

int read_log_entries(LogEntry entries[], int max_entries) {
    if (!entries || max_entries <= 0) return 0;
    
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (count < max_entries && fgets(line, sizeof(line), stdin) != NULL) {
        if (strlen(line) == 0) continue;
        
        LogEntry *entry = &entries[count];
        
        if (!parse_timestamp(line, entry->timestamp)) {
            continue;
        }
        
        entry->level = parse_level(line);
        if (!IS_VALID_LEVEL(entry->level)) {
            continue;
        }
        
        const char *level_pos = strstr(line, LEVEL_NAME(entry->level));
        if (!level_pos) continue;
        
        const char *msg_start = level_pos + strlen(LEVEL_NAME(entry->level));
        while (*msg_start == ' ' || *msg_start == ':') msg_start++;
        
        size_t msg_len = strlen(msg_start);
        if (msg_len > 0 && msg_start[msg_len - 1] == '\n') {
            msg_len--;
        }
        
        if (msg_len >= sizeof(entry->message)) {
            msg_len = sizeof(entry->message) - 1;
        }
        
        strncpy(entry->message, msg_start, msg_len);
        entry->message[msg_len] = '\0';
        
        count++;
    }
    
    return count;
}

void analyze_logs(LogEntry entries[], int count) {
    if (!entries || count <= 0) return;
    
    int level_counts[LOG_LEVELS] = {0};
    
    for (int i = 0; i < count; i++) {
        if (IS_VALID_LEVEL(entries[i].level)) {
            level_counts[entries[i].level]++;
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("====================\n");
    printf("Total entries processed: %d\n", count);
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%-7s: %d entries (%.1f%%)\n", 
               LEVEL_NAME(i), 
               level_counts[i],
               count > 0 ? (level_counts[i] * 100.0 / count) : 0.0);
    }
    
    if (count > 0) {
        printf("\nRecent entries:\n");
        int show_count = count > 5 ? 5 : count;
        for (int i = count - show_count; i < count; i++) {
            printf("[%s] %s: %s\n", 
                   entries[i].timestamp,
                   LEVEL_NAME(entries[i].level),
                   entries[i].message);
        }
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log entries (timestamp, level, message):\n");
    printf("Format: YYYY-MM-DD HH:MM:SS LEVEL: message\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR\n");
    printf("Press Ctrl+D to finish input\n\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    if (count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    analyze_logs(entries, count);
    
    return 0;
}