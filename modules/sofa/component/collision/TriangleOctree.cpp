/*******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 beta 1       *
*                (c) 2006-2007 MGH, INRIA, USTL, UJF, CNRS                     *
*                                                                              *
* This library is free software; you can redistribute it and/or modify it      *
* under the terms of the GNU Lesser General Public License as published by the *
* Free Software Foundation; either version 2.1 of the License, or (at your     *
* option) any later version.                                                   *
*                                                                              *
* This library is distributed in the hope that it will be useful, but WITHOUT  *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or        *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License  *
* for more details.                                                            *
*                                                                              *
* You should have received a copy of the GNU Lesser General Public License     *
* along with this library; if not, write to the Free Software Foundation,      *
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.           *
*                                                                              *
* Contact information: contact@sofa-framework.org                              *
*                                                                              *
* Authors: J. Allard, P-J. Bensoussan, S. Cotin, C. Duriez, H. Delingette,     *
* F. Faure, S. Fonteneau, L. Heigeas, C. Mendoza, M. Nesme, P. Neumann,        *
* and F. Poyer                                                                 *
*******************************************************************************/
#include <sofa/component/collision/TriangleModel.h>
#include <sofa/component/collision/TriangleOctree.h>
#include <sofa/component/collision/CubeModel.h>
#include <sofa/component/collision/proximity.h>

#include <sofa/component/collision/Triangle.h>
#include <sofa/core/CollisionElement.h>
#include <sofa/core/ObjectFactory.h>
#include <vector>
#include <sofa/helper/system/thread/CTime.h>

#include <cmath>
#include <GL/gl.h>
#include <GL/glut.h>


