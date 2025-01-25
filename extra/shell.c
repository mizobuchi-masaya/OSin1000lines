#include "user.h"

void asciiart(void) {
   	int x, y, i;
	float a, b, ca, cb, t;
	const static unsigned char c[] = "0123456789ABCDEF ";

	for (y = -12; y <= 12; y++) {
		for (x = -39; x <= 39; x++) {
			a = ca = x * 0.0458;
			b = cb = y * 0.08333;

			for (i = 0; i <= 15; i++) {
				t = (a * a) - (b * b) + ca;
				b = (2.0 * a * b) + cb;
				a = t;

				if ((a * a + b * b) > 4.0)
					break;
			}
            putchar(c[i]);
		}
    	putchar('\n');
	}
}

void main(void) {
#if 0
    *((volatile int *) 0x80200000) = 0x1234;
    for (;;);
#endif

    while (1) {
prompt:
        printf("> ");
        char cmdline[128];
        for (int i = 0;; i++) {
            char ch = getchar();
            putchar(ch);
            if (i == sizeof(cmdline) -1) {
                printf("command line too long\n");
                goto prompt;
            } else if (ch == '\r') {
                printf("\n");
                cmdline[i] = '\0';
                break;
            } else {
                cmdline[i] = ch;
            }
        }
        if  (strcmp(cmdline, "hello") == 0)
            printf("Hello World from shell!\n");
        else if (strcmp(cmdline, "exit") == 0)
            exit();
        else if (strcmp(cmdline, "readfile") == 0) {
            char buf[128];
            int len = readfile("./hello.txt", buf, sizeof(buf));
            buf[len] = '\0';
            printf("%s\n", buf);
        }
        else if (strcmp(cmdline, "writefile") == 0)
            writefile("./hello.txt", "Hello from shell!\n", 19);
        else if (strcmp(cmdline, "asciiart") == 0)
            asciiart();
        else
            printf("unknown command: %s\n", cmdline);
    }
}
