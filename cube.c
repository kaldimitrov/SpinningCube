#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

float rotationX = 0.0;
float rotationY = 0.0;
float rotationZ = 0.0;

float cameraDistance = 100.0;

int width = 160;
int height = 45;

float cubeWidth = 20.0;

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

void calculateForSurface(float cubeX, float cubeY, float cubeZ) {
    float x = calculateX(cubeX, cubeY, cubeZ);
    float y = calculateY(cubeX, cubeY, cubeZ);
    float z = calculateZ(cubeX, cubeY, cubeZ) + cameraDistance;

    float reciprocalZ = 1 / z;

    int screenX = (int)(width / 2 + (cubeWidth * -2) + reciprocalZ * x * 80);
    int screenY = (int)(height / 2 + 40 * reciprocalZ * y);

}

int main() {

  return 0;
}