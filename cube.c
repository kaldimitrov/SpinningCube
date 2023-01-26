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

int main() {

  return 0;
}