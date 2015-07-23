// MyString.cpp : 定义控制台应用程序的入口点。
//
/*****************************************
**功能：自己实现C++ STL的string容器类
**作者：谢凡凡
**时间：2015-07-21  02：20
*****************************************/
#include "stdafx.h"
#include <iostream>
#include <string>
#include <iomanip>                                       //后边用到函数setw设置域宽，所以包含该头文件

using namespace std;

//自己尝试写的一个string容器类
class MyString
{
    friend ostream& operator<<(ostream&,MyString&);      //输出运算符重载，友元函数
    friend istream& operator>>(istream&,MyString&);      //输入运算符重载
public:
    MyString(const char* str = NULL);                   //默认构造函数，含有一个默认参数
    MyString(const MyString& other);                    //拷贝构造函数,拷贝了数据，所以说深拷贝
    MyString& operator=(const MyString& other);         //重载赋值运算符
    MyString operator+(const MyString& other) const;     //重载加号运算符
    bool operator==(const MyString& );                  //operator==
    bool operator<(const MyString& );                   //operator<
    char& operator[](int);                     //operator[]
    size_t size() {return strlen(m_data);}
    MyString& append(const MyString& other);             //在尾部插入
    MyString& insert(unsigned int ipos,const char *);    //任意位置插入
    MyString& replace(unsigned int ipos,unsigned int num,const char *);       //替换操作
    MyString& erase(unsigned int start,unsigned int final);                   //删除函数
    int find(const char* stem,int ipos = 0);                                  //查找函数
    int find_first_of(const char* stem,int ipos = 0);                         //查找字符串中第一个在指定串中出现的字符位置
    int rfind(const char *stem,int ipos = -1);                                     //反向查找，从左往右数ipos位做为起始的位置，然后从右往左匹配，找到第一个返回位置
    int npos;                                           //查询标志  表示字符串查询失败
    //下边写迭代器类
    class Iterator
    {
        char *init;
    public:
        inline Iterator(char* init) {this->init = init;}           //构造函数
        inline bool operator!=(Iterator& it) {return this->init != it.init;}     //迭代器的几个运算符重载
        inline void operator++(int){init = init + 1;}
        inline char operator*() {return *init;}
    };
    char* Begin() {return m_data;}                        //获得迭代器的起始位置
    char* End(){return m_end;}                          //获得迭代器的尾后位置
    ~MyString();                                        //析构函数
private:
    char* m_data;                                       //指向动态内存的指针
    char* m_end;                                        //尾后指针
};

inline MyString::MyString(const char* str)              //默认构造函数设为内联  不掉用  直接替换
{
    if (!str)
    {
        m_data =NULL;
        m_end = NULL;
    }
    else
    {
        m_data = new char[strlen(str)+1];
        m_end = m_data + strlen(str);                  //尾后迭代器位置
        strcpy(m_data,str);
    }
    npos = -1;
}

inline MyString::MyString(const MyString& other)       //拷贝函数
{
    if (!other.m_data)                                 //在类的成员函数内可以访问同种对象的私有数据(同种类是友元关系)
    {
        m_data = NULL;
        m_end = NULL;
    }
    else
    {
        m_data = new char[strlen(other.m_data)+1];
        m_end = m_data + strlen(other.m_data);
        strcpy(m_data,other.m_data);
    }
    npos = -1;
}

inline MyString& MyString::operator=(const MyString& other)     //赋值运算符
{
    if (this != &other)                                         //注意，赋值运算符考虑自赋值
    {
        delete [] m_data;
        m_end = m_data;
        if (!other.m_data)
        {
            m_data = NULL;
            m_end = NULL;
        }
        else
        {
            m_data = new char[strlen(other.m_data)+1];
            m_end = m_data + strlen(other.m_data);
            strcpy(m_data,other.m_data);
        }
    }
    return *this;                                             //this指针的引用为类对象
}

inline MyString MyString::operator+(const MyString& other)const    //加号重载
{
    //考虑多种情况
    MyString newString;               //加号重载，返回一个值，所以用一个临时变量返回
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
        newString.m_end = newString.m_data + strlen(m_data) + strlen(other.m_data);
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
    else if (strlen(m_data) == 0 && strlen(other.m_data) == 0)          //表示两个都为空
    {
        return false;
    }

    int iIndex = 0;
    while (m_data[iIndex] && other.m_data[iIndex])              //相同长度逐个比较字符
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
    if (!m_data[iIndex] && other.m_data[iIndex])                 //第一个字符串比第二个字符串短 但前边相同  eg: xiefanfan xiefanfanaa
    {
        return true;
    }
    else
    {
        return false;
    }	
}

inline char& MyString::operator[](int num)
{
    if (num < 0 || num >= strlen(m_data))
    {
        cout<<"string subscript out of range"<<endl;
    }
    if (num>=0 && num<strlen(m_data))
    {
        return m_data[num];
    }
}

ostream& operator<<(ostream& os,MyString& mstem)            //友元函数，表示该函数不是该类的成员，但是可以操作类的私有数据
{
    os<<mstem.m_data;                                       //不要添加格式控制符，只做输出
    return os;                                              //return支持连续<<
}

istream& operator>>(istream& is,MyString& mstem)
{
    char temp[255];                                         //临时缓冲空间
    is>>setw(255)>>temp;
    mstem = temp;                                           //使用赋值运算符
    return is;											    //return支持连续>>
}

MyString::~MyString()                                      //析构函数
{
    if (m_data)
    {
        delete [] m_data;
        m_data = NULL;
        m_end = NULL;
    }
}

