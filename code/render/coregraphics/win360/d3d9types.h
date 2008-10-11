#pragma once
#ifndef WIN360_D3D9TYPES_H
#define WIN360_D3D9TYPES_H
//------------------------------------------------------------------------------
/**
    @class Win360::D3D9Types
    
    Provides static helper functions to convert from and to Direct3D
    data types and enumerations.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/types.h"
#include "coregraphics/pixelformat.h"
#include "coregraphics/vertexcomponent.h"
#include "coregraphics/primitivetopology.h"
#include "coregraphics/antialiasquality.h"
#include "coregraphics/imagefileformat.h"

//------------------------------------------------------------------------------
namespace Win360
{
class D3D9Types
{
public:
    /// convert Nebula pixel format to D3D9 pixel format
    static D3DFORMAT AsD3D9PixelFormat(CoreGraphics::PixelFormat::Code p);
    /// convert Direct3D to Nebula pixel format
    static CoreGraphics::PixelFormat::Code AsNebulaPixelFormat(D3DFORMAT f);
    /// convert vertex component type to D3D9 declaration type
    static D3DDECLTYPE AsD3D9VertexDeclarationType(CoreGraphics::VertexComponent::Format f);
    /// convert vertex component semantic name as D3D9 declaration usage
    static D3DDECLUSAGE AsD3D9VertexDeclarationUsage(CoreGraphics::VertexComponent::SemanticName n);
    /// convert primitive topology to D3D
    static D3DPRIMITIVETYPE AsD3D9PrimitiveType(CoreGraphics::PrimitiveTopology::Code t);
    /// convert antialias quality to D3D multisample type
    static D3DMULTISAMPLE_TYPE AsD3D9MultiSampleType(CoreGraphics::AntiAliasQuality::Code c);
    /// convert image file format to D3DX file format
    static D3DXIMAGE_FILEFORMAT AsD3DXImageFileFormat(CoreGraphics::ImageFileFormat::Code c);
};

//------------------------------------------------------------------------------
/**
*/
inline D3DFORMAT
D3D9Types::AsD3D9PixelFormat(CoreGraphics::PixelFormat::Code p)
{
    using namespace CoreGraphics;
    switch (p)
    {
        case PixelFormat::X8R8G8B8:         return D3DFMT_X8R8G8B8;
        case PixelFormat::A8R8G8B8:         return D3DFMT_A8R8G8B8;
        case PixelFormat::R5G6B5:           return D3DFMT_R5G6B5;
        case PixelFormat::A1R5G5B5:         return D3DFMT_A1R5G5B5;
        case PixelFormat::A4R4G4B4:         return D3DFMT_A4R4G4B4;
        case PixelFormat::DXT1:             return D3DFMT_DXT1;
        case PixelFormat::DXT3:             return D3DFMT_DXT3;
        case PixelFormat::DXT5:             return D3DFMT_DXT5;
        case PixelFormat::R16F:             return D3DFMT_R16F;
        case PixelFormat::G16R16F:          return D3DFMT_G16R16F;
        case PixelFormat::A16B16G16R16F:    return D3DFMT_A16B16G16R16F;
        case PixelFormat::R32F:             return D3DFMT_R32F;
        case PixelFormat::G32R32F:          return D3DFMT_G32R32F;
        case PixelFormat::A32B32G32R32F:    return D3DFMT_A32B32G32R32F;
        case PixelFormat::A8:               return D3DFMT_A8;
        case PixelFormat::A2R10G10B10:      return D3DFMT_A2R10G10B10;
        case PixelFormat::G16R16:           return D3DFMT_G16R16;
        #if __XBOX360__
        case PixelFormat::X2R10G10B10:      return D3DFMT_X2R10G10B10;
        case PixelFormat::LINDXT1:          return D3DFMT_LIN_DXT1;
        case PixelFormat::LINDXT3:          return D3DFMT_LIN_DXT3;
        case PixelFormat::LINDXT5:          return D3DFMT_LIN_DXT5;
        case PixelFormat::LINA8R8G8B8:      return D3DFMT_LIN_A8R8G8B8;
        case PixelFormat::LINX8R8G8B8:      return D3DFMT_LIN_X8R8G8B8;
        case PixelFormat::EDG16R16:         return D3DFMT_G16R16_EDRAM;
        #endif
        default:                            return D3DFMT_UNKNOWN;
    }
}

