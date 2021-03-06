#include "stdafx.h"
#include "UkeyTestAuto.h"
#include "util.h"

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(UkeyTestAuto, REGISTRY_NAME_AUTO_LOGIN);
UkeyTestAuto::UkeyTestAuto()
{
}


UkeyTestAuto::~UkeyTestAuto()
{
}

void UkeyTestAuto::setUp()
{
	if (!login()) { AfxMessageBox(L"登录失败，测试用例可能无法进行"); }

	//先获取密码重试次数，保证不被锁住
	{
		auto retryCount = parseJsonAndGetMember([]()->CString {return s_pDRS_CertSafeCtrl->RS_GetPinRetryCount(_RS_CONTAINER_ID); }, "/data/retryCount");
		if (!retryCount.first) { AfxMessageBox(L"setup失败"); }
		std::string retryCountContent = retryCount.second;
		if (retryCountContent.empty()) { AfxMessageBox(L"setup失败"); }
		if (!std::all_of(retryCountContent.begin(), retryCountContent.end(), ::isdigit)) { AfxMessageBox(L"setup失败"); }
		int cnt = atoi(retryCountContent.c_str());
		if (cnt <= 3) { AfxMessageBox(L"setup失败"); }

	}
}

void UkeyTestAuto::tearDown()
{

}

void UkeyTestAuto::testGetUserList()
{
	using namespace rapidjson;
	//预期成功1
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_GetUserList);
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(hasCode(jsonDoc));
		LOG_ASSERT(isSuccessful(jsonDoc));

		const Value* userlist = GetValueByPointer(jsonDoc, "/data/userlist");
		LOG_ASSERT(userlist&&userlist->IsString());
		std::wstring userlistContent = to_wstr(userlist->GetString());
		LOG_END();
	}

	//预期失败
	{

	}
	
}
void UkeyTestAuto::testGetCertBase64String()
{
	using namespace rapidjson;

	//预期成功1
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_GetCertBase64String, _RS_CONTAINER_ID, 1);
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(hasCode(jsonDoc));
		LOG_ASSERT(isSuccessful(jsonDoc));

		const Value* certBase64 = GetValueByPointer(jsonDoc, "/data/certBase64");
		LOG_ASSERT(certBase64&&certBase64->IsString());
		std::string certBase64Content = certBase64->GetString();
		LOG_END();
	}

	//预期成功2
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_GetCertBase64String, _RS_CONTAINER_ID, 2);
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(hasCode(jsonDoc));
		LOG_ASSERT(isSuccessful(jsonDoc));

		const Value* certBase64 = GetValueByPointer(jsonDoc, "/data/certBase64");
		LOG_ASSERT(certBase64&&certBase64->IsString());
		std::string certBase64Content = certBase64->GetString();
		LOG_END();
	}

	//预期失败
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_GetCertBase64String, L"", 1);
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(getCode(jsonDoc)=="9003");
		LOG_END();
	}

	//预期失败
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_GetCertBase64String, L"", 2);
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(getCode(jsonDoc) == "9003");
		LOG_END();
	}

	//预期失败
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_GetCertBase64String, L"123456", 1);
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(getCode(jsonDoc) == "9004");
		LOG_END();
	}

	//预期失败
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_GetCertBase64String, L"123456", 2);
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(getCode(jsonDoc) == "9004");
		LOG_END();
	}

	//预期失败
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_GetCertBase64String, L"B2810FDB-B11C-4D78-", 1);
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(getCode(jsonDoc) == "9004");
		LOG_END();
	}

	//预期失败
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_GetCertBase64String, L"B2810FDB-B11C-4D78-", 2);
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(getCode(jsonDoc) == "9004");
		LOG_END();
	}

	//预期失败 type不符要求
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_GetCertBase64String, _RS_CONTAINER_ID, 3);
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(getCode(jsonDoc) == "9012");
		LOG_END();
	}

	//预期失败 type不符要求
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_GetCertBase64String, _RS_CONTAINER_ID, 0);
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(getCode(jsonDoc) == "9012");
		LOG_END();
	}

	//预期失败 type不符要求
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_GetCertBase64String, _RS_CONTAINER_ID, -1);
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(getCode(jsonDoc) == "9012");
		LOG_END();
	}

	//预期失败 type不符要求
	{
		short tmps[1000];
		short type= tmps[67];//模拟未初始化
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_GetCertBase64String, _RS_CONTAINER_ID, type);
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(getCode(jsonDoc) == "9012");
		LOG_END();
	}
}

