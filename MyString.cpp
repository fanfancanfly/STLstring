// MyString.cpp : �������̨Ӧ�ó������ڵ㡣
//
/*****************************************
**���ܣ��Լ�ʵ��C++ STL��string������
**���ߣ�л����
**ʱ�䣺2015-07-21  02��20
*****************************************/
#include "stdafx.h"
#include <iostream>
#include <iomanip>                                       //����õ�����setw����������԰�����ͷ�ļ�

using namespace std;

class MyString
{
	friend ostream& operator<<(ostream&,MyString&);      //�����������أ���Ԫ����
	friend istream& operator>>(istream&,MyString&);      //�������������
public:
	MyString(const char* str = NULL);                   //Ĭ�Ϲ��캯��������һ��Ĭ�ϲ���
	MyString(const MyString& other);                    //�������캯��,���������ݣ�����˵���
	MyString& operator=(const MyString& other);         //���ظ�ֵ�����
	MyString operator+(const MyString& other)const;     //���ؼӺ������
	bool operator==(const MyString& );                  //operator==
	bool operator<(const MyString& );                   //operator<
	char& operator[](unsigned int);                     //operator[]
	size_t size() {return strlen(m_data);}
	MyString append(const MyString& other);             //��β������
	MyString insert(unsigned int ipos,const char *);    //����λ�ò���
	MyString& replace(unsigned int ipos,unsigned int num,const char *);       //�滻����
	MyString& erase(unsigned int start,unsigned int final);                   //ɾ������
	int find(const char* stem,int ipos = 0);                                  //���Һ���
	int find_first_of(const char* stem,int ipos = 0);                         //�����ַ����е�һ����ָ�����г��ֵ��ַ�λ��
	int rfind(const char *stem,int ipos);                                     //������ң�����������iposλ��Ϊ��ʼ��λ�ã�Ȼ���������ƥ�䣬�ҵ���һ������λ��
	~MyString()	{}                                      //��������
private:
	char* m_data;                                       //ָ��̬�ڴ��ָ��
	int npos;                                           //��ѯ��־  ��ʾ�ַ�����ѯʧ��
};

inline MyString::MyString(const char* str = NULL)       //Ĭ�Ϲ��캯����Ϊ����  ������  ֱ���滻
{
	if (!str)
	{
		m_data =NULL;
	}
	else
	{
		m_data = new char[strlen(str)+1];
		strcpy(m_data,str);
	}
	npos = -1;
}

inline MyString::MyString(const MyString& other)       //��������
{
	if (!other.m_data)                                 //����ĳ�Ա�����ڿ��Է���ͬ�ֶ����˽������(ͬ��������Ԫ��ϵ)
	{
		m_data = NULL;
	}
	else
	{
		m_data = new char[strlen(other.m_data)+1];
		strcpy(m_data,other.m_data);
	}
	npos = -1;
}

inline MyString& MyString::operator=(const MyString& other)     //��ֵ�����
{
	if (this != &other)                                         //ע�⣬��ֵ����������Ը�ֵ
	{
		delete [] m_data;
		if (!other.m_data)
		{
			m_data = NULL;
		}
		else
		{
			m_data = new char[strlen(other.m_data)+1];
			strcpy(m_data,other.m_data);
		}
	}
	return *this;                                             //thisָ�������Ϊ�����
}

inline MyString MyString::operator+(const MyString& other)const    //�Ӻ�����
{
	//���Ƕ������
	MyString newString;               //�Ӻ����أ�����һ��ֵ��������һ����ʱ��������
	if (!other.m_data)
	{
		newString = *this;
	}
	else if (!m_data)
	{
		newString = other;
	}
	else
	{
		newString.m_data = new char[strlen(m_data) + strlen(other.m_data) +1];
		strcpy(newString.m_data,m_data);
		strcat(newString.m_data,other.m_data);
	}
	return newString;
}

inline bool MyString::operator==(const MyString& other)
{
	if (strlen(other.m_data) != strlen(m_data))
	{
		return false;
	}
	else
	{
		return strcmp(other.m_data,m_data)?false:true;
	}
}

inline bool MyString::operator<(const MyString& other)                   //operator<
{
	if (strlen(m_data) == 0 && strlen(other.m_data) != 0)
	{
		return true;
	}
	else if (strlen(m_data) != 0 && strlen(other.m_data) == 0)
	{
		return false;
	}

	int iIndex = 0;
	while (m_data[iIndex] && other.m_data[iIndex])              //��ͬ��������Ƚ��ַ�
	{
		if (m_data[iIndex] < other.m_data[iIndex])
		{
			return true;
		}
		else if (m_data[iIndex] > other.m_data[iIndex])
		{
			return false;
		}
		else
		{
			++iIndex;
		}
	}
	if (!m_data[iIndex] && other.m_data[iIndex])                 //��һ���ַ����ȵڶ����ַ����� ��ǰ����ͬ  eg: xiefanfan xiefanfanaa
	{
		return true;
	}
	else
	{
		return false;
	}	
}

inline char& MyString::operator[](unsigned int num)
{
	if (num>=0 && num<strlen(m_data))
	{
		return m_data[num];
	}
}

