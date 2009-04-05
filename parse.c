#include <stdio.h>

int main() {
    unsigned char data[10];
    unsigned short pointer;
    unsigned short time;
    unsigned short *temp;

    switch (data[0]) {
        case 'C':
            printf("Sets the current frame\n");
            printf("G\n");
            break;
        case 'L':
            if (data[1] == 1) {
                printf("Turned on aux LED\n");
                printf("G\n");
                break;
            } else {
                printf("Turned off aux LED\n");
                printf("G\n");
                break;
            }
        case 'Z':
            printf("Returns the size of the animation\n");
            printf("G\n");
            break;
        case 'S':
            printf("Start animation and repeat %d times\n", data[1]);
            printf("G\n");
            break;
        case 'X':
            temp = &data[1];
            pointer = *temp;
            if (pointer > 4096) {
                printf("Bad address");
                printf("B");
                break;
            } else {
                printf("Jumped to location, %02x%02x\n", data[1], data[2]);
                printf("G\n");
                break;
            }
        case 'D':
            if (data[1] == 0 && data[2] == 0) {
                printf("Jumped to %02x%02x\n", data[3], data[4]);
                printf("G\n");
                break;
            } else {
                printf("Set data to %02x%02x%02x%02x%02x%02x with timing %02x%02x\n",
                data[3], data[4], data[5], data[6], data[7], data[8], data[1], 
                data[2]);
                printf("G\n");
                break;
            }
        case 'T':
            temp = &data[1];
            time = *temp;
            printf("Period is : &d\n", time);
            printf("G\n");
            break;
        case 'N':
            printf("No operation performed\n");
            printf("G\n");
            break;
        case 'P':
            printf("Paused the animation\n");
            printf("G\n");
            break;
        case 'R':
            printf("Reset animation\n");
            printf("G\n");
            break;
    }

    return 0;
}

