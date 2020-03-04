#include <stdio.h>
#include <stdlib.h>

#include <core/var.h>
#include <core/string.h>
#include <core/types.h>
#include <core/memory.h>
#include <renderer/gl/gl.h>
#include <windows.h>
 
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);
 


typedef struct {
    WNDCLASSEX              wcex;
    HWND                    hWnd;
    HDC                     hDC;
    HINSTANCE               hInst;
    int                     nCmdShow;
    HGLRC                   hRC;
    
    //
    bool_t                  run;    // application is run
    int                     exitCode;   // application exit code
} common_t;


common_t common;
var_t* width_g;
var_t* height_g;

/*
============
CommonInit
============
*/
void CommonInit( HINSTANCE hInst, int nCmdShow ) {
    MemZero( &common, sizeof(common) );
    common.hInst = hInst;
    common.nCmdShow = nCmdShow;
    
    width_g = VarCreate( "width_g", VAR_INT, "1366", NULL, "client screen width" );
    height_g = VarCreate( "height_g", VAR_INT, "768", NULL, "client screen height" );
}

/*
============
CommonShutdown
============
*/
void CommonShutdown( void ) {
    MemZero( &common, sizeof(common) );
}

/*
============
WinInitWindow
============
*/
void WinInitWindow( void ) {
    common.wcex.cbSize =        sizeof(WNDCLASSEX);
    common.wcex.style =         CS_OWNDC;
    common.wcex.lpfnWndProc =   WindowProc;
    common.wcex.cbClsExtra =    0;
    common.wcex.cbWndExtra =    0;
    common.wcex.hInstance =     common.hInst;
    common.wcex.hIcon =         LoadIcon(NULL, IDI_APPLICATION);
    common.wcex.hCursor =       LoadCursor(NULL, IDC_ARROW);
    common.wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    common.wcex.lpszMenuName =  NULL;
    common.wcex.lpszClassName = "ClassWndEngineApp";
    common.wcex.hIconSm =       LoadIcon(NULL, IDI_APPLICATION);
    
    if( !RegisterClassEx(&common.wcex) ) {
        perror( "RegisterClassEx failed" );
        exit(1);
    }
}

/*
============
WinCreateWindow
============
*/
void WinCreateWindow( void ) {
    common.hWnd = CreateWindowEx(
        0,
        "ClassWndEngineApp",
        "Wngine",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        width_g->i,
        height_g->i,
        NULL,
        NULL,
        common.hInst,
        NULL
    );
    common.run = btrue;
}

/*
============
WinShutdown
============
*/
void WinShutdown( void ) {
    // destroy the window explicitly
    DestroyWindow( common.hWnd );
}

void WinProcess( void ) {
    bool_t peek;
    MSG msg;
    
    // process windows messages
    do {
        if( (peek = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) ) {
            if( msg.message == WM_QUIT ) {
                common.run = bfalse;
                common.exitCode = msg.wParam;
            } else {
                TranslateMessage( &msg );
                DispatchMessage( &msg );
            }
        }
    } while( peek );
}

/*
============
RendererInit
============
*/
void RendererInit( void ) {
    PIXELFORMATDESCRIPTOR pfd;
    int fmt;
 
    /* get the device context (DC) */
    common.hDC = GetDC( common.hWnd );
 
    /* set the pixel format for the DC */
    MemZero( &pfd, sizeof(pfd) );
 
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;
 
    fmt = ChoosePixelFormat( common.hDC, &pfd );
 
    SetPixelFormat( common.hDC, fmt, &pfd );
 
    /* create and enable the render context (RC) */
    common.hRC = wglCreateContext( common.hDC );
    
    wglMakeCurrent( common.hDC, common.hRC );
}

/*
============
RendererShutdown
============
*/
void RendererShutdown( void ) {
    wglMakeCurrent( NULL, NULL );
    wglDeleteContext( common.hRC );
    ReleaseDC( common.hWnd, common.hDC );
}

