//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: log_analyzer
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
    char timestamp[TIMESTAMP_LEN + 1];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
};

static const char *level_names[] = {"ERROR", "WARN", "INFO", "DEBUG"};
static struct log_entry entries[MAX_ENTRIES];
static int entry_count = 0;

int parse_timestamp(const char *str) {
    struct tm tm = {0};
    return strptime(str, "%Y-%m-%d %H:%M:%S", &tm) != NULL;
}

int parse_level(const char *str) {
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strcmp(str, level_names[i]) == 0) {
            return i;
        }
    }
    return -1;
}

int parse_log_line(const char *line, struct log_entry *entry) {
    if (!line || !entry) return 0;
    
    char level_str[16];
    int scanned = sscanf(line, "%20s %15s %[^\n]", 
                        entry->timestamp, level_str, entry->message);
    
    if (scanned != 3) return 0;
    if (!parse_timestamp(entry->timestamp)) return 0;
    
    entry->level = parse_level(level_str);
    if (!IS_VALID_LEVEL(entry->level)) return 0;
    
    return 1;
}

void analyze_logs() {
    int level_counts[LOG_LEVELS] = {0};
    char earliest[TIMESTAMP_LEN + 1] = "";
    char latest[TIMESTAMP_LEN + 1] = "";
    
    for (int i = 0; i < entry_count; i++) {
        level_counts[entries[i].level]++;
        
        if (i == 0) {
            strncpy(earliest, entries[i].timestamp, TIMESTAMP_LEN);
            strncpy(latest, entries[i].timestamp, TIMESTAMP_LEN);
        } else {
            if (strcmp(entries[i].timestamp, earliest) < 0) {
                strncpy(earliest, entries[i].timestamp, TIMESTAMP_LEN);
            }
            if (strcmp(entries[i].timestamp, latest) > 0) {
                strncpy(latest, entries[i].timestamp, TIMESTAMP_LEN);
            }
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    printf("Time range: %s to %s\n", earliest, latest);
    printf("Level distribution:\n");
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("  %s: %d\n", level_names[i], level_counts[i]);
    }
}

int main(void) {
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter empty line to finish.\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(line, sizeof(line), stdin)) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (line[0] == '\0') break;
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
            printf("Added entry: %s\n", line);
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (entry_count > 0) {
        analyze_logs();
    } else {
        printf("No valid log entries to analyze.\n");
    }
    
    return 0;
}