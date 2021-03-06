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

#include "camera.h"

namespace F3D {
    /**
     * Camera class for all games using F3D.
     */

    Camera::Camera() :
            m_centerx(0.0f),
            m_centery(0.0f),
            m_centerz(0.0f),
            m_upx(0.0f),
            m_upy(1.0f),
            m_upz(0.0f) {
#ifdef DEBUG
        printf("Camera constructor...\n");
#endif
    }

    Camera::~Camera() {
#ifdef DEBUG
        printf("Camera destructor...\n");
#endif
    }

    void Camera::setEye(GLfloat eyex, GLfloat eyey, GLfloat eyez) {
        m_eyex = eyex;
        m_eyey = eyey;
        m_eyez = eyez;
    }

    void Camera::setCenter(GLfloat centerx, GLfloat centery, GLfloat centerz) {
        m_centerx = centerx;
        m_centery = centery;
        m_centerz = centerz;
    }

    void Camera::setUp(GLfloat upx, GLfloat upy, GLfloat upz) {
        m_upx = upx;
        m_upy = upy;
        m_upz = upz;
    }

    void Camera::identf(GLfloat m[]) {
        memset(m, 0, sizeof m[0]*16);
        m[0] = m[5] = m[10] = m[15] = 1.0f;
    }

    void Camera::normalizef(float v[3]) {
        float r;

        r = (float)sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
        if (r == 0.0)
            return;
        r = 1.f/r;

        v[0] *= r;
        v[1] *= r;
        v[2] *= r;
    }

    void Camera::crossf(float v0[3], float v1[3], float r[3]) {
        r[0] = v0[1]*v1[2] - v0[2]*v1[1];
        r[1] = v0[2]*v1[0] - v0[0]*v1[2];
        r[2] = v0[0]*v1[1] - v0[1]*v1[0];
    }

    void Camera::gluLookAt() {
        GLfloat forward[3], side[3], up[3];
        GLfloat m[4][4];

        forward[0] = m_centerx - m_eyex;
        forward[1] = m_centery - m_eyey;
        forward[2] = m_centerz - m_eyez;

        normalizef(forward);

        up[0] = m_upx;
        up[1] = m_upy;
        up[2] = m_upz;

        crossf(forward, up, side);

        normalizef(side);
        crossf(side, forward, up);

        identf(&m[0][0]);
        m[0][0] = side[0];
        m[1][0] = side[1];
        m[2][0] = side[2];

        m[0][1] = up[0];
        m[1][1] = up[1];
        m[2][1] = up[2];

        m[0][2] = -forward[0];
        m[1][2] = -forward[1];
        m[2][2] = -forward[2];

        glMultMatrixf(&m[0][0]);
        glTranslatef(-m_eyex, -m_eyey, -m_eyez);
    }

}
