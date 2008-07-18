#pragma once
#ifndef APPRENDER_PLATFORMCONFIG_H
#define APPRENDER_PLATFORMCONFIG_H

#if __WIN32__ || __XBOX360__
#define PLACEHOLDER_TEXTURENAME "tex:system/placeholder.dds"
#define DEFAULT_FRAMESHADER_NAME "DX9Default"
#elif __WII__
#define PLACEHOLDER_TEXTURENAME "tex:system/placeholder.tpl"
#define DEFAULT_FRAMESHADER_NAME "WiiDefault"
#endif

#endif