#ifndef N_CHARJOINT_H
#define N_CHARJOINT_H
//------------------------------------------------------------------------------
/**
    @class nCharJoint
    @ingroup Character

    @brief A joint in a character skeleton.

     - 06-Feb-03   floh    fixed for Nebula2

    (C) 2002 RadonLabs GmbH
*/
#include "core/types.h"
#include "math/point.h"
#include "math/matrix44.h"
#include "math/quaternion.h"
#include "util/string.h"

namespace Nebula2
{
//------------------------------------------------------------------------------
class nCharJoint
{
public:
    /// constructor
    nCharJoint();
    /// destructor
    ~nCharJoint();
    /// set parent joint index
    void SetParentJointIndex(int index);
    /// get parent joint index
    int GetParentJointIndex() const;
    /// set parent joint pointer
    void SetParentJoint(nCharJoint* p);
    /// get the parent joint
    nCharJoint* GetParentJoint() const;
    /// set the pose
    void SetPose(const Math::vector& t, const Math::quaternion& q, const Math::point& s);
    /// get pose translate
    const Math::vector& GetPoseTranslate() const;
    /// get pose rotate
    const Math::quaternion& GetPoseRotate() const;
    /// get pose scale
    const Math::point& GetPoseScale() const;
    /// set translation
    void SetTranslate(const Math::vector& t);
    /// get translation
    const Math::vector& GetTranslate() const;
    /// set rotation
    void SetRotate(const Math::quaternion& q);
    /// get rotation
    const Math::quaternion& GetRotate() const;
    /// set scale
    void SetScale(const Math::point& s);
    /// get scale
    const Math::point& GetScale() const;
    /// set optional variation scale
    void SetVariationScale(const Math::point& s);
    /// get variation scale
    const Math::point& GetVariationScale() const;
    /// set name
    void SetName(const Util::String& name);
    /// get name
    const Util::String& GetName() const; 
    /// evaluate joint
    void Evaluate();
    /// directly set the local matrix
    void SetLocalMatrix(const Math::matrix44& m);
    /// get current evaluated matrix in local space, valid after Evaluate()!
    const Math::matrix44& GetLocalMatrix() const;
    /// get the bind pose matrix in model space
    const Math::matrix44& GetPoseMatrix() const;
    /// get the inverse pose matrix in model space
    const Math::matrix44& GetInvPoseMatrix() const;
    /// set model space matrix directly, this will disable multiplication by parent joint
    void SetMatrix(const Math::matrix44& m);
    /// get current evaluated matrix in model space, valid after Evaluate()!
    const Math::matrix44& GetMatrix() const;
    /// get the skinning matrix with translation
    const Math::matrix44& GetSkinMatrix44() const;
    /// clear the uptodate flag
    void ClearUptodateFlag();
    /// return true if the joint has been evaluated 
    bool IsUptodate() const;

private:
    Math::vector poseTranslate;
    Math::quaternion poseRotate;
    Math::point poseScale;

    Math::vector translate;
    Math::quaternion rotate;
    Math::point scale;
    Math::point variationScale;

    Math::matrix44 poseMatrix;
    Math::matrix44 invPoseMatrix;

    Math::matrix44 localUnscaledMatrix;
    Math::matrix44 localScaledMatrix;
    Math::matrix44 worldUnscaledMatrix;
    Math::matrix44 worldScaledMatrix;
    Math::matrix44 skinMatrix44;
    int parentJointIndex;
    nCharJoint* parentJoint;
    bool matrixDirty;
    bool lockMatrix;
    bool isUptodate;
    