void UkeyTestAuto::testCertLogin()
{
	using namespace rapidjson;

	//预期成功
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_CertLogin, _RS_CONTAINER_ID, _RS_PASSWD);
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(hasCode(jsonDoc));
		LOG_ASSERT(isSuccessful(jsonDoc));
		LOG_END();
	}

	//预期失败
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_CertLogin, L"", _RS_PASSWD);
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(getCode(jsonDoc)=="9003");
		LOG_END();
	}

	//预期失败
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_CertLogin, L"123456", _RS_PASSWD);
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(getCode(jsonDoc) == "9004");
		LOG_END();
	}

	//预期失败
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_CertLogin, L"B2810FDB-B11C-4D78-", _RS_PASSWD);
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(getCode(jsonDoc) == "9004");
		LOG_END();
	}

	//预期失败
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_CertLogin, _RS_CONTAINER_ID, L"");
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(getCode(jsonDoc) == "9010");
		LOG_END();
	}

	//预期失败
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_CertLogin, _RS_CONTAINER_ID, L"0");
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(getCode(jsonDoc) == "9011");
		LOG_END();
	}
}

void UkeyTestAuto::testGetPinRetryCount()
{
	using namespace rapidjson;

	//预期成功1
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_GetPinRetryCount, _RS_CONTAINER_ID);
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(hasCode(jsonDoc));
		LOG_ASSERT(isSuccessful(jsonDoc));

		const Value* retryCount = GetValueByPointer(jsonDoc, "/data/retryCount");
		LOG_ASSERT(retryCount&&retryCount->IsString());
		std::string retryCountContent = retryCount->GetString();
		LOG_ASSERT(!retryCountContent.empty());
		LOG_ASSERT(std::all_of(retryCountContent.begin(), retryCountContent.end(), ::isdigit));
		LOG_END();
	}

	//预期失败1 证书不符
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_GetPinRetryCount, L"@$^%^%$fd");
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(hasCode(jsonDoc));
		LOG_ASSERT(!isSuccessful(jsonDoc));
		LOG_END();
	}
}

void UkeyTestAuto::testKeySignByP7()
{
	using namespace rapidjson;

	//预期成功1 不带原文
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_KeySignByP7, TEST_DATA_W, L"1", _RS_CONTAINER_ID);
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(hasCode(jsonDoc));
		LOG_ASSERT(isSuccessful(jsonDoc));

		const Value* signdMsg = GetValueByPointer(jsonDoc, "/data/signdMsg");
		LOG_ASSERT(signdMsg&&signdMsg->IsString());
		std::string tt = signdMsg->GetString();
		LOG_END();
	}

	//预期成功2 带原文
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_KeySignByP7, TEST_DATA_W, L"0", _RS_CONTAINER_ID);
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(hasCode(jsonDoc));
		LOG_ASSERT(isSuccessful(jsonDoc));

		const Value* signdMsg = GetValueByPointer(jsonDoc, "/data/signdMsg");
		LOG_ASSERT(signdMsg&&signdMsg->IsString());
		std::string tt = signdMsg->GetString();
		LOG_END();
	}

	//预期失败1 container_id 不符
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_KeySignByP7, TEST_DATA_W, L"1", L"random-45634562456");
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(hasCode(jsonDoc));
		LOG_ASSERT(!isSuccessful(jsonDoc));
		LOG_END();
	}
}

