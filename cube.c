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

    /// Calculate the screen coordinates of the cube
    int screenX = (int)(WIDTH / 2 + (cubeWidth * -2) + reciprocalZ * x * 80);
    int screenY = (int)(HEIGHT / 2 + 40 * reciprocalZ * y);

    // Calculate the index of the pixel for both the depth and screen array
    int pixelIndex = screenX + screenY * WIDTH;

    // Check if the pixel is outside the screen
    if(pixelIndex < 0 || pixelIndex > WIDTH * HEIGHT) {
        return;
    }

    // Check if the pixel is behind another pixel
    if(reciprocalZ < depthArray[pixelIndex]) {
        return;
    }

    depthArray[pixelIndex] = reciprocalZ;
    screenArray[pixelIndex] = surface;
}

int main() {
    // Clear the screen before starting
    int ret = write(1, "\x1b[2J", 4);
    if(ret == -1) {
        printf("Error writing to stdout");
        exit(1);
    }

    while (1) {
        // Clear the depth and screen arrays and reset the values
        memset(depthArray, 0, WIDTH * HEIGHT * 4);
        memset(screenArray, BACKGROUND, WIDTH * HEIGHT);

        /*
        * Calculate the surface of the cube:
        *  @ - Front
        *  $ - Right
        *  ~ - Left
        *  # - Back
        *  ; - Bottom
        *  + - Top
        */
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

        // Move the cursor to the top left corner
        ret = write(1, "\x1b[H", 4);
        if(ret == -1) {
            printf("Error writing to stdout");
            exit(2);
        }

        // Print the cube to the screen
        for (int k = 0; k < (WIDTH * HEIGHT); k++) {
            putchar(k % WIDTH ? screenArray[k] : 10);
        }

        // Rotate the cube on the X, Y and Z axis
        rotationX += 0.05;
        rotationY += 0.05;
        rotationZ += 0.01;

        // Sleep for 10 milliseconds
        usleep(10000);
    }

    return 0;
}