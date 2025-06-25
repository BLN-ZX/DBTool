/// @file
/// @brief 随机函数

#ifndef RANDOM_HH
#define RANDOM_HH

#include <random>
#include <utility>
#include <stdexcept>
#include <iterator>
#include <QDebug>
#include <ctime>

namespace smith
{
    // 引用为全局变量
    extern std::mt19937_64 rng;
    //  std::mt19937_64 rng((unsigned int)time(NULL));
}


template<typename T> T& random_pick(std::vector<T>& container)
{
//    std::random_device rd;
//    auto now = std::chrono::system_clock::now(); // 获取当前时间点
//    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()); // 转换为毫秒级的时间戳
//    unsigned int seed = static_cast<unsigned int>(timestamp.count());
//    std::mt19937 engine(seed);

    if (!container.size())
      throw std::runtime_error("No candidates available");

    std::uniform_int_distribution<int> pick(0, container.size()-1);
    //qDebug() << pick(engine);
    //qDebug() << pick(smith::rng);
    return container[pick(smith::rng)];
}

template<typename I>
I random_pick(I beg, I end)
{
    if (beg == end)
      throw std::runtime_error("No candidates available");

    std::uniform_int_distribution<> pick(0, std::distance(beg, end) - 1);
    std::advance(beg, pick(smith::rng));
    return beg;
}

template<typename I>
I random_pick(std::pair<I,I> iters)
{
  return random_pick(iters.first, iters.second);
}

int d6(), d9(), d12(), d20(), d42(), d100();

#endif
