#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <GLFW/glfw3.h>

//---------------------------------------------------------------------
// Vec2i helper for plotting
static void Vertex2i(int x, int y) {
    glVertex2i(x, y);
}

//---------------------------------------------------------------------
// Plot the eight symmetric points around (p,q)
static void EightWaySymmetryPlot(int p, int q, int x, int y) {
    Vertex2i(x + p, y + q);
    Vertex2i(y + p, x + q);
    Vertex2i(-y + p, x + q);
    Vertex2i(-x + p, y + q);
    Vertex2i(-x + p, -y + q);
    Vertex2i(-y + p, -x + q);
    Vertex2i(y + p, -x + q);
    Vertex2i(x + p, -y + q);
}

//---------------------------------------------------------------------
// Bresenham's circle algorithm
static void BresenhamCircle(int p, int q, int r) {
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;

    glBegin(GL_POINTS);
    EightWaySymmetryPlot(p, q, x, y);
    while (x <= y) {
        if (d <= 0) {
            d += 4 * x + 6;
        }
        else {
            d += 4 * (x - y) + 10;
            y--;
        }
        x++;
        EightWaySymmetryPlot(p, q, x, y);
    }
    glEnd();
}

//---------------------------------------------------------------------
// Render callback
static void render() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw a circle at (100,100) of radius 20
    BresenhamCircle(100, 100, 20);

    glFlush();
}

//---------------------------------------------------------------------
// Set up 2D orthographic projection matching gluOrtho2D(0,420,0,200)
static void init2D(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // left, right, bottom, top, near, far
    glOrtho(0.0, 420.0, 0.0, 200.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // default draw color red
    glColor3f(1.0f, 0.0f, 0.0f);
}

int main(void) {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return EXIT_FAILURE;
    }

    // Request an OpenGL 2.1 context (for glBegin/glEnd)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    GLFWwindow* window = glfwCreateWindow(600, 600,
        "Bresenham Circle (GLFW)",
        NULL, NULL);
    if (!window) {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    // Set swap interval to 1 for vsync
    glfwSwapInterval(1);

    // Initialize projection
    init2D(600, 600);


    // Main loop
    while (!glfwWindowShouldClose(window)) {
        render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}
