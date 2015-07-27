// MyString.cpp : �������̨Ӧ�ó������ڵ㡣
//
/*****************************************
**���ܣ��Լ�ʵ��C++ STL��string������
**���ߣ�л����
**ʱ�䣺2015-07-21  02��20
*****************************************/
#include "stdafx.h"
#include <iostream>
#include <string>
#include <iomanip>                                       //����õ�����setw����������԰�����ͷ�ļ�

using namespace std;
//�Լ�����д��һ��string������
class MyString
{
    friend ostream& operator<<(ostream&,MyString&);      //�����������أ���Ԫ����
    friend istream& operator>>(istream&,MyString&);      //�������������
public:
    //�±�д��ͨ�������࣬�������е�Ԫ�ؿ��Զ�д����
    class Iterator
    {
        char *init;
    public:
        inline Iterator(char* init) {this->init = init;}           //���캯��
        inline bool operator!=(Iterator& it) {return this->init != it.init;}     //�������ļ������������
        inline void operator++(int){init = init + 1;}
        inline char operator*() {return *init;}
    };
    //�±�д�����������࣬�������е�Ԫ�ؿ��Զ������ǲ����޸�(��ʵ����һ��const����)
    class const_Iterator
    {
        char *init;
    public:
        inline const_Iterator(char* init) {this->init = init;}           //���캯��
        inline bool operator!=(const_Iterator& it) {return this->init != it.init;}     //�������ļ������������
        inline void operator++(int){init = init + 1;}
        inline const char operator*() {return  *init;}
    };
    MyString(const char* str = NULL);                   //Ĭ�Ϲ��캯��������һ��Ĭ�ϲ���
    MyString(const MyString&,int pos1,int pos2);        //���ַ����е�һ���ֳ�ʼ����һ���ַ���
    MyString(Iterator it1,Iterator it2);     //���������캯��
    MyString(const MyString& other);                    //�������캯��,���������ݣ�����˵���
    MyString& operator=(const MyString& other);         //���ظ�ֵ�����
    MyString operator+(const MyString& other) const;    //���ؼӺ������
    MyString& operator+=(const MyString &other);        //+�Ÿ��ϸ�ֵ�����
    bool operator==(const MyString& );                  //operator==
    bool operator<(const MyString& );                   //operator<
    char& operator[](int);                              //operator[]
    size_t size() {return strlen(m_data);}              //�����ַ����ĳ���
    size_t length(){return strlen(m_data);}    
    size_t capacity(){return m_capacity;}                //n*16-1�ڴ���䷽��
    bool empty();                                 //�ж��ַ����Ƿ�Ϊ��
    void reserve(size_t num = 0);                 //����ı����ռ�  ��ֹ�ظ����� ���� ����ǰ���㹻�ռ�  Ӱ��capacity
    void resize(size_t num,char ctem = 0);            //�����ַ���Ч����ĳߴ�
    char& at(int num);                           //�ַ�����
    MyString& assign(const char *);              //c�ַ������и�ֵ
    MyString substr(int pos = 0,int num = -1) const;        //����pos��ʼ��n���ַ����ַ���
    void swap(MyString& stem);                             //������ǰ�ַ�����stem�ַ�����ֵ
    MyString& append(const MyString& other);             //��β������
    MyString& insert(unsigned int ipos,const char *);    //����λ�ò���
    MyString& replace(unsigned int ipos,unsigned int num,const char *);       //�滻����
    MyString& erase(unsigned int start,unsigned int final);                   //ɾ������
    int find(const char* stem,int ipos = 0);                                  //���Һ���
    int find_first_of(const char* stem,int ipos = 0);                         //�����ַ����е�һ����ָ�����г��ֵ��ַ�λ��
    int find_first_not_of(const char* stem,int ipos = 0);
    int rfind(const char *stem,int ipos = -1);                                //������ң�����������iposλ��Ϊ��ʼ��λ�ã�Ȼ���������ƥ�䣬�ҵ���һ������λ�� 
    int find_last_of(const char* stem,int ipos = 0);                          //����ԭ����ָ���������һ�������ַ���λ��
    int find_last_not_of(const char* stem,int ipos = 0);
    char* Begin() {return m_data;}                        //��õ���������ʼλ��
    char* End(){return m_end;}                          //��õ�������β��λ��
    ~MyString();                                        //��������
private:
    char* m_data;                                       //ָ��̬�ڴ��ָ��
    char* m_end;                                        //β��ָ��
    int npos;                                          //��ѯ��־  ��ʾ�ַ�����ѯʧ��
    int m_capacity;                                     //��ʾ�ַ���������
};

