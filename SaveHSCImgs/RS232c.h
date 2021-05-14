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
	//�e��p�����[�^�[�̐ݒ��ɐڑ����܂��B �Ԃ�l�@TRUE = �����@FALSE = ���s
	bool Connect(const char* PortNmae,  //�|�[�g�����w�肵�܂��@COM1 COM2�ȂǁA�����l��COM1
		int BaudRate,//�{�[���[�g���w�肵�܂��B�����l�� 9600
		int ByteSize,//1�o�C�g�̃r�b�g�����w�肵�܂��B�����l�� 8
		int Parity,//�p���e�B���w�肵�܂��B�p���e�B�Ȃ�: NOPARITY
				   //�����p���e�B: EVENPARITY
				   //��p���e�B: ODDPARITY
				   //�����l�́A�p���e�B�Ȃ�: NOPARITY
		int StopBits,//�X�g�b�v�r�b�g�����w�肵�܂��B
					 //1�r�b�g: ONESTOPBIT
					 //1.5�r�b�g: ONE5STOPBITS
					 //2�r�b�g: TWOSTOPBITS 
					 //�����l�́@1�r�b�g: ONESTOPBIT
		int RTS,		//RTS��ON=RTS_CONTROL_ENABLE �����l�͖����ł�
		int DTR,		//DTR��ON=DTR_CONTROL_ENABLE �����l�͖����ł�
		int ReadTimeOut,//��M���̃^�C���A�E�g�@ms�Ŏw��@�����l�́@5000ms
		int WriteTimeOut//���M���̃^�C���A�E�g�@ms�Ŏw��@�����l�́@20000ms
	);
	//������̎�M Read(char�z��,�ǂݍ��ޕ�����) �Ԃ�l�@TRUE = �����@FALSE = ���s
	bool Read(char* Buff, int NumberOfCharactersToRead);
	//������̎�M Read_CRLF(char�z��,�o�b�t�@�[�T�C�Y) �Ԃ�l�@TRUE = �����@FALSE = ���s
	//CRLF���L���b�`�����Ƃ��Ɏ�M�������܂��A�ǂݍ��ݕ����񂪃o�b�t�@�[�T�C�Y�����ӂꂽ�ꍇ��
	//���̎��_�܂ł̕����񂵂���M�ł��܂���
	bool Read_CRLF(char* Buff, int NumberOfCharactersToRead);
	//������𑗐M Send(���M������) �Ԃ�l�@TRUE = �����@FALSE = ���s
	bool Send(const char* word);
	//CHAR�����𑗐M Send(���M������) �Ԃ�l�@TRUE = �����@FALSE = ���s
	bool Send_CHAR(unsigned char wordchar);
	//�ڑ��̏�Ԃ��擾���܂�
	bool isLink();
	//�f�X�g���N�^�@�|�[�g����܂�
	~RS232c();
};

#endif // _RS_232C_H_