//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Scraping the poker-table and providing access to the scraped data.
//  As the CScraper is low-level and quite large we created 
//  a interface SCraperAccess that provides higher-level accessors
//  like "UserHasCards()".
//  Better use that interface to access scraper-data whenever possible.
//
//******************************************************************************

#define BITMAPS_DLL_EXPORTS
#include "Bitmaps.h"

bool BitmapsAreEqual(HBITMAP HBitmapLeft, HBITMAP HBitmapRight) 
{
	if (HBitmapLeft == HBitmapRight)
		return true;

	if ((HBitmapLeft == NULL) || (HBitmapRight == NULL))
		return false;

	bool bSame = false;

	HDC hdc = GetDC(NULL);
	BITMAPINFO BitmapInfoLeft = {0};
	BITMAPINFO BitmapInfoRight = {0};

	BitmapInfoLeft.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	BitmapInfoRight.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

	if (0 != GetDIBits(hdc, HBitmapLeft, 0, 0, NULL, &BitmapInfoLeft, DIB_RGB_COLORS) &&
			0 != GetDIBits(hdc, HBitmapRight, 0, 0, NULL, &BitmapInfoRight, DIB_RGB_COLORS))
	{
		// Compare the BITMAPINFOHEADERs of the two bitmaps
		if (0 == memcmp(&BitmapInfoLeft.bmiHeader, &BitmapInfoRight.bmiHeader, sizeof(BITMAPINFOHEADER)))
		{
			// The BITMAPINFOHEADERs are the same so now compare the actual bitmap bits
			BYTE *pLeftBits = new BYTE[BitmapInfoLeft.bmiHeader.biSizeImage];
			BYTE *pRightBits = new BYTE[BitmapInfoRight.bmiHeader.biSizeImage];
			BYTE *pByteLeft = NULL;
			BYTE *pByteRight = NULL;

			PBITMAPINFO pBitmapInfoLeft = &BitmapInfoLeft;
			PBITMAPINFO pBitmapInfoRight = &BitmapInfoRight;

			// calculate the size in BYTEs of the additional
			// memory needed for the bmiColor table
			int AdditionalMemory = 0;
			switch (BitmapInfoLeft.bmiHeader.biBitCount)
			{
			case 1:
				AdditionalMemory = 1 * sizeof(RGBQUAD);
				break;
			case 4:
				AdditionalMemory = 15 * sizeof(RGBQUAD);
				break;
			case 8:
				AdditionalMemory = 255 * sizeof(RGBQUAD);
				break;
			case 16:
			case 32:
				AdditionalMemory = 2 * sizeof(RGBQUAD);
			}

			if (AdditionalMemory)
			{
				// we have to allocate room for the bmiColor table that will be
				// attached to our BITMAPINFO variables
				pByteLeft = new BYTE[sizeof(BITMAPINFO) + AdditionalMemory];
				if (pByteLeft)
				{
					memset(pByteLeft, 0, sizeof(BITMAPINFO) + AdditionalMemory);
					memcpy(pByteLeft, pBitmapInfoLeft, sizeof(BITMAPINFO));
					pBitmapInfoLeft = (PBITMAPINFO)pByteLeft;
				}

				pByteRight = new BYTE[sizeof(BITMAPINFO) + AdditionalMemory];
				if (pByteRight)
				{
					memset(pByteRight, 0, sizeof(BITMAPINFO) + AdditionalMemory);
					memcpy(pByteRight, pBitmapInfoRight, sizeof(BITMAPINFO));
					pBitmapInfoRight = (PBITMAPINFO)pByteRight;
				}
			}

			if (pLeftBits && pRightBits && pBitmapInfoLeft && pBitmapInfoRight)
			{
				// zero out the bitmap bit buffers
				memset(pLeftBits, 0, BitmapInfoLeft.bmiHeader.biSizeImage);
				memset(pRightBits, 0, BitmapInfoRight.bmiHeader.biSizeImage);

				// fill the bit buffers with the actual bitmap bits
				if (0 != GetDIBits(hdc, HBitmapLeft, 0, pBitmapInfoLeft->bmiHeader.biHeight, pLeftBits, pBitmapInfoLeft, DIB_RGB_COLORS) &&
						0 != GetDIBits(hdc, HBitmapRight, 0, pBitmapInfoRight->bmiHeader.biHeight, pRightBits, pBitmapInfoRight, DIB_RGB_COLORS))
				{
					// compare the actual bitmap bits of the two bitmaps
					bSame = (0 == memcmp(pLeftBits, pRightBits, pBitmapInfoLeft->bmiHeader.biSizeImage));
				}
			}

			// clean up
			delete[] pLeftBits;
			delete[] pRightBits;
			delete[] pByteLeft;
			delete[] pByteRight;
		}
	}
	ReleaseDC(NULL, hdc);

	return bSame;
}

