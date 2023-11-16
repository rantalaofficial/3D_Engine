#include <GLUT/glut.h>
#include <math.h>
#include <iostream>
#include <vector>

/*

g++ -std=c++11 *.cpp -o engine -framework OpenGL -framework GLUT && ./engine
*/

// Camera parameters
GLfloat cameraSpeed = 0.05f;
GLfloat cameraPosX = 0.0f, cameraPosY = 0.0f, cameraPosZ = 3.0f;
GLfloat cameraTargetX = 0.0f, cameraTargetY = 0.0f, cameraTargetZ = 0.0f;

GLfloat cameraYaw = 0.0f, cameraPitch = 0.0f;

std::vector< std::vector<GLfloat> > vertices = 
{
    {-0.5f, -0.5f, 0.5f},
    {0.5f, -0.5f, 0.5f},
    {0.5f, 0.5f, 0.5f},
    {-0.5f, 0.5f, 0.5f},
    // Back face
    {-0.5f, -0.5f, -0.5f},
    {0.5f, -0.5f, -0.5f},
    {0.5f, 0.5f, -0.5f},
    {-0.5f, 0.5f, -0.5f},
    // Left face
    {-0.5f, -0.5f, 0.5f},
    {-0.5f, 0.5f, 0.5f},
    {-0.5f, 0.5f, -0.5f},
    {-0.5f, -0.5f, -0.5f},
    // Right face
    {0.5f, -0.5f, 0.5f},
    {0.5f, 0.5f, 0.5f},
    {0.5f, 0.5f, -0.5f},
    {0.5f, -0.5f, -0.5f},
    // Top face
    {-0.5f, 0.5f, 0.5f},
    {0.5f, 0.5f, 0.5f},
    {0.5f, 0.5f, -0.5f},
    {-0.5f, 0.5f, -0.5f},
    // Bottom face
    {-0.5f, -0.5f, 0.5f},
    {0.5f, -0.5f, 0.5f},
    {0.5f, -0.5f, -0.5f},
    {-0.5f, -0.5f, -0.5f},
};

std::unordered_map<char, bool> keyStates;
std::unordered_map<char, bool> specialKeyStates;

void keyboardDown(unsigned char key, int x, int y) {
    keyStates[key] = true;
}

void keyboardUp(unsigned char key, int x, int y) {
    keyStates[key] = false;
}

void specialKeys(int key, int x, int y) {
    specialKeyStates[key] = true;
}

void specialKeysUp(int key, int x, int y) {
    specialKeyStates[key] = false;
}

void drawBox() {
    glBegin(GL_LINE_LOOP);
    //glBegin(GL_QUADS);
    // Front face

    glColor3f(1.0f, 0.0f, 0.0f);

    for (int i = 0; i < vertices.size(); i++) {
        glVertex3f(vertices[i][0], vertices[i][1], vertices[i][2]);

    }

    glEnd();

}

void display() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    gluLookAt(cameraPosX, cameraPosY, cameraPosZ, cameraTargetX, cameraTargetY, cameraTargetZ, 0, 1, 0);

    drawBox();


    // print camera target
    std::cout << "Camera target: " << cameraTargetX << ", " << cameraTargetY << ", " << cameraTargetZ << std::endl;
    // print camera position
    std::cout << "Camera position: " << cameraPosX << ", " << cameraPosY << ", " << cameraPosZ << std::endl;





    glutSwapBuffers();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)width / (float)height, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void updateCameraPosition() {
    GLfloat changeX = 0.0f, changeZ = 0.0f;

    // Camera movement
    if (keyStates['w']) {
        changeX += sin(cameraYaw) * cameraSpeed;
        changeZ += -cos(cameraYaw) * cameraSpeed;
    } else if (keyStates['s']) {
        changeX += -sin(cameraYaw) * cameraSpeed;
        changeZ += cos(cameraYaw) * cameraSpeed;
    }

    if (keyStates['a']) {
        changeX += sin(cameraYaw - 1.570796) * cameraSpeed;
        changeZ += -cos(cameraYaw - 1.570796) * cameraSpeed;
    } else if (keyStates['d']) {
        changeX += sin(cameraYaw + 1.570796) * cameraSpeed;
        changeZ += -cos(cameraYaw + 1.570796) * cameraSpeed;
    }
    
    cameraPosX += changeX;
    cameraPosZ += changeZ;
    cameraTargetX += changeX;
    cameraTargetZ += changeZ;

    // Camera rotation
    if (specialKeyStates[GLUT_KEY_UP]) {
        cameraPitch += cameraSpeed;
    } else if (specialKeyStates[GLUT_KEY_DOWN]) {
        cameraPitch -= cameraSpeed;
    }

    if (specialKeyStates[GLUT_KEY_LEFT]) {
        cameraYaw -= cameraSpeed;
    } else if (specialKeyStates[GLUT_KEY_RIGHT]) {
        cameraYaw += cameraSpeed;
    }

    cameraTargetX = cameraPosX + sin(cameraYaw);
    cameraTargetZ = cameraPosZ - cos(cameraYaw);
    cameraTargetY = cameraPosY + sin(cameraPitch);
}

void update(int value) {
    updateCameraPosition();
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);  // 60 frames per second
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1200, 800);
    glutCreateWindow("MatRan 3D Engine");

    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF); // Disable key repeat

    glEnable(GL_DEPTH_TEST);


    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    //glutKeyboardFunc(keyboard);
    //glutSpecialFunc(specialKeys);

    glutKeyboardFunc(keyboardDown);
    glutKeyboardUpFunc(keyboardUp);

    glutSpecialFunc(specialKeys);
    glutSpecialUpFunc(specialKeysUp);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    glutTimerFunc(0, update, 0);

    glutMainLoop();

    

    return 0;
}