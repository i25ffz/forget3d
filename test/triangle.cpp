/*****************************************************************************
 * Copyright (C) 2009 The Forget3D Project by Martin Foo (i25ffz@hotmail.com)
 * ALL RIGHTS RESERVED
 *
 * License I
 * Permission to use, copy, modify, and distribute this software for
 * any purpose and WITHOUT a fee is granted under following requirements:
 * - You make no money using this software.
 * - The authors and/or this software is credited in your software or any
 *   work based on this software.
 *
 * Licence II
 * Permission to use, copy, modify, and distribute this software for
 * any purpose and WITH a fee is granted under following requirements:
 * - As soon as you make money using this software, you have to pay a
 *   licence fee. Until this point of time, you can use this software
 *   without a fee.
 * 	 Please contact Martin Foo (i25ffz@hotmail.com) for further details.
 * - The authors and/or this software is credited in your software or any
 *   work based on this software.
 *
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
 * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL THE AUTHORS
 * BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT, SPECIAL, INCIDENTAL,
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER,
 * INCLUDING WITHOUT LIMITATION, LOSS OF PROFIT, LOSS OF USE, SAVINGS OR
 * REVENUE, OR THE CLAIMS OF THIRD PARTIES, WHETHER OR NOT THE AUTHORS HAVE
 * BEEN ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
 * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
 *****************************************************************************/
#ifdef ANDROID
#include <sys/time.h>
#elif defined(__linux__)
#include <sys/time.h>
#include  <X11/Xlib.h>
#include  <X11/Xatom.h>
#include <X11/Xutil.h>
#endif

#include "f3d.h"
#include "world.h"
#include "camera.h"
#include "model.h"
#include "font.h"

using namespace F3D;

class ModelBase : public Model {
private:
    //private functions
    void prepareFrame() { /* do nothing! */};
public:
    /**
     * Destructor
     */
    virtual ~ModelBase() { /* do nothing! */};
};

//F3D variables
World*		world = NULL;
Camera*		camera = NULL;
ModelBase*  model = NULL;
F3D::Font*       font = NULL;
Image*      image = NULL;

static GLfloat rotation = 0.0f;
static char	strFps[16];
static int	fps = 0;
static int  is_done = 0;
static int  interval = 0;
static GLfloat alpha = 0.0f;

#ifdef ANDROID
static int  i_time = 0;
static struct timeval timeNow;
#elif defined(__linux__)
static int  i_time = 0;
static struct timeval timeNow;
static int  width = 480;
static int  height = 640;
static int  is_initialized = false;
static Display *x_display = NULL;
#endif

