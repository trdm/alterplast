// This is part of the Professional User Interface Suite library.
// Copyright (C) 2001-2003 FOSS Software, Inc.
// All rights reserved.
//
// http://www.prof-uis.com
// http://www.fossware.com
// mailto:foss@fossware.com
//
// This source code can be used, modified and redistributed
// under the terms of the license agreement that is included
// in the Professional User Interface Suite package.
//
// Warranties and Disclaimers:
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND
// INCLUDING, BUT NOT LIMITED TO, WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
// IN NO EVENT WILL FOSS SOFTWARE INC. BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY OR CONSEQUENTIAL DAMAGES,
// INCLUDING DAMAGES FOR LOSS OF PROFITS, LOSS OR INACCURACY OF DATA,
// INCURRED BY ANY PERSON FROM SUCH PERSON'S USAGE OF THIS SOFTWARE
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

#if (!defined __EXT_REGISTRY_H)
#define __EXT_REGISTRY_H

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

class __PROF_UIS_API CExtRegistry
{
public:
    HKEY hKey;

	CExtRegistry()
	{
		Open();
	};
	~CExtRegistry()
	{
		Close();
	};

	static HKEY RegOpen(
		HKEY hKey,
		LPCTSTR pszSubKey,
		DWORD dwRights
		)
	{
		HKEY hSubKey;
		if( RegOpenKeyEx(
				hKey,
				pszSubKey,
				0,
				dwRights,
				&hSubKey
				) == ERROR_SUCCESS
			)
		{
			return hSubKey;
		}
		return NULL;
	};
	static void RegClose(HKEY hKey)
	{
		if( hKey )
			RegCloseKey( hKey );
	};
	static HKEY RegConnect(
		HKEY hKey, 
		LPCTSTR pszRemote
		)
	{
		HKEY hSubKey;
		if( RegConnectRegistry(
				pszRemote,
				hKey,
				&hSubKey
				) == ERROR_SUCCESS
				)
		{
			return hSubKey;
		}
		return NULL;
	}
	static HKEY RegCreate(
		HKEY hKey,
		LPCTSTR pszSubKey,
		DWORD dwRights
		)
	{
		HKEY hSubKey;
		DWORD dwDisposition;
		if( RegCreateKeyEx(
				hKey,
				pszSubKey,
				0,
				NULL,
				REG_OPTION_NON_VOLATILE,
				dwRights,
				NULL,
				&hSubKey,
				&dwDisposition
				) == ERROR_SUCCESS
			)
		{
			return hSubKey;
		}
		return NULL;
	}

	HKEY Connect(
		HKEY hNewKey,
		LPCTSTR pszRemote
		)
	{
		return
			hKey = RegConnect(hNewKey, pszRemote);
	};
	HKEY Create(
		HKEY hNewKey,
		LPCTSTR pszSubKey,
		DWORD dwRights
		)
	{
		return
			hKey = RegCreate(hNewKey, pszSubKey, dwRights);
	};
	HKEY Open(
		HKEY hNewKey = NULL
		)
	{
		return hKey = hNewKey;
	};
	HKEY Open(
		HKEY hNewKey,
		LPCTSTR pszSubKey,
		DWORD dwRights
		)
	{
		return hKey =
			RegOpen(hNewKey, pszSubKey, dwRights);
	};
	void Close()
	{
		if( hKey != NULL )
		{
			RegClose(hKey);
			hKey = NULL;
		}
	};

