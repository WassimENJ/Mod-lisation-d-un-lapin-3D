#include <cstdio>
#include <cstdlib>
/****************************************************************/
/* works on mac using freeglut and xquartz                      */
/* you can install freeglut using homebrew                      */
/* xquartz can be downloaded at http://xquartz.macosforge.org   */
/****************************************************************/
#include <GL/freeglut.h>
#include <jpeglib.h>
#include <jerror.h>
#ifdef __WIN32
#pragma comment (lib, "jpeg.lib")
#endif
#include <cmath>
unsigned char image[450*300*3];
unsigned char image2[1300*1390*3];
unsigned char texture[1300* 2][1390 * 2][3];
const float a = 0.15;  // Demi-grand axe
const float b = 0.4;  // Demi-petit axe
float cameraDistance = 10.0f;
GLfloat cameraAngleX = 0.0f;
GLfloat cameraAngleY = 0.0f;
GLfloat rotationSpeed = 2.0f;
float zoomSpeed = 0.1f;
float rotationAngleLeftEar = 0.0f;
float rotationAngleRightEar = 0.0f;
GLfloat rotationAngleY = 0.0f;
float zoomLevel = 1.0;
bool rotationEnabled = false; // Indicateur pour activer/désactiver la rotation
GLfloat rotationAngle = 0.0f; // Angle de rotation
GLfloat earRotationAngle = 0.0f;
bool carrotVisible = true;
float headRotationAngle = 0.0f;
GLfloat lapinYPosition = 0.0f;
GLfloat lapinZPosition=0.0f;
GLfloat jumpHeight = 2.0f;  // Hauteur du saut
GLfloat jumpSpeed = 0.05f;  // Vitesse du saut
bool isJumping = false;
bool light = false;
bool movingR=false;
bool movingL=false;
bool tomber=true;
bool manger=false;
GLuint textureID1;
GLuint textureID2;
char presse;
int anglex,angley,x,y,xold,yold;

/* Prototype des fonctions */
void affichage();
void clavier(unsigned char touche,int x,int y);
void reshape(int x,int y);
void idle();
void mouse(int bouton,int etat,int x,int y);
void mousemotion(int x,int y);
void Ellipsoide(float x, float y , float z , int n);
void Ellipsoide2(float x , float y , float z, int n);
void dessinerParallelogramme(float x, float y, float largeur, float hauteur, float angleDegres);
void Keys(int key, int x, int y);
void loadJpegImage(char *fichier);
void loadJpegImage1(char *fichier);
void carotte();
int main(int argc,char **argv)
{

  /* initialisation de glut et creation
     de la fenetre */
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowPosition(200,200);
  glutInitWindowSize(500,500);
  glutCreateWindow("Lapin");

  /* Initialisation d'OpenGL */
  glClearColor(0.55f, 0.81f, 0.98f, 1.0f);
  glColor3f(1.0,1.0,1.0);
  glPointSize(2.0);
  glEnable(GL_DEPTH_TEST);
 /* Parametrage du placage de textures */
glGenTextures(1, &textureID1);
glBindTexture(GL_TEXTURE_2D, textureID1);
loadJpegImage("./poil1.jpg");
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,450,300,0,
	       GL_RGB,GL_UNSIGNED_BYTE,image);
  glEnable(GL_TEXTURE_2D);
   /* Parametrage du placage de textures */
glGenTextures(2, &textureID2);
glBindTexture(GL_TEXTURE_2D, textureID2);
loadJpegImage1("./jouet.jpg");
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,1300,1390,0,
	       GL_RGB,GL_UNSIGNED_BYTE,image2);
  glEnable(GL_TEXTURE_2D);
  /* enregistrement des fonctions de rappel */
   glutIdleFunc(idle);
  glutDisplayFunc(affichage);
  glutKeyboardFunc(clavier);
  glutSpecialFunc(Keys);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutMotionFunc(mousemotion);
  /* Entree dans la boucle principale glut */
  glutMainLoop();
  return 0;
}

void dessinerligne(float x1, float y1, float z1, float x2, float y2, float z2) {
    glBegin(GL_LINES);
    glVertex3f(x1, y1, z1);
    glVertex3f(x2, y2, z2);
    glEnd();
}

