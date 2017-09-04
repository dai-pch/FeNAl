#pragma once
#ifndef FeNAL_TOOLS_TIMER_
#define FeNAL_TOOLS_TIMER_

#include <iostream>
#include <string>
#include <ratio>
#include <chrono>

namespace FeTools {
    class RunningTime {
    public:
        
    public:
        RunningTime(): _task_id(++_task_num) {
            _begin_timer();
        }

        RunningTime(const std::string description)
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
    }; //class RunningTime
    
    inline int RunningTime::_task_num = 0;
} // namespace FeTools

#endif // FeNAL_TOOLS_TIMER_
