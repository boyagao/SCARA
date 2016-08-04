#include "Angel.h"

typedef Angel::vec4 point4;
typedef Angel::vec4 color4;

const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)
GLuint main_window;

point4 points[NumVertices];
color4 colors[NumVertices];

point4 vertices[8] = {
  point4( -0.5, -0.5,  0.5, 1.0 ),
  point4( -0.5,  0.5,  0.5, 1.0 ),
  point4(  0.5,  0.5,  0.5, 1.0 ),
  point4(  0.5, -0.5,  0.5, 1.0 ),
  point4( -0.5, -0.5, -0.5, 1.0 ),
  point4( -0.5,  0.5, -0.5, 1.0 ),
  point4(  0.5,  0.5, -0.5, 1.0 ),
  point4(  0.5, -0.5, -0.5, 1.0 )
};

// RGBA olors
point4 light_position1 = point4(-5, 0, 10, 0);
point4 light_position2 = point4(-2, 1, 1, 0);
point4 light_diffuse = point4(1, 1, 1, 1);
color4 material_diffuse = point4(1, .8, 0, 1);
float material_shininess = 100.0;


color4 vertex_colors[8] = {
  color4( 0.0, 0.0, 0.0, 1.0 ),  // black
  color4( 1.0, 0.0, 0.0, 1.0 ),  // red
  color4( 1.0, 1.0, 0.0, 1.0 ),  // yellow
  color4( 0.0, 1.0, 0.0, 1.0 ),  // green
  color4( 0.0, 0.0, 1.0, 1.0 ),  // blue
  color4( 1.0, 0.0, 1.0, 1.0 ),  // magenta
  color4( 1.0, 1.0, 1.0, 1.0 ),  // white
  color4( 0.0, 1.0, 1.0, 1.0 )   // cyan
};


// Parameters controlling the size of the Robot's arm
const GLfloat BASE_HEIGHT      = .25;
const GLfloat BASE_WIDTH       = 5.0;
const GLfloat LOWER_ARM_HEIGHT = 5.0;
const GLfloat LOWER_ARM_WIDTH  = 1.0;
const GLfloat UPPER_ARM_HEIGHT = 0.5;
const GLfloat UPPER_ARM_WIDTH  = 5.0;
const GLfloat THIRD_ARM_HEIGHT = 0.5;
const GLfloat THIRD_ARM_WIDTH  = 5.0;
const GLfloat FOURTH_ARM_HEIGHT = 4.0;
const GLfloat FOURTH_ARM_WIDTH  = 0.25;

// Shader transformation matrices
mat4  model_view;
GLuint ModelView, Projection;

// Array of rotation angles (in degrees) for each rotation axis
enum {
  Base      , 
  LowerArm  , 
  UpperArm  , 
  ThirdArm  , 
  FourthArm ,
  NumAngles 
};
int      Axis = UpperArm;
GLfloat
Theta[NumAngles] = { 
  0.0,
  0.0,
  0.0,
  0.0,
  0.0
};
  

// camera
GLfloat theta = 0.6;
GLfloat phi = -0.85;

const GLfloat  dr = 5.0 * DegreesToRadians;


// Menu option values
const int  Quit = 5;


//----------------------------------------------------------------------------
vec4 product(vec4 a, vec4 b)
{
  return vec4(a[0]*b[0], a[1]*b[1], a[2]*b[2], a[3]*b[3]);
}

void
color()
{
  vec3 n1;
  vec4 n;
  color4 diffuse_color1, diffuse_color2;

  float d1, d2;

  for (int i=0; i<30; i++) {
    n1 = normalize(cross(vertices[i+2]-vertices[i], vertices[i+1]-vertices[i]));
    n = vec4(n1[0], n1[1], n1[2], 0);

    d1 = dot(light_position1, n);
    d2 = dot(light_position2, n);

    if(d1>0.0) diffuse_color1 = d1*product(light_diffuse, material_diffuse);
    else diffuse_color1 = color4(0, 0, 0, 1);

    if(d2>0.0) diffuse_color2 = d2*product(light_diffuse, material_diffuse);
    else diffuse_color2 = color4(0, 0, 0, 1);

    colors[i] = diffuse_color1 + diffuse_color2;
  }

}


int Index = 0;

void
quad( int a, int b, int c, int d )
{
  colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
  colors[Index] = vertex_colors[b]; points[Index] = vertices[b]; Index++;
  colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
  colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
  colors[Index] = vertex_colors[b]; points[Index] = vertices[c]; Index++;
  colors[Index] = vertex_colors[d]; points[Index] = vertices[d]; Index++;

}

void
colorcube()
{
  quad( 1, 0, 3, 2 );
  quad( 2, 3, 7, 6 );
  quad( 3, 0, 4, 7 );
  quad( 6, 5, 1, 2 );
  quad( 4, 5, 6, 7 );
  quad( 5, 4, 0, 1 );
}