void Ellipsoide(float x , float y , float z, int n)
{
// glBindTexture(GL_TEXTURE_2D, headTexture);
glBegin(GL_QUADS);

for(int i = 0; i < n; i++) {
float t1 = 2.0f * M_PI * i/n ;
float t1_next = 2.0f * M_PI * (i + 1) / n;

for(int j = 0; j < n; j++) {
float t2 = M_PI * j/n ;
float t2_next = M_PI * (j + 1) / n;

// On définit les points de la première face
float x1 = x * cos(t1) * sin(t2);
float y1 = y * sin(t1) * sin(t2);
float z1 = z * cos(t2);

float x2 = x * cos(t1_next) * sin(t2);
float y2 = y * sin(t1_next) * sin(t2);
float z2 = z * cos(t2);

float x3 = x * cos(t1_next) * sin(t2_next);
float y3 = y * sin(t1_next) * sin(t2_next);
float z3 = z * cos(t2_next);

float x4 = x * cos(t1) * sin(t2_next);
float y4 = y * sin(t1) * sin(t2_next);
float z4 = z * cos(t2_next);

float u1 = 0.5f * (1.0f + atan2(z1, x1) / M_PI);
float v1 = 0.5f * (1.0f - asin(y1) / M_PI);

float u2 = 0.5f * (1.0f + atan2(z2, x2) / M_PI);
float v2 = 0.5f * (1.0f - asin(y2) / M_PI);

float u3 = 0.5f * (1.0f + atan2(z3, x3) / M_PI);
float v3 = 0.5f * (1.0f - asin(y3) / M_PI);

float u4 = 0.5f * (1.0f + atan2(z4, x4) / M_PI);
float v4 = 0.5f * (1.0f - asin(y4) / M_PI);

glTexCoord2f(u1, v1);
glVertex3f(x1, y1, z1);

glTexCoord2f(u2, v2);
glVertex3f(x2, y2, z2);

glTexCoord2f(u3, v3);
glVertex3f(x3, y3, z3);

glTexCoord2f(u4, v4);
glVertex3f(x4, y4, z4);
}
}
glEnd();
}
void Ellipsoide2(float x , float y , float z, int n)
{
GLuint Texture;
glBegin(GL_QUADS);

for(int i = 0; i < n; i++) {
float t1 = 2.0f * M_PI * i/n ;
float t1_next = 2.0f * M_PI * (i + 1) / n;

for(int j = 0; j < n; j++) {
float t2 = M_PI * j/n ;
float t2_next = M_PI * (j + 1) / n;

// On définit les points de la première face
float x1 = x * cos(t1) * sin(t2);
float y1 = y * sin(t1) * sin(t2);
float z1 = z * cos(t2);

float x2 = x * cos(t1_next) * sin(t2);
float y2 = y * sin(t1_next) * sin(t2);
float z2 = z * cos(t2);

float x3 = x * cos(t1_next) * sin(t2_next);
float y3 = y * sin(t1_next) * sin(t2_next);
float z3 = z * cos(t2_next);

float x4 = x * cos(t1) * sin(t2_next);
float y4 = y * sin(t1) * sin(t2_next);
float z4 = z * cos(t2_next);
glVertex3f(x1, y1, z1);
glVertex3f(x2, y2, z2);
glVertex3f(x3, y3, z3);
glVertex3f(x4, y4, z4);
}
}
glEnd();
}
void Sphere(const float radius,const int numSegments,const int numSlices) {
    glBegin(GL_QUAD_STRIP);

    for (int i = 0; i <= numSegments; ++i) {
        double lat0 = M_PI * (-0.5 + (double)(i - 1) / numSegments);
        double z0 = sin(lat0);
        double zr0 = cos(lat0);

        double lat1 = M_PI * (-0.5 + (double)i / numSegments);
        double z1 = sin(lat1);
        double zr1 = cos(lat1);

        for (int j = 0; j <= numSlices; ++j) {
            double lng = 2 * M_PI * (double)(j - 1) / numSlices;
             double x = cos(lng);
            double y = sin(lng);

            // Coordonnées du premier point
            glVertex3f(radius * x * zr0, radius * y * zr0, radius * z0);
            // Coordonnées du deuxième point
            glVertex3f(radius * x * zr1, radius * y * zr1, radius * z1);
        }
    }

    glEnd();
  }