    Util::String name;
};

//------------------------------------------------------------------------------
/**
*/
inline
nCharJoint::nCharJoint() :
    parentJoint(0),
    parentJointIndex(-1),
    poseScale(1.0f, 1.0f, 1.0f),
    scale(1.0f, 1.0f, 1.0f),
    variationScale(1.0f, 1.0f, 1.0f),
    matrixDirty(true),
    lockMatrix(false),
    isUptodate(false),
    rotate(0,0,0,1)    
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
nCharJoint::~nCharJoint()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nCharJoint::SetParentJoint(nCharJoint* p)
{
    this->parentJoint = p;
}

//------------------------------------------------------------------------------
/**
*/
inline
nCharJoint*
nCharJoint::GetParentJoint() const
{
    return this->parentJoint;
}

//------------------------------------------------------------------------------
/**
    The parent joint index can be -1 if this joint is the root joint.
*/
inline
void
nCharJoint::SetParentJointIndex(int index)
{
    this->parentJointIndex = index;
}

//------------------------------------------------------------------------------
/**
*/
inline
int
nCharJoint::GetParentJointIndex() const
{
    return this->parentJointIndex;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nCharJoint::SetPose(const Math::vector& t, const Math::quaternion& q, const Math::point& s)
{
    this->poseTranslate = t;
    this->translate = this->poseTranslate;
    this->poseRotate = Math::quaternion::normalize(q);
    this->rotate = this->poseRotate;
    this->poseScale = s;
    this->scale = this->poseScale;

    this->poseMatrix = Math::matrix44::identity();
    this->poseMatrix.scale(this->poseScale);
    this->poseMatrix = Math::matrix44::multiply(this->poseMatrix, Math::matrix44::rotationquaternion(this->poseRotate));
    this->poseMatrix.translate(this->poseTranslate);

    // set the initial matrix so that it undoes the pose matrix
    this->localScaledMatrix = poseMatrix;
    this->worldScaledMatrix = poseMatrix;

    // global pose matrix and compute global inverse pose matrix
    if (this->parentJoint)
    {
        this->poseMatrix = Math::matrix44::multiply(this->poseMatrix, this->parentJoint->poseMatrix);
    }
    this->invPoseMatrix = Math::matrix44::inverse(this->poseMatrix);   
    this->skinMatrix44 = Math::matrix44::identity();
    this->localUnscaledMatrix = Math::matrix44::identity();
    this->worldUnscaledMatrix = Math::matrix44::identity();
}

//------------------------------------------------------------------------------
/**
*/
inline
const Math::vector&
nCharJoint::GetPoseTranslate() const
{
    return this->poseTranslate;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Math::quaternion& 
nCharJoint::GetPoseRotate() const
{
    return this->poseRotate;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Math::point&
nCharJoint::GetPoseScale() const
{
    return this->poseScale;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nCharJoint::SetTranslate(const Math::vector& t)
{
    this->translate = t;
    this->matrixDirty = true;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Math::vector&
nCharJoint::GetTranslate() const
{
    return this->translate;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nCharJoint::SetRotate(const Math::quaternion& q)
{
    this->rotate = q;
    this->matrixDirty = true;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Math::quaternion&
nCharJoint::GetRotate() const
{
    return this->rotate;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nCharJoint::SetScale(const Math::point& s)
{
    this->scale = s;
    this->matrixDirty = true;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Math::point&
nCharJoint::GetScale() const
{
    return this->scale;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nCharJoint::SetVariationScale(const Math::point& s)
{
    this->variationScale = s;
    this->matrixDirty = true;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Math::point& 
nCharJoint::GetVariationScale() const
{
    return this->variationScale;
}

//------------------------------------------------------------------------------
/**
    Clear the uptodate flag. This flag is used in Evaluate() to check
    whether a parent joint has already been evaluated.
*/
inline
void
nCharJoint::ClearUptodateFlag() 
{
    this->isUptodate = false;
}

//------------------------------------------------------------------------------
/**
    Return true when this joint is uptodate (set when Evaluate()) has been
    called after ClearUptodateFlag().
*/
inline
bool
nCharJoint::IsUptodate() const
{
    return this->isUptodate;
}

//------------------------------------------------------------------------------
/**
    This computes the skinning matrix from the pose matrix, the translation,
    the rotation and the scale of the joint. The parent joint must already be
    uptodate!
*/
inline
void
nCharJoint::Evaluate()
{
    if (!this->isUptodate)
    {
        // any changes in position/rotation/etc ?
        if (this->matrixDirty)
        {
            this->rotate = Math::quaternion::normalize(this->rotate);
            Math::matrix44 rotateMatrix = Math::matrix44::rotationquaternion(this->rotate);

            // we need 2 local matrices, one scaled, one unscaled
            // the unscaled one is for our children, who need a parent matrix with uniform axis
            // the scaled one is for calculating the correct skin matrix
            this->localUnscaledMatrix = rotateMatrix;
            this->localUnscaledMatrix.translate(this->translate);

            this->localScaledMatrix = Math::matrix44::identity();
            this->localScaledMatrix.scale(this->scale);
            this->localScaledMatrix.scale(this->variationScale);
            this->localScaledMatrix = Math::matrix44::multiply(this->localScaledMatrix, rotateMatrix);
            this->localScaledMatrix.translate(this->translate);

            this->matrixDirty = false;
        };

        if (!this->lockMatrix)
        {
            this->worldScaledMatrix = this->localScaledMatrix;
            this->worldUnscaledMatrix = this->localUnscaledMatrix;

            if (this->parentJoint)
            {
                if (!this->parentJoint->IsUptodate())
                {
                    this->parentJoint->Evaluate();
                }

                // joint translation is affected by parent scale while the actual axis are not
                Math::point trans = this->worldUnscaledMatrix.get_position();
                trans.x() *= this->parentJoint->scale.x() * this->parentJoint->variationScale.x();
                trans.y() *= this->parentJoint->scale.y() * this->parentJoint->variationScale.y();
                trans.z() *= this->parentJoint->scale.z() * this->parentJoint->variationScale.z();
                this->worldUnscaledMatrix.set_position(trans);

                trans = this->worldScaledMatrix.get_position();
                trans.x() *= this->parentJoint->scale.x() * this->parentJoint->variationScale.x();
                trans.y() *= this->parentJoint->scale.y() * this->parentJoint->variationScale.y();
                trans.z() *= this->parentJoint->scale.z() * this->parentJoint->variationScale.z();
                this->worldScaledMatrix.set_position(trans);

                // we calculate 2 world matrices
                // the unscaled one has uniform axis, which our children need to calculate their matrices
                // the scaled one is the one used to calculate the skinmatrix (the applied scaling is the local,
                // parent scaling which influences the translation of the joint has been handled above)
                this->worldUnscaledMatrix = Math::matrix44::multiply(this->worldUnscaledMatrix, this->parentJoint->worldUnscaledMatrix);
                this->worldScaledMatrix = Math::matrix44::multiply(this->worldScaledMatrix, this->parentJoint->worldUnscaledMatrix);
            }
        }

        this->skinMatrix44 = Math::matrix44::multiply(this->invPoseMatrix, this->worldScaledMatrix);
        
        this->isUptodate = true;
    }
}

//------------------------------------------------------------------------------
/**
    Return the 4x4 skinning matrix. This is the current evaluated matrix
    multiplied by the inverse bind pose matrix.
*/
inline
const Math::matrix44&
nCharJoint::GetSkinMatrix44() const
{
    return this->skinMatrix44;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nCharJoint::SetName(const Util::String& name)
{
    this->name = name;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Util::String&
nCharJoint::GetName() const
{
    return this->name;
}

//------------------------------------------------------------------------------
/**
    Return the bind pose matrix. This matrix is already flattened into 
    model space.
*/
inline
const Math::matrix44&
nCharJoint::GetPoseMatrix() const
{
    return this->poseMatrix;
}

//------------------------------------------------------------------------------
/**
    Return the inverse bind pose matrix.
*/
inline
const Math::matrix44&
nCharJoint::GetInvPoseMatrix() const
{
    return this->invPoseMatrix;
}

//------------------------------------------------------------------------------
/**
    Set the model space matrix directly. This sets a flag for Evaluate()
    which tells it not to multiply by the parent joint's matrix.
*/
inline
void
nCharJoint::SetMatrix(const Math::matrix44& m)
{
    this->worldScaledMatrix = m;
    this->lockMatrix = true;
}

//------------------------------------------------------------------------------
/**
    Return the current evaluated matrix in model space. This matrix is only
    valid after Evaluate() has been called.
*/
inline
const Math::matrix44&
nCharJoint::GetMatrix() const
{
    return this->worldScaledMatrix;
}

//------------------------------------------------------------------------------
/**
    Directly set the local matrix. This will clear the dirty flag, which
    means the joint's translate/rotate/scale components are disabled.
*/
inline
void
nCharJoint::SetLocalMatrix(const Math::matrix44& m)
{
    this->localScaledMatrix = m;
    this->matrixDirty = false;
}

//------------------------------------------------------------------------------
/**
    Return the current evaluated matrix in local space. This matrix is only
    valid after Evaluate() has been called.
*/
inline
const Math::matrix44&
nCharJoint::GetLocalMatrix() const
{
    return this->localScaledMatrix;
}

}; // namespace Nebula2
//------------------------------------------------------------------------------
#endif
