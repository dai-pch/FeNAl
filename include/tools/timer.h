#pragma once
#ifndef FeNAL_TOOLS_TIMER_
#define FeNAL_TOOLS_TIMER_

#include <iostream>
#include <string>
#include <ratio>
#include <chrono>
#include <functional>

namespace FeTools {
    class TestSpeed {
    public:
        
    public:
        TestSpeed(const std::function<int()> func, unsigned test_num = 100,
            const std::string description = std::string())
            : _task_id(++_task_num), _task_description(description), _func(func), _test_num(test_num) {}

        ~TestSpeed() {}

        unsigned set_num(unsigned test_num) {
            _test_num = test_num;
            return _test_num;
        }
        unsigned get_num() const {
            return _test_num;
        }

        void test() const {
            int sum = 0;
            auto _begin = std::chrono::system_clock::now();
            for (int i = 0;i < _test_num;++i)
                sum += _func();
            auto _end = std::chrono::system_clock::now();
            auto _dur = std::chrono::duration_cast<std::chrono::nanoseconds>(_end - _begin);
            std::cout << "Test speed task " << _task_id << ":\n";
            if (!_task_description.empty())
                std::cout << _task_description << '\n';
            std::cout << "Result sum: " << sum << '\n';
            std::cout << "Run " << _test_num << " times." << '\n';
            std::cout << "Mean running time: " << 
                static_cast<double>(_dur.count()) / _test_num << "ns" <<std::endl;
        }

    private:
        static int _task_num;
        const int _task_id;
        const std::string _task_description;
        const std::function<int()> _func;
        unsigned _test_num;
    }; //class TestSpeed

    class RunningTime {
    public:
        
    public:
        RunningTime(const std::string description = std::string())
            : _task_id(++_task_num), _task_description(description) {
            _begin_timer();
        }

        ~RunningTime() {
            auto _end = std::chrono::system_clock::now();
            auto _dur = std::chrono::duration_cast<std::chrono::nanoseconds>(_end - _begin);
            std::cout << "Task " << _task_id << ":\n";
            if (!_task_description.empty())
                std::cout << _task_description << '\n';
            std::cout << "Running time: " << _dur.count() << "ns" <<std::endl;
        }


    private:
        void _begin_timer() {
            _begin = std::chrono::system_clock::now();
        }

    private:
        static int _task_num;
        const int _task_id;
        const std::string _task_description;
        std::chrono::system_clock::time_point _begin;
    }; // class RunningTime
    
    inline int RunningTime::_task_num = 0;
    inline int TestSpeed::_task_num = 0;
} // namespace FeTools

#endif // FeNAL_TOOLS_TIMER_
