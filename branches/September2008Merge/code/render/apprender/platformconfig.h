#pragma once
#ifndef APPRENDER_PLATFORMCONFIG_H
#define APPRENDER_PLATFORMCONFIG_H

#if __WIN32__ || __XBOX360__
#define PLACEHOLDER_TEXTURENAME "tex:system/placeholder.dds"
#define PLACEHOLDER_MESHNAME  "msh:system/placeholder_s_0.nvx2"
#define DEFAULT_FRAMESHADER_NAME "DX9Default"
#elif __WII__
#define PLACEHOLDER_TEXTURENAME "tex:system/placeholder.tpl"
#define PLACEHOLDER_MESHNAME  "msh:system/placeholder_s_0.nwdl"
#define DEFAULT_FRAMESHADER_NAME "WiiDefault"
#endif

#endif