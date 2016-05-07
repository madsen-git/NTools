// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

#include "stdafx.h"

#include <afxctl.h>
//#include "d:/VSTOOLS/AC/ANormal.h"

#include "picture.h"
#include <afxpriv2.h>
#include <afxole.h>

namespace acm {
CPicture::CPicture()	
{
	//wchar_t temp[MAX_PATH] = {0};
	//std::swprintf(temp, MAX_PATH, L"%s", acg::CAApp::getAppPath().c_str());
	std::wostringstream out;
	out << acg::CAApp::getAppPath() << L"\\YOYA";
	m_strRarPath = out.str().c_str();
	out << L"\\";
	m_strTempPic = out.str().c_str();
	m_strRarPath = out.str().c_str();
	m_strTempPic += DEF_PICTURE_TEMP_NAME;
	m_strRarPath += DEF_PICTURE_RAR_NAME;
	////TCHAR str[MAX_PATH];
	////std::wstring path = acg::CAApp::getAppPath();
	//std::wstring path = acg::CAApp::getAppPath();
	////DWORD len = ::GetSystemDirectory(str, MAX_PATH);
	////str[len] = '\0';
	////m_strRarPath.Format(_T("%s\\YOYA"), str);
	//path += L"\\YOYA";
	//
	//m_strTempPic.Format(L"%s\\temp051020axc.tmp", m_strRarPath);
	//m_strRarPic.Format(L"%s\\rar051020byd.rxp", m_strRarPath);

}		

/////////////////////////////////////////////////////////////////////////////
// CPicture properties

long CPicture::GetHandle()
{
	long result;
	GetProperty(0x0, VT_I4, (void*)&result);
	return result;
}

long CPicture::GetHPal()
{
	long result;
	GetProperty(0x2, VT_I4, (void*)&result);
	return result;
}

void CPicture::SetHPal(long propVal)
{
	SetProperty(0x2, VT_I4, propVal);
}

short CPicture::GetType()
{
	short result;
	GetProperty(0x3, VT_I2, (void*)&result);
	return result;
}

long CPicture::GetWidth()
{
	long result;
	GetProperty(0x4, VT_I4, (void*)&result);
	return result;
}

long CPicture::GetHeight()
{
	long result;
	GetProperty(0x5, VT_I4, (void*)&result);
	return result;
}

/////////////////////////////////////////////////////////////////////////////
// CPicture operations
//////////////////
// Load from resource. Looks for "IMAGE" type.
//
BOOL CPicture::Load(UINT nIDRes)
{
	// find resource in resource file
	HINSTANCE hInst = AfxGetResourceHandle();
	HRSRC hRsrc = ::FindResource(hInst,
		MAKEINTRESOURCE(nIDRes),
		_T("IMAGE")); // type _T("IMAGE")
	if (!hRsrc)
		return FALSE;

	// load resource into memory
	DWORD len = SizeofResource(hInst, hRsrc);
	BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);
	if (!lpRsrc)
		return FALSE;
	// create memory file and load it
	CMemFile file(lpRsrc, len);
	BOOL bRet = Load(file);
	FreeResource(hRsrc);
	GlobalFree(lpRsrc);
	return bRet;
}

//////////////////
// Load from path name.
//
BOOL CPicture::Load(LPCTSTR pszPathName)
{
	if(m_spIPicture != NULL)
		Free();
	if(pszPathName == NULL || ::lstrlen(pszPathName) <= 0)
	{
		return FALSE;
	}
	CFile file;
	if (!file.Open(pszPathName, CFile::modeRead|CFile::shareDenyWrite))
		return FALSE;
	BOOL bRet = Load(file);
	file.Close();
	return bRet;
}

//////////////////
// Load from CFile
//
BOOL CPicture::Load(CFile& vfile)
{
	//if(file.Open(m_strTempPic, CFile::modeCreate|CFile::modeWrite|CFile::modeNoInherit|CFile::typeBinary))
	// copy for temp file
	DWORD len = DWORD(vfile.GetLength());
	CFile file;
	if(file.Open(m_strTempPic, CFile::modeCreate|CFile::modeWrite|CFile::modeNoInherit|CFile::typeBinary))
	{
		BYTE *pv = new BYTE[len+1];
		memset(pv, 0, len+1);
		vfile.Read(pv, len);
		pv[len] = '\0';
		file.Write(pv, len);
		file.Close();
		delete[] pv;
	}
	
	vfile.SeekToBegin();
	//DoSaveTempfile(file);
	//file.SeekToBegin();
	// end copy
	CArchive ar(&vfile, CArchive::load | CArchive::bNoFlushOnDelete);
	return Load(ar);
}

