/*
 * A C++ framework for OpenGL programming in TNM046 for MT1 2021.
 *
 * This is a small, limited framework, designed to be easy to use
 * for students in an introductory computer graphics course in
 * the first year of a M Sc curriculum. It uses custom code
 * for some things that are better solved by external libraries
 * like GLM, but the emphasis is on simplicity andreadability, 
 * not generality.
 * For the window management, GLFW 3.x is used for convenience.
 * The framework should work in Windows, MacOS X and Linux.
 * GLEW is used for handling platform specific OpenGL extensions.
 * This code is dependent only on GLFW, GLEW, and OpenGL libraries. 
 * OpenGL 3.3 or higher is required.
 *
 * Authors: Stefan Gustavson (stegu@itn.liu.se) 2013-2015
 *          Martin Falk (martin.falk@liu.se) 2021
 *
 * This code is in the public domain.
 */
#if defined(WIN32) && !defined(_USE_MATH_DEFINES)
#define _USE_MATH_DEFINES
#endif

// File and console I/O for logging and error reporting
#include <iostream>
// Math header for trigonometric functions
#include <cmath>

// glew provides easy access to advanced OpenGL functions and extensions
#include <GL/glew.h>

// GLFW 3.x, to handle the OpenGL window
#include <GLFW/glfw3.h>

// include twice????
//#include "Utilities.hpp"

// --- Add this line to your includes
#include "Utilities.hpp"

#include <vector>
// --- Add this to the includes ---------------------------------------------------
#include "Shader.hpp"

#include <array>

#include "TriangleSoup.hpp"

#include "Texture.hpp"

#include "Rotator.hpp"

// Multiply 4x4 matrices m1 and m2 and return the result
std::array<float, 16> mat4mult(const std::array<float, 16>& m1, const std::array<float, 16>& m2) {
    std::array<float, 16> result;
    // Your code goes here: compute and set each element of result, e.g.:6// result[0] = m1[0] * m2[0] +
    // m1[4] * m2[1] + m1[8] * m2[2] + m1[12] * m2[3];7// etc. for the remaining 15 elements.8return
    
    result[0] = m1[0] * m2[0] + m1[4] * m2[1] + m1[8] * m2[2] + m1[12] * m2[3];
    result[1] = m1[1] * m2[0] + m1[5] * m2[1] + m1[9] * m2[2] + m1[13] * m2[3];
    result[2] = m1[2] * m2[0] + m1[6] * m2[1] + m1[10] * m2[2] + m1[14] * m2[3];
    result[3] = m1[3] * m2[0] + m1[7] * m2[1] + m1[11] * m2[2] + m1[15] * m2[3];

    result[4] = m1[0] * m2[4] + m1[4] * m2[5] + m1[8] * m2[6] + m1[12] * m2[7];
    result[5] = m1[1] * m2[4] + m1[5] * m2[5] + m1[9] * m2[6] + m1[13] * m2[7];
    result[6] = m1[2] * m2[4] + m1[6] * m2[5] + m1[10] * m2[6] + m1[14] * m2[7];
    result[7] = m1[3] * m2[4] + m1[7] * m2[5] + m1[11] * m2[6] + m1[15] * m2[7];

    result[8] = m1[0] * m2[8] + m1[4] * m2[9] + m1[8] * m2[10] + m1[12] * m2[11];
    result[9] = m1[1] * m2[8] + m1[5] * m2[9] + m1[9] * m2[10] + m1[13] * m2[11];
    result[10] = m1[2] * m2[8] + m1[6] * m2[9] + m1[10] * m2[10] + m1[14] * m2[11];
    result[11] = m1[3] * m2[8] + m1[7] * m2[9] + m1[11] * m2[10] + m1[15] * m2[11];

    result[12] = m1[0] * m2[12] + m1[4] * m2[13] + m1[8] * m2[14] + m1[12] * m2[15];
    result[13] = m1[1] * m2[12] + m1[5] * m2[13] + m1[9] * m2[14] + m1[13] * m2[15];
    result[14] = m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15];
    result[15] = m1[3] * m2[12] + m1[7] * m2[13] + m1[11] * m2[14] + m1[15] * m2[15];

    return result;
}