void dessinerCube(float taille) {

    glPushMatrix();

    glBegin(GL_QUADS);
glColor3f(1,1,1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


  glTexCoord2f(0.0,0.0);   glVertex3f(-0.5, 0.5, 0.5);
  glTexCoord2f(0.0,0.5);   glVertex3f(-0.5,-0.5, 0.5);
  glTexCoord2f(0.5,0.5);   glVertex3f( 0.5,-0.5, 0.5);
  glTexCoord2f(0.5,0.0);   glVertex3f( 0.5, 0.5, 0.5);


  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexCoord2f(0.0,0.0);   glVertex3f( 0.5, 0.5, 0.5);
  glTexCoord2f(0.0,3.0);   glVertex3f( 0.5,-0.5, 0.5);
  glTexCoord2f(3.0,3.0);   glVertex3f( 0.5,-0.5,-0.5);
  glTexCoord2f(3.0,0.0);   glVertex3f( 0.5, 0.5,-0.5);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);


  glTexCoord2f(0,0);   glVertex3f( 0.5, 0.5,-0.5);
  glTexCoord2f(0,3);   glVertex3f( 0.5,-0.5,-0.5);
  glTexCoord2f(3,3);   glVertex3f(-0.5,-0.5,-0.5);
  glTexCoord2f(3.0,0.0);   glVertex3f(-0.5, 0.5,-0.5);



  glTexCoord2f(0.0,0.0);   glVertex3f(-0.5, 0.5,-0.5);
  glTexCoord2f(0.0,0.5);   glVertex3f(-0.5,-0.5,-0.5);
  glTexCoord2f(0.5,0.5);   glVertex3f(-0.5,-0.5, 0.5);
  glTexCoord2f(0.5,0.0);   glVertex3f(-0.5, 0.5, 0.5);



  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexCoord2f(0.0,0.0);   glVertex3f(-0.5, 0.5,-0.5);
  glTexCoord2f(0.0,3.0);   glVertex3f(-0.5, 0.5, 0.5);
  glTexCoord2f(3.0,3.0);   glVertex3f( 0.5, 0.5, 0.5);
  glTexCoord2f(3.0,0.0);   glVertex3f( 0.5, 0.5,-0.5);

  glTexCoord2f(0.0,0.0);   glVertex3f(-0.5,-0.5,-0.5);
  glTexCoord2f(0.5,0);   glVertex3f(-0.5,-0.5, 0.5);
  glTexCoord2f(0.5,1);   glVertex3f( 0.5,-0.5, 0.5);
  glTexCoord2f(0,1);   glVertex3f( 0.5,-0.5,-0.5);


    glEnd();

    glPopMatrix();
}
void idle() {
    // Changez l'angle de rotation des oreilles au fil du temps
    earRotationAngle = 10.0 * sin(glutGet(GLUT_ELAPSED_TIME) * 0.0025); // Réglez la vitesse de l'animation ici
    if (isJumping) {
        lapinYPosition += jumpSpeed;
        if (lapinYPosition > jumpHeight) {
            isJumping = false;
        }
    } else {
        lapinYPosition -= jumpSpeed;
        if (lapinYPosition < 0.0f) {
            lapinYPosition = 0.0f;
        }
    }
    glutPostRedisplay();
}
void carotte()
{
  if (carrotVisible & !manger) {
 glPushMatrix();
    glColor3f(1.0, 0.65, 0.0);  // Orange
    glTranslatef(1, -1.5, -0.4);  // Position
    glRotatef(43.0, 1.0, 0.0, 0.0);  // Orientation
    glutSolidCone(0.35, 1.5, 20, 20);
    //glTranslatef(0.0, 0.0, 1.5);  //retour
    glPopMatrix();


    // Dessiner les feuilles
   glPushMatrix();
  glTranslatef(1, -1.5, -0.4);  // Position
    glutSolidSphere(0.34, 20, 20);
    glColor3f(0.0, 1.0, 0.0);  // Vert
    glTranslatef(0.0, 0.25,-0.25);
    glutSolidSphere(0.07, 20, 20);
    glPopMatrix();
}
if (manger) {
 glPushMatrix();
    glColor3f(1, 0.65, 0.0);  // Orange
    glTranslatef(0, -1.5, -0.4);  // Position
    glRotatef(43.0, 1.0, 0.0, 0.0);  // Orientation
    glutSolidCone(0.35, 1.5, 20, 20);
    //glTranslatef(0.0, 0.0, 1.5);  //retour
    glPopMatrix();


    // Dessiner les feuilles
   glPushMatrix();
  glTranslatef(0, -1.5, -0.4);  // Position
    glutSolidSphere(0.34, 20, 20);
    glColor3f(0.0, 1.0, 0.0);  // Vert
    glTranslatef(0.0, 0.25,-0.25);
    glutSolidSphere(0.07, 20, 20);
    glPopMatrix();
}
glutPostRedisplay();

}
void affichage()
{
  int i,j;
  /* effacement de l'image avec la couleur de fond */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glShadeModel(GL_SMOOTH);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
 glOrtho(-8.0 / zoomLevel, 8.0 / zoomLevel, -8.0 / zoomLevel, 8.0 / zoomLevel, -8.0, 8.0);
  gluLookAt(0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0);
  glRotatef(angley,1.0,0.0,0.0);
  glRotatef(anglex,0.0,1.0,0.0);
  glRotatef(lapinZPosition,0.0,0.0,1.0);
  glRotatef(rotationAngleY, 0.0f, 1.0f, 0.0f);
  glRotatef(cameraAngleY, 1.0f, 0.0f, 0.0f);
  glRotatef(cameraAngleX, 0.0f, 1.0f, 0.0f);


    if(light){
  // Configurations de la première lumière
   GLfloat light0Position[] = {0.0f, 1.0f, 5.0f, 0.0f};
    GLfloat light0Ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat light0Diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat light0Specular[] = {1.0f, 1.0f, 1.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, light0Position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0Ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0Specular);

    // Configurations de la deuxième lumière
// Configuration d'une lumière ponctuelle (GL_LIGHT2)

GLfloat light2Position[] = {0.0f, 1.0f, -5.0f, 1.0f};
GLfloat light2Ambient[] = {0.0f, 0.2f, 0.0f, 1.0f};  // Vert
GLfloat light2Diffuse[] = {0.0f, 1.0f, 0.0f, 1.0f};  // Vert
GLfloat light2Specular[] = {0.0f, 1.0f, 0.0f, 1.0f};  // Vert

glLightfv(GL_LIGHT2, GL_POSITION, light2Position);
glLightfv(GL_LIGHT2, GL_AMBIENT, light2Ambient);
glLightfv(GL_LIGHT2, GL_DIFFUSE, light2Diffuse);
glLightfv(GL_LIGHT2, GL_SPECULAR, light2Specular);


    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT2);
}
else{
glDisable(GL_LIGHTING);
glDisable(GL_LIGHT0);
glDisable(GL_LIGHT2);
}

   //glPushMatrix();
   //matrice de la tete
  glPushMatrix();
  glTranslatef(0.0f, lapinYPosition, 0.0f);
    glTranslatef(0.0f, 0.0f,lapinZPosition);
  //tete
  glBindTexture(GL_TEXTURE_2D, textureID1);
  glPushMatrix();
  //glRotatef(0, 0.0, 1.0, 0.0);
  glColor3f(1,1,1);
  glRotatef(earRotationAngle, 0.0, 1.0, 0.0);
  Ellipsoide(0.8,1.15,0.8,20);

   //oreilles
  glPushMatrix();
  glTranslatef(0,1.2,0.5);
  glRotatef(earRotationAngle, 0.0, 1.0, 0.0);
  glRotatef(35.0, 1.0, 0.0, 0.0);
  Ellipsoide(0.3,0.8,0.3,20);
  glTranslatef(0,-1.2,-0.5);//retour

  glColor3f(1.0, 0.71, 0.76);
  glTranslatef(0.09,1.2,0.5);
  Ellipsoide2(0.25,0.6,0.2,20);
  glTranslatef(-0.09,-1.2,-0.5);//retour
  glPopMatrix();


    glPushMatrix();
 glColor3f(1,1,1);
 glTranslatef(0,1.2,-0.5);
