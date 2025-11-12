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

#define IS_VALID_LEVEL(l) ((l) >= 0 && (l) < LOG_LEVELS)
#define LEVEL_NAME(l) (level_names[(l)])
#define IS_TIMESTAMP_CHAR(c) (isdigit((c)) || (c) == '-' || (c) == ':' || (c) == ' ' || (c) == '.')

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
} LogEntry;

const char *level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR"};

int parse_log_level(const char *str) {
    if (!str) return -1;
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strncmp(str, level_names[i], strlen(level_names[i])) == 0) {
            return i;
        }
    }
    return -1;
}

int parse_timestamp(const char *str, char *timestamp) {
    if (!str || !timestamp) return 0;
    int i = 0;
    while (str[i] && i < TIMESTAMP_LEN - 1 && IS_TIMESTAMP_CHAR(str[i])) {
        timestamp[i] = str[i];
        i++;
    }
    timestamp[i] = '\0';
    return i > 0;
}

int read_log_entry(FILE *file, LogEntry *entry) {
    if (!file || !entry) return 0;
    char line[MAX_LINE_LEN];
    if (!fgets(line, sizeof(line), file)) return 0;
    
    size_t len = strlen(line);
    if (len > 0 && line[len-1] == '\n') line[len-1] = '\0';
    
    if (!parse_timestamp(line, entry->timestamp)) return 0;
    
    char *level_start = strchr(line, ']');
    if (!level_start || level_start - line >= MAX_LINE_LEN) return 0;
    level_start++;
    while (*level_start == ' ') level_start++;
    
    char *level_end = strchr(level_start, ' ');
    if (!level_end) return 0;
    
    char level_str[16];
    size_t level_len = level_end - level_start;
    if (level_len >= sizeof(level_str)) return 0;
    strncpy(level_str, level_start, level_len);
    level_str[level_len] = '\0';
    
    entry->level = parse_log_level(level_str);
    if (entry->level == -1) return 0;
    
    char *msg_start = level_end + 1;
    while (*msg_start == ' ') msg_start++;
    
    if (strlen(msg_start) >= sizeof(entry->message)) return 0;
    strcpy(entry->message, msg_start);
    
    return 1;
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int level_counts[LOG_LEVELS] = {0};
    
    printf("Log Analyzer - Enter log entries (format: [timestamp] LEVEL message)\\n");
    printf("Enter empty line to finish.\\n");
    
    while (entry_count < MAX_ENTRIES) {
        printf("Log entry %d: ", entry_count + 1);
        
        LogEntry entry;
        if (!read_log_entry(stdin, &entry)) {
            char buffer[MAX_LINE_LEN];
            if (!fgets(buffer, sizeof(buffer), stdin)) break;
            if (strlen(buffer) == 1 || (strlen(buffer) == 2 && buffer[0] == '\\n')) break;
            printf("Invalid log format. Use: [YYYY-MM-DD HH:MM:SS] LEVEL message\\n");
            continue;
        }
        
        entries[entry_count] = entry;
        if (IS_VALID_LEVEL(entry.level)) {
            level_counts[entry.level]++;
        }
        entry_count++;
    }
    
    printf("\\n=== Log Analysis Results ===\\n");
    printf("Total entries: %d\\n", entry_count);
    printf("\\nLevel distribution:\\n");
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%-8s: %d (%.1f%%)\\n", 
               LEVEL_NAME(i), 
               level_counts[i],
               entry_count > 0 ? (level_counts[i] * 100.0 / entry_count) : 0.0);
    }
    
    printf("\\nRecent entries:\\n");
    int start = entry_count > 5 ? entry_count - 5 : 0;
    for (int i = start; i < entry_count; i++) {
        printf("[%s] %-8s %s\\n", 
               entries[i].timestamp,
               LEVEL_NAME(entries[i].level),
               entries[i].message);
    }
    
    return 0;
}