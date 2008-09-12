#pragma once
#ifndef HTTP_SVGLINECHARTWRITER_H
#define HTTP_SVGLINECHARTWRITER_H
//------------------------------------------------------------------------------
/**
    @class Http::SvgLineChartWriter
    
    Specialized SVG page writer which draws line chart diagrams.
    
    (C) 2008 Radon Labs GmbH
*/
#include "http/svg/svgpagewriter.h"

//------------------------------------------------------------------------------
namespace Http
{
class SvgLineChartWriter : public SvgPageWriter
{
    DeclareClass(SvgLineChartWriter);
public:
    /// constructor
    SvgLineChartWriter();
    /// destructor
    virtual ~SvgLineChartWriter();
    
    /// begin writing to the stream
    virtual bool Open();
    /// setup the x axis
    void SetupXAxis(const Util::String& axisName, const Util::String& unitName, int minVal, int maxVal);
    /// setup the y axis
    void SetupYAxis(const Util::String& axisName, const Util::String& unitName, float minVal, float maxVal);
    /// add a value track
    void AddTrack(const Util::String& name, const Util::String& color, const Util::Array<float>& values);
    /// draw diagram
    void Draw();

private:
    struct Track
    {
        Util::String name;
        Util::String color;
        Util::Array<float> values;
    };

    Util::String xAxisName;
    Util::String xAxisUnitName;
    int xAxisMinVal;
    int xAxisMaxVal;
    Util::String yAxisName;
    Util::String yAxisUnitName;
    float yAxisMinVal;
    float yAxisMaxVal;
    Util::Array<Track> tracks;
};

} // namespace Http
//------------------------------------------------------------------------------
#endif
    