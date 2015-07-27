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
    //下边写普通迭代器类，对容器中的元素可以读写操作
    class Iterator
    {
        char *init;
    public:
        inline Iterator(char* init) {this->init = init;}           //构造函数
        inline bool operator!=(Iterator& it) {return this->init != it.init;}     //迭代器的几个运算符重载
        inline void operator++(int){init = init + 1;}
        inline char operator*() {return *init;}
    };
    //下边写常量迭代器类，对容器中的元素可以读，但是不能修改(其实就是一个const类型)
    class const_Iterator
    {
        char *init;
    public:
        inline const_Iterator(char* init) {this->init = init;}           //构造函数
        inline bool operator!=(const_Iterator& it) {return this->init != it.init;}     //迭代器的几个运算符重载
        inline void operator++(int){init = init + 1;}
        inline const char operator*() {return  *init;}
    };
    MyString(const char* str = NULL);                   //默认构造函数，含有一个默认参数
    MyString(const MyString&,int pos1,int pos2);        //用字符串中的一部分初始化另一个字符串
    MyString(Iterator it1,Iterator it2);     //迭代器构造函数
    MyString(const MyString& other);                    //拷贝构造函数,拷贝了数据，所以说深拷贝
    MyString& operator=(const MyString& other);         //重载赋值运算符
    MyString operator+(const MyString& other) const;    //重载加号运算符
    MyString& operator+=(const MyString &other);        //+号复合赋值运算符
    bool operator==(const MyString& );                  //operator==
    bool operator<(const MyString& );                   //operator<
    char& operator[](int);                              //operator[]
    size_t size() {return strlen(m_data);}              //返回字符串的长度
    size_t length(){return strlen(m_data);}    
    size_t capacity(){return m_capacity;}                //n*16-1内存分配方案
    bool empty();                                 //判断字符串是否为空
    void reserve(size_t num = 0);                 //分配的保留空间  防止重复分配 或者 操作前有足够空间  影响capacity
    void resize(size_t num,char ctem = 0);            //调整字符有效区域的尺寸
    char& at(int num);                           //字符操作
    MyString& assign(const char *);              //c字符串进行赋值
    MyString substr(int pos = 0,int num = -1) const;        //返回pos开始的n个字符的字符串
    void swap(MyString& stem);                             //交换当前字符串与stem字符串的值
    MyString& append(const MyString& other);             //在尾部插入
    MyString& insert(unsigned int ipos,const char *);    //任意位置插入
    MyString& replace(unsigned int ipos,unsigned int num,const char *);       //替换操作
    MyString& erase(unsigned int start,unsigned int final);                   //删除函数
    int find(const char* stem,int ipos = 0);                                  //查找函数
    int find_first_of(const char* stem,int ipos = 0);                         //查找字符串中第一个在指定串中出现的字符位置
    int find_first_not_of(const char* stem,int ipos = 0);
    int rfind(const char *stem,int ipos = -1);                                //反向查找，从左往右数ipos位做为起始的位置，然后从右往左匹配，找到第一个返回位置 
    int find_last_of(const char* stem,int ipos = 0);                          //查找原串在指定串中最后一个出现字符的位置
    int find_last_not_of(const char* stem,int ipos = 0);
    char* Begin() {return m_data;}                        //获得迭代器的起始位置
    char* End(){return m_end;}                          //获得迭代器的尾后位置
    ~MyString();                                        //析构函数
private:
    char* m_data;                                       //指向动态内存的指针
    char* m_end;                                        //尾后指针
    int npos;                                          //查询标志  表示字符串查询失败
    int m_capacity;                                     //表示字符串的容量
};

