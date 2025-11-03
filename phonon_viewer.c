/*
Phonon Viewer (C puro, macOS, sin GLEW) — FIX GLSL 150 sin layout(location)
IA-Disclaimer: ensamblado con ayuda de IA como referencia.
*/
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#ifdef __APPLE__
  #include <OpenGL/gl3.h>
#else
  #include <GL/gl.h>
#endif
#include <GLFW/glfw3.h>

static double gT = 483.0;
static double gf = 6e12;
static double gVisHz = 1.0;
static bool   gPaused = false;
static double gPhase = 0.0;
static double gLastTime = 0.0;

static const double h  = 6.62607015e-34;
static const double kB = 1.380649e-23;

static double nbar(double T, double f){
    double x = (h*f)/(kB*T);
    double ex = exp(x);
    return 1.0/(ex - 1.0);
}

static const char* vsrc =
"#version 150 core\n"
"in vec2 aPos;\n"
"void main(){ gl_Position = vec4(aPos,0.0,1.0); }\n";

static const char* fsrc =
"#version 150 core\n"
"out vec4 FragColor;\n"
"uniform float occ;\n"
"void main(){\n"
"    float r = occ;\n"
"    float g = 0.2 + 0.6*(1.0 - occ);\n"
"    float b = 0.4;\n"
"    FragColor = vec4(r,g,b,1.0);\n"
"}\n";

static GLuint mkShader(GLenum type, const char* src){
    GLuint s = glCreateShader(type);
    glShaderSource(s,1,&src,NULL);
    glCompileShader(s);
    GLint ok=0; glGetShaderiv(s,GL_COMPILE_STATUS,&ok);
    if(!ok){ char log[2048]; glGetShaderInfoLog(s,2048,NULL,log); fprintf(stderr,"Shader error:\n%s\n",log); }
    return s;
}
static GLuint mkProgram(const char* vs, const char* fs){
    GLuint v = mkShader(GL_VERTEX_SHADER,vs);
    GLuint f = mkShader(GL_FRAGMENT_SHADER,fs);
    GLuint p = glCreateProgram();
    glAttachShader(p,v); glAttachShader(p,f);
    // GLSL 150: sin layout(location). Enlazamos el atributo 0 al nombre 'aPos' ANTES del link.
    glBindAttribLocation(p, 0, "aPos");
    glLinkProgram(p);
    GLint ok=0; glGetProgramiv(p,GL_LINK_STATUS,&ok);
    if(!ok){ char log[2048]; glGetProgramInfoLog(p,2048,NULL,log); fprintf(stderr,"Link error:\n%s\n",log); }
    glDeleteShader(v); glDeleteShader(f);
    return p;
}

