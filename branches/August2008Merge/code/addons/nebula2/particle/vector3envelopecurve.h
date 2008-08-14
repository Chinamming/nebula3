#pragma once
#ifndef MATH_VECTOR3ENVELOPECURVE_H
#define MATH_VECTOR3ENVELOPECURVE_H
//------------------------------------------------------------------------------
/**
    @class Math::Vector3EnvelopeCurve

    A 3-dimensional envelope curve.

    (C) 2004 RadonLabs GmbH
*/
//#include "math/math.h"
//#include "math/vector.h"

//------------------------------------------------------------------------------
namespace Nebula2
{
class Vector3EnvelopeCurve
{
public:
    /// constructor 1
    Vector3EnvelopeCurve();
    /// constructor 2
	Vector3EnvelopeCurve(const Math::float4& keyFrameValue0, const Math::float4& keyFrameValue1,
        const Math::float4& keyFrameValue2, const Math::float4& keyFrameValue3,
        const float keyFramePos1, const float keyFramePos2);
    // set all parameters
    void SetParameters(const Math::float4& keyFrameValue0, const Math::float4& keyFrameValue1,
        const Math::float4& keyFrameValue2, const Math::float4& keyFrameValue3,
        const float keyFramePos1, const float keyFramePos2);
    // assign to other color envelope curve
    void SetParameters(const Vector3EnvelopeCurve& src);
    /// get the function value; pos must be between 0 and 1
    const Math::float4 GetValue(float pos) const;

    enum
    {
        NumValues = 4,
    };

    Math::float4 keyFrameValues[NumValues];
    float keyFramePos1, keyFramePos2;   // 0 through 1
    float frequency, amplitude;         // parameters of the sinus function
};

//------------------------------------------------------------------------------
/**
*/
inline
Vector3EnvelopeCurve::Vector3EnvelopeCurve() :
    keyFramePos1(.2f),
    keyFramePos2(.8f)
{
    keyFrameValues[0] = Math::float4(1.0f, 1.0f, 1.0f,0);
    keyFrameValues[1] = Math::float4(1.0f, 1.0f, 1.0f,0);
    keyFrameValues[2] = Math::float4(1.0f, 1.0f, 1.0f,0);
    keyFrameValues[3] = Math::float4(1.0f, 1.0f, 1.0f,0);
}

//------------------------------------------------------------------------------
/**
*/
inline
Vector3EnvelopeCurve::Vector3EnvelopeCurve(const Math::float4& keyFrameValue0, 
        const Math::float4& keyFrameValue1, const Math::float4& keyFrameValue2,
        const Math::float4& keyFrameValue3, const float keyFramePos1,
        const float keyFramePos2) :
    keyFramePos1(keyFramePos1),
    keyFramePos2(keyFramePos2)
{
    this->keyFrameValues[0] = keyFrameValue0;
    this->keyFrameValues[1] = keyFrameValue1;
    this->keyFrameValues[2] = keyFrameValue2;
    this->keyFrameValues[3] = keyFrameValue3;
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
Vector3EnvelopeCurve::SetParameters(const Math::float4& keyFrameValue0, const Math::float4& keyFrameValue1,
    const Math::float4& keyFrameValue2, const Math::float4& keyFrameValue3,
    const float keyFramePos1, const float keyFramePos2)
{
    this->keyFrameValues[0] = keyFrameValue0;
    this->keyFrameValues[1] = keyFrameValue1;
    this->keyFrameValues[2] = keyFrameValue2;
    this->keyFrameValues[3] = keyFrameValue3;
    this->keyFramePos1 = keyFramePos1;
    this->keyFramePos2 = keyFramePos2;
}
//------------------------------------------------------------------------------
/**
*/
inline
void 
Vector3EnvelopeCurve::SetParameters(const Vector3EnvelopeCurve& src)
{
    this->keyFrameValues[0] = src.keyFrameValues[0];
    this->keyFrameValues[1] = src.keyFrameValues[1];
    this->keyFrameValues[2] = src.keyFrameValues[2];
    this->keyFrameValues[3] = src.keyFrameValues[3];
    this->keyFramePos1 = src.keyFramePos1;
    this->keyFramePos2 = src.keyFramePos2;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Math::float4
Vector3EnvelopeCurve::GetValue(float pos) const
{
    n_assert(pos >= 0.0);
    n_assert(pos <= 1.0);

    Math::float4 linearValue;
    if (pos < this->keyFramePos1)
    {
        linearValue = this->keyFrameValues[1];
		linearValue = Math::float4::lerp(linearValue,this->keyFrameValues[0], 
            (pos / this->keyFramePos1));
    }
    else if (pos < this->keyFramePos2)
    {
        linearValue = this->keyFrameValues[2];
		linearValue = Math::float4::lerp(linearValue, this->keyFrameValues[1], 
            (pos-this->keyFramePos1) / (this->keyFramePos2-this->keyFramePos1));
    }
    else
    {
        linearValue = this->keyFrameValues[3];
		linearValue = Math::float4::lerp(linearValue, this->keyFrameValues[2],
            (pos-this->keyFramePos2) / (1.0f-this->keyFramePos2));
    }

    return linearValue;
}

}; // namespace Nebula2
//------------------------------------------------------------------------------
#endif
