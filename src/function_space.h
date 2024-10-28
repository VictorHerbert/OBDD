#include "obdd.h"

class FunctionSpace : public OBDD {
public:

    std::tuple<Node, Node> half_adder(Node a, Node b){
        return std::make_tuple(xor2(a,b), and2(a,b));
    }

    std::tuple<Node, Node> full_adder(Node a, Node b, Node cin){
        auto [s1, c1] = half_adder(a,b);
        auto [s2, c2] = half_adder(s1,cin);
        return std::make_tuple(s2, or2(c1,c2));
    }

    NodeVector adder(NodeVector a, NodeVector b, Node cin = FALSE_ADDRESS){        
        NodeVector s(a.size() + 1);
        Node carry = cin;
        for(int i = 0; i < std::max(a.size(), b.size()); i++){
            auto result = full_adder(
                i >= a.size() ? False() : a[i],
                i >= b.size() ? False() : b[i],
                carry);
            s[i] = std::get<0>(result);
            carry = std::get<1>(result);
        }
        s[a.size()] = carry;
        return s;
    }

    Node eq(NodeVector a, NodeVector b){
        return neg(acum(bitwise(a,b, xor2), or2, False()));
    }

    Node acum(NodeVector v, Node (OBDD::*op)(Node, Node), Node def){
        for(Node node : v)
            def = (this->*op)(def, node);
        return def;
    }


    NodeVector bitwise(NodeVector a, NodeVector b, Node (OBDD::*op)(Node, Node)){
        NodeVector out(a.size());
        for(int i = 0; i < a.size(); i++)
            out[i] = (this->*op)(a[i], b[i]);
        return out;
    }

    Node isOneHot(NodeVector v){
        Node countZero = True(), countOne = False();

        for(Node node : v){
            countOne = or2(and2(countZero, node), and2(countOne, neg(node)));
            countZero = and2(countZero, neg(node));
        }

        return countOne;
    }

};