inline MyString::MyString(const char* str)              //默认构造函数设为内联  不掉用  直接替换
{
    if (!str)
    {
        m_data = new char[16];        //初始分配16个字节
        m_data[0] = '\0';             //第一个位置放字符串结束标识符
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

inline MyString::MyString(const MyString& other,int pos1,int pos2)            //构造函数
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

inline MyString::MyString(Iterator it1,Iterator it2)     //迭代器构造函数
{
    int num = 0;
    Iterator tem1 = it1;                        //保存的临时变量
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


inline MyString::MyString(const MyString& other)       //拷贝函数
{
    if (!other.m_data[0])                                 //在类的成员函数内可以访问同种对象的私有数据(同种类是友元关系)
    {
        m_data = new char[16];        //初始分配16个字节
        m_data[0] = '\0';             //第一个位置放字符串结束标识符
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

inline MyString& MyString::operator=(const MyString& other)     //赋值运算符
{
    if (this != &other)                                         //注意，赋值运算符考虑自赋值
    {
        if (!other.m_data[0])                    //other为空
        {
            if (m_data[0])
            {
                delete [] m_data;
            }
            m_data = new char[16];        //初始分配16个字节
            m_data[0] = '\0';             //第一个位置放字符串结束标识符
            m_end = m_data+1;
            m_capacity = 16 -1;
        }
        else
        {
            if (m_capacity < other.m_capacity)             //左边空间小  重新分配空间并拷贝内容
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
    return *this;                                             //this指针的引用为类对象
}

inline MyString MyString::operator+(const MyString& other)const    //加号重载
{
    //考虑多种情况
    MyString newString;               //加号重载，返回一个值，所以用一个临时变量返回
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

MyString& MyString::operator+=(const MyString &other)                //+号复合赋值运算符
{
    //考虑多种情况
    MyString newString;               //加号重载，返回一个引用，所以用一个临时变量返回给*this
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

inline bool MyString::empty()      //判断字符串是否为空
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
void MyString::reserve(size_t num)                 //分配的保留空间  防止重复分配 或者 操作前有足够空间
{
    MyString newString = *this;                    //原来的内容保存
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

void MyString::resize(size_t num,char ctem)            //调整字符有效区域的尺寸
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

inline char& MyString::operator[](int num)                  //[]运算符不管是否访问越界
{
    if (num >= 0 && num < strlen(m_data))
    {
        return m_data[num];
    }
}


char& MyString::at(int num)                                //字符操作  at函数需要对访问越界进行处理
{
    //访问越界处理code
    if (num>=0 && num<strlen(m_data))
    {
        return m_data[num];
    }
    else
    {
        throw out_of_range("string at function");         //抛出一个异常
    }
}

MyString& MyString::assign(const char *stem)              //c字符串进行赋值
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
    m_data = new char[16*num];                           //申请空间
    strcpy(m_data,stem);                                 //拷贝数据
    m_end = m_data + strlen(stem);
    m_capacity = 16*num - 1;
    return *this;
}

MyString MyString::substr(int pos,int num) const        //返回pos开始的num个字符的字符串  c语言风格
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
void MyString::swap(MyString& stem)                             //交换当前字符串与stem字符串的值
{
    MyString newString;                         //用临时变量交换
    newString = stem;
    stem = *this;
    *this = newString;
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
    //考虑多种情况
    MyString newString;               //加号重载，返回一个引用，所以用一个临时变量返回给*this
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
MyString& MyString::insert(unsigned int ipos,const char *stem)            //任意位置插入函数
{
    MyString newString;
    int iIndex;
    if (ipos >= 0 && ipos < strlen(m_data))                               //ipos在范围内部
    {
        int iLength = strlen(m_data) + strlen(stem) +1;
        int num = 1;
        while (num*16 < iLength)
        {
            ++num;
        }        
        newString.m_data = new char[num*16];     //申请空间
        newString.m_end = m_data + strlen(m_data) + strlen(stem);
        newString.m_capacity = num*16 - 1;
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

int MyString::find(const char* stem,int ipos)                                  //字串查找函数  
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
    if (ipos >=0 && ipos < length)
    {
        for ( iIndex = ipos;iIndex < length;++iIndex)
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
}

int MyString::find_first_not_of(const char* stem,int ipos)
{
    int length = strlen(m_data);
    int iIndex;
    int acc_equal;                      //统计相等的次数
    if (ipos >=0 && ipos < length)
    {
        for ( iIndex = ipos;iIndex < length;++iIndex)
        {
            acc_equal = 0;
            for (int iIndex1 = 0;iIndex1 < strlen(stem);++iIndex1)
            {
                if (m_data[iIndex] == stem[iIndex1])                         //如果匹配上一个字符，返回坐标
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

int MyString::find_last_of(const char* stem,int ipos)                          //查找原串在指定串中最后一个出现字符的位置
{
    int length = strlen(m_data);
    int iIndex;
    if (ipos >=0 && ipos < length)
    {
        for ( iIndex = length - 1;iIndex >= 0;--iIndex)
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
}

int MyString::find_last_not_of(const char* stem,int ipos)                   //查找原串在指定串中最后一个不出现的
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
                if (m_data[iIndex] == stem[iIndex1])                         //如果匹配上一个字符，返回坐标
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