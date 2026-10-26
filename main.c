#include <stdio.h>
#include <string.h>
#include "header.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        print_help();
        return 1;
    }

    /* Help */
    if (strcmp(argv[1], "-h") == 0)
    {
        print_help();
        return 0;
    }

    /* View */
    if (strcmp(argv[1], "-v") == 0)
    {
        if (argc != 3)
        {
            printf("Usage: %s -v <mp3file>\n", argv[0]);
            return 1;
        }
        view_tag(argv[2]);
        return 0;
    }

    /* Edit */
    if (strcmp(argv[1], "-e") == 0)
    {
        if (argc != 5)
        {
            printf("Usage: %s -e <mp3file> -t|-a|-A|-y|-g|-c \"value\"\n", argv[0]);
            return 1;
        }

        char tag_opt;
        if      (strcmp(argv[3], "-t") == 0) tag_opt = 't';
        else if (strcmp(argv[3], "-a") == 0) tag_opt = 'a';
        else if (strcmp(argv[3], "-A") == 0) tag_opt = 'A';
        else if (strcmp(argv[3], "-y") == 0) tag_opt = 'y';
        else if (strcmp(argv[3], "-g") == 0) tag_opt = 'g';
        else if (strcmp(argv[3], "-c") == 0) tag_opt = 'c';
        else
        {
            printf("Invalid tag option\n");
            return 1;
        }

        edit_tag(argv[2], tag_opt, argv[4]);
        return 0;
    }

    print_help();
    return 1;
}
