#include <string>
#include <iostream>

#ifndef PROD_HH
#define PROD_HH

// 运行语法树的基类
struct prod_visitor {
  virtual void visit(struct prod *p) = 0;
  virtual ~prod_visitor() { }
};

/// Base class for AST nodes
// 语法树节点的基类
struct prod {
  // 父结构体
  struct prod *pprod;

  // 范围对象到模型列/表引用的可见性。
  struct scope *scope;

  // AST中该节点的层次。根节点为0。
  int level;

  // 重复次数，生成子节点的过程中可能会产生错误
  long retries = 0;

  // 允许生成错误的最大次数
  long retry_limit = 100;
  prod(prod *parent);

  // 根据语法树层次换行和缩进
  virtual void indent(std::ostream &out);

  // 输出SQL语句
  virtual void out(std::ostream &out) = 0;

  // 使用阻抗匹配代码检查此产品是否已被列入黑名单，并引发异常
  virtual void match();

  // 访问AST的参观者。确保在派生类时访问所有子产品
  virtual void accept(prod_visitor *v) { v->visit(this); }

  // 返回一个错误给生成器
  virtual void fail(const char *reason);

  // 增加重试次数，并在超过retry_limit时引发异常
  void retry();
};

// 内联函数
inline std::ostream& operator<<(std::ostream& s, prod& p)
{
  p.out(s); return s;
}

#endif
