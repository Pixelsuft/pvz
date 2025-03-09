//#define SEXY_TRACING_ENABLED
//#define SEXY_PERF_ENABLED
//#define SEXY_MEMTRACE

#include "SexyAppBase.h"
#include "WidgetManager.h"
#include "Widget.h"
#include "Debug.h"
#include "KeyCodes.h"
#include "DDInterface.h"
#include "D3DInterface.h"
#include "D3DTester.h"
#include "DDImage.h"
#include "MemoryImage.h"
#include "Dialog.h"
#include "..\ImageLib\ImageLib.h"
#include "DSoundManager.h"
#include "DSoundInstance.h"
#include "Rect.h"
#include "FModMusicInterface.h"
#include "PropertiesParser.h"
#include "PerfTimer.h"
#include "MTRand.h"
#include "ModVal.h"
#include <process.h>
#include <direct.h>
#include <fstream>
#include <time.h>
#include <math.h>
#include <regstr.h>
#include "SysFont.h"
#include "ResourceManager.h"
#include "BassMusicInterface.h"
#include "AutoCrit.h"
#include "../PakLib/PakInterface.h"
#include <string>
#include <shlobj.h>
#include "../GameConstants.h"

#include "memmgr.h"

using namespace Sexy;

SexyAppBase* global_sexy_handle = NULL;
SexyAppBase* Sexy::gSexyAppBase = NULL;

HMODULE gDDrawDLL = NULL;
HMODULE gDSoundDLL = NULL;
HMODULE gVersionDLL = NULL;

//typedef struct { UINT cbSize; DWORD dwTime; } LASTINPUTINFO;
typedef BOOL(WINAPI* GetLastInputInfoFunc)(LASTINPUTINFO* plii);
GetLastInputInfoFunc gGetLastInputInfoFunc = NULL;
static bool gScreenSaverActive = false;

#ifndef SPI_GETSCREENSAVERRUNNING
#define SPI_GETSCREENSAVERRUNNING 114
#endif


//HotSpot: 11 4
//Size: 32 32
unsigned char gFingerCursorData[] = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xe7, 0xff, 0xff, 0xff, 0xc3, 0xff, 0xff, 0xff, 0xc3, 0xff, 0xff, 0xff, 0xc3,
	0xff, 0xff, 0xff, 0xc3, 0xff, 0xff, 0xff, 0xc0, 0xff, 0xff, 0xff, 0xc0, 0x1f, 0xff, 0xff,
	0xc0, 0x07, 0xff, 0xff, 0xc0, 0x03, 0xff, 0xfc, 0x40, 0x01, 0xff, 0xfc, 0x00, 0x01, 0xff,
	0xfc, 0x00, 0x01, 0xff, 0xfc, 0x00, 0x01, 0xff, 0xff, 0x00, 0x01, 0xff, 0xff, 0x00, 0x01,
	0xff, 0xff, 0x80, 0x01, 0xff, 0xff, 0x80, 0x03, 0xff, 0xff, 0xc0, 0x03, 0xff, 0xff, 0xc0,
	0x03, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18,
	0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00,
	0x18, 0x00, 0x00, 0x00, 0x1b, 0x00, 0x00, 0x00, 0x1b, 0x60, 0x00, 0x00, 0x1b, 0x68, 0x00,
	0x00, 0x1b, 0x6c, 0x00, 0x01, 0x9f, 0xec, 0x00, 0x01, 0xdf, 0xfc, 0x00, 0x00, 0xdf, 0xfc,
	0x00, 0x00, 0x5f, 0xfc, 0x00, 0x00, 0x7f, 0xfc, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x3f,
	0xf8, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00,
	0x0f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00
};

//HotSpot: 15 10
//Size: 32 32
unsigned char gDraggingCursorData[] = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xfc, 0x0f, 0xff, 0xff, 0xf0, 0x07, 0xff, 0xff, 0xe0,
	0x01, 0xff, 0xff, 0xe0, 0x00, 0xff, 0xff, 0xe0, 0x00, 0xff, 0xff, 0xe0, 0x00, 0xff, 0xff,
	0xe0, 0x00, 0xff, 0xfe, 0x60, 0x00, 0xff, 0xfc, 0x20, 0x00, 0xff, 0xfc, 0x00, 0x00, 0xff,
	0xfe, 0x00, 0x00, 0xff, 0xfe, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x80, 0x00,
	0xff, 0xff, 0x80, 0x01, 0xff, 0xff, 0xc0, 0x01, 0xff, 0xff, 0xe0, 0x01, 0xff, 0xff, 0xf0,
	0x03, 0xff, 0xff, 0xf8, 0x03, 0xff, 0xff, 0xf8, 0x03, 0xff, 0xff, 0xf8, 0x03, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
	0x80, 0x00, 0x00, 0x01, 0xb0, 0x00, 0x00, 0x0d, 0xb0, 0x00, 0x00, 0x0d, 0xb6, 0x00, 0x00,
	0x0d, 0xb6, 0x00, 0x00, 0x0d, 0xb6, 0x00, 0x00, 0x0d, 0xb6, 0x00, 0x00, 0x0d, 0xb6, 0x00,
	0x01, 0x8d, 0xb6, 0x00, 0x01, 0xcf, 0xfe, 0x00, 0x00, 0xef, 0xfe, 0x00, 0x00, 0xff, 0xfe,
	0x00, 0x00, 0x7f, 0xfe, 0x00, 0x00, 0x3f, 0xfe, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x1f,
	0xfc, 0x00, 0x00, 0x0f, 0xfc, 0x00, 0x00, 0x07, 0xf8, 0x00, 0x00, 0x03, 0xf8, 0x00, 0x00,
	0x03, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00
};
static DDImage* gFPSImage = NULL;

//////////////////////////////////////////////////////////////////////////

typedef HRESULT(WINAPI* SHGetFolderPathFunc)(HWND, int, HANDLE, DWORD, LPTSTR);
void* GetSHGetFolderPath(const char* theDLL, HMODULE* theMod)
{
	HMODULE aMod = LoadLibrary(theDLL);
	SHGetFolderPathFunc aFunc = NULL;

	if (aMod != NULL)
	{
		*((void**)&aFunc) = (void*)GetProcAddress(aMod, "SHGetFolderPathA");
		if (aFunc == NULL)
		{
			FreeLibrary(aMod);
			aMod = NULL;
		}
	}

	*theMod = aMod;
	return aFunc;
}

//////////////////////////////////////////////////////////////////////////

SexyAppBase::SexyAppBase()
{
	gSexyAppBase = this;
	SDL_SetHint("SDL_DISABLE_WINDOWS_IME ", "1");
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
#ifdef SDL_PLATFORM_WINDOWS
		MessageBoxA(NULL, "Failed to init SDL", "Error!", MB_ICONERROR);
#endif
		exit(0);
	}

	gVersionDLL = LoadLibraryA("version.dll");
	gDDrawDLL = LoadLibraryA("ddraw.dll");
	gDSoundDLL = LoadLibraryA("dsound.dll");
	gGetLastInputInfoFunc = (GetLastInputInfoFunc)GetProcAddress(GetModuleHandleA("user32.dll"), "GetLastInputInfo");

	ImageLib::InitJPEG2000();

	mMutex = NULL;
	mNotifyGameMessage = 0;

#ifdef _DEBUG
	mOnlyAllowOneCopyToRun = false;
#else
	mOnlyAllowOneCopyToRun = true;
#endif

	// Extract product version
	char aPath[_MAX_PATH];
	GetModuleFileNameA(NULL, aPath, 256);
	mProductVersion = GetProductVersion(aPath);
	mChangeDirTo = GetFileDir(aPath);

	mNoDefer = false;
	mFullScreenPageFlip = true; // should we page flip in fullscreen?
	mTimeLoaded = SDL_GetTicks();
	mSEHOccured = false;
	mProdName = "Product";
	mTitle = _S("SexyApp");
	mShutdown = false;
	mExitToTop = false;
	mWidth = 640;
	mHeight = 480;
	mFullscreenBits = 32;
	mIsWindowed = true;
	mIsPhysWindowed = true;
	mFullScreenWindow = false;
	mPreferredX = -1;
	mPreferredY = -1;
	mAllowMonitorPowersave = true;
	win = NULL;
	mHWnd = NULL;
	mDDInterface = NULL;
	mMusicInterface = NULL;
	mFrameTime = 10;
	mNonDrawCount = 0;
	mDrawCount = 0;
	mSleepCount = 0;
	mUpdateCount = 0;
	mUpdateAppState = 0;
	mUpdateAppDepth = 0;
	mPendingUpdatesAcc = 0.0;
	mUpdateFTimeAcc = 0.0;
	mHasPendingDraw = true;
	mIsDrawing = false;
	mLastDrawWasEmpty = false;
	mLastTimeCheck = 0;
	mUpdateMultiplier = 1;
	mPaused = false;
	mFastForwardToUpdateNum = 0;
	mFastForwardToMarker = false;
	mFastForwardStep = false;
	mSoundManager = NULL;
	mCursorNum = CURSOR_POINTER;
	mMouseIn = false;
	mRunning = false;
	mActive = true;
	mProcessInTimer = false;
	mMinimized = false;
	mPhysMinimized = false;
	mIsDisabled = false;
	mLoaded = false;
	mYieldMainThread = false;
	mLoadingFailed = false;
	mLoadingThreadStarted = false;
	mAutoStartLoadingThread = true;
	mLoadingThreadCompleted = false;
	mCursorThreadRunning = false;
	mNumLoadingThreadTasks = 0;
	mCompletedLoadingThreadTasks = 0;
	mLastDrawTick = timeGetTime();
	mNextDrawTick = timeGetTime();
	mSysCursor = true;
	mForceFullscreen = false;
	mForceWindowed = false;
	mHasFocus = true;
	mCustomCursorsEnabled = false;
	mCustomCursorDirty = false;
	mOverrideCursor = NULL;
	mIsOpeningURL = false;
	mInitialized = false;
	mLastShutdownWasGraceful = true;
	mReadFromRegistry = false;
	mCmdLineParsed = false;
	mSkipSignatureChecks = false;
	mCtrlDown = false;
	mAltDown = false;
	mStepMode = 0;
	mCleanupSharedImages = false;
	mStandardWordWrap = true;
	mbAllowExtendedChars = true;
	mEnableMaximizeButton = false;
	mMouseSensitivity = 1.0f;

	mMusicVolume = 0.85;
	mSfxVolume = 0.85;
	mMuteCount = 0;
	mAutoMuteCount = 0;
	mMuteOnLostFocus = true;
	mFPSTime = 0;
	mFPSStartTick = SDL_GetTicks();
	mFPSFlipCount = 0;
	mFPSCount = 0;
	mFPSDirtyCount = 0;
	mShowFPS = false;
	mShowFPSMode = FPS_ShowFPS;
	mDrawTime = 0;
	mScreenBltTime = 0;
	mAlphaDisabled = false;
	mDebugKeysEnabled = false;
	mOldWndProc = 0;
	mNoSoundNeeded = false;
	mWantFMod = false;

	mSyncRefreshRate = 100;
	mVSyncUpdates = false;
	mVSyncBroken = false;
	mVSyncBrokenCount = 0;
	mVSyncBrokenTestStartTick = 0;
	mVSyncBrokenTestUpdates = 0;
	mWaitForVSync = false;
	mSoftVSyncWait = true;
	mUserChanged3DSetting = false;
	mAutoEnable3D = false;
	mTest3D = false;
	mMinVidMemory3D = 6;
	mRecommendedVidMemory3D = 14;
	mRelaxUpdateBacklogCount = 0;
	mWidescreenAware = false;
	mEnableWindowAspect = false;
	mWindowAspect.Set(16, 9);
	mIsWideWindow = false;
	mAspectCorrect = true;
	mAspectNoStretch = false;
	mDiscordPresence = true;
	mSpeedModifier = 2;
	mQuickLevel = 1;
	mBankKeybinds = false;
	mZeroNineBankFormat = false;
	mAutoCollectSuns = false;
	mAutoCollectCoins = false;
	mZombieHealthbars = false;
	mPlantHealthbars = false;
	mIs3dAccel = true;
	mCrazySeeds = false;

	int i;

	for (i = 0; i < NUM_CURSORS; i++)
		mCursorImages[i] = NULL;

	for (i = 0; i < 256; i++)
		mAdd8BitMaxTable[i] = i;

	for (i = 256; i < 512; i++)
		mAdd8BitMaxTable[i] = 255;

	// Set default strings.  Init could read in overrides from partner.xml
	SetString("DIALOG_BUTTON_OK", L"OK");
	SetString("DIALOG_BUTTON_CANCEL", L"CANCEL");

	SetString("UPDATE_CHECK_TITLE", L"Update Check");
	SetString("UPDATE_CHECK_BODY", L"Checking if there are any updates available for this product ...");

	SetString("UP_TO_DATE_TITLE", L"Up to Date");
	SetString("UP_TO_DATE_BODY", L"There are no updates available for this product at this time.");
	SetString("NEW_VERSION_TITLE", L"New Version");
	SetString("NEW_VERSION_BODY", L"There is an update available for this product.  Would you like to visit the web site to download it?");

	mWidgetManager = new WidgetManager(this);
	mResourceManager = new ResourceManager(this);

	mPrimaryThreadId = 0;

#ifdef SDL_PLATFORM_WINDOWS
	if (GetSystemMetrics(86)) // check for tablet pc
	{
		mTabletPC = true;
		mFullScreenPageFlip = false; // so that tablet keyboard can show up
	}
	else
		mTabletPC = false;
#endif
}

SexyAppBase::~SexyAppBase()
{
	Shutdown();

	// Check if we should write the current 3d setting
	bool showedMsgBox = false;
	if (mUserChanged3DSetting)
	{
		bool writeToRegistry = true;
		bool is3D = false;
		bool is3DOptionSet = RegistryReadBoolean("Is3D", &is3D);
		if(!is3DOptionSet) // should we write the option?
		{
			if(!Is3DAccelerationRecommended()) // may need to prompt user if he wants to keep 3d acceleration on
			{
				if (Is3DAccelerated())
				{
					showedMsgBox = true;
					int aResult = 1;
					mDDInterface->mIs3D = aResult == 1;
					if (aResult!=1)
						writeToRegistry = false;
				}
				else
					writeToRegistry = false;
			}
		}

		if (writeToRegistry)
			RegistryWriteBoolean("Is3D", mDDInterface->mIs3D);
	}

	extern bool gD3DInterfacePreDrawError;
	if (!showedMsgBox && gD3DInterfacePreDrawError)
	{
		int aResult = 1;
		mDDInterface->mIs3D = aResult == 1;

		if (aResult==0)
			RegistryWriteBoolean("Is3D", false);
	}


	DialogMap::iterator aDialogItr = mDialogMap.begin();
	while (aDialogItr != mDialogMap.end())
	{
		mWidgetManager->RemoveWidget(aDialogItr->second);
		delete aDialogItr->second;
		++aDialogItr;
	}
	mDialogMap.clear();
	mDialogList.clear();

	delete mWidgetManager;
	delete mResourceManager;
	delete gFPSImage;
	gFPSImage = NULL;

	SharedImageMap::iterator aSharedImageItr = mSharedImageMap.begin();
	while (aSharedImageItr != mSharedImageMap.end())
	{
		SharedImage* aSharedImage = &aSharedImageItr->second;
		//	DBG_ASSERTE(aSharedImage->mRefCount == 0);		// Possibly the stupidest thing ever, I'm tired of tripping this
		delete aSharedImage->mImage;
		mSharedImageMap.erase(aSharedImageItr++);
	}

	delete mDDInterface;
	delete mMusicInterface;
	delete mSoundManager;

	if (mHWnd != NULL)
	{
		mHWnd = NULL;
		global_sexy_handle = NULL;
	}
	if (win != NULL) {
		SDL_StopTextInput(win);
		SDL_DestroyWindow(win);
		win = NULL;
	}

	WaitForLoadingThread();

	DestroyCursor(mHandCursor);
	DestroyCursor(mDraggingCursor);

	gSexyAppBase = NULL;

	if (mMutex != NULL)
		::CloseHandle(mMutex);

	FreeLibrary(gDDrawDLL);
	FreeLibrary(gDSoundDLL);
	FreeLibrary(gVersionDLL);
	SDL_Quit();
}

static BOOL CALLBACK ChangeDisplayWindowEnumProc(HWND hwnd, LPARAM lParam)
{
	typedef std::map<HWND, RECT> WindowMap;
	static WindowMap aMap;

	if (lParam == 0 && aMap.find(hwnd) == aMap.end()) // record
	{
		RECT aRect;
		if (!IsIconic(hwnd) && IsWindowVisible(hwnd))
		{
			if (GetWindowRect(hwnd, &aRect))
			{
				//				char aBuf[4096];
				//				GetWindowText(hwnd,aBuf,4000);
				//				DWORD aProcessId = 0;
				//				GetWindowThreadProcessId(hwnd,&aProcessId);
				//				SEXY_TRACE(StrFormat("%s %d - %d %d %d %d",aBuf,aProcessId,aRect.left,aRect.top,aRect.right,aRect.bottom).c_str());
				aMap[hwnd] = aRect;
			}
		}
	}
	else
	{
		WindowMap::iterator anItr = aMap.find(hwnd);
		if (anItr != aMap.end())
		{
			RECT& r = anItr->second;
			MoveWindow(hwnd, r.left, r.top, abs(r.right - r.left), abs(r.bottom - r.top), TRUE);
		}
	}
	return TRUE;
}

void SexyAppBase::ClearUpdateBacklog(bool relaxForASecond)
{
	mLastTimeCheck = timeGetTime();
	mUpdateFTimeAcc = 0.0;

	if (relaxForASecond)
		mRelaxUpdateBacklogCount = 1000;
}

bool SexyAppBase::AppCanRestore()
{
	return !mIsDisabled;
}

Dialog* SexyAppBase::NewDialog(int theDialogId, bool isModal, const SexyString& theDialogHeader, const SexyString& theDialogLines, const SexyString& theDialogFooter, int theButtonMode)
{
	Dialog* aDialog = new Dialog(NULL, NULL, theDialogId, isModal, theDialogHeader, theDialogLines, theDialogFooter, theButtonMode);
	return aDialog;
}

Dialog* SexyAppBase::DoDialog(int theDialogId, bool isModal, const SexyString& theDialogHeader, const SexyString& theDialogLines, const SexyString& theDialogFooter, int theButtonMode)
{
	KillDialog(theDialogId);

	Dialog* aDialog = NewDialog(theDialogId, isModal, theDialogHeader, theDialogLines, theDialogFooter, theButtonMode);

	AddDialog(theDialogId, aDialog);

	return aDialog;
}


Dialog* SexyAppBase::GetDialog(int theDialogId)
{
	DialogMap::iterator anItr = mDialogMap.find(theDialogId);

	if (anItr != mDialogMap.end())
		return anItr->second;

	return NULL;
}