void UkeyTestAuto::testKeySignByP1()
{
	using namespace rapidjson;

	//预期成功1
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_KeySignByP1, TEST_DATA_W, _RS_CONTAINER_ID);
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(hasCode(jsonDoc));
		LOG_ASSERT(isSuccessful(jsonDoc));

		const Value* signdMsg = GetValueByPointer(jsonDoc, "/data/signdMsg");
		LOG_ASSERT(signdMsg&&signdMsg->IsString());
		std::string tt = signdMsg->GetString();
		LOG_END();
	}

	//预期失败1 container_id 不符
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_KeySignByP1, TEST_DATA_W, L"random-45634562456");
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(hasCode(jsonDoc));
		LOG_ASSERT(!isSuccessful(jsonDoc));
		LOG_END();
	}
}

void UkeyTestAuto::testVerifySignByP1()
{
	using namespace rapidjson;

	//生成signedMsgContent
	bool isSignSuccessful = true;
	std::string signedMsgContent;
	{
		GDoc jsonDoc = parseJson([]()->CString {return s_pDRS_CertSafeCtrl->RS_KeySignByP1(TEST_DATA_W, _RS_CONTAINER_ID); });
		isSignSuccessful = isSignSuccessful && (!hasParseError(jsonDoc));
		isSignSuccessful = isSignSuccessful && (hasCode(jsonDoc));
		isSignSuccessful = isSignSuccessful && (isSuccessful(jsonDoc));

		const Value* signdMsg = GetValueByPointer(jsonDoc, "/data/signdMsg");
		isSignSuccessful = isSignSuccessful && (signdMsg&&signdMsg->IsString());
		signedMsgContent = signdMsg->GetString();
	}

	//预期成功1
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_VerifySignByP1, _RS_CERT_SIGN, TEST_DATA_W, to_wstr(signedMsgContent).data());
		LOG_ASSERT(isSignSuccessful);
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(hasCode(jsonDoc));
		LOG_ASSERT(isSuccessful(jsonDoc));
		LOG_END();
	}

	//预期失败1 签名证书格式错误
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_VerifySignByP1, L"$%^&%#&SHH", TEST_DATA_W, to_wstr(signedMsgContent).data());
		LOG_ASSERT(isSignSuccessful);
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(hasCode(jsonDoc));
		LOG_ASSERT(!isSuccessful(jsonDoc));
		LOG_END();
	}

	//预期失败2 代签名原文不符
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_VerifySignByP1, _RS_CERT_SIGN, L"4321", to_wstr(signedMsgContent).data());
		LOG_ASSERT(isSignSuccessful);
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(hasCode(jsonDoc));
		LOG_ASSERT(!isSuccessful(jsonDoc));
		LOG_END();
	}

	//预期失败2 签名值不符
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_VerifySignByP1, _RS_CERT_SIGN, TEST_DATA_W, L"#^@&^$d");
		LOG_ASSERT(isSignSuccessful);
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(hasCode(jsonDoc));
		LOG_ASSERT(!isSuccessful(jsonDoc));
		LOG_END();
	}
}

void UkeyTestAuto::testChangePassWd()
{
	//return;

	using namespace rapidjson;

	//预期成功1
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_ChangePassWd, _RS_CONTAINER_ID, _RS_PASSWD, L"87654321");
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(hasCode(jsonDoc));
		LOG_ASSERT(isSuccessful(jsonDoc));
		LOG_END();
	}

	//预期失败 containerId 错误
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_ChangePassWd, L"AAAAAAA123", L"87654321", _RS_PASSWD);
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(hasCode(jsonDoc));
		LOG_ASSERT(!isSuccessful(jsonDoc));
		LOG_END();
	}

	//预期失败 原密码错误
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_ChangePassWd, _RS_CONTAINER_ID, L"#&%&H", _RS_PASSWD);
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(hasCode(jsonDoc));
		LOG_ASSERT(!isSuccessful(jsonDoc));
		LOG_END();
	}

	//预期成功，改回初始密码
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_ChangePassWd, _RS_CONTAINER_ID, L"87654321", _RS_PASSWD);
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(hasCode(jsonDoc));
		LOG_ASSERT(isSuccessful(jsonDoc));
		LOG_END();
	}
}

