//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

struct LogEntry {
    char level[16];
    char message[MAX_LINE_LENGTH];
    int timestamp;
};

int parse_log_recursive(FILE *file, struct LogEntry *entries, int max_entries, int current_depth) {
    if (current_depth >= max_entries || current_depth >= MAX_LEVELS) {
        return current_depth;
    }

    char line[MAX_LINE_LENGTH];
    if (fgets(line, sizeof(line), file) == NULL) {
        return current_depth;
    }

    if (strlen(line) == 0 || line[0] == '\n') {
        return parse_log_recursive(file, entries, max_entries, current_depth);
    }

    struct LogEntry *entry = &entries[current_depth];
    int hour, minute, second;
    char level_buf[16];
    char message_buf[MAX_LINE_LENGTH];

    int parsed = sscanf(line, "%d:%d:%d %15s %1023[^\n]", 
                       &hour, &minute, &second, level_buf, message_buf);

    if (parsed >= 4) {
        if (hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 || second > 59) {
            return parse_log_recursive(file, entries, max_entries, current_depth);
        }

        strncpy(entry->level, level_buf, sizeof(entry->level) - 1);
        entry->level[sizeof(entry->level) - 1] = '\0';
        
        strncpy(entry->message, message_buf, sizeof(entry->message) - 1);
        entry->message[sizeof(entry->message) - 1] = '\0';
        
        entry->timestamp = hour * 3600 + minute * 60 + second;
        
        return parse_log_recursive(file, entries, max_entries, current_depth + 1);
    }

    return parse_log_recursive(file, entries, max_entries, current_depth);
}

void analyze_logs_recursive(struct LogEntry *entries, int count, int index, 
                           int *error_count, int *warning_count, int *info_count) {
    if (index >= count) {
        return;
    }

    if (strcmp(entries[index].level, "ERROR") == 0) {
        (*error_count)++;
    } else if (strcmp(entries[index].level, "WARNING") == 0) {
        (*warning_count)++;
    } else if (strcmp(entries[index].level, "INFO") == 0) {
        (*info_count)++;
    }

    analyze_logs_recursive(entries, count, index + 1, error_count, warning_count, info_count);
}

void display_logs_recursive(struct LogEntry *entries, int count, int index) {
    if (index >= count) {
        return;
    }

    int hour = entries[index].timestamp / 3600;
    int minute = (entries[index].timestamp % 3600) / 60;
    int second = entries[index].timestamp % 60;
    
    printf("%02d:%02d:%02d [%s] %s\n", hour, minute, second, 
           entries[index].level, entries[index].message);
    
    display_logs_recursive(entries, count, index + 1);
}

int main(void) {
    struct LogEntry entries[100];
    int entry_count = 0;
    
    printf("Log Analyzer - Enter log entries (format: HH:MM:SS LEVEL message)\n");
    printf("Supported levels: INFO, WARNING, ERROR\n");
    printf("Enter 'END' on a separate line to finish input\n\n");
    
    char input[MAX_LINE_LENGTH];
    int line_count = 0;
    
    while (line_count < 100) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "END") == 0) {
            break;
        }
        
        if (strlen(input) == 0) {
            continue;
        }
        
        if (strlen(input) >= MAX_LINE_LENGTH - 1) {
            printf("Input too long, skipping...\n");
            continue;
        }
        
        FILE *temp = tmpfile();
        if (temp == NULL) {
            printf("Error creating temporary file\n");
            return 1;
        }
        
        fputs(input, temp);
        rewind(temp);
        
        int new_count = parse_log_recursive(temp, entries, 100, entry_count);
        if (new_count > entry_count) {
            entry_count = new_count;
            line_count++;
        }
        
        fclose(temp);
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 0;
    }
    
    printf("\n=== Log Analysis Results ===\n");
    printf("Total entries: %d\n", entry_count);
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    analyze_logs_recursive(entries, entry_count, 0, &error_count, &warning_count, &info_count);
    
    printf("ERROR entries: %d\n", error_count