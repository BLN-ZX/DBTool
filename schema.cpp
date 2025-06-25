#include <typeinfo>
#include "schema.h"
#include "relmodel.h"

using namespace std;
// using namespace pqxx;


void schema::generate_indexes()
{

  cerr << "Generating indexes...";

  for (auto &type: types)
  {
    // 如果type为0，终止程序
    assert(type);

    // 返回类型一致
    for(auto &r: aggregates) {
      if (type->consistent(r.restype))
          aggregates_returning_type[type].push_back(&r);
    }

    //初始化函数
    for(auto &r: routines) {
      if (!type->consistent(r.restype))
          continue;
      routines_returning_type[type].push_back(&r);

      // 无参数
      if(!r.argtypes.size())
          parameterless_routines_returning_type[type].push_back(&r);
    }

    for (auto &t: tables)
    {
      // 某一列数据类型
      for (auto &c: t.columns())
      {
          if (type->consistent(c.type))
          {
            tables_with_columns_of_type[type].push_back(&t);
            break;
        }
      }
    }

    for (auto &concrete: types)
    {
      if (type->consistent(concrete))
          concrete_type[type].push_back(concrete);
    }

    // 初始化运算符
    for (auto &o: operators)
    {
      if (type->consistent(o.result))
          operators_returning_type[type].push_back(&o);
    }
  }

    // 初始化表
    for (auto &t: tables)
    {
        if (t.is_base_table)
        base_tables.push_back(&t);
    }

  cerr << "done." << endl;

  // 如果为空，则报错
  assert(booltype);
  assert(inttype);
  assert(internaltype);
  assert(arraytype);
}
