//------------------------------------------------------------------------------
//  uixmlparser.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "uifeature/uixmlparser.h"
#include "uifeature/uifactory.h"
#include "uifeature/elements/button.h"
#include "uifeature/elements/canvas.h"
#include "uifeature/elements/label.h"
#include "io/xmlreader.h"
#include "io/filestream.h"

namespace UI
{
__ImplementClass(UIXmlParser, 'UIPA', Core::RefCounted);

using namespace Util;

//------------------------------------------------------------------------------
/**
*/
UIXmlParser::UIXmlParser() : fileName(""),
							 windowCount(-1)
{
    // get factory
	this->factory = UIFactory::Instance();
}

//------------------------------------------------------------------------------
/**
*/
UIXmlParser::~UIXmlParser()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
const Util::Array<Ptr<Window>>&
UIXmlParser::ParseFile()
{
	// first open the file
	Ptr<IO::FileStream> fileStream = IO::FileStream::Create();
	fileStream->SetURI(IO::URI("export:ui/" + this->fileName));
	fileStream->Open();
	n_assert(fileStream->IsOpen());

	// open xml 
	Ptr<IO::XmlReader> xml = IO::XmlReader::Create();
	xml->SetStream(fileStream.cast<IO::Stream>());
	xml->Open();
	n_assert(xml->IsOpen());

	// check root node
	String nodeName = xml->GetCurrentNodeName();
	n_assert("Nebula3" == nodeName);

	// get next node.. has to be a window node
	if(xml->SetToFirstChild("Window"))
	{
		do
		{
			this->ParseWindowNode(xml);
		}while(xml->SetToNextChild());
	}

	return this->windows;
}

//------------------------------------------------------------------------------
/**
*/
void
UIXmlParser::ParseWindowNode(Ptr<IO::XmlReader>& xml)
{
	n_assert(xml->GetCurrentNodeName() == "Window");

	// parse node
	Ptr<Window> newWindow = Window::Create();
	String xalign = xml->GetString("xAlign");
	String yalign = xml->GetString("yAlign");
	newWindow->SetScreenAlignmentX(Window::StringToAlignmentX(xalign));
	newWindow->SetScreenAlignmentY(Window::StringToAlignmentY(yalign));
	this->windows.Append(newWindow);
	this->windowCount++;
	
	// now parse the elements in the window node
	// first has to be a canvas
	xml->SetToFirstChild();
	this->ParseCanvasNode(xml, newWindow);
	xml->SetToParent();
}

//------------------------------------------------------------------------------
/**
*/
void
UIXmlParser::ParseCanvasNode(Ptr<IO::XmlReader>& xml, Ptr<Window>& parent)
{
	n_assert(xml->GetCurrentNodeName() == "Canvas");

	// parse node
	Ptr<Canvas> newCanvas = factory->CreateElement("Canvas").cast<Canvas>();
	newCanvas->SetId(xml->GetString("id"));
	newCanvas->SetColor(xml->GetFloat4("color"));
	newCanvas->SetRelativePixelPosition(xml->GetFloat2("position"));	
	newCanvas->SetPixelSize(xml->GetFloat2("size"));
	newCanvas->SetParentWindow(parent);
	
	// set window canvas
	parent->SetCanvas(newCanvas);
	
	// call directly OnCreate() cause this is called in AttachElement, but
	// a canvas will never be attached like other elements
	newCanvas->OnCreate();

	Ptr<Element> el = newCanvas.cast<Element>();

	// parse childs recursivly, if there is one
	if(xml->SetToFirstChild())
	{		
		do
		{
			this->ParseChildElements(xml, el);
		}while(xml->SetToNextChild());
	}
}

//------------------------------------------------------------------------------
/**
*/
void
UIXmlParser::ParseChildElements(Ptr<IO::XmlReader>& xml, Ptr<Element>& parent)
{
	// set to 
	String nodeType = xml->GetCurrentNodeName();

	if		("Button" == nodeType)			this->ParseButtonNode(xml, parent);
	else if	("Label" == nodeType)			this->ParseLabelNode(xml, parent);

	// check if there are more
	if(xml->SetToFirstChild())
	{
		do
		{
			this->ParseChildElements(xml, parent);
		}
		while(xml->SetToNextChild());
	}
}

//------------------------------------------------------------------------------
/**
*/
void
UIXmlParser::ParseButtonNode(Ptr<IO::XmlReader>& xml, Ptr<Element>& parent)
{
	n_assert(xml->GetCurrentNodeName() == "Button");

	// parse node
	Ptr<Button> newButton = factory->CreateElement("Button").cast<Button>();
	newButton->SetId(xml->GetString("id"));
	newButton->SetRelativePixelPosition(xml->GetFloat2("position"));
	newButton->SetPixelSize(xml->GetFloat2("size"));
	newButton->SetVisible(xml->GetBool("visible"));
	newButton->SetEnabled(xml->GetBool("enabled"));
	newButton->SetColor(xml->GetFloat4("color"));
	newButton->SetParentElement(parent);
	newButton->SetParentWindow(this->windows[this->windowCount]);
	// and append
	Ptr<Element> el = newButton.cast<Element>();
	parent->AttachElement(el);
	
	// check for further childs
	if(xml->SetToFirstChild())
	{
		do
		{
			this->ParseChildElements(xml, el);
		}while(xml->SetToNextChild());
	}
}

//------------------------------------------------------------------------------
/**
*/
void
UIXmlParser::ParseLabelNode(Ptr<IO::XmlReader>& xml, Ptr<Element>& parent)
{
	n_assert(xml->GetCurrentNodeName() == "Label");

	// parse node
	Ptr<Label> newLabel = factory->CreateElement("Label").cast<Label>();
	newLabel->SetId(xml->GetString("id"));
	newLabel->SetRelativePixelPosition(xml->GetFloat2("position"));
	newLabel->SetPixelSize(xml->GetFloat2("size"));
	newLabel->SetVisible(xml->GetBool("visible"));
	newLabel->SetEnabled(xml->GetBool("enabled"));
	newLabel->SetColor(xml->GetFloat4("color"));
	newLabel->SetParentElement(parent);
	newLabel->SetParentWindow(this->windows[this->windowCount]);
	// and append
	Ptr<Element> el = newLabel.cast<Element>();
	parent->AttachElement(el);

	// check for further childs
	if(xml->SetToFirstChild())
	{
		do
		{
			this->ParseChildElements(xml, el);
		}while(xml->SetToNextChild());
	}
}


} // namespace UI
