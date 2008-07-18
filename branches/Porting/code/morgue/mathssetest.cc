//------------------------------------------------------------------------------
//  mathssetest.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "test/mathssetest.h"
#include "math/math_sse.h"

namespace Test
{
ImplementClass(Test::MathSSETest, 'msst', Test::TestCase);

//------------------------------------------------------------------------------
/**
*/
void
MathSSETest::Run()
{
    n_align(16) float alignedMemVec[4] = { 1.0f, 2.0f, 3.0f, 4.0f };
    n_align(4) float unalignedMemVec[4] = { 1.0f, 2.0f, 3.0f, 4.0f };
    n_align(16) float alignedMemMatrix[16] = { 1.0f, 0.0f, 0.0f, 0.0f, 
                                             0.0f, 1.0f, 0.0f, 0.0f,
                                             0.0f, 0.0f, 1.0f, 0.0f,
                                             0.0f, 0.0f, 0.0f, 1.0f };
    n_align(4) float unalignedMemMatrix[16] = { 1.0f, 0.0f, 0.0f, 0.0f, 
                                              0.0f, 1.0f, 0.0f, 0.0f,
                                              0.0f, 0.0f, 1.0f, 0.0f,
                                              0.0f, 0.0f, 0.0f, 1.0f };
    // test load functions
    float4_sse v0, v1, v2;
    float4x4_sse m0;
    
    v0 = load_sse(alignedMemVec);
    v1 = loadu_sse(unalignedMemVec);
    this->Verify(v0 == v1);
    this->Verify(v0 == _float4_sse(1.0f, 2.0f, 3.0f, 4.0f));
    this->Verify(v1 == _float4_sse(1.0f, 2.0f, 3.0f, 4.0f));

    m0 = loadcm_sse(alignedMemMatrix);
    this->Verify(m0.col[0] == _float4_sse(1.0f, 0.0f, 0.0f, 0.0f));
    this->Verify(m0.col[1] == _float4_sse(0.0f, 1.0f, 0.0f, 0.0f));
    this->Verify(m0.col[2] == _float4_sse(0.0f, 0.0f, 1.0f, 0.0f));
    this->Verify(m0.col[3] == _float4_sse(0.0f, 0.0f, 0.0f, 1.0f));
    
    m0 = loadcmu_sse(unalignedMemMatrix);
    this->Verify(m0.col[0] == _float4_sse(1.0f, 0.0f, 0.0f, 0.0f));
    this->Verify(m0.col[1] == _float4_sse(0.0f, 1.0f, 0.0f, 0.0f));
    this->Verify(m0.col[2] == _float4_sse(0.0f, 0.0f, 1.0f, 0.0f));
    this->Verify(m0.col[3] == _float4_sse(0.0f, 0.0f, 0.0f, 1.0f));

    m0 = loadrm_sse(alignedMemMatrix);
    this->Verify(m0.col[0] == _float4_sse(1.0f, 0.0f, 0.0f, 0.0f));
    this->Verify(m0.col[1] == _float4_sse(0.0f, 1.0f, 0.0f, 0.0f));
    this->Verify(m0.col[2] == _float4_sse(0.0f, 0.0f, 1.0f, 0.0f));
    this->Verify(m0.col[3] == _float4_sse(0.0f, 0.0f, 0.0f, 1.0f));

    m0 = loadrm_sse(unalignedMemMatrix);
    this->Verify(m0.col[0] == _float4_sse(1.0f, 0.0f, 0.0f, 0.0f));
    this->Verify(m0.col[1] == _float4_sse(0.0f, 1.0f, 0.0f, 0.0f));
    this->Verify(m0.col[2] == _float4_sse(0.0f, 0.0f, 1.0f, 0.0f));
    this->Verify(m0.col[3] == _float4_sse(0.0f, 0.0f, 0.0f, 1.0f));

    // test set functions
    v0 = setzero_sse();
    this->Verify(v0 == _float4_sse(0.0f, 0.0f, 0.0f, 0.0f));
    v0 = set_sse(5.0f);
    this->Verify(v0 == _float4_sse(5.0f, 5.0f, 5.0f, 5.0f));
    v0 = setx_sse(v0, 1.0f);
    this->Verify(v0 == _float4_sse(1.0f, 5.0f, 5.0f, 5.0f));
    v0 = sety_sse(v0, 2.0f);
    this->Verify(v0 == _float4_sse(1.0f, 2.0f, 5.0f, 5.0f));
    v0 = setz_sse(v0, 3.0f);
    this->Verify(v0 == _float4_sse(1.0f, 2.0f, 3.0f, 5.0f));
    v0 = setw_sse(v0, 4.0f);
    this->Verify(v0 == _float4_sse(1.0f, 2.0f, 3.0f, 4.0f));

    m0 = setidentity_sse();
    this->Verify(m0.col[0] == _float4_sse(1.0f, 0.0f, 0.0f, 0.0f));
    this->Verify(m0.col[1] == _float4_sse(0.0f, 1.0f, 0.0f, 0.0f));
    this->Verify(m0.col[2] == _float4_sse(0.0f, 0.0f, 1.0f, 0.0f));
    this->Verify(m0.col[3] == _float4_sse(0.0f, 0.0f, 0.0f, 1.0f));

    setcol0_sse(m0, _float4_sse(1.0f, 2.0f, 3.0f, 4.0f));
    setcol1_sse(m0, _float4_sse(5.0f, 6.0f, 7.0f, 8.0f));
    setcol2_sse(m0, _float4_sse(9.0f, 10.0f, 11.0f, 12.0f));
    setcol3_sse(m0, _float4_sse(13.0f, 14.0f, 15.0f, 16.0f));
    this->Verify(m0.col[0] == _float4_sse(1.0f, 2.0f, 3.0f, 4.0f));
    this->Verify(m0.col[1] == _float4_sse(5.0f, 6.0f, 7.0f, 8.0f));
    this->Verify(m0.col[2] == _float4_sse(9.0f, 10.0f, 11.0f, 12.0f));
    this->Verify(m0.col[3] == _float4_sse(13.0f, 14.0f, 15.0f, 16.0f));

    setrow0_sse(m0, _float4_sse(1.0f, 2.0f, 3.0f, 4.0f));
    setrow1_sse(m0, _float4_sse(5.0f, 6.0f, 7.0f, 8.0f));
    setrow2_sse(m0, _float4_sse(9.0f, 10.0f, 11.0f, 12.0f));
    setrow3_sse(m0, _float4_sse(13.0f, 14.0f, 15.0f, 16.0f));
    this->Verify(m0.col[0] == _float4_sse(1.0f, 5.0f, 9.0f, 13.0f));
    this->Verify(m0.col[1] == _float4_sse(2.0f, 6.0f, 10.0f, 14.0f));
    this->Verify(m0.col[2] == _float4_sse(3.0f, 7.0f, 11.0f, 15.0f));
    this->Verify(m0.col[3] == _float4_sse(4.0f, 8.0f, 12.0f, 16.0f));

    //--- get functions ---
    v0 = _float4_sse(1.0f, 2.0f, 3.0f, 4.0f);
    this->Verify(getx_sse(v0) == 1.0f);
    this->Verify(gety_sse(v0) == 2.0f);
    this->Verify(getz_sse(v0) == 3.0f);
    this->Verify(getw_sse(v0) == 4.0f);
    m0 = _float4x4_sse();
    this->Verify(getcol0_sse(m0) == _float4_sse(1.0f, 0.0f, 0.0f, 0.0f));
    this->Verify(getcol1_sse(m0) == _float4_sse(0.0f, 1.0f, 0.0f, 0.0f));
    this->Verify(getcol2_sse(m0) == _float4_sse(0.0f, 0.0f, 1.0f, 0.0f));
    this->Verify(getcol3_sse(m0) == _float4_sse(0.0f, 0.0f, 0.0f, 1.0f));
    this->Verify(getrow0_sse(m0) == _float4_sse(1.0f, 0.0f, 0.0f, 0.0f));
    this->Verify(getrow1_sse(m0) == _float4_sse(0.0f, 1.0f, 0.0f, 0.0f));
    this->Verify(getrow2_sse(m0) == _float4_sse(0.0f, 0.0f, 1.0f, 0.0f));
    this->Verify(getrow3_sse(m0) == _float4_sse(0.0f, 0.0f, 0.0f, 1.0f));

    // test store functions
    n_align(16) float alignedVecBuffer[4];
    n_align(4) float unalignedVecBuffer[4];
    n_align(16) float alignedMatrixBuffer[16];
    n_align(4) float unalignedMatrixBuffer[16];
    float* ptr;

    v0 = _float4_sse(1.0f, 2.0f, 3.0f, 4.0f);
    Memory::Clear(alignedVecBuffer, sizeof(alignedVecBuffer));
    ptr = alignedVecBuffer;
    store_sse(ptr, v0);
    this->Verify((1.0f == ptr[0]) && (2.0f == ptr[1]) && (3.0f == ptr[2]) && (4.0f == ptr[3]));

    Memory::Clear(unalignedVecBuffer, sizeof(unalignedVecBuffer));
    ptr = unalignedVecBuffer;
    storeu_sse(ptr, v0);
    this->Verify((1.0f == ptr[0]) && (2.0f == ptr[1]) && (3.0f == ptr[2]) && (4.0f == ptr[3]));

    Memory::Clear(alignedMatrixBuffer, sizeof(alignedMatrixBuffer));
    m0 = _float4x4_sse();
    ptr = alignedMatrixBuffer;
    storecm_sse(ptr, m0);
    this->Verify((1.0f == ptr[0]) && (0.0f == ptr[1]) && (0.0f == ptr[2]) && (0.0f == ptr[3]) &&
                 (0.0f == ptr[4]) && (1.0f == ptr[5]) && (0.0f == ptr[6]) && (0.0f == ptr[7]) &&
                 (0.0f == ptr[8]) && (0.0f == ptr[9]) && (1.0f == ptr[10]) && (0.0f == ptr[11]) &&
                 (0.0f == ptr[12]) && (0.0f == ptr[13]) && (0.0f == ptr[14]) && (1.0f == ptr[15]));
    
    Memory::Clear(unalignedMatrixBuffer, sizeof(unalignedMatrixBuffer));
    ptr = unalignedMatrixBuffer;
    storecmu_sse(ptr, m0);
    this->Verify((1.0f == ptr[0]) && (0.0f == ptr[1]) && (0.0f == ptr[2]) && (0.0f == ptr[3]) &&
                 (0.0f == ptr[4]) && (1.0f == ptr[5]) && (0.0f == ptr[6]) && (0.0f == ptr[7]) &&
                 (0.0f == ptr[8]) && (0.0f == ptr[9]) && (1.0f == ptr[10]) && (0.0f == ptr[11]) &&
                 (0.0f == ptr[12]) && (0.0f == ptr[13]) && (0.0f == ptr[14]) && (1.0f == ptr[15]));

    Memory::Clear(alignedMatrixBuffer, sizeof(alignedMatrixBuffer));
    ptr = alignedMatrixBuffer;
    storerm_sse(ptr, m0);
    this->Verify((1.0f == ptr[0]) && (0.0f == ptr[1]) && (0.0f == ptr[2]) && (0.0f == ptr[3]) &&
                 (0.0f == ptr[4]) && (1.0f == ptr[5]) && (0.0f == ptr[6]) && (0.0f == ptr[7]) &&
                 (0.0f == ptr[8]) && (0.0f == ptr[9]) && (1.0f == ptr[10]) && (0.0f == ptr[11]) &&
                 (0.0f == ptr[12]) && (0.0f == ptr[13]) && (0.0f == ptr[14]) && (1.0f == ptr[15]));

    Memory::Clear(unalignedMatrixBuffer, sizeof(unalignedMatrixBuffer));
    ptr = unalignedMatrixBuffer;
    storermu_sse(ptr, m0);
    this->Verify((1.0f == ptr[0]) && (0.0f == ptr[1]) && (0.0f == ptr[2]) && (0.0f == ptr[3]) &&
                 (0.0f == ptr[4]) && (1.0f == ptr[5]) && (0.0f == ptr[6]) && (0.0f == ptr[7]) &&
                 (0.0f == ptr[8]) && (0.0f == ptr[9]) && (1.0f == ptr[10]) && (0.0f == ptr[11]) &&
                 (0.0f == ptr[12]) && (0.0f == ptr[13]) && (0.0f == ptr[14]) && (1.0f == ptr[15]));

    // test stream store functions
    Memory::Clear(alignedVecBuffer, sizeof(alignedVecBuffer));
    ptr = alignedVecBuffer;
    stream_sse(ptr, v0);
    this->Verify((1.0f == ptr[0]) && (2.0f == ptr[1]) && (3.0f == ptr[2]) && (4.0f == ptr[3]));

    Memory::Clear(alignedMatrixBuffer, sizeof(alignedMatrixBuffer));
    ptr = alignedMatrixBuffer;
    streamcm_sse(ptr, m0);
    this->Verify((1.0f == ptr[0]) && (0.0f == ptr[1]) && (0.0f == ptr[2]) && (0.0f == ptr[3]) &&
                 (0.0f == ptr[4]) && (1.0f == ptr[5]) && (0.0f == ptr[6]) && (0.0f == ptr[7]) &&
                 (0.0f == ptr[8]) && (0.0f == ptr[9]) && (1.0f == ptr[10]) && (0.0f == ptr[11]) &&
                 (0.0f == ptr[12]) && (0.0f == ptr[13]) && (0.0f == ptr[14]) && (1.0f == ptr[15]));

    Memory::Clear(alignedMatrixBuffer, sizeof(alignedMatrixBuffer));
    ptr = alignedMatrixBuffer;
    streamrm_sse(ptr, m0);
    this->Verify((1.0f == ptr[0]) && (0.0f == ptr[1]) && (0.0f == ptr[2]) && (0.0f == ptr[3]) &&
                 (0.0f == ptr[4]) && (1.0f == ptr[5]) && (0.0f == ptr[6]) && (0.0f == ptr[7]) &&
                 (0.0f == ptr[8]) && (0.0f == ptr[9]) && (1.0f == ptr[10]) && (0.0f == ptr[11]) &&
                 (0.0f == ptr[12]) && (0.0f == ptr[13]) && (0.0f == ptr[14]) && (1.0f == ptr[15]));

    // test builtin operators
    v0 = _float4_sse(1.0f, 2.0f, 3.0f, 4.0f);
    v1 = _float4_sse(4.0f, 3.0f, 2.0f, 1.0f);
    v2 = v0 + v1;
    this->Verify(v2 == _float4_sse(5.0f, 5.0f, 5.0f, 5.0f));
    v2 = v1 + v0;
    this->Verify(v2 == _float4_sse(5.0f, 5.0f, 5.0f, 5.0f));
    v2 = v0 + 1.0f;
    this->Verify(v2 == _float4_sse(2.0f, 3.0f, 4.0f, 5.0f));
    v2 = 1.0f + v0;
    this->Verify(v2 == _float4_sse(2.0f, 3.0f, 4.0f, 5.0f));
    v2 = v0 - v1;
    this->Verify(v2 == _float4_sse(-3.0f, -1.0f, 1.0f, 3.0f));
    v2 = v1 - v0;
    this->Verify(v2 == _float4_sse(3.0f, 1.0f, -1.0f, -3.0f));
    v2 = v0 - 1.0f;
    this->Verify(v2 == _float4_sse(0.0f, 1.0f, 2.0f, 3.0f));
    v2 = v0 * v1;
    this->Verify(v2 == _float4_sse(4.0f, 6.0f, 6.0f, 4.0f));
    v2 = v1 * v0;
    this->Verify(v2 == _float4_sse(4.0f, 6.0f, 6.0f, 4.0f));
    v2 = v0 * 2.0f;
    this->Verify(v2 == _float4_sse(2.0f, 4.0f, 6.0f, 8.0f));
    v2 = 2.0f * v0;
    this->Verify(v2 == _float4_sse(2.0f, 4.0f, 6.0f, 8.0f));
    v2 = v0 / v1;
    this->Verify(v2 == _float4_sse(0.25f, 2.0f / 3.0f, 1.5f, 4.0f));
    v2 = v1 / v0;
    this->Verify(v2 == _float4_sse(4.0f, 1.5f, 2.0f / 3.0f, 0.25f));
    v2 = v0 / 2.0f;
    this->Verify(v2 == _float4_sse(0.5f, 1.0f, 1.5f, 2.0f));

    // min/max
    v0 = _float4_sse(1.0f, 2.0f, 4.0f, 8.0f);
    v1 = _float4_sse(8.0f, 4.0f, 2.0f, 1.0f);
    this->Verify(min_sse(2.0f, 3.0f) == 2.0f);
    this->Verify(min_sse(v0, v1) == _float4_sse(1.0f, 2.0f, 2.0f, 1.0f));
    this->Verify(max_sse(2.0f, 3.0f) == 3.0f);
    this->Verify(max_sse(v0, v1) == _float4_sse(8.0f, 4.0f, 4.0f, 8.0f));

    // rcp
    this->Verify(rcp_sse(2.0f) == 0.5f);
    this->Verify(isequal_sse(rcp_sse(v0), _float4_sse(1.0f, 0.5f, 0.25f, 0.125f), 0.001f));
    this->Verify(isequal_sse(rcp_sse(v1), _float4_sse(0.125f, 0.25f, 0.5f, 1.0f), 0.001f));

    // sqrt
    v0 = _float4_sse(4.0f, 9.0f, 16.0f, 25.0f);
    this->Verify(sqrt_sse(9.0f) == 3.0f);
    this->Verify(sqrt_sse(v0) == _float4_sse(2.0f, 3.0f, 4.0f, 5.0f));
    this->Verify(rsqrt_sse(16.0f) == 0.25f);
    this->Verify(isequal_sse(rsqrt_sse(v0), _float4_sse(0.5f, 1.0f / 3.0f, 0.25f, 0.2f), 0.001f));

    // horizontal sum
    this->Verify(hsum_sse(v0) == 54.0f);
    
    // abs
    v0 = _float4_sse(1.0f, -2.0f, 3.0f, -4.0f);
    this->Verify(abs_sse(2.0f) == 2.0f);
    this->Verify(abs_sse(-2.0f) == 2.0f);
    this->Verify(abs_sse(v0) == _float4_sse(1.0f, 2.0f, 3.0f, 4.0f));

    // cos/acos
    float s = radians_sse(90.0f);
    s = cos_sse(s);
    s = acos_sse(s);
    s = degrees_sse(s);
    this->Verify(s == 90.0f);

    v0 = radians_sse(_float4_sse(90.0f));
    v0 = cos_sse(v0);
    v0 = acos_sse(v0);
    v0 = degrees_sse(v0);
    this->Verify(v0 == _float4_sse(90.0f));

    // cross()
    v0 = _float4_sse(1.0f, 0.0f, 0.0f, 0.0f);
    v1 = _float4_sse(0.0f, 1.0f, 0.0f, 0.0f);
    v2 = cross_sse(v0, v1);
    this->Verify(v2 == _float4_sse(0.0f, 0.0f, 1.0f, 0.0f));

    // distance()
    v0 = _float4_sse(1.0f, 2.0f, 3.0f, 1.0f);
    v1 = _float4_sse(5.0f, 2.0f, 3.0f, 1.0f);
    this->Verify(distance_sse(v0, v1) == 4.0f);

    // distancesquared()
    this->Verify(distancesquared_sse(v0, v1) == 16.0f);

    // dot()
    v0 = _float4_sse(1.0f, 0.0f, 0.0f, 0.0f);
    v1 = _float4_sse(-1.0f, 0.0f, 0.0f, 0.0f);
    v2 = _float4_sse(0.0f, 1.0f, 0.0f, 0.0f);
    this->Verify(dot_sse(v0, v0) == 1.0f);
    this->Verify(dot_sse(v0, v1) == -1.0f);
    this->Verify(dot_sse(v0, v2) == 0.0f);

    // FIXME: exp()

    // floor()
    this->Verify(floor_sse(2.8f) == 2.0f);
    this->Verify(floor_sse(-2.1f) == -3.0f);
    this->Verify(floor_sse(_float4_sse(1.2f, 2.3f, 3.8f, 4.6f)) == _float4_sse(1.0f, 2.0f, 3.0f, 4.0f));

    // fmod()
    this->Verify(fmod_sse(7.5f, 3.0f) == 1.5f);
    this->Verify(fmod_sse(_float4_sse(2.5f, 3.5f, 4.5f, 5.5f), _float4_sse(1.0f, 2.0f, 3.0f, 4.0f)) == _float4_sse(0.5f, 1.5f, 1.5f, 1.5f)); 

    // mul(vec, vec)
    this->Verify(mul_sse(v0, v1) == dot_sse(v0, v1));

    // mul(vec, scalar);
    v0 = _float4_sse(2.0f, 3.0f, 4.0f, 1.0f);   // a position
    this->Verify(mul_sse(_float4_sse(2.0f, 3.0f, 4.0f, 1.0f), 2.0f) == _float4_sse(4.0f, 6.0f, 8.0f, 2.0f));
    this->Verify(mul_sse(2.0f, _float4_sse(2.0f, 3.0f, 4.0f, 1.0f)) == _float4_sse(4.0f, 6.0f, 8.0f, 2.0f));

    // mul(vec, matrix)
    v0 = _float4_sse(2.0f, 3.0f, 4.0f, 1.0f);   // a position
    v1 = _float4_sse(2.0f, 3.0f, 4.0f, 0.0f);   // a direction
    m0 = _float4x4_sse();   // an identity matrix
    v2 = mul_sse(v0, m0);
    this->Verify(v0 == v2);
    v2 = mul_sse(v1, m0);
    this->Verify(v1 == v2);

    // test mul_sse() with translation matrix
    setrow0_sse(m0, _float4_sse(1.0f, 0.0f, 0.0f, 0.0f));
    setrow1_sse(m0, _float4_sse(0.0f, 1.0f, 0.0f, 0.0f));
    setrow2_sse(m0, _float4_sse(0.0f, 0.0f, 1.0f, 0.0f));
    setrow3_sse(m0, _float4_sse(4.0f, 3.0f, 2.0f, 1.0f));
    v2 = mul_sse(v0, m0);
    this->Verify(v2 == _float4_sse(6.0f, 6.0f, 6.0f, 1.0f));
    v2 = mul_sse(v1, m0);
    this->Verify(v2 == _float4_sse(2.0f, 3.0f, 4.0f, 0.0f));
}

} // namespace Test
