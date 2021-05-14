#pragma once
#ifndef _RS_232C_H_
#define _RS_232C_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <windows.h>

class RS232c {
private:
	HANDLE m_hComm;
	DWORD size;
	DCB	m_Dcb;
public:
	//各種パラメーターの設定後に接続します。 返り値　TRUE = 成功　FALSE = 失敗
	bool Connect(const char* PortNmae,  //ポート名を指定します　COM1 COM2など、初期値はCOM1
		int BaudRate,//ボーレートを指定します。初期値は 9600
		int ByteSize,//1バイトのビット数を指定します。初期値は 8
		int Parity,//パリティを指定します。パリティなし: NOPARITY
				   //偶数パリティ: EVENPARITY
				   //奇数パリティ: ODDPARITY
				   //初期値は、パリティなし: NOPARITY
		int StopBits,//ストップビット数を指定します。
					 //1ビット: ONESTOPBIT
					 //1.5ビット: ONE5STOPBITS
					 //2ビット: TWOSTOPBITS 
					 //初期値は　1ビット: ONESTOPBIT
		int RTS,		//RTSをON=RTS_CONTROL_ENABLE 初期値は無効です
		int DTR,		//DTRをON=DTR_CONTROL_ENABLE 初期値は無効です
		int ReadTimeOut,//受信時のタイムアウト　msで指定　初期値は　5000ms
		int WriteTimeOut//送信時のタイムアウト　msで指定　初期値は　20000ms
	);
	//文字列の受信 Read(char配列,読み込む文字数) 返り値　TRUE = 成功　FALSE = 失敗
	bool Read(char* Buff, int NumberOfCharactersToRead);
	//文字列の受信 Read_CRLF(char配列,バッファーサイズ) 返り値　TRUE = 成功　FALSE = 失敗
	//CRLFをキャッチしたときに受信完了します、読み込み文字列がバッファーサイズをあふれた場合は
	//その時点までの文字列しか受信できません
	bool Read_CRLF(char* Buff, int NumberOfCharactersToRead);
	//文字列を送信 Send(送信文字列) 返り値　TRUE = 成功　FALSE = 失敗
	bool Send(const char* word);
	//CHAR文字を送信 Send(送信文字列) 返り値　TRUE = 成功　FALSE = 失敗
	bool Send_CHAR(unsigned char wordchar);
	//接続の状態を取得します
	bool isLink();
	//デストラクタ　ポートを閉じます
	~RS232c();
};

#endif // _RS_232C_H_