//glRotatef(earRotationAngle, 0.0, 1.0, 0.0);
 glRotatef(-35.0, 1.0, 0.0, 0.0);
 Ellipsoide(0.3,0.8,0.3,20);
  glTranslatef(0,-1.2,0.5);//retour


    glTranslatef(0.09,1.2,-0.5);
  glColor3f(1.0, 0.71, 0.76);
  Ellipsoide2(0.25,0.6,0.2,20);
    glTranslatef(-0.09,-1.2,0.5);//retour
glPopMatrix();



  // Yeux
  // droit
  glColor3f(1.0, 1.0, 1.0);  // Couleur blanche
  glTranslatef(0.6,0,0.4);
  Ellipsoide2(0.2,0.33,0.25,20);
  glTranslatef(-0.6,0,-0.4);//retour
  glColor3f(0,0,0);
  glTranslatef(0.72,0,0.43);
  Ellipsoide2(0.08,0.3,0.2,20);
   glTranslatef(-0.72,0,-0.43);//retour
   // gauche
  glColor3f(1.0, 1.0, 1.0);  // Couleur blanche
  glTranslatef(0.6,0,-0.4);
  Ellipsoide2(0.2,0.33,0.25,20);
  glTranslatef(-0.6,0,0.4);//retour
  glColor3f(0,0,0);
  glTranslatef(0.72,0,-0.43);
  Ellipsoide2(0.08,0.3,0.2,20);
   glTranslatef(-0.72,0,0.43);//retour

      //joues
