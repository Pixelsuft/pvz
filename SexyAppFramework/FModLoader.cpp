#include "FModLoader.h"
#include <stdlib.h>

using namespace Sexy;

FMOD_INSTANCE* Sexy::gFMod = NULL;
static long gFModLoadCount = 0;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void CheckFModFunction(size_t theFunc, const char *theName)
{
	if (theFunc==0)
	{
		char aBuf[1024];
		sprintf(aBuf,"%s function not found in fmod.dll",theName);
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", aBuf, NULL);
		exit(0);
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
FMOD_INSTANCE::FMOD_INSTANCE(const char *dllName)
{
    mModule = SDL_LoadObject("fmod.dll");
    if (!mModule)
		return;

    #define GETPROC(_x, _y)                                                                       \
    {                                                                                             \
		CheckFModFunction(*((size_t *)&_x) = (size_t)SDL_LoadFunction(mModule, _y),#_y);    \
    }

	GETPROC(FMOD_System_Create, "FMOD_System_Create");
	GETPROC(FMOD_System_Release, "FMOD_System_Release");
	GETPROC(FMOD_System_Init, "FMOD_System_Init");
	GETPROC(FMOD_System_Close, "FMOD_System_Close");
	GETPROC(FMOD_System_Update, "FMOD_System_Update");
	GETPROC(FMOD_System_GetVersion, "FMOD_System_GetVersion");
	GETPROC(FMOD_System_CreateSound, "FMOD_System_CreateSound");
	GETPROC(FMOD_System_CreateStream, "FMOD_System_CreateStream");
	GETPROC(FMOD_System_PlaySound, "FMOD_System_PlaySound");
	GETPROC(FMOD_System_GetChannel, "FMOD_System_GetChannel");
	GETPROC(FMOD_Sound_Release, "FMOD_Sound_Release");
	GETPROC(FMOD_Sound_GetLength, "FMOD_Sound_GetLength");
	GETPROC(FMOD_Sound_SetMode, "FMOD_Sound_SetMode");
	GETPROC(FMOD_Sound_GetMode, "FMOD_Sound_GetMode");
	GETPROC(FMOD_Sound_SetLoopCount, "FMOD_Sound_SetLoopCount");
	GETPROC(FMOD_Sound_GetLoopCount, "FMOD_Sound_GetLoopCount");
	GETPROC(FMOD_Channel_Stop, "FMOD_Channel_Stop");
	GETPROC(FMOD_Channel_SetPaused, "FMOD_Channel_SetPaused");
	GETPROC(FMOD_Channel_GetPaused, "FMOD_Channel_GetPaused");
	GETPROC(FMOD_Channel_SetVolume, "FMOD_Channel_SetVolume");
	GETPROC(FMOD_Channel_GetVolume, "FMOD_Channel_GetVolume");
	GETPROC(FMOD_Channel_SetPitch, "FMOD_Channel_SetPitch");
	GETPROC(FMOD_Channel_GetPitch, "FMOD_Channel_GetPitch");
	GETPROC(FMOD_Channel_SetMode, "FMOD_Channel_SetMode");
	GETPROC(FMOD_Channel_GetMode, "FMOD_Channel_GetMode");
	GETPROC(FMOD_Channel_SetCallback, "FMOD_Channel_SetCallback");
	GETPROC(FMOD_Channel_SetUserData, "FMOD_Channel_SetUserData");
	GETPROC(FMOD_Channel_GetUserData, "FMOD_Channel_GetUserData");
	GETPROC(FMOD_Channel_GetCurrentSound, "FMOD_Channel_GetCurrentSound");
	GETPROC(FMOD_Channel_SetPosition, "FMOD_Channel_SetPosition");
	GETPROC(FMOD_Channel_GetPosition, "FMOD_Channel_GetPosition");
	GETPROC(FMOD_Sound_SetMusicChannelVolume, "FMOD_Sound_SetMusicChannelVolume");
	GETPROC(FMOD_Channel_IsPlaying, "FMOD_Channel_IsPlaying");

#undef GETPROC
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
FMOD_INSTANCE::~FMOD_INSTANCE()
{
	if (mModule) {
		SDL_UnloadObject(mModule);
		mModule = NULL;
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void Sexy::LoadFModDLL()
{
	InterlockedIncrement(&gFModLoadCount);
	if (gFMod!=NULL)
		return;

	gFMod = new FMOD_INSTANCE("fmod.dll");
	if (gFMod->mModule==NULL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", "Can't find fmod.dll.", NULL);
		exit(0);
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void Sexy::FreeFModDLL()
{
	if (gFMod!=NULL)
	{
		if (InterlockedDecrement(&gFModLoadCount) <= 0)
		{
			delete gFMod;
			gFMod = NULL;
		}
	}
}