inline MyString::MyString(const char* str)              //Ĭ�Ϲ��캯����Ϊ����  ������  ֱ���滻
{
    if (!str)
    {
        m_data = new char[16];        //��ʼ����16���ֽ�
        m_data[0] = '\0';             //��һ��λ�÷��ַ���������ʶ��
        m_end = m_data+1;
        m_capacity = 16 - 1;
    }
    else
    {
        int num = 1;
        while (strlen(str) > num*16)
        {
            ++num;
        }
        m_data = new char[num*16];
        m_end = m_data + strlen(str);
        strcpy(m_data,str);
        m_capacity = 16*num - 1;
    }
    npos = -1;
}

inline MyString::MyString(const MyString& other,int pos1,int pos2)            //���캯��
{
    m_data = new char[other.m_capacity];
    m_end = m_data + pos2;
    m_capacity = other.m_capacity;
    int iIndex;
    for (iIndex = 0;iIndex < pos2;++iIndex)
    {
        m_data[iIndex] = other.m_data[pos1 + iIndex];
    }
    m_data[iIndex] = '\0';
}

inline MyString::MyString(Iterator it1,Iterator it2)     //���������캯��
{
    int num = 0;
    Iterator tem1 = it1;                        //�������ʱ����
    while (it1 != it2)
    {
        num++;
        it1++;
    }
    int num1 = 1;
    while (num1*16 < num)
    {
        ++num1;
    }
    m_data = new char[16*num1];
    m_end  = m_data + num;
    m_capacity = num1*16;
    int index = 0;
    for (Iterator Ittem = tem1;Ittem != it2;Ittem++)
    {
        m_data[index] = *Ittem;
        index++;
    }
    m_data[index] = '\0';
}


inline MyString::MyString(const MyString& other)       //��������
{
    if (!other.m_data[0])                                 //����ĳ�Ա�����ڿ��Է���ͬ�ֶ����˽������(ͬ��������Ԫ��ϵ)
    {
        m_data = new char[16];        //��ʼ����16���ֽ�
        m_data[0] = '\0';             //��һ��λ�÷��ַ���������ʶ��
        m_end = m_data+1;
        m_capacity = 16 - 1;
    }
    else
    {
        m_data = new char[other.m_capacity];
        strcpy(m_data,other.m_data);
        m_end = m_data + strlen(other.m_data);
        m_capacity = other.m_capacity;
    }
    npos = -1;
}

inline MyString& MyString::operator=(const MyString& other)     //��ֵ�����
{
    if (this != &other)                                         //ע�⣬��ֵ����������Ը�ֵ
    {
        if (!other.m_data[0])                    //otherΪ��
        {
            if (m_data[0])
            {
                delete [] m_data;
            }
            m_data = new char[16];        //��ʼ����16���ֽ�
            m_data[0] = '\0';             //��һ��λ�÷��ַ���������ʶ��
            m_end = m_data+1;
            m_capacity = 16 -1;
        }
        else
        {
            if (m_capacity < other.m_capacity)             //��߿ռ�С  ���·���ռ䲢��������
            {
                delete [] m_data;
                m_data = new char[other.m_capacity];
                strcpy(m_data,other.m_data);
                m_end = m_data + strlen(other.m_data);
                m_capacity = other.m_capacity;
            }
            else
            {
                strcpy(m_data,other.m_data);
                m_end = m_data + strlen(other.m_data);
            }
        }
    }
    return *this;                                             //thisָ�������Ϊ�����
}

