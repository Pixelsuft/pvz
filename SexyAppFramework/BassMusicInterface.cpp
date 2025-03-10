#include "BassMusicInterface.h"
#include "BassLoader.h"
#include "..\PakLib\PakInterface.h"

using namespace Sexy;

#define BASS2_MUSIC_RAMP			BASS_MUSIC_RAMP	// normal ramping

#define BASS_CONFIG_BUFFER			0

BassMusicInfo::BassMusicInfo()
{	
	mVolume = 0.0;
	mVolumeAdd = 0.0;
	mVolumeCap = 1.0;
	mStopOnFade = false;
	mHMusic = NULL;
	mHStream = NULL;
}

BassMusicInterface::BassMusicInterface(HWND theHWnd)
{
	LoadBassDLL();

	MIXERCONTROLDETAILS mcd;
	MIXERCONTROLDETAILS_UNSIGNED mxcd_u;
	MIXERLINECONTROLS mxlc;
	MIXERCONTROL mlct;
	MIXERLINE mixerLine;
	HMIXEROBJ phmx;
	MIXERCAPS pmxcaps;	

	mixerOpen((HMIXER*) &phmx, 0, 0, 0, MIXER_OBJECTF_MIXER);
	mixerGetDevCaps(0, &pmxcaps, sizeof(pmxcaps));

	mxlc.cbStruct = sizeof(mxlc);	
	mxlc.cbmxctrl = sizeof(mlct);
	mxlc.pamxctrl = &mlct;
	mxlc.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
	mixerLine.cbStruct = sizeof(mixerLine);
	mixerLine.dwComponentType = MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT;
	mixerGetLineInfo(phmx, &mixerLine, MIXER_GETLINEINFOF_COMPONENTTYPE);
	mxlc.dwLineID = mixerLine.dwLineID;
	mixerGetLineControls(phmx, &mxlc, MIXER_GETLINECONTROLSF_ONEBYTYPE);	

	mcd.cbStruct = sizeof(mcd);
	mcd.dwControlID = mlct.dwControlID;
	mcd.cChannels = 1;
	mcd.cMultipleItems = 0;
	mcd.cbDetails = sizeof(mxcd_u);
	mcd.paDetails = &mxcd_u;
	
	mixerGetControlDetails(phmx, &mcd, 0L);

#if 0
	BOOL success;
	if (gBass->mVersion2)
	{
		success = gBass->BASS_Init2(1, 44100, 0, theHWnd, NULL);
		gBass->BASS_SetConfig(BASS_CONFIG_BUFFER, 2000);
	}
	else
		success = gBass->BASS_Init(-1, 44100, 0, theHWnd);
#endif

	mixerSetControlDetails(phmx, &mcd, 0L);

	// gBass->BASS_Start();

	mixerClose((HMIXER) phmx);

	mMaxMusicVolume = 40;

	// mMusicLoadFlags = gBass->mVersion2 ? BASS_MUSIC_LOOP | BASS2_MUSIC_RAMP : BASS_MUSIC_LOOP;
}

BassMusicInterface::~BassMusicInterface()
{
	// gBass->BASS_Stop();
	// gBass->BASS_Free();

	FreeBassDLL();
}

bool BassMusicInterface::LoadMusic(int theSongId, const std::string& theFileName)
{
	return false;
}

void BassMusicInterface::PlayMusic(int theSongId, int theOffset, bool noLoop)
{
	return;
}

void BassMusicInterface::StopMusic(int theSongId)
{
	return;
}

void BassMusicInterface::StopAllMusic()
{
	return;
}

void BassMusicInterface::UnloadMusic(int theSongId)
{	
	BassMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{
		mMusicMap.erase(anItr);
	}
}

void BassMusicInterface::UnloadAllMusic()
{
	mMusicMap.clear();
}

void BassMusicInterface::PauseMusic(int theSongId)
{
	return;
}

void BassMusicInterface::PauseAllMusic()
{
	return;
}

void BassMusicInterface::ResumeAllMusic()
{
	return;
}

void BassMusicInterface::ResumeMusic(int theSongId)
{
	return;
}

void BassMusicInterface::FadeIn(int theSongId, int theOffset, double theSpeed, bool noLoop)
{
	return;
}

void BassMusicInterface::FadeOut(int theSongId, bool stopSong, double theSpeed)
{
	return;
}

void BassMusicInterface::FadeOutAll(bool stopSong, double theSpeed)
{
	return;
}

void BassMusicInterface::SetVolume(double theVolume)
{
	return;
}

void BassMusicInterface::SetSongVolume(int theSongId, double theVolume)
{
	return;
}

void BassMusicInterface::SetSongMaxVolume(int theSongId, double theMaxVolume)
{
	return;
}

bool BassMusicInterface::IsPlaying(int theSongId)
{
	return false;
}

void BassMusicInterface::SetMusicAmplify(int theSongId, double theAmp)
{
	return;
}

void BassMusicInterface::Update()
{
	return;
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
// MODs are broken up into several orders or patterns. This returns the current order a song is on.
int BassMusicInterface::GetMusicOrder(int theSongId)
{
	return 0;
}