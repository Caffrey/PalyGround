#include "spdlog/spdlog.h"
#include "string"
#include "taskflow/taskflow.hpp"

int main(int argc, char *argv[])
{


    spdlog::info("alksdjflkasjdf");

    std::string fuck = "alskdflaksdjf";
    spdlog::error("alksdjflaksdjflk {}",fuck);


    tf::Executor executor;
    tf::Taskflow taskflow;

    auto [A,B,C,D] = taskflow.emplace(
        []()
        {
            for(int i = 0; i < 10; i++)
            {
                spdlog::info("alksdjflkasjdf");

            }
            
        },
        []()
        {
            for(int i = 0; i < 10; i++)
            {
                spdlog::error("alksdjflkasjdf");

            }
        },
        []()
        {
            for(int i = 0; i < 10; i++)
            {
                spdlog::warn("asd1232");

            }
        },
        []()
        {
            for(int i = 0; i < 10; i++)
            {
                spdlog::warn("fuck");
            }
        });

    C.succeed(A);

    executor.run(taskflow).wait();
    
    return 0;
}