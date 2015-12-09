#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"

const GLuint WIDTH = 1920/2, HEIGHT = 1080/2;
GLFWwindow* window;
GLboolean keyboardKeys[1024] = {GL_FALSE};
float keyboardKeysTimePressed[1024] = {0};
float keyboardKeysTimeReleased[1024] = {0};
glm::vec3 direction(0.0f, 0.0f, 0.0f);
glm::vec3 rotation(0.0f, 0.0f, 0.0f);
glm::mat4 view;

GLfloat normalizeCursorMovement( GLfloat movement, GLfloat range ) {
	return (movement - floor(movement/range)*range)/range;
}

void fps() {
	static GLuint frames = 0;
	static GLfloat fpsTimeout = 0.0f;
	if( fpsTimeout == 0.0f ) {
		fpsTimeout = glfwGetTime() + 1.0f;
	}
	if( glfwGetTime() > fpsTimeout ) {
		fpsTimeout = glfwGetTime() + 1.0f;
		printf( "%d fps\n", frames);
		frames = 0;
	}
	++frames;
}

void error_callback( int error, const char* description ) {
	fprintf( stderr, "%s", description );
	//fputs( description, stderr );
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
}

static void key_callback( GLFWwindow* window, int key, int scancode, int action, int mods ) {
	printf("key action\n");
	if( key > 1023 ) {
		fprintf( stderr, "key input %d too large", key );
		key = 1023;
	}
	if( scancode > 1023 ) {
		fprintf( stderr, "scancode input %d too large", scancode );
		scancode = 1023;
	}
	if( action == GLFW_PRESS ) {
		keyboardKeys[key] = GL_TRUE;
		keyboardKeysTimePressed[key] = glfwGetTime();
		if( key == GLFW_KEY_ESCAPE ) {
			glfwSetWindowShouldClose( window, GL_TRUE );
		}
	} else if( action == GLFW_RELEASE ) {
		keyboardKeys[key] = GL_FALSE;
		keyboardKeysTimeReleased[key] = glfwGetTime();
	}
}

void updateCamera() {
	// TODO handle overflow, can use glfwSetCursorPos
	double xpos, ypos;
	xpos = ypos = 0;
	glfwGetCursorPos( window, &xpos, &ypos );
	GLfloat temp;
	temp = normalizeCursorMovement( xpos, WIDTH * 4 ) * 2 * PI;
	rotation.y = temp;
	direction.x = sin( temp );
	direction.z = -cos( temp );
	temp = normalizeCursorMovement( ypos, WIDTH * 4 ) * 2 * PI;
	rotation.x = temp;
	//printf( "direction x %f, y %f, z %f\n", direction.x, direction.y, direction.z );
	
	glm::mat4 tempMat;
	if( keyboardKeys[GLFW_KEY_W] ) {
		//TODO finish this
		view = glm::translate( view, -1.0f * (GLfloat)(glfwGetTime() - keyboardKeysTimePressed[GLFW_KEY_W]) * direction );
	}
	if( keyboardKeys[GLFW_KEY_S] ) {
		view = glm::translate( view, (GLfloat)(glfwGetTime() - keyboardKeysTimePressed[GLFW_KEY_S]) * direction );
	}
	if( keyboardKeys[GLFW_KEY_A] ) {
		//view = glm::translate( view, glm::vec3( direction.x, direction.y, direction.z) );
	}
	if( keyboardKeys[GLFW_KEY_D] ) {
	}
	if( keyboardKeys[GLFW_KEY_Q] ) {
	}
	if( keyboardKeys[GLFW_KEY_E] ) {
	}
}

int main() {
	glfwSetErrorCallback( error_callback );
	if ( !glfwInit() ) {
		return -1;
	}
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
	window = glfwCreateWindow( WIDTH, HEIGHT, "particle simulation", nullptr, nullptr );
	if ( !window ) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent( window );
	glfwSetKeyCallback( window, key_callback );
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glewExperimental = GL_TRUE;
	if( glewInit() != GLEW_OK ) {
		fprintf( stderr, "glewInit() failed\n" );
	}
	glViewport( 0, 0, WIDTH, HEIGHT );
	glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
	glEnable( GL_DEPTH_TEST );
	Shader shader( "./vertexShader.glsl", "./fragmentShader.glsl" );

}
