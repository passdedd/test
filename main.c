/* test.c */
#include <stdio.h>
#include <stdlib.h>
int main()
{
        FILE *f;
        char s[1024];
        int ret;

        f = popen("./test.py 99", "r");
        while((ret=fread(s,3,1024,f))>0) {
                fwrite(s,3,ret,stdout);
        }
        fclose(f);
        return 0;
}