// Print the elements of a matrix m
void mat4print(const std::array<float, 16>& m) {
    printf("Matrix: \n");
    printf("%6.2f %6.2f %6.2f %6.2f \n", m[0], m[4], m[8], m[12]);
    printf("%6.2f %6.2f %6.2f %6.2f \n", m[1], m[5], m[9], m[13]);
    printf("%6.2f %6.2f %6.2f %6.2f \n", m[2], m[6], m[10], m[14]);
    printf("%6.2f %6.2f %6.2f %6.2f \n", m[3], m[7], m[11], m[15]);
    printf("\n");
}

std::array<float, 16> mat4identity() {
    std::array<float, 16> temp = {
        1.0f, 0.0f, 0.0f, 0.0f, 
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, 
        0.0f, 0.0f, 0.0f, 1.0f
    };
    return temp;
}

std::array<float, 16> mat4rotx(float angle) {
    std::array<float, 16> temp = {
        1.0f, 0.0f,         0.0f,        0.0f, 
        0.0f, cos(angle), sin(angle), 0.0f,
        0.0f, -sin(angle), cos(angle), 0.0f, 
        0.0f, 0.0f,         0.0f,       1.0f
    };
    return temp;
}

std::array<float, 16> mat4roty(float angle) {
    std::array<float, 16> temp = {
        cos(angle), 0.0f, sin(angle), 0.0f, 
        0.0f,       1.0f, 0.0f,         0.0f,                          
        -sin(angle), 0.0f, cos(angle), 0.0f, 
        0.0f,       0.0f, 0.0f,         1.0f
    };
    return temp;
}

std::array<float, 16> mat4rotz(float angle) {
    std::array<float, 16> temp = {
        cos(angle), sin(angle), 0.0f, 0.0f, 
        -sin(angle), cos(angle), 0.0f, 0.0f,               
        0.0f,       0.0f,       1.0f, 0.0f, 
        0.0f,       0.0f,       0.0f, 1.0f
    };
    return temp;
}

std::array<float, 16> mat4scale(float scale) {
    std::array<float, 16> temp = {
        scale, 0.0f, 0.0f, 0, 
        0.0f, scale, 0.0f, 0,                 
        0.0f, 0.0f, scale, 0, 
        0.0f, 0.0f, 0.0f, 1.0f
    };
    return temp;
}

std::array<float, 16> mat4translate(float x, float y, float z) {
    
    std::array<float, 16> temp = {
        1.0f, 0.0f, 0.0f, 0.0f, 
        0.0f, 1.0f, 0.0f, 0.0f,                  
        0.0f, 0.0f, 1.0f, 0.0f, 
        x,      y,      z, 1.0f
    };

    return temp;
}