bool SexyAppBase::KillDialog(int theDialogId, bool removeWidget, bool deleteWidget)
{
	DialogMap::iterator anItr = mDialogMap.find(theDialogId);

	if (anItr != mDialogMap.end())
	{
		Dialog* aDialog = anItr->second;

		// set the result to something else so DoMainLoop knows that the dialog is gone 
		// in case nobody else sets mResult		
		if (aDialog->mResult == -1)
			aDialog->mResult = 0;

		DialogList::iterator aListItr = std::find(mDialogList.begin(), mDialogList.end(), aDialog);
		if (aListItr != mDialogList.end())
			mDialogList.erase(aListItr);

		mDialogMap.erase(anItr);

		if (removeWidget || deleteWidget)
			mWidgetManager->RemoveWidget(aDialog);

		if (aDialog->IsModal())
		{
			ModalClose();
			mWidgetManager->RemoveBaseModal(aDialog);
		}

		if (deleteWidget)
			SafeDeleteWidget(aDialog);

		return true;
	}

	return false;
}

bool SexyAppBase::KillDialog(int theDialogId)
{
	return KillDialog(theDialogId, true, true);
}

bool SexyAppBase::KillDialog(Dialog* theDialog)
{
	return KillDialog(theDialog->mId);
}

int SexyAppBase::GetDialogCount()
{
	return mDialogMap.size();
}

void SexyAppBase::AddDialog(int theDialogId, Dialog* theDialog)
{
	KillDialog(theDialogId);

	if (theDialog->mWidth == 0)
	{
		// Set the dialog position ourselves
		int aWidth = mWidth / 2;
		theDialog->Resize((mWidth - aWidth) / 2, mHeight / 5, aWidth, theDialog->GetPreferredHeight(aWidth));
	}

	mDialogMap.insert(DialogMap::value_type(theDialogId, theDialog));
	mDialogList.push_back(theDialog);

	mWidgetManager->AddWidget(theDialog);
	if (theDialog->IsModal())
	{
		mWidgetManager->AddBaseModal(theDialog);
		ModalOpen();
	}
}

void SexyAppBase::AddDialog(Dialog* theDialog)
{
	AddDialog(theDialog->mId, theDialog);
}

void SexyAppBase::ModalOpen()
{
}

void SexyAppBase::ModalClose()
{
}

void SexyAppBase::DialogButtonPress(int theDialogId, int theButtonId)
{
	if (theButtonId == Dialog::ID_YES)
		ButtonPress(2000 + theDialogId);
	else if (theButtonId == Dialog::ID_NO)
		ButtonPress(3000 + theDialogId);
}

void SexyAppBase::DialogButtonDepress(int theDialogId, int theButtonId)
{
	if (theButtonId == Dialog::ID_YES)
		ButtonDepress(2000 + theDialogId);
	else if (theButtonId == Dialog::ID_NO)
		ButtonDepress(3000 + theDialogId);
}

void SexyAppBase::GotFocus()
{
}

void SexyAppBase::LostFocus()
{
}

void SexyAppBase::URLOpenFailed(const std::string& theURL)
{
	mIsOpeningURL = false;
}

void SexyAppBase::URLOpenSucceeded(const std::string& theURL)
{
	mIsOpeningURL = false;

	if (mShutdownOnURLOpen)
		Shutdown();
}

bool SexyAppBase::OpenURL(const std::string& theURL, bool shutdownOnOpen)
{
	if ((!mIsOpeningURL) || (theURL != mOpeningURL))
	{
		mShutdownOnURLOpen = shutdownOnOpen;
		mIsOpeningURL = true;
		mOpeningURL = theURL;
		mOpeningURLTime = SDL_GetTicks();

		if ((int)ShellExecuteA(NULL, "open", theURL.c_str(), NULL, NULL, SW_SHOWNORMAL) > 32)
		{
			return true;
		}
		else
		{
			URLOpenFailed(theURL);
			return false;
		}
	}

	return true;
}

std::string SexyAppBase::GetProductVersion(const std::string& thePath)
{
	// Dynamically Load Version.dll
	typedef DWORD(APIENTRY* GetFileVersionInfoSizeFunc)(LPSTR lptstrFilename, LPDWORD lpdwHandle);
	typedef BOOL(APIENTRY* GetFileVersionInfoFunc)(LPSTR lptstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData);
	typedef BOOL(APIENTRY* VerQueryValueFunc)(const LPVOID pBlock, LPSTR lpSubBlock, LPVOID* lplpBuffer, PUINT puLen);

	static GetFileVersionInfoSizeFunc aGetFileVersionInfoSizeFunc = NULL;
	static GetFileVersionInfoFunc aGetFileVersionInfoFunc = NULL;
	static VerQueryValueFunc aVerQueryValueFunc = NULL;

	if (aGetFileVersionInfoSizeFunc == NULL)
	{
		aGetFileVersionInfoSizeFunc = (GetFileVersionInfoSizeFunc)GetProcAddress(gVersionDLL, "GetFileVersionInfoSizeA");
		aGetFileVersionInfoFunc = (GetFileVersionInfoFunc)GetProcAddress(gVersionDLL, "GetFileVersionInfoA");
		aVerQueryValueFunc = (VerQueryValueFunc)GetProcAddress(gVersionDLL, "VerQueryValueA");
	}

	// Get Product Version
	std::string aProductVersion;

	uint aSize = aGetFileVersionInfoSizeFunc((char*)thePath.c_str(), 0);
	if (aSize > 0)
	{
		uchar* aVersionBuffer = new uchar[aSize];
		aGetFileVersionInfoFunc((char*)thePath.c_str(), 0, aSize, aVersionBuffer);
		char* aBuffer;
		if (aVerQueryValueFunc(aVersionBuffer,
			"\\StringFileInfo\\040904B0\\ProductVersion",
			(void**)&aBuffer,
			&aSize))
		{
			aProductVersion = aBuffer;
		}
		else if (aVerQueryValueFunc(aVersionBuffer,
			"\\StringFileInfo\\040904E4\\ProductVersion",
			(void**)&aBuffer,
			&aSize))
		{
			aProductVersion = aBuffer;
		}

		delete aVersionBuffer;
	}

	return aProductVersion;
}

void SexyAppBase::WaitForLoadingThread()
{
	while ((mLoadingThreadStarted) && (!mLoadingThreadCompleted))
		Sleep(20);
}

void SexyAppBase::SetCursorImage(int theCursorNum, Image* theImage)
{
	if ((theCursorNum >= 0) && (theCursorNum < NUM_CURSORS))
	{
		mCursorImages[theCursorNum] = theImage;
		EnforceCursor();
	}
}

void WriteScreenShotThread(void* theArg)
{
	//////////////////////////////////////////////////////////////////////////
	// Validate the passed parameter
	DDImage* theImage = (DDImage*)theArg;
	if (theImage == NULL) return;

	//////////////////////////////////////////////////////////////////////////
	// Get free image name
	std::string anImageDir = GetAppDataFolder() + "_screenshots";
	MkDir(anImageDir);
	anImageDir += "/";

	WIN32_FIND_DATAA aData;
	int aMaxId = 0;
	std::string anImagePrefix = "image";
	HANDLE aHandle = FindFirstFileA((anImageDir + "*.png").c_str(), &aData);
	if (aHandle != INVALID_HANDLE_VALUE)
	{
		do {
			int aNum = 0;
			if (sscanf(aData.cFileName, (anImagePrefix + "%d.png").c_str(), &aNum) == 1)
			{
				if (aNum > aMaxId)
					aMaxId = aNum;
			}

		} while (FindNextFileA(aHandle, &aData));
		FindClose(aHandle);
	}
	std::string anImageName = anImageDir + anImagePrefix + StrFormat("%d.png", aMaxId + 1);

	//////////////////////////////////////////////////////////////////////////
	// Write image
	ImageLib::Image aSaveImage;
	aSaveImage.mBits = theImage->mBits;
	aSaveImage.mWidth = theImage->mWidth;
	aSaveImage.mHeight = theImage->mHeight;
	ImageLib::WritePNGImage(anImageName, &aSaveImage);
	aSaveImage.mBits = NULL;

	//////////////////////////////////////////////////////////////////////////
	// delete the image in this thread
	delete theImage;
}

void SexyAppBase::TakeScreenshot()
{
	if (mDDInterface == NULL || mDDInterface->mDrawSurface == NULL)
		return;

	// Capture screen
	LPDIRECTDRAWSURFACE aSurface = mDDInterface->mDrawSurface;

	// Temporarily set the mDrawSurface to NULL so DDImage::Check3D 
	// returns false so we can lock the surface.
	mDDInterface->mDrawSurface = NULL;

	DDImage* anImage = new DDImage(mDDInterface);
	anImage->SetSurface(aSurface);
	anImage->GetBits();
	anImage->DeleteDDSurface();
	mDDInterface->mDrawSurface = aSurface;

	if (anImage->mBits == NULL)
	{
		delete anImage;
		return;
	}

	_beginthread(WriteScreenShotThread, 0, anImage);

	//	ClearUpdateBacklog();
}

void SexyAppBase::DumpProgramInfo()
{
	Deltree(GetAppDataFolder() + "_dump");

	for (;;)
	{
		if (mkdir((GetAppDataFolder() + "_dump").c_str()))
			break;
		Sleep(100);
	}

	std::fstream aDumpStream((GetAppDataFolder() + "_dump\\imagelist.html").c_str(), std::ios::out);

	time_t aTime;
	time(&aTime);
	tm* aTM = localtime(&aTime);

	aDumpStream << "<HTML><BODY BGCOLOR=EEEEFF><CENTER><FONT SIZE=+2><B>" << asctime(aTM) << "</B></FONT><BR>" << std::endl;

	int anImgNum = 0;

	int aThumbWidth = 64;
	int aThumbHeight = 64;

	ImageLib::Image anImageLibImage;
	anImageLibImage.mWidth = aThumbWidth;
	anImageLibImage.mHeight = aThumbHeight;
	anImageLibImage.mBits = new unsigned long[aThumbWidth * aThumbHeight];

	typedef std::multimap<int, MemoryImage*, std::greater<int> > SortedImageMap;

	int aTotalMemory = 0;

	SortedImageMap aSortedImageMap;
	MemoryImageSet::iterator anItr = mMemoryImageSet.begin();
	while (anItr != mMemoryImageSet.end())
	{
		MemoryImage* aMemoryImage = *anItr;

		int aNumPixels = aMemoryImage->mWidth * aMemoryImage->mHeight;

		DDImage* aDDImage = dynamic_cast<DDImage*>(aMemoryImage);

		int aBitsMemory = 0;
		int aSurfaceMemory = 0;
		int aPalletizedMemory = 0;
		int aNativeAlphaMemory = 0;
		int aRLAlphaMemory = 0;
		int aRLAdditiveMemory = 0;
		int aTextureMemory = 0;

		int aMemorySize = 0;
		if (aMemoryImage->mBits != NULL)
			aBitsMemory = aNumPixels * 4;
		if ((aDDImage != NULL) && (aDDImage->mSurface != NULL))
			aSurfaceMemory = aNumPixels * 4; // Assume 32bit screen...
		if (aMemoryImage->mColorTable != NULL)
			aPalletizedMemory = aNumPixels + 256 * 4;
		if (aMemoryImage->mNativeAlphaData != NULL)
		{
			if (aMemoryImage->mColorTable != NULL)
				aNativeAlphaMemory = 256 * 4;
			else
				aNativeAlphaMemory = aNumPixels * 4;
		}
		if (aMemoryImage->mRLAlphaData != NULL)
			aRLAlphaMemory = aNumPixels;
		if (aMemoryImage->mRLAdditiveData != NULL)
			aRLAdditiveMemory = aNumPixels;
		if (aMemoryImage->mD3DData != NULL)
			aTextureMemory += ((TextureData*)aMemoryImage->mD3DData)->mTexMemSize;

		aMemorySize = aBitsMemory + aSurfaceMemory + aPalletizedMemory + aNativeAlphaMemory + aRLAlphaMemory + aRLAdditiveMemory + aTextureMemory;
		aTotalMemory += aMemorySize;

		aSortedImageMap.insert(SortedImageMap::value_type(aMemorySize, aMemoryImage));

		++anItr;
	}

	aDumpStream << "Total Image Allocation: " << CommaSeperate(aTotalMemory).c_str() << " bytes<BR>";
	aDumpStream << "<TABLE BORDER=1 CELLSPACING=0 CELLPADDING=4>";

	int aTotalMemorySize = 0;
	int aTotalBitsMemory = 0;
	int aTotalSurfaceMemory = 0;
	int aTotalPalletizedMemory = 0;
	int aTotalNativeAlphaMemory = 0;
	int aTotalRLAlphaMemory = 0;
	int aTotalRLAdditiveMemory = 0;
	int aTotalTextureMemory = 0;

	SortedImageMap::iterator aSortedItr = aSortedImageMap.begin();
	while (aSortedItr != aSortedImageMap.end())
	{
		MemoryImage* aMemoryImage = aSortedItr->second;

		char anImageName[256];
		sprintf(anImageName, "img%04d.png", anImgNum);

		char aThumbName[256];
		sprintf(aThumbName, "thumb%04d.jpg", anImgNum);

		aDumpStream << "<TR>" << std::endl;

		aDumpStream << "<TD><A HREF=" << anImageName << "><IMG SRC=" << aThumbName << " WIDTH=" << aThumbWidth << " HEIGHT=" << aThumbHeight << "></A></TD>" << std::endl;

		int aNumPixels = aMemoryImage->mWidth * aMemoryImage->mHeight;

		DDImage* aDDImage = dynamic_cast<DDImage*>(aMemoryImage);

		int aMemorySize = aSortedItr->first;

		int aBitsMemory = 0;
		int aSurfaceMemory = 0;
		int aPalletizedMemory = 0;
		int aNativeAlphaMemory = 0;
		int aRLAlphaMemory = 0;
		int aRLAdditiveMemory = 0;
		int aTextureMemory = 0;
		std::string aTextureFormatName;

		if (aMemoryImage->mBits != NULL)
			aBitsMemory = aNumPixels * 4;
		if ((aDDImage != NULL) && (aDDImage->mSurface != NULL))
			aSurfaceMemory = aNumPixels * 4; // Assume 32bit screen...
		if (aMemoryImage->mColorTable != NULL)
			aPalletizedMemory = aNumPixels + 256 * 4;
		if (aMemoryImage->mNativeAlphaData != NULL)
		{
			if (aMemoryImage->mColorTable != NULL)
				aNativeAlphaMemory = 256 * 4;
			else
				aNativeAlphaMemory = aNumPixels * 4;
		}
		if (aMemoryImage->mRLAlphaData != NULL)
			aRLAlphaMemory = aNumPixels;
		if (aMemoryImage->mRLAdditiveData != NULL)
			aRLAdditiveMemory = aNumPixels;
		if (aMemoryImage->mD3DData != NULL)
		{
			aTextureMemory += ((TextureData*)aMemoryImage->mD3DData)->mTexMemSize;

			switch (((TextureData*)aMemoryImage->mD3DData)->mPixelFormat)
			{
			case PixelFormat_A8R8G8B8: aTextureFormatName = "A8R8G8B8"; break;
			case PixelFormat_A4R4G4B4: aTextureFormatName = "A4R4G4B4"; break;
			case PixelFormat_R5G6B5: aTextureFormatName = "R5G6B5"; break;
			case PixelFormat_Palette8: aTextureFormatName = "Palette8"; break;
			}
		}

		aTotalMemorySize += aMemorySize;
		aTotalBitsMemory += aBitsMemory;
		aTotalTextureMemory += aTextureMemory;
		aTotalSurfaceMemory += aSurfaceMemory;
		aTotalPalletizedMemory += aPalletizedMemory;
		aTotalNativeAlphaMemory += aNativeAlphaMemory;
		aTotalRLAlphaMemory += aRLAlphaMemory;
		aTotalRLAdditiveMemory += aRLAdditiveMemory;



		char aStr[256];
		sprintf(aStr, "%d x %d<BR>%s bytes", aMemoryImage->mWidth, aMemoryImage->mHeight, CommaSeperate(aMemorySize).c_str());
		aDumpStream << "<TD ALIGN=RIGHT>" << aStr << "</TD>" << std::endl;

		aDumpStream << "<TD>" << SexyStringToString(((aBitsMemory != 0) ? _S("mBits<BR>") + CommaSeperate(aBitsMemory) : _S("&nbsp;"))) << "</TD>" << std::endl;
		aDumpStream << "<TD>" << SexyStringToString(((aPalletizedMemory != 0) ? _S("Palletized<BR>") + CommaSeperate(aPalletizedMemory) : _S("&nbsp;"))) << "</TD>" << std::endl;
		aDumpStream << "<TD>" << SexyStringToString(((aSurfaceMemory != 0) ? _S("DDSurface<BR>") + CommaSeperate(aSurfaceMemory) : _S("&nbsp;"))) << "</TD>" << std::endl;
		aDumpStream << "<TD>" << SexyStringToString(((aMemoryImage->mD3DData != NULL) ? _S("Texture<BR>") + StringToSexyString(aTextureFormatName) + _S("<BR>") + CommaSeperate(aTextureMemory) : _S("&nbsp;"))) << "</TD>" << std::endl;

		aDumpStream << "<TD>" << SexyStringToString(((aMemoryImage->mIsVolatile) ? _S("Volatile") : _S("&nbsp;"))) << "</TD>" << std::endl;
		aDumpStream << "<TD>" << SexyStringToString(((aMemoryImage->mForcedMode) ? _S("Forced") : _S("&nbsp;"))) << "</TD>" << std::endl;
		aDumpStream << "<TD>" << SexyStringToString(((aMemoryImage->mHasAlpha) ? _S("HasAlpha") : _S("&nbsp;"))) << "</TD>" << std::endl;
		aDumpStream << "<TD>" << SexyStringToString(((aMemoryImage->mHasTrans) ? _S("HasTrans") : _S("&nbsp;"))) << "</TD>" << std::endl;
		aDumpStream << "<TD>" << SexyStringToString(((aNativeAlphaMemory != 0) ? _S("NativeAlpha<BR>") + CommaSeperate(aNativeAlphaMemory) : _S("&nbsp;"))) << "</TD>" << std::endl;
		aDumpStream << "<TD>" << SexyStringToString(((aRLAlphaMemory != 0) ? _S("RLAlpha<BR>") + CommaSeperate(aRLAlphaMemory) : _S("&nbsp;"))) << "</TD>" << std::endl;
		aDumpStream << "<TD>" << SexyStringToString(((aRLAdditiveMemory != 0) ? _S("RLAdditive<BR>") + CommaSeperate(aRLAdditiveMemory) : _S("&nbsp;"))) << "</TD>" << std::endl;
		aDumpStream << "<TD>" << (aMemoryImage->mFilePath.empty() ? "&nbsp;" : aMemoryImage->mFilePath) << "</TD>" << std::endl;

		aDumpStream << "</TR>" << std::endl;

		// Write thumb

		MemoryImage aCopiedImage(*aMemoryImage);

		ulong* aBits = aCopiedImage.GetBits();

		ulong* aThumbBitsPtr = anImageLibImage.mBits;

		for (int aThumbY = 0; aThumbY < aThumbHeight; aThumbY++)
			for (int aThumbX = 0; aThumbX < aThumbWidth; aThumbX++)
			{
				int aSrcX = (int)(aCopiedImage.mWidth * (aThumbX + 0.5)) / aThumbWidth;
				int aSrcY = (int)(aCopiedImage.mHeight * (aThumbY + 0.5)) / aThumbHeight;

				*(aThumbBitsPtr++) = aBits[aSrcX + (aSrcY * aCopiedImage.mWidth)];
			}

		ImageLib::WriteJPEGImage((GetAppDataFolder() + std::string("_dump\\") + aThumbName).c_str(), &anImageLibImage);

		// Write high resolution image

		ImageLib::Image anFullImage;
		anFullImage.mBits = aCopiedImage.GetBits();
		anFullImage.mWidth = aCopiedImage.GetWidth();
		anFullImage.mHeight = aCopiedImage.GetHeight();

		ImageLib::WritePNGImage((GetAppDataFolder() + std::string("_dump\\") + anImageName).c_str(), &anFullImage);

		anFullImage.mBits = NULL;

		anImgNum++;

		aSortedItr++;
	}

	aDumpStream << "<TD>Totals</TD>" << std::endl;
	aDumpStream << "<TD>" << SexyStringToString(CommaSeperate(aTotalMemorySize)) << "</TD>" << std::endl;
	aDumpStream << "<TD>" << SexyStringToString(CommaSeperate(aTotalBitsMemory)) << "</TD>" << std::endl;
	aDumpStream << "<TD>" << SexyStringToString(CommaSeperate(aTotalPalletizedMemory)) << "</TD>" << std::endl;
	aDumpStream << "<TD>" << SexyStringToString(CommaSeperate(aTotalSurfaceMemory)) << "</TD>" << std::endl;
	aDumpStream << "<TD>" << SexyStringToString(CommaSeperate(aTotalTextureMemory)) << "</TD>" << std::endl;
	aDumpStream << "<TD>&nbsp;</TD>" << std::endl;
	aDumpStream << "<TD>&nbsp;</TD>" << std::endl;
	aDumpStream << "<TD>&nbsp;</TD>" << std::endl;
	aDumpStream << "<TD>&nbsp;</TD>" << std::endl;
	aDumpStream << "<TD>" << SexyStringToString(CommaSeperate(aTotalNativeAlphaMemory)) << "</TD>" << std::endl;
	aDumpStream << "<TD>" << SexyStringToString(CommaSeperate(aTotalRLAlphaMemory)) << "</TD>" << std::endl;
	aDumpStream << "<TD>" << SexyStringToString(CommaSeperate(aTotalRLAdditiveMemory)) << "</TD>" << std::endl;
	aDumpStream << "<TD>&nbsp;</TD>" << std::endl;

	aDumpStream << "</TABLE></CENTER></BODY></HTML>" << std::endl;
}