MyString& MyString::append(const MyString& other)             //在尾部插入  参考operator+函数
{
    MyString newString;                                       //申请一个临时空间  将原始字符串保存起来
    if (!other.m_data)
    {
        return *this;
    }
    else if (!m_data)
    {
        m_data = new char[strlen(other.m_data) + 1];
        m_end = m_data + strlen(other.m_data);
        strcpy(m_data,other.m_data);
        return *this;
    }
    else
    {
        newString = *this;
        m_data = new char[strlen(newString.m_data) + strlen(other.m_data) +1];
        m_end = m_data + strlen(newString.m_data) + strlen(other.m_data);
        strcpy(m_data,newString.m_data);
        strcat(m_data,other.m_data);
        return *this;
    }
}
MyString& MyString::insert(unsigned int ipos,const char *stem)            //任意位置插入函数
{
    MyString newString;
    int iIndex;
    if (ipos >= 0 && ipos < strlen(m_data))                               //ipos在范围内部
    {
        newString.m_data = new char[strlen(m_data) + strlen(stem) +1];     //申请空间
        newString.m_end = m_data + strlen(m_data) + strlen(stem);
        for (iIndex = 0;iIndex < strlen(m_data) + strlen(stem);++iIndex)
        {
            if (iIndex < ipos)                                             //拷贝原始串  ipos前的数据                        
            {
                newString.m_data[iIndex] = m_data[iIndex];
            }
            else
            {
                if (iIndex >= ipos && iIndex < (ipos + strlen(stem)))     //添加子串
                {
                    newString.m_data[iIndex] = stem[iIndex - ipos];
                }
                else                                                      //添加原来字符串后边的串
                {
                    newString.m_data[iIndex] = m_data[iIndex - strlen(stem)];
                }
            }
        }
        newString.m_data[iIndex] = NULL;                                      //最后一位置空 表示字符串结束
    }
    *this = newString;
    return *this;
}

MyString& MyString::replace(unsigned int ipos,unsigned int num,const char *stem)            //替换操作                                   
{
    MyString newString;                                                   //临时缓存字符串
    int iIndex;
    if (ipos >= 0 && ipos < strlen(m_data) )
    {
        int iNewlen = strlen(m_data) + strlen(stem) - num;                //新的长度
        newString.m_data = new char[iNewlen + 1];
        newString.m_end = m_data + iNewlen;
        for (iIndex = 0;iIndex < iNewlen;++iIndex)
        {
            if (iIndex < ipos)                                             //拷贝ipos索引前的字符
            {
                newString.m_data[iIndex] = m_data[iIndex];
            }
            else if (iIndex >= ipos && iIndex < ipos + strlen(stem))       //拷贝替换的串
            {
                newString.m_data[iIndex] = stem[iIndex - ipos];
            }
            else                                                           //拷贝原串剩余的部分
            {
                newString.m_data[iIndex] = m_data[iIndex - strlen(stem) + num];       
            }
        }
    }
    newString.m_data[iIndex] = '\0';                                   //字符串最后置为0                
    *this = newString;  
    return *this;
}
/////////////////////////////此处应该用的迭代器/////////////////////////////////////////////
MyString& MyString::erase(unsigned int start,unsigned int final)                   //删除函数
{
    if (start >= 0 && start < strlen(m_data) && final >= 0 && final < strlen(m_data) && start <= final)
    {
        int iIndex;
        for (iIndex = final;iIndex < strlen(m_data);++iIndex )
        {
            m_data[iIndex - final + start] = m_data[iIndex];                       //后边覆盖前边
        }
        m_data[iIndex - (final - start)] = '\0';
        m_end = m_data + iIndex - (final - start);
    }
    return *this;
}

int MyString::find(const char* stem,int ipos )                                  //字串查找函数  
{
    if (ipos + strlen(stem) > strlen(m_data))                                      //超出范围
    {
        return npos;
    }
    for (int iIndex = ipos;iIndex < strlen(m_data) - strlen(stem);++iIndex)        //在长的串中匹配指定的串  若要快速匹配  可以用KMP算法
    {
        int jval = 0;
        while (stem[jval] && stem[jval] == m_data[iIndex + jval])                 //逐位匹配
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

int MyString::find_first_of(const char* stem,int ipos)                         //查找字符串中第一个在指定串中出现的字符位置
{
    int length = strlen(m_data);
    int iIndex;
    for ( iIndex = 0;iIndex < length;++iIndex)
    {
        for (int iIndex1 = 0;iIndex1 < strlen(stem);++iIndex1)
        {
            if (m_data[iIndex] == stem[iIndex1])                         //如果匹配上一个字符，返回坐标
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
int MyString::rfind(const char *stem,int ipos)                                 //反向查找
{
    if (ipos == npos)
    {
        ipos = strlen(m_data);
    }
    for (int iIndex = ipos;iIndex >= 0;--iIndex)
    {
        int slen = strlen(m_data);                         //原串长度
        int slen1 = strlen(stem);                          //匹配串长度
        if ((slen - iIndex) >= slen1)                      //当前字符后边字符数大于等于带匹配串
        {
            int tem = 0;
            while (m_data[iIndex + tem] == stem[tem] && tem < slen1)        //逐项匹配
            {
                ++tem;
            }
            if (tem >= slen1)
            {
                return iIndex;
            }
        }
    }
    return npos;                                             //如果上边没有匹配上，表示不存在匹配项
}


int _tmain(int argc, _TCHAR* argv[])
{
    MyString str("fasfd");
    str.erase(0,2);
    MyString::Iterator start = str.Begin();
    MyString::Iterator end = str.End();
    while(start != end)
    {
        cout<<*start<<endl;
        start++;
    }

    return 0;
}