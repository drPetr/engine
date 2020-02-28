#include <stdio.h>

#include <core/var.h>
#include <core/string.h>
#include <core/types.h>
#include <core/memory.h>
#include <renderer/bitmap/bitmap.h>
/*
var_t* s_vol;
var_t* s_mouse;

int main( int argc, char** argv ) {
*/    /*VarInit();
    char scl[] = "warning";

    s_vol = VarCreate( "s_vol", VAR_FLT, "0.8", "0;100", "sound volume" );
    s_mouse = VarCreate( "s_mouse", VAR_FLT, "0.5", "0;1", "sound volume" );

    VarUpdate( s_vol, "22.412323" );

    printf( "name: %s; val: %f [%f;%f]\n", s_vol->name, s_vol->f, s_vol->fmin, s_vol->fmax );
    printf( "name: %s; val: %f [%f;%f]\n", s_mouse->name, s_mouse->f, s_mouse->fmin, s_mouse->fmax );
    //printf( "%d\n", s_vol->upd );

    VarShutdown();
    return 0;*//*
}*/

#include <windows.h>
#include <gl/glext.h>
 
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

typedef struct {
    void*           data;
    uint32_t        tex;
    uint32_t        width;
    uint32_t        height;
    bitmap_t        bmp;
} texture_t;

#include <stdio.h>
/*
============
RLoadTextureFromFile
============
*/
void RLoadTextureFromFile( texture_t* tex, const char* filename ) {
    FILE* f = fopen( filename, "rb" );
    if( f ) {
        BitmapLoadFromFile( f,  &(tex->bmp) );
        fclose(f);
        
        tex->data = tex->bmp.data;
        tex->width = tex->bmp.width;
        tex->height = tex->bmp.height;
        
        glActiveTexture(GL_TEXTURE1);
        glGenTextures( 1, &(tex->tex) );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, tex->width, tex->height, 0, GL_BGR, GL_UNSIGNED_BYTE, tex->data );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
        glGenerateMipmap( GL_TEXTURE_2D );
        glBindTexture( GL_TEXTURE_2D, &(tex->tex) );
    }
}



int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    float a;
    texture_t tex;
    
    CommonInit( hInstance, nCmdShow );
    WinInitWindow();
    WinCreateWindow();
    RendererInit();
    WinShow();

    
    RLoadTextureFromFile( &tex, "bitmap.bmp" );
    
    a = 0.0f;
    /* program main loop */
    while( common.run ) {
        WinProcess();
        
        glViewport( 0, 0, width_g->i, height_g->i );
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        //glFrustum( -100, 100, -100, 100, -100, 10 );
        
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear( GL_COLOR_BUFFER_BIT );
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture( GL_TEXTURE_2D, &tex.tex );
        glPushMatrix();
        //
        glScalef( 1.0f / (float)width_g->i, 1.0f / (float)height_g->i, 1.0f );

        //glEnable(GL_SMOOTH);

        
        glRotatef( a, 0.0f, 0.0f, 1.0f );

        glBegin( GL_QUADS );
        
            
            glTexCoord2f( 0.0f, 0.0f );
            glVertex2f( 0.0f, 0.0f );
            
            glTexCoord2f( 0.0f, 1.0f );
            glVertex2f( 0.0f, 350.0f );
            
            glTexCoord2f( 1.0f, 0.0f );
            glVertex2f( 350.0f, 0.0f );
            
            glTexCoord2f( 1.0f, 1.0f );
            glVertex2f( 350.0f, 350.0f );
            

        glEnd();


        glPopMatrix();

        SwapBuffers( common.hDC );

        Sleep (1);
        a += 3.0f;
    }

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