double SexyAppBase::GetLoadingThreadProgress()
{
	if (mLoaded)
		return 1.0;
	if (!mLoadingThreadStarted)
		return 0.0;
	if (mNumLoadingThreadTasks == 0)
		return 0.0;
	return min(mCompletedLoadingThreadTasks / (double)mNumLoadingThreadTasks, 1.0);
}

bool SexyAppBase::RegistryWrite(const std::string& theValueName, ulong theType, const uchar* theValue, ulong theLength)
{
	if (mRegKey.length() == 0)
		return false;

	HKEY aGameKey;

	std::string aKeyName = RemoveTrailingSlash("SOFTWARE\\" + mRegKey);
	std::string aValueName;

	int aSlashPos = (int)theValueName.rfind('\\');
	if (aSlashPos != -1)
	{
		aKeyName += "\\" + theValueName.substr(0, aSlashPos);
		aValueName = theValueName.substr(aSlashPos + 1);
	}
	else
	{
		aValueName = theValueName;
	}

	int aResult = RegOpenKeyExA(HKEY_CURRENT_USER, aKeyName.c_str(), 0, KEY_WRITE, &aGameKey);
	if (aResult != ERROR_SUCCESS)
	{
		ulong aDisp;
		aResult = RegCreateKeyExA(HKEY_CURRENT_USER, aKeyName.c_str(), 0, "Key", REG_OPTION_NON_VOLATILE,
			KEY_ALL_ACCESS, NULL, &aGameKey, &aDisp);
	}

	if (aResult != ERROR_SUCCESS)
	{
		return false;
	}

	RegSetValueExA(aGameKey, aValueName.c_str(), 0, theType, theValue, theLength);
	RegCloseKey(aGameKey);

	return true;
}

bool SexyAppBase::RegistryWriteString(const std::string& theValueName, const std::string& theString)
{
	return RegistryWrite(theValueName, REG_SZ, (uchar*)theString.c_str(), theString.length());
}

bool SexyAppBase::RegistryWriteInteger(const std::string& theValueName, int theValue)
{
	return RegistryWrite(theValueName, REG_DWORD, (uchar*)&theValue, sizeof(int));
}

bool SexyAppBase::RegistryWriteBoolean(const std::string& theValueName, bool theValue)
{
	int aValue = theValue ? 1 : 0;
	return RegistryWrite(theValueName, REG_DWORD, (uchar*)&aValue, sizeof(int));
}

bool SexyAppBase::RegistryWriteData(const std::string& theValueName, const uchar* theValue, ulong theLength)
{
	return RegistryWrite(theValueName, REG_BINARY, (uchar*)theValue, theLength);
}

void SexyAppBase::WriteToRegistry()
{
	RegistryWriteInteger("MusicVolume", (int)(mMusicVolume * 100));
	RegistryWriteInteger("SfxVolume", (int)(mSfxVolume * 100));
	RegistryWriteInteger("Muted", (mMuteCount - mAutoMuteCount > 0) ? 1 : 0);
	RegistryWriteInteger("ScreenMode", mIsWindowed ? 0 : 1);
	RegistryWriteInteger("AspectCorrection", mAspectCorrect ? 0 : 1);
	RegistryWriteInteger("AspectNoStretch", mAspectNoStretch ? 0 : 1);
	RegistryWriteInteger("PreferredX", mPreferredX);
	RegistryWriteInteger("PreferredY", mPreferredY);
	RegistryWriteInteger("CustomCursors", mCustomCursorsEnabled ? 1 : 0);
	RegistryWriteInteger("InProgress", 0);
	RegistryWriteInteger("QotL_SpeedModifier", mSpeedModifier);
	RegistryWriteInteger("QotL_QuickLevel", mQuickLevel);
	RegistryWriteBoolean("WaitForVSync", mWaitForVSync);
	RegistryWriteBoolean("QotL_DiscordPresence", mDiscordPresence);
	RegistryWriteBoolean("QotL_BankKeybinds", mBankKeybinds);
	RegistryWriteBoolean("QotL_ZeroNineBankFormat", mZeroNineBankFormat);
	RegistryWriteBoolean("QotL_AutoCollectSuns", mAutoCollectSuns);
	RegistryWriteBoolean("QotL_AutoCollectCoins", mAutoCollectCoins);
	RegistryWriteBoolean("QotL_ZombieHealthbars", mZombieHealthbars);
	RegistryWriteBoolean("QotL_PlantHealthbars", mPlantHealthbars);
	RegistryWriteBoolean("QotL_CrazyDaveSeeds", mCrazySeeds);
	RegistryWriteBoolean("3DAcceleration", mIs3dAccel);
	RegistryWriteInteger("MouseSensitivity", (int)(mMouseSensitivity * 100));
}

bool SexyAppBase::RegistryEraseKey(const SexyString& _theKeyName)
{
	std::string theKeyName = SexyStringToStringFast(_theKeyName);
	if (mRegKey.length() == 0)
		return false;

	std::string aKeyName = RemoveTrailingSlash("SOFTWARE\\" + mRegKey) + "\\" + theKeyName;

	int aResult = RegDeleteKeyA(HKEY_CURRENT_USER, aKeyName.c_str());
	if (aResult != ERROR_SUCCESS)
	{
		return false;
	}

	return true;
}

void SexyAppBase::RegistryEraseValue(const SexyString& _theValueName)
{
	std::string theValueName = SexyStringToStringFast(_theValueName);
	if (mRegKey.length() == 0)
		return;

	HKEY aGameKey;
	std::string aKeyName = RemoveTrailingSlash("SOFTWARE\\" + mRegKey);
	std::string aValueName;

	int aSlashPos = (int)theValueName.rfind('\\');
	if (aSlashPos != -1)
	{
		aKeyName += "\\" + theValueName.substr(0, aSlashPos);
		aValueName = theValueName.substr(aSlashPos + 1);
	}
	else
	{
		aValueName = theValueName;
	}

	int aResult = RegOpenKeyExA(HKEY_CURRENT_USER, aKeyName.c_str(), 0, KEY_WRITE, &aGameKey);
	if (aResult == ERROR_SUCCESS)
	{
		RegDeleteValueA(aGameKey, aValueName.c_str());
		RegCloseKey(aGameKey);
	}
}

bool SexyAppBase::RegistryGetSubKeys(const std::string& theKeyName, StringVector* theSubKeys)
{
	theSubKeys->clear();

	if (mRegKey.length() == 0)
		return false;

	HKEY aKey;

	std::string aKeyName = RemoveTrailingSlash(RemoveTrailingSlash("SOFTWARE\\" + mRegKey) + "\\" + theKeyName);
	int aResult = RegOpenKeyExA(HKEY_CURRENT_USER, aKeyName.c_str(), 0, KEY_READ, &aKey);

	if (aResult == ERROR_SUCCESS)
	{
		for (int anIdx = 0; ; anIdx++)
		{
			char aStr[1024];

			aResult = RegEnumKeyA(aKey, anIdx, aStr, 1024);
			if (aResult != ERROR_SUCCESS)
				break;

			theSubKeys->push_back(aStr);
		}

		RegCloseKey(aKey);
		return true;
	}
	else
	{
		return false;
	}
}

bool SexyAppBase::RegistryRead(const std::string& theValueName, ulong* theType, uchar* theValue, ulong* theLength)
{
	return RegistryReadKey(theValueName, theType, theValue, theLength, HKEY_CURRENT_USER);
}

bool SexyAppBase::RegistryReadKey(const std::string& theValueName, ulong* theType, uchar* theValue, ulong* theLength, HKEY theKey)
{
	if (mRegKey.length() == 0)
		return false;

	HKEY aGameKey;

	std::string aKeyName = RemoveTrailingSlash("SOFTWARE\\" + mRegKey);
	std::string aValueName;

	int aSlashPos = (int)theValueName.rfind('\\');
	if (aSlashPos != -1)
	{
		aKeyName += "\\" + theValueName.substr(0, aSlashPos);
		aValueName = theValueName.substr(aSlashPos + 1);
	}
	else
	{
		aValueName = theValueName;
	}

	if (RegOpenKeyExA(theKey, aKeyName.c_str(), 0, KEY_READ, &aGameKey) == ERROR_SUCCESS)
	{
		if (RegQueryValueExA(aGameKey, aValueName.c_str(), 0, theType, (uchar*)theValue, theLength) == ERROR_SUCCESS)
		{
			RegCloseKey(aGameKey);
			return true;
		}

		RegCloseKey(aGameKey);
	}

	return false;
}

bool SexyAppBase::RegistryReadString(const std::string& theKey, std::string* theString)
{
	char aStr[1024];

	ulong aType;
	ulong aLen = sizeof(aStr) - 1;
	if (!RegistryRead(theKey, &aType, (uchar*)aStr, &aLen))
		return false;

	if (aType != REG_SZ)
		return false;

	aStr[aLen] = 0;

	*theString = aStr;
	return true;
}

bool SexyAppBase::RegistryReadInteger(const std::string& theKey, int* theValue)
{
	ulong aType;
	ulong aLong;
	ulong aLen = 4;
	if (!RegistryRead(theKey, &aType, (uchar*)&aLong, &aLen))
		return false;

	if (aType != REG_DWORD)
		return false;

	*theValue = aLong;
	return true;
}

bool SexyAppBase::RegistryReadBoolean(const std::string& theKey, bool* theValue)
{
	int aValue;
	if (!RegistryReadInteger(theKey, &aValue))
		return false;

	*theValue = aValue != 0;
	return true;
}

bool SexyAppBase::RegistryReadData(const std::string& theKey, uchar* theValue, ulong* theLength)
{
	ulong aType;
	ulong aLen = *theLength;
	if (!RegistryRead(theKey, &aType, (uchar*)theValue, theLength))
		return false;

	if (aType != REG_BINARY)
		return false;

	return true;
}

void SexyAppBase::ReadFromRegistry()
{
	mReadFromRegistry = true;
	mRegKey = SexyStringToString(GetString("RegistryKey", StringToSexyString(mRegKey)));

	if (mRegKey.length() == 0)
		return;

	int anInt;
	if (RegistryReadInteger("MusicVolume", &anInt))
		mMusicVolume = anInt / 100.0;

	if (RegistryReadInteger("SfxVolume", &anInt))
		mSfxVolume = anInt / 100.0;

	if (RegistryReadInteger("Muted", &anInt))
		mMuteCount = anInt;

	if (RegistryReadInteger("ScreenMode", &anInt))
		mIsWindowed = anInt == 0;

	if (RegistryReadInteger("MouseSensitivity", &anInt))
		mMouseSensitivity = anInt / 100.0f;
	if (RegistryReadInteger("AspectCorrection", &anInt))
		mAspectCorrect = anInt == 0;

	// TODO: Uncomment this line to make this value persistent
	// 	if (RegistryReadInteger("AspectNoStretch", &anInt))
	// 		mAspectNoStretch = anInt == 0;

	RegistryReadInteger("PreferredX", &mPreferredX);
	RegistryReadInteger("PreferredY", &mPreferredY);
	RegistryReadInteger("QotL_SpeedModifier", &mSpeedModifier);
	RegistryReadInteger("QotL_QuickLevel", &mQuickLevel);
	mQuickLevel = max(1, min(mQuickLevel, NUM_LEVELS));

	if (RegistryReadInteger("CustomCursors", &anInt))
		EnableCustomCursors(anInt != 0);

	RegistryReadBoolean("WaitForVSync", &mWaitForVSync);
	RegistryReadBoolean("QotL_DiscordPresence", &mDiscordPresence);
	RegistryReadBoolean("QotL_BankKeybinds", &mBankKeybinds);
	RegistryReadBoolean("QotL_ZeroNineBankFormat", &mZeroNineBankFormat);
	RegistryReadBoolean("QotL_AutoCollectSuns", &mAutoCollectSuns);
	RegistryReadBoolean("QotL_AutoCollectCoins", &mAutoCollectCoins);
	RegistryReadBoolean("QotL_ZombieHealthbars", &mZombieHealthbars);
	RegistryReadBoolean("QotL_PlantHealthbars", &mPlantHealthbars);
	RegistryReadBoolean("QotL_CrazyDaveSeeds", &mCrazySeeds);
	RegistryReadBoolean("3DAcceleration", &mIs3dAccel);

	if (RegistryReadInteger("InProgress", &anInt))
		mLastShutdownWasGraceful = anInt == 0;

	RegistryWriteInteger("InProgress", 1);
}

bool SexyAppBase::WriteBytesToFile(const std::string& theFileName, const void* theData, unsigned long theDataLen)
{
	MkDir(GetFileDir(theFileName));
	FILE* aFP = fopen(theFileName.c_str(), "w+b");

	if (aFP == NULL)
	{
		return false;
	}

	fwrite(theData, 1, theDataLen, aFP);
	fclose(aFP);

	return true;
}

bool SexyAppBase::WriteBufferToFile(const std::string& theFileName, const Buffer* theBuffer)
{
	return WriteBytesToFile(theFileName, theBuffer->GetDataPtr(), theBuffer->GetDataLen());
}


bool SexyAppBase::ReadBufferFromFile(const std::string& theFileName, Buffer* theBuffer, bool dontWriteToDemo)
{
	PFILE* aFP = p_fopen(theFileName.c_str(), "rb");

	if (aFP == NULL)
	{
		return false;
	}

	p_fseek(aFP, 0, SEEK_END);
	int aFileSize = p_ftell(aFP);
	p_fseek(aFP, 0, SEEK_SET);

	uchar* aData = new uchar[aFileSize];

	p_fread(aData, 1, aFileSize, aFP);
	p_fclose(aFP);

	theBuffer->Clear();
	theBuffer->SetData(aData, aFileSize);

	delete[] aData;

	return true;
}

bool SexyAppBase::FileExists(const std::string& theFileName)
{
	PFILE* aFP = p_fopen(theFileName.c_str(), "rb");

	if (aFP == NULL)
		return false;

	p_fclose(aFP);
	return true;
}

bool SexyAppBase::EraseFile(const std::string& theFileName)
{
	return DeleteFileA(theFileName.c_str()) != 0;
}

void SexyAppBase::SEHOccured()
{
	SetMusicVolume(0);
	mSEHOccured = true;
	EnforceCursor();
}

std::string SexyAppBase::GetGameSEHInfo()
{
	int aSecLoaded = (SDL_GetTicks() - mTimeLoaded) / 1000;

	char aTimeStr[16];
	sprintf(aTimeStr, "%02d:%02d:%02d", (aSecLoaded / 60 / 60), (aSecLoaded / 60) % 60, aSecLoaded % 60);

	char aThreadIdStr[16];
	sprintf(aThreadIdStr, "%X", mPrimaryThreadId);

	std::string anInfoString =
		"Product: " + mProdName + "\r\n" +
		"Version: " + mProductVersion + "\r\n";

	anInfoString +=
		"Time Loaded: " + std::string(aTimeStr) + "\r\n"
		"Fullscreen: " + (mIsWindowed ? std::string("No") : std::string("Yes")) + "\r\n"
		"Primary ThreadId: " + aThreadIdStr + "\r\n";

	return anInfoString;
}

void SexyAppBase::GetSEHWebParams(DefinesMap* theDefinesMap)
{
}

void SexyAppBase::ShutdownHook()
{
}

void SexyAppBase::Shutdown()
{
	if ((mPrimaryThreadId != 0) && (GetCurrentThreadId() != mPrimaryThreadId))
	{
		mLoadingFailed = true;
	}
	else if (!mShutdown)
	{
		mExitToTop = true;
		mShutdown = true;
		ShutdownHook();

		// Blah
		while (mCursorThreadRunning)
		{
			Sleep(10);
		}

		if (mMusicInterface != NULL)
			mMusicInterface->StopAllMusic();

		if ((!mIsPhysWindowed) && (mDDInterface != NULL) && (mDDInterface->mDD != NULL))
		{
			mDDInterface->mDD->RestoreDisplayMode();
		}

		SDL_HideWindow(win);

		RestoreScreenResolution();

		if (mReadFromRegistry)
			WriteToRegistry();

		ImageLib::CloseJPEG2000();
	}
}