void UkeyTestAuto::testVerifyIdentity()
{
	using namespace rapidjson;
	//预期成功1 加密证书
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_VerifyIdentity, _RS_CERT_ENCRYPT, _RS_AUTH_CODE);
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(hasCode(jsonDoc));
		LOG_ASSERT(isSuccessful(jsonDoc));

		//TODO: 文档中数据是在/data中，这里测试了是在/data/data里，并不是/data里
		const Value* data = GetValueByPointer(jsonDoc, "/data");
		LOG_ASSERT(data&&data->IsString());
		std::string dataContent = data->GetString();
		LOG_END();
	}

	//预期成功2 签名证书
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_VerifyIdentity, _RS_CERT_SIGN, _RS_AUTH_CODE);
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(hasCode(jsonDoc));
		LOG_ASSERT(isSuccessful(jsonDoc));

		//TODO: 文档中数据是在/data中，这里测试了是在/data/data里，并不是/data里
		const Value* data = GetValueByPointer(jsonDoc, "/data");
		LOG_ASSERT(data&&data->IsString());
		std::string dataContent = data->GetString();
		LOG_END();
	}

	//预期失败1 证书不符合要求
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_VerifyIdentity, L"kglijdshgsd96785", _RS_AUTH_CODE);
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(hasCode(jsonDoc));
		LOG_ASSERT(!isSuccessful(jsonDoc));
		LOG_END();
	}
}

void UkeyTestAuto::testKeyGetKeySn()
{
	using namespace rapidjson;
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_KeyGetKeySn);
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(hasCode(jsonDoc));
		LOG_ASSERT(isSuccessful(jsonDoc));
		LOG_END();
	}

#if UNNECISSARY_ASSERT
	//预期成功
	{
		GDoc jsonDoc = parseJson([]()->CString {return s_pDRS_CertSafeCtrl->RS_KeyGetKeySn(); });
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(hasCode(jsonDoc));
		LOG_ASSERT(isSuccessful(jsonDoc));

		const Value* keySn = GetValueByPointer(jsonDoc, "/data/keySn");
		LOG_ASSERT(keySn&&keySn->IsString());
		std::string keySnContent = keySn->GetString();

		//TODO: 文档中示例有containerId，但实际获取数据里并没有这个字段
		//const Value* containerId = GetValueByPointer(jsonDoc, "/data/containerId");
		//LOG_ASSERT(containerId&&containerId->IsString());
		//std::string containerIdContent = containerId->GetString();
	}
#endif
}

void UkeyTestAuto::testKeyGetDeviceInfo()
{
	using namespace rapidjson;
	//预期成功1 设备序列号
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_KeyGetDeviceInfo, _RS_CONTAINER_ID, L"1");
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(hasCode(jsonDoc));
		LOG_ASSERT(isSuccessful(jsonDoc));

		const Value* info = GetValueByPointer(jsonDoc, "/data/info");
		LOG_ASSERT(info&&info->IsString());
		std::string infoContent = info->GetString();
		LOG_END();
	}

	//预期失败1 容器名不符
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_KeyGetDeviceInfo, L"", L"1");
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(hasCode(jsonDoc));
		LOG_ASSERT(!isSuccessful(jsonDoc));
		LOG_END();
	}
}

void UkeyTestAuto::testKeyDigitalSignByP1()
{

}

void UkeyTestAuto::testVerifyDigitalSignByP1()
{

}

void UkeyTestAuto::testKeyEncryptData()
{
	using namespace rapidjson;

	//预期成功
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_KeyEncryptData, L"123456aAX", _RS_CERT_ENCRYPT);
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(hasCode(jsonDoc));
		LOG_ASSERT(isSuccessful(jsonDoc));
		LOG_END();
	}

	//预期失败
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_KeyEncryptData, L"123456aAX", L"");
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(getCode(jsonDoc)=="9005");
		LOG_END();
	}

	//预期失败
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_KeyEncryptData, L"", _RS_CERT_ENCRYPT);
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(getCode(jsonDoc) == "9023");
		LOG_END();
	}

	//预期失败
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_KeyEncryptData, L"123456aAX", L"123456");
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(getCode(jsonDoc) == "9024");
		LOG_END();
	}