//----------------------------------------------------------------------------

/* Define the three parts */
/* Note use of push/pop to return modelview matrix
   to its state before functions were entered and use
   rotation, translation, and scaling to create instances
   of symbols (cube and cylinder */

void
base()
{
  mat4 instance = ( Translate( 0.0, 0.5 * BASE_HEIGHT, 0.0 ) *
		    Scale( BASE_WIDTH,
			   BASE_HEIGHT,
			   BASE_WIDTH ) );

  glUniformMatrix4fv( ModelView, 1, GL_TRUE, model_view * instance );

  glDrawArrays( GL_TRIANGLES, 0, NumVertices );
}

//----------------------------------------------------------------------------

void
lower_arm()
{
  mat4 instance = ( Translate( 0.0, 0.5 * LOWER_ARM_HEIGHT, 0.0 ) *
		    Scale( LOWER_ARM_WIDTH,
			   LOWER_ARM_HEIGHT,
			   LOWER_ARM_WIDTH ) );
    
  glUniformMatrix4fv( ModelView, 1, GL_TRUE, model_view * instance );
  glDrawArrays( GL_TRIANGLES, 0, NumVertices );
}

//----------------------------------------------------------------------------

void
upper_arm()
{
  mat4 instance = ( Translate( UPPER_ARM_WIDTH - LOWER_ARM_WIDTH/2, 0.5 * UPPER_ARM_HEIGHT, 0.0 ) *
		    Scale( UPPER_ARM_WIDTH * 2,
			   UPPER_ARM_HEIGHT,
			   LOWER_ARM_WIDTH ) );
    
  glUniformMatrix4fv( ModelView, 1, GL_TRUE, model_view * instance );
  glDrawArrays( GL_TRIANGLES, 0, NumVertices );
}

//----------------------------------------------------------------------------

void
third_arm()
{
  mat4 instance = ( Translate( THIRD_ARM_WIDTH - LOWER_ARM_WIDTH/2, 0.5 * THIRD_ARM_HEIGHT, 0.0 ) *
		    Scale( THIRD_ARM_WIDTH * 2,
			   THIRD_ARM_HEIGHT,
			   LOWER_ARM_WIDTH ) );
    
  glUniformMatrix4fv( ModelView, 1, GL_TRUE, model_view * instance );
  glDrawArrays( GL_TRIANGLES, 0, NumVertices );
}

//----------------------------------------------------------------------------

void
fourth_arm()
{
  mat4 instance = ( Translate( FOURTH_ARM_WIDTH - LOWER_ARM_WIDTH/2, -THIRD_ARM_HEIGHT*2.5, 0.0 ) *
		    Scale( FOURTH_ARM_WIDTH * 2,
			   FOURTH_ARM_HEIGHT,
			   LOWER_ARM_WIDTH/2 ) );
    
  glUniformMatrix4fv( ModelView, 1, GL_TRUE, model_view * instance );
  glDrawArrays( GL_TRIANGLES, 0, NumVertices );
}

//----------------------------------------------------------------------------

void
display( void )
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  // Accumulate ModelView Matrix as we traverse the tree
  point4  eye( sin(theta)*cos(phi),
	       sin(theta)*sin(phi),
	       cos(theta),
	       1.0 );
  point4  at( 0,0,0,1);
  point4  up( 0,1,0,0);

  model_view = LookAt(eye, at, up);
  base();

  model_view *= ( Translate(0.0, BASE_HEIGHT, 0.0) *
		  RotateZ(Theta[LowerArm]) );
  lower_arm();


  model_view *= ( Translate(0.0, LOWER_ARM_HEIGHT, 0.0) *
		  RotateY(Theta[UpperArm]) );
  upper_arm();


  model_view *= ( Translate(THIRD_ARM_WIDTH + UPPER_ARM_WIDTH - LOWER_ARM_WIDTH, UPPER_ARM_HEIGHT, 0.0) *
		  RotateY(Theta[ThirdArm]) );
  third_arm();


  model_view *= ( Translate(THIRD_ARM_WIDTH + UPPER_ARM_WIDTH - LOWER_ARM_WIDTH, UPPER_ARM_HEIGHT, 0.0) *
		  Translate(0.0, Theta[FourthArm], 0.0) );
  fourth_arm();

  glutSwapBuffers();
}

//----------------------------------------------------------------------------