void SexyAppBase::RestoreScreenResolution()
{
	if (mFullScreenWindow)
	{
		EnumWindows(ChangeDisplayWindowEnumProc, 0); // get any windows that appeared while we were running
		ChangeDisplaySettings(NULL, 0);
		EnumWindows(ChangeDisplayWindowEnumProc, 1); // restore window pos
		mFullScreenWindow = false;
	}
}

void SexyAppBase::DoExit(int theCode)
{
	RestoreScreenResolution();
	exit(theCode);
}

void SexyAppBase::UpdateFrames()
{
	mUpdateCount++;

	if (!mMinimized)
	{
		if (mWidgetManager->UpdateFrame())
			++mFPSDirtyCount;
	}

	mMusicInterface->Update();
	CleanSharedImages();
}

void SexyAppBase::DoUpdateFramesF(float theFrac)
{
	if ((mVSyncUpdates) && (!mMinimized))
		mWidgetManager->UpdateFrameF(theFrac);
}

bool SexyAppBase::DoUpdateFrames()
{
	SEXY_AUTO_PERF("SexyAppBase::DoUpdateFrames");

	if (gScreenSaverActive)
		return false;

	if ((mLoadingThreadCompleted) && (!mLoaded))
	{
		::SetThreadPriority(::GetCurrentThread(), THREAD_PRIORITY_NORMAL);
		mLoaded = true;
		mYieldMainThread = false;
		LoadingThreadCompleted();
	}

	UpdateFrames();
	return true;
}

bool gIsFailing = false;

void SexyAppBase::Redraw(Rect* theClipRect)
{
	SEXY_AUTO_PERF("SexyAppBase::Redraw");

	// Do mIsDrawing check because we could enter here at a bad time if any windows messages
	//  are processed during WidgetManager->Draw
	if ((mIsDrawing) || (mShutdown))
		return;

	if (gScreenSaverActive)
		return;

	static DWORD aRetryTick = 0;
	if (!mDDInterface->Redraw(theClipRect))
	{
		extern bool gD3DInterfacePreDrawError;
		gD3DInterfacePreDrawError = false; // this predraw error happens naturally when ddraw is failing
		if (!gIsFailing)
		{
			//gDebugStream << SDL_GetTicks() << " Redraw failed!" << std::endl;
			gIsFailing = true;
		}

		WINDOWPLACEMENT aWindowPlacement;
		ZeroMemory(&aWindowPlacement, sizeof(aWindowPlacement));
		aWindowPlacement.length = sizeof(aWindowPlacement);
		::GetWindowPlacement(mHWnd, &aWindowPlacement);

		DWORD aTick = SDL_GetTicks();
		if ((mActive || (aTick - aRetryTick > 1000 && mIsPhysWindowed)) && (aWindowPlacement.showCmd != SW_SHOWMINIMIZED) && (!mMinimized))
		{
			aRetryTick = aTick;

			mWidgetManager->mImage = NULL;

			int aResult = InitDDInterface();

			//gDebugStream << SDL_GetTicks() << " ReInit..." << std::endl;

			if (0 && (mIsWindowed) && (aResult == DDInterface::RESULT_INVALID_COLORDEPTH))
			{
				//gDebugStream << SDL_GetTicks() << "ReInit Invalid Colordepth" << std::endl;
				if (!mActive) // don't switch to full screen if not active app
					return;

				SwitchScreenMode(false);
				// mForceFullscreen = true;
				return;
			}
			else if (0 && aResult == DDInterface::RESULT_3D_FAIL)
			{
				Set3DAcclerated(false);
				return;
			}
			else if (0 && aResult != DDInterface::RESULT_OK)
			{
				//gDebugStream << SDL_GetTicks() << " ReInit Failed" << std::endl;
				//Fail("Failed to initialize DirectDraw");
				//Sleep(1000);				

				return;
			}

			ReInitImages();

			mWidgetManager->mImage = mDDInterface->GetScreenImage();
			mWidgetManager->MarkAllDirty();

			mLastTime = timeGetTime();
		}
	}
	else
	{
		if (gIsFailing)
		{
			//gDebugStream << SDL_GetTicks() << " Redraw succeeded" << std::endl;
			gIsFailing = false;
			aRetryTick = 0;
		}
	}

	mFPSFlipCount++;
}

///////////////////////////// FPS Stuff
static PerfTimer gFPSTimer;
static int gFrameCount;
static int gFPSDisplay;
static bool gForceDisplay = false;
static void CalculateFPS()
{
	gFrameCount++;

	static SysFont aFont(gSexyAppBase, "Tahoma", 8);
	if (gFPSImage == NULL)
	{
		gFPSImage = new DDImage(gSexyAppBase->mDDInterface);
		gFPSImage->Create(50, aFont.GetHeight() + 4);
		gFPSImage->SetImageMode(false, false);
		gFPSImage->SetVolatile(true);
		gFPSImage->mPurgeBits = false;
		gFPSImage->mWantDDSurface = true;
		gFPSImage->PurgeBits();
	}

	if (gFPSTimer.GetDuration() >= 1000 || gForceDisplay)
	{
		gFPSTimer.Stop();
		if (!gForceDisplay)
			gFPSDisplay = (int)(gFrameCount * 1000 / gFPSTimer.GetDuration() + 0.5f);
		else
		{
			gForceDisplay = false;
			gFPSDisplay = 0;
		}

		gFPSTimer.Start();
		gFrameCount = 0;

		Graphics aDrawG(gFPSImage);
		aDrawG.SetFont(&aFont);
		SexyString aFPS = StrFormat(_S("FPS: %d"), gFPSDisplay);
		aDrawG.SetColor(0x000000);
		aDrawG.FillRect(0, 0, gFPSImage->GetWidth(), gFPSImage->GetHeight());
		aDrawG.SetColor(0xFFFFFF);
		aDrawG.DrawString(aFPS, 2, aFont.GetAscent());
		//gFPSImage->mKeepBits = false;
		//gFPSImage->GenerateDDSurface();
		gFPSImage->mBitsChangedCount++;
	}
}

///////////////////////////// FPS Stuff to draw mouse coords
static void FPSDrawCoords(int theX, int theY)
{
	static SysFont aFont(gSexyAppBase, "Tahoma", 8);
	if (gFPSImage == NULL)
	{
		gFPSImage = new DDImage(gSexyAppBase->mDDInterface);
		gFPSImage->Create(50, aFont.GetHeight() + 4);
		gFPSImage->SetImageMode(false, false);
		gFPSImage->SetVolatile(true);
		gFPSImage->mPurgeBits = false;
		gFPSImage->mWantDDSurface = true;
		gFPSImage->PurgeBits();
	}

	Graphics aDrawG(gFPSImage);
	aDrawG.SetFont(&aFont);
	SexyString aFPS = StrFormat(_S("%d,%d"), theX, theY);
	aDrawG.SetColor(0x000000);
	aDrawG.FillRect(0, 0, gFPSImage->GetWidth(), gFPSImage->GetHeight());
	aDrawG.SetColor(0xFFFFFF);
	aDrawG.DrawString(aFPS, 2, aFont.GetAscent());
	gFPSImage->mBitsChangedCount++;
}

static void UpdateScreenSaverInfo(DWORD theTick)
{
	if (!gSexyAppBase->mIsPhysWindowed)
		return;

	// Get screen saver timeout		
	static DWORD aPeriodicTick = 0;
	static DWORD aScreenSaverTimeout = 60000;
	static BOOL aScreenSaverEnabled = TRUE;

	if (theTick - aPeriodicTick > 10000)
	{
		aPeriodicTick = theTick;

		int aTimeout = 0;

		SystemParametersInfo(SPI_GETSCREENSAVETIMEOUT, 0, &aTimeout, 0);
		SystemParametersInfo(SPI_GETSCREENSAVEACTIVE, 0, &aScreenSaverEnabled, 0);
		aTimeout -= 2;

		if (aTimeout < 1)
			aTimeout = 1;

		aScreenSaverTimeout = aTimeout * 1000;

		if (!aScreenSaverEnabled)
			gScreenSaverActive = false;
	}

	// Get more accurate last user input time
	if (gGetLastInputInfoFunc)
	{
		LASTINPUTINFO anInfo;
		anInfo.cbSize = sizeof(anInfo);
		if (gGetLastInputInfoFunc(&anInfo))
		{
			if (anInfo.dwTime > theTick)
				anInfo.dwTime = theTick;

			gSexyAppBase->mLastUserInputTick = anInfo.dwTime;
		}
	}

	if (!aScreenSaverEnabled)
		return;

	DWORD anIdleTime = theTick - gSexyAppBase->mLastUserInputTick;
	if (gScreenSaverActive)
	{
		BOOL aBool = FALSE;
		if (SystemParametersInfo(SPI_GETSCREENSAVERRUNNING, 0, &aBool, 0))
		{
			if (aBool) // screen saver not off yet
				return;
		}

		if (anIdleTime < aScreenSaverTimeout)
		{
			gScreenSaverActive = false;
			gSexyAppBase->mWidgetManager->MarkAllDirty();
		}
	}
	else if (anIdleTime > aScreenSaverTimeout)
		gScreenSaverActive = true;
}

bool SexyAppBase::DrawDirtyStuff()
{
	// TODO: hack
	mSoftVSyncWait = true;
	mWaitForVSync = true;
	SEXY_AUTO_PERF("SexyAppBase::DrawDirtyStuff");
	MTAutoDisallowRand aDisallowRand;

	if (gIsFailing) // just try to reinit
	{
		Redraw(NULL);
		mHasPendingDraw = false;
		mLastDrawWasEmpty = true;
		return false;
	}

	if (mShowFPS)
	{
		switch (mShowFPSMode)
		{
		case FPS_ShowFPS: CalculateFPS(); break;
		case FPS_ShowCoords:
			if (mWidgetManager != NULL)
				FPSDrawCoords(mWidgetManager->mLastMouseX, mWidgetManager->mLastMouseY);
			break;
		}
	}

	DWORD aStartTime = timeGetTime();

	// Update user input and screen saver info
	static DWORD aPeriodicTick = 0;
	if (aStartTime - aPeriodicTick > 1000)
	{
		aPeriodicTick = aStartTime;
		UpdateScreenSaverInfo(aStartTime);
	}

	if (gScreenSaverActive)
	{
		mHasPendingDraw = false;
		mLastDrawWasEmpty = true;
		return false;
	}

	mIsDrawing = true;
	bool drewScreen = mWidgetManager->DrawScreen();
	mIsDrawing = false;

	if ((drewScreen || (aStartTime - mLastDrawTick >= 1000) || (mCustomCursorDirty)) &&
		((int)(aStartTime - mNextDrawTick) >= 0))
	{
		mLastDrawWasEmpty = false;

		mDrawCount++;

		DWORD aMidTime = timeGetTime();

		mFPSCount++;
		mFPSTime += aMidTime - aStartTime;

		mDrawTime += aMidTime - aStartTime;

		if (mShowFPS)
		{
			Graphics g(mDDInterface->GetScreenImage());
			g.DrawImage(gFPSImage, mWidth - gFPSImage->GetWidth() - 10, mHeight - gFPSImage->GetHeight() - 10);
		}

		if (mWaitForVSync && mIsPhysWindowed && mSoftVSyncWait)
		{
			DWORD aTick = timeGetTime();
			if (aTick - mLastDrawTick < mDDInterface->mMillisecondsPerFrame)
				Sleep(mDDInterface->mMillisecondsPerFrame - (aTick - mLastDrawTick));
		}

		DWORD aPreScreenBltTime = timeGetTime();
		mLastDrawTick = aPreScreenBltTime;

		Redraw(NULL);

		// This is our one UpdateFTimeAcc if we are vsynched
		UpdateFTimeAcc();

		DWORD aEndTime = timeGetTime();

		mScreenBltTime = aEndTime - aPreScreenBltTime;

#ifdef _DEBUG
		/*if (mFPSTime >= 5000) // Show FPS about every 5 seconds
		{
			ulong aTickNow = SDL_GetTicks();

			OutputDebugString(StrFormat(_S("Theoretical FPS: %d\r\n"), (int) (mFPSCount * 1000 / mFPSTime)).c_str());
			OutputDebugString(StrFormat(_S("Actual      FPS: %d\r\n"), (mFPSFlipCount * 1000) / max((aTickNow - mFPSStartTick), 1)).c_str());
			OutputDebugString(StrFormat(_S("Dirty Rate     : %d\r\n"), (mFPSDirtyCount * 1000) / max((aTickNow - mFPSStartTick), 1)).c_str());

			mFPSTime = 0;
			mFPSCount = 0;
			mFPSFlipCount = 0;
			mFPSStartTick = aTickNow;
			mFPSDirtyCount = 0;
		}*/
#endif

		if ((mLoadingThreadStarted) && (!mLoadingThreadCompleted))
		{
			int aTotalTime = aEndTime - aStartTime;

			mNextDrawTick += 35 + max(aTotalTime, 15);

			if ((int)(aEndTime - mNextDrawTick) >= 0)
				mNextDrawTick = aEndTime;

			/*char aStr[256];
			sprintf(aStr, "Next Draw Time: %d\r\n", mNextDrawTick);
			OutputDebugString(aStr);*/
		}
		else
			mNextDrawTick = aEndTime;

		mHasPendingDraw = false;
		mCustomCursorDirty = false;

		return true;
	}
	else
	{
		mHasPendingDraw = false;
		mLastDrawWasEmpty = true;
		return false;
	}
}

void SexyAppBase::BeginPopup()
{
	if (!mIsPhysWindowed)
	{
		if (mDDInterface && mDDInterface->mDD)
		{
			mDDInterface->mDD->FlipToGDISurface();
			mNoDefer = true;
		}
	}
}

void SexyAppBase::EndPopup()
{
	if (!mIsPhysWindowed)
		mNoDefer = false;

	ClearUpdateBacklog();
	ClearKeysDown();

	if (mWidgetManager->mDownButtons)
	{
		mWidgetManager->DoMouseUps();
		ReleaseCapture();
	}
}

int SexyAppBase::MsgBox(const std::string& theText, const std::string& theTitle, int theFlags)
{
	BeginPopup();
	int fl = 0;
	if (theFlags & MB_ICONERROR)
		fl |= SDL_MESSAGEBOX_INFORMATION;
	else if (theFlags & MB_ICONWARNING)
		fl |= SDL_MESSAGEBOX_WARNING;
	else
		fl |= SDL_MESSAGEBOX_ERROR;
	EndPopup();
	return IDOK;
}

int SexyAppBase::MsgBox(const std::wstring& theText, const std::wstring& theTitle, int theFlags)
{
#ifdef SDL_PLATFORM_WINDOWS
	BeginPopup();
	int aResult = MessageBoxW(mHWnd, theText.c_str(), theTitle.c_str(), theFlags);
	EndPopup();

	return aResult;
#endif
	return IDOK;
}

void SexyAppBase::Popup(const std::string& theString)
{
	BeginPopup();
	if (!mShutdown)
		::MessageBoxA(mHWnd, theString.c_str(), SexyStringToString(GetString("FATAL_ERROR", _S("FATAL ERROR"))).c_str(), MB_APPLMODAL | MB_ICONSTOP);
	EndPopup();
}

void SexyAppBase::Popup(const std::wstring& theString)
{
	BeginPopup();
	if (!mShutdown)
		::MessageBoxW(mHWnd, theString.c_str(), SexyStringToWString(GetString("FATAL_ERROR", _S("FATAL ERROR"))).c_str(), MB_APPLMODAL | MB_ICONSTOP);
	EndPopup();
}

void SexyAppBase::SafeDeleteWidget(Widget* theWidget)
{
	WidgetSafeDeleteInfo aWidgetSafeDeleteInfo;
	aWidgetSafeDeleteInfo.mUpdateAppDepth = mUpdateAppDepth;
	aWidgetSafeDeleteInfo.mWidget = theWidget;
	mSafeDeleteList.push_back(aWidgetSafeDeleteInfo);
}

BOOL CALLBACK EnumCloseThing2(HWND hwnd, LPARAM lParam)
{
	//CloseWindow(hwnd);
	char aClassName[256];
	if (GetClassNameA(hwnd, aClassName, 256) != 0)
	{
		if (strcmp(aClassName, "Internet Explorer_Server") == 0)
		{
			DestroyWindow(hwnd);
		}
		else
		{
			EnumChildWindows(hwnd, EnumCloseThing2, lParam);
		}
	}

	return TRUE;
}

BOOL CALLBACK EnumCloseThing(HWND hwnd, LPARAM lParam)
{
	// CloseWindow(hwnd);
	char aClassName[256];
	if (GetClassNameA(hwnd, aClassName, 256) != 0)
	{
		if (strcmp(aClassName, "AmWBC_WClass") == 0)
		{
			EnumChildWindows(hwnd, EnumCloseThing2, lParam);
		}
	}

	return TRUE;
}

void SexyAppBase::HandleNotifyGameMessage(int theType, int theParam)
{
	/*
	if (theType == 0) // bring to front message
	{
		WINDOWPLACEMENT aWindowPlacement;
		aWindowPlacement.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement(mHWnd, &aWindowPlacement);

		if (aWindowPlacement.showCmd == SW_SHOWMINIMIZED)
			ShowWindow(mHWnd, SW_RESTORE);

		::SetForegroundWindow(mHWnd);
	}
	*/
}

void SexyAppBase::RehupFocus()
{
	bool wantHasFocus = mActive && !mMinimized;

	if (wantHasFocus != mHasFocus)
	{
		mHasFocus = wantHasFocus;

		if (mHasFocus)
		{
			if (mMuteOnLostFocus)
				Unmute(true);

			mWidgetManager->GotFocus();
			GotFocus();
		}
		else
		{
			if (mMuteOnLostFocus)
				Mute(true);

			mWidgetManager->LostFocus();
			LostFocus();

			ReleaseCapture();
			mWidgetManager->DoMouseUps();
		}
	}
}

void SexyAppBase::ClearKeysDown()
{
	if (mWidgetManager != NULL) // fix stuck alt-key problem
	{
		for (int aKeyNum = 0; aKeyNum < 0xFF; aKeyNum++)
			mWidgetManager->mKeyDown[aKeyNum] = false;
	}
	mCtrlDown = false;
	mAltDown = false;
}

int aNumBigMoveMessages = 0;
int aNumSmallMoveMessages = 0;
int aNumTimerMessages = 0;