namespace sofa
{

namespace component
{

namespace collision
{

using sofa::helper::system::thread::CTime;
using sofa::helper::system::thread::ctime_t;


void TriangleOctree::draw ()
{

    Vector3 center;
    if (internal || objects.size ())
    {
        center =
            (Vector3 (x, y, z) + Vector3 (size / 2, size / 2, size / 2));
        glPushMatrix ();
        glTranslatef (center[0], center[1], center[2]);
        glutWireCube (size);
        glPopMatrix ();
    }
    for (int i = 0; i < 8; i++)
    {
        if (childVec[i])
            childVec[i]->draw ();
    }
}

void TriangleOctree::insert (double _x, double _y, double _z,
        double inc, int t)
{
    if (inc >= size)
    {
        objects.push_back (t);
    }
    else
    {

        double size2 = size / 2;
        int dx = (_x >= (x + size2)) ? 1 : 0;
        int dy = (_y >= (y + size2)) ? 1 : 0;
        int dz = (_z >= (z + size2)) ? 1 : 0;

        int i = dx * 4 + dy * 2 + dz;
        if (!childVec[i])
        {
            is_leaf = false;
            childVec[i] =
                new TriangleOctree (tm, x + dx * size2, y + dy * size2,
                        z + dz * size2, size2);
        }

        childVec[i]->insert (_x, _y, _z, inc, t);
    }
}

inline
unsigned int choose_next (double x, double y, double z,
        unsigned int a, unsigned int b,
        unsigned int c)
{
    if (x < y)
    {
        if (x < z)
            return a;
        else
            return c;
    }
    else
    {
        if (y < z)
            return b;
        else
            return c;
    }
}

int TriangleOctree::nearestTriangle (int minIndex,
        const Vector3 & origin,
        const Vector3 & direction)
{
    static DistancePointTri proximitySolver;
    static RayTriangleIntersection intersectionSolver;
    Vector3 P;
    Triangle t1 (tm, minIndex);
    double minDist;
    float t, u, v;
    double distTmp;
    if (intersectionSolver.
        NewComputation (&t1, origin, direction, t, u, v))
    {


        minDist = t;
    }
    else
    {
        minDist = 10e8;
        minIndex = -1;
    }
    for (int i = 0; i < objects.size (); i++)
    {

        Triangle t2 (tm, objects[i]);
        if (!intersectionSolver.
            NewComputation (&t2, origin, direction, t, u, v))
            continue;

        if (t <= minDist)
        {
            minDist = distTmp;
            minIndex = objects[i];
        }


    }
    return minIndex;
}

int TriangleOctree::trace (const Vector3 & origin,
        const Vector3 & direction, double tx0,
        double ty0, double tz0, double tx1,
        double ty1, double tz1, unsigned int a,
        unsigned int b)
{


    if (tx1 < 0.0 || ty1 < 0.0 || tz1 < 0.0)
        return -1;


    if (is_leaf)
    {
        if (objects.size ())
        {

            return nearestTriangle (objects[0], origin, direction);

        }
        else
        {
            return -1;
        }
    }

    const double &x0 = x;
    const double &y0 = y;
    const double &z0 = z;
    const double &x1 = x + size;
    const double &y1 = y + size;
    const double &z1 = z + size;
    const double INF = 1e9;

    double txm, tym, tzm;

    switch (b)
    {
    case 0:
        txm = 0.5 * (tx0 + tx1);
        tym = 0.5 * (ty0 + ty1);
        tzm = 0.5 * (tz0 + tz1);
        break;
    case 1:
        txm = 0.5 * (tx0 + tx1);
        tym = 0.5 * (ty0 + ty1);
        tzm = origin[2] < 0.5 * (z0 + z1) ? +INF : -INF;
        break;
    case 2:
        txm = 0.5 * (tx0 + tx1);
        tym = origin[1] < 0.5 * (y0 + y1) ? +INF : -INF;
        tzm = 0.5 * (tz0 + tz1);
        break;
    case 3:
        txm = 0.5 * (tx0 + tx1);
        tym = origin[1] < 0.5 * (y0 + y1) ? +INF : -INF;
        tzm = origin[2] < 0.5 * (z0 + z1) ? +INF : -INF;
        break;
    case 4:
        txm = origin[0] < 0.5 * (x0 + x1) ? +INF : -INF;
        tym = 0.5 * (ty0 + ty1);
        tzm = 0.5 * (tz0 + tz1);
        break;
    case 5:
        txm = origin[0] < 0.5 * (x0 + x1) ? +INF : -INF;
        tym = 0.5 * (ty0 + ty1);
        tzm = origin[2] < 0.5 * (z0 + z1) ? +INF : -INF;
        break;
    case 6:
        txm = origin[0] < 0.5 * (x0 + x1) ? +INF : -INF;
        tym = origin[1] < 0.5 * (y0 + y1) ? +INF : -INF;
        tzm = 0.5 * (tz0 + tz1);
        break;
    case 7:
        txm = origin[0] < 0.5 * (x0 + x1) ? +INF : -INF;
        tym = origin[1] < 0.5 * (y0 + y1) ? +INF : -INF;
        tzm = origin[2] < 0.5 * (z0 + z1) ? +INF : -INF;
        break;
    default:
        assert (!"Internal failure.");
    }
    unsigned int current_octant = 0;

    if (tx0 > ty0)
    {
        if (tx0 > tz0)
        {
            // max(tx0, ty0, tz0) is tx0. Entry plane is YZ.
            if (tym < tx0)
                current_octant |= 2;
            if (tzm < tx0)
                current_octant |= 1;
        }
        else
        {
            // max(tx0, ty0, tz0) is tz0. Entry plane is XY.
            if (txm < tz0)
                current_octant |= 4;
            if (tym < tz0)
                current_octant |= 2;
        }
    }
    else
    {
        if (ty0 > tz0)
        {
            // max(tx0, ty0, tz0) is ty0. Entry plane is XZ.
            if (txm < ty0)
                current_octant |= 4;
            if (tzm < ty0)
                current_octant |= 1;
        }
        else
        {
            // max(tx0, ty0, tz0) is tz0. Entry plane is XY.
            if (txm < tz0)
                current_octant |= 4;
            if (tym < tz0)
                current_octant |= 2;
        }
    }

    // This special state indicates algorithm termination.
    const unsigned int END = 8;

    while (true)
    {
        int idxMin;
        switch (current_octant)
        {
        case 0:
            if (childVec[a])
            {
                idxMin =
                    childVec[a]->trace (origin, direction, tx0, ty0, tz0,
                            txm, tym, tzm, a, b);
                if (idxMin != -1)
                    return nearestTriangle (idxMin, origin, direction);
            }
            current_octant = choose_next (txm, tym, tzm, 4, 2, 1);
            break;




        case 1:
            if (childVec[1 ^ a])
            {
                idxMin =
                    childVec[1 ^ a]->trace (origin, direction, tx0, ty0,
                            tzm, txm, tym, tz1, a, b);
                if (idxMin != -1)
                    return nearestTriangle (idxMin, origin, direction);
            }
            current_octant = choose_next (txm, tym, tz1, 5, 3, END);
            break;



        case 2:
            if (childVec[2 ^ a])
            {
                idxMin =
                    childVec[2 ^ a]->trace (origin, direction, tx0, tym,
                            tz0, txm, ty1, tzm, a, b);
                if (idxMin != -1)
                    return nearestTriangle (idxMin, origin, direction);
            }
            current_octant = choose_next (txm, ty1, tzm, 6, END, 3);
            break;
        case 3:
            if (childVec[3 ^ a])
            {
                idxMin =
                    childVec[3 ^ a]->trace (origin, direction, tx0, tym,
                            tzm, txm, ty1, tz1, a, b);
                if (idxMin != -1)
                    return nearestTriangle (idxMin, origin, direction);
            }
            current_octant = choose_next (txm, ty1, tz1, 7, END, END);
            break;
        case 4:
            if (childVec[4 ^ a])
            {
                idxMin =
                    childVec[4 ^ a]->trace (origin, direction, txm, ty0,
                            tz0, tx1, tym, tzm, a, b);
                if (idxMin != -1)
                    return nearestTriangle (idxMin, origin, direction);
            }
            current_octant = choose_next (tx1, tym, tzm, END, 6, 5);
            break;
        case 5:
            if (childVec[5 ^ a])
            {
                idxMin =
                    childVec[5 ^ a]->trace (origin, direction, txm, ty0,
                            tzm, tx1, tym, tz1, a, b);
                if (idxMin != -1)
                    return nearestTriangle (idxMin, origin, direction);
            }
            current_octant = choose_next (tx1, tym, tz1, END, 7, END);
            break;
        case 6:
            if (childVec[6 ^ a])
            {
                idxMin =
                    childVec[6 ^ a]->trace (origin, direction, txm, tym,
                            tz0, tx1, ty1, tzm, a, b);
                if (idxMin != -1)
                    return nearestTriangle (idxMin, origin, direction);
            }
            current_octant = choose_next (tx1, ty1, tzm, END, END, 7);
            break;
        case 7:
            if (childVec[7 ^ a])
            {
                idxMin =
                    childVec[7 ^ a]->trace (origin, direction, txm, tym,
                            tzm, tx1, ty1, tz1, a, b);
                if (idxMin != -1)
                    return nearestTriangle (idxMin, origin, direction);
            }
        case END:
            return -1;
            //      return false;
        }
    }


}
int TriangleOctree::trace (Vector3 origin, Vector3 direction)
{
    unsigned int a = 0;
    unsigned int b = 0;

    if (direction[0] == 0.0)
    {
        //direction.m_x = EPSILON;
        b |= 4;
    }
    else if (direction[0] < 0.0)
    {
        origin[0] = -origin[0];
        direction[0] = -direction[0];
        a |= 4;
    }

    if (direction[1] == 0.0)
    {
        b |= 2;
    }
    else if (direction[1] < 0.0)
    {
        origin[1] = -origin[1];
        direction[1] = -direction[1];
        a |= 2;
    }

    if (direction[2] == 0.0)
    {
        b |= 1;
    }
    else if (direction[2] < 0.0)
    {
        origin[2] = -origin[2];
        direction[2] = -direction[2];
        a |= 1;
    }
    double tx0 = (-CUBE_SIZE - origin[0]) / direction[0];
    double tx1 = (CUBE_SIZE - origin[0]) / direction[0];
    double ty0 = (-CUBE_SIZE - origin[1]) / direction[1];
    double ty1 = (CUBE_SIZE - origin[1]) / direction[1];
    double tz0 = (-CUBE_SIZE - origin[2]) / direction[2];
    double tz1 = (CUBE_SIZE - origin[2]) / direction[2];
    if (bb_max3 (tx0, ty0, tz0) < bb_min3 (tx1, ty1, tz1))
        trace (origin, direction, tx0, ty0, tz0, tx1, ty1, tz1, a, b);


}

int TriangleOctree::findInputTriangle (int inputTriangle,
        const Vector3 & origin,
        const Vector3 & direction)
{
    static DistancePointTri proximitySolver;
    static RayTriangleIntersection intersectionSolver;
    Vector3 P;

    double minDist;
    float t, u, v;
    double distTmp;

    for (int i = 0; i < objects.size (); i++)
    {

        Triangle t2 (tm, objects[i]);
        if (!intersectionSolver.
            NewComputation (&t2, origin, direction, t, u, v))
            continue;


        if (objects[i] != inputTriangle)
        {
            inputTriangle = (inputTriangle != -1) ? -1 : objects[i];

        }


    }
    return inputTriangle;
}
int TriangleOctree::traceVolume (const Vector3 & origin,
        const Vector3 & direction, double tx0,
        double ty0, double tz0, double tx1,
        double ty1, double tz1, unsigned int a,
        unsigned int b, int inputTriangle,
        vector < vertexData * >&vertexVec)
{


    if (tx1 < 0.0 || ty1 < 0.0 || tz1 < 0.0)
        return -1;


    if (objects.size ())
    {
        int tmpInputTriangle =
            findInputTriangle (inputTriangle, origin, direction);

        if (inputTriangle != -1 && tmpInputTriangle == -1)
        {

            int dx;
            int dy;
            int dz;

            for (int i = 0; i < vertexVec.size (); i++)
            {
                int id = vertexVec[i]->childId;

                TriangleOctree *tp = vertexVec[i]->parent;
                double size2 = tp->size / 2;
                if (tp->childVec[id])
                {
                    dz = id & 1;
                    id >>= 1;
                    dy = id & 1;
                    id >>= 1;
                    dx = id & 1;


                    tp->childVec[vertexVec[i]->childId] =
                        new TriangleOctree (tp->tm, tp->x + dx * size2,
                                tp->y + dy * size2,
                                tp->z + dz * size2, size2);
                    tp->childVec[vertexVec[i]->childId]->internal = true;
                }
            }

        }
        inputTriangle = tmpInputTriangle;
    }


    const double &x0 = x;
    const double &y0 = y;
    const double &z0 = z;
    const double &x1 = x + size;
    const double &y1 = y + size;
    const double &z1 = z + size;
    const double INF = 1e9;

    double txm, tym, tzm;

    switch (b)
    {
    case 0:
        txm = 0.5 * (tx0 + tx1);
        tym = 0.5 * (ty0 + ty1);
        tzm = 0.5 * (tz0 + tz1);
        break;
    case 1:
        txm = 0.5 * (tx0 + tx1);
        tym = 0.5 * (ty0 + ty1);
        tzm = origin[2] < 0.5 * (z0 + z1) ? +INF : -INF;
        break;
    case 2:
        txm = 0.5 * (tx0 + tx1);
        tym = origin[1] < 0.5 * (y0 + y1) ? +INF : -INF;
        tzm = 0.5 * (tz0 + tz1);
        break;
    case 3:
        txm = 0.5 * (tx0 + tx1);
        tym = origin[1] < 0.5 * (y0 + y1) ? +INF : -INF;
        tzm = origin[2] < 0.5 * (z0 + z1) ? +INF : -INF;
        break;
    case 4:
        txm = origin[0] < 0.5 * (x0 + x1) ? +INF : -INF;
        tym = 0.5 * (ty0 + ty1);
        tzm = 0.5 * (tz0 + tz1);
        break;
    case 5:
        txm = origin[0] < 0.5 * (x0 + x1) ? +INF : -INF;
        tym = 0.5 * (ty0 + ty1);
        tzm = origin[2] < 0.5 * (z0 + z1) ? +INF : -INF;
        break;
    case 6:
        txm = origin[0] < 0.5 * (x0 + x1) ? +INF : -INF;
        tym = origin[1] < 0.5 * (y0 + y1) ? +INF : -INF;
        tzm = 0.5 * (tz0 + tz1);
        break;
    case 7:
        txm = origin[0] < 0.5 * (x0 + x1) ? +INF : -INF;
        tym = origin[1] < 0.5 * (y0 + y1) ? +INF : -INF;
        tzm = origin[2] < 0.5 * (z0 + z1) ? +INF : -INF;
        break;
    default:
        assert (!"Internal failure.");
    }
    unsigned int current_octant = 0;

    if (tx0 > ty0)
    {
        if (tx0 > tz0)
        {
            // max(tx0, ty0, tz0) is tx0. Entry plane is YZ.
            if (tym < tx0)
                current_octant |= 2;
            if (tzm < tx0)
                current_octant |= 1;
        }
        else
        {
            // max(tx0, ty0, tz0) is tz0. Entry plane is XY.
            if (txm < tz0)
                current_octant |= 4;
            if (tym < tz0)
                current_octant |= 2;
        }
    }
    else
    {
        if (ty0 > tz0)
        {
            // max(tx0, ty0, tz0) is ty0. Entry plane is XZ.
            if (txm < ty0)
                current_octant |= 4;
            if (tzm < ty0)
                current_octant |= 1;
        }
        else
        {
            // max(tx0, ty0, tz0) is tz0. Entry plane is XY.
            if (txm < tz0)
                current_octant |= 4;
            if (tym < tz0)
                current_octant |= 2;
        }
    }

    // This special state indicates algorithm termination.
    const unsigned int END = 8;

    while (true)
    {
        int idxMin;
        switch (current_octant)
        {
        case 0:
            if (childVec[a])
            {

                inputTriangle =
                    childVec[a]->traceVolume (origin, direction, tx0, ty0,
                            tz0, txm, tym, tzm, a, b,
                            inputTriangle, vertexVec);

            }
            else
            {
                if (inputTriangle != -1 && !objects.size ())
                {
                    vertexVec.push_back (new vertexData (this, a));
                }
            }

            current_octant = choose_next (txm, tym, tzm, 4, 2, 1);
            break;




        case 1:
            if (childVec[1 ^ a])
            {
                inputTriangle =
                    childVec[1 ^ a]->traceVolume (origin, direction, tx0,
                            ty0, tzm, txm, tym, tz1,
                            a, b, inputTriangle,
                            vertexVec);
            }
            else
            {
                if (inputTriangle != -1 && !objects.size ())
                {
                    vertexVec.push_back (new vertexData (this, 1 ^ a));
                }
            }
            current_octant = choose_next (txm, tym, tz1, 5, 3, END);
            break;



        case 2:
            if (childVec[2 ^ a])
            {
                inputTriangle =
                    childVec[2 ^ a]->traceVolume (origin, direction, tx0,
                            tym, tz0, txm, ty1, tzm,
                            a, b, inputTriangle,
                            vertexVec);
            }
            else
            {
                if (inputTriangle != -1 && !objects.size ())
                {
                    vertexVec.push_back (new vertexData (this, 2 ^ a));
                }
            }
            current_octant = choose_next (txm, ty1, tzm, 6, END, 3);
            break;
        case 3:
            if (childVec[3 ^ a])
            {
                inputTriangle =
                    childVec[3 ^ a]->traceVolume (origin, direction, tx0,
                            tym, tzm, txm, ty1, tz1,
                            a, b, inputTriangle,
                            vertexVec);
            }
            else
            {
                if (inputTriangle != -1 && !objects.size ())
                {
                    vertexVec.push_back (new vertexData (this, 3 ^ a));
                }
            }
            current_octant = choose_next (txm, ty1, tz1, 7, END, END);
            break;
        case 4:
            if (childVec[4 ^ a])
            {
                inputTriangle =
                    childVec[4 ^ a]->traceVolume (origin, direction, txm,
                            ty0, tz0, tx1, tym, tzm,
                            a, b, inputTriangle,
                            vertexVec);
            }
            else
            {
                if (inputTriangle != -1 && !objects.size ())
                {
                    vertexVec.push_back (new vertexData (this, 4 ^ a));
                }
            }
            current_octant = choose_next (tx1, tym, tzm, END, 6, 5);
            break;
        case 5:
            if (childVec[5 ^ a])
            {
                inputTriangle =
                    childVec[5 ^ a]->traceVolume (origin, direction, txm,
                            ty0, tzm, tx1, tym, tz1,
                            a, b, inputTriangle,
                            vertexVec);
            }
            else
            {
                if (inputTriangle != -1 && !objects.size ())
                {
                    vertexVec.push_back (new vertexData (this, 5 ^ a));
                }
            }
            current_octant = choose_next (tx1, tym, tz1, END, 7, END);
            break;
        case 6:
            if (childVec[6 ^ a])
            {
                inputTriangle =
                    childVec[6 ^ a]->traceVolume (origin, direction, txm,
                            tym, tz0, tx1, ty1, tzm,
                            a, b, inputTriangle,
                            vertexVec);
            }
            else
            {
                if (inputTriangle != -1 && !objects.size ())
                {
                    vertexVec.push_back (new vertexData (this, 6 ^ a));
                }
            }
            current_octant = choose_next (tx1, ty1, tzm, END, END, 7);
            break;
        case 7:
            if (childVec[7 ^ a])
            {
                inputTriangle =
                    childVec[7 ^ a]->traceVolume (origin, direction, txm,
                            tym, tzm, tx1, ty1, tz1,
                            a, b, inputTriangle,
                            vertexVec);
            }
            else
            {
                if (inputTriangle != -1 && !objects.size ())
                {
                    vertexVec.push_back (new vertexData (this, 7 ^ a));
                }
            }
        case END:
            return inputTriangle;
            //      return false;
        }
    }

    return inputTriangle;
}
int TriangleOctree::traceVolume (Vector3 origin, Vector3 direction)
{
    unsigned int a = 0;
    unsigned int b = 0;

    if (direction[0] == 0.0)
    {
        //direction.m_x = EPSILON;
        b |= 4;
    }
    else if (direction[0] < 0.0)
    {
        origin[0] = -origin[0];
        direction[0] = -direction[0];
        a |= 4;
    }

    if (direction[1] == 0.0)
    {
        b |= 2;
    }
    else if (direction[1] < 0.0)
    {
        origin[1] = -origin[1];
        direction[1] = -direction[1];
        a |= 2;
    }

    if (direction[2] == 0.0)
    {
        b |= 1;
    }
    else if (direction[2] < 0.0)
    {
        origin[2] = -origin[2];
        direction[2] = -direction[2];
        a |= 1;
    }
    double tx0 = (-CUBE_SIZE - origin[0]) / direction[0];
    double tx1 = (CUBE_SIZE - origin[0]) / direction[0];
    double ty0 = (-CUBE_SIZE - origin[1]) / direction[1];
    double ty1 = (CUBE_SIZE - origin[1]) / direction[1];
    double tz0 = (-CUBE_SIZE - origin[2]) / direction[2];
    double tz1 = (CUBE_SIZE - origin[2]) / direction[2];
    vector < vertexData * >vd;
    if (bb_max3 (tx0, ty0, tz0) < bb_min3 (tx1, ty1, tz1))
        traceVolume (origin, direction, tx0, ty0, tz0, tx1, ty1, tz1, a, b,
                -1, vd);


}
int TriangleOctree::traceVolume (int n)
{
    double inc = 2 * (double) CUBE_SIZE / n;
    for (x = -CUBE_SIZE; x < CUBE_SIZE; x += inc)
        for (y = -CUBE_SIZE; y < CUBE_SIZE; y += inc)
        {

            traceVolume (Vector3 (-CUBE_SIZE, x, y), Vector3 (1, 0, 0));
        }
}


}				// namespace collision

}				// namespace component

}				// namespace sofa
