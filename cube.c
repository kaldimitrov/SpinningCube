#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#define WIDTH 160
#define HEIGHT 45

#define cameraDistance 100.0
#define cubeWidth 20.0
#define incrementSpeed 0.75

#define BACKGROUND ' '

float rotationX = 0.0;
float rotationY = 0.0;
float rotationZ = 0.0;

float depthArray[WIDTH * HEIGHT];
char screenArray[WIDTH * HEIGHT];


float calculateX(int cubeX, int cubeY, int cubeZ) {
    float x = cubeY * sin(rotationX) * sin(rotationY) * cos(rotationZ) - 
              cubeZ * cos(rotationX) * sin(rotationY) * cos(rotationZ) + 
              cubeY * cos(rotationX) * sin(rotationZ) + 
              cubeZ * sin(rotationX) * sin(rotationZ) + 
              cubeX * cos(rotationY) * cos(rotationZ);

    return x;
}

float calculateY(int cubeX, int cubeY, int cubeZ) {
    float y = cubeY * cos(rotationX) * cos(rotationZ) + 
              cubeZ * sin(rotationX) * cos(rotationZ) -
              cubeY * sin(rotationX) * sin(rotationY) * sin(rotationZ) +
              cubeZ * cos(rotationX) * sin(rotationY) * sin(rotationZ) -
              cubeX * cos(rotationY) * sin(rotationZ);

    return y;
}

float calculateZ(int cubeX, int cubeY, int cubeZ) {
    float z = cubeZ * cos(rotationX) * cos(rotationY) -
              cubeY * sin(rotationX) * cos(rotationY) + 
              cubeX * sin(rotationY);

    return z;
}

void calculateForSurface(float cubeX, float cubeY, float cubeZ, char surface) {
    float x = calculateX(cubeX, cubeY, cubeZ);
    float y = calculateY(cubeX, cubeY, cubeZ);
    float z = calculateZ(cubeX, cubeY, cubeZ) + cameraDistance;

    float reciprocalZ = 1 / z;

    int screenX = (int)(WIDTH / 2 + (cubeWidth * -2) + reciprocalZ * x * 80);
    int screenY = (int)(HEIGHT / 2 + 40 * reciprocalZ * y);

    int pixelIndex = screenX + screenY * WIDTH;

    if(pixelIndex < 0 || pixelIndex > WIDTH * HEIGHT) {
        return;
    }

    if(reciprocalZ < depthArray[pixelIndex]) {
        return;
    }

    depthArray[pixelIndex] = reciprocalZ;
    screenArray[pixelIndex] = surface;
}

int main() {
    int ret = write(1, "\x1b[2J", 4);
    if(ret == -1) {
        printf("Error writing to stdout");
        exit(1);
    }

    while (1) {
        memset(depthArray, 0, WIDTH * HEIGHT * 4);
        memset(screenArray, BACKGROUND, WIDTH * HEIGHT);

        for (float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += incrementSpeed) {
            for (float cubeY = -cubeWidth; cubeY < cubeWidth; cubeY += incrementSpeed) {
                calculateForSurface(cubeX, cubeY, -cubeWidth, '@');
                calculateForSurface(cubeWidth, cubeY, cubeX, '$');
                calculateForSurface(-cubeWidth, cubeY, -cubeX, '~');
                calculateForSurface(-cubeX, cubeY, cubeWidth, '#');
                calculateForSurface(cubeX, -cubeWidth, -cubeY, ';');
                calculateForSurface(cubeX, cubeWidth, cubeY, '+');
            }
        }

        ret = write(1, "\x1b[H", 4);
        if(ret == -1) {
            printf("Error writing to stdout");
            exit(2);
        }

        for (int k = 0; k < (WIDTH * HEIGHT); k++) {
            putchar(k % WIDTH ? screenArray[k] : 10);
        }

        rotationX += 0.05;
        rotationY += 0.05;
        rotationZ += 0.01;

        usleep(8000 * 2);
    }

    return 0;
}