#ifndef __MESSAGEHEADER_HPP__
#define __MESSAGEHEADER_HPP__
enum CMD
{
	CMD_LOGIN = 1,
	CMD_LOGIN_RESULT,
	CMD_LOGOUT,
	CMD_LOGOUT_RESULT,
	CMD_NEW_USER_JOIN,
	CMD_ERROR
};

//��Ϣͷ
struct DataHeader
{
	short dataLength;    //���ݳ���
	short cmd;            //����
};
//��Ϣ��
struct Login :public DataHeader
{
	Login() {
		dataLength = sizeof(Login);
		cmd = CMD_LOGIN;
	}
	char userName[32];
	char passWord[32];
};
//�����Ƿ��ͳɹ�
struct LoginResult : public DataHeader
{
	LoginResult() :result(0) {
		dataLength = sizeof(LoginResult);
		cmd = CMD_LOGIN_RESULT;
	}
	int result;
};
//�ǳ�
struct LogOut : public DataHeader
{
	LogOut() {
		dataLength = sizeof(LogOut);
		cmd = CMD_LOGOUT;
	}
	char userName[32];
};
//�ǳ����
struct LogOutResult : public DataHeader
{
	LogOutResult() :result(0) {
		dataLength = sizeof(LogOutResult);
		cmd = CMD_LOGOUT_RESULT;
	}
	int result;
};
struct NewUser : public DataHeader
{
	NewUser() {
		dataLength = sizeof(DataHeader);
		cmd = CMD_NEW_USER_JOIN;
		result = 0;
	}
	int result;
};

#endif