#if defined(__linux__)
int main(int argc, char *argv[]) {
    Window root;
    XSetWindowAttributes swa;
    XSetWindowAttributes  xattr;
    Atom wm_state;
    XWMHints hints;
    XEvent xev;
    EGLConfig ecfg;
    EGLint num_config;
    Window win;
    EGLNativeWindowType  hwnd;

    /*
     * X11 native display initialization
     */

    x_display = XOpenDisplay(NULL);
    if ( x_display == NULL )
    {
        return EGL_FALSE;
    }

    root = DefaultRootWindow(x_display);

    swa.event_mask  =  ExposureMask | PointerMotionMask | KeyPressMask;
    win = XCreateWindow(
               x_display, root,
               0, 0, width, height, 0,
               CopyFromParent, InputOutput,
               CopyFromParent, CWEventMask,
               &swa );

    xattr.override_redirect = false;
    XChangeWindowAttributes ( x_display, win, CWOverrideRedirect, &xattr );

    hints.input = true;
    hints.flags = InputHint;
    XSetWMHints(x_display, win, &hints);

    // make the window visible on the screen
    XMapWindow (x_display, win);
    XStoreName (x_display, win, "triangle");

    // get identifiers for the provided atom name strings
    wm_state = XInternAtom (x_display, "_NET_WM_STATE", false);

    memset ( &xev, 0, sizeof(xev) );
    xev.type                 = ClientMessage;
    xev.xclient.window       = win;
    xev.xclient.message_type = wm_state;
    xev.xclient.format       = 32;
    xev.xclient.data.l[0]    = 1;
    xev.xclient.data.l[1]    = false;
    XSendEvent (
       x_display,
       DefaultRootWindow ( x_display ),
       false,
       SubstructureNotifyMask,
       &xev );

    hwnd = (EGLNativeWindowType) win;
#else
// default main entry
int main(int argc, char *argv[]) {
#endif
    printf("world->init()...\n");

    world = World::getInstance();
#ifdef ANDROID
    world->init();
#else
    if (!world->init(hwnd)) {
        printf("Init world error!\n");

        return 0;
    }

    //after create world, set is_initialized to true
    is_initialized = true;
#endif // ANDROID

    world->setCameraCount(2);

    camera = world->getActiveCamera();
    camera->setEye(0.0f, 0.0f, 20.0f);

    camera = world->getCamera(1);
    camera->setEye(0.0f, 20.0f, 20.0f);

    world->setActiveCamera(1);

    //triangle data
    GLfloat vertices0[9] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };
    GLfloat vertices1[9] = {
        0.0f, -1.0f, -1.0f,
        0.0f, -1.0f, 1.0f,
        0.0f, 1.0f, 0.0f
    };
    GLubyte colors[12] = {
        255, 0, 0, 0,
        0, 255, 0, 0,
        0, 0, 255 ,0
    };

    model = new ModelBase();
    model->setMeshCount(2);
    //init first triangle
    model->setVertices(vertices0, 9 * sizeof(GLfloat));
    model->setColors(colors, 12 * sizeof(GLubyte));
    model->setTriangleNums(1, 0);
    //init second triangle
    colors[2] = colors[6] = 128;
    model->setVertices(vertices1, 9 * sizeof(GLfloat), 1);
    model->setColors(colors, 12 * sizeof(GLubyte), 1);
    model->setTriangleNums(1, 1);

    model->setPosition(0.0f, 0.0f, -5.0f);
    model->setScale(5.0f, 5.0f, 5.0f);

    printf("create triangle OK!\n");

    Color4f color = {1.0f, 1.0f, 1.0f, 0.2f};
    font = new F3D::Font(16, 16, "font.bmp");
    font->setFontColor(&color);

    image = new Image("f3d_logo.bmp");
    color.alpha = alpha;
    image->setImageColor(&color);

    printf("start loop...\n");
    is_done = 1;
#if defined(ANDROID) || defined(__linux___)
    gettimeofday(&timeNow, NULL);
    i_time = CLOCK(timeNow);
#endif
    sprintf(strFps, "Fps:%.2f", 0.0f);
    printf("strFps: %s\n", strFps);

    while (is_done) {
        world->prepareRender();

        model->setRotate(0.0f, rotation, 0.0f);
        model->renderModel();

        //draw f3d logo at botton left anchor
        image->drawImage(4, 4, BOTTOM_RIGHT);

        //printf("strFps: %s\n", strFps);
        color.alpha = 1.0;
        color.blue = 0.0f;
        font->setFontColor(&color);
        font->drawString(4, 4, 24, 36, strFps, TOP_LEFT);

        color.blue = 1.0f;
        font->setFontColor(&color);
        font->drawString(4, 4, 24, 36, "Tap screen!");

        world->finishRender();

        fps++;
        rotation += 2.0f;

        if (alpha <= 1.0f) {
            alpha += 0.002f;
            color.alpha = alpha;
            image->setImageColor(&color);
        }
#if defined(ANDROID) || defined(__linux___)
        gettimeofday(&timeNow, NULL);
        interval = CLOCK(timeNow) - i_time;
        if (interval >= 20000) {
            is_done = 0;
            printf("fps\t\t: %.2f\n", fps * 1000.0f / interval);
        }
        if (((CLOCK(timeNow) - i_time) / 1000) % 2 == 0 && interval > 0)
            sprintf(strFps, "Fps:%.2f", fps * 1000.0f / interval);
#endif
    }

    delete font;
    delete model;
    delete image;
    World::release();

    return 0;
}
