#include "relmodel.h"

map<string, sqltype*> sqltype::typemap;

sqltype * sqltype::get(string n)
{
    if (typemap.count(n))
       return typemap[n];       //包含这个元素
    else
       return typemap[n] = new sqltype(n);   //不包含就新建一个对象
}

bool sqltype::consistent(struct sqltype *rvalue)
{
    return this == rvalue;
}