GLuint createVertexBuffer(int location, int dimensions, const std::vector<float>& vertices) {
    GLuint bufferID;
    // Generate buffer, activate it and copy the data
    glGenBuffers(1, &bufferID);
    glBindBuffer(GL_ARRAY_BUFFER, bufferID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    // Tell OpenGL how the data is stored in our buffer
    // Attribute location (must match layout(location=#) statement in shader)
    // Number of dimensions (3 -> vec3 in the shader, 2-> vec2 in the shader),
    // type GL_FLOAT, not normalized, stride 0, start at element 0
    glVertexAttribPointer(location, dimensions, GL_FLOAT, GL_FALSE, 0, nullptr);
    // Enable the attribute in the currently bound VAO
    glEnableVertexAttribArray(location);
    return bufferID;
}

GLuint createIndexBuffer(const std::vector<unsigned int>& indices) {
    GLuint bufferID;
    // Generate buffer, activate it and copy the data
    glGenBuffers(1, &bufferID);
    // Activate (bind) the index buffer and copy data to it.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
    // Present our vertex indices to OpenGL
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    return bufferID;
}

// create and return a perspective matrix
//
// vfov is the vertical field of view (in the y direction)
// aspect is the aspect ratio of the viewport (width/height)
// znear is the distance to the near clip plane (znear > 0)
// zfar is the distance to the far clip plane (zfar > znear)
std::array<float, 16> mat4perspective(float vfov, float aspect, float znear, float zfar) {
    //float f = cos(vfov / 2) / sin(vfov / 2);

    float f = 1 / tan(vfov / 2);

    std::array<float, 16> temp = {
        f/aspect, 0.0f, 0.0f, 0.0f, 
        0.0f, f, 0.0f, 0.0f,
        0.0f, 0.0f, -(zfar + znear)/(zfar - znear), -1.0f, 
        0.0f, 0.0f, -(2 * zfar * znear)/(zfar - znear), 0.0f
    };

    return temp;

}

/*
 * main(int argc, char *argv[]) - the standard C++ entry point for the program
 */
int main(int, char *[]) {

    // --- Add this to the variable declarations --------------------------------------
    Shader myShader;

  int width, height;

  const GLFWvidmode
      *vidmode;        // GLFW struct to hold information about the display
  GLFWwindow *window;  // GLFW struct to hold information about the window

  // Initialise GLFW
  glfwInit();

  // Determine the desktop size
  vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());

  // Make sure we are getting a GL context of at least version 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // Exclude old legacy cruft from the context. We don't need it, and we don't
  // want it
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  // Open a square window (aspect 1:1) to fill half the screen height
  window = glfwCreateWindow(vidmode->height / 2, vidmode->height / 2,
                            "GLprimer", NULL, NULL);
  if (!window) {
    std::cout << "Unable to open window. Terminating.\n";
    glfwTerminate();  // No window was opened, so we can't continue in any
                      // useful way
    return -1;
  }

  // Make the newly created window the "current context" for OpenGL
  // (This step is strictly required, or things will simply not work)
  glfwMakeContextCurrent(window);

  // initialize glew
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    std::cerr << "Error: " << glewGetErrorString(err) << "\n";
    glfwTerminate();
    return -1;
  }

  // Show some useful information on the GL context
  std::cout << "GL vendor:       " << glGetString(GL_VENDOR)
            << "\nGL renderer:     " << glGetString(GL_RENDERER)
            << "\nGL version:      " << glGetString(GL_VERSION)
            << "\nDesktop size:    " << vidmode->width << " x "
            << vidmode->height << "\n";

  // Get window size. It may start out different from the requested
  // size, and will change if the user resizes the window
  // glfwGetWindowSize(window, &width, &height);
  // Set viewport. This is the pixel rectangle we want to draw into
  // glViewport(0, 0, width, height);  // The entire window

  glfwSwapInterval(0);  // Do not wait for screen refresh between frames, base value 0
 
  // --- Add this in main() after glewInit() and before the rendering loop ----
  myShader.createShader("../shaders/vertex.glsl", "../shaders/fragment.glsl");

  
  // Do this before the rendering loop

  GLint locationTime = glGetUniformLocation(myShader.id(), "time");
  if (locationTime == -1) {  // If the variable is not found, -1 is returned
      std::cout << "Unable to locate variable'time'in shader!\n";
  }

  //mat4print(mat4mult(mat4translate(0.0f, 0.0f, -3.0f), mat4rotx(M_PI/2)));

  // --- Put this before your rendering loop
  // Generate a triangle
  TriangleSoup myShape;
  TriangleSoup myDino;
  myDino.readOBJ("meshes/trex.obj");
  //TriangleSoup mySphere;

  myShape.createSphere(0.5f, 100);
  //mySphere.createSphere(1, 100);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  // --- Put this before the rendering loop
  // Locate the sampler2D uniform in the shader program
  GLint locationTex = glGetUniformLocation(myShader.id(), "tex");
  // Generate one texture object with data from a TGA file
  Texture myTexture;
  myTexture.createTexture("textures/earth.tga");
  Texture myDinoTex;
  myDinoTex.createTexture("textures/trex.tga");

  // --- Put this before the rendering loop, but after the window is opened.
  KeyRotator myKeyRotator(window);
  MouseRotator myMouseRotator(window);


  // Main loop
  while (!glfwWindowShouldClose(window)) {
    // move to while loop
    glfwGetWindowSize(window, &width, &height);
    glViewport(0, 0, width, height);
    
    // Set the clear color to a dark gray (RGBA)
    glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
    // Clear the color and depth buffers for drawing
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* ---- Rendering code should go here ---- */

    glUseProgram(myShader.id());

    /*
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr); // was 3
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);                   // GL_FILL
    glCullFace(GL_BACK);
    */
    
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr); // was 3
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);                   // LINE
    glCullFace(GL_BACK);                                        // GL_FRONT
    
    // Do this in the rendering loop to update the uniform variable "time"
    float time = static_cast<float>(glfwGetTime());  // Number of seconds since the program was started
    glUseProgram(myShader.id());            // Activate the shader to set its variables
    glUniform1f(locationTime, time);        // Copy the value to the shader program


    // --- Put this in the rendering loop
    myKeyRotator.poll();
    // Create a rotation matrix that depends on myKeyRotator.phi and myKeyRotator.theta
    std::array<GLfloat, 16> matKeyRotator =
        mat4mult(mat4rotx(-myKeyRotator.theta()), mat4roty(-myKeyRotator.phi()));
    myMouseRotator.poll();
    // Create rotation matrix that depends on myMouseRotator.phi and myMouseRotator.theta
    std::array<GLfloat, 16> matMouseRotator =
        mat4mult(mat4rotx(myMouseRotator.theta()), mat4roty(-myMouseRotator.phi()));


    std::array<GLfloat, 16> matT = matMouseRotator;
    GLint locationT = glGetUniformLocation(myShader.id(), "T");
    glUniformMatrix4fv(locationT, 1, GL_FALSE, matT.data());  // Copy the value
   
    std::array<GLfloat, 16> matVrid = mat4rotx(-M_PI / 2);
    std::array<GLfloat, 16> matMinska = mat4scale(0.2f);
    std::array<GLfloat, 16> matSpin = mat4roty(time);
    std::array<GLfloat, 16> matFlytt = mat4translate(0.5f, 0.0f, 0.0f);
    std::array<GLfloat, 16> matOrbit = mat4roty(time / 2);
    std::array<GLfloat, 16> matCam = mat4rotx(M_PI / 8);  // --- Put this in the rendering loop

    // std::array<GLfloat, 16> matRot =mat4mult(matKey, matMouse);
    std::array<GLfloat, 16> matA = mat4mult(matMinska, matVrid);
    std::array<GLfloat, 16> matB = mat4mult(matSpin, matA);
    std::array<GLfloat, 16> matC = mat4mult(matFlytt, matB);
    std::array<GLfloat, 16> matD = mat4mult(matOrbit, matC);
    std::array<GLfloat, 16> matE = mat4mult(matCam, matD);

    

    std::array<GLfloat, 16> matP = mat4perspective((M_PI / 4), 1.0f, 0.1f, 100.0f);
    GLint locationP = glGetUniformLocation(myShader.id(), "P");
    // glUseProgram(myShader.id());  // Activate the shader to set its variables
    glUniformMatrix4fv(locationP, 1, GL_FALSE, matP.data());  // Copy the value

    // --- Put this in the rendering loop
    // Draw the TriangleSoup object mySphere
    // with a shader program that uses a texture
    //glUseProgram(myShader.id());
    glUniform1i(locationTex, 0);

    // earth below

    std::array<GLfloat, 16> matMV = mat4mult(mat4mult(mat4translate(0.0f, 0.0f, -2.5f), matE), mat4scale(3.0f));

    GLint locationMV = glGetUniformLocation(myShader.id(), "MV");
    //glUseProgram(myShader.id());  // Activate the shader to set its variables
    glUniformMatrix4fv(locationMV, 1, GL_FALSE, matMV.data());  // Copy the value
    
    glBindTexture(GL_TEXTURE_2D, myTexture.id());
    myShape.render();


    // dino below

    matMV = mat4mult(mat4mult(mat4translate(0.0f, 0.0f, -2.5f), matKeyRotator), mat4scale(0.6f));

    glUniformMatrix4fv(locationMV, 1, GL_FALSE, matMV.data());  // Copy the value

    glBindTexture(GL_TEXTURE_2D, myDinoTex.id());
    myDino.render();
    
    // restore previous state (no texture, no shader)
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);

    // --- Put this in the rendering loop
    // Draw the triangle
    //myShape.render();

    // --- Insert this line into your rendering loop.
    util::displayFPS(window);

    // Swap buffers, i.e. display the image and prepare for next frame
    glfwSwapBuffers(window);

    // Poll events (read keyboard and mouse input)
    glfwPollEvents();

    // Exit if the ESC key is pressed (and also if the window is closed)
    if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
      glfwSetWindowShouldClose(window, GL_TRUE);
    }
  }

  // Close the OpenGL window and terminate GLFW
  glfwDestroyWindow(window);
  glfwTerminate();
}
