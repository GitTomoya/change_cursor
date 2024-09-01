#include <windows.h>
#include "ColorCursor.h"

#ifdef _DEBUG
#undef THIS_FILE
#define new DEBUG_NEW
#endif

CColorCursor::CColorCursor()
{

}

CColorCursor::~CColorCursor()
{

}

HCURSOR CColorCursor::CreateCursorFromBitmap(HBITMAP hSourceBitmap,
											 COLORREF clrTransparent,
											 DWORD   xHotspot,DWORD   yHotspot)
{
	HCURSOR hRetCursor = NULL;

	do
	{
		if(NULL == hSourceBitmap)
		{
			break;
		}

		HBITMAP hAndMask = NULL;
		HBITMAP hXorMask = NULL;
		GetMaskBitmaps(hSourceBitmap,clrTransparent,hAndMask,hXorMask);
		if(NULL == hAndMask || NULL == hXorMask)
		{
			break;
		}

		ICONINFO iconinfo = {0};
		iconinfo.fIcon		= FALSE;
		iconinfo.xHotspot	= xHotspot;
		iconinfo.yHotspot	= yHotspot;
		iconinfo.hbmMask	= hAndMask;
		iconinfo.hbmColor	= hXorMask;

		hRetCursor = ::CreateIconIndirect(&iconinfo);

	}
	while(0);

	return hRetCursor;
}

void CColorCursor::GetMaskBitmaps(HBITMAP hSourceBitmap, COLORREF clrTransparent, 
								  HBITMAP &hAndMaskBitmap, HBITMAP &hXorMaskBitmap)
{
	HDC hDC					= ::GetDC(NULL);
	HDC hMainDC				= ::CreateCompatibleDC(hDC); 
	HDC hAndMaskDC			= ::CreateCompatibleDC(hDC); 
	HDC hXorMaskDC			= ::CreateCompatibleDC(hDC); 

	BITMAP bm;
	::GetObject(hSourceBitmap,sizeof(BITMAP),&bm);

	
	hAndMaskBitmap	= ::CreateCompatibleBitmap(hDC,bm.bmWidth,bm.bmHeight);
	hXorMaskBitmap	= ::CreateCompatibleBitmap(hDC,bm.bmWidth,bm.bmHeight);

	HBITMAP hOldMainBitmap = (HBITMAP)::SelectObject(hMainDC,hSourceBitmap);
	HBITMAP hOldAndMaskBitmap	= (HBITMAP)::SelectObject(hAndMaskDC,hAndMaskBitmap);
	HBITMAP hOldXorMaskBitmap	= (HBITMAP)::SelectObject(hXorMaskDC,hXorMaskBitmap);

	COLORREF MainBitPixel;
	for(int x=0;x<bm.bmWidth;++x)
	{
		for(int y=0;y<bm.bmHeight;++y)
		{
			MainBitPixel = ::GetPixel(hMainDC,x,y);
			if(MainBitPixel == clrTransparent)
			{
				::SetPixel(hAndMaskDC,x,y,RGB(255,255,255));
				::SetPixel(hXorMaskDC,x,y,RGB(0,0,0));
			}
			else
			{
				::SetPixel(hAndMaskDC,x,y,RGB(0,0,0));
				::SetPixel(hXorMaskDC,x,y,MainBitPixel);
			}
		}
	}
	
	::SelectObject(hMainDC,hOldMainBitmap);
	::SelectObject(hAndMaskDC,hOldAndMaskBitmap);
	::SelectObject(hXorMaskDC,hOldXorMaskBitmap);

	::DeleteDC(hXorMaskDC);
	::DeleteDC(hAndMaskDC);
	::DeleteDC(hMainDC);

	::ReleaseDC(NULL,hDC);
}
