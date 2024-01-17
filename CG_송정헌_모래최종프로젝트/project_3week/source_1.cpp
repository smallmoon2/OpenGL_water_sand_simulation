#include<iostream>
#include <Windows.h>
#include<GL/glew.h>
#include<GL/glut.h>
#include "water.h"
#include "sand.h"
#include <random>
#include "Bitmap.h"
#include "mmsystem.h"
#pragma comment(lib,"winmm")
#include "Digitalv.h"
#define _WINDOW_WIDTH 800
#define _WINDOW_HEIGHT 800

float _S = 0.3;
int Knum = 1;
int clubnum = 0;
int clubnum2 = 0;
sand sandz[1000];
water waterz[1000];
int ck = 0;
int mv = 0;
int one = 0;
int under = 0;
int Dq[4] ;
int WMM = 0;


//@@@@@@@@@@@@@@@@@@@@@@@@@ cubemap
void Init(const char* filename)
{
    Bitmap texture;
    texture.open(filename);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.getWidth(), texture.getHeight(), 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, texture._image);

#ifdef USE_MIPMAP
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture.getWidth(), texture.getHeight(), GL_BGR_EXT, GL_UNSIGNED_BYTE, texture._image);
#endif

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);	// GL_REPEAT
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);	// GL_REPEAT
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

#ifdef USE_MIPMAP
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
#endif
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);	//GL_DECAL, GL_REPLACE, GL_MODULATE

#ifdef USE_MIPMAP
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);	// GL_REPEAT
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	// GL_REPEAT
#endif

    glColor3f(1.0, 1.0, 0.0);

    glEnable(GL_TEXTURE_2D);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40, 1, 0.1, 100);

    glClearColor(0.0, 0.0, 0.0, 0.0);
}



//@@@@@@@@@@@@@@@@@@@@@@@@@@
int prevPos[2] = { 0, 0 };
unsigned char mouseState[3] = { 0 };

float zoom = 15.0;
float rot[2] = { 0.0f, 0.001f }; // x,y축에 대한 회전
float trans[2] = { 0.0f, 0.0f }; // 이동
//각 정점마다 색깔

void Draw(void)
{
    glEnable(GL_DEPTH_TEST); // 깊이맵을 사용!
    glEnable(GL_LIGHTING); // 조명을 쓸겁니다.
    glEnable(GL_LIGHT0); // 첫번째 조명을 적용할거예요.
    glDisable(GL_LIGHTING); // 조명 끌거예요.
}

void CameraTransform(void)
{
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -zoom);
    glTranslatef(trans[0], trans[1], 0.0);
    glRotatef(rot[0], 1, 0, 0);
    glRotatef(rot[1], 0, 1, 0);
}

void Motion(int x, int y)
{
    int dx = x - prevPos[0];
    int dy = y - prevPos[1];
    prevPos[0] = x;
    prevPos[1] = y;

    if (mouseState[2]) {
        zoom -= (float)0.05f * dx;
    }
    else if (mouseState[0]) {
        rot[0] += (float)0.5 * dy;
        rot[1] += (float)0.5 * dx;
    }
    else if (mouseState[1]) {
        trans[0] += (float)0.05 * dx;
        trans[1] -= (float)0.05 * dy;
    }
    glutPostRedisplay();
}

