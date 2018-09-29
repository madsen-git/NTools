#if !defined(AFX_PICTURE_H__E6BCD284_A5F6_4684_AFFD_0BAA24B8B9B1__INCLUDED_)
#define AFX_PICTURE_H__E6BCD284_A5F6_4684_AFFD_0BAA24B8B9B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++
#include <atlbase.h>
#include <comdef.h>
#include <string>
//#include "d:/VSTOOLS/AC/ANormal.h"
// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.
/////////////////////////////////////////////////////////////////////////////
// CPicture wrapper class
namespace acm {
#define DEF_PICTURE_TEMP_NAME	L"pic_temp.tmp"
#define DEF_PICTURE_RAR_NAME	L"pic_rar.rxp"
class CPicture : public COleDispatchDriver
{
public:
	CPicture();
	CPicture(LPCTSTR vname, LPCTSTR vpath = NULL)	
	{
		//TCHAR str[MAX_PATH];
		//CString path;
		//DWORD len = ::GetSystemDirectory(str, MAX_PATH);
		//str[len] = '\0';
		CString str = acg::CAApp::getAppPath().c_str();
		if(vpath == NULL || ::lstrlen(vpath) <= 0)
			m_strRarPath.Format(_T("%s\\YOYA"), str);
		else
		{
			m_strRarPath = vpath;
			if(m_strRarPath.Right(1).CompareNoCase(_T("\\")) == 0)
				m_strRarPath = m_strRarPath.Left(m_strRarPath.GetLength() - 1);
		}
		// 
		if(vname == NULL || ::lstrlen(vname) <= 0)
		{
			m_strTempPic.Format(_T("%s\\%s"), m_strRarPath, DEF_PICTURE_TEMP_NAME);
			m_strRarPic.Format(_T("%s\\%s"), m_strRarPath, DEF_PICTURE_RAR_NAME);
		}
		else
		{
			CString sss = vname;
			if(sss.Find('.') > 0)
			{
				m_strTempPic.Format(_T("%s\\%s"), m_strRarPath, sss);
				m_strRarPic.Format(_T("%s\\%s.rxp"), m_strRarPath, sss.Left(sss.Find('.')-1));
			}
			else
			{
				m_strTempPic.Format(_T("%s\\%s.tmp"), m_strRarPath, vname);
				m_strRarPic.Format(_T("%s\\%s.rxp"), m_strRarPath, vname);
			}
		}

	}		
	// Calls COleDispatchDriver default constructor
	CPicture(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) 
	{
		CString str = acg::CAApp::getAppPath().c_str();
		//TCHAR str[254];
		//CString path = acg::CAApp::getAppPath().c_str();;
		//DWORD len = ::GetSystemDirectory(str, 254);
		//str[len] = '\0';
		m_strRarPath.Format(_T("%s\\YOYA"), str);
		m_strTempPic.Format(_T("%s\\%s"), m_strRarPath, DEF_PICTURE_TEMP_NAME);
		m_strRarPic.Format(_T("%s\\%s"), m_strRarPath, DEF_PICTURE_RAR_NAME);
	}
	CPicture(const CPicture& dispatchSrc) : COleDispatchDriver(dispatchSrc) 
	{
		CString str = acg::CAApp::getAppPath().c_str();
		//TCHAR str[254];
		//CString path = acg::CAApp::getAppPath().c_str();;
		//DWORD len = ::GetSystemDirectory(str, 254);
		//str[len] = '\0';
		m_strRarPath.Format(_T("%s\\YOYA"), str);
		m_strTempPic.Format(_T("%s\\%s"), m_strRarPath, DEF_PICTURE_TEMP_NAME);
		m_strRarPic.Format(_T("%s\\%s"), m_strRarPath, DEF_PICTURE_RAR_NAME);
	}
	~CPicture()
	{
		// 删除临时文件
		if(m_strRarPic.Right(17) != CString(DEF_PICTURE_TEMP_NAME))
			::DeleteFile(m_strRarPic);
		if(m_strTempPic.Right(16) != CString(DEF_PICTURE_RAR_NAME))
			::DeleteFile(m_strTempPic);

	}
// Attributes
public:
	long GetHandle();
	long GetHPal();
	void SetHPal(long);
	short GetType();
	long GetWidth();
	long GetHeight();

// Operations
public:
	// 图片另存为
	BOOL SaveAs(LPCTSTR vfullpathname = NULL);
	// 从变体中加载图像
	BOOL Load(const _variant_t& var, bool visrar = false);
	// 将压缩后的图片文件数据保存到var中，并将数据长度放在*vlen
	BOOL DoPicToVariant(_variant_t& var, DWORD* vlen = NULL, BOOL visrar = FALSE);
	BOOL DoPicToVariant(COleVariant& var, DWORD* vlen = NULL, BOOL visrar = FALSE)
	{
		BOOL bl = FALSE;
		_variant_t vvv;
		bl = DoPicToVariant(vvv, vlen, visrar);
		var = vvv;
		return bl;
	}
	//BOOL DoSaveTempfile(CFile& vfile, LPCTSTR vtempfile = NULL, LPCTSTR vrarfile = NULL);
	// method 'Render' not emitted because of invalid return type or parameter type
	// Load frm various sosurces
	BOOL Load(UINT nIDRes);
	BOOL Load(LPCTSTR pszPathName);
	BOOL Load(CFile& vfile);
	BOOL Load(CArchive& ar);
	BOOL Load(IStream* pstm);