	static bool RegLoadString(
		HKEY hKey,
		LPCTSTR pszSubKey, LPCTSTR pszValName,
		LPTSTR pszString, DWORD dwLength
		)
	{
		ASSERT(pszString);
		dwLength *= sizeof(TCHAR);
		HKEY hSubKey = pszSubKey ? RegOpen (hKey, pszSubKey, KEY_READ) : hKey;
		if( hSubKey )
		{
			DWORD dwType, dwRealLength;
			if( RegQueryValueEx(
					hSubKey, pszValName, 0, &dwType,
					NULL, &dwRealLength
					) == ERROR_SUCCESS
				)
			{
				if(	(dwType == REG_SZ
					|| dwType == REG_EXPAND_SZ
					|| dwType == REG_LINK
					|| dwType == REG_MULTI_SZ
					)
					&&
					dwLength >= dwRealLength
					)
				{
					if( RegQueryValueEx(
							hSubKey, pszValName, 0, NULL,
							(LPBYTE) pszString, &dwRealLength
							) == ERROR_SUCCESS
						)
					{
						if( hSubKey != hKey )
							RegClose( hSubKey );
						return true;
					}
				}
			}
			if( hSubKey != hKey )
				RegClose( hSubKey );
		}
		return false;
	};
	static bool RegLoadNewString(
		HKEY hKey,
		LPCTSTR pszSubKey, LPCTSTR pszValName,
		LPTSTR *pszString, DWORD *pdwLength
		)
	{
		ASSERT(pszString);
		HKEY hSubKey =
			pszSubKey ?
				RegOpen(hKey, pszSubKey, KEY_READ) : hKey;
		if( hSubKey )
		{
			DWORD dwType, dwRealLength;
			if (RegQueryValueEx (hSubKey, pszValName, 0, &dwType, NULL, &dwRealLength) == ERROR_SUCCESS)
			{
				if( dwType == REG_SZ
					|| dwType == REG_EXPAND_SZ
					|| dwType == REG_LINK
					|| dwType == REG_MULTI_SZ
					)
				{
					LPTSTR pszNewString = (LPTSTR)
						::malloc( dwRealLength*sizeof(TCHAR) );
					if( pszNewString )
					{
						if( RegQueryValueEx(
								hSubKey, pszValName,
								0, NULL,
								(LPBYTE) pszNewString,
								&dwRealLength
								) == ERROR_SUCCESS
							)
						{
							*pszString = pszNewString;
							if( pdwLength )
							{
								*pdwLength = dwRealLength;
							}
							if( hSubKey != hKey )
								RegClose( hSubKey );
							dwRealLength /= sizeof(TCHAR);
							return true;
						}
						::free( pszNewString );
					}
				}
			}
			if( hSubKey != hKey )
				RegClose( hSubKey );
		}
		return false;
	};
	static bool RegSaveString(
		HKEY hKey,
		LPCTSTR pszSubKey, LPCTSTR pszValName,
		LPCTSTR pszString
		)
	{
		HKEY hSubKey =
			pszSubKey ?
			RegCreate(hKey, pszSubKey, KEY_WRITE) : hKey;
		if( hSubKey )
		{
			if( RegSetValueEx(
					hSubKey,
					pszValName, 0, REG_SZ,
					(LPBYTE) pszString,
					(_tcslen(pszString) + 1)*sizeof(TCHAR)
					) == ERROR_SUCCESS
				)
			{
				if( hSubKey != hKey )
					RegClose( hSubKey );
				return true;
			}
			if( hSubKey != hKey )
				RegClose( hSubKey );
		}
		return false;
	};

	bool LoadNewString(LPCTSTR pszValName, LPTSTR *pszString, DWORD *pdwLength)
	{
		return RegLoadNewString(hKey, NULL, pszValName, pszString, pdwLength);
	};
	bool LoadNewString(LPCTSTR pszSubKey, LPCTSTR pszValName, LPTSTR *pszString, DWORD *pdwLength)
	{
		return RegLoadNewString(hKey, pszSubKey, pszValName, pszString, pdwLength);
	};
	bool LoadString(LPCTSTR pszSubKey, LPCTSTR pszValName, LPTSTR pszString, DWORD dwLength)
	{
		return RegLoadString(hKey, pszSubKey, pszValName, pszString, dwLength);
	};
	bool LoadString(LPCTSTR pszValName, LPTSTR pszString, DWORD dwLength)
	{
		return RegLoadString(hKey, NULL, pszValName, pszString, dwLength);
	};
	bool SaveString(LPCTSTR pszValName, LPCTSTR pszString)
	{
		return RegSaveString(hKey, NULL, pszValName, pszString);
	};
	bool SaveString(LPCTSTR pszSubKey, LPCTSTR pszValName, LPCTSTR pszString)
	{
		return RegSaveString(hKey, pszSubKey, pszValName, pszString);
	};

	static bool RegLoadNumber(
		HKEY hKey,
		LPCTSTR pszSubKey,
		LPCTSTR pszValName,
		DWORD *pdwNumber
		)
	{
		ASSERT(pdwNumber);
		HKEY hSubKey =
			pszSubKey ?
				RegOpen(hKey, pszSubKey, KEY_READ) : hKey;
		if (hSubKey)
		{
			DWORD dwType, dwSize;
			if( RegQueryValueEx(
					hSubKey, pszValName, 0, &dwType,
					NULL, &dwSize
					) == ERROR_SUCCESS
				)
			{
				if( dwType == REG_DWORD )
				{
					ASSERT(dwSize == sizeof (DWORD));
					if( RegQueryValueEx(
							hSubKey, pszValName, 0, NULL,
							(LPBYTE) pdwNumber, &dwSize
							) == ERROR_SUCCESS
						)
					{
						ASSERT(dwSize == sizeof (DWORD));
						if( hSubKey != hKey )
							RegClose( hSubKey );
						return true;
					}
				}
			}
			if( hSubKey != hKey )
				RegClose( hSubKey );
		}
		return false;
	};
	static bool RegSaveNumber(
		HKEY hKey,
		LPCTSTR pszSubKey,
		LPCTSTR pszValName,
		DWORD dwNumber
		)
	{
		HKEY hSubKey =
			pszSubKey ?
				RegCreate(hKey, pszSubKey, KEY_WRITE) : hKey;
		if( hSubKey )
		{
			if( RegSetValueEx(
					hSubKey, pszValName, 0, REG_DWORD,
					(LPBYTE) &dwNumber, sizeof (DWORD)
					) == ERROR_SUCCESS
				)
			{
				if( hSubKey != hKey )
					RegClose( hSubKey );
				return true;
			}
			if( hSubKey != hKey )
				RegClose( hSubKey );
		}
		return false;
	};

