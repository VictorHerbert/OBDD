#include <bits/stdc++.h>

using FuncPtr = void(*)();

namespace vtest{
    int test_count = 0;
    int error_count = 0;

    int success_count = 0;
    int not_implemented_count = 0;
    int fail_count = 0;

    struct TestData{
        FuncPtr function;
        std::string name;
        int assert_count;
        int error_count;
    };

    std::vector<TestData> registered_functions;
    int current_test_idx;
}

#define T_FUNC(func_name)  \
namespace vtest{ \
    void func_name##_register () __attribute__((constructor)); \
    void func_name(); \
    void func_name##_register(){ \
        vtest::registered_functions.push_back({ \
            .function = func_name, \
            .name = #func_name, \
            .assert_count = 0, \
            .error_count = 0, \
        }); \
    } \
}\
void vtest::func_name()

#define T_ASSERT(value, expected) \
    vtest::registered_functions[vtest::current_test_idx].assert_count++; \
    if(value != expected) \
        vtest::registered_functions[vtest::current_test_idx].error_count++;
//std::cout << "ASSERT at line " << __LINE__ << ": expected " << expected << ", got " << value << std::endl;

const char RED[] = "\033[1;31m";
const char GREEN[] = "\033[92m";
const char YELLOW[] = "\033[93m";
const char RESET[] = "\033[0m";

int main(){
    std::cout << "Testing starting...\n" << std::endl;

    for(vtest::current_test_idx = 0; vtest::current_test_idx < vtest::registered_functions.size(); vtest::current_test_idx++){
        vtest::TestData *data = &vtest::registered_functions[vtest::current_test_idx];

        std::cout << data->name << ": ";
        data->function();
        if(data->error_count != 0){
            vtest::fail_count++;
            std::cout << RED << "FAILED\n" << RESET;
        }
        else if(data->assert_count == 0){
            vtest::not_implemented_count++;
            std::cout <<  YELLOW << "NOT_IMPLEMENTED\n" << RESET;
        }
        else{
            vtest::success_count++;
            std::cout << GREEN << "PASSED\n" << RESET;
        }
        vtest::test_count++;
    }

    std::cout << "\n" << vtest::success_count << "/" << vtest::test_count << " tests passed" << std::endl;

    return 0;
}

/*

*/