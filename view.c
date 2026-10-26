#include <stdio.h>
#include <string.h>
#include "header.h"

void view_tag(const char *file)
{
    FILE *fp = fopen(file, "rb");
    if (!fp)
    {
        printf("File open error\n");
        return;
    }

    unsigned char header[10];
    fread(header, 1, 10, fp);

    if (memcmp(header, "ID3", 3) != 0)
    {
        printf("No ID3 tag found\n");
        fclose(fp);
        return;
    }

    int tag_size = syncsafe_to_int(&header[6]);
    long tag_end = tag_size + 10;

    while (ftell(fp) < tag_end)
    {
        char id[5] = {0};
        unsigned char size_bytes[4];

        if (fread(id, 1, 4, fp) != 4)
            break;

        fread(size_bytes, 1, 4, fp);

        int size =
            (size_bytes[0] << 24) |
            (size_bytes[1] << 16) |
            (size_bytes[2] << 8)  |
             size_bytes[3];

        fseek(fp, 2, SEEK_CUR);

        if (!strncmp(id, "TIT2", 4) ||
            !strncmp(id, "TPE1", 4) ||
            !strncmp(id, "TALB", 4) ||
            !strncmp(id, "TYER", 4) ||
            !strncmp(id, "TCON", 4))
        {
            fseek(fp, 1, SEEK_CUR);
            char data[256] = {0};
            fread(data, 1, size - 1, fp);

            if (!strncmp(id, "TIT2", 4)) printf("Title   : %s\n", data);
            if (!strncmp(id, "TPE1", 4)) printf("Artist  : %s\n", data);
            if (!strncmp(id, "TALB", 4)) printf("Album   : %s\n", data);
            if (!strncmp(id, "TYER", 4)) printf("Year    : %s\n", data);
            if (!strncmp(id, "TCON", 4)) printf("Genre   : %s\n", data);
        }
        else if (!strncmp(id, "COMM", 4))
        {
            char ch;
            fseek(fp, 4, SEEK_CUR);
            while ((ch = fgetc(fp)) != '\0');

            char comment[256] = {0};
            fread(comment, 1, size, fp);
            printf("Comment : %s\n", comment);
        }
        else
        {
            fseek(fp, size, SEEK_CUR);
        }
    }

    fclose(fp);
}