	bool LoadNumber(LPCTSTR pszValName, DWORD *pdwNumber)
	{
		return RegLoadNumber(hKey, NULL, pszValName, pdwNumber);
	};
	bool LoadNumber(LPCTSTR pszSubKey, LPCTSTR pszValName, DWORD *pdwNumber)
	{
		return RegLoadNumber(hKey, pszSubKey, pszValName, pdwNumber);
	};
	bool SaveNumber(LPCTSTR pszValName, DWORD dwNumber)
	{
		return RegSaveNumber(hKey, NULL, pszValName, dwNumber);
	};
	bool SaveNumber(LPCTSTR pszSubKey, LPCTSTR pszValName, DWORD dwNumber)
	{
		return RegSaveNumber(hKey, pszSubKey, pszValName, dwNumber);
	};

	static bool RegLoadBinary(
		HKEY hKey,
		LPCTSTR pszSubKey,
		LPCTSTR pszValName,
		LPBYTE pbyteData,
		DWORD dwSize
		)
	{
		ASSERT(pbyteData);
		HKEY hSubKey =
			pszSubKey ?
				RegOpen(hKey, pszSubKey, KEY_READ) : hKey;
		if( hSubKey )
		{
			DWORD dwType, dwRealSize;
			if( RegQueryValueEx(
					hSubKey, pszValName, 0, &dwType, NULL,
					&dwRealSize
					) == ERROR_SUCCESS
				)
			{
				if( dwType == REG_BINARY
					&& dwSize >= dwRealSize
					)
				{
					if( RegQueryValueEx(
							hSubKey, pszValName, 0, NULL,
							pbyteData, &dwRealSize
							) == ERROR_SUCCESS
						)
					{
						if( hSubKey != hKey )
							RegClose( hSubKey );
						return true;
					}
				}
			}
			if( hSubKey != hKey )
				RegClose( hSubKey );
		}
		return false;
	};
	static bool RegLoadNewBinary(
		HKEY hKey,
		LPCTSTR pszSubKey, LPCTSTR pszValName,
		LPBYTE *pbyteData,
		DWORD *pdwSize
		)
	{
		ASSERT(pbyteData);
		HKEY hSubKey =
			pszSubKey ?
				RegOpen(hKey, pszSubKey, KEY_READ) : hKey;
		if( hSubKey )
		{
			DWORD dwType, dwRealSize;
			if( RegQueryValueEx(
					hSubKey, pszValName,
					0, &dwType, NULL,
					&dwRealSize
					) == ERROR_SUCCESS
				)
			{
				if( dwType == REG_BINARY )
				{
					LPBYTE pbyteNewData = (LPBYTE)
						::malloc( dwRealSize*sizeof(TCHAR) );
					if( pbyteNewData )
					{
						if( RegQueryValueEx(
								hSubKey, pszValName, 0, NULL,
								pbyteNewData, &dwRealSize
								) == ERROR_SUCCESS
							)
						{
							*pbyteData = pbyteNewData;
							*pdwSize = dwRealSize;
							if( hSubKey != hKey )
								RegClose( hSubKey );
							return true;
						}
						::free( pbyteNewData );
					}
				}
			}
			if( hSubKey != hKey )
				RegClose( hSubKey );
		}
		return false;
	};
	static bool RegSaveBinary(
		HKEY hKey,
		LPCTSTR pszSubKey,
		LPCTSTR pszValName,
		const LPBYTE pbyteData,
		DWORD dwSize
		)
	{
		HKEY hSubKey =
			pszSubKey ?
				RegCreate(hKey, pszSubKey, KEY_WRITE) : hKey;
		if( hSubKey )
		{
			if( RegSetValueEx(
					hSubKey, pszValName, 0, REG_BINARY,
					pbyteData, dwSize
					) == ERROR_SUCCESS
				)
			{
				if( hSubKey != hKey )
					RegClose( hSubKey );
				return true;
			}
			if( hSubKey != hKey )
				RegClose( hSubKey );
		}
		return false;
	};

