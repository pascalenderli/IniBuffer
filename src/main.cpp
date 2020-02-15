/**
 * @file main.cpp
 * @author Pascal Enderli
 * @date 2020.01.19
 * @brief Demo Executable for IniFile class,
 */

#include"ini_buffer.h"
#include<time.h>

int main()
{
    //-----------------------------------------------------------------------------------
    // Configuration

    bool run_test = true;
    bool run_benchmark = true;

    //-----------------------------------------------------------------------------------
    // Simple Test

    if(run_test)
    {
    	try
    	{
            IniBuffer ini;
            ini.LoadFile("data/test.ini");
            ini.AddValue("added_test_section", "key1", std::string("text1"));
            ini.WriteFile("results/test_result.ini");

            bool c1 = ini.GetValue<bool>("06_booleans", "key01");
            bool c2 = ini.GetValue<bool>("06_booleans", "key02");
            bool c3 = ini.GetValue<bool>("06_booleans", "key03");
            std::cout<<"06_booleans\n"<<"key01: "<<c1<<"\n"<<"key02: "<<c2<<"\n"<<"key03: "<<c3<<"\n\n";

            int d1 = ini.GetValue<int>("04_integers", "key01");
            int d2 = ini.GetValue<int>("04_integers", "key11");
            int d3 = ini.GetValue<int>("04_integers", "key26");
            std::cout<<"04_integers\n"<<"key01: "<<d1<<"\n"<<"key11: "<<d2<<"\n"<<"key26: "<<d3<<"\n\n";
        }
        catch(IniException& e)
        {
            std::cout<<e.what()<<'\n';
            abort();
        }
    }
    //-----------------------------------------------------------------------------------
    // Benchmark

    if(run_benchmark)
    {
        clock_t t;
        clock_t dt;
        try
        {
            for(int i = 1; i<=1000; ++i)
            {
                dt = clock();
                IniBuffer ini_performance;
                ini_performance.LoadFile("data/performance.ini");
                ini_performance.WriteFile("results/performance_result.ini");
                dt = clock()-dt;
                t = t + (dt-t)/i;
                ini_performance.Clear();
            }
            std::cout<<"Average parsing time: "<<1000*((float)t)/(CLOCKS_PER_SEC)<<" ms\n";
        }
        catch(IniException& e)
        {
            std::cout<<e.what()<<'\n';
            abort();
        }
    }
    return 0;
}
