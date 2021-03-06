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

#include "terrain.h"

namespace F3D {
    /**
     * Terrain class for all games using F3D.
     */

    Terrain::Terrain(const char *filename, int size, int scale, GLboolean is_absPath) {
        FILE *fp;
        GLubyte *hmap;
        int vt_idx = 0, uv_idx = 0, hm_idx = 0, in_idx = 0, x, y;
        // used for the texture calculation
        float texdelta = 2.0f/size;
        float u_tile = 1.0f;
        float v_tile = 1.0f;

        // allocate some memory for the heightmap
        hmap = (GLubyte *)malloc(size * size * sizeof(GLubyte));

#ifdef DEBUG
        printf("Terrain constructor...\n");
#endif
        setMeshCount(1);

        // open the heightmap file
		fp = fopen(Utils::getFileName(filename, is_absPath), "rb");

        // read in the heightmap into hmap
        if (fp != NULL)
            fread(hmap, sizeof(GLubyte), size*size, fp);
        else {
#ifdef DEBUG
        printf("Open terrain file error!\n");
#endif
            return;
        }

        // we dont need the file anymore, so close it
        fclose(fp);
#ifdef DEBUG
        printf("Start generate terrain...\n");
#endif
        // calculate the 3D heightfield
        // the number of vertices is the size of the heightmap squared
        float *uvs = (float *) malloc(size * size * 2 * sizeof(float));
        float *vertices = (float *) malloc(size * size * 3 * sizeof(float));

        GLshort *indices = (GLshort *) malloc((size-1)*(size-1) * 6 * sizeof(GLshort));
        setTriangleNums((size - 1) * (size - 1) * 2);

        // loop through every pixel in the heightmap
        for (y = 0; y < size; y++) {
            for (x = 0; x < size; x++) {
                // Get the current vertex we want to calculate
                hm_idx = y * size + x;

                // x, z coordinates are given by this pixel's x, y coords multiplied
                // by a scaling factor (which defines how far away from each other
                // each vertex is on the 3D heightfield).
                vertices[vt_idx++] = (float)x * scale;
                // calculate the height (y coordinate) from the value in the heightmap
                vertices[vt_idx++] = (float)(hmap[hm_idx] * (scale/5));
                vertices[vt_idx++] = (float)y * scale;

                // calculate the texture coordinates
                uvs[uv_idx++] = u_tile * ((float)x * texdelta * 0.5f);
                uvs[uv_idx++] = v_tile * (1.0f - (float)y * texdelta * 0.5f);
            }
        }

        // Calculate the indices
        for (y = 0; y < size - 1; y++) {
            for (x = 0; x < size - 1; x++) {
                hm_idx = y*(size)+x;

                indices[in_idx++] = hm_idx;
                indices[in_idx++] = hm_idx+1;
                indices[in_idx++] = hm_idx+(size);

                indices[in_idx++] = hm_idx+(size)+1;
                indices[in_idx++] = hm_idx+(size);
                indices[in_idx++] = hm_idx+1;
            }
        }

        setVertices(vertices, size * size * 3 * sizeof(float));
        setUvs(uvs, size * size * 2 * sizeof(float));
        setIndices(indices, (size-1)*(size-1) * 6 * sizeof(GLshort));

        FREEANDNULL(vertices);
        FREEANDNULL(uvs);
        FREEANDNULL(indices);
        // we dont need the heightmap anymore, get rid of it
        FREEANDNULL(hmap);
#ifdef DEBUG
        printf("Generate terrain OK!\n");
#endif
    }

    Terrain::~Terrain() {
#ifdef DEBUG
        printf("Terrain destructor...\n");
#endif
    }

    void Terrain::prepareFrame() {
#ifdef DEBUG
        static bool isFirst = true;
        if (isFirst) {
            printf("Call Terrain::prepareFrame()...\n");
            isFirst = false;
        }
#endif
    }

}
