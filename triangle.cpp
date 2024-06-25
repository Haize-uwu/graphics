#include <cstdlib>

#include <iostream>
using namespace std;

#include <GL/glew.h>
// for window and openGL context
#include <SDL2/SDL.h>

/* GLOBAL VARIABLES */
GLuint program;
GLint attribute_coord2d;


/* This is where you create the GLSL program */
bool init_resources(void){
    GLint compile_ok = GL_FALSE, link_ok = GL_FALSE;
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    const char *vs_source =
        //"#version 400" // OpenGL ES 4.6
        "#version 460\n" // OpenGL 4.6
        "in vec2 coord2d;"
        "void main(void){ "
        "  gl_Position = vec4(coord2d, 0.0, 1.0);"
        "}";

    glShaderSource(vs, 1, &vs_source , NULL);
    glCompileShader(vs);
    glGetShaderiv(vs, GL_COMPILE_STATUS, &compile_ok);
    if (!compile_ok) {
        cerr << "error in the vertex shader"<<endl;
        return false;
    }

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fs_source =
        "#version 460\n"
        "out vec4 FragColor;"
        "void main(void){"
        "  FragColor[0] = gl_FragCoord.x/640.0;"
        "  FragColor[1] = gl_FragCoord.y/480.0;"
        "  FragColor[2] = 0.5;"
        "}";

    glShaderSource(fs, 1, &fs_source, NULL);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &compile_ok);
    if (!compile_ok) {
        cerr << "Error in fragment shader"<< endl;
        return false;
    }

    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
    if (!link_ok) {
        cerr << "error in linking program" <<endl;
        return false;
    }

    /* pass each triangle point to the vertex
     * shader */

    const char* attribute_name = "coord2d";
    attribute_coord2d = glGetAttribLocation(program, attribute_name);
    if (attribute_coord2d == -1) {
        cerr << "Could not bind attribute" << endl;
        return false;
    }
    return true;
}

/* draw shit */
void render(SDL_Window* window){
    /*Clear the background*/
    glClearColor(0.0,0.0,0.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);
    glEnableVertexAttribArray(attribute_coord2d);
    GLfloat triangle_vertices[] = {
        0.0, 0.8, -0.8, -0.8, 0.8, -0.8,
    };

    /* Describe our vertices array in openGl
     * It cant guess format automatically */
    glVertexAttribPointer(
        attribute_coord2d, // attribute
        2, // number of elements per vertex
        GL_FLOAT, // type of each
        GL_FALSE, // take values as is
        0, // no extra data between each position
        triangle_vertices // pointer to C array
    );

    /* Push each element in buff_vertices to the vertex shader */
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(attribute_coord2d);

    /* Display result */
    SDL_GL_SwapWindow(window);
}

/* Destroy GLSL program */
void free_resources() { glDeleteProgram(program); }

void mainLoop(SDL_Window* window){
    while (true){
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT){
                return;
            }
        }
        render(window);
    }
}

int main(int argc, char* argv[]){
    /* Initialising SDL functions */
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow(
        "My First Triangle", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640, 480, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    SDL_GL_CreateContext(window);

    /* Extension wrangler initilising */
    GLenum glew_status = glewInit();
    if (glew_status != GLEW_OK){
        cerr << "error: glewInit: "<< glewGetErrorString(glew_status);
        return EXIT_FAILURE;
    }
    /* if all functions run without error and cant initilise
     * resources */
    if (!init_resources())
        return EXIT_FAILURE;

    /* enter main loop if everything OK */
    mainLoop(window);
    /* if program exits normally */
    free_resources();
    return EXIT_SUCCESS;
}
