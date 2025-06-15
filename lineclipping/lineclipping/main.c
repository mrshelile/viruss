//#include <GLFW/glfw3.h>
//#include <math.h>
//#include <stdlib.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <stdbool.h>    // for bool, true, false
//// Region codes
//#define INSIDE 0  // 0000
//#define LEFT   1  // 0001
//#define RIGHT  2  // 0010
//#define BOTTOM 4  // 0100
//#define TOP    8  // 1000
//
//// Clipping window boundaries
//float xmin = 50.0f, ymin = 10.0f, xmax = 80.0f, ymax = 40.0f;
//// Viewport boundaries
//float vxmin = 200.0f, vymin = 50.0f, vxmax = 350.0f, vymax = 150.0f;
//
//// Original line endpoints (p1 inside, p2 outside)
//float x_start = 70.0f, y_start = 20.0f;  // inside window
//float x_end = 100.0f, y_end = 10.0f;  // outside window
//// Clipped line endpoints
//float cx0, cy0, cx1, cy1;
//bool clipped = false;
//
//int abs(int n) { return (n > 0) ? n : -n; }
//
//// Function prototypes
//int ComputeOutCode(float x, float y);
//bool CohenSutherlandLineClipAndDraw(float xs, float ys, float xe, float ye);
//void DDA(float X0, float Y0, float X1, float Y1);
//void DrawClippingWindow();
//void DrawViewport();
//void display(void);
//void keyboard(unsigned char key, int x, int y);
//
//void DDA(float X0, float Y0, float X1, float Y1) {
//    float dx = X1 - X0;
//    float dy = Y1 - Y0;
//    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
//    float Xincrement = dx / (float)steps;
//    float Yincrement = dy / (float)steps;
//    float X = X0;
//    float Y = Y0;
//
//    glBegin(GL_POINTS);
//    for (int v = 0; v <= steps; v++) {
//        glVertex2f(X, Y);
//        X += Xincrement;
//        Y += Yincrement;
//    }
//    glEnd();
//}
//
//// Compute region outcode for point (x,y)
//int ComputeOutCode(float x, float y) {
//    int code = INSIDE;
//    if (x < xmin)      code |= LEFT;
//    else if (x > xmax) code |= RIGHT;
//    if (y < ymin)      code |= BOTTOM;
//    else if (y > ymax) code |= TOP;
//    return code;
//}
//
//// Cohen-Sutherland line clipping algorithm
//bool CohenSutherlandLineClipAndDraw(float xs, float ys, float xe, float ye) {
//    float x0 = xs, y0 = ys, x1 = xe, y1 = ye;
//    int out0 = ComputeOutCode(x0, y0);
//    int out1 = ComputeOutCode(x1, y1);
//    bool accept = false;
//
//    while (true) {
//        if ((out0 | out1) == INSIDE) {
//            accept = true;
//            break;
//        }
//        else if (out0 & out1) {
//            break;
//        }
//        else {
//            int outcodeOut = out0 ? out0 : out1;
//            float x, y;
//
//            if (outcodeOut & TOP) {
//                x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
//                y = ymax;
//            }
//            else if (outcodeOut & BOTTOM) {
//                x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
//                y = ymin;
//            }
//            else if (outcodeOut & RIGHT) {
//                y = y0 + (y1 - y0) * (xmax - x0) / (x1 - x0);
//                x = xmax;
//            }
//            else { // LEFT
//                y = y0 + (y1 - y0) * (xmin - x0) / (x1 - x0);
//                x = xmin;
//            }
//
//            if (outcodeOut == out0) {
//                x0 = x; y0 = y; out0 = ComputeOutCode(x0, y0);
//            }
//            else {
//                x1 = x; y1 = y; out1 = ComputeOutCode(x1, y1);
//            }
//        }
//    }
//
//    if (accept) {
//        cx0 = x0; cy0 = y0;
//        cx1 = x1; cy1 = y1;
//    }
//    return accept;
//}
//
//
//void setup2DProjection(int width, int height) {
//   
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    glOrtho(0.0, width, 0.0, height, -1.0, 1.0);
//    glMatrixMode(GL_MODELVIEW);
//}
//
//int main(void) {
//    if (!glfwInit())
//        return -1;
//
//    GLFWwindow* window = glfwCreateWindow(640, 480, "Cohen-Sutherland Clipping & Viewport", NULL, NULL);
//    if (!window) {
//        glfwTerminate();
//        return -1;
//    }
//
//    glfwMakeContextCurrent(window);
//    setup2DProjection(640, 480);
//    glClearColor(0.0, 0.0, 0.0, 1.0); // Black background
//
//    while (!glfwWindowShouldClose(window)) {
//        glClear(GL_COLOR_BUFFER_BIT);
//        glColor3f(1.0, 0.0, 0.0); // Red color
//
//        DDA(110, 110, 800, 300);
//
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    glfwTerminate();
//    return 0;
//}
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

// Region codes
#define INSIDE 0  // 0000
#define LEFT   1  // 0001
#define RIGHT  2  // 0010
#define BOTTOM 4  // 0100
#define TOP    8  // 1000