void Mouse(int button, int state, int x, int y)
{
    prevPos[0] = x;
    prevPos[1] = y;
    switch (button) {
    case GLUT_LEFT_BUTTON:
        mouseState[0] = (state == GLUT_DOWN ? 1 : 0);
        break;
    case GLUT_MIDDLE_BUTTON:
        mouseState[1] = (state == GLUT_DOWN ? 1 : 0);
        break;
    case GLUT_RIGHT_BUTTON:
        mouseState[2] = (state == GLUT_DOWN ? 1 : 0);
        break;
    }
    glutPostRedisplay();
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@
void cubepoint(GLfloat size, GLfloat x, GLfloat y, GLfloat z) {
    GLfloat myCube[8][3];

    //정육면체의 8개의 정점 3차원 좌표(꼭지점)
    //순서대로 0부터 7번의 번호가 매겨질 것이다.(밑에서)

    GLfloat MyVertices[8][3] = { {-0.25,-0.25,0.25},{-0.25,0.25,0.25},{0.25,0.25,0.25},
    {0.25,-0.25,0.25},{-0.25,-0.25,-0.25},{-0.25,0.25,-0.25},
    {0.25,0.25,-0.25},{0.25,-0.25,-0.25} };
    //정육면체의 8개의 정점 3차원 좌표(꼭지점)
    //순서대로 0부터 7번의 번호가 매겨질 것이다.(밑에서)

    GLfloat MyColor[8][3] = { {0.2,0.2,0.2},{1.0,0.0,0.0},{1.0,1.0,0.0},
    {0.0,1.0,0.0},{0.0,0.0,1.0},{1.0,0.0,1.0},
    {1.0,1.0,1.0},{0.0,1.0,1.0} };

    for (int i = 0; i < 8; i++) {
        MyVertices[i][0] = size * MyVertices[i][0] + x;
        MyVertices[i][1] = size * MyVertices[i][1] + y;
        MyVertices[i][2] = size * MyVertices[i][2] + z;
    }
    int VertexList[6][4] = { {0,3,2,1},{2,3,7,6},{7,4,5,6},{1,2,6,5},{0,3,7,4},{0,4,5,1} };
    //각 정점에 번호를 매기고 그 번호에 따라 면을 지정함

    for (int i = 0; i < 6; i++) {
        int index0 = VertexList[i][0];//6번 돌면서 각 정점 번호에 모두 접근하여 값을 할당한다.
        int index1 = VertexList[i][1];
        int index2 = VertexList[i][2];
        int index3 = VertexList[i][3];

        glBegin(GL_POLYGON);
        //정점 번호가 0번이면 {-0.25,-0.25,0.25}가 들어갈 것이다.
        //이렇게 모든 정점 번호들을 할당시켜준다.

        glColor3f(1.0f, 0.0f, 0.0f); glVertex3fv(MyVertices[index0]);
        glColor3f(1.0f, 0.0f, 0.0f); glVertex3fv(MyVertices[index1]);
        glColor3f(1.0f, 0.0f, 0.0f); glVertex3fv(MyVertices[index2]);
        glColor3f(1.0f, 0.0f, 0.0f); glVertex3fv(MyVertices[index3]);

        glEnd();
    }
}
void mycube2(GLfloat size, GLfloat x, GLfloat y, GLfloat z) {

    GLfloat myCube[8][3];

    //정육면체의 8개의 정점 3차원 좌표(꼭지점)
    //순서대로 0부터 7번의 번호가 매겨질 것이다.(밑에서)

    GLfloat MyVertices[8][3] = { {-0.25,-0.25,0.25},{-0.25,0.25,0.25},{0.25,0.25,0.25},
    {0.25,-0.25,0.25},{-0.25,-0.25,-0.25},{-0.25,0.25,-0.25},
    {0.25,0.25,-0.25},{0.25,-0.25,-0.25} };
    //정육면체의 8개의 정점 3차원 좌표(꼭지점)
    //순서대로 0부터 7번의 번호가 매겨질 것이다.(밑에서)

    GLfloat MyColor[8][3] = { {0.2,0.2,0.2},{1.0,0.0,0.0},{1.0,1.0,0.0},
    {0.0,1.0,0.0},{0.0,0.0,1.0},{1.0,0.0,1.0},
    {1.0,1.0,1.0},{0.0,1.0,1.0} };

    for (int i = 0; i < 8; i++) {
        MyVertices[i][0] = size * MyVertices[i][0] + x;
        MyVertices[i][1] = size * MyVertices[i][1] + y;
        MyVertices[i][2] = size * MyVertices[i][2] + z;
    }
    int VertexList[6][4] = { {0,3,2,1},{2,3,7,6},{7,4,5,6},{1,2,6,5},{0,3,7,4},{0,4,5,1} };
    //각 정점에 번호를 매기고 그 번호에 따라 면을 지정함

    for (int i = 0; i < 6; i++) {
        int index0 = VertexList[i][0];//6번 돌면서 각 정점 번호에 모두 접근하여 값을 할당한다.
        int index1 = VertexList[i][1];
        int index2 = VertexList[i][2];
        int index3 = VertexList[i][3];

        glBegin(GL_POLYGON);
        //정점 번호가 0번이면 {-0.25,-0.25,0.25}가 들어갈 것이다.
        //이렇게 모든 정점 번호들을 할당시켜준다.

        glColor3f(0.1f, 0.1f, 0.3f); glVertex3fv(MyVertices[index0]);
        glColor3f(0.1f, 0.1f, 0.3f); glVertex3fv(MyVertices[index1]);
        glColor3f(0.1f, 0.1f, 0.3f); glVertex3fv(MyVertices[index2]);
        glColor3f(0.1f, 0.1f, 0.3f); glVertex3fv(MyVertices[index3]);

        glEnd();
    }
}
void mycube(GLfloat size, GLfloat x, GLfloat y, GLfloat z) {
    GLfloat myCube[8][3];

    //정육면체의 8개의 정점 3차원 좌표(꼭지점)
    //순서대로 0부터 7번의 번호가 매겨질 것이다.(밑에서)

    GLfloat MyVertices[8][3] = { {-0.25,-0.25,0.25},{-0.25,0.25,0.25},{0.25,0.25,0.25},
    {0.25,-0.25,0.25},{-0.25,-0.25,-0.25},{-0.25,0.25,-0.25},
    {0.25,0.25,-0.25},{0.25,-0.25,-0.25} };
    //정육면체의 8개의 정점 3차원 좌표(꼭지점)
    //순서대로 0부터 7번의 번호가 매겨질 것이다.(밑에서)

    GLfloat MyColor[8][3] = { {0.2,0.2,0.2},{1.0,0.0,0.0},{1.0,1.0,0.0},
    {0.0,1.0,0.0},{0.0,0.0,1.0},{1.0,0.0,1.0},
    {1.0,1.0,1.0},{0.0,1.0,1.0} };

    for (int i = 0; i < 8; i++) {
        MyVertices[i][0] = size * MyVertices[i][0] + x;
        MyVertices[i][1] = size * MyVertices[i][1] + y;
        MyVertices[i][2] = size * MyVertices[i][2] + z;
    }
    int VertexList[6][4] = { {0,3,2,1},{2,3,7,6},{7,4,5,6},{1,2,6,5},{0,3,7,4},{0,4,5,1} };
    //각 정점에 번호를 매기고 그 번호에 따라 면을 지정함

    for (int i = 0; i < 6; i++) {
        int index0 = VertexList[i][0];//6번 돌면서 각 정점 번호에 모두 접근하여 값을 할당한다.
        int index1 = VertexList[i][1];
        int index2 = VertexList[i][2];
        int index3 = VertexList[i][3];

        glBegin(GL_POLYGON);
        //정점 번호가 0번이면 {-0.25,-0.25,0.25}가 들어갈 것이다.
        //이렇게 모든 정점 번호들을 할당시켜준다.

        //glColor3f(1.0f, 0.7f, 0.6f); glVertex3fv(MyVertices[index0]);
        //glColor3f(1.0f, 0.7f, 0.5f); glVertex3fv(MyVertices[index1]);
        //glColor3f(1.0f, 0.7f, 0.4f); glVertex3fv(MyVertices[index2]);
        //glColor3f(1.0f, 0.7f, 0.3f); glVertex3fv(MyVertices[index3]);

        glTexCoord2f(0.0, 0.0); glVertex3fv(MyVertices[index0]);
        glTexCoord2f(0.0, 1.0); glVertex3fv(MyVertices[index1]);
        glTexCoord2f(1.0, 1.0); glVertex3fv(MyVertices[index2]);
        glTexCoord2f(1.0, 0.0); glVertex3fv(MyVertices[index3]);

        glEnd();
    }
}
void MyDisplay() {

    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();
    CameraTransform();
    
    Init("sand.bmp");
    glBegin(GL_POLYGON);

    glTexCoord2f(0.0, 0.0); glVertex3f(-19.5f, -0.5f, 19.5f);
    glTexCoord2f(0.0, 1.0); glVertex3f(-19.5f, -0.5f, -19.5f);
    glTexCoord2f(1.0, 1.0); glVertex3f(19.5f, -0.5f, -19.5f);
    glTexCoord2f(1.0, 0.0); glVertex3f(19.5f, -0.5f, 19.5f);

    glEnd();



    Init("sea2.bmp");
    glBegin(GL_POLYGON);

    glTexCoord2f(0.0, 0.0); glVertex3f(-19.5f, -0.5f, -19.5f);
    glTexCoord2f(0.0, 1.0); glVertex3f(-19.5f, 19.5f, -19.5f);
    glTexCoord2f(1.0, 1.0); glVertex3f(19.5f, 19.5f, -19.5f);
    glTexCoord2f(1.0, 0.0); glVertex3f(19.5f, -0.5f, -19.5f);

    glEnd();


    glBegin(GL_POLYGON);

    glTexCoord2f(0.0, 0.0); glVertex3f(-19.5f, -0.5f, 19.5f);
    glTexCoord2f(0.0, 1.0); glVertex3f(-19.5f, 19.5f, 19.5f);
    glTexCoord2f(1.0, 1.0); glVertex3f(19.5f, 19.5f, 19.5f);
    glTexCoord2f(1.0, 0.0); glVertex3f(19.5f, -0.5f, 19.5f);

    glEnd();


    glBegin(GL_POLYGON);

    glTexCoord2f(0.0, 0.0); glVertex3f(19.5f, -0.5f, 19.5f);
    glTexCoord2f(0.0, 1.0); glVertex3f(19.5f, 19.5f, 19.5f);
    glTexCoord2f(1.0, 1.0); glVertex3f(19.5f, 19.5f, -19.5f);
    glTexCoord2f(1.0, 0.0); glVertex3f(19.5f, -0.5f, -19.5f);

    glEnd();


    glBegin(GL_POLYGON);

    glTexCoord2f(0.0, 0.0); glVertex3f(-19.5f, -0.5f, 19.5f);
    glTexCoord2f(0.0, 1.0); glVertex3f(-19.5f, 19.5f, 19.5f);
    glTexCoord2f(1.0, 1.0); glVertex3f(-19.5f,19.5f, -19.5f);
    glTexCoord2f(1.0, 0.0); glVertex3f(-19.5f, -0.5f, -19.5f);

    glEnd();


    //glBegin(GL_POLYGON);
    //glColor3f(0.0f, 0.7f, 0.9f);
    //glVertex3f(-9.5f, -0.5f, 9.5f);
    //glVertex3f(-9.5f, -0.5f, -9.5f);
    //glVertex3f(-9.5f, 9.5f, -9.5f);
    //glVertex3f(-9.5f, 9.5f, 9.5f);
    //glEnd();

    Init("물색.bmp");

    glBegin(GL_POLYGON);


    for (int i = 0; i < clubnum2; i++) {
        mycube2(_S, waterz[i]._x, waterz[i]._y, waterz[i]._z);
    }
    glEnd();

    Init("모래.bmp");

    glBegin(GL_POLYGON);
    for (int i = 0; i < clubnum; i++) {
        mycube(_S, sandz[i]._x, sandz[i]._y, sandz[i]._z);
    }


    cubepoint(_S, sandz[clubnum]._x, sandz[clubnum]._y, sandz[clubnum]._z);

    glEnd();



    glutSwapBuffers();

}
void undermove(int n) {
    if (sandz[n - 1]._y > -0.37f) {
        ck = 0;
        //for (int i = 0; i < n - 1; i++) {
        //    if (sandz[n - 1]._x + 0.15f == sandz[i]._x && sandz[n - 1]._y - 0.15f == sandz[i]._y && sandz[n - 1]._z == sandz[i]._z) {
        //        ck = 6;
        //        mv = 0;

        //        break;
        //    }
        //    else {
        //        ck = 1;
        //        mv = 1;
        //    }
        //}
        //if (mv == 0) {
        //    for (int i = 0; i < n - 1; i++) {
        //        if (sandz[n - 1]._x - 0.15f == sandz[i]._x && sandz[n - 1]._y - 0.15f == sandz[i]._y && sandz[n - 1]._z == sandz[i]._z) {
        //            ck = 6;
        //            mv = 0;
        //            break;
        //        }
        //        else {
        //            ck = 2;
        //            mv = 1;
        //        }
        //    }
        //}

        //if (mv == 0) {
        //    for (int i = 0; i < n - 1; i++) {
        //        if (sandz[n - 1]._x == sandz[i]._x && sandz[n - 1]._y - 0.15f == sandz[i]._y && sandz[n - 1]._z + 0.15f == sandz[i]._z) {
        //            ck = 6;
        //            mv = 0;
        //            break;
        //        }
        //        else {
        //            ck = 3;
        //            mv = 1;
        //        }
        //    }
        //}

        //if (mv == 0) {
        //    for (int i = 0; i < n - 1; i++) {
        //        if (sandz[n - 1]._x == sandz[i]._x && sandz[n - 1]._y - 0.15f == sandz[i]._y && sandz[n - 1]._z - 0.15f == sandz[i]._z) {
        //            ck = 6;
        //            mv = 0;

        //            break;
        //        }   
        //        else {
        //            ck = 4;
        //            mv = 1;
        //        }
        //    }
        //}

        sandz[n - 1]._y = sandz[n - 1]._y - 0.15f;
        


        mv = 0;

    }
}
void undermove2(int n) {
    if (waterz[n - 1]._y > -0.37f) {
        ck = 0;

        waterz[n - 1]._y = waterz[n - 1]._y - 0.15f;
        mv = 0;

    }
}
void MyReshape(int width, int height) {
    glViewport(0, 0, width, height);
    GLfloat f_w = (GLfloat)width / (GLfloat)_WINDOW_WIDTH;
    GLfloat f_h = (GLfloat)height / (GLfloat)_WINDOW_HEIGHT;
    glMatrixMode(GL_PROJECTION);


    if (width == 0) {
        height = 1;
    }
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)width / height, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //보는 각도 조절
    //위치를 약간 오른쪽으로 틀고 보고자 하는 객체를 바라보도록(0,0,0)지정
    //카메라의 머리는 y축방향으로 위쪽을 보도록함. 일반적으로 (0,1,0)을 씀.(0,1,0)이 아니면 카메라를
    //돌려서 사용하는거나 마찬가지니까 그럴일이 별로 없으니..
    //gluPerspective로 구현할 수 있지만 이 내용은 나중에 하므로
    //일단 gluLookAt으로 대용함
}
void watermove2() {
    for (int i = 0; i < clubnum2; i++) {
        if (waterz[i]._y > -0.44f && waterz[i].crash == 0) {
            waterz[i]._y = waterz[i]._y - 0.03f;
        }
    }
    for (int n = 1; n < clubnum2 + 1; n++) {
        for (int j = 0; j < clubnum2; j++) {
            if (n - 1 != j) {
                if (waterz[n - 1]._x == waterz[j]._x && waterz[n - 1]._y == waterz[j]._y && waterz[n - 1]._z == waterz[j]._z) {
                    printf("gd");
                    waterz[n - 1].crash = 1;
                    //************************************************************************************

                    //************************************************************************************
                    ck = 0;
                    for (int i = 0; i < clubnum - 1; i++) {
                        if (waterz[n - 1]._x + 0.15f == waterz[i]._x && waterz[n - 1]._y == waterz[i]._y && waterz[n - 1]._z == waterz[i]._z) {
                            ck++;
                            mv = 0;
                            Dq[0] = 5;
                            break;
                        }
                        else {
                            Dq[0] = 1;
                            mv = 0;
                        }
                    }
                    if (mv == 0) {
                        for (int i = 0; i < clubnum - 1; i++) {
                            if (waterz[n - 1]._x - 0.15f == waterz[i]._x && waterz[n - 1]._y == waterz[i]._y && waterz[n - 1]._z == waterz[i]._z) {
                                ck++;
                                mv = 0;
                                Dq[1] = 5;
                                break;
                            }
                            else {
                                Dq[1] = 2;
                                mv = 0;
                            }
                        }
                    }

                    if (mv == 0) {
                        for (int i = 0; i < clubnum - 1; i++) {
                            if (waterz[n - 1]._x == waterz[i]._x && waterz[n - 1]._y == waterz[i]._y && waterz[n - 1]._z + 0.15f == waterz[i]._z) {
                                ck++;
                                mv = 0;
                                Dq[2] = 5;
                                break;
                            }
                            else {
                                Dq[2] = 3;
                                mv = 0;
                            }
                        }
                    }

                    if (mv == 0) {
                        for (int i = 0; i < clubnum - 1; i++) {

                            if (waterz[n - 1]._x == waterz[i]._x && waterz[n - 1]._y == waterz[i]._y && waterz[n - 1]._z - 0.15f == waterz[i]._z) {
                                ck++;
                                mv = 0;
                                Dq[3] = 5;
                                break;
                            }
                            else {
                                Dq[3] = 4;
                                mv = 0;
                            }
                        }
                    }

                    int ck1 = 0;
                    do {
                        std::random_device rd;
                        std::mt19937 gen(rd());
                        std::uniform_int_distribution<int> dis(0, 3);
                        ck1 = Dq[dis(gen)];

                    } while (ck1 == 5 && ck != 4);



                    if (ck == 4) {
                        std::random_device rd;
                        std::mt19937 gen(rd());
                        std::uniform_int_distribution<int> dis(1, 4);
                        ck1 = dis(gen);
                    }

                    if (ck1 == 1) {
                        waterz[n - 1]._x = waterz[n - 1]._x + 0.15f;
                        undermove2(n);

                    }
                    if (ck1 == 2) {
                        waterz[n - 1]._x = waterz[n - 1]._x - 0.15f;
                        undermove2(n);
                    }
                    if (ck1 == 3) {
                        waterz[n - 1]._z = waterz[n - 1]._z + 0.15f;
                        undermove2(n);
                    }
                    if (ck1 == 4) {
                        waterz[n - 1]._z = waterz[n - 1]._z - 0.15f;
                        undermove2(n);
                    }


                    for (int i = 0; i < 4; i++) {
                        Dq[i] = 5;
                    }
                    ck = 0;
                    mv = 0;
                }
            }
        }
    }


    glutPostRedisplay();
}
void watermove() {
    for (int i = 0; i < clubnum2; i++) {
        if (waterz[i]._y > -0.44f && waterz[i].crash == 0) {
            waterz[i]._y = waterz[i]._y - 0.03f;
        }
    }
    int who = clubnum;
    if (who < clubnum2) {
        who = clubnum2;
    }
    for (int n = 1; n < clubnum2 + 1; n++) {
        for (int j = 0; j < who; j++) {
            if (n - 1 != j) {
                
                if ((waterz[n - 1]._x == sandz[j]._x && waterz[n - 1]._y == sandz[j]._y && waterz[n - 1]._z == sandz[j]._z) || (waterz[n - 1]._x == waterz[j]._x && waterz[n - 1]._y == waterz[j]._y && waterz[n - 1]._z == waterz[j]._z)){
                    waterz[n - 1].crash = 1;
                    printf("겹침");
                    //************************************************************************************
                    //************************************************************************************
                    ck = 0;
                    for (int i = 0; i < clubnum; i++) {
                        if (waterz[n - 1]._x + 0.15f == sandz[i]._x && waterz[n - 1]._y == sandz[i]._y && waterz[n - 1]._z == sandz[i]._z) {
                            ck++;
                            mv = 0;
                            Dq[0] = 5;
                            break;
                        }
                        else {
                            Dq[0] = 1;
                            mv = 0;
                        }
                    }
                    if (mv == 0) {
                        for (int i = 0; i < clubnum ; i++) {
                            if (waterz[n - 1]._x - 0.15f == sandz[i]._x && waterz[n - 1]._y == sandz[i]._y && waterz[n - 1]._z == sandz[i]._z) {
                                ck++;
                                mv = 0;
                                Dq[1] = 5;
                                break;
                            }
                            else {
                                Dq[1] = 2;
                                mv = 0;
                            }
                        }
                    }

                    if (mv == 0) {
                        for (int i = 0; i < clubnum ; i++) {
                            if (waterz[n - 1]._x == sandz[i]._x && waterz[n - 1]._y == sandz[i]._y && waterz[n - 1]._z + 0.15f == sandz[i]._z) {
                                ck++;
                                mv = 0;
                                Dq[2] = 5;
                                break;
                            }
                            else {
                                Dq[2] = 3;
                                mv = 0;
                            }
                        }
                    }

                    if (mv == 0) {
                        for (int i = 0; i < clubnum ; i++) {

                            if (waterz[n - 1]._x == sandz[i]._x && waterz[n - 1]._y == sandz[i]._y && waterz[n - 1]._z - 0.15f == sandz[i]._z) {
                                ck++;
                                mv = 0;
                                Dq[3] = 5;
                                break;
                            }
                            else {
                                Dq[3] = 4;
                                mv = 0;
                            }
                        }
                    }

                    int ck1 = 0;
                    do {
                        std::random_device rd;
                        std::mt19937 gen(rd());
                        std::uniform_int_distribution<int> dis(0, 3);
                        ck1 = Dq[dis(gen)];

                    } while (ck1 == 5 && ck != 4);




                    if (ck == 4 ) {
                        waterz[n - 1]._y = waterz[n - 1]._y + 0.15f;
                        std::random_device rd;
                        std::mt19937 gen(rd());
                        std::uniform_int_distribution<int> dis(1, 4);
                        ck1 = dis(gen);
                        printf("%d", ck1);
                    }

                    if (ck1 == 1) {

                        waterz[n - 1]._x = waterz[n - 1]._x + 0.15f;
                        undermove2(n);

                    }
                    if (ck1 == 2) {
                        waterz[n - 1]._x = waterz[n - 1]._x - 0.15f;
                        undermove2(n);
                    }
                    if (ck1 == 3) {
                        waterz[n - 1]._z = waterz[n - 1]._z + 0.15f;
                        undermove2(n);
                    }
                    if (ck1 == 4) {
                        waterz[n - 1]._z = waterz[n - 1]._z - 0.15f;
                        undermove2(n);
                    }


                    for (int i = 0; i < 4; i++) {
                        Dq[i] = 5;
                    }
                    ck = 0;
                    mv = 0;
                }
            }
        }
    }


    
    glutPostRedisplay();
}