//////////////////
// Load from archive--create stream and load from stream.
//
BOOL CPicture::Load(CArchive& ar)
{
	CArchiveStream arcstream(&ar);
	return Load((IStream*)&arcstream);
}

//////////////////
// Load from stream (IStream). This is the one that really does it: call
// OleLoadPicture to do the work.
//
BOOL CPicture::Load(IStream* pstm)
{
	// 
	
	HRESULT hr = OleLoadPicture(pstm, 0, FALSE,
		IID_IPicture, (void**)&m_spIPicture);
	ASSERT(SUCCEEDED(hr) && m_spIPicture);
	
	return TRUE;
}

// DoLoadFromVariant
BOOL CPicture::Load(const _variant_t &var, bool visrar)
{
	BOOL bl = TRUE;
	try
	{
		Free();
		if((var.vt == (VT_UI1|VT_ARRAY)) && (var.parray != NULL))
		{
			DWORD size = var.parray->rgsabound[0].cElements;
			if(size == 0)
				return FALSE;
			char* pv = new char[size];
			if(pv != NULL)
			{
				TCHAR *pBuf = NULL;
				SafeArrayAccessData(var.parray,(void **)&pBuf);
				memcpy(pv,pBuf,size);		
				//pv[size-1] = '\0';
				SafeArrayUnaccessData(var.parray);
				CFile file;
				if(visrar)
					file.Open(m_strRarPic,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
				else
					file.Open(m_strTempPic,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
				file.Write(pv,size);
				//file.Flush();
				file.Close();
				delete[] pv;
				pv = NULL;
				// unrar
				if(visrar)
				{
					WIN32_FIND_DATA data;
					memset(&data, 0, sizeof(LPWIN32_FIND_DATA));
					HANDLE hfind = ::FindFirstFile(m_strRarPath + CString(_T("\\UnRAR.exe")), &data);
					if(hfind == INVALID_HANDLE_VALUE || hfind == (HANDLE)(LONG)INVALID_FILE_SIZE)
					{
						throw(-1L);
					}
					else
					{
						FindClose(hfind);
					}
					// ��ѹ���ļ�
					CString cmdline;
					cmdline.Format(_T("%s\\UnRAR.exe e -o+ -inul %s %s"), m_strRarPath, m_strRarPic, m_strRarPath);
					STARTUPINFO si;
					PROCESS_INFORMATION pi;
					::memset(&si, 0, sizeof(STARTUPINFO));
					::memset(&pi, 0, sizeof(PROCESS_INFORMATION));
					si.cb = sizeof(si);
					si.dwFlags = STARTF_USESHOWWINDOW;
					si.wShowWindow = SW_HIDE;

					if(!CreateProcess(NULL, _bstr_t(cmdline), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
					{
						throw(-2L);
					}
					else
					{
						WaitForSingleObject( pi.hProcess, INFINITE );
					   // Close process and thread handles. 
						CloseHandle( pi.hProcess );
						CloseHandle( pi.hThread );
						//::WinExec(cmdline, SW_HIDE);
						// 
						Load(m_strTempPic);
					}
				}
				else
				{
					Load(m_strTempPic);
				}
			}
		}
		else
		{
		}
	}
	catch(...)
	{
		bl = FALSE;
		LPVOID lpMsgBuf = NULL;
		FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf,
			0,
			NULL 
		);
		// Process any inserts in lpMsgBuf.
		// ...
		// Display the string.
		if(lpMsgBuf == NULL)
		{
			//MessageBox(NULL, "����ͼƬʧ�ܣ�", "��Ϣ��ʾ3", MB_OK|MB_ICONINFORMATION);
			WriteInfo(_T("����ͼƬʧ�ܣ�"), _T("PICTURE Load"));
		}
		else
		{
			//MessageBox( NULL, (LPCTSTR)lpMsgBuf, "��Ϣ��ʾ3", MB_OK | MB_ICONINFORMATION );
			WriteInfo((LPCTSTR)lpMsgBuf, _T("PICTURE Load"));
		}
		// Free the buffer.
		LocalFree( lpMsgBuf );
	}
	return bl;
}

//////////////////
// Render to device context. Covert to HIMETRIC for IPicture.
//
BOOL CPicture::Render(CDC* pDC, CRect rc, LPCRECT prcMFBounds) const
{
	ASSERT(pDC);

	if (rc.IsRectNull()) {
		CSize sz = GetImageSize(pDC);
		rc.right = sz.cx;
		rc.bottom = sz.cy;
	}
	long hmWidth = 0, hmHeight = 0; // HIMETRIC units
	GetHIMETRICSize(hmWidth, hmHeight);
	m_spIPicture->Render(*pDC, rc.left, rc.top, rc.Width(), rc.Height(),
		0, hmHeight, hmWidth, -hmHeight, prcMFBounds);

	return TRUE;
}

//////////////////
// Get image size in pixels. Converts from HIMETRIC to device coords.
//
CSize CPicture::GetImageSize(CDC* pDC) const
{
	if (!m_spIPicture)
		return CSize(0,0);

	LONG hmWidth, hmHeight; // HIMETRIC units
	m_spIPicture->get_Width(&hmWidth);
	m_spIPicture->get_Height(&hmHeight);
	CSize sz(hmWidth,hmHeight);
	if (pDC==NULL) {
		CWindowDC dc(NULL);
		dc.HIMETRICtoDP(&sz); // convert to pixels
	} else {
		pDC->HIMETRICtoDP(&sz);
	}
	return sz;
}

BOOL CPicture::DoPicToVariant(_variant_t &var, DWORD* vlen, BOOL visrar)
{
	var.Clear();
	var.vt = VT_NULL;
	var.scode = DISP_E_PARAMNOTFOUND;
	if(m_spIPicture == NULL)
	{
		return TRUE;
	}
	BOOL bl = TRUE;
	if(vlen != NULL)
		*vlen = 1;
	try
	{
		//WIN32_FIND_DATA data;
		//memset(&data, 0, sizeof(LPWIN32_FIND_DATA));
		//HANDLE hfind = ::FindFirstFile(m_strRarPic, &data);
		//if(hfind == INVALID_HANDLE_VALUE || hfind == (HANDLE)(LONG)INVALID_FILE_SIZE)
		//{
		if(visrar)
		{
			// ѹ���ļ������ڣ�����ѹ���ļ�
			CString cmdline;
			cmdline.Format(_T("%s\\Rar.exe a -o+ -ep1 -inul %s %s"), m_strRarPath, m_strRarPic, m_strTempPic);
			//::WinExec(cmdline, SW_HIDE);
			STARTUPINFO si;
			PROCESS_INFORMATION pi;
			::memset(&si, 0, sizeof(STARTUPINFO));
			::memset(&pi, 0, sizeof(PROCESS_INFORMATION));
			si.cb = sizeof(si);
			si.dwFlags = STARTF_USESHOWWINDOW;
			si.wShowWindow = SW_HIDE;
			if(!CreateProcess(NULL, _bstr_t(cmdline), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
			{
				throw(-2L);
			}
			else
			{
				WaitForSingleObject( pi.hProcess, INFINITE );
			   // Close process and thread handles. 
				CloseHandle( pi.hProcess );
				CloseHandle( pi.hThread );
				//::WinExec(cmdline, SW_HIDE);
			}
		}
		else
		{	// ֱ�ӽ�m_strTempPic �ļ����Ƹ�m_strRarPic
			::CopyFile(m_strTempPic, m_strRarPic, FALSE);
		}
		//}
		//else
		//	FindClose(hfind);
		CFile file;
		DWORD filelen;
		char *buf = NULL, *bufbegin = NULL;
		SAFEARRAY *psa;
		SAFEARRAYBOUND rgsabound[1];

		if(file.Open(m_strRarPic, CFile::modeRead|CFile::typeBinary))
		{
			filelen = DWORD(file.GetLength());
			if(vlen != NULL)
				*vlen = filelen;
			if(filelen <= 0)
				throw(-2L);
			bufbegin = buf = new char[filelen];
			if(file.Read(buf,filelen) != filelen)
			{
				delete[] bufbegin;
				bufbegin = NULL;
				file.Close();
				throw(-3L);
			}
			file.Close();
			rgsabound[0].lLbound = 0;
			rgsabound[0].cElements = filelen;
			psa = SafeArrayCreate(VT_UI1, 1, rgsabound);
			for (long i = 0; i < (long)filelen; i++)
				SafeArrayPutElement(psa, &i, buf++);
			var.vt = VT_ARRAY | VT_UI1;
			var.parray = psa;
			delete[] bufbegin;
			bufbegin = NULL;
		}
	}
	catch(...)
	{
		bl = FALSE;
		var.Clear();
		var.vt = VT_NULL;
		var.scode = DISP_E_PARAMNOTFOUND;
		LPVOID lpMsgBuf = NULL;
		FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf,
			0,
			NULL 
		);
		// Process any inserts in lpMsgBuf.
		// ...
		// Display the string.
		if(lpMsgBuf == NULL)
		{
			WriteInfo(_T("����ͼƬʧ�ܣ�"), _T("PICTURE PicToVariant"));
			//MessageBox(NULL, "����ͼƬʧ�ܣ�", "��Ϣ��ʾ2", MB_OK|MB_ICONINFORMATION);
		}
		else
		{
			WriteInfo((LPCTSTR)lpMsgBuf, _T("PICTURE PicToVariant"));
			//MessageBox( NULL, (LPCTSTR)lpMsgBuf, "��Ϣ��ʾ2", MB_OK | MB_ICONINFORMATION );
		}
		// Free the buffer.
		LocalFree( lpMsgBuf );
	}
	return bl;
}

void CPicture::WriteInfo(LPCTSTR vinfo, LPCTSTR vtitle)
{
	if(vinfo == NULL)
		return;
	int ret = -1;
	DWORD len = 0;
	CString str;
	TCHAR syspath[MAX_PATH];
	::GetSystemDirectory(syspath, MAX_PATH);
	COleDateTime dt = COleDateTime::GetCurrentTime();
	str.Format(_T("%s\\YOYA\\info\\pic%04d%02d%02d.ino"), syspath, dt.GetYear(), dt.GetMonth(), dt.GetDay());
	WIN32_FIND_DATA data;
	memset(&data, 0, sizeof(LPWIN32_FIND_DATA));
	::lstrcat(syspath, _T("\\YOYA\\info"));
	HANDLE hfind = ::FindFirstFile(syspath, &data);
	if(hfind == INVALID_HANDLE_VALUE || hfind == (HANDLE)(LONG)INVALID_FILE_SIZE)
	{
		::CreateDirectory(syspath, NULL);
	}
	else
		FindClose(hfind);
	HANDLE hfile = ::CreateFile(str, GENERIC_READ|GENERIC_WRITE, 
		FILE_SHARE_READ, NULL, OPEN_ALWAYS, NULL, NULL);
	if(hfile)
	{
	//	BOOL bl = ::SetEndOfFile(hfile);
		if(vtitle != NULL)
			str.Format(_T("%s %s\t %s\t%s\r\n"), dt.Format(VAR_DATEVALUEONLY), dt.Format(VAR_TIMEVALUEONLY), vtitle, vinfo);
		else
			str.Format(_T("%s %s\t %s\r\n"), dt.Format(VAR_DATEVALUEONLY), dt.Format(VAR_TIMEVALUEONLY), vinfo);
		SetFilePointer( hfile, 0, 0, FILE_END ); 
		::WriteFile(hfile, str, str.GetLength(), &len, NULL);
	}
	if(hfile != NULL && hfile != INVALID_HANDLE_VALUE)
		CloseHandle(hfile);
}

// ͼƬ����Ϊ
BOOL CPicture::SaveAs(LPCTSTR vfullpathname)
{
	BOOL bl = TRUE;
	try
	{
		CString path(vfullpathname);
		
		if(path.IsEmpty())
		{
			TCHAR szFilter[] = _T("JPG Files (*.jpg)\0*.JPG\0BMP Files (*.bmp)\0*.BMP\0All Files (*.*)\0*.*\0\0");
			CFileDialog dlg(FALSE);
			dlg.m_ofn.lpstrFilter = szFilter;//_T("*.xls");
			dlg.m_ofn.lpstrDefExt = _T("bmp");//_T("xls");
			if(dlg.DoModal() == IDOK)
			{
				path = dlg.GetPathName();
			}
		}
		if(!path.IsEmpty())
		{
			::CopyFile(m_strTempPic, path, FALSE);
		}

	}
	catch(...)
	{
		bl = FALSE;
		
	}

	return bl;
}





}