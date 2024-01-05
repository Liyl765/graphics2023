#include <iostream>
#include <stdlib.h>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <GL/glut.h>
#include <math.h>
#include <Windows.h>
#include <string>
#define  GLUT_WHEEL_UP 3           
#define  GLUT_WHEEL_DOWN 4
using namespace std;
typedef OpenMesh::TriMesh_ArrayKernelT<> MyMesh;

int mousetate = 0; 
GLfloat Oldx = 0.0; 
GLfloat Oldy = 0.0;
GLuint texture;

float xRotate = 0.0f; 
float yRotate = 0.0f;
float ty = 0.0f;
float tx = 0.0f;
float scale = 0.004;

//�ļ���ȡ�йص�
MyMesh mesh; 
//"dinosaur.obj";
const string  file = "./objdata/";
const string file_1 = file + "cow.obj";
const string file_2 = file + "bunny.obj";
const string file_3 = file + "dinosaur.obj";
const string file_4 = file + "mba1.obj";
const string file_5 = file + "monkey.obj";
const string file_6 = file + "porsche.obj";
//const string file_7 = "teddy.obj";
const string file_7 = file + "huangfeng.obj";
const string file_8 = file + "file.obj";
const string file_9 = file + "face.sur";
int currentfile = 1;

GLuint showFaceList, showWireList;
int showstate = 1;
bool showFace = true;
bool showWire = false;
bool showFlatlines = false;