#if UNNECISSARY_ASSERT
	//预期成功1
	{
		auto encRsKey = parseJsonAndGetMember([]()->CString {return s_pDRS_CertSafeCtrl->RS_KeyEncryptData(TEST_DATA_W, _RS_CERT_ENCRYPT); }, "/data/encRsKey");
		LOG_ASSERT(encRsKey.first);
		std::string& encRsKeyContent = encRsKey.second;
		LOG_ASSERT(!encRsKeyContent.empty());
	}

	//预期失败1 证书不符
	{
		GDoc jsonDoc = parseJson([]()->CString {return s_pDRS_CertSafeCtrl->RS_KeyEncryptData(TEST_DATA_W, L"#$@sdf"); });
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(hasCode(jsonDoc));
		LOG_ASSERT(!isSuccessful(jsonDoc));
	}
#endif
}

void UkeyTestAuto::testKeyDecryptData()
{
	using namespace rapidjson;
	
	bool isEncryptSuccessful = true;
	auto encRsKey = parseJsonAndGetMember([]()->CString {return s_pDRS_CertSafeCtrl->RS_KeyEncryptData(TEST_DATA_W, _RS_CERT_ENCRYPT); }, "/data/encRsKey");
	isEncryptSuccessful = isEncryptSuccessful && (encRsKey.first);
	std::string& encRsKeyContent = encRsKey.second;
	isEncryptSuccessful = isEncryptSuccessful && (!encRsKeyContent.empty());

	//预期成功1
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_KeyDecryptData, to_wstr(encRsKeyContent).data(), _RS_CONTAINER_ID);
		LOG_ASSERT(isEncryptSuccessful);
		const Value* rsKey = GetValueByPointer(jsonDoc, "/data/rsKey");
		LOG_ASSERT(rsKey&&rsKey->IsString());
		std::string rsKeyContent = rsKey->GetString();
		LOG_ASSERT(rsKeyContent == TEST_DATA);
		LOG_END();
	}

	//预期失败1 待解密密文不符
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_KeyDecryptData, L"sdflk76585", _RS_CONTAINER_ID);
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(hasCode(jsonDoc));
		LOG_ASSERT(!isSuccessful(jsonDoc));
		LOG_END();
	}

	//预期失败1 containerId不符
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_KeyDecryptData, to_wstr(encRsKeyContent).data(), L"kjhjk345");
		LOG_ASSERT(isEncryptSuccessful);
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(hasCode(jsonDoc));
		LOG_ASSERT(!isSuccessful(jsonDoc));
		LOG_END();
	}
}

void UkeyTestAuto::testKeyEncryptFile()
{
	using namespace rapidjson;

	//预期成功1
	{
		bool isFileCreated = true;
		std::wstring inFile = fs::current_path().wstring() + L"/test_encrypt_file_in.txt";
		std::wstring outFile = fs::current_path().wstring() + L"/test_encrypt_file_out.txt";
		fs::ofstream ofs(fs::path(inFile), std::ios::binary);
		isFileCreated = isFileCreated && (ofs);
		ofs.write(TEST_DATA, sizeof(TEST_DATA)-1);
		isFileCreated = isFileCreated && (ofs);
		ofs.close();
		isFileCreated = isFileCreated && (fs::exists(inFile));

		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_KeyEncryptFile, inFile.data(), outFile.data(), _RS_CERT_ENCRYPT);
		LOG_ASSERT(isFileCreated);
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(hasCode(jsonDoc));
		LOG_ASSERT(isSuccessful(jsonDoc));
		LOG_ASSERT(fs::exists(outFile));
		LOG_END();
	}

	//预期失败1 原文件不存在
	{
		//TODO: 这里是直接收到空数据，没有code
		bool isFileNotExists = true;
		std::wstring inFile = L"C:/agag2342sdfgsfg";
		std::wstring outFile = fs::current_path().wstring() + L"/test_encrypt_file_out.txt";
		isFileNotExists = isFileNotExists && (!fs::exists(inFile));

		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_KeyEncryptFile, inFile.data(), outFile.data(), _RS_CERT_ENCRYPT);
		LOG_ASSERT(isFileNotExists);
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(hasCode(jsonDoc));
		LOG_ASSERT(!isSuccessful(jsonDoc));
		LOG_END();
	}
}

