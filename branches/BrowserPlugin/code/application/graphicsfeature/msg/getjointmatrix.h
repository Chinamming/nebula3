#ifndef MSG_GETJOINTMATRIX_H
#define MSG_GETJOINTMATRIX_H
//------------------------------------------------------------------------------
/**
    @class Messaging::GetJointMatrix
    
    Returns the current transformation of a character joint in world space.
    
    (C) 2006 Radon Labs GmbH
*/
#include "messaging/message.h"
#include "math/matrix44.h"

//------------------------------------------------------------------------------
namespace GraphicsFeature
{
class GetJointMatrix : public Messaging::Message
{
    DeclareClass(GetJointMatrix);
    DeclareMsgId;
public:
    /// constructor
    GetJointMatrix();
    /// set joint name
    void SetJointName(const Util::String& n);
    /// get joint name
    const Util::String& GetJointName() const;
    /// set joint matrix
    void SetMatrix(const Math::matrix44& m);
    /// get joint matrix
    const Math::matrix44& GetMatrix() const;
    /// return true if joint matrix was set by target entity.
    bool IsJointMatrixSet() const;	
	/// set if get local joint matrix
    void SetLocalJointMatrixOnly(const bool localJointMatrixOnly);
	/// get local joint matrix
    bool IsLocalJointMatrixOnly() const;	
    
private:        

    Util::String jointName;
    Math::matrix44 jointMatrix;
    bool isJointMatrixSet;
	bool localJointMatrixOnly;
};

//------------------------------------------------------------------------------
/**
*/
inline
void
GetJointMatrix::SetJointName(const Util::String& n)
{
    n_assert(n.IsValid());
    this->jointName = n;

    // reset set handled flag
    this->isJointMatrixSet = false;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Util::String&
GetJointMatrix::GetJointName() const
{
    return this->jointName;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
GetJointMatrix::SetMatrix(const Math::matrix44& m)
{
    this->jointMatrix = m;

    // set handled flag
    this->isJointMatrixSet = true;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Math::matrix44&
GetJointMatrix::GetMatrix() const
{
    return this->jointMatrix;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
GetJointMatrix::IsJointMatrixSet() const
{
    // reset set handled flag
    return this->isJointMatrixSet;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
GetJointMatrix::SetLocalJointMatrixOnly(const bool b) 
{  
	this->localJointMatrixOnly = b;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
GetJointMatrix::IsLocalJointMatrixOnly() const
{
    // reset set handled flag
    return this->localJointMatrixOnly;
}
} // namespace GraphicsFeature
//------------------------------------------------------------------------------
#endif

    