static void key(GLFWwindow* w, int k, int sc, int action, int mods){
    (void)sc; (void)mods;
    if(action==GLFW_PRESS || action==GLFW_REPEAT){
        if(k==GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(w,1);
        if(k==GLFW_KEY_UP)    gT *= 1.02;
        if(k==GLFW_KEY_DOWN)  gT /= 1.02;
        if(k==GLFW_KEY_RIGHT) gf *= 1.02;
        if(k==GLFW_KEY_LEFT)  gf /= 1.02;

        if(k==GLFW_KEY_W) gT *= 1.10;
        if(k==GLFW_KEY_S) gT /= 1.10;
        if(k==GLFW_KEY_D) gf *= 1.10;
        if(k==GLFW_KEY_A) gf /= 1.10;

        if(k==GLFW_KEY_EQUAL) gVisHz *= 1.25;      // '+'
        if(k==GLFW_KEY_MINUS) gVisHz = fmax(0.05, gVisHz/1.25);
        if(k==GLFW_KEY_SPACE) gPaused = !gPaused;

        if(k==GLFW_KEY_R){
            gT=483.0; gf=6e12; gVisHz=1.0;
            gPaused=false; gPhase=0.0; gLastTime=glfwGetTime();
        }
    }
}

int main(void){
    if(!glfwInit()){ fprintf(stderr,"GLFW init failed\n"); return 1; }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,2);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
#endif
    GLFWwindow* win = glfwCreateWindow(900,300,"Phonon Viewer (C, GLSL150)",NULL,NULL);
    if(!win){ fprintf(stderr,"Window failed\n"); glfwTerminate(); return 1; }
    glfwMakeContextCurrent(win);
    glfwSetKeyCallback(win,key);
    gLastTime = glfwGetTime();

    GLuint prog = mkProgram(vsrc,fsrc);
    GLint uOcc = glGetUniformLocation(prog,"occ");

    // Geometrías
    const float y0=-0.85f, y1=-0.65f;

    GLuint vaoBar,vboBar; glGenVertexArrays(1,&vaoBar); glBindVertexArray(vaoBar);
    glGenBuffers(1,&vboBar); glBindBuffer(GL_ARRAY_BUFFER,vboBar);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*12, NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,2*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    GLuint vaoFrame,vboFrame; glGenVertexArrays(1,&vaoFrame); glBindVertexArray(vaoFrame);
    glGenBuffers(1,&vboFrame); glBindBuffer(GL_ARRAY_BUFFER,vboFrame);
    const float frame[8] = { -1.0f,y0,  1.0f,y0,  1.0f,y1,  -1.0f,y1 };
    glBufferData(GL_ARRAY_BUFFER,sizeof(frame),frame,GL_STATIC_DRAW);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,2*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    GLuint vaoLine,vboLine; glGenVertexArrays(1,&vaoLine); glBindVertexArray(vaoLine);
    glGenBuffers(1,&vboLine); glBindBuffer(GL_ARRAY_BUFFER,vboLine);
    const float midLine[4] = { -0.95f, 0.25f, 0.95f, 0.25f };
    glBufferData(GL_ARRAY_BUFFER,sizeof(midLine),midLine,GL_STATIC_DRAW);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,2*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    GLuint vaoBead,vboBead; glGenVertexArrays(1,&vaoBead); glBindVertexArray(vaoBead);
    glGenBuffers(1,&vboBead); glBindBuffer(GL_ARRAY_BUFFER,vboBead);
    glBufferData(GL_ARRAY_BUFFER,sizeof(float)*12,NULL,GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,2*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    glClearColor(0.08f,0.08f,0.10f,1.0f);

    while(!glfwWindowShouldClose(win)){
        glClear(GL_COLOR_BUFFER_BIT);

        double now = glfwGetTime();
        if(!gPaused) gPhase += 2.0*M_PI*gVisHz*(now - gLastTime);
        gLastTime = now;

        const double occ  = nbar(gT,gf);
        const double frac = fmin(1.0, occ/(occ+2.0));
        const float  xR   = -1.0f + 2.0f*(float)frac;

        // Barra
        const float barVerts[12] = {
            -1.0f,y0,   xR,y0,   xR,y1,
            -1.0f,y0,   xR,y1,  -1.0f,y1
        };
        glBindVertexArray(vaoBar);
        glBindBuffer(GL_ARRAY_BUFFER,vboBar);
        glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(barVerts),barVerts);
        glUseProgram(prog);
        glUniform1f(uOcc, (float)fmin(1.0, occ/3.0));
        glDrawArrays(GL_TRIANGLES,0,6);

        // Marco
        glBindVertexArray(vaoFrame);
        glLineWidth(2.0f);
        glUniform1f(uOcc, 0.25f);
        glDrawArrays(GL_LINE_LOOP,0,4);

        // Línea
        glBindVertexArray(vaoLine);
        glLineWidth(1.5f);
        glUniform1f(uOcc, 0.15f);
        glDrawArrays(GL_LINES,0,2);

        // Bead oscilante
        const double amp = 0.15 + 0.70 * tanh(0.7*sqrt(occ + 0.5));
        const float cx = (float)(amp * cos(gPhase));
        const float cy = 0.25f;
        const float sx = 0.035f, sy = 0.06f;
        const float bead[12] = {
            cx-sx, cy-sy,   cx+sx, cy-sy,   cx+sx, cy+sy,
            cx-sx, cy-sy,   cx+sx, cy+sy,   cx-sx, cy+sy
        };
        glBindVertexArray(vaoBead);
        glBindBuffer(GL_ARRAY_BUFFER,vboBead);
        glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(bead),bead);
        glUniform1f(uOcc, 1.0f);
        glDrawArrays(GL_TRIANGLES,0,6);

        char title[256];
        snprintf(title,sizeof(title),
            "n_bar(T=%.2f K, f=%.3e Hz) = %.6f  |  vis %.2f Hz  |  ↑/↓ T  ←/→ f  +/- vel  Esp pausa  R reset",
            gT, gf, occ, gVisHz);
        glfwSetWindowTitle(win, title);

        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1,&vaoBar);   glDeleteBuffers(1,&vboBar);
    glDeleteVertexArrays(1,&vaoFrame); glDeleteBuffers(1,&vboFrame);
    glDeleteVertexArrays(1,&vaoLine);  glDeleteBuffers(1,&vboLine);
    glDeleteVertexArrays(1,&vaoBead);  glDeleteBuffers(1,&vboBead);
    glfwDestroyWindow(win);
    glfwTerminate();
    return 0;
}
