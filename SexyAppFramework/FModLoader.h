#ifndef __FMODLOADER_H__
#define __FMODLOADER_H__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>

#include "fmod.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
namespace Sexy
{

struct FMOD_INSTANCE
{
	FMOD_INSTANCE(const char *dllName);
	virtual ~FMOD_INSTANCE();

    HMODULE         mModule;

	signed char     (F_API *FSOUND_SetBufferSize)(int len_ms);
    signed char     (F_API *FSOUND_SetHWND)(void *hwnd);
    signed char     (F_API *FSOUND_Init)(int mixrate, int maxsoftwarechannels, unsigned int flags);
    void            (F_API *FSOUND_Close)();

	FSOUND_SAMPLE * (F_API *FSOUND_Sample_Load)(int index, const char *name_or_data, unsigned int mode, int memlength);
    FSOUND_SAMPLE * (F_API *FSOUND_Sample_Alloc)(int index, int length, unsigned int mode, int deffreq, int defvol, int defpan, int defpri);
    void            (F_API *FSOUND_Sample_Free)(FSOUND_SAMPLE *sptr);
    signed char     (F_API *FSOUND_Sample_Lock)(FSOUND_SAMPLE *sptr, int offset, int length, void **ptr1, void **ptr2, unsigned int *len1, unsigned int *len2);
    signed char     (F_API *FSOUND_Sample_Unlock)(FSOUND_SAMPLE *sptr, void *ptr1, void *ptr2, unsigned int len1, unsigned int len2);
    signed char     (F_API *FSOUND_Sample_SetLoopPoints)(FSOUND_SAMPLE *sptr, int loopstart, int loopend);
    unsigned int    (F_API *FSOUND_Sample_GetLength)(FSOUND_SAMPLE *sptr);
    signed char     (F_API *FSOUND_Sample_GetDefaults)(FSOUND_SAMPLE *sptr, int *deffreq, int *defvol, int *defpan, int *defpri);
    unsigned int    (F_API *FSOUND_Sample_GetMode)(FSOUND_SAMPLE *sptr);

	int             (F_API *FSOUND_PlaySound)(int channel, FSOUND_SAMPLE *sptr);
    signed char     (F_API *FSOUND_StopSound)(int channel);
    signed char     (F_API *FSOUND_SetVolume)(int channel, int vol);
    signed char     (F_API *FSOUND_SetPaused)(int channel, signed char paused);
    signed char     (F_API *FSOUND_SetLoopMode)(int channel, unsigned int loopmode);
    signed char     (F_API *FSOUND_IsPlaying)(int channel);

    FSOUND_STREAM * (F_API *FSOUND_Stream_OpenFile)(const char *filename, unsigned int mode, int memlength);
	signed char     (F_API *FSOUND_Stream_Close)(FSOUND_STREAM *stream);
    int             (F_API *FSOUND_Stream_Play)(int channel, FSOUND_STREAM *stream);

    FMUSIC_MODULE * (F_API *FMUSIC_LoadSong)(const char *name);
    signed char     (F_API *FMUSIC_PlaySong)(FMUSIC_MODULE *mod);
    signed char     (F_API *FMUSIC_StopSong)(FMUSIC_MODULE *mod);
    signed char     (F_API *FMUSIC_SetLooping)(FMUSIC_MODULE *mod, signed char looping);
    signed char     (F_API *FMUSIC_SetOrder)(FMUSIC_MODULE *mod, int order);
    signed char     (F_API *FMUSIC_SetPaused)(FMUSIC_MODULE *mod, signed char pause);
    signed char     (F_API *FMUSIC_SetMasterVolume)(FMUSIC_MODULE *mod, int volume);
    signed char     (F_API *FMUSIC_IsPlaying)(FMUSIC_MODULE *mod);
} ;


extern FMOD_INSTANCE *gFMod;

void LoadFModDLL(); // exits on failure
void FreeFModDLL();

} // namespace Sexy

#endif