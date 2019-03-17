#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void nec(FILE* fp) {
    fprintf(fp, "\t\t\ttry\n");
    fprintf(fp, "\t\t\t{\n");
    fprintf(fp, "\t\t\t\tField f = NativeInputEvent.class.getDeclaredField(\"reserved\");\n");
    fprintf(fp, "\t\t\t\tf.setAccessible(true);\n");
    fprintf(fp, "\t\t\t\tf.setShort(e, (short) 0x01);\n");
    fprintf(fp, "\t\t\t}\n");
    fprintf(fp, "\t\t\tcatch (Exception ex)\n");
    fprintf(fp, "\t\t\t{\n");
    fprintf(fp, "\t\t\t\tex.printStackTrace();\n");
    fprintf(fp, "\t\t\t}\n\n");
    fprintf(fp, "\t\t\ttyping = true;\n\n");
}

int split(char s[], char delim) {
    int i = 0;
    while(s[i] != delim){
        i++;
        if(s[i] > 96 && s[i] < 123) {
            s[i] -= 32;
        }
    }
    s[i] = '\0';
    return i;
}

int main(int argc, char* argv[])
{
    FILE* readFile;
    
    FILE* writeFile;
    writeFile = fopen("HelloWorld.java", "w");

    char buf[1000];
    // Before Key Pressed
    readFile = fopen("Hello", "r");
    while(fgets(buf, sizeof buf, readFile) != NULL)
    {
        fprintf(writeFile, "%s", buf);
    }
    fclose(readFile);

    int read = 0;
    int p = 0;

    readFile = fopen(argv[1], "r");
    while(fgets(buf, sizeof buf, readFile) != NULL)
    {
        printf("%s", buf);
        if(read) {
            if(buf[0] == '}') {
                read--;
                fprintf(writeFile, "\t\t}\n");
            }
            else {
                p = split(buf, ':');
                fprintf(writeFile, "\t\tif (e.getKeyCode() == NativeKeyEvent.VC_%s)\n", &buf[1]);
                fprintf(writeFile, "\t\t{\n");
                nec(writeFile);
                while(buf[++p] != '\0') {
                    if(buf[p] > 65 && buf[p] < 91)
                    {
                        fprintf(writeFile, "\t\t\trobot.keyPress(KeyEvent.VK_SHIFT);\n");
                        fprintf(writeFile, "\t\t\trobot.keyPress(KeyEvent.VK_%c);\n", buf[p]);
                        fprintf(writeFile, "\t\t\trobot.keyRelease(KeyEvent.VK_%c);\n", buf[p]);
                        fprintf(writeFile, "\t\t\trobot.keyRelease(KeyEvent.VK_SHIFT);\n");
                    }
                    else if(buf[p] > 96 && buf[p] < 123) {
                        fprintf(writeFile, "\t\t\trobot.keyPress(KeyEvent.VK_%c);\n", buf[p]-32);
                        fprintf(writeFile, "\t\t\trobot.keyRelease(KeyEvent.VK_%c);\n", buf[p]-32);
                    }
                    else if(buf[p] == 33) {
                        fprintf(writeFile, "\t\t\trobot.keyPress(KeyEvent.VK_SHIFT);\n");
                        fprintf(writeFile, "\t\t\trobot.keyPress(KeyEvent.VK_1);\n");
                        fprintf(writeFile, "\t\t\trobot.keyRelease(KeyEvent.VK_1);\n");
                        fprintf(writeFile, "\t\t\trobot.keyRelease(KeyEvent.VK_SHIFT);\n");
                    }
                }
            }
        }
        else {
            if(buf[0] == '{') {
                read++;
            }
        }
    }
    fclose(readFile);

    // After Key Pressed
    readFile = fopen("World", "r");
    while(fgets(buf, sizeof buf, readFile) != NULL)
    {
        fprintf(writeFile, "%s", buf);
    }
    fclose(readFile);

    fclose(writeFile);

    return 0;
}