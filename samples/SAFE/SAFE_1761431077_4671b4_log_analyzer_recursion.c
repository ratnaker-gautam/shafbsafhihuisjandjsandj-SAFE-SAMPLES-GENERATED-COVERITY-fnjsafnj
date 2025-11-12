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

int parse_log_recursive(FILE *file, struct LogEntry *entries, int max_entries, int current_index, int *error_count) {
    if (current_index >= max_entries || feof(file)) {
        return current_index;
    }

    char buffer[MAX_LINE_LENGTH];
    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        return current_index;
    }

    if (strlen(buffer) >= MAX_LINE_LENGTH - 1) {
        (*error_count)++;
        return parse_log_recursive(file, entries, max_entries, current_index, error_count);
    }

    struct LogEntry *entry = &entries[current_index];
    char level_buf[16];
    char message_buf[MAX_LINE_LENGTH];
    int timestamp;

    int parsed = sscanf(buffer, "%d %15s %1023[^\n]", &timestamp, level_buf, message_buf);
    
    if (parsed == 3 && timestamp >= 0 && timestamp <= 86400) {
        strncpy(entry->level, level_buf, sizeof(entry->level) - 1);
        entry->level[sizeof(entry->level) - 1] = '\0';
        strncpy(entry->message, message_buf, sizeof(entry->message) - 1);
        entry->message[sizeof(entry->message) - 1] = '\0';
        entry->timestamp = timestamp;
        current_index++;
    } else {
        (*error_count)++;
    }

    return parse_log_recursive(file, entries, max_entries, current_index, error_count);
}

void analyze_logs_recursive(struct LogEntry *entries, int count, int index, int *info_count, int *warning_count, int *error_count) {
    if (index >= count) {
        return;
    }

    if (strcmp(entries[index].level, "INFO") == 0) {
        (*info_count)++;
    } else if (strcmp(entries[index].level, "WARNING") == 0) {
        (*warning_count)++;
    } else if (strcmp(entries[index].level, "ERROR") == 0) {
        (*error_count)++;
    }

    analyze_logs_recursive(entries, count, index + 1, info_count, warning_count, error_count);
}

void print_analysis_recursive(struct LogEntry *entries, int count, int index) {
    if (index >= count) {
        return;
    }

    printf("Time: %d, Level: %s, Message: %s\n", 
           entries[index].timestamp, 
           entries[index].level, 
           entries[index].message);
    
    print_analysis_recursive(entries, count, index + 1);
}

int main(void) {
    struct LogEntry entries[100];
    int entry_count = 0;
    int parse_errors = 0;
    int info_count = 0;
    int warning_count = 0;
    int error_count = 0;

    printf("Enter log entries (format: timestamp LEVEL message)\n");
    printf("Example: 3600 INFO System started\n");
    printf("Enter 'END' on a separate line to finish input\n");

    char input_buffer[MAX_LINE_LENGTH];
    FILE *temp_file = tmpfile();
    if (temp_file == NULL) {
        fprintf(stderr, "Error creating temporary file\n");
        return 1;
    }

    while (fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
        if (strncmp(input_buffer, "END", 3) == 0 && (input_buffer[3] == '\n' || input_buffer[3] == '\0')) {
            break;
        }
        if (fputs(input_buffer, temp_file) == EOF) {
            fprintf(stderr, "Error writing to temporary file\n");
            fclose(temp_file);
            return 1;
        }
    }

    rewind(temp_file);
    entry_count = parse_log_recursive(temp_file, entries, 100, 0, &parse_errors);
    fclose(temp_file);

    analyze_logs_recursive(entries, entry_count, 0, &info_count, &warning_count, &error_count);

    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    printf("INFO entries: %d\n", info_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("ERROR entries: %d\n", error_count);
    printf("Parse errors: %d\n", parse_errors);

    if (entry_count > 0) {
        printf("\nAll log entries:\n");
        print_analysis_recursive(entries, entry_count, 0);
    }

    return 0;
}