//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
} LogEntry;

typedef void (*LogProcessor)(LogEntry*);

void count_entries(LogEntry* entry) {
    static int count = 0;
    count++;
    printf("Processed %d entries\n", count);
}

void show_errors(LogEntry* entry) {
    if (strcmp(entry->level, "ERROR") == 0) {
        printf("ERROR: %s - %s\n", entry->timestamp, entry->message);
    }
}

void show_warnings(LogEntry* entry) {
    if (strcmp(entry->level, "WARNING") == 0) {
        printf("WARNING: %s - %s\n", entry->timestamp, entry->message);
    }
}

void process_log_file(const char* filename, LogProcessor processor) {
    if (filename == NULL || processor == NULL) {
        fprintf(stderr, "Invalid input parameters\n");
        return;
    }

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Cannot open file: %s\n", filename);
        return;
    }

    char line[MAX_LINE_LENGTH];
    int entry_count = 0;
    LogEntry entries[MAX_ENTRIES];

    while (fgets(line, sizeof(line), file) != NULL && entry_count < MAX_ENTRIES) {
        line[strcspn(line, "\n")] = '\0';

        if (strlen(line) == 0) continue;

        LogEntry entry;
        int parsed = sscanf(line, "%31s %15s %255[^\n]", 
                           entry.timestamp, entry.level, entry.message);

        if (parsed == 3) {
            if (strlen(entry.timestamp) > 0 && strlen(entry.level) > 0 && strlen(entry.message) > 0) {
                entries[entry_count] = entry;
                entry_count++;
            }
        }
    }

    fclose(file);

    for (int i = 0; i < entry_count; i++) {
        processor(&entries[i]);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <logfile>\n", argv[0]);
        return 1;
    }

    if (strlen(argv[1]) == 0) {
        fprintf(stderr, "Invalid filename\n");
        return 1;
    }

    printf("Processing log file: %s\n", argv[1]);

    printf("\n=== Counting all entries ===\n");
    process_log_file(argv[1], count_entries);

    printf("\n=== Showing errors ===\n");
    process_log_file(argv[1], show_errors);

    printf("\n=== Showing warnings ===\n");
    process_log_file(argv[1], show_warnings);

    return 0;
}