void SexyAppBase::ShowMemoryUsage()
{
	DWORD aTotal = 0;
	DWORD aFree = 0;

	if (mDDInterface->mDD7 != NULL)
	{
		DDSCAPS2 aCaps;
		ZeroMemory(&aCaps, sizeof(aCaps));
		aCaps.dwCaps = DDSCAPS_VIDEOMEMORY;
		mDDInterface->mDD7->GetAvailableVidMem(&aCaps, &aTotal, &aFree);
	}

	MemoryImageSet::iterator anItr = mMemoryImageSet.begin();
	typedef std::pair<int, int> FormatUsage;
	typedef std::map<PixelFormat, FormatUsage> FormatMap;
	FormatMap aFormatMap;
	int aTextureMemory = 0;
	while (anItr != mMemoryImageSet.end())
	{
		MemoryImage* aMemoryImage = *anItr;
		if (aMemoryImage->mD3DData != NULL)
		{
			TextureData* aData = (TextureData*)aMemoryImage->mD3DData;
			aTextureMemory += aData->mTexMemSize;

			FormatUsage& aUsage = aFormatMap[aData->mPixelFormat];
			aUsage.first++;
			aUsage.second += aData->mTexMemSize;
		}

		++anItr;
	}

	std::string aStr;

	const char* aDesc;
	if (Is3DAccelerationRecommended())
		aDesc = "Recommended";
	else if (Is3DAccelerationSupported())
		aDesc = "Supported";
	else
		aDesc = "Unsupported";

	aStr += StrFormat("3D-Mode is %s (3D is %s on this system)\r\n\r\n", Is3DAccelerated() ? "On" : "Off", aDesc);

	aStr += StrFormat("Num Images: %d\r\n", (int)mMemoryImageSet.size());
	aStr += StrFormat("Num Sounds: %d\r\n", mSoundManager->GetNumSounds());
	aStr += StrFormat("Video Memory: %s/%s KB\r\n", SexyStringToString(CommaSeperate((aTotal - aFree) / 1024)).c_str(), SexyStringToString(CommaSeperate(aTotal / 1024)).c_str());
	aStr += StrFormat("Texture Memory: %s KB\r\n", CommaSeperate(aTextureMemory / 1024).c_str());

	FormatUsage aUsage = aFormatMap[PixelFormat_A8R8G8B8];
	aStr += StrFormat("A8R8G8B8: %d - %s KB\r\n", aUsage.first, SexyStringToString(CommaSeperate(aUsage.second / 1024)).c_str());
	aUsage = aFormatMap[PixelFormat_A4R4G4B4];
	aStr += StrFormat("A4R4G4B4: %d - %s KB\r\n", aUsage.first, SexyStringToString(CommaSeperate(aUsage.second / 1024)).c_str());
	aUsage = aFormatMap[PixelFormat_R5G6B5];
	aStr += StrFormat("R5G6B5: %d - %s KB\r\n", aUsage.first, SexyStringToString(CommaSeperate(aUsage.second / 1024)).c_str());
	aUsage = aFormatMap[PixelFormat_Palette8];
	aStr += StrFormat("Palette8: %d - %s KB\r\n", aUsage.first, SexyStringToString(CommaSeperate(aUsage.second / 1024)).c_str());

	MsgBox(aStr, "Video Stats", MB_OK);
	mLastTime = timeGetTime();
}

bool SexyAppBase::IsAltKeyUsed(WPARAM wParam)
{
	int aChar = tolower(wParam);
	switch (aChar)
	{
	case 13: // alt-enter
	case 'r':
		return true;
	default:
		return false;
	}
}
void SexyAppBase::ShowFPS(bool show)
{
	mShowFPS = show;

	mWidgetManager->MarkAllDirty();

	if (mShowFPS)
	{
		gFPSTimer.Start();
		gFrameCount = 0;
		gFPSDisplay = 0;
		gForceDisplay = true;
	}
}

bool SexyAppBase::DebugKeyDown(SDL_Scancode theKey)
{
	if ((theKey == SDL_SCANCODE_R) && (mWidgetManager->mKeyDown[KEYCODE_MENU]))
	{
#ifndef RELEASEFINAL
		if (!ReparseModValues())
		{
			for (int aKeyNum = 0; aKeyNum < 0xFF; aKeyNum++) // prevent alt from getting stuck
				mWidgetManager->mKeyDown[aKeyNum] = false;
		}
#endif
	}
	else if (theKey == SDL_SCANCODE_F3)
	{
		if (mWidgetManager->mKeyDown[KEYCODE_SHIFT])
		{
			if (++mShowFPSMode >= Num_FPS_Types)
				mShowFPSMode = 0;
			ShowFPS(true);
		}
		else
			ShowFPS(!mShowFPS);
	}
	else if (theKey == SDL_SCANCODE_F8)
	{
		if (mWidgetManager->mKeyDown[KEYCODE_SHIFT])
		{
			Set3DAcclerated(!Is3DAccelerated());

			char aBuf[512];
			sprintf(aBuf, "3D-Mode: %s", Is3DAccelerated() ? "ON" : "OFF");
			MsgBox(aBuf, "Mode Switch", MB_OK);
			mLastTime = timeGetTime();
		}
		else
			ShowMemoryUsage();

		return true;
	}
	else if (theKey == SDL_SCANCODE_F10)
	{
#ifndef RELEASEFINAL
		if (mWidgetManager->mKeyDown[KEYCODE_CONTROL])
		{
			if (mUpdateMultiplier == 0.25)
				mUpdateMultiplier = 1.0;
			else
				mUpdateMultiplier = 0.25;
		}
		else if (mWidgetManager->mKeyDown[KEYCODE_SHIFT])
		{
			mStepMode = 0;
			ClearUpdateBacklog();
		}
		else
			mStepMode = 1;
#endif

		return true;
	}
	else if (theKey == SDL_SCANCODE_F11)
	{
		if (mWidgetManager->mKeyDown[KEYCODE_SHIFT])
			DumpProgramInfo();
		else
			TakeScreenshot();

		return true;
	}
	else if (theKey == SDL_SCANCODE_F2)
	{
		bool isPerfOn = !SexyPerf::IsPerfOn();
		if (isPerfOn)
		{
			//			MsgBox("Perf Monitoring: ON", "Perf Monitoring", MB_OK);
			ClearUpdateBacklog();
			SexyPerf::BeginPerf();
		}
		else
		{
			SexyPerf::EndPerf();
			MsgBox(SexyPerf::GetResults().c_str(), "Perf Results", MB_OK);
			ClearUpdateBacklog();
		}
	}
	else
		return false;

	return false;
}

bool SexyAppBase::DebugKeyDownAsync(int theKey, bool ctrlDown, bool altDown)
{
	return false;
}

void SexyAppBase::CloseRequestAsync()
{
}

void SexyAppBase::Done3dTesting()
{
}

// return file name that you want to upload
std::string	SexyAppBase::NotifyCrashHook()
{
	return "";
}

void SexyAppBase::MakeWindow()
{
	//OutputDebugString("MAKING WINDOW\r\n");

	if (mHWnd != NULL)
	{
		global_sexy_handle = NULL;
		HWND anOldWindow = mHWnd;
		mHWnd = NULL;
		// DestroyWindow(anOldWindow);
		mWidgetManager->mImage = NULL;
	}
	if (win != NULL) {
		SDL_DestroyWindow(win);
		win = NULL;
	}


	if ((mIsWindowed && !mFullScreenWindow))
	{
		win = SDL_CreateWindow(
			"Plants vs. Zombies",
			mWidth,
			mHeight,
			0
		);
		mHWnd = (HWND)SDL_GetPointerProperty(SDL_GetWindowProperties(win), SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL);
		if (mPreferredX == -1 && 0)
		{
			/*::MoveWindow(mHWnd,
				aDesktopRect.left + ((aDesktopRect.right - aDesktopRect.left) - aWidth) / 2,
				aDesktopRect.top + (int)(((aDesktopRect.bottom - aDesktopRect.top) - aHeight) * 0.382),
				aWidth, aHeight, FALSE);*/
		}
		mIsPhysWindowed = true;
	}
	else
	{
		win = SDL_CreateWindow(
			"Plants vs. Zombies",
			mWidth,
			mHeight,
			SDL_WINDOW_FULLSCREEN
		);
		mHWnd = (HWND)SDL_GetPointerProperty(SDL_GetWindowProperties(win), SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL);
		mIsPhysWindowed = false;
	}
	if (!win) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", "Failed to create window!", win);
		exit(0);
	}
	SDL_StartTextInput(win);

	/*char aStr[256];
	sprintf(aStr, "HWND: %d\r\n", mHWnd);
	OutputDebugString(aStr);*/

	global_sexy_handle = this;

	if (mDDInterface == NULL)
	{
		mDDInterface = new DDInterface(this);

		// Enable 3d setting
		bool is3D = false;
		bool is3DOptionSet = RegistryReadBoolean("Is3D", &is3D);
		if (is3DOptionSet)
		{
			if (mAutoEnable3D)
			{
				mAutoEnable3D = false;
				mTest3D = true;
			}

			if (is3D)
				mTest3D = true;

			mDDInterface->mIs3D = is3D;
		}
	}

	int aResult = InitDDInterface();

	if (mDDInterface->mD3DTester != NULL && mDDInterface->mD3DTester->ResultsChanged())
		RegistryEraseValue(_S("Is3D"));

	bool isActive = mActive;
	mActive = GetActiveWindow() == mHWnd;

	mPhysMinimized = false;
	if (mMinimized)
	{
		if (mMuteOnLostFocus)
			Unmute(true);

		mMinimized = false;
		isActive = mActive; // set this here so we don't call RehupFocus again.
		RehupFocus();
	}

	if (isActive != mActive)
		RehupFocus();

	ReInitImages();

	mWidgetManager->mImage = mDDInterface->GetScreenImage();
	mWidgetManager->MarkAllDirty();

	SetTimer(mHWnd, 100, mFrameTime, NULL);
}

void SexyAppBase::DeleteNativeImageData()
{
	MemoryImageSet::iterator anItr = mMemoryImageSet.begin();
	while (anItr != mMemoryImageSet.end())
	{
		MemoryImage* aMemoryImage = *anItr;
		aMemoryImage->DeleteNativeData();
		++anItr;
	}
}

void SexyAppBase::DeleteExtraImageData()
{
	AutoCrit anAutoCrit(mDDInterface->mCritSect);
	MemoryImageSet::iterator anItr = mMemoryImageSet.begin();
	while (anItr != mMemoryImageSet.end())
	{
		MemoryImage* aMemoryImage = *anItr;
		aMemoryImage->DeleteExtraBuffers();
		++anItr;
	}
}

void SexyAppBase::ReInitImages()
{
	MemoryImageSet::iterator anItr = mMemoryImageSet.begin();
	while (anItr != mMemoryImageSet.end())
	{
		MemoryImage* aMemoryImage = *anItr;
		aMemoryImage->ReInit();
		++anItr;
	}
}


void SexyAppBase::LoadingThreadProc()
{
}

void SexyAppBase::LoadingThreadCompleted()
{
}

void SexyAppBase::LoadingThreadProcStub(void* theArg)
{
	SexyAppBase* aSexyApp = (SexyAppBase*)theArg;

	aSexyApp->LoadingThreadProc();

	char aStr[256];
	sprintf(aStr, "Resource Loading Time: %i\r\n", (int)(SDL_GetTicks() - aSexyApp->mTimeLoaded));
	OutputDebugStringA(aStr);

	aSexyApp->mLoadingThreadCompleted = true;
}

