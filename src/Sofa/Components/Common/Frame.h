#ifndef SOFA_COMPONENTS_COMMON_FRAME_H
#define SOFA_COMPONENTS_COMMON_FRAME_H

#include <Sofa/Components/Common/Vec.h>
#include <Sofa/Components/Common/Mat.h>
#include <Sofa/Components/Common/Quat.h>

namespace Sofa
{
namespace Components
{
namespace Common
{

/** The affine transformation of points and vectors from a coordinate system (the local frame) to another (the reference frame). The transformation is not necessarily rigid.

When defined using (origin, rotation, scale), the transfrom can be seen as three frame displacements starting from the reference frame: translation, then rotation along the new origin, then scale along the new axes. Conversely, the projection of a point from the local frame to the reference frame undergoes scaling, then rotation, then translation. Vectors do not undergo translation since they model directions or displacement.

The product F1F2 can be seen as a frame displacement F1 starting from the reference frame followed by a frame displacement F2 starting from the new frame. It is used in frame hierarchies to model the transfromation frome frame 2 wrt the reference frame.

\author Francois Faure, INRIA-UJF, 2006
*/
class Frame
{
public:
    typedef float Real;
    typedef Vec<3,Real> Vec3;
    typedef Quater<Real> Quat;
    typedef Mat<3,3,Real> Mat33;
private:
    Vec3 origin_;
    Mat33 basis_;
public:
    /** Define the local frame using origin and basis vectors */
    Frame (const Vec3 &origin, const Mat33& m );

    /** Define the local frame using three transforms */
    Frame (const Vec3 &origin, const Quat &orientation, const Vec3& scale=Vec3(1,1,1) );

    /** Define the local frame without rotation */
    Frame (const Vec3 &origin );

    /// Default constructor set the transformation to identity
    Frame ();

    /** Define the local frame using three transforms */
    void setTransform( const Vec3& origin, const Quat& orientation, const Vec3& scale );

    /// Reset the transformation to identity
    void clear();

    /// Origin of the local frame in the reference frame,
    Vec3& getOrigin ();

    /// Origin of the local frame in the reference frame,
    const Vec3& getOrigin () const;

    /// Set the origin of the local frame in the reference frame,
    void setOrigin( const Vec3& origin );

    /// The columns represent the base vectors of the local frame wrt the reference frame
    Mat33& getBasis ();

    /// The columns represent the base vectors of the local frame wrt the reference frame
    const Mat33& getBasis () const;

    /// The columns represent the base vectors of the local frame wrt the reference frame
    void setBasis( const Mat33& m );

    /// The identity transform
    static Frame identity();

    /// Apply a transformation defined in the child frame (mult. right)
    void multRight( const Frame& c );

    /// compute the product with another frame on the right
    Frame mult( const Frame& c ) const;

    /// Write the OpenGL transformation matrix
    void writeOpenGlMatrix( float m[16] ) const;

    /// compute the projection of a vector from the local frame to the reference frame
    template<class V>
    V projectVector( const V& v ) const
    {
        return V(
                basis_[0][0]*v[0]+basis_[0][1]*v[1]+basis_[0][2]*v[2],
                basis_[1][0]*v[0]+basis_[1][1]*v[1]+basis_[1][2]*v[2],
                basis_[2][0]*v[0]+basis_[2][1]*v[1]+basis_[2][2]*v[2]
                );
    }

    /// compute the projection of a point from the local frame to the reference frame
    template<class V>
    V projectPoint( const V& v ) const
    {
        return projectVector(v)+origin_;
    }

    /// Transformation from the reference frame to the local frame
    Frame inversed() const;

    friend std::ostream& operator << (std::ostream& out, const Frame& c );
};
}//Common
}//Components
}//Sofa

#endif
