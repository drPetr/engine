#include <stdio.h>
#include <stdlib.h>

#include <core/var.h>
#include <core/string.h>
#include <core/types.h>
#include <core/memory.h>
#include <renderer/gl/gl.h>
#include <windows.h>
#include <core/fs.h>
#include <core/debug.h>
 
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
        "engine",
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






/*
FsFprintf();
FsVfprintf();
FsFgetc();
FsGets();

int FsCreate( const char* fileName, uint32_t flags );
int FsRemove( const char* fileName );
FsRename( const char* fileName, const char* newFileName );
*/

#include <stdlib.h>


char* LoadFileData( const char* fileName ) {
    file_t* f;
    char* str;
    size_t size;
    
    if( NULL == (f = FileOpen( fileName, F_READ | F_LOCAL )) ) {
        return NULL;
    }
    
    size = FileSize( f );
    str = malloc( size + 100 );
    FileRead( str, size, f );
    str[size] = 0;
    FileClose( f );
    
    return str;
}

void DeleteFileData( char* str ) {
    free( str );
}



GLuint LoadShader( const char* fileName, GLenum type ) {
    
    GLuint sh;
    GLint code;
    char log[1024];
    char* shsrc;
    
    // vertex shader
    shsrc = LoadFileData( fileName );
    if( shsrc == NULL ) {
        printf( "error: cannot load shader '%s'\n", fileName );
        DeleteFileData( shsrc );
        return 0;
    }
    
    sh = glCreateShader( type );
    glShaderSource( sh, 1, (const GLchar**)&shsrc, NULL );
    glCompileShader( sh );
    glGetShaderiv( sh, GL_COMPILE_STATUS, &code );
    if( !code ) {
        glGetShaderInfoLog( sh, sizeof(log), NULL, log );
        printf( "error: cannot compile shader\n" );
        printf( "%s\n", log );
        glDeleteShader( sh );
        sh = 0;   
    }
    
    DeleteFileData( shsrc );
    
    return sh;
}


GLuint LoadShaderProgram( const char* vshName, const char* pshName ) {
    GLuint vsh;
    GLuint psh;
    GLuint program = 0;
    GLint code;
    char log[1024];
    
    vsh = LoadShader( vshName, GL_VERTEX_SHADER );
    psh = LoadShader( pshName, GL_FRAGMENT_SHADER );
    
    if( vsh == 0 || psh == 0 ) {
        goto goret;
    }
    
    program = glCreateProgram();
    glAttachShader( program, vsh );
    glAttachShader( program, psh );
    glLinkProgram( program );
    glGetProgramiv( program, GL_LINK_STATUS, &code );
    if( !code ) {
        glGetProgramInfoLog( program, sizeof(log), NULL, log );
        printf( "error: cannot make shader program\n" );
        printf( "%s\n", log );
    }
    
goret:
    if( vsh ) {
        glDeleteShader( vsh );
    }
    if( psh ) {
        glDeleteShader( psh );
    }
   
    return program;
}

enum varInfoFlags_t {
    INFO_INT8       = 0x01,
    INFO_UINT8      = 0x02,
    INFO_INT16      = 0x03,
    INFO_UINT16     = 0x04,
    INFO_INT32      = 0x05,
    INFO_UINT32     = 0x06,
    INFO_INT64      = 0x07,
    INFO_UINT64     = 0x08,
    INFO_FLT        = 0x09,
    INFO_DBL        = 0x0a,
    INFO_TYPE_MASK  = 0xff,
    
    INFO_HEX_VIEW   = 0x200
};



typedef struct infoVar_t {
    uint32_t    flags;
    uint32_t    indent;
    char        varName[32];
    char        varDescr[64];
    struct infoVar_t*    next;
} infoVar_t;



typedef struct {
    infoVar_t*      first;
    infoVar_t*      last;
} infoStruct_t;



void InfoVarAppend( infoStruct_t* info, const char* name, const char* descr, uint32_t indent, uint32_t flags ) {
    infoVar_t* last = info->last;
    
    if( !last ) {
        info->first = (infoVar_t*)malloc( sizeof(infoVar_t) );
        info->last = info->first;
        last = info->first;
    } else {
        info->last->next = (infoVar_t*)malloc( sizeof(infoVar_t) );
        info->last = info->last->next;
        last = info->last;
    }
    
    last->flags = flags;
    last->indent = indent;
    StrCpy( last->varName, name );
    StrCpy( last->varDescr, descr );
    last->next = NULL;
}



void VarInfo( infoStruct_t* info, const void* ptr ) {
    infoVar_t* vinfo = info->first;
    char* p = (char*)ptr;
    
    while( vinfo ) {
        
        printf( "%s: ", vinfo->varName );
        
        switch( vinfo->flags & INFO_TYPE_MASK ) {
            case INFO_INT8:
                printf( "%d", (int32_t)(*(int8_t*)p) );
                break;
            case INFO_UINT8:
                printf( "%u", (uint32_t)(*(uint8_t*)p) );
                break;
            case INFO_INT16:
                printf( "%d", (int32_t)(*(int16_t*)p) );
                break;
            case INFO_UINT16:
                printf( "%u", (uint32_t)(*(uint16_t*)p) );
                break;
            case INFO_INT32:
                printf( "%d", (int32_t)(*(int32_t*)p) );
                break;
            case INFO_UINT32:
                 printf( "%u", (uint32_t)(*(uint32_t*)p) );
                break;
            case INFO_INT64:
                break;
            case INFO_UINT64:
                break;
            case INFO_FLT:
                break;
            case INFO_DBL:
                break;
            default:
                printf( "error: unknown typename\n" );
                goto gonext;
                break;
        }
        
        printf( "\n" );
        
gonext:
        vinfo = vinfo->next;
    }
}






