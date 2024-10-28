#ifndef OBDD_H
#define OBDD_H

#include <vector>
#include <map>
#include <unordered_map>

#include<fstream>

typedef std::size_t Node;
typedef std::vector<Node> NodeVector;

struct NodeData{
    Node top, low, high;

    bool operator == (const NodeData &other) const {
        return 
            (low == other.low) &&
            (high == other.high) &&
            (top == other.top);
    }
};

template<std::size_t TABLE_SIZE>
struct NodeDataHashFunction{
    std::size_t operator()(const NodeData& node) const{
        return ((node.low<<16) + (node.high<<32) + (node.top<<0)) % TABLE_SIZE;
    }
};


class OBDD {
protected:
    const static std::size_t UNIQUE_TABLE_CAPACITY = 1e4+3;
    const static NodeData FALSE_NODE;
    const static NodeData TRUE_NODE;

    std::unordered_map<NodeData, Node, NodeDataHashFunction<UNIQUE_TABLE_CAPACITY>> unique_table;
    std::unordered_map<NodeData, Node, NodeDataHashFunction<UNIQUE_TABLE_CAPACITY>> computed_table;    
    std::vector<std::string> labels;

    Node addNodeData(NodeData node);
    Node ite(Node i, Node t, Node e);

    int varCount;

    const static Node FALSE_ADDRESS;
    const static Node TRUE_ADDRESS;
    
public:
    std::vector<NodeData> nodes;

    OBDD();

    const std::string getLabel(Node f);

    Node top(Node f);

    Node high(Node f);

    Node low(Node f);

    const Node True();

    const Node False();

    bool isConstant(Node f);

    bool isVariable(Node x);

    Node depth(Node f);

    Node createVar(std::string label = "");

    NodeVector createVars(int n, std::string label = "");

    Node coFactorTrue(Node f, Node x);

    Node coFactorFalse(Node f, Node x);

    Node coFactorTrue(Node f);

    Node coFactorFalse(Node f);

    Node neg(Node f);

    Node and2(Node a, Node b);

    Node or2(Node a, Node b);

    Node xor2(Node a, Node b);

    //Node nand2(Node a, Node b);

    //Node nor2(Node a, Node b);

    //Node xnor2(Node a, Node b);

    //string visualizeBDD(Node root);

    static NodeVector nodeVectorFromInt(int x, int n);

    std::vector<std::pair<int, bool>> dfs(Node root);

    void dfs_recursion(Node root, std::vector<std::pair<int,bool>> &prev);

    void printNodeMermaid(Node node, bool level = true, bool hideZero = false);
};

std::ostream& operator << (std::ostream& os, OBDD& tree);

#endif