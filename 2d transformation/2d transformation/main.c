#include <GLFW/glfw3.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#define PI 3.14159265358979323846

float transX = 0, transY = 0;
float scaleX = 1, scaleY = 1;
float angle = 0;
bool reflectX = false, reflectY = false;
float shearX = 0, shearY = 0;

GLfloat square[4][2] = {
    { -50, -50 },
    {  50, -50 },
    {  50,  50 },
    { -50,  50 }
};

// Simple text rendering using GL lines (only shows capital letters and a few symbols)
void drawChar(float x, float y, char c, float size) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);
    glBegin(GL_LINES);
    switch (c) {
    case 'X': glVertex2f(0, 0); glVertex2f(1, 1); glVertex2f(1, 0); glVertex2f(0, 1); break;
    case 'Y': glVertex2f(0, 1); glVertex2f(0.5, 0.5); glVertex2f(1, 1); glVertex2f(0.5, 0.5); break;
    case 'O': glVertex2f(0, 0); glVertex2f(1, 0); glVertex2f(1, 0); glVertex2f(1, 1);
        glVertex2f(1, 1); glVertex2f(0, 1); glVertex2f(0, 1); glVertex2f(0, 0); break;
    case ':': glVertex2f(0.5, 0.7); glVertex2f(0.5, 0.7); glVertex2f(0.5, 0.3); glVertex2f(0.5, 0.3); break;
    case '.': glVertex2f(0.5, 0); glVertex2f(0.5, 0); break;
    case '1': glVertex2f(0.5, 0); glVertex2f(0.5, 1); break;
    case '0': glVertex2f(0, 0); glVertex2f(1, 0); glVertex2f(1, 0); glVertex2f(1, 1);
        glVertex2f(1, 1); glVertex2f(0, 1); glVertex2f(0, 1); glVertex2f(0, 0); break;
    case '(': glVertex2f(0.6, 0); glVertex2f(0.3, 0.5); glVertex2f(0.3, 0.5); glVertex2f(0.6, 1); break;
    case ')': glVertex2f(0.4, 0); glVertex2f(0.7, 0.5); glVertex2f(0.7, 0.5); glVertex2f(0.4, 1); break;
    case '-': glVertex2f(0, 0.5); glVertex2f(1, 0.5); break;
    default: break; // You can add more letters here
    }
    glEnd();
    glPopMatrix();
}

void drawText(float x, float y, const char* str, float size) {
    while (*str) {
        if (*str != ' ') drawChar(x, y, *str, size);
        x += size * 10;  // move right
        str++;
    }
}

void applyTransformations() {
    glTranslatef(transX, transY, 0);
    glRotatef(angle, 0, 0, 1);
    glScalef(scaleX * (reflectX ? -1 : 1), scaleY * (reflectY ? -1 : 1), 1);

    float shearMatrix[16] = {
        1, shearY, 0, 0,
        shearX, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    glMultMatrixf(shearMatrix);
}

void drawSquare() {
    glColor3f(1, 0, 0);
    glBegin(GL_QUADS);
    for (int i = 0; i < 4; ++i)
        glVertex2fv(square[i]);
    glEnd();
}

void drawOverlayText(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(0, 0, 0);

    char buffer[128];
    float x = 10, y = height - 20;

    snprintf(buffer, sizeof(buffer), "TRANS (%.0f, %.0f)", transX, transY);
    drawText(x, y, buffer, 1); y -= 20;

    snprintf(buffer, sizeof(buffer), "SCALE (%.2f, %.2f)", scaleX, scaleY);
    drawText(x, y, buffer, 1); y -= 20;

    snprintf(buffer, sizeof(buffer), "ROT %.0f DEG", angle);
    drawText(x, y, buffer, 1); y -= 20;

    snprintf(buffer, sizeof(buffer), "REFLECT X:%s Y:%s", reflectX ? "ON" : "OFF", reflectY ? "ON" : "OFF");
    drawText(x, y, buffer, 1); y -= 20;

    snprintf(buffer, sizeof(buffer), "SHEAR X:%.2f Y:%.2f", shearX, shearY);
    drawText(x, y, buffer, 1);
}

void key_callback(GLFWwindow* w, int key, int sc, int action, int mods) {
    if (action != GLFW_PRESS) return;
    const float delta = 10.0f, scaleStep = 0.1f, angleStep = 5.0f, shearStep = 0.1f;

    switch (key) {
    case GLFW_KEY_LEFT:  transX -= delta; break;
    case GLFW_KEY_RIGHT: transX += delta; break;
    case GLFW_KEY_UP:    transY += delta; break;
    case GLFW_KEY_DOWN:  transY -= delta; break;
    case GLFW_KEY_W: scaleX += scaleStep; scaleY += scaleStep; break;
    case GLFW_KEY_S: scaleX = fmaxf(0.1f, scaleX - scaleStep);
        scaleY = fmaxf(0.1f, scaleY - scaleStep); break;
    case GLFW_KEY_A: angle += angleStep; break;
    case GLFW_KEY_D: angle -= angleStep; break;
    case GLFW_KEY_R: reflectX = !reflectX; break;
    case GLFW_KEY_T: reflectY = !reflectY; break;
    case GLFW_KEY_J: shearX -= shearStep; break;
    case GLFW_KEY_K: shearX += shearStep; break;
    case GLFW_KEY_U: shearY += shearStep; break;
    case GLFW_KEY_I: shearY -= shearStep; break;
    case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(w, 1); break;
    }
}

int main() {
    if (!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(600, 600, "2D Transformations", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glClearColor(1, 1, 1, 1);

    while (!glfwWindowShouldClose(window)) {
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        glViewport(0, 0, w, h);
        glClear(GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-300, 300, -300, 300, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        applyTransformations();
        drawSquare();

        drawOverlayText(w, h);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