void SexyAppBase::StartLoadingThread()
{
	if (!mLoadingThreadStarted)
	{
		mYieldMainThread = true;
		::SetThreadPriority(::GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
		mLoadingThreadStarted = true;
		_beginthread(LoadingThreadProcStub, 0, this);
	}
}
void SexyAppBase::CursorThreadProc()
{
	::SetThreadPriority(::GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);

	POINT aLastCursorPos = { 0, 0 };
	int aLastDrawCount = 0;

	while (!mShutdown)
	{
		//		if (mProcessInTimer)
		//			PostMessage(mHWnd,WM_TIMER,101,0);

		POINT aCursorPos;

		::GetCursorPos(&aCursorPos);
		::ScreenToClient(mHWnd, &aCursorPos);

		if (aLastDrawCount != mDrawCount)
		{
			// We did a draw so we may have committed a pending mNextCursorX/Y 
			aLastCursorPos.x = mDDInterface->mCursorX;
			aLastCursorPos.y = mDDInterface->mCursorY;
		}

		if ((aCursorPos.x != aLastCursorPos.x) ||
			(aCursorPos.y != aLastCursorPos.y))
		{
			DWORD aTimeNow = timeGetTime();
			if (aTimeNow - mNextDrawTick > mDDInterface->mMillisecondsPerFrame + 5)
			{
				// Do the special drawing if we are rendering at less than full framerate				
				mDDInterface->SetCursorPos(aCursorPos.x, aCursorPos.y);
				aLastCursorPos = aCursorPos;
			}
			else
			{
				// Set them up to get assigned in the next screen redraw
				mDDInterface->mNextCursorX = aCursorPos.x;
				mDDInterface->mNextCursorY = aCursorPos.y;
			}
		}

		Sleep(10);
	}

	mCursorThreadRunning = false;
}

void SexyAppBase::CursorThreadProcStub(void* theArg)
{
	CoInitialize(NULL);
	SexyAppBase* aSexyApp = (SexyAppBase*)theArg;
	aSexyApp->CursorThreadProc();
}

void SexyAppBase::StartCursorThread()
{
	if (!mCursorThreadRunning)
	{
		mCursorThreadRunning = true;
		::SetThreadPriority(::GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
		_beginthread(CursorThreadProcStub, 0, this);
	}
}

void SexyAppBase::SwitchScreenMode(bool wantWindowed, bool is3d, bool force)
{
	if (mForceFullscreen)
		wantWindowed = false;

	if (mIsWindowed == wantWindowed && !force)
	{
		Set3DAcclerated(is3d);
		return;
	}

	// Set 3d acceleration preference
	Set3DAcclerated(is3d,false);

	// Always make the app windowed when playing demos, in order to
	//  make it easier to track down bugs.  We place this after the
	//  sanity check just so things get re-initialized and stuff
	//if (mPlayingDemoBuffer)
	//	wantWindowed = true;

	mIsWindowed = wantWindowed;

	MakeWindow();

	// We need to do this check to allow IE to get focus instead of
	//  stealing it away for ourselves
	if (!mIsOpeningURL)
	{
		::ShowWindow(mHWnd, SW_NORMAL);
		::SetForegroundWindow(mHWnd);
	}
	else
	{
		// Show it but don't activate it
		::ShowWindow(mHWnd, SW_SHOWNOACTIVATE);
	}

	if (mSoundManager != NULL)
	{
		mSoundManager->SetCooperativeWindow(mHWnd, mIsWindowed);
	}

	mLastTime = timeGetTime();
}

void SexyAppBase::SwitchScreenMode(bool wantWindowed)
{
	SwitchScreenMode(wantWindowed, Is3DAccelerated());
}

void SexyAppBase::SwitchScreenMode()
{
	SwitchScreenMode(mIsWindowed, Is3DAccelerated(), true);
}

void SexyAppBase::SetAlphaDisabled(bool isDisabled)
{
	if (mAlphaDisabled != isDisabled)
	{
		mAlphaDisabled = isDisabled;
		mDDInterface->SetVideoOnlyDraw(mAlphaDisabled);
		mWidgetManager->mImage = mDDInterface->GetScreenImage();
		mWidgetManager->MarkAllDirty();
	}
}

void SexyAppBase::EnforceCursor()
{
	bool wantSysCursor = true;

	if (mDDInterface == NULL)
		return;

	if ((mSEHOccured) || (!mMouseIn))
	{
		::SetCursor(::LoadCursor(NULL, IDC_ARROW));
		if (mDDInterface->SetCursorImage(NULL))
			mCustomCursorDirty = true;
	}
	else
	{
		if ((mCursorImages[mCursorNum] == NULL) ||
			((!mCustomCursorsEnabled) && (mCursorNum != CURSOR_CUSTOM)))
		{
			if (mOverrideCursor != NULL)
				::SetCursor(mOverrideCursor);
			else if (mCursorNum == CURSOR_POINTER)
				::SetCursor(::LoadCursor(NULL, IDC_ARROW));
			else if (mCursorNum == CURSOR_HAND)
				::SetCursor(mHandCursor);
			else if (mCursorNum == CURSOR_TEXT)
				::SetCursor(::LoadCursor(NULL, IDC_IBEAM));
			else if (mCursorNum == CURSOR_DRAGGING)
				::SetCursor(mDraggingCursor);
			else if (mCursorNum == CURSOR_CIRCLE_SLASH)
				::SetCursor(::LoadCursor(NULL, IDC_NO));
			else if (mCursorNum == CURSOR_SIZEALL)
				::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
			else if (mCursorNum == CURSOR_SIZENESW)
				::SetCursor(::LoadCursor(NULL, IDC_SIZENESW));
			else if (mCursorNum == CURSOR_SIZENS)
				::SetCursor(::LoadCursor(NULL, IDC_SIZENS));
			else if (mCursorNum == CURSOR_SIZENWSE)
				::SetCursor(::LoadCursor(NULL, IDC_SIZENWSE));
			else if (mCursorNum == CURSOR_SIZEWE)
				::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
			else if (mCursorNum == CURSOR_WAIT)
				::SetCursor(::LoadCursor(NULL, IDC_WAIT));
			else if (mCursorNum == CURSOR_CUSTOM)
				::SetCursor(NULL); // Default to not showing anything
			else if (mCursorNum == CURSOR_NONE)
				::SetCursor(NULL);
			else
				::SetCursor(::LoadCursor(NULL, IDC_ARROW));

			if (mDDInterface->SetCursorImage(NULL))
				mCustomCursorDirty = true;
		}
		else
		{
			if (mDDInterface->SetCursorImage(mCursorImages[mCursorNum]))
				mCustomCursorDirty = true;

			::SetCursor(NULL);

			wantSysCursor = false;
		}
	}

	if (wantSysCursor != mSysCursor)
	{
		mSysCursor = wantSysCursor;
	}
}

void SexyAppBase::ProcessSafeDeleteList()
{
	MTAutoDisallowRand aDisallowRand;

	WidgetSafeDeleteList::iterator anItr = mSafeDeleteList.begin();
	while (anItr != mSafeDeleteList.end())
	{
		WidgetSafeDeleteInfo* aWidgetSafeDeleteInfo = &(*anItr);
		if (mUpdateAppDepth <= aWidgetSafeDeleteInfo->mUpdateAppDepth)
		{
			delete aWidgetSafeDeleteInfo->mWidget;
			anItr = mSafeDeleteList.erase(anItr);
		}
		else
			++anItr;
	}
}

void SexyAppBase::UpdateFTimeAcc()
{
	DWORD aCurTime = timeGetTime();

	if (mLastTimeCheck != 0)
	{
		int aDeltaTime = aCurTime - mLastTimeCheck;

		mUpdateFTimeAcc = min(mUpdateFTimeAcc + aDeltaTime, 200.0);

		if (mRelaxUpdateBacklogCount > 0)
			mRelaxUpdateBacklogCount = max(mRelaxUpdateBacklogCount - aDeltaTime, 0);
	}

	mLastTimeCheck = aCurTime;
}

//int aNumCalls = 0;
//DWORD aLastCheck = 0;

bool SexyAppBase::Process(bool allowSleep)
{
	if (mLoadingFailed)
		Shutdown();

	bool isVSynched = (mVSyncUpdates) && (!mLastDrawWasEmpty) && (!mVSyncBroken) &&
		((!mIsPhysWindowed) || (mIsPhysWindowed && mWaitForVSync && !mSoftVSyncWait));
	double aFrameFTime;
	double anUpdatesPerUpdateF;

	if (mVSyncUpdates)
	{
		aFrameFTime = (1000.0 / mSyncRefreshRate) / mUpdateMultiplier;
		anUpdatesPerUpdateF = (float)(1000.0 / (mFrameTime * mSyncRefreshRate));
	}
	else
	{
		aFrameFTime = mFrameTime / mUpdateMultiplier;
		anUpdatesPerUpdateF = 1.0;
	}

	// Make sure we're not paused
	if ((!mPaused) && (mUpdateMultiplier > 0))
	{
		ulong aStartTime = timeGetTime();

		ulong aCurTime = aStartTime;
		int aCumSleepTime = 0;

		// When we are VSynching, only calculate this FTimeAcc right after drawing

		if (!isVSynched)
			UpdateFTimeAcc();

		// mNonDrawCount is used to make sure we draw the screen at least
		// 10 times per second, even if it means we have to slow down
		// the updates to make it draw 10 times per second in "game time"

		bool didUpdate = false;

		if (mUpdateAppState == UPDATESTATE_PROCESS_1)
		{
			if ((++mNonDrawCount < (int)ceil(10 * mUpdateMultiplier)) || (!mLoaded))
			{
				bool doUpdate = false;

				if (isVSynched)
				{
					// Synch'ed to vertical refresh, so update as soon as possible after draw
					doUpdate = (!mHasPendingDraw) || (mUpdateFTimeAcc >= (int)(aFrameFTime * 0.75));
				}
				else if (mUpdateFTimeAcc >= aFrameFTime)
				{
					doUpdate = true;
				}

				if (doUpdate)
				{
					// Do VSyncBroken test.  This test fails if we're in fullscreen and
					// "don't vsync" has been forced in Advanced settings up Display Properties
					if (mUpdateMultiplier == 1.0)
					{
						mVSyncBrokenTestUpdates++;
						if (mVSyncBrokenTestUpdates >= (DWORD)((1000 + mFrameTime - 1) / mFrameTime))
						{
							// It has to be running 33% fast to be "broken" (25% = 1/0.800)
							if (aStartTime - mVSyncBrokenTestStartTick <= 800)
							{
								// The test has to fail 3 times in a row before we decide that
								//  vsync is broken overall
								mVSyncBrokenCount++;
								if (mVSyncBrokenCount >= 3)
									mVSyncBroken = true;
							}
							else
								mVSyncBrokenCount = 0;

							mVSyncBrokenTestStartTick = aStartTime;
							mVSyncBrokenTestUpdates = 0;
						}
					}

					bool hadRealUpdate = DoUpdateFrames();
					if (hadRealUpdate)
						mUpdateAppState = UPDATESTATE_PROCESS_2;

					mHasPendingDraw = true;
					didUpdate = true;
				}
			}
		}
		else if (mUpdateAppState == UPDATESTATE_PROCESS_2)
		{
			mUpdateAppState = UPDATESTATE_PROCESS_DONE;

			mPendingUpdatesAcc += anUpdatesPerUpdateF;
			mPendingUpdatesAcc -= 1.0;
			ProcessSafeDeleteList();

			// Process any extra updates
			while (mPendingUpdatesAcc >= 1.0)
			{
				++mNonDrawCount;
				bool hasRealUpdate = DoUpdateFrames();
				DBG_ASSERTE(hasRealUpdate);

				if (!hasRealUpdate)
					break;

				ProcessSafeDeleteList();
				mPendingUpdatesAcc -= 1.0;
			}

			//aNumCalls++;
			DoUpdateFramesF((float)anUpdatesPerUpdateF);
			ProcessSafeDeleteList();

			// Don't let mUpdateFTimeAcc dip below 0
			//  Subtract an extra 0.2ms, because sometimes refresh rates have some
			//  fractional component that gets truncated, and it's better to take off
			//  too much to keep our timing tending toward occuring right after 
			//  redraws
			if (isVSynched)
				mUpdateFTimeAcc = max(mUpdateFTimeAcc - aFrameFTime - 0.2f, 0.0);
			else
				mUpdateFTimeAcc -= aFrameFTime;

			if (mRelaxUpdateBacklogCount > 0)
				mUpdateFTimeAcc = 0;

			didUpdate = true;
		}

		if (!didUpdate)
		{
			mUpdateAppState = UPDATESTATE_PROCESS_DONE;

			mNonDrawCount = 0;

			if (mHasPendingDraw)
			{
				DrawDirtyStuff();
			}
			else
			{
				// Let us take into account the time it took to draw dirty stuff			
				int aTimeToNextFrame = (int)(aFrameFTime - mUpdateFTimeAcc);
				if (aTimeToNextFrame > 0)
				{
					if (!allowSleep)
						return false;

					// Wait till next processing cycle
					++mSleepCount;
					Sleep(aTimeToNextFrame);

					aCumSleepTime += aTimeToNextFrame;
				}
			}
		}

		if (mYieldMainThread)
		{
			// This is to make sure that the title screen doesn't take up any more than 
			// 1/3 of the processor time

			ulong anEndTime = timeGetTime();
			int anElapsedTime = (anEndTime - aStartTime) - aCumSleepTime;
			int aLoadingYieldSleepTime = min(250, (anElapsedTime * 2) - aCumSleepTime);

			if (aLoadingYieldSleepTime >= 0)
			{
				if (!allowSleep)
					return false;

				Sleep(aLoadingYieldSleepTime);
			}
		}
	}

	ProcessSafeDeleteList();
	return true;
}

void SexyAppBase::DoMainLoop()
{
	while (!mShutdown)
	{
		if (mExitToTop)
			mExitToTop = false;
		UpdateApp();
	}
}

Sexy::KeyCode ScancodeToSexyCode(SDL_Scancode c) {
	// TODO
	if ((int)SDL_SCANCODE_F1 <= (int)c && (int)c <= (int)SDL_SCANCODE_F24)
		return (Sexy::KeyCode)((int)c - (int)SDL_SCANCODE_F1 + (int)Sexy::KeyCode::KEYCODE_F1);
	if ((int)SDL_SCANCODE_1 <= (int)c && (int)c <= (int)SDL_SCANCODE_0)
		return (Sexy::KeyCode)((int)c - (int)SDL_SCANCODE_1 + (int)Sexy::KeyCode::KEYCODE_NUMPAD1);
	switch (c) {
	case SDL_SCANCODE_LSHIFT: {
		return Sexy::KeyCode::KEYCODE_SHIFT;
	}
	case SDL_SCANCODE_LCTRL: {
		return Sexy::KeyCode::KEYCODE_CONTROL;
	}
	case SDL_SCANCODE_LALT: {
		return Sexy::KeyCode::KEYCODE_MENU;
	}
	case SDL_SCANCODE_RETURN: {
		return Sexy::KeyCode::KEYCODE_RETURN;
	}
	case SDL_SCANCODE_TAB: {
		return Sexy::KeyCode::KEYCODE_TAB;
	}
	case SDL_SCANCODE_SPACE: {
		return Sexy::KeyCode::KEYCODE_SPACE;
	}
	case SDL_SCANCODE_ESCAPE: {
		return Sexy::KeyCode::KEYCODE_ESCAPE;
	}
	case SDL_SCANCODE_UP: {
		return Sexy::KeyCode::KEYCODE_UP;
	}
	case SDL_SCANCODE_DOWN: {
		return Sexy::KeyCode::KEYCODE_DOWN;
	}
	case SDL_SCANCODE_LEFT: {
		return Sexy::KeyCode::KEYCODE_LEFT;
	}
	case SDL_SCANCODE_RIGHT: {
		return Sexy::KeyCode::KEYCODE_RIGHT;
	}
	case SDL_SCANCODE_DELETE: {
		return Sexy::KeyCode::KEYCODE_DELETE;
	}
	case SDL_SCANCODE_INSERT: {
		return Sexy::KeyCode::KEYCODE_INSERT;
	}
	case SDL_SCANCODE_BACKSPACE: {
		return Sexy::KeyCode::KEYCODE_BACK;
	}
	}
	return Sexy::KeyCode::KEYCODE_UNKNOWN;
}

void SexyAppBase::ProcessDeferMessage(SDL_Event* ev) {
	switch (ev->type) {
	case SDL_EVENT_MOUSE_BUTTON_DOWN:
	case SDL_EVENT_MOUSE_BUTTON_UP:
	case SDL_EVENT_MOUSE_MOTION: {
		int x, y;
		if (ev->type == SDL_EVENT_MOUSE_MOTION) {
			x = ev->motion.x;
			y = ev->motion.y;
		}
		else {
			x = ev->button.x;
			y = ev->button.y;
		}
		mWidgetManager->RemapMouse(x, y);

		mLastUserInputTick = mLastTimerTime;

		mWidgetManager->MouseMove(x, y);

		if (!mMouseIn)
		{
			mMouseIn = true;
			EnforceCursor();
		}

		if (ev->type == SDL_EVENT_MOUSE_BUTTON_DOWN && ev->button.button == SDL_BUTTON_LEFT) {
			mWidgetManager->MouseDown(x, y, 1);
		}
		else if (ev->type == SDL_EVENT_MOUSE_BUTTON_DOWN && ev->button.button == SDL_BUTTON_RIGHT) {
			mWidgetManager->MouseDown(x, y, -1);
		}
		else if (ev->type == SDL_EVENT_MOUSE_BUTTON_DOWN && ev->button.button == SDL_BUTTON_MIDDLE) {
			mWidgetManager->MouseDown(x, y, 3);
		}
		else if (ev->type == SDL_EVENT_MOUSE_BUTTON_UP && ev->button.button == SDL_BUTTON_LEFT) {
			mWidgetManager->MouseUp(x, y, 1);
		}
		else if (ev->type == SDL_EVENT_MOUSE_BUTTON_UP && ev->button.button == SDL_BUTTON_RIGHT) {
			mWidgetManager->MouseUp(x, y, -1);
		}
		else if (ev->type == SDL_EVENT_MOUSE_BUTTON_UP && ev->button.button == SDL_BUTTON_MIDDLE) {
			mWidgetManager->MouseUp(x, y, 3);
		}
		break;
	}
	case SDL_EVENT_MOUSE_WHEEL: {
		mWidgetManager->MouseWheel((int)ev->wheel.y);
		break;
	}
	case SDL_EVENT_KEY_DOWN: {
		mLastUserInputTick = mLastTimerTime;

		if (ev->key.scancode == SDL_SCANCODE_RETURN && gSexyAppBase->mAltDown && !mForceFullscreen && !mForceWindowed)
		{
			SwitchScreenMode(!mIsWindowed);
			ClearKeysDown();
			break;
		}
		else if ((ev->key.scancode == SDL_SCANCODE_D) && (mWidgetManager != NULL) && (mWidgetManager->mKeyDown[KEYCODE_CONTROL]) && (mWidgetManager->mKeyDown[KEYCODE_MENU]))
		{
			// PlaySoundA("c:\\windows\\media\\Windows XP Menu Command.wav", NULL, SND_ASYNC);
			mDebugKeysEnabled = !mDebugKeysEnabled;
		}

		if (mDebugKeysEnabled)
		{
			if (DebugKeyDown(ev->key.scancode))
				break;
		}

		mWidgetManager->KeyDown(ScancodeToSexyCode(ev->key.scancode));
		break;
	}
	case SDL_EVENT_KEY_UP: {
		mWidgetManager->KeyUp(ScancodeToSexyCode(ev->key.scancode));
		break;
	}
	case SDL_EVENT_TEXT_INPUT: {
		size_t cid = SDL_strlen(ev->text.text);
		if (cid == 1) {
			cid--;
			if (32 <= ev->text.text[cid] && ev->text.text[cid] < 128)
				mWidgetManager->KeyChar((SexyChar)ev->text.text[cid]);
		}
		break;
	}
	case SDL_EVENT_TEXT_EDITING: {
		// mWidgetManager->KeyChar((SexyChar)ev->text.);
		break;
	}
	case SDL_EVENT_WINDOW_MOVED: {
		break;
	}
	case SDL_EVENT_WINDOW_RESIZED: {
		break;
	}
	}
}

void SexyAppBase::HandleEvent(SDL_Event* ev) {
	SexyAppBase* aSexyApp = (SexyAppBase*)global_sexy_handle;
	switch (ev->type) {
	case SDL_EVENT_WINDOW_RESIZED:
	case SDL_EVENT_WINDOW_MOVED:
	case SDL_EVENT_MOUSE_BUTTON_DOWN:
	case SDL_EVENT_MOUSE_BUTTON_UP:
	case SDL_EVENT_MOUSE_MOTION:
	case SDL_EVENT_KEY_DOWN:
	case SDL_EVENT_KEY_UP:
	case SDL_EVENT_TEXT_INPUT:
	case SDL_EVENT_QUIT:
	case SDL_EVENT_MOUSE_WHEEL: {
		bool keyDown = ev->type == SDL_EVENT_KEY_DOWN;

		if ((keyDown) || (ev->type == SDL_EVENT_KEY_UP))
		{
			if (ev->key.scancode == SDL_SCANCODE_LCTRL)
				aSexyApp->mCtrlDown = keyDown;
			if (ev->key.scancode == SDL_SCANCODE_LALT)
				aSexyApp->mAltDown = keyDown;
		}

		bool pushMessage = true;
		if (ev->type == SDL_EVENT_WINDOW_MINIMIZED || ev->type == SDL_EVENT_WINDOW_RESTORED)
		{
			aSexyApp->mPhysMinimized = ev->type == SDL_EVENT_WINDOW_MINIMIZED;
		}
		else if (ev->type == SDL_EVENT_QUIT)
		{
			aSexyApp->CloseRequestAsync();
			return;
		}
		break;
	}
	}
	ProcessDeferMessage(ev);
}

bool SexyAppBase::UpdateAppStep(bool* updated)
{
	if (updated != NULL)
		*updated = false;

	if (mExitToTop)
		return false;

	if (mUpdateAppState == UPDATESTATE_PROCESS_DONE)
		mUpdateAppState = UPDATESTATE_MESSAGES;

	mUpdateAppDepth++;

	// We update in two stages to avoid doing a Process if our loop termination
	//  condition has already been met by processing windows messages		
	if (mUpdateAppState == UPDATESTATE_MESSAGES)
	{
		SDL_Event ev;
		global_sexy_handle->mIsDisabled = false;
		while (SDL_PollEvent(&ev) && !mShutdown) {
			HandleEvent(&ev);
		}
		global_sexy_handle->mUpdateAppState = UPDATESTATE_PROCESS_1;
	}
	else
	{
		// Process changes state by itself
		if (mStepMode)
		{
			if (mStepMode == 2)
			{
				SDL_Delay(mFrameTime);
				mUpdateAppState = UPDATESTATE_PROCESS_DONE; // skip actual update until next step
			}
			else
			{
				mStepMode = 2;
				DoUpdateFrames();
				DoUpdateFramesF(1.0f);
				DrawDirtyStuff();
			}
		}
		else
		{
			int anOldUpdateCnt = mUpdateCount;
			Process();
			if (updated != NULL)
				*updated = mUpdateCount != anOldUpdateCnt;
		}
	}

	mUpdateAppDepth--;

	return true;
}

bool SexyAppBase::UpdateApp()
{
	bool updated;
	for (;;)
	{
		if (!UpdateAppStep(&updated))
			return false;
		if (updated)
			return true;
	}
}

int SexyAppBase::InitDDInterface()
{
	PreDDInterfaceInitHook();
	DeleteNativeImageData();
	int aResult = mDDInterface->Init(mHWnd, mIsPhysWindowed);
	if (DDInterface::RESULT_OK == aResult)
	{
		mScreenBounds.mX = (mWidth - mDDInterface->mWidth) / 2;
		mScreenBounds.mY = (mHeight - mDDInterface->mHeight) / 2;
		mScreenBounds.mWidth = mDDInterface->mWidth;
		mScreenBounds.mHeight = mDDInterface->mHeight;
		mWidgetManager->Resize(mScreenBounds, mDDInterface->mPresentationRect);
		PostDDInterfaceInitHook();
	}
	return aResult;
}

void SexyAppBase::PreTerminate()
{
}

void SexyAppBase::Start()
{
	if (mShutdown)
		return;

	StartCursorThread();

	if (mAutoStartLoadingThread)
		StartLoadingThread();

	SDL_ShowWindow(win);

	timeBeginPeriod(1);

	int aCount = 0;
	int aSleepCount = 0;

	DWORD aStartTime = timeGetTime();

	mRunning = true;
	mLastTime = aStartTime;
	mLastUserInputTick = aStartTime;
	mLastTimerTime = aStartTime;

	DoMainLoop();
	ProcessSafeDeleteList();

	mRunning = false;

	WaitForLoadingThread();

	char aString[256];
	sprintf(aString, "Seconds       = %g\r\n", (timeGetTime() - aStartTime) / 1000.0);
	OutputDebugStringA(aString);
	//sprintf(aString, "Count         = %d\r\n", aCount);
	//OutputDebugString(aString);
	sprintf(aString, "Sleep Count   = %d\r\n", mSleepCount);
	OutputDebugStringA(aString);
	sprintf(aString, "Update Count  = %d\r\n", mUpdateCount);
	OutputDebugStringA(aString);
	sprintf(aString, "Draw Count    = %d\r\n", mDrawCount);
	OutputDebugStringA(aString);
	sprintf(aString, "Draw Time     = %d\r\n", mDrawTime);
	OutputDebugStringA(aString);
	sprintf(aString, "Screen Blt    = %d\r\n", mScreenBltTime);
	OutputDebugStringA(aString);
	if (mDrawTime + mScreenBltTime > 0)
	{
		sprintf(aString, "Avg FPS       = %d\r\n", (mDrawCount * 1000) / (mDrawTime + mScreenBltTime));
		OutputDebugStringA(aString);
	}

	timeEndPeriod(1);

	PreTerminate();

	WriteToRegistry();
}

bool SexyAppBase::CheckSignature(const Buffer& theBuffer, const std::string& theFileName)
{
	// Add your own signature checking code here
	return false;
}

bool SexyAppBase::LoadProperties(const std::string& theFileName, bool required, bool checkSig)
{
	Buffer aBuffer;
	if (!ReadBufferFromFile(theFileName, &aBuffer))
	{
		if (!required)
			return true;
		else
		{
			Popup(GetString("UNABLE_OPEN_PROPERTIES", _S("Unable to open properties file ")) + StringToSexyString(theFileName));
			return false;
		}
	}
	if (checkSig)
	{
		if (!CheckSignature(aBuffer, theFileName))
		{
			Popup(GetString("PROPERTIES_SIG_FAILED", _S("Signature check failed on ")) + StringToSexyString(theFileName + "'"));
			return false;
		}
	}

	PropertiesParser aPropertiesParser(this);

	// Load required language-file properties
	if (!aPropertiesParser.ParsePropertiesBuffer(aBuffer))
	{
		Popup(aPropertiesParser.GetErrorText());
		return false;
	}
	else
		return true;
}

bool SexyAppBase::LoadProperties()
{
	// Load required language-file properties
	return LoadProperties("properties\\default.xml", true, false);
}

void SexyAppBase::LoadResourceManifest()
{
	if (!mResourceManager->ParseResourcesFile("properties\\resources.xml"))
		ShowResourceError(true);
}

void SexyAppBase::ShowResourceError(bool doExit)
{
	Popup(mResourceManager->GetErrorText());
	if (doExit)
		DoExit(0);
}

bool SexyAppBase::GetBoolean(const std::string& theId)
{
	StringBoolMap::iterator anItr = mBoolProperties.find(theId);
	DBG_ASSERTE(anItr != mBoolProperties.end());

	if (anItr != mBoolProperties.end())
		return anItr->second;
	else
		return false;
}

bool SexyAppBase::GetBoolean(const std::string& theId, bool theDefault)
{
	StringBoolMap::iterator anItr = mBoolProperties.find(theId);

	if (anItr != mBoolProperties.end())
		return anItr->second;
	else
		return theDefault;
}

int SexyAppBase::GetInteger(const std::string& theId)
{
	StringIntMap::iterator anItr = mIntProperties.find(theId);
	DBG_ASSERTE(anItr != mIntProperties.end());

	if (anItr != mIntProperties.end())
		return anItr->second;
	else
		return false;
}

int SexyAppBase::GetInteger(const std::string& theId, int theDefault)
{
	StringIntMap::iterator anItr = mIntProperties.find(theId);

	if (anItr != mIntProperties.end())
		return anItr->second;
	else
		return theDefault;
}

double SexyAppBase::GetDouble(const std::string& theId)
{
	StringDoubleMap::iterator anItr = mDoubleProperties.find(theId);
	DBG_ASSERTE(anItr != mDoubleProperties.end());

	if (anItr != mDoubleProperties.end())
		return anItr->second;
	else
		return false;
}

double SexyAppBase::GetDouble(const std::string& theId, double theDefault)
{
	StringDoubleMap::iterator anItr = mDoubleProperties.find(theId);

	if (anItr != mDoubleProperties.end())
		return anItr->second;
	else
		return theDefault;
}

SexyString SexyAppBase::GetString(const std::string& theId)
{
	StringWStringMap::iterator anItr = mStringProperties.find(theId);
	DBG_ASSERTE(anItr != mStringProperties.end());

	if (anItr != mStringProperties.end())
		return WStringToSexyString(anItr->second);
	else
		return _S("");
}

SexyString SexyAppBase::GetString(const std::string& theId, const SexyString& theDefault)
{
	StringWStringMap::iterator anItr = mStringProperties.find(theId);

	if (anItr != mStringProperties.end())
		return WStringToSexyString(anItr->second);
	else
		return theDefault;
}

StringVector SexyAppBase::GetStringVector(const std::string& theId)
{
	StringStringVectorMap::iterator anItr = mStringVectorProperties.find(theId);
	DBG_ASSERTE(anItr != mStringVectorProperties.end());

	if (anItr != mStringVectorProperties.end())
		return anItr->second;
	else
		return StringVector();
}

void SexyAppBase::SetString(const std::string& theId, const std::wstring& theValue)
{
	std::pair<StringWStringMap::iterator, bool> aPair = mStringProperties.insert(StringWStringMap::value_type(theId, theValue));
	if (!aPair.second) // Found it, change value
		aPair.first->second = theValue;
}


void SexyAppBase::SetBoolean(const std::string& theId, bool theValue)
{
	std::pair<StringBoolMap::iterator, bool> aPair = mBoolProperties.insert(StringBoolMap::value_type(theId, theValue));
	if (!aPair.second) // Found it, change value
		aPair.first->second = theValue;
}

void SexyAppBase::SetInteger(const std::string& theId, int theValue)
{
	std::pair<StringIntMap::iterator, bool> aPair = mIntProperties.insert(StringIntMap::value_type(theId, theValue));
	if (!aPair.second) // Found it, change value
		aPair.first->second = theValue;
}

void SexyAppBase::SetDouble(const std::string& theId, double theValue)
{
	std::pair<StringDoubleMap::iterator, bool> aPair = mDoubleProperties.insert(StringDoubleMap::value_type(theId, theValue));
	if (!aPair.second) // Found it, change value
		aPair.first->second = theValue;
}

void SexyAppBase::DoParseCmdLine()
{
	char* aCmdLine = GetCommandLineA();
	char* aCmdLinePtr = aCmdLine;
	if (aCmdLinePtr[0] == '"')
	{
		aCmdLinePtr = strchr(aCmdLinePtr + 1, '"');
		if (aCmdLinePtr != NULL)
			aCmdLinePtr++;
	}

	if (aCmdLinePtr != NULL)
	{
		aCmdLinePtr = strchr(aCmdLinePtr, ' ');
		if (aCmdLinePtr != NULL)
			ParseCmdLine(aCmdLinePtr + 1);
	}

	mCmdLineParsed = true;
}

void SexyAppBase::ParseCmdLine(const std::string& theCmdLine)
{
	std::string aCurParamName;
	std::string aCurParamValue;

	int aSpacePos = 0;
	bool inQuote = false;
	bool onValue = false;

	for (int i = 0; i < (int)theCmdLine.length(); i++)
	{
		char c = theCmdLine[i];
		bool atEnd = false;

		if (c == '"')
		{
			inQuote = !inQuote;

			if (!inQuote)
				atEnd = true;
		}
		else if ((c == ' ') && (!inQuote))
			atEnd = true;
		else if (c == '=')
			onValue = true;
		else if (onValue)
			aCurParamValue += c;
		else
			aCurParamName += c;

		if (i == theCmdLine.length() - 1)
			atEnd = true;

		if (atEnd && !aCurParamName.empty())
		{
			HandleCmdLineParam(aCurParamName, aCurParamValue);
			aCurParamName = "";
			aCurParamValue = "";
			onValue = false;
		}
	}
}

void SexyAppBase::HandleCmdLineParam(const std::string& theParamName, const std::string& theParamValue)
{if (theParamName == "-crash")
	{
		// Try to access NULL
		char* a = 0;
		*a = '!';
	}
	else if (theParamName == "-changedir")
	{
		mChangeDirTo = theParamValue;
	}
	else
	{
		Popup(GetString("INVALID_COMMANDLINE_PARAM", _S("Invalid command line parameter: ")) + StringToSexyString(theParamName));
		DoExit(0);
	}
}

void SexyAppBase::PreDisplayHook()
{
}

void SexyAppBase::PreDDInterfaceInitHook()
{
}

void SexyAppBase::PostDDInterfaceInitHook()
{
}

bool SexyAppBase::ChangeDirHook(const char* theIntendedPath)
{
	return false;
}

MusicInterface* SexyAppBase::CreateMusicInterface(HWND theWindow)
{
	/*if (mNoSoundNeeded)
		return new MusicInterface;
	else if (mWantFMod)
		return new FModMusicInterface(mInvisHWnd);
	else*/
	return new BassMusicInterface(NULL);
}

void SexyAppBase::InitPropertiesHook()
{
}

void SexyAppBase::InitHook()
{
}

void SexyAppBase::Init()
{
	mPrimaryThreadId = GetCurrentThreadId();

	if (mShutdown)
		return;

	if (gDDrawDLL == NULL || gDSoundDLL == NULL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "You need DirectX", "This application requires DirectX to run.  You can get DirectX at http://www.microsoft.com/directx", NULL);
		DoExit(0);
	}

	InitPropertiesHook();
	ReadFromRegistry();

	//  	if (CheckForVista())
	{
		HMODULE aMod;
		SHGetFolderPathFunc aFunc = (SHGetFolderPathFunc)GetSHGetFolderPath("shell32.dll", &aMod);
		if (aFunc == NULL || aMod == NULL)
			aFunc = (SHGetFolderPathFunc)GetSHGetFolderPath("shfolder.dll", &aMod);

		if (aMod != NULL)
		{
			char aPath[MAX_PATH];
			aFunc(NULL, CSIDL_COMMON_APPDATA, NULL, SHGFP_TYPE_CURRENT, aPath);

			std::string aDataPath = RemoveTrailingSlash(aPath) + "\\" + mFullCompanyName + "\\" + mProdName;
			SetAppDataFolder("savefiles\\");

			FreeLibrary(aMod);
		}
	}

	if (!mCmdLineParsed)
		DoParseCmdLine();

	if (gHInstance == NULL)
		gHInstance = (HINSTANCE)GetModuleHandle(NULL);

	// Change directory
	if (!ChangeDirHook(mChangeDirTo.c_str()))
		chdir(mChangeDirTo.c_str());

	gPakInterface->AddPakFile("main.pak");

	// Create a message we can use to talk to ourselves inter-process
	mNotifyGameMessage = RegisterWindowMessage((_S("Notify") + StringToSexyString(mProdName)).c_str());

	// Create a globally unique mutex
	mMutex = CreateMutex(NULL, TRUE, (StringToSexyString(mProdName) + _S("Mutex")).c_str());
	if (::GetLastError() == ERROR_ALREADY_EXISTS)
		HandleGameAlreadyRunning();

	mRandSeed = SDL_GetTicks();
	SRand(mRandSeed);

	srand(SDL_GetTicks());

	mIsWideWindow = sizeof(SexyChar) == sizeof(wchar_t);

	global_sexy_handle = this;

	mHandCursor = CreateCursor(gHInstance, 11, 4, 32, 32, gFingerCursorData, gFingerCursorData + sizeof(gFingerCursorData) / 2);
	mDraggingCursor = CreateCursor(gHInstance, 15, 10, 32, 32, gDraggingCursorData, gDraggingCursorData + sizeof(gDraggingCursorData) / 2);

	// Let app do something before showing window, or switching to fullscreen mode
	// NOTE: Moved call to PreDisplayHook above mIsWindowed and GetSystemsMetrics
	// checks because the checks below use values that could change in PreDisplayHook.
	// PreDisplayHook must call mWidgetManager->Resize if it changes mWidth or mHeight.
	PreDisplayHook();

	mWidgetManager->Resize(Rect(0, 0, mWidth, mHeight), Rect(0, 0, mWidth, mHeight));

	if (mFullScreenWindow) // change resoultion using ChangeDisplaySettings
	{
		EnumWindows(ChangeDisplayWindowEnumProc, 0); // record window pos
		DEVMODE dm;
		EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);

		// Switch resolutions
		if (dm.dmPelsWidth != mWidth || dm.dmPelsHeight != mHeight || (dm.dmBitsPerPel != 16 && dm.dmBitsPerPel != 32))
		{
			dm.dmPelsWidth = mWidth;
			dm.dmPelsHeight = mHeight;
			dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY;

			if (dm.dmBitsPerPel != 16 && dm.dmBitsPerPel != 32) // handle 24-bit/256 color case
			{
				dm.dmBitsPerPel = 16;
				dm.dmFields |= DM_BITSPERPEL;
			}

			if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
			{
				mFullScreenWindow = false;
				mIsWindowed = false;
			}
		}
	}

	MakeWindow();

	if (mSoundManager == NULL)
		mSoundManager = new DSoundManager(mNoSoundNeeded ? NULL : mHWnd, mWantFMod);

	SetSfxVolume(mSfxVolume);

	mMusicInterface = CreateMusicInterface(NULL);

	SetMusicVolume(mMusicVolume);

	InitHook();

	mInitialized = true;
}

