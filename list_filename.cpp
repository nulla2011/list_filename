#include <iostream>
#include <fstream>
using namespace std;

int lenthdec(char a)
{
    if (a >= '\xA0' && a <= '\xBF')
        return (int(a - '\xA0'));
    else
        return int(a);
}
int main(int argc, char const *argv[])
{
    ifstream manifest("e821415207bf0601ed040cb438d526c478a22873", ios::binary);

    if (!manifest)
    {
        cout << "open ERROR!" << endl;
        system("PAUSE");
        return (-1);
    }
    ofstream out("list.txt");
    int off = 0;
    char cursor;
    //网址获取??????
    while (1)
    {
        manifest.seekg(off);
        manifest.read((char *)(&cursor), 1);

        if (cursor == '\x8F') //end????
        {
            manifest.seekg(off - 1);
            manifest.read((char *)(&cursor), 1);
            if (cursor == '\06')
            {
                manifest.seekg(off - 2);
                manifest.read((char *)(&cursor), 1);

                if (cursor == '\xCD')
                {
                    break;
                }
            }
        }

        /*         if (cursor == '\xB4')//name start
        { 
            off++;
            while(1){
                manifest.read((char *)(&cursor), 1);
                if (cursor == '\xB4')
                    continue;
                if (cursor=='\x93') //name end
                {
                    cout<<endl;
                    break;
                }
                
                cout<<cursor;
                off++;
                
            }
            
        } */

        if (cursor == '\x93')//mark
        {
            manifest.seekg(off - 1);
            manifest.read((char *)(&cursor), 1);
            char b2;
            manifest.seekg(off - 2);
            manifest.read((char *)(&b2), 1);
            if (cursor >= 'a' && cursor <= 'z' && ((b2 >= '0' && b2 <= '9') || (b2 >= 'a' && b2 <= 'z')))//是否扩展名
            {
                int len;
                for (len = 0; len < 60; len++)
                {
                    manifest.seekg(off - len);
                    manifest.read((char *)(&cursor), 1);
                    if (lenthdec(cursor) == len - 1)//文件名长度须等于前一字符的设定
                    {
                        off = off - len;
                        break;
                    }
                }

                for (int i = 0; i < len - 1; i++)
                {
                    manifest.seekg(off + i + 1);
                    manifest.read((char *)(&cursor), 1);
                    out << cursor;
                }
                off = off + len;
                out << endl;
            }
        }

        //cout << cursor << endl;
        off++;
    }

    manifest.close();
    out.close();
    cout << "success" << endl;
    system("PAUSE");
    return 0;
}
