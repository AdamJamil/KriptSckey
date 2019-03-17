#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    FILE *fp;
    fp = fopen(argv[1], "r");

    char line [1000];
    while(fgets(line, sizeof line, fp) != NULL)
    {

    }



    return 0;
}