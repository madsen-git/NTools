#pragma once
#include "AC/sysupport.h"


namespace acm{

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	regKey类	2014-08-13
//	注册表管理类
////////////////////////////////////////////////////////////////////////////////////////////////////////
class regKey
{
public:
	regKey(void);
	~regKey(void);
	operator HKEY() const;
	/*****************************************************************************************
		<< --- regKey::create		2014-08-13 --- >>
		说明：创建或打开一个新的键值
		参数：
		hKeyParent	= 一个打开项的句柄，或者一个标准项名
		pKeyName	= 欲创建的新子项的名字
		samDesired	= 安全访问标记
		返回值：是否成功
	*****************************************************************************************/
	bool create(HKEY hKeyParent, PCWSTR pKeyName, REGSAM samDesired = KEY_READ | KEY_WRITE);
	/*****************************************************************************************
		<< --- regKey::open		2014-08-13 --- >>
		说明：打开一个指定的注册表键
		参数：
		hKeyParent	= 需要打开的主键的名称
		pKeyName	= 需要打开的子键的名称
		samDesired	= 安全访问标记
		返回值：是否成功
	*****************************************************************************************/
	bool open(HKEY hKeyParent, PCWSTR pKeyName, REGSAM samDesired = KEY_READ | KEY_WRITE);
	/*****************************************************************************************
		<< --- regKey::close		2014-08-13 --- >>
		说明：释放指定注册键的句柄
		参数：
		返回值：是否成功
	*****************************************************************************************/
	bool close();

	/****************************************************************************************
		<< --- 	existsKey	2015-8-17 --- >>         
		说明：查验指定项是否存在
		参数：
			pkeyName	= 查验指定项是否存在
			pType		= 不为空时返回指定项的数据类型
		返回值：是否存在
	*****************************************************************************************/
	bool existsKey(PCWSTR pkeyName, DWORD *pType = NULL);
	/****************************************************************************************
		<< --- 	deleteKey	2015-8-17 --- >>         
		说明：删除指定项
		参数：
			pkeyName	= 被删除项的key名
		返回值：是否成功
	*****************************************************************************************/
	bool deleteKey(PCWSTR pkeyName);
	/****************************************************************************************
		<< --- 	createKey	2015-8-17 --- >>         
		说明：创建指定项，如果已经存在就更新其值
		参数：
			pkeyName	= 被创建项的key名
			pValue		= 项的值
		返回值：是否成功
	*****************************************************************************************/
	bool createKey(PCWSTR pkeyName, PCWSTR pValue);
	/****************************************************************************************
		<< --- 	readValue	2015-2-15 --- >>         
		说明：从注册表中读取值，并返回值类型
		参数：
		pValueName	= 要读取值的键名称
		sbValue		= 返回的值数据
		pdwType		= 数据类型
		返回值：是否执行成功
	*****************************************************************************************/
	bool readValue(PCWSTR pValueName, SBDataType &sbValue, DWORD *pdwType);
	/*****************************************************************************************
		<< --- regKey::readUI4		2014-08-13 --- >>
		说明：读取无符合32位键值
		参数：
		pValueName	= 要读取值的键名称
		dwValue		= 返回的键值
		返回值：是否成功
	*****************************************************************************************/
	bool readUI4(PCWSTR pValueName, DWORD &dwValue);
	/*****************************************************************************************
		<< --- regKey::readUI8		2014-08-13 --- >>
		说明：读取无符合64位键值
		参数：
		pValueName	= 要读取值的键名称
		ullValue	= 返回的键值
		返回值：是否成功
	*****************************************************************************************/
	bool readUI8(PCWSTR pValueName, ULONGLONG &ullValue);
	/*****************************************************************************************
		<< --- regKey::readString		2014-08-13 --- >>
		说明：读取字符型键值
		参数：
		pValueName	= 要读取值的键名称
		wsValue		= 返回的键值
		uiBytes		= 可能要读取的最大字节数
		返回值：是否成功
	*****************************************************************************************/
	bool readString(PCWSTR pValueName, std::wstring &wsValue);
	bool readStrings(PCWSTR pValueName, std::vector<std::wstring> &vecValue);
	/*****************************************************************************************
		<< --- regKey::readBinary		2014-08-13 --- >>
		说明：读取二进制型键值
		参数：
		pValueName	= 要读取值的键名称
		sbValue		= 返回的键值
		返回值：是否成功
	*****************************************************************************************/
	bool readBinary(PCWSTR pValueName, SBDataType &sbValue);

