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

#include "world.h"
#include "mathlib.h"

namespace F3D {
    /**
     * World class for all games using F3D.
     */

    World* World::m_instance = NULL;

    World::World() :
#ifndef ANDROID_NDK
            m_display(EGL_NO_DISPLAY),
            m_context(EGL_NO_CONTEXT),
            m_surface(EGL_NO_SURFACE),
#endif
			m_width(0),
			m_height(0),
            m_bgred(0.0f),
            m_bggreen(0.0f),
            m_bgblue(0.0f),
            m_bgalpha(0.5f),
            m_fovy(60.0f),
            m_znear(1.0f),
            m_zfar(1000.0f),
            m_cameras(NULL),
            m_fog(NULL),
            m_light(NULL) {
#ifdef DEBUG
        printf("World constructor...\n");
#endif
        setCameraCount(1);
        setActiveCamera(0);
    }

    World::~World() {
        deinitEGL();

        DELETEANDNULL(m_cameras, true);
        DELETEANDNULL(m_fog, false);
        DELETEANDNULL(m_light, false);
#ifdef DEBUG
        printf("World destructor...\n");
#endif
    }

    World* World::getInstance() {
        if (m_instance == NULL) {
            m_instance = new World();
        }

        return m_instance;
    }

    void World::release() {
        if (m_instance != NULL)
            delete m_instance;

        m_instance = NULL;
    }

    void World::setBgColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
        m_bgred = red;
        m_bggreen = green;
        m_bgblue = blue;
        m_bgalpha = alpha;
    }

    // camera functions
    void World::setCameraCount(int cameraCount) {
        if (m_cameras != NULL) {
            delete[] m_cameras;
            m_cameras = NULL;
        }

        if (cameraCount > 0) {
#ifdef DEBUG
            printf("World create [%d] cameras ...\n", cameraCount);
#endif
            m_cameraCount = cameraCount;
            // create meshs
            m_cameras = new Camera[m_cameraCount];
        }
    }

    int World::getCameraCount() {
        return m_cameraCount;
    }

    Camera *World::getCamera(int cameraIndex) {
        return &m_cameras[cameraIndex];
    }

    void World::setActiveCamera(int cameraIndex) {
        m_cameraIndex = cameraIndex;
    }

    Camera *World::getActiveCamera() {
        return &m_cameras[m_cameraIndex];
    }

    void World::setFog(Fog* fog) {
        m_fog = fog;
    }

    void World::setLight(Light* light) {
        m_light = light;
    }

    bool World::initEGL() {
#ifndef ANDROID_NDK
        EGLint config_nums = -1;
        EGLint maj_ver;
        EGLint min_ver;
        EGLConfig config;

		EGLint config_attribs[] = {
            EGL_RED_SIZE,       5,
            EGL_GREEN_SIZE,     6,
            EGL_BLUE_SIZE,      5,
			EGL_DEPTH_SIZE,    16,
			EGL_STENCIL_SIZE,   0,
            EGL_NONE
        };


        //start init EGL
        m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

        if (!checkEglError("eglGetDisplay"))
            return false;

        eglInitialize(m_display, &maj_ver, &min_ver);
#ifdef DEBUG
        printf("maj_ver: %d, min_ver: %d\n", maj_ver, min_ver);
#endif
        if (!checkEglError("eglInitialize"))
            return false;

        eglGetConfigs(m_display, NULL, 0, &config_nums);
#ifdef DEBUG
        printf("config_nums: %d\n", config_nums);
#endif
        if (!checkEglError("eglGetConfigs"))
            return false;

        eglChooseConfig(m_display, config_attribs, &config, 1, &config_nums);
        if (!checkEglError("eglChooseConfig"))
            return false;
#if defined(__linux__)
        m_surface = eglCreateWindowSurface(m_display, config, m_hwnd, NULL);
#else
        m_surface = eglCreateWindowSurface(m_display, config,
                                           android_createDisplaySurface(), NULL);
#endif
        if (!checkEglError("eglCreateWindowSurface"))
            return false;

        m_context = eglCreateContext(m_display, config, NULL, NULL);
        if (!checkEglError("eglCreateContext"))
            return false;

        eglMakeCurrent(m_display, m_surface, m_surface, m_context);
        if (!checkEglError("eglMakeCurrent"))
            return false;

        eglQuerySurface(m_display, m_surface, EGL_WIDTH, &m_width);
        eglQuerySurface(m_display, m_surface, EGL_HEIGHT, &m_height);

#ifdef DEBUG
        printf("********EGL info********\n");
        printf("EGL_VENDOR\t: %s\n", eglQueryString(m_display, EGL_VENDOR));
        printf("EGL_VERSION\t: %s\n", eglQueryString(m_display, EGL_VERSION));
        printf("EGL_EXTENSIONS\t: %s\n", eglQueryString(m_display, EGL_EXTENSIONS));
        printf("EGL_CLIENT_APIS\t: %s\n", eglQueryString(m_display, EGL_CLIENT_APIS));

        printf("m_width\t\t: %d\n", m_width);
        printf("m_height\t: %d\n", m_height);
#endif

        checkEglError("eglAll");
#endif
        return true;
    }

    bool World::initGL() {
        // Initialize viewport and projection.
        glViewport( 0, 0, m_width, m_height );

        //set the matrix mode
        gluPerspective();
#if 0
        //reset the matrix mode
        glMatrixMode(GL_MODELVIEW); //GL_PROJECTION,GL_MODELVIEW
        glLoadIdentity();

        // set some init status
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glShadeModel(GL_SMOOTH);//GL_SMOOTH,GL_FLAT
        //glEnable(GL_CULL_FACE);
        //glCullFace(GL_FRONT);

        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); //GL_FASTEST GL_NICEST