// Clipping window boundaries
static float xmin = 50.0f, ymin = 10.0f, xmax = 80.0f, ymax = 40.0f;
// Viewport boundaries
static float vxmin = 200.0f, vymin = 50.0f, vxmax = 350.0f, vymax = 150.0f;

// Original line endpoints
static float x_start = 70.0f, y_start = 20.0f;
static float x_end = 100.0f, y_end = 10.0f;

// Clipped line endpoints & flag
static float cx0, cy0, cx1, cy1;
static bool clipped = false;

// Compute absolute value
static int iabs(int x) { return x < 0 ? -x : x; }

// DDA line‐drawing
static void DDA(float X0, float Y0, float X1, float Y1) {
    float dx = X1 - X0;
    float dy = Y1 - Y0;
    int steps = iabs((int)dx) > iabs((int)dy) ? iabs((int)dx) : iabs((int)dy);
    float Xinc = dx / (float)steps;
    float Yinc = dy / (float)steps;
    float X = X0, Y = Y0;
    glBegin(GL_POINTS);
    for (int i = 0; i <= steps; i++) {
        glVertex2f(X, Y);
        X += Xinc;
        Y += Yinc;
    }
    glEnd();
}

// Compute region outcode for point (x,y)
static int ComputeOutCode(float x, float y) {
    int code = INSIDE;
    if (x < xmin) code |= LEFT;
    else if (x > xmax) code |= RIGHT;
    if (y < ymin) code |= BOTTOM;
    else if (y > ymax) code |= TOP;
    return code;
}

// Cohen–Sutherland clipping
static bool CohenSutherlandClip(float xs, float ys, float xe, float ye) {
    float x0 = xs, y0 = ys, x1 = xe, y1 = ye;
    int out0 = ComputeOutCode(x0, y0);
    int out1 = ComputeOutCode(x1, y1);
    bool accept = false;

    while (1) {
        if ((out0 | out1) == INSIDE) {
            accept = true;
            break;
        }
        else if (out0 & out1) {
            break;
        }
        else {
            float x, y;
            int outcodeOut = out0 ? out0 : out1;

            if (outcodeOut & TOP) {
                x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
                y = ymax;
            }
            else if (outcodeOut & BOTTOM) {
                x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
                y = ymin;
            }
            else if (outcodeOut & RIGHT) {
                y = y0 + (y1 - y0) * (xmax - x0) / (x1 - x0);
                x = xmax;
            }
            else { // LEFT
                y = y0 + (y1 - y0) * (xmin - x0) / (x1 - x0);
                x = xmin;
            }

            if (outcodeOut == out0) {
                x0 = x;  y0 = y;
                out0 = ComputeOutCode(x0, y0);
            }
            else {
                x1 = x;  y1 = y;
                out1 = ComputeOutCode(x1, y1);
            }
        }
    }

    if (accept) {
        cx0 = x0; cy0 = y0;
        cx1 = x1; cy1 = y1;
    }
    return accept;
}

// Draw the clipping window in green
static void DrawClippingWindow() {
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(xmin, ymin);
    glVertex2f(xmax, ymin);
    glVertex2f(xmax, ymax);
    glVertex2f(xmin, ymax);
    glEnd();
}

// Draw the viewport in yellow
static void DrawViewport() {
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(vxmin, vymin);
    glVertex2f(vxmax, vymin);
    glVertex2f(vxmax, vymax);
    glVertex2f(vxmin, vymax);
    glEnd();
}

// Render each frame
static void render() {
    glClear(GL_COLOR_BUFFER_BIT);

    // 1) Original line in red
    glColor3f(1.0f, 0.0f, 0.0f);
    DDA(x_start, y_start, x_end, y_end);

    // 2) Clipping window
    DrawClippingWindow();

    if (clipped) {
        // 3) Clipped segment in cyan
        glColor3f(0.0f, 1.0f, 1.0f);
        DDA(cx0, cy0, cx1, cy1);

        // 4) Viewport
        DrawViewport();

        // 5) Mapped-to-viewport line in magenta
        float sx = (vxmax - vxmin) / (xmax - xmin);
        float sy = (vymax - vymin) / (ymax - ymin);
        float vx0 = vxmin + (cx0 - xmin) * sx;
        float vy0 = vymin + (cy0 - ymin) * sy;
        float vx1 = vxmin + (cx1 - xmin) * sx;
        float vy1 = vymin + (cy1 - ymin) * sy;

        glColor3f(1.0f, 0.0f, 1.0f);
        DDA(vx0, vy0, vx1, vy1);
    }

    glFlush();
}

// GLFW key callback: on 'C' or 'c', perform clipping
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS && (key == GLFW_KEY_C)) {
        clipped = CohenSutherlandClip(x_start, y_start, x_end, y_end);
    }
}

int main(void) {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to init GLFW\n");
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(640, 480, "Cohen-Sutherland Clipping & Viewport (GLFW)", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    // Setup 2D orthographic projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 400.0, 0.0, 200.0, -1.0, 1.0);  // match your original gluOrtho2D(0,400,0,200)
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
