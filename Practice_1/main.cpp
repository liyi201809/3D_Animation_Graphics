#if defined(_WIN32)
#	include <windows.h>
#endif

#include <cmath>
#include <cassert>
#include <cstdlib>
#include <cstring>

#include <vector>
#include <iostream>

#include <GL/glut.h>

#include "mouse.h"
//#include "argumentParser.h"

//START READING HERE!!!


//////Predefined global variables

//Use the enum values to define different rendering modes
//The mode is used by the function display and the mode is
//chosen during execution with the keys 1-9
enum DisplayModeType {TRIANGLE=1, FACE=2, CUBE=3, ARM=4, MESH=5,};

DisplayModeType DisplayMode = TRIANGLE;

unsigned int W_fen = 800;  // screen width
unsigned int H_fen = 800;  // screen height

float LightPos[4] = {1,1,0.4,1};
std::vector<float> MeshVertices;
std::vector<unsigned int> MeshTriangles;

//Declare your own global variables here:
int myVariableThatServesNoPurpose;

float anim_x = 0.f;


////////// Draw Functions

//function to draw coordinate axes with a certain length (1 as a default)
void drawCoordSystem(float length=1)
{
	//draw simply colored axes

	//remember all states of the GPU
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	//deactivate the lighting state
	glDisable(GL_LIGHTING);
	//draw axes
	glBegin(GL_LINES);
		glColor3f(1,0,0);
		glVertex3f(0,0,0);
		glVertex3f(length,0,0);

		glColor3f(0,1,0);
		glVertex3f(0,0,0);
		glVertex3f(0,length,0);

		glColor3f(0,0,1);
		glVertex3f(0,0,0);
		glVertex3f(0,0,length);
	glEnd();

	//reset to previous state
	glPopAttrib();
}

/**
 * Several drawing functions for you to work on
*/
float x0 = 0;

void drawTriangle()
{
	//a simple example of a drawing function for a triangle
	//1) try changing its color to red
	//2) try changing its vertex positions
	//3) add a second triangle in blue
	//4) add a global variable (initialized at 0), which represents the
	// x-coordinate of the first vertex of each triangle
	//5) go to the function animate and increment this variable
	//by a small value - observe the animation.

	glBegin(GL_TRIANGLES);
		glColor3f(1, 0, 0);
		glNormal3f(0, 0, 1);
		glVertex3f(anim_x, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 1, 0);
	glEnd();
}


void drawUnitFace()
{
	//1) draw a unit quad in the x,y plane oriented along the z axis
	//2) make sure the orientation of the vertices is positive (counterclock wise)
	//3) What happens if the order is inversed?
	glBegin(GL_QUADS);
	glColor3f(0, 1, 0);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex2f(0.0, 0.0);
	glVertex2f(1.0, 0.0);
	glVertex2f(1.0, 1.0);
	glVertex2f(0.0, 1.0);
	glEnd();
}


void drawUnitCube()
{
	//1) draw a cube using your function drawUnitFace
	//rely on glTranslate, glRotate, glPushMatrix, and glPopMatrix
	//the latter two influence the model matrix, as seen during the course.
	//glPushMatrix stores the current matrix and puts a copy on
	//the top of a stack.
	//glPopMatrix pops the top matrix on the stack
	
	drawUnitFace();
	glPushMatrix();
	glTranslatef(1.0, 0.0, 0.0);
	glRotated(-90, 0.0, 1.0, 0.0);
	drawUnitFace();

	glTranslatef(1.0, 0.0, 0.0);
	glRotated(-90, 0.0, 1.0, 0.0);
	drawUnitFace();

	glPopMatrix();
	glTranslatef(0.0, 1.0, 0.0);
	glRotated(90, 1.0, 0.0, 0.0);
	drawUnitFace();
}

