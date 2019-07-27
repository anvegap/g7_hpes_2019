#include <stdio.h>
#include <stdlib.h>

int main()
{
    void *p = malloc(10);
    free(p);
    fprintf(stderr,"Message: hello!\n");
    return 0;
}
