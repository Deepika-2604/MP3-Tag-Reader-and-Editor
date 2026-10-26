#ifndef HEADER_H
#define HEADER_H

void view_tag(const char *filename);
void edit_tag(const char *filename, char option, const char *value);
void print_help(void);

int syncsafe_to_int(unsigned char s[4]);

#endif