glColor3f(1,1,1);
  glTranslatef(0.1,-0.45,0.4);
  Ellipsoide(0.8,0.6,0.7,20);
  glTranslatef(-0.1,0.45,-0.4);//retour

  glColor3f(1,1,1);
  dessinerligne(0.75,-0.6,0,1,-0.25,0.8);
  dessinerligne(0.75,-0.6,0,1,-0.45,0.8);

  dessinerligne(0.75,-0.6,0,1,-0.25,-0.8);
  dessinerligne(0.75,-0.6,0,1,-0.45,-0.8);

glColor3f(1,1,1);
  glTranslatef(0.1,-0.45,-0.4);
  Ellipsoide(0.8,0.6,0.7,20);
  glTranslatef(-0.1,0.45,0.4);//retour

     //nez
 glColor3f(1.0, 0.5, 0.5);
 glTranslatef(0.8,-0.25,0);
  Ellipsoide2(0.1,0.1,0.2,20);
   glTranslatef(-0.8,0.25,0);//retour
  glColor3f(1.0, 0.71, 0.76);
  glTranslatef(0.84,-0.195,0);
  Ellipsoide2(0.08,0.05,0.09,20);
  glTranslatef(-0.84,0.195,0);//retour


 //bouche
 //langue
 glColor3f(0.7,0,0.1);
 glTranslatef(0.7,-0.7,0);
Ellipsoide2(0.07,0.3,0.2,20);
 glTranslatef(-0.7,0.7,0);//retour
//dents
 glColor3f(1,1,1);
 glTranslatef(0.75,-0.6,0);
Ellipsoide2(0.05,0.12,0.12,20);
glTranslatef(-0.75,0.6,0);//retour

//glRotatef(headRotationAngle, 0.0, 1.0, 0.0); // Rotation automatique autour de l'axe Y
glPopMatrix();



//corps
  glColor3f(1,1,1);
  glTranslatef(0,-1.9,0);
  Ellipsoide(0.82,1.15,0.9,20);
  glTranslatef(0,1.9,0);//retour

  glTranslatef(-0.5,-2.6,0);
  Ellipsoide(0.8,1.1,0.9,20);
  glTranslatef(0.5,2.6,0);//retour

  //queue

  glColor3f(1,1,1);
  glTranslatef(-1.2,-3.3,0);
  Sphere(0.3,20,20);
 glTranslatef(1.2,3.3,0);//retour