void
init( void )
{
  colorcube();
    
  // Create a vertex array object
  GLuint vao;
  glGenVertexArraysAPPLE( 1, &vao );
  glBindVertexArrayAPPLE( vao );

  // Create and initialize a buffer object
  GLuint buffer;
  glGenBuffers( 1, &buffer );
  glBindBuffer( GL_ARRAY_BUFFER, buffer );
  glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors),
		NULL, GL_DYNAMIC_DRAW );
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors );
    
  // Load shaders and use the resulting shader program
  GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );
  glUseProgram( program );
    
  GLuint vPosition = glGetAttribLocation( program, "vPosition" );
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
			 BUFFER_OFFSET(0) );

  GLuint vColor = glGetAttribLocation( program, "vColor" );
  glEnableVertexAttribArray( vColor );
  glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
			 BUFFER_OFFSET(sizeof(points)) );

  ModelView = glGetUniformLocation( program, "ModelView" );
  Projection = glGetUniformLocation( program, "Projection" );

  glEnable( GL_DEPTH );
  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

  glClearColor( 1.0, 1.0, 1.0, 1.0 ); 
}

//----------------------------------------------------------------------------

void
menu( int option )
{
  if ( option == Quit ) {
    glutDestroyWindow( main_window );
  }
  else {
    Axis = option;
  }
}

//----------------------------------------------------------------------------

void
reshape( int width, int height )
{
  glViewport( 0, 0, width, height );

  GLfloat  left = -10.0, right = 30.0;
  GLfloat  bottom = -5.0, top = 15.0;
  GLfloat  zNear = -30.0, zFar = 30.0;

  GLfloat aspect = GLfloat(width)/height;

  if ( aspect > 1.0 ) {
    left *= aspect;
    right *= aspect;
  }
  else {
    bottom /= aspect;
    top /= aspect;
  }

  mat4 projection = Ortho( left, right, bottom, top, zNear, zFar );
  glUniformMatrix4fv( Projection, 1, GL_TRUE, projection );

  model_view = mat4( 1.0 );  // An Identity matrix
}

//----------------------------------------------------------------------------

void
keyboard( unsigned char key, int x, int y )
{
  switch( key ) {
  case 033: // Escape Key
  case 'q': case 'Q':
    printf("%1.3f, %1.3f", theta, phi);
    glutDestroyWindow( main_window );
    break;
  case 'K': theta += dr; glutPostRedisplay(); break;
  case 'k': theta -= dr; glutPostRedisplay(); break;
  case 'J': phi += dr; glutPostRedisplay(); break;
  case 'j': phi -= dr; glutPostRedisplay(); break;
  case 'r': case 'R':
    theta = 0.6;
    phi = -0.85;
    glutPostRedisplay();
    break;
  case '+':
    if (Axis == FourthArm) {
      if (Theta[Axis] < 2.7) {
	Theta[Axis] += .1;
      }
    }
    if (Axis == ThirdArm) {
      if (Theta[FourthArm] >= 2.6) {
	Theta[Axis] += 5.0;
	if (Theta[Axis] > 360.0) {Theta[Axis] -= 360.0; }
      } else {
	if (Theta[Axis] <170) {
	  Theta[Axis] += 5.0;
	}
      }
    }
    if (Axis == UpperArm) {
      Theta[Axis] += 5.0;
      if ( Theta[Axis] > 360.0 ) { Theta[Axis] -= 360.0; }
    }
    glutPostRedisplay();
    break;
  case '-':
    if (Axis == FourthArm) {      
      if (Theta[Axis] > -.7) {
	Theta[Axis] -= .1;
      }
    }
    if (Axis == ThirdArm) {
      if (Theta[FourthArm] >= 2.6) {
	Theta[Axis] -= 5.0;
	if (Theta[Axis] < 0.0) { Theta[Axis] += 360.0; }
      } else {
	if (Theta[Axis] >= -170) {
	  Theta[Axis] -= 5.0;
	}
      }
    }
    if (Axis == UpperArm) {
      Theta[Axis] -= 5.0;
      if ( Theta[Axis] < 0.0 ) { Theta[Axis] += 360.0; }
    }
    glutPostRedisplay();
    break;
  }
}

//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{
  printf("\n\n--------------------INSTRUCTIONS-------------------- \n");
  printf("Right click with mouse to select joint (default is Joint One) \n");
  printf("Press + to rotate joint clockwise or move up \n");
  printf("Press - to rotate joint counterclockwise or move down \n");
  printf("Press J or SHIFT+J to decrease/increase Phi component of the angle \n");
  printf("Press K or SHIFT+K to decrease/increase Theta component of the angle \n\n");
  glutInit( &argc, argv );
  glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
  glutInitWindowSize( 512, 512 );
  main_window = glutCreateWindow( "SCARA" );

  glewInit();
  init();

  glutDisplayFunc( display );
  glutReshapeFunc( reshape );
  glutKeyboardFunc( keyboard );

  glutCreateMenu( menu );
  glutAddMenuEntry( "Joint One", UpperArm );
  glutAddMenuEntry( "Joint Two", ThirdArm );
  glutAddMenuEntry( "Joint Three", FourthArm );    
  glutAddMenuEntry( "Quit", Quit );
  glutAttachMenu( GLUT_RIGHT_BUTTON );

  glutMainLoop();
  return 0;
}