/////// for the rotating ARM //////////////////////////////////////////////////////////////////////////////////// 
static int Angle1 = 0, Angle2 = 0, Angle3 =0;
void wireBox(GLdouble width, GLdouble height, GLdouble depth) {
	glPushMatrix();
	glScalef(width, height, depth);
	glutWireCube(0.5);
	glPopMatrix();
}
void special(int key, int, int) {
	switch (key) {
	case GLUT_KEY_LEFT: (Angle2 += 5) %= 360; break;
	case GLUT_KEY_RIGHT: (Angle2 -= 5) %= 360; break;
	case GLUT_KEY_UP: (Angle3 += 5) %= 360; break;
	case GLUT_KEY_DOWN: (Angle3 -= 5) %= 360; break;
	default: return;
	}
	glutPostRedisplay();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void drawArm()
{
	//produce a three-unit arm (upperarm, forearm, hand) making use of your
	//function drawUnitCube to define each of them
	//1) define 3 global variables that control the angles
	//between the arm parts
	//and add cases to the keyboard function to control these values

	//2) use these variables to define your arm
	//use glScalef to achieve different arm length
	//use glRotate/glTranslate to correctly place the elements

	//3 optional) make an animated snake out of these boxes
	//(an arm with 10 joints that moves using the animate function)
	glRotatef((GLfloat)Angle1, 0.0, 0.0, 1.0);
	glTranslatef(0.5, 0.0, 0.0);
	wireBox(2.0, 0.4, 1.0);

	glTranslatef(0.5, 0.0, 0.0);
	glRotatef((GLfloat)Angle2, 0.0, 0.0, 1.0);
	glTranslatef(0.5, 0.0, 0.0);
	wireBox(2.0, 0.4, 1.0);

	glTranslatef(0.5, 0.0, 0.0);
	glRotatef((GLfloat)Angle3, 0.0, 0.0, 1.0);
	glTranslatef(0.5, 0.0, 0.0);
	wireBox(2.0, 0.4, 1.0);

	glPopMatrix();
	glFlush();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void drawLight()
{
	//1) use glutSolidSphere to draw a sphere at the light's position LightPos
	//use glTranslatef to move it to the right location
	//to make the light source bright, follow the drawCoordSystem function
	//to deactivate the lighting temporarily and draw it in yellow

	//2) make the light position controllable via the keyboard function

	//3) add normal information to all your faces of the previous functions
	//and observe the shading after pressing 'L' to activate the lighting
	//you can use 'l' to turn it off again
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	//deactivate the lighting state
	glDisable(GL_LIGHTING);

	glTranslatef(LightPos[0], LightPos[1], LightPos[2]);
	glColor3f(1, 1, 0);
	glutSolidSphere(LightPos[3]/10, 16, 16);

	glPopAttrib();
}


void drawMesh()
{
	//1) use the mesh data structure;
	//each triangle is defined with 3 consecutive indices in the MeshTriangles table
	//these indices correspond to vertices stored in the MeshVertices table.
	//provide a function that draws these triangles.

	//2) compute the normals of these triangles

	//3) try computing a normal per vertex as the average of the adjacent face normals
	// call glNormal3f with the corresponding values before each vertex
	// What do you observe with respect to the lighting?

	//4) try loading your own model (export it from Blender as a Wavefront obj) and replace the provided mesh file.
	
	glBegin(GL_TRIANGLES);  // putting this line outside the for loop improves the speed
	for (int i = 0; i<MeshTriangles.size(); i=i+3 ) {
			
		    glColor3f(1, 0, 0);
			std::vector<float> v1{ MeshVertices[MeshTriangles[i] * 3], MeshVertices[MeshTriangles[i] * 3 + 1] , MeshVertices[MeshTriangles[i] * 3 + 2] };
			std::vector<float> v2{ MeshVertices[MeshTriangles[i + 1] * 3] ,MeshVertices[MeshTriangles[i + 1] * 3 + 1], MeshVertices[MeshTriangles[i + 1] * 3 + 2] };
			std::vector<float> v3{ MeshVertices[MeshTriangles[i + 2] * 3] ,MeshVertices[MeshTriangles[i + 2] * 3 + 1], MeshVertices[MeshTriangles[i + 2] * 3 + 2] };

			std::vector<float> v4{v2[0] - v1[0], v2[1] - v1[1], v2[2] - v1[2]};  //two edges of each triangle
			std::vector<float> v5{v3[0] - v1[0], v3[1] - v1[1], v3[2] - v1[2]};

			std::vector<float> v6;
			v6.push_back(v4[1] * v5[2] - v4[2] * v5[1]);  // calculate the cross products
			v6.push_back(v4[0] * v5[2] - v4[2] * v5[0]);
			v6.push_back(v4[0] * v5[1] - v4[1] * v5[0]);

			glNormal3f(v6[0], v6[1], v6[2]);

			glVertex3f(v1[0], v1[1], v1[2]);
			glVertex3f(v2[0], v2[1], v2[2]);
			glVertex3f(v3[0], v3[1], v3[2]);
			//glVertex3f(MeshVertices[MeshTriangles[i]*3], MeshVertices[MeshTriangles[i]*3+1], MeshVertices[MeshTriangles[i]*3 + 2]);
			/*glVertex3f(MeshVertices[MeshTriangles[i+1] * 3], MeshVertices[MeshTriangles[i+1] * 3 + 1], MeshVertices[MeshTriangles[i+1] * 3 + 2]);
			glVertex3f(MeshVertices[MeshTriangles[i+2] * 3], MeshVertices[MeshTriangles[i+2] * 3 + 1], MeshVertices[MeshTriangles[i+2] * 3 + 2]);*/
	}
	glEnd();
}


void display( )
{
	//set the light to the right position
    glLightfv(GL_LIGHT0,GL_POSITION,LightPos);
	drawLight();

	switch( DisplayMode )
	{
	case TRIANGLE:
		//drawMesh();
		drawCoordSystem();
		drawTriangle();
		break;
	case FACE:
		drawCoordSystem();
		drawUnitFace();
		break;
	case CUBE: 
		drawCoordSystem();
		drawUnitCube();
		break;
	case ARM:
		drawCoordSystem();
		drawArm();
		break;
	case MESH:
		drawCoordSystem();
		drawMesh();
		break;

	default:

		break;
	}
}

int determm = 0;
/**
 * Animation
 */
void animate( )
{   
	// for the triangle uses
	if (anim_x <= 3)
	{
		anim_x += 0.001f;
	}	
	// for rotating arms

}


//take keyboard input into account
void keyboard(unsigned char key, int x, int y)
{
	std::cout << "Key " << int(key) << " pressed at " << x << ", " << y << "\n";

	if ((key>='1')&&(key<='9'))
	{
		DisplayMode= (DisplayModeType) (key-'0');
		return;
	}

	switch (key)
    {
	case 27:     // Escape to exit the program.
        exit(0);
	case 'L':
		//turn lighting on
		glEnable(GL_LIGHTING);
		break;
	case 'l':
		//turn lighting off
		glDisable(GL_LIGHTING);
		break;
    // controlling the rotation angles of arm
	case 'n':
		(Angle1 += 5) %= 360;
		break;
	case 'm':
		(Angle1 -= 5) %= 360;
		break;
    }
}




//Nothing needed below this point
//STOP READING //STOP READING //STOP READING
//STOP READING //STOP READING //STOP READING
//STOP READING //STOP READING //STOP READING
//STOP READING //STOP READING //STOP READING


void displayInternal(void);
void reshape(int w, int h);
bool loadMesh(const char * filename);
void init()
{
    glDisable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);

	//int MatSpec [4] = {1,1,1,1};
 //   glMaterialiv(GL_FRONT_AND_BACK,GL_SPECULAR,MatSpec);
 //   glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,10);


    // Enable Depth test
    glEnable( GL_DEPTH_TEST );

	//glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
	//Draw frontfacing polygons as filled
    glPolygonMode(GL_FRONT,GL_FILL);
	//draw backfacing polygons as outlined
    glPolygonMode(GL_BACK,GL_LINE);
	glShadeModel(GL_SMOOTH);
	loadMesh("David.obj");
}


//load mesh
//this code is NOT how you should load a mesh... it is a bit hacky...
void centerAndScaleToUnit (std::vector<float> & vertices)
{
	if (vertices.size()%3!=0)
	{
		std::cout << "ERROR while loading!!!\n";
		return;
	}

	float x=0;
	float y=0;
	float z=0;
    for  (unsigned int i = 0; i < vertices.size (); i+=3)
	{
		x += vertices[i];
        y += vertices[i+1];
        z += vertices[i+2];
	}


	x /= vertices.size ()/3;
	y /= vertices.size ()/3;
	z /= vertices.size ()/3;

    float maxD = 0;
    for (unsigned int i = 0; i < vertices.size (); i+=3)
	{
        float dX= (vertices[i]-x);
        float dY= (vertices[i+1]-y);
        float dZ= (vertices[i+2]-z);

		float m = sqrt(dX*dX+dY*dY+dZ*dZ);
        if (m > maxD)
            maxD = m;
    }
    for (unsigned int i = 0; i < vertices.size (); i+=3)
	{
			vertices[i] = (vertices[i] - x) / maxD;
			vertices[i+1] = (vertices[i+1] - y) / maxD;
			vertices[i+2] = (vertices[i+2] - z) / maxD;
	}
}

bool loadMesh(const char * filename)
{
    const unsigned int LINE_LEN=256;
    char s[LINE_LEN];
    FILE * in;
#ifdef WIN32
    errno_t error=fopen_s(&in, filename,"r");
    if (error!=0)
#else
        in = fopen(filename,"r");
    if (!(in))
#endif
        return false;

	//temp stuff
    float x, y, z;
    std::vector<int> vhandles;



    while(in && !feof(in) && fgets(s, LINE_LEN, in))
    {
        // vertex
        if (strncmp(s, "v ", 2) == 0)
        {
            if (sscanf(s, "v %f %f %f", &x, &y, &z))
                MeshVertices.push_back(x);
				MeshVertices.push_back(y);
				MeshVertices.push_back(z);
        }
        // face
        else if (strncmp(s, "f ", 2) == 0)
        {
            int component(0), nV(0);
            bool endOfVertex(false);
            char *p0, *p1(s+2); //place behind the "f "

            vhandles.clear();

            while (*p1 == ' ') ++p1; // skip white-spaces

            while (p1)
            {
                p0 = p1;

                // overwrite next separator

                // skip '/', '\n', ' ', '\0', '\r' <-- don't forget Windows
                while (*p1 != '/' && *p1 != '\r' && *p1 != '\n' &&
                       *p1 != ' ' && *p1 != '\0')
                    ++p1;

                // detect end of vertex
                if (*p1 != '/') endOfVertex = true;

                // replace separator by '\0'
                if (*p1 != '\0')
                {
                    *p1 = '\0';
                    p1++; // point to next token
                }

                // detect end of line and break
                if (*p1 == '\0' || *p1 == '\n')
                    p1 = 0;


                // read next vertex component
                if (*p0 != '\0')
                {
                    switch (component)
                    {
                    case 0: // vertex
                        vhandles.push_back(atoi(p0)-1);
                        break;

                    case 1: // texture coord
                        //assert(!vhandles.empty());
                        //assert((unsigned int)(atoi(p0)-1) < texcoords.size());
                        //_bi.set_texcoord(vhandles.back(), texcoords[atoi(p0)-1]);
                        break;

                    case 2: // normal
                        //assert(!vhandles.empty());
                        //assert((unsigned int)(atoi(p0)-1) < normals.size());
                        //_bi.set_normal(vhandles.back(), normals[atoi(p0)-1]);
                        break;
                    }
                }

                ++component;

                if (endOfVertex)
                {
                    component = 0;
                    nV++;
                    endOfVertex = false;
                }
            }


            if (vhandles.size()>3)
            {
                //model is not triangulated, so let us do this on the fly...
                //to have a more uniform mesh, we add randomization
                unsigned int k=(false)?(rand()%vhandles.size()):0;
                for (unsigned int i=0;i<vhandles.size()-2;++i)
                {
                    MeshTriangles.push_back(vhandles[(k+0)%vhandles.size()]);
					MeshTriangles.push_back(vhandles[(k+i+1)%vhandles.size()]);
					MeshTriangles.push_back(vhandles[(k+i+2)%vhandles.size()]);
                }
            }
            else if (vhandles.size()==3)
            {
                MeshTriangles.push_back(vhandles[0]);
				MeshTriangles.push_back(vhandles[1]);
				MeshTriangles.push_back(vhandles[2]);
            }
            else
            {
                std::cout<<"TriMesh::LOAD: Unexpected number of face vertices (<3). Ignoring face \n";
            }
        }
        memset(&s, 0, LINE_LEN);
    }
    fclose(in);
    centerAndScaleToUnit (MeshVertices);
	return true;
}




// Program entry point. Everything starts here.
int main(int argc, char** argv)
{
    glutInit(&argc, argv);

	// Create a Window with double-buffering, RGB(A) colors and a depth buffer
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );

	// Set a default position and size
    glutInitWindowPosition(200, 100);
    glutInitWindowSize(W_fen,H_fen);
    glutCreateWindow(argv[0]);

    init( );

    // Initialize viewpoint
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0,0,-4);
    tbInitTransform();
    tbHelp();

	glutSpecialFunc(special);

	// Hook up the callbacks
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(displayInternal);
    glutMouseFunc(tbMouseFunc);
    glutMotionFunc(tbMotionFunc);
    glutIdleFunc(animate);

	// Hand over control to GLUT
	// glutMainLoop() never returns, so anything after it is never executed.
    glutMainLoop();

    return 0;
}

// OpenGL helpers. You don't need to touch these.
void displayInternal(void)
{
	// Clear screen
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glClear( GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT);

	// Load identity matrix
	glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

	// Load up view transforms
    tbVisuTransform();

	// Your rendering function
    display( );

	// Swap buffers and ask GLUT to show the results.
    glutSwapBuffers();
    glutPostRedisplay();
}
void reshape(int w, int h)
{
	// Called when the window is resized.
	// Update the viewport and projection matrix.
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho (-1.1, 1.1, -1.1,1.1, -1000.0, 1000.0);
    gluPerspective (50, (float)w/h, 1, 10);
    glMatrixMode(GL_MODELVIEW);
}