//------------------------------------------------------------------------------
/**
*/
inline CoreGraphics::PixelFormat::Code
D3D9Types::AsNebulaPixelFormat(D3DFORMAT f)
{
    using namespace CoreGraphics;
    switch (f)
    {
        case D3DFMT_X8R8G8B8:               return PixelFormat::X8R8G8B8;
        case D3DFMT_A8R8G8B8:               return PixelFormat::A8R8G8B8;
        case D3DFMT_R5G6B5:                 return PixelFormat::R5G6B5;
        case D3DFMT_A1R5G5B5:               return PixelFormat::A1R5G5B5;
        case D3DFMT_A4R4G4B4:               return PixelFormat::A4R4G4B4;
        case D3DFMT_DXT1:                   return PixelFormat::DXT1;
        case D3DFMT_DXT3:                   return PixelFormat::DXT3;
        case D3DFMT_DXT5:                   return PixelFormat::DXT5;
        case D3DFMT_R16F:                   return PixelFormat::R16F;
        case D3DFMT_G16R16F:                return PixelFormat::G16R16F;
        case D3DFMT_A16B16G16R16F:          return PixelFormat::A16B16G16R16F;
        case D3DFMT_R32F:                   return PixelFormat::R32F;
        case D3DFMT_G32R32F:                return PixelFormat::G32R32F;
        case D3DFMT_A32B32G32R32F:          return PixelFormat::A32B32G32R32F;
        case D3DFMT_A8:                     return PixelFormat::A8;
        case D3DFMT_A2R10G10B10:            return PixelFormat::A2R10G10B10;
        case D3DFMT_G16R16:                 return PixelFormat::G16R16;
        #if __XBOX360__
        case D3DFMT_X2R10G10B10:        return PixelFormat::X2R10G10B10;
        case D3DFMT_LIN_DXT1:           return PixelFormat::LINDXT1;
        case D3DFMT_LIN_DXT3:           return PixelFormat::LINDXT3;
        case D3DFMT_LIN_DXT5:           return PixelFormat::LINDXT5;
        case D3DFMT_LIN_A8R8G8B8:       return PixelFormat::LINA8R8G8B8;
        case D3DFMT_LIN_X8R8G8B8:       return PixelFormat::LINX8R8G8B8;
        case D3DFMT_G16R16_EDRAM:       return PixelFormat::EDG16R16;
        #endif
        default:                        return PixelFormat::InvalidPixelFormat;
    }
}

//------------------------------------------------------------------------------
/**
*/
inline D3DDECLTYPE
D3D9Types::AsD3D9VertexDeclarationType(CoreGraphics::VertexComponent::Format f)
{
    using namespace CoreGraphics;
    switch (f)
    {
        case VertexComponent::Float:    return D3DDECLTYPE_FLOAT1;
        case VertexComponent::Float2:   return D3DDECLTYPE_FLOAT2;
        case VertexComponent::Float3:   return D3DDECLTYPE_FLOAT3;
        case VertexComponent::Float4:   return D3DDECLTYPE_FLOAT4;
        case VertexComponent::UByte4:   return D3DDECLTYPE_UBYTE4;
        case VertexComponent::Short2:   return D3DDECLTYPE_SHORT2;
        case VertexComponent::Short4:   return D3DDECLTYPE_SHORT4;
        case VertexComponent::UByte4N:  return D3DDECLTYPE_UBYTE4N;
        case VertexComponent::Short2N:  return D3DDECLTYPE_SHORT2N;
        case VertexComponent::Short4N:  return D3DDECLTYPE_SHORT4N;
        default:                        
            n_error("D3D9Types::AsDirect3DVertexDeclarationType(): invalid input parameter!");
            return D3DDECLTYPE_UNUSED;
    }
}

