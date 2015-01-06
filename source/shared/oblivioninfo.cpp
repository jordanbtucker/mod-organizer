/*
Copyright (C) 2012 Sebastian Herbord. All rights reserved.

This file is part of Mod Organizer.

Mod Organizer is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Mod Organizer is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Mod Organizer.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "oblivioninfo.h"
#include <tchar.h>
#include <ShlObj.h>
#include "util.h"
#include <sstream>
#include "windows_error.h"
#include "error_report.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <boost/assign.hpp>

namespace MOShared {


OblivionInfo::OblivionInfo(const std::wstring &moDirectory, const std::wstring &moDataDirectory, const std::wstring &gameDirectory)
  : GameInfo(moDirectory, moDataDirectory, gameDirectory)
{
  identifyMyGamesDirectory(L"oblivion");
}

bool OblivionInfo::identifyGame(const std::wstring &searchPath)
{
  return FileExists(searchPath, L"Oblivion.exe") &&
         FileExists(searchPath, L"OblivionLauncher.exe");
}

unsigned long OblivionInfo::getBSAVersion()
{
  return 0x67;
}

std::wstring OblivionInfo::getRegPathStatic()
{
  HKEY key;
  LONG errorcode = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Software\\Bethesda Softworks\\Oblivion",
                                  0, KEY_QUERY_VALUE, &key);

  if (errorcode != ERROR_SUCCESS) {
    return std::wstring();
  }

  WCHAR temp[MAX_PATH];
  DWORD bufferSize = MAX_PATH;

  if (::RegQueryValueExW(key, L"Installed Path", nullptr, nullptr, (LPBYTE)temp, &bufferSize) == ERROR_SUCCESS) {
    return std::wstring(temp);
  } else {
    return std::wstring();
  }
}

std::wstring OblivionInfo::getInvalidationBSA()
{
  return L"Oblivion - Invalidation.bsa";
}

bool OblivionInfo::isInvalidationBSA(const std::wstring &bsaName)
{
  static LPCWSTR invalidation[] = { L"Oblivion - Invalidation.bsa", L"ArchiveInvalidationInvalidated!.bsa",
                                    L"BSARedirection.bsa", nullptr };

  for (int i = 0; invalidation[i] != nullptr; ++i) {
    if (wcscmp(bsaName.c_str(), invalidation[i]) == 0) {
      return true;
    }
  }
  return false;
}

std::vector<std::wstring> OblivionInfo::getPrimaryPlugins()
{
  return boost::assign::list_of(L"oblivion.esm");
}


std::vector<std::wstring> OblivionInfo::getVanillaBSAs()
{
  return boost::assign::list_of(L"Oblivion - Meshes.bsa")
                               (L"Oblivion - Textures - Compressed.bsa")
                               (L"Oblivion - Sounds.bsa")
                               (L"Oblivion - Voices1.bsa")
                               (L"Oblivion - Voices2.bsa")
                               (L"Oblivion - Misc.bsa");
}


std::vector<std::wstring> OblivionInfo::getDLCPlugins()
{
  return boost::assign::list_of (L"DLCShiveringIsles.esp")
                                (L"Knights.esp")
                                (L"DLCFrostcrag.esp")
                                (L"DLCSpellTomes.esp")
                                (L"DLCMehrunesRazor.esp")
                                (L"DLCOrrery.esp")
                                (L"DLCSpellTomes.esp")
                                (L"DLCThievesDen.esp")
                                (L"DLCVileLair.esp")
                                (L"DLCHorseArmor.esp")
      ;
}


std::vector<std::wstring> OblivionInfo::getSavegameAttachmentExtensions()
{
  return boost::assign::list_of(L"obse");
}


std::vector<std::wstring> OblivionInfo::getIniFileNames()
{
  return boost::assign::list_of(L"oblivion.ini")(L"oblivionprefs.ini");
}


std::wstring OblivionInfo::getSEName()
{
  return L"obse";
}


std::wstring OblivionInfo::getNexusPage(bool nmmScheme)
{
  if (nmmScheme) {
    return L"http://nmm.nexusmods.com/oblivion";
  } else {
    return L"http://www.nexusmods.com/oblivion";
  }
}


std::wstring OblivionInfo::getNexusInfoUrlStatic()
{
  return L"http://nmm.nexusmods.com/oblivion";
}


int OblivionInfo::getNexusModIDStatic()
{
  return 38277;
}

bool OblivionInfo::rerouteToProfile(const wchar_t *fileName, const wchar_t*)
{
  static LPCWSTR profileFiles[] = { L"oblivion.ini", L"oblivionprefs.ini", L"plugins.txt", nullptr };

  for (int i = 0; profileFiles[i] != nullptr; ++i) {
    if (_wcsicmp(fileName, profileFiles[i]) == 0) {
      return true;
    }
  }
  return false;
}

std::wstring OblivionInfo::getReferenceDataFile()
{
  return L"Oblivion - Meshes.bsa";
}


std::wstring OblivionInfo::getOMODExt()
{
  return L"omod";
}


} // namespace MOShared
