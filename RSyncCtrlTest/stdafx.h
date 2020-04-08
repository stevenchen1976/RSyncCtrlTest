﻿
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持
#include <afxdisp.h>
#include <afxdisp.h>









#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#include <vector>

#include "util.h"
#include "CDRS_CertSafe.h"
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>


inline void onAssertPass(std::string filename, int line, std::string cond)
{
	std::string u8=u8"通过：";
	u8.append(filename).append(u8"第").append(std::to_string(line)).append(u8"行\n条件：").append(cond).append("\n\n");

	fs::ofstream ofs(fs::current_path().append(L"/通过的测试.txt"), std::ios::binary);
	if (ofs)
	{
		ofs.write(u8.data(), u8.length());
	}
}
#define ASSERT2(cond) \
(CPPUNIT_ASSERT_MESSAGE("", cond); \
CPPUNIT_NS::SourceLine location=CPPUNIT_SOURCELINE(); \
onAssertPass(location.fileName(), location.lineNumber(), #cond);)


#define REGISTRY_NAME_AUTO "自动化测试部分"
#define REGISTRY_NAME_MANUAL "手动测试部分"
#define REGISTRY_NAME_LOGIN "登录测试"
#define REGISTRY_NAME_LOGOUT "无登录测试(插拔key一次)"

#define REGISTRY_NAME_AUTO_LOGIN "登录测试(自动测试)"
#define REGISTRY_NAME_MANUAL_LOGIN "登录测试(手动测试)"
#define REGISTRY_NAME_AUTO_LOGOUT "无登录测试(插拔key一次)(自动测试)"
#define REGISTRY_NAME_MANUAL_LOGOUT "无登录测试(插拔key一次)(手动测试)"

#define TEST 0
#define TEST_WITH_INFO_POPUP 1//测试特殊用例时弹出提示信息框（如提示用户拔掉key）
#define CPPUNIT_CSL 0//CPPUNIT无界面版，并且有测试log
#define UNNECISSARY_ASSERT 0//暂时不用考虑的断言

#define TEST_DATA "1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"//待测试数据（十个数字和26个字母大小写）
#define TEST_DATA_W L"1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"//待测试数据（十个数字和26个字母大小写）
#define _RS_CONTAINER_ID (m_ContainerId.GetBuffer())//containerId
#define _RS_PASSWD (m_PassWord.GetBuffer())
#define _RS_AUTH_CODE (m_AuthCode.GetBuffer())
#define _RS_CERT_ENCRYPT (m_CertEncrypt.GetBuffer())//加密证书
#define _RS_CERT_SIGN (m_CertSign.GetBuffer())//签名证书


extern CString m_PassWord;
extern CString m_ContainerId;
extern CString m_AuthCode;
extern CString m_CertEncrypt;
extern CString m_CertSign;

extern CString m_RsignCloud;
extern CString m_TokenLogin;
extern CString m_TokenEncrypt;
extern CString m_TokenDecrypt;
extern CString m_TokenSeal;
extern CString m_TokenCert;

extern std::vector<seal> m_SealList;



extern CDRS_CertSafe* s_pDRS_CertSafeCtrl;//ocx接口
extern HWND s_hMainWnd;
extern CWnd* s_pMainWnd;
//extern  CRs_certsafectrl1 *g_SafeCerCtrl;
typedef LRESULT(CALLBACK *WndProc)(HWND, UINT, WPARAM, LPARAM);
extern  BOOL MyCreateWindow(WndProc _wndProc, HWND* _pHWND);

extern void MyDestroyWindow();
extern LRESULT CALLBACK MyWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);