#ifdef DEBUG
        printf("********GL info********\n");
        printf("GL_VENDOR\t: %s\n", glGetString(GL_VENDOR));
        printf("GL_RENDERER\t: %s\n", glGetString(GL_RENDERER));
        printf("GL_VERSION\t: %s\n", glGetString(GL_VERSION));
        printf("GL_EXTENSIONS\t: %s\n", glGetString(GL_EXTENSIONS));
#endif

#endif
        return true;
    }

    void World::deinitEGL() {
#ifndef ANDROID_NDK
        eglMakeCurrent(m_display, NULL, NULL, NULL);
        eglDestroyContext(m_display, m_context);
        eglDestroySurface(m_display, m_surface);
        eglTerminate(m_display);
#endif
    }

#ifdef ANDROID
    bool World::init() {
#else
    bool World::init(EGLNativeWindowType hwnd) {
        m_hwnd = hwnd;
#endif // ANDROID
        if (!initEGL()) {
#ifdef DEBUG
            printf("initEGL() error!\n");
#endif // !DEBUG
            return false;
        }

        initGL();

        return true;
    }

    void World::setSize(int width, int height) {
        m_width = width;
        m_height = height;
    }

	void World::resize(int width, int height) {
		setSize(width, height);
		//after resize screen, call initGL() again
		initGL();
	}

    int World::getWidth() {
        return m_width;
    }

    int World::getHeight() {
        return m_height;
    }

    void World::setPerspective(GLfloat fovy, GLfloat znear, GLfloat zfar) {
        m_fovy = fovy;
        m_znear = znear;
        m_zfar = zfar;
    }

    void World::gluPerspective() {
    /*
       // Compute the window aspect ratio
       aspect = (GLfloat) esContext->width / (GLfloat) esContext->height;

       // Generate a perspective matrix with a 60 degree FOV
       esMatrixLoadIdentity( &perspective );
       esPerspective( &perspective, 60.0f, aspect, 1.0f, 20.0f );
       */


        // Start in projection mode.
        GLfloat xmin, xmax, ymin, ymax;
        GLfloat aspect = (GLfloat)m_width / (GLfloat)m_height;
        Matrix perspective;

        ymax = (float) (m_znear * tan(m_fovy * M_PI / 360.0));
        ymin = -ymax;
        xmin = ymin * aspect;
        xmax = ymax * aspect;

#ifdef DEBUG
        printf("glFrustumf()\t: %.3f, %.3f, %.3f, %.3f, %.3f, %.3f\n", xmin, xmax, ymin, ymax, m_znear, m_zfar);
#endif
        //glFrustumf(xmin, xmax, ymin, ymax, m_znear, m_zfar);
    }

    bool World::checkEglError(const char *name) {
#ifndef ANDROID_NDK
        unsigned err = eglGetError();

        if (err != EGL_SUCCESS) {
#ifdef DEBUG
            printf("%s() error: 0x%4x!\n", name, err);
#endif /* DEBUG */
            return false;
        } else {
            return true;
        }
#else
        return true;
#endif // !ANDROID_NDK
    }

    void World::prepareRender() {
        // Set the screen background color.
        glClearColor(m_bgred, m_bggreen, m_bgblue, m_bgalpha);
		glClearDepthf(1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#if 0
        glLoadIdentity();

        m_cameras[m_cameraIndex].gluLookAt();

        if (m_fog != NULL) {
            glEnable(GL_FOG);
            m_fog->initFog();
        } else
            glDisable(GL_FOG);

        if (m_light != NULL) {
            glEnable(GL_LIGHTING);
            glEnable(GL_NORMALIZE);
            glEnable(GL_COLOR_MATERIAL);
            m_light->initLight();
        } else {
            glDisable(GL_LIGHTING);
            glDisable(GL_NORMALIZE);
            glDisable(GL_COLOR_MATERIAL);
        }
#endif
    }

    void World::finishRender() {
#ifndef ANDROID_NDK
        eglSwapBuffers(m_display, m_surface);
#endif
    }

}
