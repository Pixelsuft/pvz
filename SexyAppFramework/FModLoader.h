#ifndef __FMODLOADER_H__
#define __FMODLOADER_H__

#include <stdio.h>
#include <SDL3/SDL.h>
#include "fmod.h"
#define F_API SDLCALL

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
namespace Sexy
{

struct FMOD_INSTANCE
{
	FMOD_INSTANCE(const char *dllName);
	virtual ~FMOD_INSTANCE();

    SDL_SharedObject* mModule;
	FMOD_RESULT(F_API* FMOD_System_Create)(FMOD_SYSTEM**, unsigned int);
	FMOD_RESULT(F_API* FMOD_System_Release)(FMOD_SYSTEM*);
	FMOD_RESULT(F_API* FMOD_System_Init)(FMOD_SYSTEM*, int, FMOD_INITFLAGS, void*);
	FMOD_RESULT(F_API* FMOD_System_Close)(FMOD_SYSTEM*);
	FMOD_RESULT(F_API* FMOD_System_Update)(FMOD_SYSTEM*);
	FMOD_RESULT(F_API* FMOD_System_GetVersion)(FMOD_SYSTEM*, unsigned int*);
	FMOD_RESULT(F_API* FMOD_System_CreateSound)(FMOD_SYSTEM*, const char*, FMOD_MODE, FMOD_CREATESOUNDEXINFO*, FMOD_SOUND**);
	FMOD_RESULT(F_API* FMOD_System_CreateStream)(FMOD_SYSTEM*, const char*, FMOD_MODE, FMOD_CREATESOUNDEXINFO*, FMOD_SOUND**);
	FMOD_RESULT(F_API* FMOD_System_PlaySound)(FMOD_SYSTEM*, FMOD_SOUND*, FMOD_CHANNELGROUP*, FMOD_BOOL, FMOD_CHANNEL**);
	FMOD_RESULT(F_API* FMOD_System_GetChannel)(FMOD_SYSTEM*, int, FMOD_CHANNEL**);
	FMOD_RESULT(F_API* FMOD_Sound_Release)(FMOD_SOUND*);
	FMOD_RESULT(F_API* FMOD_Sound_GetLength)(FMOD_SOUND*, unsigned int*, FMOD_TIMEUNIT);
	FMOD_RESULT(F_API* FMOD_Sound_SetMode)(FMOD_SOUND*, FMOD_MODE);
	FMOD_RESULT(F_API* FMOD_Sound_GetMode)(FMOD_SOUND*, FMOD_MODE*);
	FMOD_RESULT(F_API* FMOD_Sound_SetLoopCount)(FMOD_SOUND*, int);
	FMOD_RESULT(F_API* FMOD_Sound_GetLoopCount)(FMOD_SOUND*, int*);
	FMOD_RESULT(F_API* FMOD_Channel_Stop)(FMOD_CHANNEL*);
	FMOD_RESULT(F_API* FMOD_Channel_SetPaused)(FMOD_CHANNEL*, FMOD_BOOL);
	FMOD_RESULT(F_API* FMOD_Channel_GetPaused)(FMOD_CHANNEL*, FMOD_BOOL*);
	FMOD_RESULT(F_API* FMOD_Channel_SetVolume)(FMOD_CHANNEL*, float);
	FMOD_RESULT(F_API* FMOD_Channel_GetVolume)(FMOD_CHANNEL*, float*);
	FMOD_RESULT(F_API* FMOD_Channel_SetPitch)(FMOD_CHANNEL*, float);
	FMOD_RESULT(F_API* FMOD_Channel_GetPitch)(FMOD_CHANNEL*, float*);
	FMOD_RESULT(F_API* FMOD_Channel_SetMode)(FMOD_CHANNEL*, FMOD_MODE);
	FMOD_RESULT(F_API* FMOD_Channel_GetMode)(FMOD_CHANNEL*, FMOD_MODE*);
	FMOD_RESULT(F_API* FMOD_Channel_SetUserData)(FMOD_CHANNEL*, void*);
	FMOD_RESULT(F_API* FMOD_Channel_GetUserData)(FMOD_CHANNEL*, void**);
	FMOD_RESULT(F_API* FMOD_Channel_GetCurrentSound)(FMOD_CHANNEL*, FMOD_SOUND**);
	FMOD_RESULT(F_API* FMOD_Channel_SetPosition)(FMOD_CHANNEL*, unsigned int, FMOD_TIMEUNIT);
	FMOD_RESULT(F_API* FMOD_Channel_GetPosition)(FMOD_CHANNEL*, unsigned int*, FMOD_TIMEUNIT);
	FMOD_RESULT(F_API* FMOD_Channel_SetCallback)(FMOD_CHANNEL*, FMOD_CHANNELCONTROL_CALLBACK);
	FMOD_RESULT(F_API* FMOD_Sound_SetMusicChannelVolume)(FMOD_SOUND*, int, float);
	FMOD_RESULT(F_API* FMOD_Channel_IsPlaying)(FMOD_CHANNEL*, FMOD_BOOL*);
} ;


extern FMOD_INSTANCE *gFMod;

void LoadFModDLL(); // exits on failure
void FreeFModDLL();

} // namespace Sexy

#endif