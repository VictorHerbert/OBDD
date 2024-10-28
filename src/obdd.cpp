#include <bits/stdc++.h>
#include <string>
#include "obdd.h"

const Node OBDD::FALSE_ADDRESS = 0;
const Node OBDD::TRUE_ADDRESS = 1;

const NodeData OBDD::FALSE_NODE = {.top = OBDD::FALSE_ADDRESS, .low = 0, .high = 0};
const NodeData OBDD::TRUE_NODE =  {.top = OBDD::TRUE_ADDRESS, .low = 1, .high = 1};

OBDD::OBDD(){
    unique_table.reserve(UNIQUE_TABLE_CAPACITY);

    addNodeData(FALSE_NODE);
    addNodeData(TRUE_NODE);

    labels = {"False", "True"};
    varCount = 0;
}

const std::string OBDD::getLabel(Node f){
    return isVariable(f) | isConstant(f) ? labels[f] : "";
}

Node OBDD::top(Node f){return nodes[f].top;}

Node OBDD::high(Node f){return nodes[f].high;}

Node OBDD::low(Node f){return nodes[f].low;}

const Node OBDD::True(){return 1;}

const Node OBDD::False(){return 0;}


bool OBDD::isConstant(Node f){
    return f == True() || f == False();
}
bool OBDD::isVariable(Node f){
    return low(f) == False() && high(f) == True();
}

Node OBDD::addNodeData(NodeData node){
    if(unique_table.find(node) == unique_table.end()){
        nodes.push_back(node);
        unique_table[node] = nodes.size()-1;
    }
    return unique_table[node];
}

Node OBDD::createVar(std::string label){
    varCount++;
    labels.push_back(label);
    return addNodeData({
        .top=nodes.size(), .low=FALSE_ADDRESS, .high=TRUE_ADDRESS
    });
}

NodeVector OBDD::createVars(int n, std::string label){
    NodeVector newnodes(n);
    varCount+=n;    
    for(int i = 0; i < n; i++){
        newnodes[i] = addNodeData({.top=nodes.size(), .low=FALSE_ADDRESS, .high=TRUE_ADDRESS});
        //labels.push_back(label + "[" + std::to_string(i) + "]");
        if(label != "")
            labels.push_back(label + std::to_string(i));
    }
    return newnodes;
}

Node OBDD::ite(Node i, Node t, Node e){
    if(i == OBDD::True()) return t;
    if(i == OBDD::False()) return e;
    if(t == OBDD::True() & e == False()) return i;
    if(t == e) return t;

    NodeData nodeData = {.top=i, .low=e, .high=t};

    if(computed_table.find(nodeData) == unique_table.end()){        
        Node topVar = top(i);
        if(!isConstant(t))
            topVar = std::min(topVar, top(t));
        if(!isConstant(e))
            topVar = std::min(topVar, top(e));

        Node high = ite(coFactorTrue(i,topVar), coFactorTrue(t,topVar), coFactorTrue(e,topVar));
        Node low = ite(coFactorFalse(i,topVar), coFactorFalse(t,topVar), coFactorFalse(e,topVar));

        Node result = (high == low) ? high : addNodeData({.top=topVar, .low=low, .high=high});

        computed_table[nodeData] = result;
            
        return result;
    }
    return computed_table[nodeData];
    
}

Node OBDD::coFactorTrue(Node f, Node x){
    if(isConstant(f) || isConstant(x)){
        return f;
    }
    if(top(f) == x){
        return high(f);
    }else{
        return ite(top(f),
            coFactorTrue(high(f), x),
            coFactorTrue(low(f), x));
    }
}
    
Node OBDD::coFactorTrue(Node f){
    return coFactorTrue(f, top(f));
}

Node OBDD::coFactorFalse(Node f, Node x){

    Node fTop = top(f);
    Node fHigh   = high(f);
    Node fLow    = low(f);

    if(isConstant(f) || isConstant(x) || fTop > x){
        return f;
    }
    if(fTop == x){
        return fLow;
    }else{
        auto T = coFactorFalse(fHigh, x);
        auto F = coFactorFalse(fLow, x);
        return ite(fTop, T, F);
    }
}

Node OBDD::coFactorFalse(Node f){
    return coFactorFalse(f, top(f));
}

Node OBDD::neg(Node f){
    return ite(f, 0, 1);
}

