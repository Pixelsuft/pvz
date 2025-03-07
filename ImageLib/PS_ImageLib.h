#ifndef __PS_IMAGELIB_H__
#define __PS_IMAGELIB_H__

#include "ImageLib.h"
#include <string>
#include <math.h>
#include <tchar.h>
#include "..\PakLib\PakInterface.h"
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#if !defined(_MSC_VER) && !defined(__uuidof)
#define __uuidof(x) (IID_ ## x)
#endif
#include <Windows.h>
#include <objidl.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

namespace ps {
	ULONG_PTR gdi_token;
	Gdiplus::GdiplusStartupInput gdi_start_input;

    class GDIWindowsStream : public IStream
    {
        long refCount;
        PFILE* stream;

    public:
        GDIWindowsStream(PFILE* _stream) : refCount(1) {
            this->stream = _stream;
        }

        virtual ~GDIWindowsStream() {
            // p_fclose(this->stream);
        }

        virtual ULONG STDMETHODCALLTYPE AddRef(void) {
            return (ULONG)InterlockedIncrement(&refCount);
        }

        virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** ppvObject) {
            if (iid == __uuidof(IUnknown) || iid == __uuidof(ISequentialStream)
                || iid == __uuidof(IStream)) {
                *ppvObject = static_cast<IStream*>(this);
                AddRef();
                return S_OK;
            }
            else {
                return E_NOINTERFACE;
            }
        }

        virtual ULONG STDMETHODCALLTYPE Release(void) {
            ULONG ret = InterlockedDecrement(&refCount);
            if (ret == 0) {
                // delete this;
                return 0;
            }
            return ret;
        }

        virtual HRESULT STDMETHODCALLTYPE Read(void* pv, ULONG cb, ULONG* pcbRead) {
            size_t read = p_fread(pv, 1, cb, this->stream);
            if (pcbRead) {
                *pcbRead = (ULONG)read;
            }
            return read == cb ? S_OK : S_FALSE;
        }

        virtual HRESULT STDMETHODCALLTYPE Write(const void* pv, ULONG cb, ULONG* pcbWritten) {
            /*
            size_t written = al_fwrite(fp, pv, cb);
            if (pcbWritten) {
                *pcbWritten = written;
            }
            return written == cb ? S_OK : STG_E_CANTSAVE;
            */
            return E_NOTIMPL;
        }

        virtual HRESULT STDMETHODCALLTYPE Seek(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER* plibNewPosition) {
            ULONGLONG ret = p_ftell(this->stream);
            int rs = 0;
            if (dwOrigin == STREAM_SEEK_SET) {
                ret = dlibMove.QuadPart;
                rs = p_fseek(this->stream, (long)dlibMove.QuadPart, SEEK_SET);
            }
            else if (dwOrigin == STREAM_SEEK_CUR) {
                ret += dlibMove.QuadPart;
                rs = p_fseek(this->stream, (long)dlibMove.QuadPart, SEEK_CUR);
            }
            else {
                return E_NOTIMPL;
            }
            if (plibNewPosition) {
                plibNewPosition->QuadPart = (ULONGLONG)ret;
            }
            return rs == 0 ? S_OK : E_FAIL;
        }

        virtual HRESULT STDMETHODCALLTYPE Stat(STATSTG* pstatstg, DWORD grfStatFlag) {
            // SDL_memset(pstatstg, 0, sizeof(*pstatstg));
            pstatstg->type = STGTY_STREAM;
            int pos = p_ftell(this->stream);
            if (p_fseek(this->stream, 0, SEEK_END) != 0)
                return E_FAIL;
            pstatstg->cbSize.QuadPart = (ULONGLONG)p_ftell(this->stream);
            if (p_fseek(this->stream, pos, SEEK_SET) != 0)
                return E_FAIL;
            return S_OK;
        }

        virtual HRESULT STDMETHODCALLTYPE Clone(IStream** ppstm) {
            return E_NOTIMPL;
        }

        virtual HRESULT STDMETHODCALLTYPE Commit(DWORD grfCommitFlags) {
            return E_NOTIMPL;
        }

        virtual HRESULT STDMETHODCALLTYPE CopyTo(IStream* pstm, ULARGE_INTEGER cb, ULARGE_INTEGER* pcbRead, ULARGE_INTEGER* pcbWritten) {
            return E_NOTIMPL;
        }

        virtual HRESULT STDMETHODCALLTYPE LockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType) {
            return E_NOTIMPL;
        }

        virtual HRESULT STDMETHODCALLTYPE Revert() {
            return E_NOTIMPL;
        }

        virtual HRESULT STDMETHODCALLTYPE SetSize(ULARGE_INTEGER libNewSize) {
            return E_NOTIMPL;
        }

        virtual HRESULT STDMETHODCALLTYPE UnlockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType) {
            return E_NOTIMPL;
        }
    };

	void Image_Init() {
		if (Gdiplus::GdiplusStartup(&gdi_token, &gdi_start_input, NULL) != Gdiplus::Ok) {
			MessageBoxA(0, "Failed to init gdiplus!", "Error!", MB_ICONERROR);
			exit(1);
		}
	}

	void Image_Close() {
		Gdiplus::GdiplusShutdown(gdi_token);
	}

	ImageLib::Image* Image_GetPng(const std::string& theFileName) {
        PFILE* fp;
        if ((fp = p_fopen(theFileName.c_str(), "rb")) == NULL)
            return NULL;
        GDIWindowsStream* s = new GDIWindowsStream(fp);
        if (!s) {
            p_fclose(fp);
            return NULL;
        }
        Gdiplus::Bitmap* gdi_bmp = Gdiplus::Bitmap::FromStream(s, false);
        ImageLib::Image* result = NULL;
        if (gdi_bmp) {
            const uint32_t w = gdi_bmp->GetWidth();
            const uint32_t h = gdi_bmp->GetHeight();
            const Gdiplus::PixelFormat pf = gdi_bmp->GetPixelFormat();
            Gdiplus::BitmapData* gdi_lock = new Gdiplus::BitmapData;
            Gdiplus::Rect rect(0, 0, w, h);
            Gdiplus::Status lock_ret = gdi_bmp->LockBits(&rect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, gdi_lock);
            if (lock_ret == Gdiplus::Ok) {
                result = new ImageLib::Image;
                result->mWidth = w;
                result->mHeight = h;
                result->mBits = new unsigned long[w * h];
                memcpy(result->mBits, gdi_lock->Scan0, w * h * 4);                
                gdi_bmp->UnlockBits(gdi_lock);
            }
            else {
                // MessageBoxA(0, "Fail!!!", "", 0);
            }
            delete gdi_lock;
            Gdiplus::GdiplusBase::operator delete(gdi_bmp);
        }
        else {
            // Fail
        }
        s->Release();
        p_fclose(fp);
        delete s;
        return result;
	}

    ImageLib::Image* Image_GetJpeg(const std::string& theFileName) {
        return Image_GetPng(theFileName);
    }

    ImageLib::Image* Image_GetGif(const std::string& theFileName) {
        return Image_GetPng(theFileName);
    }
}

#endif //__IMAGELIB_H__