void SexyAppBase::HandleGameAlreadyRunning()
{
	if (mOnlyAllowOneCopyToRun)
	{
		// Notify the other window and then shut ourselves down
		if (mNotifyGameMessage != 0)
			PostMessage(HWND_BROADCAST, mNotifyGameMessage, 0, 0);

		DoExit(0);
	}
}

void SexyAppBase::CopyToClipboard(const std::string& theString)
{
	HGLOBAL				aGlobalHandle;
	char* theData;
	WCHAR* theWData;

	if (OpenClipboard(mHWnd))
	{
		EmptyClipboard();

		aGlobalHandle = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, theString.length() + 1);
		theData = (char*)GlobalLock(aGlobalHandle);
		strcpy(theData, theString.c_str());
		GlobalUnlock(aGlobalHandle);
		SetClipboardData(CF_TEXT, aGlobalHandle);
		SetClipboardData(CF_OEMTEXT, aGlobalHandle);
		SetClipboardData(CF_LOCALE, aGlobalHandle);

		int aSize = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, theString.c_str(), theString.length(), NULL, 0);
		aGlobalHandle = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, (aSize + 1) * sizeof(WCHAR));
		theWData = (WCHAR*)GlobalLock(aGlobalHandle);
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, theString.c_str(), theString.length(), theWData, aSize);
		theWData[aSize] = '\0';
		GlobalUnlock(aGlobalHandle);
		SetClipboardData(CF_UNICODETEXT, aGlobalHandle);

		CloseClipboard();
	}
}

std::string	SexyAppBase::GetClipboard()
{
	HGLOBAL				aGlobalHandle;
	std::string			aString;

	if (OpenClipboard(mHWnd))
	{
		aGlobalHandle = GetClipboardData(CF_TEXT);
		if (aGlobalHandle != NULL)
		{
			char* theData = (char*)GlobalLock(aGlobalHandle);
			if (theData != NULL)
			{
				aString = theData;
				GlobalUnlock(aGlobalHandle);
			}
		}

		CloseClipboard();
	}

	return aString;
}

void SexyAppBase::SetCursor(int theCursorNum)
{
	mCursorNum = theCursorNum;
	EnforceCursor();
}

int SexyAppBase::GetCursor()
{
	return mCursorNum;
}

void SexyAppBase::EnableCustomCursors(bool enabled)
{
	mCustomCursorsEnabled = enabled;
	EnforceCursor();
}

Sexy::DDImage* SexyAppBase::GetImage(const std::string& theFileName, bool commitBits)
{
	ImageLib::Image* aLoadedImage = ImageLib::GetImage(theFileName, true);

	if (aLoadedImage == NULL)
		return NULL;

	DDImage* anImage = new DDImage(mDDInterface);
	anImage->mFilePath = theFileName;
	anImage->SetBits(aLoadedImage->GetBits(), aLoadedImage->GetWidth(), aLoadedImage->GetHeight(), commitBits);
	anImage->mFilePath = theFileName;
	delete aLoadedImage;

	return anImage;
}

Sexy::DDImage* SexyAppBase::CreateCrossfadeImage(Sexy::Image* theImage1, const Rect& theRect1, Sexy::Image* theImage2, const Rect& theRect2, double theFadeFactor)
{
	MemoryImage* aMemoryImage1 = dynamic_cast<MemoryImage*>(theImage1);
	MemoryImage* aMemoryImage2 = dynamic_cast<MemoryImage*>(theImage2);

	if ((aMemoryImage1 == NULL) || (aMemoryImage2 == NULL))
		return NULL;

	if ((theRect1.mX < 0) || (theRect1.mY < 0) ||
		(theRect1.mX + theRect1.mWidth > theImage1->GetWidth()) ||
		(theRect1.mY + theRect1.mHeight > theImage1->GetHeight()))
	{
		DBG_ASSERTE("Crossfade Rect1 out of bounds");
		return NULL;
	}

	if ((theRect2.mX < 0) || (theRect2.mY < 0) ||
		(theRect2.mX + theRect2.mWidth > theImage2->GetWidth()) ||
		(theRect2.mY + theRect2.mHeight > theImage2->GetHeight()))
	{
		DBG_ASSERTE("Crossfade Rect2 out of bounds");
		return NULL;
	}

	int aWidth = theRect1.mWidth;
	int aHeight = theRect1.mHeight;

	DDImage* anImage = new DDImage(mDDInterface);
	anImage->Create(aWidth, aHeight);

	ulong* aDestBits = anImage->GetBits();
	ulong* aSrcBits1 = aMemoryImage1->GetBits();
	ulong* aSrcBits2 = aMemoryImage2->GetBits();

	int aSrc1Width = aMemoryImage1->GetWidth();
	int aSrc2Width = aMemoryImage2->GetWidth();
	ulong aMult = (int)(theFadeFactor * 256);
	ulong aOMM = (256 - aMult);

	for (int y = 0; y < aHeight; y++)
	{
		ulong* s1 = &aSrcBits1[(y + theRect1.mY) * aSrc1Width + theRect1.mX];
		ulong* s2 = &aSrcBits2[(y + theRect2.mY) * aSrc2Width + theRect2.mX];
		ulong* d = &aDestBits[y * aWidth];

		for (int x = 0; x < aWidth; x++)
		{
			ulong p1 = *s1++;
			ulong p2 = *s2++;

			//p1 = 0;
			//p2 = 0xFFFFFFFF;

			*d++ =
				((((p1 & 0x000000FF) * aOMM + (p2 & 0x000000FF) * aMult) >> 8) & 0x000000FF) |
				((((p1 & 0x0000FF00) * aOMM + (p2 & 0x0000FF00) * aMult) >> 8) & 0x0000FF00) |
				((((p1 & 0x00FF0000) * aOMM + (p2 & 0x00FF0000) * aMult) >> 8) & 0x00FF0000) |
				((((p1 >> 24) * aOMM + (p2 >> 24) * aMult) << 16) & 0xFF000000);
		}
	}

	anImage->BitsChanged();

	return anImage;
}