void SaveBitmapToFile(HBITMAP bitmap, char* path) {
  HDC					hdcScreen = CreateDC("DISPLAY", NULL, NULL, NULL);
  HDC					hdcCompatible = CreateCompatibleDC(hdcScreen);
  PBITMAPINFO	pbmi = NULL;
  HANDLE			hf = NULL; // file handle
  BITMAPFILEHEADER	hdr; // bitmap file-header
  PBITMAPINFOHEADER	pbih = NULL; // bitmap info-header
  LPBYTE			lpBits = NULL; // memory pointer
  DWORD				dwTotal = 0; // total count of bytes
  DWORD				cb = 0; // incremental count of bytes
  BYTE				*hp = NULL; // byte pointer
  DWORD				dwTmp = 0;
  BITMAP			bmp;

  memset(&bmp, 0, sizeof(BITMAP));
  GetObject(bitmap, sizeof(BITMAP), &bmp);
  memset(&hdr, 0, sizeof(hdr));
  int cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
  if (cClrBits>8) {
    // No Palette (normally)
    pbmi = (PBITMAPINFO)calloc(1, sizeof(BITMAPINFOHEADER));
  }
  else {
    pbmi = (PBITMAPINFO)calloc(1, sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1 << (min(8, cClrBits))));
    pbmi->bmiHeader.biClrUsed = (1 << cClrBits);
  }
  // Initialize the fields in the BITMAPINFO structure.
  pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  // Retrieve the color table (RGBQUAD array) and the bits
  // (array of palette indices) from the DIB.
  if (!GetDIBits(hdcCompatible, bitmap, 0, bmp.bmHeight, NULL, pbmi, DIB_RGB_COLORS)) {
    goto to_return;
  }
  pbih = &(pbmi->bmiHeader);
  pbmi->bmiHeader.biCompression = BI_RGB;
  lpBits = (LPBYTE)calloc(1, pbih->biSizeImage);
  if (!lpBits) {
    goto to_return;
  }
  if (!GetDIBits(hdcCompatible, bitmap, 0, (WORD)pbih->biHeight, lpBits, pbmi, DIB_RGB_COLORS)) {
    goto to_return;
  }
  // Create the .BMP file.
  hf = CreateFile(path, GENERIC_READ | GENERIC_WRITE, (DWORD)0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);
  if (hf == INVALID_HANDLE_VALUE) {
    goto to_return;
  }
  hdr.bfType = 0x4d42; // 0x42 = "B" 0x4d = "M"
                       // Compute the size of the entire file.
  hdr.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed * sizeof(RGBQUAD) + pbih->biSizeImage);
  hdr.bfReserved1 = 0;
  hdr.bfReserved2 = 0;
  // Compute the offset to the array of color indices.
  hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed * sizeof(RGBQUAD);
  // Copy the BITMAPFILEHEADER into the .BMP file.
  if (!WriteFile(hf, (LPVOID)&hdr, sizeof(BITMAPFILEHEADER), (LPDWORD)&dwTmp, NULL)) {
    goto to_return;
  }
  // Copy the BITMAPINFOHEADER and RGBQUAD array into the file.
  if (!WriteFile(hf, (LPVOID)pbih, sizeof(BITMAPINFOHEADER) + pbih->biClrUsed * sizeof(RGBQUAD), (LPDWORD)&dwTmp, (NULL))) {
    goto to_return;
  }
  // Copy the array of color indices into the .BMP file.
  dwTotal = cb = pbih->biSizeImage;
  hp = lpBits;
  if (!WriteFile(hf, (LPSTR)hp, (int)cb, (LPDWORD)&dwTmp, NULL)) {
    goto to_return;
  }
  // Close the .BMP file.
  if (!CloseHandle(hf)) {
    goto to_return;
  }
  to_return:
  ;
  // Free memory.
  if (pbmi)free(pbmi);
  if (lpBits) {
    free(lpBits);
  }
  DeleteDC(hdcCompatible);
  DeleteDC(hdcScreen);
}