	/*****************************************************************************************
		<< --- regKey::writeUI4		2014-08-13 --- >>
		说明：写无符合32位键值
		参数：
		pValueName	= 要读取值的键名称
		dwValue		= 被写入的值
		返回值：键值
	*****************************************************************************************/
	bool writeUI4(PCWSTR pValueName, DWORD dwValue);
	/*****************************************************************************************
		<< --- regKey::writeUI8		2014-08-13 --- >>
		说明：写无符合64位键值
		参数：
		pValueName	= 要读取值的键名称
		dwValue		= 被写入的值
		返回值：键值
	*****************************************************************************************/
	bool writeUI8(PCWSTR pValueName, ULONGLONG ullValue);
	/*****************************************************************************************
		<< --- regKey::writeString		2014-08-13 --- >>
		说明：写字符串键值
		参数：
		pValueName	= 要读取值的键名称
		wsValue		= 被写入的值
		dwType		= 键值类型
		返回值：键值
	*****************************************************************************************/
	bool writeString(PCWSTR pValueName, const std::wstring &wsValue, DWORD dwType = REG_SZ /*REG_SZ*/);
	/*****************************************************************************************
		<< --- regKey::writeBinary		2014-08-13 --- >>
		说明：写二进制键值
		参数：
		pValueName	= 要读取值的键名称
		sbValue		= 被写入的值
		返回值：键值
	*****************************************************************************************/
	bool writeBinary(PCWSTR pValueName, const SBDataType &sbValue);

	/*****************************************************************************************
		<< --- regKey::enumKey		2014-08-13 --- >>
		说明：枚举键名称
		参数：
		iIndex		= 枚举索引
		wsName		= 返回的键名称
		plastWriteTime	= 最后修改时间
		uiChars		= 估算的键名称长度
		返回值：是否成功
		*****************************************************************************************/
	bool enumKey(DWORD iIndex, std::wstring &wsName, FILETIME *plastWriteTime = NULL, ULONG uiChars = MAX_PATH);

	/*****************************************************************************************
		<< --- regKey::notifyChangeKeyValue		2014-08-13 --- >>
		说明：监控注册表
		参数：
		bWatchSubtree	= 是否监测注册表项子项
		dwNotifyFilter	= REG_NOTIFY_CHANGE_NAME、REG_NOTIFY_CHANGE_ATTRIBUTES、REG_NOTIFY_CHANGE_LAST_SET、REG_NOTIFY_CHANGE_SECURITY
		hEvent			= 接收通知的事件
		返回值：是否成功
	*****************************************************************************************/
	bool notifyChangeKeyValue(bool bWatchSubtree, DWORD dwNotifyFilter, HANDLE hEvent);
public:
	bool succeeded() const;
	LONG getLastError() const;
public:
	// 本对象需要管理员权限才能使用，所以封装几个普通权限的函数

	/*****************************************************************************************
		<< --- regKey::readString		2014-08-13 --- >>
		说明：读取字符型键值
		参数：
		hKey		= 主键标示
		pSubKeyName	= 子键名称
		pValue		= 要读取值的键名称
		wsValue		= 返回的键值
		返回值：是否成功
	*****************************************************************************************/
	static bool readString(HKEY hKey, PCWSTR pSubKeyName, PCWSTR pValue, std::wstring &wsValue);
	/*****************************************************************************************
		<< --- regKey::writeString		2014-08-13 --- >>
		说明：写字符型键值
		参数：
		hKey		= 主键标示
		pSubKeyName	= 子键名称
		pValue		= 要写值的键名称
		wsValue		= 被写的键值
		返回值：是否成功
	*****************************************************************************************/
	static bool writeString(HKEY hKey, PCWSTR pSubKeyName, PCWSTR pValue, const std::wstring &wsValue);
protected:
	HKEY	m_hKey;
	LONG	m_lLastError;
};

}