inline MyString MyString::operator+(const MyString& other)const    //�Ӻ�����
{
    //���Ƕ������
    MyString newString;               //�Ӻ����أ�����һ��ֵ��������һ����ʱ��������
    if (!other.m_data[0])
    {
        newString = *this;
    }
    else if (!m_data[0])
    {
        newString = other;
    }
    else
    {
        int num = 1;
        while (num*16 < (strlen(m_data) + strlen(other.m_data) +1))
        {
            ++num;
        }
        newString.m_data = new char[num*16];
        newString.m_capacity = num*16 - 1;
        newString.m_end = newString.m_data + strlen(m_data) + strlen(other.m_data);
        strcpy(newString.m_data,m_data);
        strcat(newString.m_data,other.m_data);
    }
    return newString;
}

MyString& MyString::operator+=(const MyString &other)                //+�Ÿ��ϸ�ֵ�����
{
    //���Ƕ������
    MyString newString;               //�Ӻ����أ�����һ�����ã�������һ����ʱ�������ظ�*this
    if (!other.m_data[0])
    {
        newString = *this;
    }
    else if (!m_data[0])
    {
        newString = other;
    }
    else
    {
        int num = 1;
        while (num*16 < (strlen(m_data) + strlen(other.m_data) +1))
        {
            ++num;
        }
        newString.m_data = new char[num*16];
        newString.m_capacity = num*16 - 1;
        newString.m_end = newString.m_data + strlen(m_data) + strlen(other.m_data);
        strcpy(newString.m_data,m_data);
        strcat(newString.m_data,other.m_data);
    }
    *this = newString;
    return *this;
}

inline bool MyString::empty()      //�ж��ַ����Ƿ�Ϊ��
{
    if (!m_data[0])
    {
        return 1;
    }
    else
    {
        return 0;
    }
}  
void MyString::reserve(size_t num)                 //����ı����ռ�  ��ֹ�ظ����� ���� ����ǰ���㹻�ռ�
{
    MyString newString = *this;                    //ԭ�������ݱ���
    int num1 = 1;
    while (16*num1 < newString.m_capacity)
    {
        ++num1;
    }

    if (m_data[0])
    {
        delete [] m_data;
    }
    m_data = new char[16*num1];
    m_end = m_data + strlen(newString.m_data);
    strcpy(m_data,newString.m_data);
    m_capacity = 16*num1 - 1;

}

