//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 256
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

int parse_log_level(const char *str) {
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strcmp(str, level_names[i]) == 0) {
            return i;
        }
    }
    return -1;
}

int parse_timestamp(const char *str, char *timestamp) {
    if (strlen(str) < TIMESTAMP_LEN) return 0;
    for (int i = 0; i < TIMESTAMP_LEN; i++) {
        if (!IS_TIMESTAMP_CHAR(str[i])) return 0;
    }
    strncpy(timestamp, str, TIMESTAMP_LEN);
    timestamp[TIMESTAMP_LEN] = '\0';
    return 1;
}

int read_log_entry(FILE *file, LogEntry *entry) {
    char line[MAX_LINE_LEN];
    if (fgets(line, sizeof(line), file) == NULL) return 0;
    
    line[strcspn(line, "\n")] = '\0';
    if (strlen(line) < TIMESTAMP_LEN + 6) return 0;
    
    if (!parse_timestamp(line, entry->timestamp)) return 0;
    
    const char *level_start = line + TIMESTAMP_LEN + 1;
    char level_str[10];
    int level_len = 0;
    while (level_start[level_len] && level_start[level_len] != ' ' && level_len < 9) {
        level_str[level_len] = level_start[level_len];
        level_len++;
    }
    level_str[level_len] = '\0';
    
    entry->level = parse_log_level(level_str);
    if (!IS_VALID_LEVEL(entry->level)) return 0;
    
    const char *msg_start = level_start + level_len + 1;
    if (strlen(msg_start) >= sizeof(entry->message)) return 0;
    strcpy(entry->message, msg_start);
    
    return 1;
}

void analyze_logs(LogEntry *entries, int count) {
    int level_counts[LOG_LEVELS] = {0};
    char earliest[TIMESTAMP_LEN + 1] = "";
    char latest[TIMESTAMP_LEN + 1] = "";
    
    for (int i = 0; i < count; i++) {
        if (IS_VALID_LEVEL(entries[i].level)) {
            level_counts[entries[i].level]++;
        }
        
        if (i == 0 || strcmp(entries[i].timestamp, earliest) < 0) {
            strcpy(earliest, entries[i].timestamp);
        }
        if (i == 0 || strcmp(entries[i].timestamp, latest) > 0) {
            strcpy(latest, entries[i].timestamp);
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    printf("Time range: %s to %s\n", earliest, latest);
    printf("Level distribution:\n");
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("  %s: %d\n", LEVEL_NAME(i), level_counts[i]);
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (timestamp level message):\n");
    printf("Format: YYYY-MM-DD HH:MM:SS LEVEL message\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR\n");
    printf("Enter empty line to finish.\n");
    
    char input[MAX_LINE_LEN];
    while (entry_count < MAX_ENTRIES) {
        printf("> ");
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        
        input[strcspn(input, "\n")] = '\0';
        if (strlen(input) == 0) break;
        
        FILE *temp = tmpfile();
        if (temp == NULL) {
            fprintf(stderr, "Error creating temporary file\n");
            return 1;
        }
        
        fputs(input, temp);
        rewind(temp);
        
        if (read_log_entry(temp, &entries[entry_count])) {
            entry_count++;
            printf("Added entry: %s %s %s\n", 
                   entries[entry_count-1].timestamp,
                   LEVEL_NAME(entries[entry_count-1].level),
                   entries[entry_count-1].message);
        } else {
            printf("Invalid log format. Please use: YYYY-MM-DD HH:MM:SS LEVEL message\n");
        }
        
        fclose