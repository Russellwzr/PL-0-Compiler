
#ifndef SYNTAX_ANALYZER_H
#define SYNTAX_ANALYZER_H
#include <bits/stdc++.h>
using namespace std;

class Syntax_Analyzer{

    public:

        Syntax_Analyzer();

        Syntax_Analyzer(vector<string>& SS, vector<string>& LL);

        ~Syntax_Analyzer();

        //********************************************************************************
        //
        // Recursive Descent Subroutines For Syntax Analysis
        //
        //

        bool Program();

        bool SubProgram(int);

        bool ConstantDeclare(int);

        bool ConstantDefine(int);

        //bool UnsignedNum(int);

        bool VariableDeclare(int);

        //bool Identifier(int);

        bool ProcedureDeclare(int);

        bool ProcedureHead(int);

        bool Sentence(int);

        bool Assignment(int);

        bool Combined(int);

        bool Condition(int);

        bool Expression(int);

        bool Item(int);

        bool Factor(int);

        //bool AddorSubOp(int);

        //bool MulorDivOp(int);

        bool RelationOp(int);

        bool IfSentence(int);

        bool CallSentence(int);

        bool WhileSentence(int);

        bool ReadSentence(int);

        bool WriteSentence(int);

        //bool Letter(int);

        //bool Num(int);

        bool EmptySentence(int);

        //
        //
        //
        //********************************************************************************

        // Output Error Information
        void PrintErrorPos(string msg);

        // Insert a node to the syntax tree
        void InsertNode(int father, string label);

        // Output the syntax tree
        void TreeOutput(int);

        // Depth first search
        void dfs(int &index, int depth);

        // Transfer the output for debugging
        void TransferOutput(bool debug);

    private:

        vector<string> SYM;         // word type (KEY/IDENT/NUM)
        vector<string> LABEL;       // word itself (lexical analyzer's output)
        int ip;                     // index pointer for LABEL and SYM

        vector<vector<int> > tree;  // adjacency list for syntax tree
        vector<string> nodeInfo;    // tree node information (node name)
        int curIndex;               // current tree node number (node id)

        // for output transfer
        vector<string> tokens;
        string exp;
        string ans;

        // procedure declare level ( if procnt >= 4 then print syntax error.)
        int procnt;

};

#endif //COMPILER_LAB_SYNTAX_ANALYZER_H
