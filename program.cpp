#include <GLUT/glut.h>
// #include <GL/glut.h>
#include "tinyxml/tinyxml.h"

void display(void) {
    /* Limpar todos os pixels */
    glClear (GL_COLOR_BUFFER_BIT);
    /* Desenhar um polígono branco (retângulo) */
    glColor3f (1.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(0.25, 0.25, 0.0);
    glVertex3f(0.75, 0.25, 0.0);
    glVertex3f(0.75, 0.75, 0.0);
    glVertex3f(0.25, 0.75, 0.0);
    glEnd();
    /* Não esperar! */
    glFlush ();
}

void init (void) {
    /* selecionar cor de fundo (preto) */
    glClearColor (0.0, 0.0, 0.0, 0.0);
    /* inicializar sistema de viz. */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize (250, 250);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("hello");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    /* C ANSI requer que main retorne um inteiro */
    return 0;
}