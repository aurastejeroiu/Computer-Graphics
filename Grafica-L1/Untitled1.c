#include <GL/glut.h>
#include<GLFW/glfw3.h>
#include <glad/glad.h>
void set_pixel(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

 

void draw_circle(int x0, int y0, int radius) {
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;

 

    set_pixel(x0, y0 + radius);
    set_pixel(x0, y0 - radius);
    set_pixel(x0 + radius, y0);
    set_pixel(x0 - radius, y0);

 

    while (x <= y) {
        if (d < 0) {
            d = d + 4 * x + 6;
        } else {
            d = d + 4 * (x - y) + 10;
            y--;
        }
        x++;

 

        set_pixel(x0 + x, y0 + y);
        set_pixel(x0 - x, y0 + y);
        set_pixel(x0 + x, y0 - y);
        set_pixel(x0 - x, y0 - y);
        set_pixel(x0 + y, y0 + x);
        set_pixel(x0 - y, y0 + x);
        set_pixel(x0 + y, y0 - x);
        set_pixel(x0 - y, y0 - x);
    }
}

 

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 1.0);
    glPointSize(2.0);
    draw_circle(250, 250, 100);
    glFlush();
}

 

void init() {
    glClearColor(1.0, 1.0, 1.0,0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 500.0, 0.0, 500.0);
}

 

int main1(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Bresenham's Drawing Algorithm");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
