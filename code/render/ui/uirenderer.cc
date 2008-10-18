//------------------------------------------------------------------------------
//  uirenderer.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "ui/uirenderer.h"
#include "ui/uirenderprotocol.h"
#include "graphics/graphicsinterface.h"
#include "threading/thread.h"

namespace UI
{
__ImplementClass(UIRenderer, 'UIRR', Core::RefCounted);
__ImplementSingleton(UIRenderer);

using namespace Util;
using namespace Math;
using namespace Threading;

//------------------------------------------------------------------------------
/**
*/
UIRenderer::UIRenderer()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
UIRenderer::~UIRenderer()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
UIRenderer::DrawElement(const Math::rectangle<float>& rect, const Math::float4& color)
{
	Ptr<UIElement> el = UIElement::Create();
	el->SetColor(color);
	el->SetDimension(rect);	
	this->elements.Append(el);
}

//------------------------------------------------------------------------------
/**    
*/
void
UIRenderer::OnFrame()
{
    // send a RenderUIElements message to the render thread, send this
    // every frame, whether there are any ui elements or not as this will
    // also cleanup the ui elements from the previous frame in the
    // render thread
	if(this->elements.Size() > 0)
	{
		Ptr<CoreUI::RenderUIElements> msg = CoreUI::RenderUIElements::Create();
		msg->SetThreadId(Thread::GetMyThreadId());
		msg->SetUIElements(this->elements);
		Graphics::GraphicsInterface::Instance()->Send(msg.cast<Messaging::Message>());
		this->elements.Clear();
	}
}

} // namespace Debug