/*
============
WinShow
============
*/
void WinShow( void ) {
    ShowWindow( common.hWnd, common.nCmdShow );
}




    
const char vertexShaderSrc[] =
"#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"void main() {\n"
"    gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\n";

const char fragmentShaderSrc[] =
"#version 330 core\n"
"out vec4 color;\n"
"void main() {\n"
"    color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n";


void inf() {
    static int ok = 1;
    printf( "ok %d\n", ok++ );
}

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
                    
    const float prim[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f,  -0.5f, 0.0f,
        0.0f,   0.5f, 0.0f
    };
    GLuint vbo; // vertex buffer objects (VBO)
    GLuint vsh; // vertex shader
    GLuint fsh; // fragment shader
    GLuint psh; // shader program
    GLint code;
                    
    GLuint vao;
    GLuint quadvbo;
    //GLuint glprogram;   // shader

    char log[1024];
    
    CommonInit( hInstance, nCmdShow );
    WinInitWindow();
    WinCreateWindow();
    RendererInit();
    WinShow();


    // объекты вершинного буфера (vertex buffer objects (VBO))
    glGenBuffers( 1, &vbo );
    glBindBuffer( GL_ARRAY_BUFFER, vbo ); 
    glBufferData( GL_ARRAY_BUFFER, sizeof(prim), prim, GL_STATIC_DRAW );


    // vertex shader
    vsh = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vsh, 1, (const char**)&vertexShaderSrc, NULL );
    glCompileShader( vsh );
    glGetShaderiv( vsh, GL_COMPILE_STATUS, &code );
    if( !code ) {
        glGetShaderInfoLog( vsh, sizeof(log), NULL, log );
        printf( "error: cannot compile vertex shader\n" );
        printf( "%s\n", log );
    }
    
    
    // fragment shader
    fsh = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fsh, 1, (const char**)&fragmentShaderSrc, NULL );
    glCompileShader( fsh );
    glGetShaderiv( fsh, GL_COMPILE_STATUS, &code );
    if( !code ) {
        glGetShaderInfoLog( fsh, sizeof(log), NULL, log );
        printf( "error: cannot compile fragment shader\n" );
        printf( "%s\n", log );
    }


    // make shader program
    psh = glCreateProgram();
    glAttachShader( psh, vsh );
    glAttachShader( psh, fsh );
    glLinkProgram( psh );
    glGetProgramiv( psh, GL_LINK_STATUS, &code );
    if( !code ) {
        glGetProgramInfoLog( psh, sizeof(log), NULL, log );
        printf( "error: cannot make shader program\n" );
        printf( "%s\n", log );
    }
    
    
    // delete shaders
    glDeleteShader( vsh );
    glDeleteShader( fsh );
    
    
    // 
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0 );
    glEnableVertexAttribArray( 0 );


    while( common.run ) {
        WinProcess();
        glClear( GL_COLOR_BUFFER_BIT );
        glViewport( 0, 0, width_g->i, height_g->i );
        //glScalef( 1.0f / (float)width_g->i, 1.0f / (float)height_g->i, 1.0f );
        
        glBindBuffer( GL_ARRAY_BUFFER, vbo );
        glBufferData( GL_ARRAY_BUFFER, sizeof(prim), prim, GL_STATIC_DRAW );
        
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0 );

        glUseProgram( psh );
        
        //glDrawBuffer( vbo );
        
        SwapBuffers( common.hDC );
        Sleep (1);
    }
    
    glDeleteBuffers( 1, &quadvbo );
    glDeleteVertexArrays( 1, &vao );

    RendererShutdown();
    WinShutdown();
    CommonShutdown();
 
    return common.exitCode;
}

LRESULT CALLBACK WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
    switch( uMsg ) {
        case WM_CLOSE:
            PostQuitMessage(0);
            break;
        case WM_DESTROY:
            return 0;
        case WM_KEYDOWN:
            switch( wParam ) {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                break;
            }
            break;
        case WM_SIZE:
            VarUpdateInt( width_g, (int)LOWORD(lParam) );
            VarUpdateFloat( height_g, (int)HIWORD(lParam) );
            break;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
 
    return 0;
}