void TakeScreenshot(HWND in_window, HBITMAP *out_bitmap) {
  // Bitblt the attached windows bitmap into a HDC
  //HDC hdcScreen = CreateDC("DISPLAY", NULL, NULL, NULL);
  HDC hdcScreen = GetDC(in_window);
  HDC hdcCompat = CreateCompatibleDC(hdcScreen);
  RECT rect;
  GetClientRect(in_window, &rect);
  //!!!leak
  // https://docs.microsoft.com/en-us/windows/desktop/api/wingdi/nf-wingdi-createcompatiblebitmap
  *out_bitmap = CreateCompatibleBitmap(hdcScreen, rect.right, rect.bottom);
  SelectObject(hdcCompat, *out_bitmap);
  //BitBlt(*out_bitmap, 0, 0, rect.right, rect.bottom, hdcScreen/*???*/, 0, 0, SRCCOPY);
  
  BitBlt(hdcCompat, 0, 0, rect.right, rect.bottom, hdcScreen/*???*/, 0, 0, SRCCOPY);
  //!!! temp
  SaveBitmapToFile(*out_bitmap, "debug.bmp");
  //SelectObject(hdcCompat, old_bitmap);
  //DeleteObject(attached_bitmap);
  DeleteDC(hdcCompat);
  DeleteDC(hdcScreen);
  //ReleaseDC(in_window,);
}


void ExtractSubImage(HBITMAP source_image, HBITMAP *destination_image,
  CRect subimage_position) {
  MessageBox(0, "ExtractSubImage", "ExtractSubImage", 0);
  // https://stackoverflow.com/questions/5687263/copying-a-bitmap-from-another-hbitmap 
  HDC dc_source = CreateCompatibleDC(NULL);
  SelectObject(dc_source, source_image);
  HDC dc_destination = CreateCompatibleDC(NULL);
  SelectObject(dc_destination, destination_image);
  // copy bitmap data
  int width = subimage_position.right - subimage_position.left + 1;
  int height = subimage_position.bottom - subimage_position.top + 1);
  BitBlt(dc_destination, 0, 0, 
    width,
    height,
    dc_source,
    subimage_position.left,
    subimage_position.top,
    SRCCOPY);
  MessageBox(0, "ExtractSubImage done", "ExtractSubImage", 0);
}


void temp(HBITMAP source_image, HBITMAP *destination_image,
  CRect subimage_position)
{
  /*#write_log(Preferences()->debug_scraper(),
  "[CScraper] ProcessRegion %s (%i, %i, %i, %i)\n",
  r_iter->first, r_iter->second.left, r_iter->second.top,
  r_iter->second.right, r_iter->second.bottom);
  write_log(Preferences()->debug_scraper(),
  "[CScraper] ProcessRegion color %i radius %i transform %s\n",
  r_iter->second.color, r_iter->second.radius, r_iter->second.transform);
  */
  HDC				hdc = GetDC(_connected_window);
HDC				hdcScreen = CreateDC("DISPLAY", NULL, NULL, NULL);
HDC				hdcCompatible = CreateCompatibleDC(hdcScreen);
// Get "current" bitmap
///old_bitmap = (HBITMAP)SelectObject(hdcCompatible, r_iter->second.cur_bmp);
BitBlt(hdcCompatible, 0, 0,
  subimage_position.right - subimage_position + 1,
  subimage_position.bottom - subimage_position.top + 1,
  hdc,
  r_iter->second.left,
  r_iter->second.top,
  SRCCOPY);
///SelectObject(hdcCompatible, old_bitmap);
// If the bitmaps are different, then continue on
/*#if (BitmapsAreEqual(r_iter->second.last_bmp, r_iter->second.cur_bmp)) {
MessageBox(0, "Bitmaps equal", "ProcessRegion", 0);
} else {*/
/*#MessageBox(0, "Bitmaps not equal", "ProcessRegion", 0);
// Copy into "last" bitmap
old_bitmap = (HBITMAP)SelectObject(hdcCompatible, r_iter->second.last_bmp);
BitBlt(hdcCompatible, 0, 0, r_iter->second.right - r_iter->second.left + 1,
r_iter->second.bottom - r_iter->second.top + 1,
hdc, r_iter->second.left, r_iter->second.top, SRCCOPY);
SelectObject(hdcCompatible, old_bitmap);*/
///}
DeleteDC(hdcCompatible);
DeleteDC(hdcScreen);
ReleaseDC(_connected_window, hdc);
///  return bitmaps_equal;