//jambes
glColor3f(1,1,1);
 glTranslatef(-0.1,-2.8,-0.4);
  Ellipsoide(1,0.6,0.7,20);
   glTranslatef(0.1,2.8,0.4);//retour

    glTranslatef(-0.1,-2.8,0.4);
  Ellipsoide(1,0.6,0.7,20);
   glTranslatef(0.1,2.8,-0.4);//retour


   //pattes
       glTranslatef(0.3,-3.4,-0.5);
  Ellipsoide(0.9,0.35,0.4,20);
   glTranslatef(-0.3,3.4,0.5);//retour


   glTranslatef(0.3,-3.4,0.5);
  Ellipsoide(0.9,0.35,0.4,20);
   glTranslatef(-0.3,3.4,-0.5);//retour

    //bras1
     glPushMatrix();
  glColor3f(1,1,1);
   glTranslatef(0,-1.45,0.8);
   glRotatef(-30.0, 1.0, 0.0, 0.0);
   Ellipsoide(0.3,0.55,0.3,20);
    glPopMatrix();


     //bras2
  glPushMatrix();
   glTranslatef(0,-1.45,-0.8);
   glRotatef(30.0, 1.0, 0.0, 0.0);
   Ellipsoide(0.3,0.55,0.3,20);
    glPopMatrix();


    //main1
     glPushMatrix();

   glTranslatef(0.4,-1.9,0.75);
   glRotatef(60.0, 1.0, 0.0, 0.0);
   glRotatef(60.0, 0.0, 0.0, 1.0);
   Ellipsoide(0.3,0.7,0.3,20);
    glPopMatrix();

   //main2
     glPushMatrix();

   glTranslatef(0.4,-1.9,-0.75);
   glRotatef(-60.0, 1.0, 0.0, 0.0);
   glRotatef(60.0, 0.0, 0.0, 1.0);
   Ellipsoide(0.3,0.7,0.3,20);
    glPopMatrix();

glBindTexture(GL_TEXTURE_2D, 0);
carotte();
/*
    //Repère
    //axe x en rouge
    glBegin(GL_LINES);
        glColor3f(1.0,0.0,0.0);
    	glVertex3f(0, 0,0.0);
    	glVertex3f(1, 0,0.0);
    glEnd();
    //axe des y en vert
 glBegin(GL_LINES);
    	glColor3f(0.0,1.0,0.0);
    	glVertex3f(0, 0,0.0);
    	glVertex3f(0, 1,0.0);
    glEnd();
    //axe des z en bleu
    glBegin(GL_LINES);
    	glColor3f(0.0,0.0,1.0);
    	glVertex3f(0, 0,0.0);
    	glVertex3f(0, 0,1.0);
    glEnd();*/
     glPopMatrix();
     if(tomber)
     {
glBindTexture(GL_TEXTURE_2D, textureID2);
//jouets


//jouets positionnés
 glColor3f(1,0,0);
glTranslatef(0,-0.2,2.9);
dessinerCube(1);
glTranslatef(0,0.2,-2.9);//retour

glPushMatrix();
  glColor3f(0.988, 0.216, 0.918);
glTranslatef(0,-0.6,2.9);
glRotatef(90,1,0,0);
glutSolidCylinder(0.2,1,20,20);
 glPopMatrix();


 glPushMatrix();
 glColor3f(0, 0.694, 1);
glTranslatef(0,-1.6,2.9);
glRotatef(90,1,0,0);
glutSolidCone(0.5,1.1,20,20);
 glPopMatrix();


  glColor3f(1,1,1);
glTranslatef(0,-3.2,2.9);
dessinerCube(1);
glTranslatef(0,3.2,-2.9);//retour

glPushMatrix();
  glColor3f(0.541, 0.278, 0.11);
glTranslatef(0,-3.7,0);
glRotatef(90,1,0,0);
glutSolidCylinder(6,0.3,20,20);
 glPopMatrix();

glBindTexture(GL_TEXTURE_2D, 0);
}
else
{
//jouets par terre
glBindTexture(GL_TEXTURE_2D, textureID2);

glColor3f(1,0,0);
glTranslatef(-3.5,-3.2,0.5);
dessinerCube(1);
glTranslatef(3.5,3.2,-0.5);//retour

glPushMatrix();
glColor3f(0.988, 0.216, 0.918);
glTranslatef(-4,-3.5,-2);
//glRotatef(90,1,0,0);
glutSolidCylinder (0.2,1,20,20);
glPopMatrix();


glPushMatrix();
glColor3f(0, 0.694, 1);
glTranslatef(1,-3.7,2.9);
glRotatef(-90,1,0,0);
glutSolidCone(0.5,1,20,20);
glPopMatrix();


glColor3f(1,1,1);
glTranslatef(4,-3.2,-0.5);
dessinerCube(1);
glTranslatef(-4,3.2,0.5);//retour


glPushMatrix();
glColor3f(0.541, 0.278, 0.11);
glTranslatef(0,-3.7,0);
glRotatef(90,1,0,0);
glutSolidCylinder(6,0.3,20,20);
glPopMatrix();
glBindTexture(GL_TEXTURE_2D, 0);
}
  glFlush();

  //On echange les buffers
  glutSwapBuffers();
}

