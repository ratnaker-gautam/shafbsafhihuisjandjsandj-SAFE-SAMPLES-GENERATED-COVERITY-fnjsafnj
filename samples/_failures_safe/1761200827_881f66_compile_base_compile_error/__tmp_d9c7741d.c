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
    int scanned = sscanf(line, "%19s %15s %1023[^\n]", 
                        entry->timestamp, level_str, entry->message);
    
    if (scanned != 3) return 0;
    if (!parse_timestamp(entry->timestamp)) return 0;
    
    int level = parse_level(level_str);
    if (!IS_VALID_LEVEL(level)) return 0;
    
    entry->level = level;
    return 1;
}

void load_logs(void) {
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (timestamp level message):\n");
    printf("Format: YYYY-MM-DD HH:MM:SS LEVEL message text\n");
    printf("Levels: ERROR, WARN, INFO, DEBUG\n");
    printf("Enter empty line to finish.\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin)) {
        if (line[0] == '\n') break;
        
        line[strcspn(line, "\n")] = 0;
        
        if (strlen(line) == 0) continue;
        
        struct log_entry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count++] = entry;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
}

void analyze_logs(void) {
    if (entry_count == 0) {
        printf("No logs to analyze.\n");
        return;
    }
    
    int level_counts[LOG_LEVELS] = {0};
    char earliest[TIMESTAMP_LEN + 1] = "";
    char latest[TIMESTAMP_LEN + 1] = "";
    
    strcpy(earliest, entries[0].timestamp);
    strcpy(latest, entries[0].timestamp);
    
    for (int i = 0; i < entry_count; i++) {
        level_counts[entries[i].level]++;
        
        if (strcmp(entries[i].timestamp, earliest) < 0) {
            strcpy(earliest, entries[i].timestamp);
        }
        if (strcmp(entries[i].timestamp, latest) > 0) {
            strcpy(latest, entries[i].timestamp);
        }
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    printf("Time range: %s to %s\n", earliest, latest);
    printf("Level distribution:\n");
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        float percentage = (float)level_counts[i] / entry_count * 100;
        printf("  %s: %d (%.1f%%)\n", level_names[i], level_counts[i], percentage);
    }
}

void filter_by_level(int level) {
    if (!IS_VALID_LEVEL(level)) {
        printf("Invalid log level.\n");
        return;
    }
    
    printf("\nEntries with level %s:\n", level_names[level]);
    int found = 0;
    
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].level == level) {
            printf("%s %s %s\n", entries[i].timestamp, level_names[entries[i].level], entries[i].message);
            found++;
        }
    }
    
    if (!found) {
        printf("No entries found.\n");
    }
}

int main(void) {
    load_logs();
    
    if (entry_count == 0) {
        printf("No valid logs loaded.\n");
        return 1;
    }
    
    analyze_logs();
    
    printf("\nEnter level to filter (ERROR/WARN/INFO/DEBUG) or 'quit': ");
    char input[16];
    
    while (fgets(input, sizeof(input), stdin)) {
        input[strcspn(input, "\n")] = 0;
        
        if