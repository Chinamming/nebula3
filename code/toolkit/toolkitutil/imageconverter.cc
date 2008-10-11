//------------------------------------------------------------------------------
//  imageconverter.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "toolkitutil/imageconverter.h"
#include "IL/il.h"
#include "IL/ilu.h"
#include "io/ioserver.h"
#include "io/uri.h"

namespace ToolkitUtil
{
using namespace Util;
using namespace IO;

//------------------------------------------------------------------------------
/**
*/
ImageConverter::ImageConverter() :
    maxWidth(1024),
    maxHeight(1024),
    dstNumChannels(0),
    dstWidth(0),
    dstHeight(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ImageConverter::~ImageConverter()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ImageConverter::Result
ImageConverter::Convert()
{
    n_assert(this->srcFile.IsValid());
    n_assert(this->dstFile.IsValid());
    IoServer* ioServer = IoServer::Instance();

    // initialize DevIL
    ilInit();

    // need to convert file paths to absolute paths, since IL doesn't
    // know Nebula assigns of course
    URI absSrcPath = ioServer->ResolveAssignsInString(this->srcFile);
    URI absDstPath = ioServer->ResolveAssignsInString(this->dstFile);

    // load image
    ILuint image;
    ilGenImages(1, &image);
    ilBindImage(image);
    ILboolean res = ilLoadImage((ILstring)absSrcPath.LocalPath().AsCharPtr());
    if (IL_TRUE != res)
    {
        ilDeleteImages(1, &image);
        return ErrorOpenSrcFile;
    }
    ILuint imgWidth = ilGetInteger(IL_IMAGE_WIDTH);
    ILuint imgHeight = ilGetInteger(IL_IMAGE_HEIGHT);

    // query image width and height and scale if necessary
    if ((imgWidth > this->maxWidth) || (imgHeight > this->maxHeight))
    {
        iluImageParameter(ILU_FILTER, ILU_SCALE_BOX);
        iluScale(this->maxWidth, this->maxHeight, 1);
    }
    this->dstWidth = ilGetInteger(IL_IMAGE_WIDTH);
    this->dstHeight = ilGetInteger(IL_IMAGE_HEIGHT);
    this->dstNumChannels = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);

    // save the image as TGA
    res = ilSave(IL_TGA, (char*) absDstPath.LocalPath().AsCharPtr());
    if (IL_TRUE != res)
    {
        ilDeleteImages(1, &image);
        return ErrorWriteDstFile;
    }

    // everything ok
    ilDeleteImages(1, &image);
    return Success;
}

} // namespace ToolkitUtil