void clavier(unsigned char touche,int x,int y)
{
  switch (touche)
    {
    case 'p': /* affichage du carre plein */
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
      glutPostRedisplay();
      break;
    case 'f': /* affichage en mode fil de fer */
      glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
      glutPostRedisplay();
      break;
    case 's' : /* Affichage en mode sommets seuls */
      glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
      glutPostRedisplay();
      break;
    case 'd':
      glEnable(GL_DEPTH_TEST);
      glutPostRedisplay();
      break;
    case 'D':
      glDisable(GL_DEPTH_TEST);
      glutPostRedisplay();
      break;
    case 'a'://Les faces à l'envers s'affichent en fil de fer
      glPolygonMode(GL_FRONT,GL_FILL);
      glPolygonMode(GL_BACK,GL_LINE);
      glutPostRedisplay();
    break;
    case 'q' : /*la touche 'q' permet de quitter le programme */
      exit(0);
    case 'Z':
         zoomLevel += 0.1;
         glutPostRedisplay();
         break;
    case 'z':
         zoomLevel -= 0.1;
         if (zoomLevel < 0.1) zoomLevel = 0.1;
         glutPostRedisplay();
         break;
    case 'r':
            // Tourner l'objet à gauche
         tomber=true;
         glutPostRedisplay();
         break;
    case ' ':
         carrotVisible = !carrotVisible;  // Basculer la visibilité de la carotte
         glutPostRedisplay();
         break;
    case 'j' :
          if (!isJumping) {
          isJumping = true;
          glutPostRedisplay();
         }
         break;
    case 'l' :
         light = true;
         glutPostRedisplay();
        break;
    case 'L' :
        light = false;
         glutPostRedisplay();
        break;
    case 'x' :
       if (!movingR) {
        lapinZPosition += 0.5;
         movingL=false;
         if(lapinZPosition==2)
         {
            tomber=false;
         }
        if (lapinZPosition >3) {
            movingR= true;
        }
    }
    glutPostRedisplay();
        break;
    case 'X' :
        if (!movingL) {
        lapinZPosition -= 0.5;
        movingR=false;
        if (lapinZPosition <-3) {
            movingL= true;
        }
    }
    glutPostRedisplay();
        break;
    case 'm' :
        manger=true;
        break;
    case 'M' :
        manger=false;
        break;

}
}
void Keys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            cameraAngleX -= rotationSpeed;
            glutPostRedisplay();
            break;
        case GLUT_KEY_RIGHT:
            cameraAngleX += rotationSpeed;
            glutPostRedisplay();
            break;
        case GLUT_KEY_UP:
            cameraAngleY -= rotationSpeed;
            glutPostRedisplay();
            break;
        case GLUT_KEY_DOWN:
            cameraAngleY += rotationSpeed;
            glutPostRedisplay();
            break;
    }

    glutPostRedisplay();
}
void reshape(int x,int y)
{
  if (x<y)
    glViewport(0,(y-x)/2,x,x);
  else
    glViewport((x-y)/2,0,y,y);
}