	// render to device context
	BOOL Render(CDC* pDC, CRect rc=CRect(0,0,0,0),
		LPCRECT prcMFBounds=NULL) const;
	BOOL Render(CDC* pDC, CWnd* pwnd, int vid, BOOL visfull = FALSE)
	{
		CBrush brush;
		CRect rect;
		pwnd->GetDlgItem(vid)->GetWindowRect(&rect);
		pwnd->ScreenToClient(&rect);
		brush.CreateSolidBrush(RGB(100,100,100));
		pDC->FillRect(rect, &brush);
		
		if(pwnd == NULL || vid <= 0 || pDC == NULL || m_spIPicture == NULL)
			return FALSE;
		if(visfull)
		{
		}
		else
		{
			long boxH, boxW, imgH, imgW, w, h;
			boxH = rect.Height();
			boxW = rect.Width();

			CSize size = GetImageSize();
			imgH = size.cy;
			imgW = size.cx;
			if((float(boxW)/boxH) > (float(imgW)/imgH))
			{ 
				h = boxH;
				w = long(float(imgW)/imgH * h);
				rect.DeflateRect((boxW - w)/2, 0);
			}
			else
			{
				w = boxW;
				h = long(float(imgH)/imgW * w);
				rect.DeflateRect(0, (boxH - h)/2);
			}
		}
		Render(pDC, rect);
		return TRUE;

	}
	CSize GetImageSize(CDC* pDC=NULL) const;

	operator IPicture*() {
		return m_spIPicture;
	}

	void GetHIMETRICSize(OLE_XSIZE_HIMETRIC& cx, OLE_YSIZE_HIMETRIC& cy) const {
		cx = cy = 0;
		const_cast<CPicture*>(this)->m_hr = m_spIPicture->get_Width(&cx);
		ASSERT(SUCCEEDED(m_hr));
		const_cast<CPicture*>(this)->m_hr = m_spIPicture->get_Height(&cy);
		ASSERT(SUCCEEDED(m_hr));
	}

	void Free() {
		if (m_spIPicture) {
			m_spIPicture.Release();
		}
		// 删除临时文件
		::DeleteFile(m_strRarPic);
		::DeleteFile(m_strTempPic);
		/*SHFILEOPSTRUCT shdel;
		shdel.hwnd = NULL;
		shdel.wFunc = FO_DELETE;
		shdel.pFrom = _T("c:\\ydoor.txt");//m_strRarPic;
		shdel.pTo = NULL;
		shdel.fFlags = FOF_ALLOWUNDO | FOF_NOERRORUI|FOF_NOCONFIRMATION;
		::SHFileOperation(&shdel);
		shdel.pFrom = m_strTempPic;
		::SHFileOperation(&shdel);*/
	}

protected:
	CComQIPtr<IPicture>m_spIPicture;		 // ATL smart pointer to IPicture
	HRESULT m_hr;
private:
	void WriteInfo(LPCTSTR vinfo, LPCTSTR vtitle = NULL);
	CString m_strRarPath;	// rar.exe unrar.exe所在的路径
	CString m_strTempPic;	// 共显示用的未压缩的图片
	CString m_strRarPic;	// 压缩后的图片
};


}
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PICTURE_H__E6BCD284_A5F6_4684_AFFD_0BAA24B8B9B1__INCLUDED_)
