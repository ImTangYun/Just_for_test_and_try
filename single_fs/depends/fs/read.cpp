#include <stdio.h>

int main()
{
    FILE* fp = fopen("data_storage.dat", "r");
    fseek(fp, 557, SEEK_SET);
    char buff[22];
    fread(buff, 1, 21, fp);
    buff[21] = '\0';
    printf("buff:%s\n", buff);
    return 0;
}