Node OBDD::and2(Node a, Node b){
    return ite(a,b, False());
}

Node OBDD::or2(Node a, Node b){
    return ite(a, True(), b);
}

Node OBDD::xor2(Node a, Node b){
    return ite(a,neg(b), b);
}

Node OBDD::depth(Node f){
    if(isConstant(f))
        return varCount;
    return top(f)-2;
}

NodeVector OBDD::nodeVectorFromInt(int x, int n){
    NodeVector v(n);
    for(int i = 0; i < n; i++)
        v[i] = (x>>i) & 1;

    return v;
}


std::ostream& operator<<(std::ostream& os, OBDD& tree)
{
    os << "Top\t| Low\t| High\t| Label" << std::endl;
    for(int i = 0; i < tree.nodes.size(); i++){
        NodeData node = tree.nodes[i];
        os << node.top << "\t| " << node.low << "\t| " << node.high << "\t| " << tree.getLabel(i) << std::endl;
    }
    os << std::endl;
    
    return os;
}

const int UNVISITED = -1;

std::vector<std::pair<int, bool>> OBDD::dfs(Node root){
    std::vector<std::pair<int,bool>> prev(nodes.size(), std::make_pair(UNVISITED, 0));
    std::cout << "flowchart TD\n";
    std::cout << " 0(0)\n 1(1)\n style 0 stroke:#333,stroke-width:3px\n style 1 stroke:#333,stroke-width:3px\n";
    prev[0] = std::make_pair(0,0); prev[1] = std::make_pair(1,0);
    dfs_recursion(root, prev);
    return prev;
}

void OBDD::printNodeMermaid(Node node, bool level, bool hideZero){
    std::cout << " " << node << "(" << labels[top(node)] << ")\n";

    if(!hideZero || low(node) != False()){
        std::cout << " " <<  node << " -.";
        if(level)
            for(int i = depth(node); i < depth(low(node))-1; i++)
                std::cout << ".";
        std::cout << "-> " << low(node) << "\n";
    }

    if(!hideZero || high(node) != False()){
        std::cout << " " <<  node << " -";
        if(level)
            for(int i = depth(node); i < depth(high(node))-1; i++)
                std::cout << "-";
        std::cout << "-> " << high(node) << "\n";
    }

}

void OBDD::dfs_recursion(Node root, std::vector<std::pair<int,bool>> &prev){
    if(prev[root].first != UNVISITED)
        return;

    //printNodeMermaid(root, false);
    
    dfs_recursion(low(root), prev);
    prev[low(root)] = std::make_pair(root, 0);

    dfs_recursion(high(root), prev);
    prev[high(root)] = std::make_pair(root, 1);
}

/*NodeVector mermaidTree(OBDD tree, std::vector<Node> roots, bool level = true, bool hideZero = true){
    std::vector<bool> marc (tree.nodes.size(), false);
    std::queue<Node> order;
    std::vector<Node> prev(tree.nodes.size(), -1);
    marc[0] = true; marc[1] = true;
        
    std::cout << "flowchart TD\n";
    std::cout << " 0(0)\n 1(1)\n style 0 stroke:#333,stroke-width:3px\n style 1 stroke:#333,stroke-width:3px\n";

    for(Node root : roots){
        order.emplace(root);

        while(!order.empty()){
            Node node = order.front(); order.pop();
            //if(node == tree.True())
            //    break;
            if(marc[node])
                continue;

            marc[node] = true;
            if(prev[tree.low(node)] == -1)
                prev[tree.low(node)] = node;
            if(prev[tree.high(node)] == -1)
                prev[tree.high(node)] = node;

            order.push(tree.low(node));
            order.push(tree.high(node));

            std::cout << " " << node << "(" << labels[tree.top(node)] << ")\n";

            if(!hideZero || tree.low(node) != tree.False()){
                std::cout << " " <<  node << " -.";
                if(level)
                    for(int i = tree.depth(node); i < tree.depth(tree.low(node))-1; i++)
                        std::cout << ".";
                std::cout << "-> " << tree.low(node) << "\n";
            }

            if(!hideZero || tree.high(node) != tree.False()){
                std::cout << " " <<  node << " -";
                if(level)
                    for(int i = tree.depth(node); i < tree.depth(tree.high(node))-1; i++)
                        std::cout << "-";
                std::cout << "-> " << tree.high(node) << "\n";
            }
            
        }
    }

    return prev;
}*/