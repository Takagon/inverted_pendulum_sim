#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<GL/gl.h>
#include<GL/glut.h>
#include<math.h>

#define PI 3.1415926535
#define g 8
#define WINDOW_HEIGHT 720
#define WINDOW_WIDTH 1280


bool simON = false;
double f = 0.0;


double deg2rad(double);
void Init_pendulum(void);
void Init_cart(void);
void resize(int,int);
void display_interted_pendulum(void);
void idle(void);
void init(void);

struct State
{
    double x, y;
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

void state_update(void)
{
    // double a = (-(1/4)*pendulum.m*g)/((1/3)*(cart.m + pendulum.m) - (1/4)*pendulum.m);
    // double b = (-(1/3)*(cart.m + pendulum.m)*g)/((1/4)*pendulum.m*pendulum.l - (1/3)*(pendulum.m + cart.m)*pendulum.l);
    // double c = (1/3)/((1/3)*(pendulum.m + cart.m) - (1/4)*pendulum.m);
    // double d = (1/2)/((1/4)*pendulum.m*pendulum.l - (1/3)*(pendulum.m + cart.m)*pendulum.l);

    // double A[4][4] = {{0,1,0,0},
    //                   {0,0,a,0},
    //                   {0,0,0,1},
    //                   {0,0,b,0}};

    // double B[4] = {0,c,0,d};
    // double s = sin(deg2rad(pendulum.pivot.theta));
    // double c = cos(deg2rad(pendulum.pivot.theta));
    // cart.state.ax = ((-1/4)*pendulum.m*s*c*cart.state.x_dot*)/();
}

double t = 0;
double dt = 0.001;

void display(void){
    glClear(GL_COLOR_BUFFER_BIT);
    state_update();
    display_interted_pendulum();
    glutSwapBuffers();

    double s = sin(pendulum.pivot.theta);
    double c = cos(pendulum.pivot.theta);
    
    pendulum.j = (1/3)*pendulum.m*pendulum.l*pendulum.l;

    double ax = (cart.m*pendulum.l*pendulum.pivot.theta_dot*pendulum.pivot.theta_dot*s - pendulum.m*pendulum.l*pendulum.pivot.theta_2dot*c - cart.mu*cart.state.x_dot + f)/(cart.m + pendulum.m);
    double at = (pendulum.m*pendulum.l*g*s - pendulum.m*pendulum.l*c*cart.state.ax - pendulum.mu*pendulum.pivot.theta_dot)/(pendulum.j + pendulum.m*pendulum.l*pendulum.l);

    cart.state.ax = ax;
    pendulum.pivot.theta_2dot = at;

    double tmp1 = cart.state.x_dot + cart.state.ax * dt;
    double tmp2 = cart.state.x + cart.state.x_dot * dt;
    
    double tmp3 = pendulum.pivot.theta_dot + pendulum.pivot.theta_2dot * dt;
    double tmp4 = pendulum.pivot.theta + pendulum.pivot.theta_dot * dt;

    cart.state.x_dot = tmp1;
    cart.state.x = tmp2;
    pendulum.pivot.theta_dot = tmp3; 
    pendulum.pivot.theta = tmp4;

    t = t + dt;
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
        case 'q':
        case 'Q':
        case '\033': exit(0);
    default:
        break;
  }
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
}

void Init_pendulum(){
    pendulum.pivot.x = 0.0;
    pendulum.pivot.y = 0.0;
    pendulum.pivot.theta = 90;
    pendulum.l = 2.0;
    pendulum.m = 1.0;
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

void resize(int w, int h){
    glViewport(0, 0, w, h);
    glLoadIdentity();
    glOrtho(-w/200.0, w/200.0, -h/200.0, h/200.0, -1.0, 1.0);
}

void idle(void){
    glutPostRedisplay();
}