void MyIdle() {
    glMatrixMode(GL_MODELVIEW);
    for (int i = 0; i < clubnum; i++) {
        if (sandz[i]._y > -0.44f && sandz[i].crash == 0) {
            sandz[i]._y = sandz[i]._y - 0.03f;
        }
    }

    for (int n = 1; n < clubnum+1; n++){
        for (int j = 0; j < clubnum - 1; j++) {
            if (n - 1 != j) {
                if (sandz[n - 1]._x == sandz[j]._x && sandz[n - 1]._y == sandz[j]._y && sandz[n - 1]._z == sandz[j]._z) {
                    sandz[n - 1].crash = 1;
                    //************************************************************************************
                    
                    //************************************************************************************
                    ck = 0;
                    for (int i = 0; i < clubnum - 1; i++) {
                        if (sandz[n - 1]._x + 0.15f == sandz[i]._x && sandz[n - 1]._y == sandz[i]._y && sandz[n - 1]._z == sandz[i]._z) {
                            ck ++;
                            mv = 0;
                            Dq[0] = 5;
                            break;
                        }
                        else {
                            Dq[0] = 1;
                            mv = 0;
                        }
                    }
                    if (mv == 0) {
                        for (int i = 0; i < clubnum - 1; i++) {
                            if (sandz[n - 1]._x - 0.15f == sandz[i]._x && sandz[n - 1]._y == sandz[i]._y && sandz[n - 1]._z == sandz[i]._z) {
                                ck++;
                                mv = 0;
                                Dq[1] = 5;
                                break;
                            }
                            else {
                                Dq[1] = 2;
                                mv = 0;
                            }
                        }
                    }

                    if (mv == 0) {
                        for (int i = 0; i < clubnum - 1; i++) {
                            if (sandz[n - 1]._x == sandz[i]._x && sandz[n - 1]._y == sandz[i]._y && sandz[n - 1]._z + 0.15f == sandz[i]._z) {
                                ck++;
                                mv = 0;
                                Dq[2] = 5;
                                break;
                            }
                            else {
                                Dq[2] = 3;
                                mv = 0;
                            }
                        }
                    }

                    if (mv == 0) {
                        for (int i = 0; i < clubnum - 1; i++) {

                            if (sandz[n - 1]._x == sandz[i]._x && sandz[n - 1]._y == sandz[i]._y && sandz[n - 1]._z - 0.15f == sandz[i]._z) {
                                ck++;
                                mv = 0;
                                Dq[3] = 5;
                                break;
                            }
                            else {
                                Dq[3] = 4;
                                mv = 0;
                            }
                        }
                    }

                    int ck1 = 0;
                    do {
                        std::random_device rd;
                        std::mt19937 gen(rd());
                        std::uniform_int_distribution<int> dis(0, 3);
                        ck1 = Dq[dis(gen)];

                    } while (ck1 == 5 && ck != 4);



                    if (ck == 4) {
                        sandz[n - 1]._y = sandz[n - 1]._y + 0.15f;
                    }

                    if (ck1 == 1) {
                        sandz[n - 1]._x = sandz[n - 1]._x + 0.15f;
                        undermove(n);

                    }
                    if (ck1 == 2) {
                        sandz[n - 1]._x = sandz[n - 1]._x - 0.15f;
                        undermove(n);
                    }
                    if (ck1 == 3) {
                        sandz[n - 1]._z = sandz[n - 1]._z + 0.15f;
                        undermove(n);
                    }
                    if (ck1 == 4) {
                        sandz[n - 1]._z = sandz[n - 1]._z - 0.15f;
                        undermove(n);
                    }


                    for (int i = 0; i < 4; i++) {
                        Dq[i] = 5;
                    }
                    ck = 0;
                    mv = 0;
                }
            }
        }
    }
    watermove();
    glutPostRedisplay();
}


void MyMouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON  && state == GLUT_DOWN) {
        clubnum++;
    }
    //std::random_device rd;
    //std::mt19937 gen(rd());
    //
    //std::uniform_int_distribution<int> dis(0, 3);
    //printf("난수 : %d \n", dis(gen));

    printf("좌표 : %d \n", clubnum);
      

    glutPostRedisplay();
}

void spckeycallback(unsigned char key, int x, int y)
{
    if (key == 27)
        exit(0);
    if (key == 's')
    {


        PlaySound(TEXT("sand.wav"), 0,  SND_ASYNC );
        
        clubnum++;
        glutPostRedisplay();
    }
    else if (key == 'w')
    {
        clubnum2++;
        glutPostRedisplay();
    }
}
void spckeycallback2(int key, int x, int y)
{

    if (key == GLUT_KEY_LEFT)
    {
        for (int n = clubnum; n < 1000; n++) {
            sandz[n]._x = sandz[n]._x - 0.15f;
            glutPostRedisplay();
        }
        for (int n = clubnum2; n < 1000; n++) {
            waterz[n]._x = waterz[n]._x - 0.15f;
            glutPostRedisplay();
        }
    }
    if (key == GLUT_KEY_RIGHT)
    {
        for (int n = clubnum; n < 1000; n++) {
            sandz[n]._x = sandz[n]._x + 0.15f;

            glutPostRedisplay();
        }
        for (int n = clubnum2; n < 1000; n++) {

            waterz[n]._x = waterz[n]._x + 0.15f;
            glutPostRedisplay();
        }
    }
    if (key == GLUT_KEY_UP)
    {
        for (int n = clubnum; n < 1000; n++) {
            sandz[n]._z = sandz[n]._z - 0.15f;
            glutPostRedisplay();
        }
        for (int n = clubnum2; n < 1000; n++) {
            waterz[n]._z = waterz[n]._z - 0.15f;
            glutPostRedisplay();
        }
    }
    if (key == GLUT_KEY_DOWN)
    {
        for (int n = clubnum; n < 1000; n++) {
            sandz[n]._z = sandz[n]._z + 0.15f;
            glutPostRedisplay();
        }
        for (int n = clubnum2; n < 1000; n++) {
            waterz[n]._z = waterz[n]._z + 0.15f;
            glutPostRedisplay();
        }
    }
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(_WINDOW_WIDTH, _WINDOW_HEIGHT);
    glutCreateWindow("Cube Drawing");
    //콜백함수
    
    
    glutDisplayFunc(MyDisplay);
    glutIdleFunc(MyIdle);
    glutReshapeFunc(MyReshape);




    glutKeyboardFunc(spckeycallback);
    glutSpecialFunc(spckeycallback2);

    glutMouseFunc(Mouse);
    glutMotionFunc(Motion);
    glutMainLoop();
}