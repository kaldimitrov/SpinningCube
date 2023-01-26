#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

float rotationX = 0.0;
float rotationY = 0.0;
float rotationZ = 0.0;

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

int main() {

  return 0;
}