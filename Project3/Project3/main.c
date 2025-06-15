#include <GLFW/glfw3.h>
#include <math.h>
#include <stdlib.h>

int abs(int n) { return (n > 0) ? n : -n; }

void DDA(float X0, float Y0, float X1, float Y1) {
    float dx = X1 - X0;
    float dy = Y1 - Y0;
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    float Xincrement = dx / (float)steps;
    float Yincrement = dy / (float)steps;
    float X = X0;
    float Y = Y0;

    glBegin(GL_POINTS);
    for (int v = 0; v <= steps; v++) {
        glVertex2f(X, Y);
        X += Xincrement;
        Y += Yincrement;
    }
    glEnd();
}

void setup2DProjection(int width, int height) {
   
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, width, 0.0, height, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(void) {
    if (!glfwInit())
        return -1;

    GLFWwindow* window = glfwCreateWindow(640, 480, "DDA Line with GLFW", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    setup2DProjection(640, 480);
    glClearColor(0.0, 0.0, 0.0, 1.0); // Black background

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0, 0.0, 0.0); // Red color

        DDA(110, 110, 800, 300);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}