void PrintZipLocalHeader( zipLocalHeader_t* h ) {
    // uint32_t        signature;      // local file header signature (0x04034b50)
    printf( "signature: %x\n", h->signature );
    // uint16_t        extractVer;     // version needed to extract
    printf( "extractVer: %u\n", h->extractVer );
    // uint16_t        flags;          // general purpose bit flag
    printf( "flags: %u\n", h->flags );
    // uint16_t        compress;       // compression method
    printf( "compress: %u\n", h->compress );
    // uint16_t        ftime;          // last mod file time
    printf( "ftime: %u\n", h->ftime );
    // uint16_t        fdate;          // last mod file date
    printf( "fdate: %u\n", h->fdate );
    // uint32_t        crc32;          // crc-32
    printf( "crc32: %x\n", h->crc32 );
    // uint32_t        compressSize;   // compressed size
    printf( "compressSize: %u\n", h->compressSize );
    // uint32_t        uncompressSize; // uncompressed size
    printf( "uncompressSize: %u\n", h->uncompressSize );
    // uint16_t        fnameLen;       // file name length
    printf( "fnameLen: %u\n", h->fnameLen );
    // uint16_t        extraLen;       // extra field length
    printf( "extraLen: %u\n", h->extraLen );
}

void PrintEndOfCD( zipEndOfCD_t* e ) {
    // uint32_t        signature;      // end of central dir signature (0x06054b50)
    printf( "signature: %x\n", e->signature );
    // uint16_t        diskNum;        // number of this disk
    printf( "diskNum: %u\n", e->diskNum );
    // uint16_t        diskNumCD;      // number of the disk with the start of the central directory
    printf( "diskNumCD: %u\n", e->diskNumCD );
    // uint16_t        totalNumInDisk; // total number of entries in the central directory on this disk
    printf( "totalNumInDisk: %u\n", e->totalNumInDisk );
    // uint16_t        totalNumInCD;   // total number of entries in the central directory
    printf( "totalNumInCD: %u\n", e->totalNumInCD );
    // uint32_t        sizeOfCD;       // size of the central directory
    printf( "sizeOfCD: %u\n", e->sizeOfCD );
    // uint32_t        offset;         // offset of start of central directory with respect to the starting disk number
    printf( "offset: %u\n", e->offset );
    // uint16_t        comLen;         // ZIP file comment length
    printf( "comLen: %u\n", e->comLen );
}


/*
============
ZipProc
============
*/
int ZipProc( file_t* f, const zipLocalHeader_t* lfh, ssize_t offset, 
            const char* fileName, const char* extra, void* userData ) {
    
    //printf( "%s\n", fileName );
    
    return 0;
}





int main( int argc, char** argv ) {
    file_t* file;
    file_t* f;
    ecode_t e;
    
    // open the file
    if( NULL == (file = FileOpen( "data/test.zip", F_LOCAL | F_READ | F_BINARY )) ) {
        printf( "error: cannot open file 'data/test.zip'\n" );
        return 0;
    }
    
    // open the file
    if( NULL == (f = FileOpen( "data/test.zip", F_LOCAL | F_READ | F_BINARY )) ) {
        printf( "error: cannot open file 2 'data/test.zip'\n" );
        return 0;
    }
    
    if( E_OK != (e = ZipCheck( file )) ) {
        printf( "e_error: %d\n", e );
    }
    if( E_OK != (e = ZipProcess( file, ZipProc, NULL )) ) {
        printf( "e_error: %d\n", e );
    }
    
    FileClose( file );
    FileClose( f );
    
    return 0;
}

/*
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    
    
    
    
    
    */
    /*const float prim[] = {
        -0.5f, -0.5f, 1.0f,
        0.5f,  -0.5f, -1.0f,
        0.0f,   0.5f, 0.0f
    };
    
    GLuint vbo; // vertex buffer objects (VBO)
    GLuint psh; // shader program
    GLuint vao;
    GLuint quadvbo;
    //GLuint glprogram;   // shader
    
    CommonInit( hInstance, nCmdShow );
    WinInitWindow();
    WinCreateWindow();
    RendererInit();
    WinShow();


    // объекты вершинного буфера (vertex buffer objects (VBO))
    glGenBuffers( 1, &vbo );
    glBindBuffer( GL_ARRAY_BUFFER, vbo ); 
    glBufferData( GL_ARRAY_BUFFER, sizeof(prim), prim, GL_STATIC_DRAW );

    psh = LoadShaderProgram( "shader.vsh", "shader.psh" );
    
    // 
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0 );
    glEnableVertexAttribArray( 0 );

    glClearColor( 0.3f, 0.3f, 0.3f, 0.0f );

    while( common.run ) {
        WinProcess();
        glClear( GL_COLOR_BUFFER_BIT );
        glViewport( 0, 0, width_g->i, height_g->i );
        //glScalef( 1.0f / (float)width_g->i, 1.0f / (float)height_g->i, 1.0f );
        
        glBindBuffer( GL_ARRAY_BUFFER, vbo );
        glBufferData( GL_ARRAY_BUFFER, sizeof(prim), prim, GL_STATIC_DRAW );
        
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0 );

        glUseProgram( psh );
        
        glDrawArrays( GL_TRIANGLES, 0, 3 ); 
        
        SwapBuffers( common.hDC );
        Sleep (1);
    }
    
    glDeleteBuffers( 1, &quadvbo );
    glDeleteVertexArrays( 1, &vao );

    RendererShutdown();
    WinShutdown();
    CommonShutdown();
 
    return common.exitCode;*//*
}*/

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
