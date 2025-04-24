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
/*
std::vector<GLfloat> matrixMult(std::vector<GLfloat> mat1, std::vector<GLfloat> mat2) {
    std::vector<GLfloat> resultArray; 
    

    int row = 0;
    for (int i = 0; i < 16; i++) {

        resultArray[i] = 0;

        for (int j = 0; j < 4; j++) {
            
            for (int k = 0; k < 4; k++) {
                resultArray[i] += mat1[j + row * 4] * mat2[j * 4 + k];
            }
            row++;
        }
    }
    return resultArray;
}
*/

/*
1, 0, 0, 0,
0, cos(x), -sin(x), 0,
0, sin(x), cos(x), 0
0, 0, 0, 1

cos(x), 0, -sin(x), 0,
0, 1, 0, 0,
sin(x), 0, cos(x), 0
0, 0, 0, 1

cos(x), -sin(x), 0, 0,
sin(x), cos(x), 0, 0,
0, 0, 1, 0
0, 0, 0, 1


*/

// Multiply 4x4 matrices m1 and m2 and return the result
std::array<float, 16> mat4mult(const std::array<float, 16>& m1, const std::array<float, 16>& m2) {
    std::array<float, 16> result;
    // Your code goes here: compute and set each element of result, e.g.:6// result[0] = m1[0] * m2[0] +
    // m1[4] * m2[1] + m1[8] * m2[2] + m1[12] * m2[3];7// etc. for the remaining 15 elements.8return
    
    result[0] = m1[0] * m2[0] + m1[4] * m2[1] + m1[8] * m2[2] + m1[12] * m2[3];
    result[4] = m1[1] * m2[0] + m1[5] * m2[1] + m1[9] * m2[2] + m1[13] * m2[3];
    result[8] = m1[2] * m2[0] + m1[6] * m2[1] + m1[10] * m2[2] + m1[14] * m2[3];
    result[12] = m1[3] * m2[0] + m1[7] * m2[1] + m1[11] * m2[2] + m1[15] * m2[3];

    result[1] = m1[0] * m2[4] + m1[4] * m2[5] + m1[8] * m2[6] + m1[12] * m2[7];
    result[5] = m1[1] * m2[4] + m1[5] * m2[5] + m1[9] * m2[6] + m1[13] * m2[7];
    result[9] = m1[2] * m2[4] + m1[6] * m2[5] + m1[10] * m2[6] + m1[14] * m2[7];
    result[13] = m1[3] * m2[4] + m1[7] * m2[5] + m1[11] * m2[6] + m1[15] * m2[7];

    result[2] = m1[0] * m2[8] + m1[4] * m2[9] + m1[8] * m2[10] + m1[12] * m2[11];
    result[6] = m1[1] * m2[8] + m1[5] * m2[9] + m1[9] * m2[10] + m1[13] * m2[11];
    result[10] = m1[2] * m2[8] + m1[6] * m2[9] + m1[10] * m2[10] + m1[14] * m2[11];
    result[14] = m1[3] * m2[8] + m1[7] * m2[9] + m1[11] * m2[10] + m1[15] * m2[11];

    result[3] = m1[0] * m2[12] + m1[4] * m2[13] + m1[8] * m2[14] + m1[12] * m2[15];
    result[7] = m1[1] * m2[12] + m1[5] * m2[13] + m1[9] * m2[14] + m1[13] * m2[15];
    result[11] = m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15];
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
        cos(angle), 0.0f, -sin(angle), 0.0f, 
        0.0f,       1.0f, 0.0f,         0.0f,                          
        sin(angle), 0.0f, cos(angle), 0.0f, 
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

/*
 * main(int argc, char *argv[]) - the standard C++ entry point for the program
 */
