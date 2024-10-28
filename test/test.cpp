#include <bits/stdc++.h>

#include "test.h"
#include "../src/function_space.h"

T_FUNC(one_hot_test){
    FunctionSpace s;
    NodeVector v = FunctionSpace::nodeVectorFromInt(0b0001, 4);
    T_ASSERT(s.isOneHot(v), s.True());
}


/*T_FUNC(main_test){
    FunctionSpace s;
    NodeVector a = s.createVars(3, "a");
    NodeVector b = s.createVars(3, "b");
    NodeVector c = s.adder(a,b);

    NodeVector it = FunctionSpace::nodeVectorFromInt(7, 4);

    Node eq = s.eq(c, it);
    Node non_zero = s.and2(s.eq(b,FunctionSpace::nodeVectorFromInt(0, 9)), s.eq(a,FunctionSpace::nodeVectorFromInt(0, 9)));
    Node expr = s.and2(eq, non_zero);

    auto v = s.dfs(eq);
    
    int prev = 1;
    while(v[prev].first != -1){
        auto [next, val] = v[prev];
        std::cout << s.getLabel(s.top(next)) << " " << val << std::endl;
        prev = next;
    }

    //std::cout << s << std::endl;

    
    return 0;
}*/