	bool LoadBinary(
		LPCTSTR pszValName,
		LPBYTE pbyteData,
		DWORD dwSize
		)
	{
		return
			RegLoadBinary(
				hKey,
				NULL,
				pszValName,
				pbyteData,
				dwSize
				);
	};
	bool LoadBinary(
		LPCTSTR pszSubKey,
		LPCTSTR pszValName,
		LPBYTE pbyteData,
		DWORD dwSize
		)
	{
		return
			RegLoadBinary(
				hKey,
				pszSubKey,
				pszValName,
				pbyteData,
				dwSize
				);
	};
	bool LoadNewBinary(
		LPCTSTR pszValName,
		LPBYTE *pbyteData,
		DWORD *pdwSize
		)
	{
		return
			RegLoadNewBinary(
				hKey,
				NULL,
				pszValName,
				pbyteData,
				pdwSize
				);
	};
	bool LoadNewBinary(
			LPCTSTR pszSubKey,
			LPCTSTR pszValName,
			LPBYTE *pbyteData,
			DWORD *pdwSize
			)
	{
		return
			RegLoadNewBinary(
				hKey,
				pszSubKey,
				pszValName,
				pbyteData,
				pdwSize
				);
	};
	bool SaveBinary(
		LPCTSTR pszValName,
		const LPBYTE pbyteData,
		DWORD dwSize
		)
	{
		return
			RegSaveBinary(
				hKey, NULL, pszValName,
				pbyteData, dwSize
				);
	};
	bool SaveBinary(
		LPCTSTR pszSubKey, LPCTSTR pszValName,
		const LPBYTE pbyteData, DWORD dwSize
		)
	{
		return
			RegSaveBinary(
				hKey, pszSubKey, pszValName,
				pbyteData, dwSize
				);
	};

	static bool RegDeleteKey(
		HKEY hKey, LPCTSTR pszSubKey, LPCTSTR pszValName
		)
	{
		HKEY hSubKey =
			pszSubKey ?
				RegOpen(hKey, pszSubKey, KEY_READ | KEY_WRITE)
				: 
				hKey;
		if( hSubKey )
		{
			if( pszValName )
			{
				if( RegDeleteValue(
						hSubKey,
						pszValName
						) == ERROR_SUCCESS
					)
				{
					if( hSubKey != hKey )
						RegClose( hSubKey );
					return true;
				}
			}
			else
			{
				if( RegDeleteSubKeys(hSubKey) )
				{
					if( hSubKey != hKey )
						RegClose (hSubKey);
					return
						::RegDeleteKey(
							hKey,
							pszSubKey
							) == ERROR_SUCCESS;
				}
			}
			if( hSubKey != hKey )
				RegClose( hSubKey );
		}
		return false;
	};
	static bool RegDeleteSubKeys(HKEY hKey)
	{
		DWORD dwSubKeyCnt, dwMaxSubKey;
		if( RegQueryInfoKey(
				hKey,
				NULL,
				NULL,
				0,
				&dwSubKeyCnt,
				&dwMaxSubKey,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL
				) == ERROR_SUCCESS
			)
		{
			if( dwSubKeyCnt )
			{
				dwMaxSubKey += 1;
				LPTSTR pszKeyName = (LPTSTR)
					::malloc( dwMaxSubKey*sizeof(TCHAR) );
				if( pszKeyName )
				{
					do
					{
						if( RegEnumKey(
								hKey,
								--dwSubKeyCnt,
								pszKeyName,
								dwMaxSubKey
								) == ERROR_SUCCESS
							)
						{
							HKEY hSubKey =
								RegOpen(
									hKey,
									pszKeyName,
									KEY_READ | KEY_WRITE
									);
							if( hSubKey )
							{
								if( RegDeleteSubKeys(hSubKey) )
								{
									RegClose( hSubKey );
									if( ::RegDeleteKey(
											hKey,
											pszKeyName
											) != ERROR_SUCCESS
										)
									{
										::free( pszKeyName );
										return false;
									}
								}
								else
								{
									RegClose( hSubKey );
									::free( pszKeyName );
									return false;
								}
							}
							else
							{
								::free( pszKeyName );
								return false;
							}
						}
						else
						{
							::free( pszKeyName );
							return false;
						}
					}
					while( dwSubKeyCnt );
					::free( pszKeyName );
				}
				else
				{
					return false;
				}
			}
			return true;
		}
		return false;
	};

	bool DeleteKey(LPCTSTR pszValName)
	{
		return RegDeleteKey(hKey, NULL, pszValName);
	};
	bool DeleteKey(
		LPCTSTR pszSubKey, LPCTSTR pszValName)
	{
		return RegDeleteKey(hKey, pszSubKey, pszValName);
	};
	bool DeleteSubKeys()
	{
		return RegDeleteSubKeys( hKey );
	};
}; // class CExtRegistry

#endif // __EXT_REGISTRY_H
