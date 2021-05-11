
#ifndef SYNTAX_ANALYZER_H
#define SYNTAX_ANALYZER_H
#include <bits/stdc++.h>
using namespace std;

class Syntax_Analyzer{

    public:

        Syntax_Analyzer();

        Syntax_Analyzer(vector<string>& SS, vector<string>& LL);

        ~Syntax_Analyzer();

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

        void PrintErrorPos(string msg);

        void InsertNode(int father, string label);

        void TreeOutput(int);

        void dfs(int &index, int depth);

        void TransferOutput(bool debug);

        vector<string> SYM;
        vector<string> LABEL;
        int ip;

        vector<vector<int> > tree;
        vector<string> nodeInfo;
        int curIndex;

        vector<string> tokens;
        string exp;
        string ans;

        int procnt;
};

#endif //COMPILER_LAB_SYNTAX_ANALYZER_H