void MyString::resize(size_t num,char ctem)            //�����ַ���Ч����ĳߴ�
{
    MyString newString = *this;
    if (num <= strlen(m_data))
    {
        erase(num,strlen(m_data));
    }
    else
    {
        int num1 = 1;
        while (num1*16 < num)
        {
            ++num1;
        }
        if (m_data[0])
        {
            delete [] m_data;
        }
        m_data = new char[num1*16];
        m_end = m_data + strlen(m_data) + num;
        m_capacity = num1*16 - 1;
        int iIndex = 0;
        while (iIndex < strlen(m_data) + num)
        {
            if (iIndex < strlen(m_data))
            {
                m_data[iIndex] = newString.m_data[iIndex];
            }
            else if (iIndex < strlen(m_data) && iIndex < strlen(m_data + num))
            {
                m_data[iIndex] = ctem;
            }
            ++iIndex;
        }
        m_data[iIndex] = '\0';
    }
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
    else if (strlen(m_data) == 0 && strlen(other.m_data) == 0)          //��ʾ������Ϊ��
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

inline char& MyString::operator[](int num)                  //[]����������Ƿ����Խ��
{
    if (num >= 0 && num < strlen(m_data))
    {
        return m_data[num];
    }
}


char& MyString::at(int num)                                //�ַ�����  at������Ҫ�Է���Խ����д���
{
    //����Խ�紦��code
    if (num>=0 && num<strlen(m_data))
    {
        return m_data[num];
    }
    else
    {
        throw out_of_range("string at function");         //�׳�һ���쳣
    }
}

MyString& MyString::assign(const char *stem)              //c�ַ������и�ֵ
{
    if (!stem)
    {
        m_data = new char[16];
        m_data[0] = '\0';
        m_end = m_data + 1;
        m_capacity = 16 - 1;
        return *this;
    }
    int iLength = strlen(stem);
    if (m_data[0])
    {
        delete [] m_data;
    }
    int num = 1;
    while (num*16 < strlen(stem + 1))
    {
        ++num;
    }
    m_data = new char[16*num];                           //����ռ�
    strcpy(m_data,stem);                                 //��������
    m_end = m_data + strlen(stem);
    m_capacity = 16*num - 1;
    return *this;
}

MyString MyString::substr(int pos,int num) const        //����pos��ʼ��num���ַ����ַ���  c���Է��
{
    MyString newString;
    int iIndex;
    if (pos >=0 && (pos+num) < strlen(m_data))
    {
        newString.m_data = new char[num + 1];
        newString.m_end = newString.m_data + num;
        for (iIndex = 0;iIndex < num;++iIndex)
        {
            newString.m_data[iIndex] = m_data[pos + iIndex];
        }
        newString.m_data[iIndex] = NULL;
    }
    return newString;
}
void MyString::swap(MyString& stem)                             //������ǰ�ַ�����stem�ַ�����ֵ
{
    MyString newString;                         //����ʱ��������
    newString = stem;
    stem = *this;
    *this = newString;
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

MyString::~MyString()                                      //��������
{
    if (m_data)
    {
        delete [] m_data;
        m_data = NULL;
        m_end = NULL;
    }
}

MyString& MyString::append(const MyString& other)             //��β������  �ο�operator+����
{
    //���Ƕ������
    MyString newString;               //�Ӻ����أ�����һ�����ã�������һ����ʱ�������ظ�*this
    if (!other.m_data[0])
    {
        newString = *this;
    }
    else if (!m_data[0])
    {
        newString = other;
    }
    else
    {
        int num = 1;
        while (num*16 < (strlen(m_data) + strlen(other.m_data) +1))
        {
            ++num;
        }
        newString.m_data = new char[num*16];
        newString.m_capacity = num*16 - 1;
        newString.m_end = newString.m_data + strlen(m_data) + strlen(other.m_data);
        strcpy(newString.m_data,m_data);
        strcat(newString.m_data,other.m_data);
    }
    *this = newString;
    return *this;
}
MyString& MyString::insert(unsigned int ipos,const char *stem)            //����λ�ò��뺯��
{
    MyString newString;
    int iIndex;
    if (ipos >= 0 && ipos < strlen(m_data))                               //ipos�ڷ�Χ�ڲ�
    {
        int iLength = strlen(m_data) + strlen(stem) +1;
        int num = 1;
        while (num*16 < iLength)
        {
            ++num;
        }        
        newString.m_data = new char[num*16];     //����ռ�
        newString.m_end = m_data + strlen(m_data) + strlen(stem);
        newString.m_capacity = num*16 - 1;
        for (iIndex = 0;iIndex < strlen(m_data) + strlen(stem);++iIndex)
        {
            if (iIndex < ipos)                                             //����ԭʼ��  iposǰ������                        
            {
                newString.m_data[iIndex] = m_data[iIndex];
            }
            else
            {
                if (iIndex >= ipos && iIndex < (ipos + strlen(stem)))     //����Ӵ�
                {
                    newString.m_data[iIndex] = stem[iIndex - ipos];
                }
                else                                                      //���ԭ���ַ�����ߵĴ�
                {
                    newString.m_data[iIndex] = m_data[iIndex - strlen(stem)];
                }
            }
        }
        newString.m_data[iIndex] = NULL;                                      //���һλ�ÿ� ��ʾ�ַ�������
    }
    *this = newString;
    return *this;
}

MyString& MyString::replace(unsigned int ipos,unsigned int num,const char *stem)            //�滻����                                   
{
    MyString newString;                                                   //��ʱ�����ַ���
    int iIndex;
    if (ipos >= 0 && ipos < strlen(m_data) )
    {
        int iNewlen = strlen(m_data) + strlen(stem) - num;                //�µĳ���
        int num1 = 1;
        while (num1*16 < iNewlen)
        {
            ++num1;
        }
        newString.m_data = new char[num1*16];
        newString.m_end = m_data + iNewlen;
        newString.m_capacity = num1*16 - 1;
        for (iIndex = 0;iIndex < iNewlen;++iIndex)
        {
            if (iIndex < ipos)                                             //����ipos����ǰ���ַ�
            {
                newString.m_data[iIndex] = m_data[iIndex];
            }
            else if (iIndex >= ipos && iIndex < ipos + strlen(stem))       //�����滻�Ĵ�
            {
                newString.m_data[iIndex] = stem[iIndex - ipos];
            }
            else                                                           //����ԭ��ʣ��Ĳ���
            {
                newString.m_data[iIndex] = m_data[iIndex - strlen(stem) + num];       
            }
        }
    }
    newString.m_data[iIndex] = '\0';                                   //�ַ��������Ϊ0                
    *this = newString;  
    return *this;
}
/////////////////////////////�˴�Ӧ���õĵ�����/////////////////////////////////////////////
MyString& MyString::erase(unsigned int start,unsigned int final)                   //ɾ������
{
    if (start >= 0 && start < strlen(m_data) && final >= 0 && final < strlen(m_data) && start <= final)
    {
        int iIndex;
        for (iIndex = final;iIndex < strlen(m_data);++iIndex )
        {
            m_data[iIndex - final + start] = m_data[iIndex];                       //��߸���ǰ��
        }
        m_data[iIndex - (final - start)] = '\0';
        m_end = m_data + iIndex - (final - start);
    }
    return *this;
}

int MyString::find(const char* stem,int ipos)                                  //�ִ����Һ���  
{
    if (ipos + strlen(stem) > strlen(m_data))                                      //������Χ
    {
        return npos;
    }
    for (int iIndex = ipos;iIndex < strlen(m_data) - strlen(stem);++iIndex)        //�ڳ��Ĵ���ƥ��ָ���Ĵ�  ��Ҫ����ƥ��  ������KMP�㷨
    {
        int jval = 0;
        while (stem[jval] && stem[jval] == m_data[iIndex + jval])                 //��λƥ��
        {
            jval++;
        }
        if (jval >= strlen(stem))
        {
            return iIndex;
        }
    }
    return npos;
}

int MyString::find_first_of(const char* stem,int ipos)                         //�����ַ����е�һ����ָ�����г��ֵ��ַ�λ��
{
    int length = strlen(m_data);
    int iIndex;
    if (ipos >=0 && ipos < length)
    {
        for ( iIndex = ipos;iIndex < length;++iIndex)
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
}

int MyString::find_first_not_of(const char* stem,int ipos)
{
    int length = strlen(m_data);
    int iIndex;
    int acc_equal;                      //ͳ����ȵĴ���
    if (ipos >=0 && ipos < length)
    {
        for ( iIndex = ipos;iIndex < length;++iIndex)
        {
            acc_equal = 0;
            for (int iIndex1 = 0;iIndex1 < strlen(stem);++iIndex1)
            {
                if (m_data[iIndex] == stem[iIndex1])                         //���ƥ����һ���ַ�����������
                {
                    ++acc_equal;
                }
            }
            if (acc_equal == 0)
            {
                return iIndex;
            }

        }
        if (iIndex >= length)
        {
            return npos;
        }
    }    
}

int MyString::rfind(const char *stem,int ipos)                                 //�������
{
    if (ipos == npos)
    {
        ipos = strlen(m_data);
    }
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

int MyString::find_last_of(const char* stem,int ipos)                          //����ԭ����ָ���������һ�������ַ���λ��
{
    int length = strlen(m_data);
    int iIndex;
    if (ipos >=0 && ipos < length)
    {
        for ( iIndex = length - 1;iIndex >= 0;--iIndex)
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
}

int MyString::find_last_not_of(const char* stem,int ipos)                   //����ԭ����ָ���������һ�������ֵ�
{
    int length = strlen(m_data);
    int iIndex;
    int acc_equal;
    if (ipos >=0 && ipos < length)
    {
        for ( iIndex = length - 1;iIndex >= 0;--iIndex)
        {
            acc_equal = 0;
            for (int iIndex1 = 0;iIndex1 < strlen(stem);++iIndex1)
            {
                if (m_data[iIndex] == stem[iIndex1])                         //���ƥ����һ���ַ�����������
                {
                    ++acc_equal;
                }
            }
            if (acc_equal == 0)
            {
                return iIndex;
            }
        }
        if (iIndex >= length)
        {
            return npos;
        }
    } 
}

int _tmain(int argc, _TCHAR* argv[])
{
    MyString str = "haha";
    cout<<str.at(-1)<<endl;

    return 0;
}