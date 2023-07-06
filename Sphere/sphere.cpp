#include <iostream>
#include<vector>

using namespace std;

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "SOIL2/SOIL2.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

const GLuint WIDTH = 800, HEIGHT = 600;
GLfloat vertices[1000000];
GLuint indices2[1000000];
int n=12;

void sphereCoordinates(GLfloat r);

int main()
{
    glfwInit( );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "Cube3D", nullptr, nullptr );
    
    int screenWidth, screenHeight;
    glfwGetFramebufferSize( window, &screenWidth, &screenHeight );
    
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent( window );
    
    glewExperimental = GL_TRUE;
    
    if ( GLEW_OK != glewInit( ) )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    
    glViewport( 0, 0, screenWidth, screenHeight );
    
    glEnable( GL_DEPTH_TEST );
    
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    
    Shader ourShader( "res/shaders/core.vs", "res/shaders/core.frag" );
    
    
    sphereCoordinates(0.3);
    GLuint VBO,VAO,EBO;

    glGenVertexArrays( 1, &VAO );
    glGenBuffers( 1, &VBO );
    glGenBuffers( 1, &EBO );
    glBindVertexArray(VAO);
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
      glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices2 ), indices2, GL_STATIC_DRAW );
    // Position attribute
    glVertexAttribPointer( 0, 3, GL_FLOAT , GL_FALSE, 5 * sizeof( GLfloat ), ( GLvoid * )0 );
    glEnableVertexAttribArray( 0 );
    // TexCoord attribute
    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof( GLfloat ), ( GLvoid * )( 3 * sizeof( GLfloat ) ) );
    glEnableVertexAttribArray( 2 );
    glBindVertexArray( 0 );
    
    
    GLuint texture;
    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    int width, height;
    unsigned char *image = SOIL_load_image( "res/images/image6.png", &width, &height, 0, SOIL_LOAD_RGBA );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image );
    glGenerateMipmap( GL_TEXTURE_2D );
    SOIL_free_image_data( image );
    glBindTexture( GL_TEXTURE_2D, 0 );
    
    glm::mat4 projection;
    projection = glm::perspective( 45.0f, ( GLfloat )screenWidth / ( GLfloat )screenHeight, 0.1f, 100.0f );
    
    while ( !glfwWindowShouldClose( window ) )
    {
        glfwPollEvents( );
        glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D, texture );
        glUniform1i(glGetUniformLocation( ourShader.Program, "ourTexture1" ), 0 );
        
        ourShader.Use( );
        
        glm::mat4 model = glm::mat4( 1.0f );
        glm::mat4 view = glm::mat4( 1.0f );
        model = glm::rotate( model, ( GLfloat)glfwGetTime( ) *1.0f, glm::vec3( 1.0f, 1.0f, 0.0f ) );
        view = glm::translate( view, glm::vec3( 0.0f, 0.0f, -1.0f ) );
        
        GLint modelLoc = glGetUniformLocation( ourShader.Program, "model" );
        GLint viewLoc = glGetUniformLocation( ourShader.Program, "view" );
        GLint projLoc = glGetUniformLocation( ourShader.Program, "projection" );
        
        glUniformMatrix4fv( modelLoc, 1, GL_FALSE, glm::value_ptr( model ) );
        glUniformMatrix4fv( viewLoc, 1, GL_FALSE, glm::value_ptr( view ) );
        glUniformMatrix4fv( projLoc, 1, GL_FALSE, glm::value_ptr( projection ) );

        
        glBindVertexArray( VAO);
        glDrawElements(GL_TRIANGLES, sizeof(indices2) / sizeof(int), GL_UNSIGNED_INT, 0);
        glBindVertexArray( 0 );
        
        glfwSwapBuffers( window );
    }
    glDeleteVertexArrays( 1, &VAO );
    glDeleteBuffers( 1, &VBO );
    glDeleteBuffers( 1, &EBO );
    
    glfwTerminate( );
    
    return EXIT_SUCCESS;
}

void sphereCoordinates(GLfloat r){
    
    int longitude=360;
    int latitude=180;
    float angle0=M_PI*2/longitude;
    float angle1=M_PI/latitude;

    vector<float> v;

    for(int i=0;i<=latitude+1;i++){
        for(int j=0;j<longitude;j++){
            float x=r*sin(angle1*i)*sin(angle0*j);
            float y=r*cos(angle1*i);
            float z=r*sin(angle1*i)*cos(angle0*j);
            v.push_back(x);
            v.push_back(y);
            v.push_back(z);
            v.push_back(angle0*j/16*M_PI);
            v.push_back(angle1*i+0.05);
            }
        }

    vector<float> ind;

    for(int i=1;i<2*longitude;i++){
        ind.push_back(0);
        ind.push_back(i);
        ind.push_back(i+1);
        }
    ind.push_back(0);
    ind.push_back(2*longitude);
    ind.push_back(1);
    
    int n=0;
    int p=2*longitude;

    for(int j=0;j<latitude;j++){
        for(int i=n+1;i<n+p;i++){
            ind.push_back(i);
            ind.push_back(i+1);
            ind.push_back(p+i);
            ind.push_back(p+i);
            ind.push_back(p+i+1);
            ind.push_back(i+1);
        }
        n+=p;
        ind.push_back(n);
        ind.push_back(n-p+1);
        ind.push_back(n+p);
        ind.push_back(n+p);
        ind.push_back(n+1);
        ind.push_back(n-p+1);
    }
    for(int i=n+1;i<n+p;i++){
        ind.push_back(i);
        ind.push_back(i+1);
        ind.push_back(n+p+1);
        }
    ind.push_back(n+p);
    ind.push_back(n+1);
    ind.push_back(n+p+1);
    
    for(int i=0;i<v.size();i++){
        vertices[i]=(GLfloat)v[i];
        }
    for(int i=0;i<ind.size();i++){
        indices2[i]=(GLuint)ind[i];
        }
    }