void SexyAppBase::ColorizeImage(Image* theImage, const Color& theColor)
{
	MemoryImage* aSrcMemoryImage = dynamic_cast<MemoryImage*>(theImage);

	if (aSrcMemoryImage == NULL)
		return;

	ulong* aBits;
	int aNumColors;

	if (aSrcMemoryImage->mColorTable == NULL)
	{
		aBits = aSrcMemoryImage->GetBits();
		aNumColors = theImage->GetWidth() * theImage->GetHeight();
	}
	else
	{
		aBits = aSrcMemoryImage->mColorTable;
		aNumColors = 256;
	}

	if ((theColor.mAlpha <= 255) && (theColor.mRed <= 255) &&
		(theColor.mGreen <= 255) && (theColor.mBlue <= 255))
	{
		for (int i = 0; i < aNumColors; i++)
		{
			ulong aColor = aBits[i];

			aBits[i] =
				((((aColor & 0xFF000000) >> 8) * theColor.mAlpha) & 0xFF000000) |
				((((aColor & 0x00FF0000) * theColor.mRed) >> 8) & 0x00FF0000) |
				((((aColor & 0x0000FF00) * theColor.mGreen) >> 8) & 0x0000FF00) |
				((((aColor & 0x000000FF) * theColor.mBlue) >> 8) & 0x000000FF);
		}
	}
	else
	{
		for (int i = 0; i < aNumColors; i++)
		{
			ulong aColor = aBits[i];

			int aAlpha = ((aColor >> 24) * theColor.mAlpha) / 255;
			int aRed = (((aColor >> 16) & 0xFF) * theColor.mRed) / 255;
			int aGreen = (((aColor >> 8) & 0xFF) * theColor.mGreen) / 255;
			int aBlue = ((aColor & 0xFF) * theColor.mBlue) / 255;

			if (aAlpha > 255)
				aAlpha = 255;
			if (aRed > 255)
				aRed = 255;
			if (aGreen > 255)
				aGreen = 255;
			if (aBlue > 255)
				aBlue = 255;

			aBits[i] = (aAlpha << 24) | (aRed << 16) | (aGreen << 8) | (aBlue);
		}
	}

	aSrcMemoryImage->BitsChanged();
}

DDImage* SexyAppBase::CreateColorizedImage(Image* theImage, const Color& theColor)
{
	MemoryImage* aSrcMemoryImage = dynamic_cast<MemoryImage*>(theImage);

	if (aSrcMemoryImage == NULL)
		return NULL;

	DDImage* anImage = new DDImage(mDDInterface);

	anImage->Create(theImage->GetWidth(), theImage->GetHeight());

	ulong* aSrcBits;
	ulong* aDestBits;
	int aNumColors;

	if (aSrcMemoryImage->mColorTable == NULL)
	{
		aSrcBits = aSrcMemoryImage->GetBits();
		aDestBits = anImage->GetBits();
		aNumColors = theImage->GetWidth() * theImage->GetHeight();
	}
	else
	{
		aSrcBits = aSrcMemoryImage->mColorTable;
		aDestBits = anImage->mColorTable = new ulong[256];
		aNumColors = 256;

		anImage->mColorIndices = new uchar[anImage->mWidth * theImage->mHeight];
		memcpy(anImage->mColorIndices, aSrcMemoryImage->mColorIndices, anImage->mWidth * theImage->mHeight);
	}

	if ((theColor.mAlpha <= 255) && (theColor.mRed <= 255) &&
		(theColor.mGreen <= 255) && (theColor.mBlue <= 255))
	{
		for (int i = 0; i < aNumColors; i++)
		{
			ulong aColor = aSrcBits[i];

			aDestBits[i] =
				((((aColor & 0xFF000000) >> 8) * theColor.mAlpha) & 0xFF000000) |
				((((aColor & 0x00FF0000) * theColor.mRed) >> 8) & 0x00FF0000) |
				((((aColor & 0x0000FF00) * theColor.mGreen) >> 8) & 0x0000FF00) |
				((((aColor & 0x000000FF) * theColor.mBlue) >> 8) & 0x000000FF);
		}
	}
	else
	{
		for (int i = 0; i < aNumColors; i++)
		{
			ulong aColor = aSrcBits[i];

			int aAlpha = ((aColor >> 24) * theColor.mAlpha) / 255;
			int aRed = (((aColor >> 16) & 0xFF) * theColor.mRed) / 255;
			int aGreen = (((aColor >> 8) & 0xFF) * theColor.mGreen) / 255;
			int aBlue = ((aColor & 0xFF) * theColor.mBlue) / 255;

			if (aAlpha > 255)
				aAlpha = 255;
			if (aRed > 255)
				aRed = 255;
			if (aGreen > 255)
				aGreen = 255;
			if (aBlue > 255)
				aBlue = 255;

			aDestBits[i] = (aAlpha << 24) | (aRed << 16) | (aGreen << 8) | (aBlue);
		}
	}

	anImage->BitsChanged();

	return anImage;
}

DDImage* SexyAppBase::CopyImage(Image* theImage, const Rect& theRect)
{
	DDImage* anImage = new DDImage(mDDInterface);

	anImage->Create(theRect.mWidth, theRect.mHeight);

	Graphics g(anImage);
	g.DrawImage(theImage, -theRect.mX, -theRect.mY);

	anImage->CopyAttributes(theImage);

	return anImage;
}

DDImage* SexyAppBase::CopyImage(Image* theImage)
{
	return CopyImage(theImage, Rect(0, 0, theImage->GetWidth(), theImage->GetHeight()));
}

void SexyAppBase::MirrorImage(Image* theImage)
{
	MemoryImage* aSrcMemoryImage = dynamic_cast<MemoryImage*>(theImage);

	ulong* aSrcBits = aSrcMemoryImage->GetBits();

	int aPhysSrcWidth = aSrcMemoryImage->mWidth;
	for (int y = 0; y < aSrcMemoryImage->mHeight; y++)
	{
		ulong* aLeftBits = aSrcBits + (y * aPhysSrcWidth);
		ulong* aRightBits = aLeftBits + (aPhysSrcWidth - 1);

		for (int x = 0; x < (aPhysSrcWidth >> 1); x++)
		{
			ulong aSwap = *aLeftBits;

			*(aLeftBits++) = *aRightBits;
			*(aRightBits--) = aSwap;
		}
	}

	aSrcMemoryImage->BitsChanged();
}

void SexyAppBase::FlipImage(Image* theImage)
{
	MemoryImage* aSrcMemoryImage = dynamic_cast<MemoryImage*>(theImage);

	ulong* aSrcBits = aSrcMemoryImage->GetBits();

	int aPhysSrcHeight = aSrcMemoryImage->mHeight;
	int aPhysSrcWidth = aSrcMemoryImage->mWidth;
	for (int x = 0; x < aPhysSrcWidth; x++)
	{
		ulong* aTopBits = aSrcBits + x;
		ulong* aBottomBits = aTopBits + (aPhysSrcWidth * (aPhysSrcHeight - 1));

		for (int y = 0; y < (aPhysSrcHeight >> 1); y++)
		{
			ulong aSwap = *aTopBits;

			*aTopBits = *aBottomBits;
			aTopBits += aPhysSrcWidth;
			*aBottomBits = aSwap;
			aBottomBits -= aPhysSrcWidth;
		}
	}

	aSrcMemoryImage->BitsChanged();
}

void SexyAppBase::RotateImageHue(Sexy::MemoryImage* theImage, int theDelta)
{
	while (theDelta < 0)
		theDelta += 256;

	int aSize = theImage->mWidth * theImage->mHeight;
	DWORD* aPtr = theImage->GetBits();
	for (int i = 0; i < aSize; i++)
	{
		DWORD aPixel = *aPtr;
		int alpha = aPixel & 0xff000000;
		int r = (aPixel >> 16) & 0xff;
		int g = (aPixel >> 8) & 0xff;
		int b = aPixel & 0xff;

		int maxval = max(r, max(g, b));
		int minval = min(r, min(g, b));
		int h = 0;
		int s = 0;
		int l = (minval + maxval) / 2;
		int delta = maxval - minval;

		if (delta != 0)
		{
			s = (delta * 256) / ((l <= 128) ? (minval + maxval) : (512 - maxval - minval));

			if (r == maxval)
				h = (g == minval ? 1280 + (((maxval - b) * 256) / delta) : 256 - (((maxval - g) * 256) / delta));
			else if (g == maxval)
				h = (b == minval ? 256 + (((maxval - r) * 256) / delta) : 768 - (((maxval - b) * 256) / delta));
			else
				h = (r == minval ? 768 + (((maxval - g) * 256) / delta) : 1280 - (((maxval - r) * 256) / delta));

			h /= 6;
		}

		h += theDelta;
		if (h >= 256)
			h -= 256;

		double v = (l < 128) ? (l * (255 + s)) / 255 :
			(l + s - l * s / 255);

		int y = (int)(2 * l - v);

		int aColorDiv = (6 * h) / 256;
		int x = (int)(y + (v - y) * ((h - (aColorDiv * 256 / 6)) * 6) / 255);
		if (x > 255)
			x = 255;

		int z = (int)(v - (v - y) * ((h - (aColorDiv * 256 / 6)) * 6) / 255);
		if (z < 0)
			z = 0;

		switch (aColorDiv)
		{
		case 0: r = (int)v; g = x; b = y; break;
		case 1: r = z; g = (int)v; b = y; break;
		case 2: r = y; g = (int)v; b = x; break;
		case 3: r = y; g = z; b = (int)v; break;
		case 4: r = x; g = y; b = (int)v; break;
		case 5: r = (int)v; g = y; b = z; break;
		default: r = (int)v; g = x; b = y; break;
		}

		*aPtr++ = alpha | (r << 16) | (g << 8) | (b);

	}

	theImage->BitsChanged();
}

ulong SexyAppBase::HSLToRGB(int h, int s, int l)
{
	int r;
	int g;
	int b;

	double v = (l < 128) ? (l * (255 + s)) / 255 :
		(l + s - l * s / 255);

	int y = (int)(2 * l - v);

	int aColorDiv = (6 * h) / 256;
	int x = (int)(y + (v - y) * ((h - (aColorDiv * 256 / 6)) * 6) / 255);
	if (x > 255)
		x = 255;

	int z = (int)(v - (v - y) * ((h - (aColorDiv * 256 / 6)) * 6) / 255);
	if (z < 0)
		z = 0;

	switch (aColorDiv)
	{
	case 0: r = (int)v; g = x; b = y; break;
	case 1: r = z; g = (int)v; b = y; break;
	case 2: r = y; g = (int)v; b = x; break;
	case 3: r = y; g = z; b = (int)v; break;
	case 4: r = x; g = y; b = (int)v; break;
	case 5: r = (int)v; g = y; b = z; break;
	default: r = (int)v; g = x; b = y; break;
	}

	return 0xFF000000 | (r << 16) | (g << 8) | (b);
}

ulong SexyAppBase::RGBToHSL(int r, int g, int b)
{
	int maxval = max(r, max(g, b));
	int minval = min(r, min(g, b));
	int hue = 0;
	int saturation = 0;
	int luminosity = (minval + maxval) / 2;
	int delta = maxval - minval;

	if (delta != 0)
	{
		saturation = (delta * 256) / ((luminosity <= 128) ? (minval + maxval) : (512 - maxval - minval));

		if (r == maxval)
			hue = (g == minval ? 1280 + (((maxval - b) * 256) / delta) : 256 - (((maxval - g) * 256) / delta));
		else if (g == maxval)
			hue = (b == minval ? 256 + (((maxval - r) * 256) / delta) : 768 - (((maxval - b) * 256) / delta));
		else
			hue = (r == minval ? 768 + (((maxval - g) * 256) / delta) : 1280 - (((maxval - r) * 256) / delta));

		hue /= 6;
	}

	return 0xFF000000 | (hue) | (saturation << 8) | (luminosity << 16);
}

void SexyAppBase::HSLToRGB(const ulong* theSource, ulong* theDest, int theSize)
{
	for (int i = 0; i < theSize; i++)
	{
		ulong src = theSource[i];
		theDest[i] = (src & 0xFF000000) | (HSLToRGB((src & 0xFF), (src >> 8) & 0xFF, (src >> 16) & 0xFF) & 0x00FFFFFF);
	}
}

void SexyAppBase::RGBToHSL(const ulong* theSource, ulong* theDest, int theSize)
{
	for (int i = 0; i < theSize; i++)
	{
		ulong src = theSource[i];
		theDest[i] = (src & 0xFF000000) | (RGBToHSL(((src >> 16) & 0xFF), (src >> 8) & 0xFF, (src & 0xFF)) & 0x00FFFFFF);
	}
}

void SexyAppBase::PrecacheAdditive(MemoryImage* theImage)
{
	theImage->GetRLAdditiveData(mDDInterface);
}

void SexyAppBase::PrecacheAlpha(MemoryImage* theImage)
{
	theImage->GetRLAlphaData();
}

void SexyAppBase::PrecacheNative(MemoryImage* theImage)
{
	theImage->GetNativeAlphaData(mDDInterface);
}


void SexyAppBase::PlaySample(int theSoundNum)
{
	if (!mSoundManager)
		return;

	SoundInstance* aSoundInstance = mSoundManager->GetSoundInstance(theSoundNum);
	if (aSoundInstance != NULL)
	{
		aSoundInstance->Play(false, true);
	}
}


void SexyAppBase::PlaySample(int theSoundNum, int thePan)
{
	if (!mSoundManager)
		return;

	SoundInstance* aSoundInstance = mSoundManager->GetSoundInstance(theSoundNum);
	if (aSoundInstance != NULL)
	{
		aSoundInstance->SetPan(thePan);
		aSoundInstance->Play(false, true);
	}
}

bool SexyAppBase::IsMuted()
{
	return mMuteCount > 0;
}

void SexyAppBase::Mute(bool autoMute)
{
	mMuteCount++;
	if (autoMute)
		mAutoMuteCount++;

	SetMusicVolume(mMusicVolume);
	SetSfxVolume(mSfxVolume);
}

void SexyAppBase::Unmute(bool autoMute)
{
	if (mMuteCount > 0)
	{
		mMuteCount--;
		if (autoMute)
			mAutoMuteCount--;
	}

	SetMusicVolume(mMusicVolume);
	SetSfxVolume(mSfxVolume);
}


double SexyAppBase::GetMusicVolume()
{
	return mMusicVolume;
}

void SexyAppBase::SetMusicVolume(double theVolume)
{
	mMusicVolume = theVolume;

	if (mMusicInterface != NULL)
		mMusicInterface->SetVolume((mMuteCount > 0) ? 0.0 : mMusicVolume);
}

double SexyAppBase::GetSfxVolume()
{
	return mSfxVolume;
}

void SexyAppBase::SetSfxVolume(double theVolume)
{
	mSfxVolume = theVolume;

	if (mSoundManager != NULL)
		mSoundManager->SetVolume((mMuteCount > 0) ? 0.0 : mSfxVolume);
}

double SexyAppBase::GetMasterVolume()
{
	return mSoundManager->GetMasterVolume();
}

void SexyAppBase::SetMasterVolume(double theMasterVolume)
{
	mSfxVolume = theMasterVolume;
	mSoundManager->SetMasterVolume(mSfxVolume);
}

void SexyAppBase::AddMemoryImage(MemoryImage* theMemoryImage)
{
	AutoCrit anAutoCrit(mDDInterface->mCritSect);
	mMemoryImageSet.insert(theMemoryImage);
}

void SexyAppBase::RemoveMemoryImage(MemoryImage* theMemoryImage)
{
	AutoCrit anAutoCrit(mDDInterface->mCritSect);
	MemoryImageSet::iterator anItr = mMemoryImageSet.find(theMemoryImage);
	if (anItr != mMemoryImageSet.end())
		mMemoryImageSet.erase(anItr);

	Remove3DData(theMemoryImage);
}

void SexyAppBase::Remove3DData(MemoryImage* theMemoryImage)
{
	if (mDDInterface)
		mDDInterface->Remove3DData(theMemoryImage);
}


bool SexyAppBase::Is3DAccelerated()
{
	return mDDInterface->mIs3D;
}

bool SexyAppBase::Is3DAccelerationSupported()
{
	return true;
	if (mDDInterface->mD3DTester)
		return mDDInterface->mD3DTester->Is3DSupported();
	else
		return false;
}

bool SexyAppBase::Is3DAccelerationRecommended()
{
	return true;
	if (mDDInterface->mD3DTester)
		return mDDInterface->mD3DTester->Is3DRecommended();
	else
		return false;
}

void SexyAppBase::Set3DAcclerated(bool is3D, bool reinit)
{
	if (mDDInterface->mIs3D == is3D)
		return;

	mUserChanged3DSetting = true;
	mDDInterface->mIs3D = is3D;

	if (reinit)
	{
		int aResult = InitDDInterface();

		if (is3D && aResult != DDInterface::RESULT_OK)
		{
			//Set3DAcclerated(false, reinit);
			return;
		}
		else if (aResult != DDInterface::RESULT_OK)
		{
			Popup(GetString("FAILED_INIT_DIRECTDRAW", _S("Failed to initialize DirectDraw: ")) + StringToSexyString(DDInterface::ResultToString(aResult) + " " + mDDInterface->mErrorString));
			DoExit(1);
		}

		ReInitImages();

		mWidgetManager->mImage = mDDInterface->GetScreenImage();
		mWidgetManager->MarkAllDirty();
	}
}

SharedImageRef SexyAppBase::GetSharedImage(const std::string& theFileName, const std::string& theVariant, bool* isNew)
{
	std::string anUpperFileName = StringToUpper(theFileName);
	std::string anUpperVariant = StringToUpper(theVariant);

	std::pair<SharedImageMap::iterator, bool> aResultPair;
	SharedImageRef aSharedImageRef;

	{
		AutoCrit anAutoCrit(mDDInterface->mCritSect);
		aResultPair = mSharedImageMap.insert(SharedImageMap::value_type(SharedImageMap::key_type(anUpperFileName, anUpperVariant), SharedImage()));
		aSharedImageRef = &aResultPair.first->second;
	}

	if (isNew != NULL)
		*isNew = aResultPair.second;

	if (aResultPair.second)
	{
		// Pass in a '!' as the first char of the file name to create a new image
		if ((theFileName.length() > 0) && (theFileName[0] == '!'))
			aSharedImageRef.mSharedImage->mImage = new DDImage(mDDInterface);
		else
			aSharedImageRef.mSharedImage->mImage = GetImage(theFileName, false);
	}

	return aSharedImageRef;
}

void SexyAppBase::CleanSharedImages()
{
	AutoCrit anAutoCrit(mDDInterface->mCritSect);

	if (mCleanupSharedImages)
	{
		// Delete shared images with reference counts of 0
		// This doesn't occur in ~SharedImageRef because sometimes we can not only access the image
		//  through the SharedImageRef returned by GetSharedImage, but also by calling GetSharedImage
		//  again with the same params -- so we can have instances where we do the 'final' deref on
		//  an image but immediately re-request it via GetSharedImage
		SharedImageMap::iterator aSharedImageItr = mSharedImageMap.begin();
		while (aSharedImageItr != mSharedImageMap.end())
		{
			SharedImage* aSharedImage = &aSharedImageItr->second;
			if (aSharedImage->mRefCount == 0)
			{
				delete aSharedImage->mImage;
				mSharedImageMap.erase(aSharedImageItr++);
			}
			else
				++aSharedImageItr;
		}

		mCleanupSharedImages = false;
	}
}
