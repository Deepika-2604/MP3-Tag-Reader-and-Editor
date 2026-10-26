#include <stdio.h>
#include <string.h>
#include "header.h"

int syncsafe_to_int(unsigned char s[4])
{
    return (s[0] << 21) | (s[1] << 14) | (s[2] << 7) | s[3];
}

void edit_tag(const char *filename, char option, const char *value)
{
    FILE *fp = fopen(filename, "rb+");
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

    const char *fid = NULL;

    switch (option)
    {
        case 't': fid = "TIT2"; break;
        case 'a': fid = "TPE1"; break;
        case 'A': fid = "TALB"; break;
        case 'y': fid = "TYER"; break;
        case 'g': fid = "TCON"; break;
        case 'c': fid = "COMM"; break;
    }

    while (ftell(fp) < tag_end)
    {
        char id[5] = {0};
        unsigned char size_bytes[4];

        fread(id, 1, 4, fp);
        fread(size_bytes, 1, 4, fp);

        int size =
            (size_bytes[0] << 24) |
            (size_bytes[1] << 16) |
            (size_bytes[2] << 8)  |
             size_bytes[3];

        fseek(fp, 2, SEEK_CUR);

        if (strcmp(id, fid) == 0)
        {
            long start = ftell(fp);

            if (option == 'c')
            {
                fseek(fp, 4, SEEK_CUR);
                while (fgetc(fp) != '\0');
            }
            else
            {
                fseek(fp, 1, SEEK_CUR);
            }

            int max_len = size - (ftell(fp) - start);

            if ((int)strlen(value) <= max_len)
            {
                fwrite(value, 1, strlen(value), fp);
                for (int i = strlen(value); i < max_len; i++)
                    fputc('\0', fp);

                printf("%s updated successfully\n", fid);
            }
            else
                printf("Value too long\n");

            break;
        }
        else
            fseek(fp, size, SEEK_CUR);
    }

    fclose(fp);
}