//------------------------------------------------------------------------------
/**
*/
inline D3DDECLUSAGE
D3D9Types::AsD3D9VertexDeclarationUsage(CoreGraphics::VertexComponent::SemanticName n)
{
    using namespace CoreGraphics;
    switch (n)
    {
        case VertexComponent::Position:     return D3DDECLUSAGE_POSITION;
        case VertexComponent::Normal:       return D3DDECLUSAGE_NORMAL;
        case VertexComponent::Tangent:      return D3DDECLUSAGE_TANGENT;
        case VertexComponent::Binormal:     return D3DDECLUSAGE_BINORMAL;
        case VertexComponent::TexCoord:     return D3DDECLUSAGE_TEXCOORD;
        case VertexComponent::SkinWeights:  return D3DDECLUSAGE_BLENDWEIGHT;
        case VertexComponent::SkinJIndices: return D3DDECLUSAGE_BLENDINDICES;
        case VertexComponent::Color:        return D3DDECLUSAGE_COLOR;
        default:
            n_error("D3D9Types::AsDirect3DVertexDeclarationUsage(): invalid input parameter!");
            return D3DDECLUSAGE_POSITION;
    }
}

//------------------------------------------------------------------------------
/**
*/
inline D3DPRIMITIVETYPE
D3D9Types::AsD3D9PrimitiveType(CoreGraphics::PrimitiveTopology::Code t)
{
    using namespace CoreGraphics;
    switch (t)
    {
        case PrimitiveTopology::PointList:      return D3DPT_POINTLIST;
        case PrimitiveTopology::LineList:       return D3DPT_LINELIST;
        case PrimitiveTopology::LineStrip:      return D3DPT_LINESTRIP;
        case PrimitiveTopology::TriangleList:   return D3DPT_TRIANGLELIST;
        case PrimitiveTopology::TriangleStrip:  return D3DPT_TRIANGLESTRIP;
        default:
            n_error("D3D9Types::AsDirect3DPrimitiveType(): unsupported topology '%s'!",
                CoreGraphics::PrimitiveTopology::ToString(t).AsCharPtr());
            return D3DPT_TRIANGLELIST;
    }
}

//------------------------------------------------------------------------------
/**
*/
inline D3DMULTISAMPLE_TYPE
D3D9Types::AsD3D9MultiSampleType(CoreGraphics::AntiAliasQuality::Code c)
{
    using namespace CoreGraphics;
    switch (c)
    {
        case AntiAliasQuality::None:    
            return D3DMULTISAMPLE_NONE;
        case AntiAliasQuality::Low:     
            return D3DMULTISAMPLE_2_SAMPLES;
        case AntiAliasQuality::Medium:  
        case AntiAliasQuality::High:
            return D3DMULTISAMPLE_4_SAMPLES;
        default:
            n_error("D3D9Types::AsD3D9MultiSampleType(): unsupported AntiAliasQuality!");
            return D3DMULTISAMPLE_NONE;
    }
}

//------------------------------------------------------------------------------
/**
*/
inline D3DXIMAGE_FILEFORMAT
D3D9Types::AsD3DXImageFileFormat(CoreGraphics::ImageFileFormat::Code c)
{
    using namespace CoreGraphics;
    switch (c)
    {
        case ImageFileFormat::BMP:  return D3DXIFF_BMP;
        case ImageFileFormat::JPG:  return D3DXIFF_JPG;
        case ImageFileFormat::PNG:  return D3DXIFF_PNG;
        case ImageFileFormat::DDS:  return D3DXIFF_DDS;
        default:
            n_error("D3D9Types::AsD3DXImageFileFormat(): unsupported ImageFileFormat!");
            return D3DXIFF_BMP;
    }
}

} // namespace Win360
//------------------------------------------------------------------------------
#endif
