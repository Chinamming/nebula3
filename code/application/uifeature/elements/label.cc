//------------------------------------------------------------------------------
//  uifeature/elements/label.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "uifeature/elements/label.h"
#include "uifeature/elements/window.h"
#include "uifeature/uiserver.h"

namespace UI
{
__ImplementClass(Label, 'UILA', Element);

//------------------------------------------------------------------------------
/**
*/
Label::Label() 
    //blinking (false),    
    //cycleTime(0.6f)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
Label::~Label()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    This method is called when the element has been attached to a parent.
*/
void
Label::OnCreate()
{
    // call parent class
    Element::OnCreate();

    // get initial texture path
    //this->textureName = this->RecurseGetTexture(this->GetGfxNode());
    //this->color = this->RecurseGetColor(this->GetGfxNode());
    //this->emissiveColor= this->RecurseGetEmissiveColor(this->GetGfxNode());
    //n_assert(this->textureName.IsValid());
}

//------------------------------------------------------------------------------
/**
    Called before rendering happens. This updates the visibility of our
    Nebula2 visual nodes.
*/
void
Label::OnFrame()
{
    Element::OnFrame();

  //  if (!(this->GetParentWindow() /*&& this->GetParentWindow()->IsFadingEnabled() && this->GetParentWindow()->IsWithinFade()*/)
  //      && this->blinking && this->IsVisible())        
  //  {
		//Math::float4 bColor = this->emissiveColor;
  //      Math::float4 fadeColor = this->color;
		//Timing::Time currentTime = UIServer::Instance()->GetTime();
  //     
  //      if ((this->startTime + (this->cycleTime/2.0)) > currentTime) // fadein
  //      {
  //          bColor.x= (float)((currentTime - this->startTime)/(this->cycleTime/2.0));
  //          bColor.y= bColor.x;
  //          bColor.z= bColor.y;
  //          fadeColor.w= 0.5f;
  //          fadeColor.w+= (float)((currentTime - this->startTime)/(this->cycleTime/2.0));
  //      }       
  //      else if ((this->startTime + this->cycleTime) > currentTime) // fadeout
  //      {
  //          bColor.x= 1.f - (float)((currentTime - (this->startTime + (this->cycleTime/2.0)))/(this->cycleTime/2.0));
  //          bColor.y= bColor.x;
  //          bColor.z= bColor.y;
  //          fadeColor.w= 1.f - (float) ((currentTime - (this->startTime + (this->cycleTime/2.0)))/(this->cycleTime/2.0));
  //          fadeColor.w= n_max(fadeColor.w, 0.5f);
  //      }
  //      else
  //      {
  //          this->startTime = currentTime;

  //          bColor.x= 0.f;
  //          bColor.y= 0.f;
  //          bColor.z= 0.f;
  //          fadeColor.w = 0.5f;
  //      }
  //      this->SetEmissiveColor(bColor);
  //      this->SetColor(fadeColor);
  //  }
}

//------------------------------------------------------------------------------
/**
    This sets a new texture on all underlying Nebula2 scene nodes.
*/
void
Label::SetTexture(const Util::String& texName)
{
    //if (this->gfxNode.isvalid())
    //{
    //    Util::String tName;

    //    //set default texture if textureName isn't valid
    //    if (texName.IsEmpty() || texName == " ")
    //    {
    //        tName = "textures:system/white.dds";
    //    }
    //    else
    //    {
    //        tName = texName;
    //    }
    //    if (tName != this->textureName)
    //    {
    //        this->RecurseSetTexture(this->gfxNode, tName);
    //        this->textureName = tName;
    //        this->SetDirty();
    //    }
    //}
    //else
    //{
    //    n_printf("Texture could not be set, because gfxNode has already been released!\nThis might happen if the gfxEntity has already been released (i.e. if this function is called during the OnDestroy function)");
    //}
	this->textureName = texName;
}

//------------------------------------------------------------------------------
/**
    This sets a new color on all underlying Nebula2 scene nodes.
*/
void
Label::SetColor(const Math::float4& c)
{
    //if (this->gfxNode.isvalid())
    //{
    //    if (this->color != c)
    //    {
    //        this->RecurseSetColor(this->gfxNode, c);
    //        this->color = c;
    //        this->SetDirty();
    //    }
    //}
    //else
    //{
    //    n_printf("Color could not be set, because gfxNode has already been released!\nThis might happen if the gfxEntity has already been released (i.e. if this function is called during the OnDestroy function)\n");
    //}
	this->color = c;
}

//------------------------------------------------------------------------------
/**
    This sets a new emissiveColor on all underlying Nebula2 scene nodes.
*/
void
Label::SetEmissiveColor(const Math::float4& e)
{
    //if (this->gfxNode.isvalid())
    //{
    //    if (this->emissiveColor != e)
    //    {
    //        this->RecurseSetEmissiveColor(this->gfxNode, e);
    //        this->emissiveColor = e;
    //        this->SetDirty();
    //    }
    //}
    //else
    //{
    //    n_printf("EmissiveColor could not be set, because gfxNode has already been released!\nThis might happen if the gfxEntity has already been released (i.e. if this function is called during the OnDestroy function)\n");
    //}
	this->emissiveColor = e;
}

//------------------------------------------------------------------------------
/**
    Internal method to recursively set a new texture on underlying
    Nebula2 resource objects.    
*/
//void
//Label::RecurseSetTexture(nRoot* node, const Util::String& texName)
//{
//    if (node->IsA(this->abstractShaderNodeClass) && ((nAbstractShaderNode*)node)->IsActive())
//    {
//        nAbstractShaderNode* shaderNode = (nAbstractShaderNode*) node;
//        if (shaderNode->GetTexture(nShaderState::DiffMap0) != texName)
//        {
//            shaderNode->SetTexture(nShaderState::DiffMap0, texName.AsCharPtr());
//        }
//    }
//    nRoot* child;
//    for (child = node->GetHead(); child != 0; child = child->GetSucc())
//    {
//        this->RecurseSetTexture(child, texName);
//    }
//}

//------------------------------------------------------------------------------
/**
    Internal method to recursively set a new texture on underlying
    Nebula2 resource objects.

    FIXME: returns only the texture of the first active abstract shader node.
    TODO: return the texture for a specific state.
*/
//nString
//Label::RecurseGetTexture(nRoot* node)
//{
//    if (node->IsA(this->abstractShaderNodeClass) && ((nAbstractShaderNode*)node)->IsActive())
//    {
//        return ((nAbstractShaderNode*)node)->GetTexture(nShaderState::DiffMap0);
//    }
//
//    nRoot* child;
//    for (child = node->GetHead(); child != 0; child = child->GetSucc())
//    {
//        Util::String result = this->RecurseGetTexture(child);
//
//        if(result.IsValid())
//            return result;
//    }
//
//    return "";
//}

//------------------------------------------------------------------------------
/**
    Internal method to recursively get the color of the underlying
    Nebula2 resource objects.    
*/
//Math::float4
//Label::RecurseGetColor(nRoot* node)
//{
//    if (node->IsA(this->abstractShaderNodeClass) && ((nAbstractShaderNode*)node)->IsActive())
//    {
//        return ((nAbstractShaderNode*)node)->GetVector(nShaderState::MatDiffuse);
//    }
//
//    nRoot* child;
//    for (child = node->GetHead(); child != 0; child = child->GetSucc())
//    {
//        Math::float4 result = this->RecurseGetColor(child);
//
//        if(result != Math::float4(-1.f, -1.f, -1.f, -1.f))
//            return result;
//    }
//
//    return Math::float4(-1.f, -1.f, -1.f, -1.f);
//}

//------------------------------------------------------------------------------
/**
*/
//void
//Label::RecurseSetColor(nRoot* node, const Math::float4& c)
//{
//    if (node->IsA(this->abstractShaderNodeClass))
//    {
//        ((nAbstractShaderNode*)node)->SetVector(nShaderState::MatDiffuse, c);
//    }
//    nRoot* child;
//    for (child = node->GetHead(); child != 0; child = child->GetSucc())
//    {
//        this->RecurseSetColor(child, c);
//    }
//}

//------------------------------------------------------------------------------
/**   
*/
//Math::float4
//Label::RecurseGetEmissiveColor(nRoot* node)
//{
//    if (node->IsA(this->abstractShaderNodeClass) && ((nAbstractShaderNode*)node)->IsActive())
//    {
//        return ((nAbstractShaderNode*)node)->GetVector(nShaderState::MatEmissive);
//    }
//
//    nRoot* child;
//    for (child = node->GetHead(); child != 0; child = child->GetSucc())
//    {
//        Math::float4 result = this->RecurseGetEmissiveColor(child);
//
//        if(result != Math::float4(-1.f, -1.f, -1.f, -1.f))
//            return result;
//    }
//
//    return Math::float4(-1.f, -1.f, -1.f, -1.f);
//}

//------------------------------------------------------------------------------
/**
*/
//void
//Label::RecurseSetEmissiveColor(nRoot* node, const Math::float4& e)
//{
//    if (node->IsA(this->abstractShaderNodeClass))
//    {
//        ((nAbstractShaderNode*)node)->SetVector(nShaderState::MatEmissive, e);
//    }
//    nRoot* child;
//    for (child = node->GetHead(); child != 0; child = child->GetSucc())
//    {
//        this->RecurseSetEmissiveColor(child, e);
//    }
//}

//------------------------------------------------------------------------------
/**
*/
//void
//Label::SetBlinking(bool b)
//{
//    if (this->blinking != b)
//    {
//        Math::float4 originalColor = this->GetColor();
//
//        if (!b)
//        {
//            this->SetEmissiveColor(Math::float4(0.f, 0.f, 0.f, 1.f));            
//            this->SetColor(Math::float4(originalColor.x, originalColor.y, originalColor.z, 1.f));
//        }
//        else
//        {
//            this->emissiveColor = Math::float4(0.f, 0.f, 0.f, 1.f);
//            this->color = Math::float4(originalColor.x, originalColor.y, originalColor.z, 0.3f);
//            this->startTime = Server::Instance()->GetTime();
//        }
//        this->blinking = b;
//        this->SetDirty();
//    }
//}

//------------------------------------------------------------------------------
/**
*/
//void
//Label::CloneAttributes(Element* target)
//{
//    Control::CloneAttributes(target);
//
//    n_assert(target->IsA(UI::Label::RTTI));
//
//    Label* label = (Label*) target;
//    label->SetTexture(this->textureName);
//    label->SetBlinking(this->blinking);    
//}

}; // namespace UI
