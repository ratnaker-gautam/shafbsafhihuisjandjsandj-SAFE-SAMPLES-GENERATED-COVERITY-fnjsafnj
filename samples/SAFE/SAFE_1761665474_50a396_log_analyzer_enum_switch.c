//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum LogLevel {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_CRITICAL
};

struct LogEntry {
    char timestamp[20];
    enum LogLevel level;
    char message[256];
};

int parse_log_level(const char* level_str) {
    if (strcmp(level_str, "DEBUG") == 0) return LOG_DEBUG;
    if (strcmp(level_str, "INFO") == 0) return LOG_INFO;
    if (strcmp(level_str, "WARNING") == 0) return LOG_WARNING;
    if (strcmp(level_str, "ERROR") == 0) return LOG_ERROR;
    if (strcmp(level_str, "CRITICAL") == 0) return LOG_CRITICAL;
    return -1;
}

const char* level_to_string(enum LogLevel level) {
    switch (level) {
        case LOG_DEBUG: return "DEBUG";
        case LOG_INFO: return "INFO";
        case LOG_WARNING: return "WARNING";
        case LOG_ERROR: return "ERROR";
        case LOG_CRITICAL: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

int validate_timestamp(const char* timestamp) {
    if (strlen(timestamp) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 10) {
            if (timestamp[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    return 1;
}

int read_log_entry(struct LogEntry* entry) {
    char line[300];
    char level_str[20];
    
    if (fgets(line, sizeof(line), stdin) == NULL) return 0;
    
    if (sscanf(line, "%19s %19s %255[^\n]", entry->timestamp, level_str, entry->message) != 3) {
        return 0;
    }
    
    if (!validate_timestamp(entry->timestamp)) return 0;
    
    int level = parse_log_level(level_str);
    if (level == -1) return 0;
    
    entry->level = level;
    return 1;
}

void analyze_log_level(enum LogLevel level, int* counts) {
    switch (level) {
        case LOG_DEBUG:
            counts[0]++;
            break;
        case LOG_INFO:
            counts[1]++;
            break;
        case LOG_WARNING:
            counts[2]++;
            break;
        case LOG_ERROR:
            counts[3]++;
            break;
        case LOG_CRITICAL:
            counts[4]++;
            break;
        default:
            break;
    }
}

void print_analysis(int* counts, int total) {
    printf("Log Analysis Results:\n");
    printf("====================\n");
    printf("Total entries: %d\n", total);
    
    for (int i = 0; i < 5; i++) {
        const char* level_name = level_to_string(i);
        int count = counts[i];
        float percentage = total > 0 ? (count * 100.0f) / total : 0.0f;
        printf("%-8s: %d (%.1f%%)\n", level_name, count, percentage);
    }
}

int main() {
    struct LogEntry entry;
    int counts[5] = {0};
    int total_entries = 0;
    int valid_entries = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'END' on a separate line to finish.\n");
    
    while (1) {
        printf("Log entry %d: ", total_entries + 1);
        
        char first_word[10];
        if (fgets(first_word, sizeof(first_word), stdin) == NULL) break;
        
        if (strcmp(first_word, "END\n") == 0) break;
        
        if (strlen(first_word) > 0 && first_word[strlen(first_word)-1] == '\n') {
            first_word[strlen(first_word)-1] = '\0';
        }
        
        if (strcmp(first_word, "END") == 0) break;
        
        ungetc(first_word[0], stdin);
        for (size_t i = 1; i < strlen(first_word); i++) {
            ungetc(first_word[i], stdin);
        }
        
        if (read_log_entry(&entry)) {
            analyze_log_level(entry.level, counts);
            valid_entries++;
        } else {
            printf("Invalid log entry format. Skipping.\n");
            while (getchar() != '\n');
        }
        
        total_entries++;
        
        if (total_entries >= 100) {
            printf("Maximum entries reached (100).\n");
            break;
        }
    }
    
    if (valid_entries > 0) {
        print_analysis(counts, valid_entries);
    } else {
        printf("No valid log entries to analyze.\n");
    }
    
    return 0;
}