#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<GL/gl.h>
#include<GL/glut.h>
#include<math.h>

#define PI 3.1415926535
#define g 9.81
#define WINDOW_HEIGHT 720
#define WINDOW_WIDTH 1280

double t = 0;
double dt = 0.001;

bool simON = false;
double sim_time = 3.0;

double f = 0.0;
double x[4] = {0,0,0,0};
double dx[4] = {0,0,0,0};
double A[4][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
double B[4][1] = {{0},{0},{0},{0}};
double C[4][1] = {{0},{0},{0},{0}};

void Init_pendulum(void);
void Init_cart(void);
void resize(int,int);
void display_interted_pendulum(void);
void idle(void);
void init(void);
double deg2rad(double);
void mouse(int,int,int,int);
void keyboard(unsigned char, int, int);
void model_matrix(void);

struct State
{
    double x, y;
    double x_v,y_v;
    double x_dot, y_dot;
    double ax, ay;
    double theta;
    double theta_dot;
    double theta_2dot;
    double fx, fy;
    double px, py;
};

struct Cart
{
    struct State state;
    double m;
    double h, w;
    double mu;
};

struct Pendulum
{
    struct State pivot;  
    double m;
    double l;
    double j;
    double mu;
};

struct Pendulum pendulum;
struct Cart cart;

void state_equation(void)
{
    x[0] = cart.state.x;
    x[1] = pendulum.pivot.theta;
    x[2] = cart.state.x_dot;
    x[3] = pendulum.pivot.theta_dot;

    double p = x[0];
    double theta = x[1];
    double dp = x[2];
    double dtheta = x[3];

    dx[0] = dp;
    dx[1] = dtheta;
    dx[2] = (-pendulum.l*pendulum.m*sin(pendulum.pivot.theta)*pendulum.pivot.theta_dot*pendulum.pivot.theta_dot+g*pendulum.m*sin(pendulum.pivot.theta)*cos(pendulum.pivot.theta)+f)/(cart.m+pendulum.m*sin(pendulum.pivot.theta)*sin(pendulum.pivot.theta));
    dx[3] = (g*(cart.m+pendulum.m)*sin(pendulum.pivot.theta)-((pendulum.l*pendulum.m*sin(pendulum.pivot.theta)*pendulum.pivot.theta_2dot*pendulum.pivot.theta_2dot-f)*cos(pendulum.pivot.theta)))/(pendulum.l*(cart.m+pendulum.m*sin(pendulum.pivot.theta)*sin(pendulum.pivot.theta)));

    cart.state.x = p + dx[0] * dt;
    cart.state.x_dot = dp + dx[1] * dt;
    pendulum.pivot.theta = theta + dx[2] * dt;
    pendulum.pivot.theta_dot = dtheta + dx[3] * dt;
}

void display(void){
    glClear(GL_COLOR_BUFFER_BIT);
    state_equation();
    display_interted_pendulum();
    glutSwapBuffers();
    t = t + dt;
}

int main(int argc, char *argv[]){
    glutInitWindowPosition(0,0);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutCreateWindow(argv[0]);
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    init();
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}

void display_interted_pendulum(){
    glBegin(GL_LINE_LOOP);
    glVertex2d(cart.state.x - cart.w/2, cart.state.y + cart.h/2);
    glVertex2d(cart.state.x + cart.w/2, cart.state.y + cart.h/2);
    glVertex2d(cart.state.x + cart.w/2, cart.state.y - cart.h/2);
    glVertex2d(cart.state.x - cart.w/2, cart.state.y - cart.h/2);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2d(cart.state.x + pendulum.pivot.x,pendulum.pivot.y);
    glVertex2d(cart.state.x + pendulum.pivot.x + pendulum.l * sin(pendulum.pivot.theta),  pendulum.pivot.y + pendulum.l * cos(pendulum.pivot.theta));
    glEnd();
}

void init(void){
    glClearColor(0.0,0.0,1.0,1.0);
    Init_pendulum();
    Init_cart();
    model_matrix();
}

void Init_pendulum(){
    pendulum.pivot.x = 0.0;
    pendulum.pivot.y = 0.0;

    pendulum.pivot.theta = deg2rad(90);

    pendulum.l = 2.0;
    pendulum.m = 0.5;
    pendulum.mu = 0.1;
}

void Init_cart(){
    cart.state.x = 0.0;
    cart.state.y = 0.0;
    cart.w = 1.0;
    cart.h = 0.5;
    cart.m = 15.0;
    cart.mu = 0.2;
}

void model_matrix(void){
    A[0][0] = 0;
    A[0][1] = 0;
    A[0][2] = 1;
    A[0][3] = 0;

    A[1][0] = 0;
    A[1][1] = 0;
    A[1][2] = 0;
    A[1][3] = 1;

    A[2][0] = 0;
    A[2][1] = g*pendulum.m/cart.m;
    A[2][2] = 0;
    A[2][3] = 0;

    A[3][0] = 0;
    A[3][1] = g*(pendulum.m+cart.m)/(pendulum.l*cart.m);
    A[3][2] = 0;
    A[3][3] = 0;

    B[0][0] = 0;
    B[0][1] = 0;
    B[0][2] = 1/cart.m;
    B[0][3] = 1/(pendulum.l*cart.m);
}

void resize(int w, int h){
    glViewport(0, 0, w, h);
    glLoadIdentity();
    glOrtho(-w/200.0, w/200.0, -h/200.0, h/200.0, -1.0, 1.0);
}

void idle(void){
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    switch (button) {
        case GLUT_LEFT_BUTTON:

            if (state == GLUT_DOWN) {
            }else{
            }
            break;

        case GLUT_RIGHT_BUTTON:

            if (state == GLUT_DOWN) {
                /* コマ送り (1ステップだけ進める) */
                glutPostRedisplay();
            }
            break;

        default:
            break;
    }
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 's': simON = true; break;
        case 'd': simON = false; break;
        case 'q':
        case 'Q':
        case '\033': exit(0);
    default:
        break;
  }
}

double deg2rad(double degree){
    return degree * (PI/180);
}