void mouse(int button, int state,int x,int y)
{
  /* si on appuie sur le bouton gauche */
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    presse = 1; /* le booleen presse passe a 1 (vrai) */
    xold = x; /* on sauvegarde la position de la souris */
    yold=y;
  }
  /* si on relache le bouton gauche */
  if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    presse=0; /* le booleen presse passe a 0 (faux) */
}

void mousemotion(int x,int y)
  {
    if (presse) /* si le bouton gauche est presse */
    {
      /* on modifie les angles de rotation de l'objet
	 en fonction de la position actuelle de la souris et de la derniere
	 position sauvegardee */
      anglex=anglex+(x-xold);
      angley=angley+(y-yold);
      glutPostRedisplay(); /* on demande un rafraichissement de l'affichage */
    }

    xold=x; /* sauvegarde des valeurs courante de le position de la souris */
    yold=y;
  }
void loadJpegImage(char *fichier)
{
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE *file;
  unsigned char *ligne;

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);
#ifdef __WIN32
  if (fopen_s(&file,fichier,"rb") != 0)
    {
      fprintf(stderr,"Erreur : impossible d'ouvrir le fichier texture.jpg\n");
      exit(1);
    }
#elif __GNUC__
  if ((file = fopen(fichier,"rb")) == 0)
    {
      fprintf(stderr,"Erreur : impossible d'ouvrir le fichier texture.jpg\n");
      exit(1);
    }
#endif
  jpeg_stdio_src(&cinfo, file);
  jpeg_read_header(&cinfo, TRUE);

  if ((cinfo.image_width!=450)||(cinfo.image_height!=300)) {
    fprintf(stdout,"Erreur : l'image doit etre de taille 256x256\n");
    exit(1);
  }
  if (cinfo.jpeg_color_space==JCS_GRAYSCALE) {
    fprintf(stdout,"Erreur : l'image doit etre de type RGB\n");
    exit(1);
  }

  jpeg_start_decompress(&cinfo);
  ligne=image;
  while (cinfo.output_scanline<cinfo.output_height)
    {
      ligne=image+3*450*cinfo.output_scanline;
      jpeg_read_scanlines(&cinfo,&ligne,1);
    }
  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
}
void loadJpegImage1(char *fichier)
{
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE *file;
  unsigned char *ligne;

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);
#ifdef __WIN32
  if (fopen_s(&file,fichier,"rb") != 0)
    {
      fprintf(stderr,"Erreur : impossible d'ouvrir le fichier texture.jpg\n");
      exit(1);
    }
#elif __GNUC__
  if ((file = fopen(fichier,"rb")) == 0)
    {
      fprintf(stderr,"Erreur : impossible d'ouvrir le fichier texture.jpg\n");
      exit(1);
    }
#endif
  jpeg_stdio_src(&cinfo, file);
  jpeg_read_header(&cinfo, TRUE);

  if ((cinfo.image_width!=1300)||(cinfo.image_height!=1390)) {
    fprintf(stdout,"Erreur : l'image doit etre de taille 256x256\n");
    exit(1);
  }
  if (cinfo.jpeg_color_space==JCS_GRAYSCALE) {
    fprintf(stdout,"Erreur : l'image doit etre de type RGB\n");
    exit(1);
  }

  jpeg_start_decompress(&cinfo);
  ligne=image2;
  while (cinfo.output_scanline<cinfo.output_height)
    {
      ligne=image2+3*1300*cinfo.output_scanline;
      jpeg_read_scanlines(&cinfo,&ligne,1);
    }
  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
 for(int i=0;i<1300*2;i++)
{
    for(int j=0;j<1390*2;j++)
    {
        if((i<1300 && j<1390)||(i>=1300 && j>=1390))
        {
            texture[i][j][0]=image2[(i%256)*256*3+(j%256)*3];
            texture[i][j][1]=image2[(i%256)*256*3+(j%256)*3+1];
            texture[i][j][2]=image2[(i%256)*256*3+(j%256)*3+2];
        }
        else
        {
            texture[i][j][0]=255;
            texture[i][j][1]=255;
            texture[i][j][2]=255;
        }
    }
}
}

