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
// #include "Utilities.hpp"

// --- Add this line to your includes
#include "Utilities.hpp"

#include <vector>
// --- Add this to the includes ---------------------------------------------------
#include "Shader.hpp"

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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(),
                 GL_STATIC_DRAW);
    return bufferID;
}

/*
 * main(int argc, char *argv[]) - the standard C++ entry point for the program
 */
int main(int, char*[]) {
    // ------------------------------------------------------------------------
    // --- Put this code at the top of your main() function.
    // Vertex coordinates (x,y,z) for three vertices
    const std::vector<GLfloat> vertexArrayData = {
        -1.0f, -1.0f, 0.0f,  // First vertex, xyz
        1.0f,  -1.0f, 0.0f,  // Second vertex, xyz
        0.0f,  1.0f,  0.0f   // Third vertex, xyz
    };
    const std::vector<GLuint> indexArrayData = {0, 1, 2};

    // --- Add this after the other vertex array declarations --------------
    const std::vector<GLfloat> colorArrayData = {
        1.0f, 0.0f, 0.0f,  // Red
        0.0f, 1.0f, 0.0f,  // Green
        0.0f, 0.0f, 1.0f,  // Blue
    };

    // --- Add this to the variable declarations --------------------------------------
    Shader myShader;

    int width, height;

    const GLFWvidmode* vidmode;  // GLFW struct to hold information about the display
    GLFWwindow* window;          // GLFW struct to hold information about the window

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
    window = glfwCreateWindow(vidmode->height / 2, vidmode->height / 2, "GLprimer", NULL, NULL);
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
              << "\nDesktop size:    " << vidmode->width << " x " << vidmode->height << "\n";

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
    /*
    // Generate 1 buffer, put the resulting identifier in vertexBufferID
    GLuint vertexBufferID = 0;
    glGenBuffers(1, &vertexBufferID);
    // Activate the vertex buffer object
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    // Present our vertex coordinates to OpenGL
    glBufferData(GL_ARRAY_BUFFER, vertexArrayData.size() * sizeof(GLfloat), vertexArrayData.data(),
    GL_STATIC_DRAW);
    // Specify the format of the data in the vertex buffer, and copy the data.
    // The six arguments specify, from left to right:
    // Attribute 0, must match the "layout" statement in the shader.
    // Dimensions 3, means 3D (x,y,z) - this becomes a vec3 in the shader.
    // Type GL_FLOAT, means we have "float" input data in the array.
    // GL_FALSE means "no normalization". This has no meaning for float data.
    // Stride 0, meaning (x,y,z) values are packed tightly together without gaps.
    // Array buffer offset 0 means our data starts at the first element.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    // Enable vertex attribute array 0 to send xyz coordinates to the shader.
    glEnableVertexAttribArray(0);

    // --- Add this after glEnableVertexAttribArray(0) ---------------------
    // Generate a second vertex buffer, activate it and copy data to it
    GLuint colorBufferID = 0;  // Vertex colors
    glGenBuffers(1, &colorBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
    glBufferData(GL_ARRAY_BUFFER, colorArrayData.size() * sizeof(GLfloat), colorArrayData.data(),
    GL_STATIC_DRAW);
    // Tell OpenGL how the data is stored in our color buffer
    // Attribute #1, 3 dimensions (R,G,B -> vec3 in the shader),
    // type GL_FLOAT, not normalized, stride 0, start at element 0
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    // Enable a second attribute (in this case, to hold vertex colors)
    glEnableVertexAttribArray(1);


    // Generate 1 buffer, put the resulting identifier in indexBufferID
    GLuint indexBufferID = 0;
    glGenBuffers(1, &indexBufferID);
    // Activate (bind) the index buffer and copy data to it.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    // Present our vertex indices to OpenGL
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArrayData.size() * sizeof(GLuint),
    indexArrayData.data(), GL_STATIC_DRAW);
    // Deactivate the vertex array object again to be nice
    glBindVertexArray(0);
    */
    // --- Add this in main() after glewInit() and before the rendering loop ----
    myShader.createShader("../shaders/vertex.glsl", "../shaders/fragment.glsl");

    // My test code
    float red = 0.3f;
    float green = 0.3f;
    float blue = 0.3f;
    int counter = 0;

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // move to while loop
        glfwGetWindowSize(window, &width, &height);
        glViewport(0, 0, width, height);

        // Set the clear color to a dark gray (RGBA)
        glClearColor(red, green, blue, 0.0f);
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
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

        counter++;
        if (counter == 1000) {
            red += red + 0.01f;
            green += green + 0.02f;
            blue += blue + 0.03f;
            counter = 0;
        }

        if (red > 0.9f) {
            red = 0.0f;
        }
        if (green > 0.9f) {
            green = 0.0f;
        }
        if (blue > 0.9f) {
            blue = 0.0f;
        }

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
