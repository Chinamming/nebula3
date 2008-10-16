#pragma once
#ifndef IO_UIXMLREADER_H
#define IO_UIXMLREADER_H
//------------------------------------------------------------------------------
/**
    @class IO::UIExcelXmlReader
    
    This reader parses the XML - ui file and creates the UI tree.
	    
    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "util/string.h"
#include "util/array.h"
#include "uifeature/elements/window.h"

namespace IO
{
	class XmlReader;
};

//------------------------------------------------------------------------------
namespace UI
{
class UIFactory;
class Window;

class UIXmlParser : public Core::RefCounted
{
    __DeclareClass(UIXmlParser);
public:
    /// constructor
    UIXmlParser();
    /// destructor
    virtual ~UIXmlParser();

	/// sets the filename of the ui.xml file
	void SetFileName(const Util::String& name);
	/// gets the filename
	const Util::String& GetFileName() const;

	/// opens the file and creates the gui structure
	const Util::Array<Ptr<Window>>& ParseFile();

private:
	/// parse the window node
	void ParseWindowNode(Ptr<IO::XmlReader>& xml);
	/// parse the button node
	void ParseCanvasNode(Ptr<IO::XmlReader>& xml, Ptr<Window>& parent);

	/// parses a single element
	void ParseChildElements(Ptr<IO::XmlReader>& xml, Ptr<Element>& parent);

	/// parse the button node
	void ParseButtonNode(Ptr<IO::XmlReader>& xml, Ptr<Element>& parent);
	/// parse the label node
	void ParseLabelNode(Ptr<IO::XmlReader>& xml, Ptr<Element>& parent);

	Util::String fileName;
	Util::Array<Ptr<Window>> windows;
	Ptr<UIFactory> factory;
	int windowCount;
};

//------------------------------------------------------------------------------
/**
*/
inline
void
UIXmlParser::SetFileName(const Util::String& name)
{
	this->fileName = name;
}


//------------------------------------------------------------------------------
/**
*/
inline
const Util::String& 
UIXmlParser::GetFileName() const
{
	return this->fileName;
}

} // namespace UI
//------------------------------------------------------------------------------
#endif
