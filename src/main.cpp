#include <iostream>

#include <BMP/BMP.hpp>
#include <BMP/ioProcessBMP.hpp>

int main()
{
  OPENFILENAMEW ofn;

  wchar_t szFile[260];       // buffer for file name
  HANDLE hf;

  ZeroMemory(&ofn, sizeof(ofn));
  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = NULL;
  ofn.lpstrFile = szFile;
  // Set lpstrFile[0] to '\0' so that GetOpenFileName does not
  // use the contents of szFile to initialize itself.
  ofn.lpstrFile[0] = '\0';
  ofn.nMaxFile = sizeof(szFile);
  ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
  ofn.nFilterIndex = 1;
  ofn.lpstrFileTitle = NULL;
  ofn.nMaxFileTitle = 0;
  ofn.lpstrInitialDir = NULL;
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

  if (GetOpenFileNameW(&ofn) == true)
  {
    bmpconv::BMP bmp = bmpconv::readBMP(szFile);
  }
}
