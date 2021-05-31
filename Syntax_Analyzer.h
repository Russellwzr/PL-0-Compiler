
#ifndef SYNTAX_ANALYZER_H
#define SYNTAX_ANALYZER_H
#include <bits/stdc++.h>
using namespace std;


struct tableElement{
    string NAME;
    string KIND;
    int LEVEL;
    int VAL;
    int ADR;
    int NEXT;
    int CODEINDEX;
    tableElement(){
        NAME = KIND = "";
        LEVEL = VAL = ADR = 0;
        NEXT = -1;
        CODEINDEX = 0;
    }
    tableElement(string NA, string KI, int LE, int VA, int AD, int NE){
        NAME = NA;
        KIND = KI;
        LEVEL = LE;
        VAL = VA;
        ADR = AD;
        NEXT = NE;
        CODEINDEX = 0;
    }
    tableElement(string NA, string KI, int LE, int VA, int AD, int NE, int CO){
        NAME = NA;
        KIND = KI;
        LEVEL = LE;
        VAL = VA;
        ADR = AD;
        NEXT = NE;
        CODEINDEX = CO;
    }
};

struct tableList{
    vector<tableElement> t;
    int preIdx1;
    int preIdx2;
    int size;
    tableList(){
        preIdx1 = preIdx2 = -1;
        size = 3;
    }
    tableList(int p1, int p2){
        preIdx1 = p1;
        preIdx2 = p2;
        size = 3;
    }
};

struct Instruction{
    string f;
    int l;
    int a;
    Instruction(){
        f = "";
        l = a = 0;
    }
    Instruction(string ff,int ll, int aa){
        f = ff;
        l = ll;
        a = aa;
    }
};

struct Address{
    int l;
    int a;
    Address(){
        l = a = 0;
    }
    Address(int ll, int aa){
        l = ll;
        a = aa;
    }
};


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

        bool SubProgram(int, bool);

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

        void TableOutput();

        Address CalAddress(string name, bool flag);

        bool IsRedefined(string name, int idx);

        void CodeOutput();

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
        //int procnt;

        vector<tableList> TABLE;
        int LEV;                    // LEV should < 4
        int TX;
        int curADR;

        vector<Instruction> CODE;
        //bool MAIN;

};

#endif //COMPILER_LAB_SYNTAX_ANALYZER_H