int main(int, char *[]) {

    

    // ------------------------------------------------------------------------
    // --- Put this code at the top of your main() function.
    // Vertex coordinates (x,y,z) for three vertices
    /*
    const std::vector<GLfloat> vertexArrayData = {
        -1.0f, -1.0f, 0.0f,  // First vertex, xyz
        1.0f,  -1.0f, 0.0f,  // Second vertex, xyz
        0.0f,  1.0f, 0.0f  // Third vertex, xyz
    };
    const std::vector<GLuint> indexArrayData = {0, 1, 2};

    // --- Add this after the other vertex array declarations --------------
    const std::vector<GLfloat> colorArrayData = {
        1.0f, 0.0f, 0.0f,  // Red
        0.0f, 1.0f, 0.0f,  // Green
        0.0f, 0.0f, 1.0f,  // Blue
    };
    */
    
    const std::vector<GLfloat> vertexArrayData = {
        1.0f, -1.0f, -1.0f,  // 0
        1.0f, 1.0f, -1.0f,  // 1
        1.0f, 1.0f, 1.0f,  // 2
        1.0f,  -1.0f, 1.0f,  // 3

        -1.0f,  -1.0f, -1.0f,   // 4
        -1.0f, -1.0f, 1.0f,  // 5
        -1.0f, 1.0f, -1.0f,  // 6
        -1.0f, 1.0f, 1.0f, // 7
    };
    const std::vector<GLuint> indexArrayData = {
        0, 1, 3,    1, 2, 3, // face 1
        1, 6, 7,    1, 7, 2, // face 2
        3, 2, 7,    3, 7, 5, // face 3
        0, 3, 5,    0, 5, 4, // face 4
        0, 6, 1,    0, 4, 6, // face 5
        6, 4, 5,    6, 5, 7, // face 6
    };
    // --- Add this after the other vertex array declarations --------------
    const std::vector<GLfloat> colorArrayData = {
        0.0f, 0.0f, 0.0f,  //0
        1.0f, 0.7f, 0.7f,  //1
        0.6f, 0.0f, 0.6f,  //2
        1.0f, 1.0f, 0.0f,  //3
        1.0f, 1.0f, 1.0f,  //4
        0.0f, 0.5f, 1.0f,  //5
        1.0f, 1.0f, 0.0f,  //6
        1.0f, 0.0f, 1.0f,  //7
    };

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

  // ------------------------------------------------------------------------
  // ---- Put this code after glewInit(), but before the rendering loop

  // Generate 1 Vertex array object, put the resulting identifier in vertexArrayID
  GLuint vertexArrayID = 0;
  glGenVertexArrays(1, &vertexArrayID);
  // Activate the vertex array object
  glBindVertexArray(vertexArrayID);


  // Create the vertex buffer objects for attribute locations 0 and 1
  // (the list of vertex coordinates and the list of vertex colors).
  GLuint vertexBufferID = createVertexBuffer(0, 3, vertexArrayData);
  GLuint colorBufferID = createVertexBuffer(1, 3, colorArrayData);
  // Create the index buffer object (the list of triangles).
  GLuint indexBufferID = createIndexBuffer(indexArrayData);
  // Deactivate the vertex array object again to be nice
  glBindVertexArray(0);
  
  // --- Add this in main() after glewInit() and before the rendering loop ----
  myShader.createShader("../shaders/vertex.glsl", "../shaders/fragment.glsl");

  
  // Do this before the rendering loop

  GLint locationTime = glGetUniformLocation(myShader.id(), "time");
  if (locationTime == -1) {  // If the variable is not found, -1 is returned
      std::cout << "Unable to locate variable'time'in shader!\n";
  }

  mat4print(mat4translate(6, 3, 5));
  mat4print(mat4mult(mat4translate(6, 3, 5), mat4scale(2)));

  glEnable(GL_CULL_FACE);

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
    
    // --- Add this to the rendering loop, right before the call to glBindVertexArray()
    glUseProgram(myShader.id());

    // ------------------------------------------------------------------------
    // ---- Put the following code in the rendering loop
    // Activate the vertex array object we want to draw (we may have several)
    glBindVertexArray(vertexArrayID);
    // Draw our triangle with 3 vertices.
    // When the last argument of glDrawElements is nullptr, it means
    // "use the previously bound index buffer". (This is not obvious.)
    // The index buffer is part of the VAO state and is bound with it.

    //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr); // was 3
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // GL_FILL
    //glCullFace(GL_BACK);
    //
    //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr); // was 3
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // GL_LINE
    //glCullFace(GL_FRONT);

    glDrawElements(GL_TRIANGLES, indexArrayData.size() * sizeof(GLuint), GL_UNSIGNED_INT, nullptr);                    // was 3
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);                   // GL_FILL
    glCullFace(GL_BACK);

    glDrawElements(GL_TRIANGLES, indexArrayData.size() * sizeof(GLuint), GL_UNSIGNED_INT, nullptr);                    // was 3
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);                   // GL_LINE
    glCullFace(GL_FRONT);
    
    // Do this in the rendering loop to update the uniform variable "time"
    float time = static_cast<float>(glfwGetTime());  // Number of seconds since the program was started
    glUseProgram(myShader.id());            // Activate the shader to set its variables
    glUniform1f(locationTime, time);        // Copy the value to the shader program

    std::array<GLfloat, 16> matVAngle = mat4rotx(15 * (M_PI/180));
    std::array<GLfloat, 16> matROrbit = mat4roty(1 * (time / (2 * M_PI)));
    std::array<GLfloat, 16> matT = mat4translate(0, 0, 0.5);
    std::array<GLfloat, 16> matRSpin = mat4roty(5 * (time / M_PI));
    std::array<GLfloat, 16> matTransformation = mat4mult(mat4mult(mat4mult(matVAngle, matROrbit), matT), matRSpin);
    /*
    std::array<GLfloat, 16> matScen = mat4rotx(45);
    std::array<GLfloat, 16> matMinska = mat4scale(1);
    std::array<GLfloat, 16> matOrbit = mat4roty(time / 2);
    std::array<GLfloat, 16> matSpin = mat4rotz(time);
    std::array<GLfloat, 16> matFlytt = mat4translate(0.5f, 0.0f, 0.0f);

    std::array<GLfloat, 16> matA = mat4mult(matSpin, matMinska);
    std::array<GLfloat, 16> matB = mat4mult(matFlytt, matA);
    std::array<GLfloat, 16> matC = mat4mult(matOrbit, matB);
    std::array<GLfloat, 16> matM = mat4mult(matScen, matC);
    */
    std::array<GLfloat, 16> matV = matTransformation;
    GLint locationV = glGetUniformLocation(myShader.id(), "V");
    glUseProgram(myShader.id());  // Activate the shader to set its variables
    glUniformMatrix4fv(locationV, 1, GL_FALSE, matV.data());  // Copy the value

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

  // ------------------------------------------------------------------------
  // ---- Put the following code directly after the rendering loop next to the others
  // release the color buffers
  glDeleteBuffers(1, &colorBufferID);

  // ------------------------------------------------------------------------
  // ---- Put the following code directly after the rendering loop (before glfwDestroyWindow())
  // release the vertex and index buffers as well as the vertex array
  glDeleteVertexArrays(1, &vertexArrayID);
  glDeleteBuffers(1, &vertexBufferID);
  glDeleteBuffers(1, &indexBufferID);

  // Close the OpenGL window and terminate GLFW
  glfwDestroyWindow(window);
  glfwTerminate();
}