void UkeyTestAuto::testKeyDecryptFile()
{
	using namespace rapidjson;

	//预期成功1
	{
		bool isEncrypted = true;
		std::wstring inFile = fs::current_path().wstring() + L"/test_encrypt_file_in.txt";
		std::wstring outFile = fs::current_path().wstring() + L"/test_encrypt_file_out.txt";
		fs::ofstream ofs(fs::path(inFile), std::ios::binary);
		isEncrypted = isEncrypted && (ofs);
		ofs.write(TEST_DATA, sizeof(TEST_DATA)-1);
		isEncrypted = isEncrypted && (ofs);
		ofs.close();
		isEncrypted = isEncrypted && (fs::exists(inFile));

		GDoc jsonDoc = parseJson([&inFile, &outFile]()->CString {return s_pDRS_CertSafeCtrl->RS_KeyEncryptFile(inFile.data(), outFile.data(), _RS_CERT_ENCRYPT); });
		isEncrypted = isEncrypted && (!hasParseError(jsonDoc));
		isEncrypted = isEncrypted && (hasCode(jsonDoc));
		isEncrypted = isEncrypted && (isSuccessful(jsonDoc));

		isEncrypted = isEncrypted && (fs::exists(outFile));

		{
			std::wstring decryptOutFile = fs::current_path().wstring() + L"/test_decrypt_file_out.txt";
			LOG_BEG2(s_pDRS_CertSafeCtrl->RS_KeyDecryptFile, outFile.data(), decryptOutFile.data(), _RS_CONTAINER_ID);
			LOG_ASSERT(isEncrypted);
			LOG_ASSERT(!hasParseError(jsonDoc));
			LOG_ASSERT(hasCode(jsonDoc));
			LOG_ASSERT(isSuccessful(jsonDoc));
			LOG_ASSERT(fs::exists(decryptOutFile));

			fs::ifstream ifs(fs::path(decryptOutFile), std::ios::binary);
			char tt[sizeof(TEST_DATA) - 1];
			ifs.read(tt, sizeof(TEST_DATA) - 1);
			LOG_ASSERT(ifs);
			LOG_ASSERT(std::equal(tt, tt + (sizeof(TEST_DATA) - 1), TEST_DATA));

			//TODO: 这里测试的结果，源文件只有4个字节长度，解密后的文件，虽然前4个字节是正确的，但是整个文件会强制变成128字节，并且后面的是乱码
			LOG_ASSERT(fs::file_size(fs::path(decryptOutFile)) == (sizeof(TEST_DATA) - 1));
			LOG_END();
		}
	}
}

void UkeyTestAuto::testKeyEncryptByDigitalEnvelope()
{

}

void UkeyTestAuto::testKeyDecryptByDigitalEnvelope()
{

}


/*
参数：
	containerId:容器名
返回：Json格式的字符串
	例：
	{"code":"0000","msg":"执行成功。","data":""}
功能：获取指定介质key的打开状态，code值为0000表示已登录，非0000表示未登录。
*/
void UkeyTestAuto::testKeyStatus()
{
	//预期成功1
	{
		LOG_BEG2(s_pDRS_CertSafeCtrl->RS_KeyStatus, _RS_CONTAINER_ID);
		LOG_ASSERT(!hasParseError(jsonDoc));
		LOG_ASSERT(hasCode(jsonDoc));
		LOG_ASSERT(isSuccessful(jsonDoc));//这里测试的结果应该是0000，未登录情况应该非0000
		LOG_END();
	}
}