ostream& operator<<(ostream& os,MyString& mstem)            //��Ԫ��������ʾ�ú������Ǹ���ĳ�Ա�����ǿ��Բ������˽������
{
	os<<mstem.m_data;                                       //��Ҫ��Ӹ�ʽ���Ʒ���ֻ�����
	return os;                                              //return֧������<<
}

istream& operator>>(istream& is,MyString& mstem)
{
	char temp[255];                                         //��ʱ����ռ�
	is>>setw(255)>>temp;
	mstem = temp;                                           //ʹ�ø�ֵ�����
	return is;											    //return֧������>>
}

MyString MyString::append(const MyString& other)             //��β������  �ο�operator+����
{
	MyString newString;
	if (!other.m_data)
	{
		newString = *this;
	}
	else
	{
		newString.m_data = new char[strlen(m_data) + strlen(other.m_data) +1];
		strcpy(newString.m_data,m_data);
		strcat(newString.m_data,other.m_data);
	}
	return newString;
}
MyString MyString::insert(unsigned int ipos,const char *stem)            //����λ�ò��뺯��
{
	MyString newString;
	int iIndex;
	if (ipos >= 0 && ipos <= strlen(m_data))                               //ipos�ڷ�Χ�ڲ�
	{
		newString.m_data = new char[strlen(m_data) + strlen(stem) +1];     //����ռ�
		for (iIndex = 0;iIndex < strlen(newString.m_data)-1;++iIndex)
		{
			if (iIndex < ipos)                                             //����ԭʼ��  iposǰ������                        
			{
				newString.m_data[iIndex] = m_data[iIndex];
			}
			else
			{
				if (iIndex >= ipos && iIndex < (ipos + strlen(stem)))
				{
					newString.m_data[iIndex] = stem[iIndex - ipos];
				}
				else
				{
					newString.m_data[iIndex] = m_data[iIndex - strlen(stem)];
				}
			}
		}
		newString.m_data[iIndex] = NULL;                                      //���һλ�ÿ�
	}
}

MyString& MyString::replace(unsigned int ipos,unsigned int num,const char *stem)            //�滻����                                   
{
	if (ipos >= 0 && ipos < strlen(m_data) && (ipos + num) >= 0 && (ipos + num) < strlen(m_data) )
	{
		for (int iIndex =0;iIndex < strlen(m_data);++iIndex)
		{
			if (iIndex >= ipos && iIndex<(ipos + strlen(stem)))            //�滻����
			{
				m_data[iIndex] = stem[iIndex - ipos];
			}
			else
			{
				m_data[iIndex] = m_data[iIndex + num -strlen(stem)];     //ԭʼ����û���滻�� ��ǰ�ƶ�
			}
		}
	}
	return *this;
}
/////////////////////////////�˴�Ӧ���õĵ�����/////////////////////////////////////////////
MyString& MyString::erase(unsigned int start,unsigned int final)                   //ɾ������
{
	if (start >= 0 && start < strlen(m_data) && final >= 0 && final < strlen(m_data) && start <= final)
	{
		for (int iIndex = final;iIndex < strlen(m_data);++iIndex )
		{
			m_data[iIndex - final + start] = m_data[iIndex];                       //��߸���ǰ��
		}
	}
	return *this;
}

int MyString::find(const char* stem,int ipos = 0)                                  //�ִ����Һ���  
{
	if (ipos + strlen(stem) > strlen(m_data))                                      //������Χ
	{
		return npos;
	}
	for (int iIndex = ipos;iIndex < strlen(m_data) - strlen(stem);++iIndex)        //�ڳ��Ĵ���ƥ��ָ���Ĵ�  ��Ҫ����ƥ��  ������KMP�㷨
	{
		int jval;
		for (jval = 0;jval < strlen(stem);++jval)
		{
			if (m_data[iIndex + jval] != stem[jval])
			{
				return npos;
			}
		}
		if (jval == strlen(stem))
		{
			return iIndex;
		}
	}
}

int MyString::find_first_of(const char* stem,int ipos = 0)                         //�����ַ����е�һ����ָ�����г��ֵ��ַ�λ��
{
	int length = strlen(m_data);
	int iIndex;
	for ( iIndex = 0;iIndex < length;++iIndex)
	{
		for (int iIndex1 = 0;iIndex1 < strlen(stem);++iIndex1)
		{
			if (m_data[iIndex] == stem[iIndex1])                         //���ƥ����һ���ַ�����������
			{
				return iIndex;
			}
		}
	}
	if (iIndex >= length)
	{
		return npos;
	}
}
int MyString::rfind(const char *stem,int ipos)                                 //�������
{
	for (int iIndex = ipos;iIndex >= 0;--iIndex)
	{
		int slen = strlen(m_data);                         //ԭ������
		int slen1 = strlen(stem);                          //ƥ�䴮����
		if ((slen - iIndex) >= slen1)                      //��ǰ�ַ�����ַ������ڵ��ڴ�ƥ�䴮
		{
			int tem = 0;
			while (m_data[iIndex + tem] == stem[tem] && tem < slen1)        //����ƥ��
			{
				++tem;
			}
			if (tem >= slen1)
			{
				return iIndex;
			}
		}
	}
	return npos;                                             //����ϱ�û��ƥ���ϣ���ʾ������ƥ����
}












int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}

