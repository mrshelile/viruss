#include <GLFW/glfw3.h>
#include <stdlib.h>

// Bresenham’s line algorithm
void Bresenham(int x0, int y0, int x1, int y1) {
    int dx = x1 - x0;
    int dy = y1 - y0;
    int x = x0, y = y0;
    int p = 2 * dy - 2 * dx;

    glBegin(GL_POINTS);
    glVertex2i(x, y);
    while (x < x1) {
        if (p > 0) {
            p += 2 * dy - 2 * dx;
            x++;  y++;
        }
        else {
            p += 2 * dy;
            x++;
        }
        glVertex2i(x, y);
    }
    glEnd();
}

int main(void) {
    // 1) Init GLFW
    if (!glfwInit()) return -1;
    // Request compatibility profile for immediate‐mode
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    // 2) Create window
    GLFWwindow* win = glfwCreateWindow(600, 600,
        "Bresenham Line (GLFW)",
        NULL, NULL);
    if (!win) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(win);

    // 3) Setup projection once
    glClearColor(0, 0, 0, 1);              // black bg
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // match gluOrtho2D(0,420,0,200)
    glOrtho(0.0, 420.0, 0.0, 200.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);

    // 4) Main loop
    while (!glfwWindowShouldClose(win)) {
        int w, h;
        glfwGetFramebufferSize(win, &w, &h);
        glViewport(0, 0, w, h);

        glClear(GL_COLOR_BUFFER_BIT);

        // draw a line from (110,10) to (210,30)
        Bresenham(110, 10, 210, 30);

        glFlush();  // ensure points are drawn
        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    // 5) Cleanup
    glfwDestroyWindow(win);
    glfwTerminate();
    return 0;
}
