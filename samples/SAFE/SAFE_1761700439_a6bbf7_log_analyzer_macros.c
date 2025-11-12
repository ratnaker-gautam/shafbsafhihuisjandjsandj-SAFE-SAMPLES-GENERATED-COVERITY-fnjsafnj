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
#define LEVEL_STR(l) (level_strings[(l)])
#define IS_TIMESTAMP_CHAR(c) (isdigit((c)) || (c) == '-' || (c) == ':' || (c) == ' ' || (c) == '.')

static const char *level_strings[] = {"DEBUG", "INFO", "WARNING", "ERROR"};

typedef struct {
    char timestamp[TIMESTAMP_LEN + 1];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
} LogEntry;

int parse_log_level(const char *str) {
    if (str == NULL) return -1;
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strncmp(str, LEVEL_STR(i), strlen(LEVEL_STR(i))) == 0) {
            return i;
        }
    }
    return -1;
}

int parse_timestamp(const char *line, char *timestamp) {
    if (line == NULL || timestamp == NULL) return 0;
    int pos = 0;
    while (line[pos] && IS_TIMESTAMP_CHAR(line[pos]) && pos < TIMESTAMP_LEN) {
        timestamp[pos] = line[pos];
        pos++;
    }
    timestamp[pos] = '\0';
    return (pos > 0);
}

int parse_log_entry(const char *line, LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    
    char temp[MAX_LINE_LEN];
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
    if (!parse_timestamp(temp, entry->timestamp)) return 0;
    
    char *level_start = temp + strlen(entry->timestamp);
    while (*level_start && *level_start == ' ') level_start++;
    
    char *level_end = level_start;
    while (*level_end && *level_end != ' ') level_end++;
    if (*level_end) *level_end = '\0';
    
    entry->level = parse_log_level(level_start);
    if (!IS_VALID_LEVEL(entry->level)) return 0;
    
    char *message_start = level_end + 1;
    while (*message_start && *message_start == ' ') message_start++;
    
    if (strlen(message_start) >= sizeof(entry->message)) return 0;
    strncpy(entry->message, message_start, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void analyze_logs(LogEntry *entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    int level_counts[LOG_LEVELS] = {0};
    
    for (int i = 0; i < count; i++) {
        if (IS_VALID_LEVEL(entries[i].level)) {
            level_counts[entries[i].level]++;
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("====================\n");
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%-8s: %d entries\n", LEVEL_STR(i), level_counts[i]);
    }
    printf("Total   : %d entries\n", count);
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin) != NULL) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        if (parse_log_entry(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log entry format: %s\n", line);
        }
    }
    
    if (entry_count > 0) {
        analyze_logs(entries, entry_count);
    } else {
        printf("No valid log entries to analyze.\n");
    }
    
    return 0;
}