void setLightRes() {
    //GLfloat lightPosition[] = { 0.0f, 0.0f, 1.0f, 0.0f };
    GLfloat lightPosition[] = { 0.0f, 1.0f, 0.0f, 0.0f }; // ƽ�й�Դ


    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glEnable(GL_LIGHTING); //���ù�Դ
    glEnable(GL_LIGHT0);   //ʹ��ָ���ƹ�
}
void SetupRC()
{
    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    GLfloat ambientLight[] = { 1.0f, 0.0f, 0.0f, 0.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

//��ʼ���������   
void initGL()
{
    //ʵ����ʾ�б�
    glClearColor(0.0, 0.0, 0.0, 0.0);

    glClearDepth(2.0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST); 
    setLightRes();
    SetupRC();
    showFaceList = glGenLists(1);
    showWireList = glGenLists(1);
    //int temp = mesh.n_edges();

    // ���� wire 
    glNewList(showWireList, GL_COMPILE);
    glDisable(GL_LIGHTING);
    glLineWidth(1.0f);
    glColor3f(0.5f, 0.5f, 0.5f);//��ɫ
    glBegin(GL_LINES);
    //������άͼ����
    for (MyMesh::HalfedgeIter he_it = mesh.halfedges_begin(); he_it != mesh.halfedges_end(); ++he_it) {
        glVertex3fv(mesh.point(mesh.from_vertex_handle(*he_it)).data());
        glVertex3fv(mesh.point(mesh.to_vertex_handle(*he_it)).data());
    }
    glEnd();
    glEnable(GL_LIGHTING);
    glEndList();

    // ����flat
    glNewList(showFaceList, GL_COMPILE);
    //������ͼ����
    for (MyMesh::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it) {
        glBegin(GL_TRIANGLES);
        for (MyMesh::FaceVertexIter fv_it = mesh.fv_iter(*f_it); fv_it.is_valid(); ++fv_it) {
            glNormal3fv(mesh.normal(*fv_it).data());  
            glVertex3fv(mesh.point(*fv_it).data());
        }
        glEnd();
    }
    glEndList();
}


// ������ı��С��ʱ�򣬸ı䴰�ڴ�Сʱ����ͼ�α���
void myReshape(GLint w, GLint h)
{
    glViewport(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w > h)
        glOrtho(-static_cast<GLdouble>(w) / h, static_cast<GLdouble>(w) / h, -1.0, 1.0, -100.0, 100.0);
    else
        glOrtho(-1.0, 1.0, -static_cast<GLdouble>(h) / w, static_cast<GLdouble>(h) / w, -100.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


// ��ȡ�ļ��ĺ���
void readfile(string file) {
    mesh.request_vertex_normals();
    if (!mesh.has_vertex_normals())
    {
        cout << "���󣺱�׼�������� �����ߡ�������" << endl;
        return;
    }
    OpenMesh::IO::Options opt;
    if (!OpenMesh::IO::read_mesh(mesh, file, opt))
    {
        cout << "�޷���ȡ�ļ�:" << file << endl;
        return;
    }
    else cout << "�ɹ���ȡ�ļ�:" << file << endl;
    cout << endl; 
    if (!opt.check(OpenMesh::IO::Options::VertexNormal))
    {
        mesh.request_face_normals();
        mesh.update_normals();
        mesh.release_face_normals();
    }
}

//  ���̽��� 1. �л��ļ� 2.�л���ʾ
void mySpecial(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_F1:
        cout << "��ȡ�ļ�:" << file_1 << " ��......" << endl;
        readfile(file_1);
        scale = 0.004;
        currentfile = 1;
        initGL();
        break;
    case GLUT_KEY_F2:
        cout << "��ȡ�ļ�:" << file_2 << " ��......" << endl;
        readfile(file_2);
        scale = 0.003;
        currentfile = 2;
        initGL();
        break;
    case GLUT_KEY_F3:
        cout << "��ȡ�ļ�:" << file_3 << " ��......" << endl;
        readfile(file_3);
        scale = 0.02;
        currentfile = 3;
        initGL();
        break;
    case GLUT_KEY_F4:
        cout << "��ȡ�ļ�:" << file_4 << " ��......" << endl;
        readfile(file_4);
        scale = 0.005;
        currentfile = 3;
        initGL();
        break;
    case GLUT_KEY_F5:
        cout << "��ȡ�ļ�:" << file_5 << " ��......" << endl;
        readfile(file_5);
        scale = 0.5;
        currentfile = 5;
        initGL();
        break;
    case GLUT_KEY_F6:
        cout << "��ȡ�ļ�:" << file_6 << " ��......" << endl;
        readfile(file_6);
        scale = 0.02;
        currentfile = 3;
        initGL();
        break;
    case GLUT_KEY_F7:
        cout << "��ȡ�ļ�:" << file_7 << " ��......" << endl;
        readfile(file_7);
        scale = 0.04;
        currentfile = 3;
        initGL();
        break;
    case GLUT_KEY_F8:
        cout << "��ȡ�ļ�:" << file_8 << " ��......" << endl;
        readfile(file_8);
        scale = 0.04;
        currentfile = 3;
        initGL();
        break;
    case GLUT_KEY_F9:
        cout << "��ȡ�ļ�:" << file_9 << " ��......" << endl;
        readfile(file_9);
        scale = 0.04;
        currentfile = 3;
        initGL();
        break;
    case GLUT_KEY_INSERT:
        if (showFace == true) {
            showFace = false;
            showWire = true;

            cout << "�л���ʾģʽΪ��WireFrame" << endl;
        }
        else if (showWire == true)
        {
            showWire = false;
            showFlatlines = true;
            cout << "�л���ʾģʽΪ��Flatlines" << endl;
        }
        else if (showFlatlines == true) {
            showFlatlines = false;
            showFace = true;
            //DisplaySphere(0.5, "G:\\source\\OpenGL\\Cow\\checkerboard3.bmp");
            cout << "�л���ʾģʽΪ��Flat" << endl;
        }
        break;
    case GLUT_KEY_LEFT:
        tx -= 0.01;
        break;
    case GLUT_KEY_RIGHT:
        tx += 0.01;
        break;
    case GLUT_KEY_UP:
        ty += 0.01;
        break;
    case GLUT_KEY_DOWN:
        ty -= 0.01;
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

// ��꽻��
void myMouse(int button, int state, int x, int y)
{    
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        mousetate = 1;
        Oldx = x;
        Oldy = y;
    }
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
        mousetate = 0;
    //�����¼�
    if (state == GLUT_UP && button == GLUT_WHEEL_UP) {
        //cout << "hello" << endl; 
        if (currentfile == 1)
            scale += 0.0005;
        if (currentfile == 2)
            scale += 0.001;
        if (currentfile == 5) {
            scale += 0.1;
        }
        else
            scale += 0.001;
    }
    if (state == GLUT_UP && button == GLUT_WHEEL_DOWN) {
        //cout << "good" << endl;
        if (currentfile == 1)
            scale -= 0.0005;
        if (currentfile == 2)
            scale -= 0.001;
        if (currentfile == 5) {
            scale -= 0.1;
        }
        else
            scale -= 0.001;
    }
    glutPostRedisplay();
}

// ����˶�ʱ
void onMouseMove(int x, int y) {
    if (mousetate) {
        yRotate += x - Oldx;
        glutPostRedisplay();
        Oldx = x;
        xRotate += y - Oldy;
        glutPostRedisplay();
        Oldy = y;
    }
}

void myDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glRotatef(xRotate, 1.0f, 0.0f, 0.0f); 
    glRotatef(yRotate, 0.0f, 1.0f, 0.0f);
    //glTranslatef(0.0f, 0.0f, ty);
    glTranslatef(tx, ty, 0);        
    glScalef(scale, scale, scale); 

    if (showFace)
        glCallList(showFaceList);
    if (showFlatlines) {
        glCallList(showFaceList);
        glCallList(showWireList);
    }
    if (showWire)
        glCallList(showWireList);

    glutSwapBuffers();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); 
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 500);
    glutCreateWindow("Mesh Viewer");
    glutMouseFunc(myMouse);        
    glutMotionFunc(onMouseMove);
    glutSpecialFunc(&mySpecial);
    glutReshapeFunc(&myReshape);
    glutDisplayFunc(&myDisplay);

    glutMainLoop();
    return 0;
}