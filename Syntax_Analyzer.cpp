#include "Syntax_Analyzer.h"
#include "utils.h"

const int DX = 3;

Syntax_Analyzer::Syntax_Analyzer(){
    ip = 0;
    curIndex = 0;
    SYM.clear();
    LABEL.clear();
    tree.clear();
    nodeInfo.clear();
    tokens.clear();
    exp = "";
    ans = "";
    LEV = 0;
    TX = 0;
    curADR = DX;
    TABLE.push_back(tableList());
}

Syntax_Analyzer::Syntax_Analyzer(vector<string>& SS, vector<string>& LL){
    ip = 0;
    curIndex = 0;
    SYM = SS;
    LABEL = LL;
    tree.clear();
    nodeInfo.clear();
    tokens.clear();
    exp = "";
    ans = "";
    LEV = 0;
    TX = 0;
    curADR = DX;
    TABLE.push_back(tableList());
}

Syntax_Analyzer::~Syntax_Analyzer(){
    ip = 0;
    curIndex = 0;
    SYM.clear();
    LABEL.clear();
    tree.clear();
    nodeInfo.clear();
    tokens.clear();
    exp = "";
    ans = "";
    LEV = 0;
    TX = 0;
    curADR = DX;
}

//***********************************************************//
//                                                           //
//               functions for syntax analyzer               //
//                                                           //
//***********************************************************//

// Insert a node to the syntax tree
// int fatherID : the father's node id
// string label : the node name
void Syntax_Analyzer::InsertNode(int fatherID, string label){
    // add new node
    vector<int> tmp;
    tree.push_back(tmp);
    // add node name
    nodeInfo.push_back(label);
    // add link : father ----> this node
    tree[fatherID].push_back(curIndex);
    // node number ++
    curIndex ++;
}

// <程序>→<分程序>.
bool Syntax_Analyzer::Program(){

    string msg = "PROGRAM";

    // Insert root node to the syntax tree
    int fatherID = curIndex;
    vector<int> tmp;
    tree.push_back(tmp);
    nodeInfo.push_back("PROGRAM");
    curIndex ++;

    // Error : Out of range
    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    // Jump to MAIN
    GEN("JMP", 0, 0);

    // Judge <分程序>
    if(!SubProgram(fatherID, true, 0, 0)){
        PrintErrorPos(msg);
        return false;
    }

    // Error : Out of range
    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    // Judge .
    if(LABEL[ip] == "." && ip == LABEL.size() - 1){
        InsertNode(fatherID, LABEL[ip]);
    }
    else{
        PrintErrorPos(msg);
        return false;
    }

    return true;

}

// <分程序>→ [<常量说明部分>][<变量说明部分>][<过程说明部分>]<语句>
bool Syntax_Analyzer::SubProgram(int f, bool MAIN, int ProcedureEntryIdx1, int ProcedureEntryIdx2){
    string msg = "SUBPROG";
    // Insert current node to the syntax tree
    int fatherID = curIndex;
    InsertNode(f, msg);
    // Error : Out of range
    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }
    // Judge [<常量说明部分>]
    if(LABEL[ip] == "CONST"){
        if(!ConstantDeclare(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    // Judge [<变量说明部分>]
    if(LABEL[ip] == "VAR"){
        if(!VariableDeclare(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    // Judge [<过程说明部分>]
    if(LABEL[ip] == "PROCEDURE"){
        if(!ProcedureDeclare(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    // allocate space for this procedure
    GEN("INT", 0, 3);
    int entryIdx = CODE.size() - 1;
    // write back the entry address of MAIN
    if(MAIN){
        MAIN = false;
        CODE[0].a = entryIdx;
    }
    else{
        // write back the procedure (in the table list) address
        TABLE[ProcedureEntryIdx1].t[ProcedureEntryIdx2].ADR = entryIdx;
    }
    // Judge <语句>
    if(!Sentence(fatherID)){
        PrintErrorPos(msg);
        return false;
    }
    // write back the "INT" Instruction's "a"
    CODE[entryIdx].a = TABLE[TX].size;
    // procedure return
    GEN("OPR", 0, 0);
    return true;
}

// <常量说明部分> → CONST<常量定义>{ ,<常量定义>};
bool Syntax_Analyzer::ConstantDeclare(int f){

    // Insert current node to the syntax tree
    int fatherID = curIndex;
    string msg = "CONSTANTDECLARE";
    InsertNode(f, msg);

    // Error : Out of range
    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    // Judge CONST
    if(LABEL[ip] != "CONST"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }
    // Judge <常量定义>
    if(!ConstantDefine(fatherID)){
        PrintErrorPos(msg);
        return false;
    }
    // Judge { ,<常量定义>}
    while(LABEL[ip] == ","){
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
        if(!ConstantDefine(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    // Judge ;
    if(LABEL[ip] != ";"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
        return true;
    }
}

// <常量定义> → <标识符>=<无符号整数>
bool Syntax_Analyzer::ConstantDefine(int f){

    tableElement e;
    // Insert current node to the syntax tree
    int fatherID = curIndex;
    string msg = "CONSTANTDEFINE";
    InsertNode(f, msg);

    // Error : Out of range
    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    // Judge <标识符>
    if(SYM[ip] != "IDENT"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        e.NAME = LABEL[ip];
        e.KIND = "CONSTANT";
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }

    // Judge =
    if(LABEL[ip] != "="){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }

    // Judge <无符号整数>
    if(SYM[ip] != "NUM"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        // string to int
        e.VAL = atoi(LABEL[ip].c_str());
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }

    // Judge re-defined
    if(!IsRedefined(e.NAME, TX)){
        TABLE[TX].t.push_back(e);
    }
    else{
        string tmpmsg = "Error: re-define " + e.NAME + " !";
        PrintErrorPos(tmpmsg);
        return false;
    }

    return true;

}

// <变量说明部分> → VAR<标识符>{ ,<标识符>};
bool Syntax_Analyzer::VariableDeclare(int f){

    // Insert current node to the syntax tree
    int fatherID = curIndex;
    string msg = "VARIABLEDECLARE";
    InsertNode(f, msg);

    // Error : Out of range
    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    // VAR
    if(LABEL[ip] != "VAR"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }

    // <标识符>
    if(SYM[ip] != "IDENT"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        // add to TABLE
        tableElement e;
        e.NAME = LABEL[ip];
        e.KIND = "VARIABLE";
        e.LEVEL = LEV;
        e.ADR = curADR ++;
        // Judge re-defined
        if(!IsRedefined(e.NAME, TX)){
            TABLE[TX].t.push_back(e);
            TABLE[TX].size ++;
        }
        else{
            string tmpmsg = "Error: re-define " + e.NAME + " !";
            PrintErrorPos(tmpmsg);
            return false;
        }
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }

    // { ,<标识符>}
    while(LABEL[ip] == ","){
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
        if(SYM[ip] != "IDENT"){
            PrintErrorPos(msg);
            return false;
        }
        else{
            // add to TABLE
            tableElement e;
            e.NAME = LABEL[ip];
            e.KIND = "VARIABLE";
            e.LEVEL = LEV;
            e.ADR = curADR ++;
            // Judge re-defined
            if(!IsRedefined(e.NAME, TX)){
                TABLE[TX].t.push_back(e);
                TABLE[TX].size ++;
            }
            else{
                string tmpmsg = "Error: re-define " + e.NAME + " !";
                PrintErrorPos(tmpmsg);
                return false;
            }
            InsertNode(fatherID, LABEL[ip]);
            ip ++;
        }
    }

    // ;
    if(LABEL[ip] != ";"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
        return true;
    }

}

// <过程说明部分> → <过程首部><分程序>;{<过程说明部分>}
bool Syntax_Analyzer::ProcedureDeclare(int f){
    // Judge procedure declare level
    LEV ++;
    if(LEV >= 4){
        cerr<<"Error: Too Many Procedure Nesting Levels!"<<endl;
        return false;
    }
    // Insert current node to the syntax tree
    int fatherID = curIndex;
    string msg = "PROCEDUREDECLARE";
    InsertNode(f, msg);

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }
    // ProcedureEntryIdx1: index1 (1D) of this procedure in TABLE
    // ProcedureEntryIdx2: index2 (2D) of this procedure in TABLE
    int ProcedureEntryIdx1, ProcedureEntryIdx2;
    // <过程首部>
    if(!ProcedureHead(fatherID, ProcedureEntryIdx1, ProcedureEntryIdx2)){
        PrintErrorPos(msg);
        return false;
    }

    // <分程序>
    if(!SubProgram(fatherID, false, ProcedureEntryIdx1, ProcedureEntryIdx2)){
        PrintErrorPos(msg);
        return false;
    }

    // ;
    if(LABEL[ip] != ";"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }

    // reset LEV
    LEV--;
    int idx1 = TABLE[TX].preIdx1;
    int idx2 = TABLE[TX].preIdx2;
    TX = idx1;
    curADR = TABLE[TX].t[idx2].ADR + 1;
    // {<过程说明部分>}
    while(LABEL[ip] == "PROCEDURE"){
        if(!ProcedureDeclare(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    return true;
}

// <过程首部> → PROCEDURE <标识符>;
bool Syntax_Analyzer::ProcedureHead(int f, int& ProcedureEntryIdx1, int& ProcedureEntryIdx2){
    // Insert current node to the syntax tree
    int fatherID = curIndex;
    string msg = "PROCEDUREHEAD";
    InsertNode(f, msg);

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    // PROCEDURE
    if(LABEL[ip] != "PROCEDURE"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }
    // <标识符>
    if(SYM[ip] != "IDENT"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        TABLE.push_back(tableList());
        int curTX = TABLE.size() - 1;
        tableElement e;
        e.NAME = LABEL[ip];
        e.KIND = "PROCEDURE";
        e.LEVEL = LEV - 1;
        e.ADR = CODE.size();
        if(!IsRedefined(e.NAME, TX)){
            TABLE[TX].t.push_back(e);
            ProcedureEntryIdx1 = TX;                        // 1D index of this procedure
            ProcedureEntryIdx2 = TABLE[TX].t.size() - 1;    // 2D index of this procedure
            TABLE[curTX].preIdx1 = TX;
            TABLE[curTX].preIdx2 = TABLE[TX].t.size() - 1;
            TX = curTX;
            curADR = DX;
        }
        else{
            string tmpmsg = "Error: re-define " + e.NAME + " !";
            PrintErrorPos(tmpmsg);
            return false;
        }
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }
    // ;
    if(LABEL[ip] != ";"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }
    return true;
}

// <语句> → <赋值语句>|<条件语句>|<当型循环语句>|<过程调用语句>|<读语句>|<写语句>|<复合语句>|<空语句>
bool Syntax_Analyzer::Sentence(int f){
    // Insert current node to the syntax tree
    int fatherID = curIndex;
    string msg = "SENTENCE";
    InsertNode(f, msg);

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    // <赋值语句>
    if(SYM[ip] == "IDENT"){
        if(!Assignment(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    // <条件语句>
    else if(LABEL[ip] == "IF"){
        if(!IfSentence(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    // <当型循环语句>
    else if(LABEL[ip] == "WHILE"){
        if(!WhileSentence(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    // <过程调用语句>
    else if(LABEL[ip] == "CALL"){
        if(!CallSentence(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    // <读语句>
    else if(LABEL[ip] == "READ"){
        if(!ReadSentence(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    // <写语句>
    else if(LABEL[ip] == "WRITE"){
        if(!WriteSentence(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    // <复合语句>
    else if(LABEL[ip] == "BEGIN"){
        if(!Combined(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }
    // <空语句>
    else{
        EmptySentence(fatherID);
        return true;
    }

    return true;
}

// <赋值语句> → <标识符>:=<表达式>
bool Syntax_Analyzer::Assignment(int f){
    // Insert current node to the syntax tree
    int fatherID = curIndex;
    string msg = "ASSIGNMENT";
    InsertNode(f, msg);

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    // <标识符>
    if(SYM[ip] != "IDENT"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }

    // :=
    if(LABEL[ip] != ":="){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }

    // search for variable position
    Address ad = LookUp(LABEL[ip - 2], false);
    if(ad.a == -1 && ad.l == -1){
        // Illegal access to variables
        string tmpmsg = "Error: Illegal access to variables !";
        PrintErrorPos(tmpmsg);
        return false;
    }
    if(ad.l == -1){
        // try modifying the value of the constant
        string tmpmsg = "Error: The value of the constant cannot be modified !";
        PrintErrorPos(tmpmsg);
        return false;
    }
    // <表达式>
    if(!Expression(fatherID)){
        PrintErrorPos(msg);
        return false;
    }
    // insert "STO" Instruction
    GEN("STO", ad.l, ad.a);
    return true;
}

// <复合语句> → BEGIN<语句>{ ;<语句>} END
bool Syntax_Analyzer::Combined(int f){
    // Insert current node to the syntax tree
    int fatherID = curIndex;
    string msg = "COMBINED";
    InsertNode(f, msg);

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    // BEGIN
    if(LABEL[ip] != "BEGIN"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }

    // <语句>
    if(!Sentence(fatherID)){
        PrintErrorPos(msg);
        return false;
    }

    // { ;<语句>}
    while(LABEL[ip] == ";"){
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
        if(!Sentence(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
    }

    // END
    if(LABEL[ip] != "END"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
        return true;
    }
}

// <条件> → <表达式><关系运算符><表达式>|ODD<表达式>
bool Syntax_Analyzer::Condition(int f){
    // Insert current node to the syntax tree
    int fatherID = curIndex;
    string msg = "CONDITION";
    InsertNode(f, msg);
    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }
    // ODD<表达式>
    if(LABEL[ip] == "ODD"){
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
        if(!Expression(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
        // Insert "OPR" instruction for "ODD"
        GEN("OPR", 0, 14);
    }
    // <表达式><关系运算符><表达式>
    else{
        if(!Expression(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
        string curOP = LABEL[ip];
        if(!RelationOp(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
        if(!Expression(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
        // Insert "OPR" instruction according to <关系运算符>
        if(curOP == "="){
            GEN("OPR", 0, 6);
        }
        else if(curOP == "#"){
            GEN("OPR", 0, 7);
        }
        else if(curOP == "<"){
            GEN("OPR", 0, 8);
        }
        else if(curOP == "<="){
            GEN("OPR", 0, 9);
        }
        else if(curOP == ">"){
            GEN("OPR", 0, 10);
        }
        else if(curOP == ">="){
            GEN("OPR", 0, 11);
        }
    }
    return true;
}

// <表达式> → [+|-]<项>{<加减运算符><项>}
bool Syntax_Analyzer::Expression(int f){
    // Insert current node to the syntax tree
    int fatherID = curIndex;
    string msg = "EXPRESSION";
    InsertNode(f, msg);

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    bool IsNegative = false;
    // [+|-]
    if(LABEL[ip] == "+" || LABEL[ip] == "-"){
        if(LABEL[ip] == "-"){
            IsNegative = true;
        }
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }
    // <项>
    if(!Item(fatherID)){
        PrintErrorPos(msg);
        return false;
    }
    if(IsNegative){
        // -
        GEN("OPR", 0, 15);
    }
    // {<加减运算符><项>}
    while(LABEL[ip] == "+" || LABEL[ip] == "-"){
        bool IsAdd = (LABEL[ip] == "+");
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
        if(!Item(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
        if(IsAdd){
            GEN("OPR", 0, 2);
        }
        else{
            GEN("OPR", 0, 3);
        }
    }
    return true;
}

// <项> → <因子>{<乘除运算符><因子>}
bool Syntax_Analyzer::Item(int f){
    // Insert current node to the syntax tree
    int fatherID = curIndex;
    string msg = "ITEM";
    InsertNode(f, msg);

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    // <因子>
    if(!Factor(fatherID)){
        PrintErrorPos(msg);
        return false;
    }
    // {<乘除运算符><因子>}
    while(LABEL[ip] == "*" || LABEL[ip] == "/"){
        bool IsMul = (LABEL[ip] == "*");
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
        if(!Factor(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
        if(IsMul){
            GEN("OPR", 0, 4);
        }
        else{
            GEN("OPR", 0, 5);
        }
    }
    return true;
}

// <因子> → <标识符>|<无符号整数>|(<表达式>)
bool Syntax_Analyzer::Factor(int f){
    // Insert current node to the syntax tree
    int fatherID = curIndex;
    string msg = "FACTOR";
    InsertNode(f, msg);

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    // (<表达式>)
    if(LABEL[ip] == "("){
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
        if(!Expression(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
        if(LABEL[ip] != ")"){
            PrintErrorPos(msg);
            return false;
        }
        else{
            InsertNode(fatherID, LABEL[ip]);
            ip ++;
            return true;
        }
    }

    // <标识符>
    if(SYM[ip] == "IDENT"){
        Address ad = LookUp(LABEL[ip], false);
        // not defined
        if(ad.l == -1 && ad.a == -1){
            string tmpmsg = "Error: this variable is not defined !";
            PrintErrorPos(tmpmsg);
            return false;
        }
        // CONSTANT
        if(ad.l == -1){
            GEN("LIT", 0, ad.a);
        }
        // VARIABLE
        else{
            GEN("LOD", ad.l, ad.a);
        }
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
        return true;
    }

    // <无符号整数>
    if(SYM[ip] == "NUM"){
        GEN("LIT", 0, atoi(LABEL[ip].c_str()));
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
        return true;
    }

    return false;
}

// <关系运算符> → =|#|<|<=|>|>=
bool Syntax_Analyzer::RelationOp(int f){
    string msg = LABEL[ip];
    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }
    if(LABEL[ip] == "=" || LABEL[ip] == "#" || LABEL[ip] == "<"
    || LABEL[ip] == "<=" || LABEL[ip] == ">" || LABEL[ip] == ">="){
        InsertNode(f, LABEL[ip]);
        ip ++;
        return true;
    }
    else{
        PrintErrorPos(msg);
        return false;
    }
}

// <条件语句> → IF<条件>THEN<语句>
bool Syntax_Analyzer::IfSentence(int f){
    // Insert current node to the syntax tree
    int fatherID = curIndex;
    string msg = "IFSENTENCE";
    InsertNode(f, msg);
    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }
    // IF
    if(LABEL[ip] == "IF"){
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
        // <条件>
        if(!Condition(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
        // insert "JPC" instruction the jump address remain to write......
        GEN("JPC", 0, 0);
        int codeIdx = CODE.size() - 1;
        // THEN
        if(LABEL[ip] != "THEN"){
            PrintErrorPos(msg);
            return false;
        }
        else{
            InsertNode(fatherID, LABEL[ip]);
            ip ++;
        }
        // <语句>
        if(!Sentence(fatherID)){
            PrintErrorPos(msg);
            return false;
        }
        // write the jump address of the "JPC" instruction
        CODE[codeIdx].a = CODE.size();
        return true;
    }
    else{
        PrintErrorPos(msg);
        return false;
    }
}

// <过程调用语句> → CALL<标识符>
bool Syntax_Analyzer::CallSentence(int f){
    // Insert current node to the syntax tree
    int fatherID = curIndex;
    string msg = "CALLSENTENCE";
    InsertNode(f, msg);

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    // CALL
    if(LABEL[ip] != "CALL"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }
    // <标识符>
    if(SYM[ip] != "IDENT"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        Address ad = LookUp(LABEL[ip], true);
        if(ad.l == -1 && ad.a == -1){
            string tmpmsg = "Error: the procedure that is called is not defined !";
            PrintErrorPos(tmpmsg);
            return false;
        }
        else{
            GEN("CAL", ad.l, ad.a);
        }
        ip ++;
    }
    return true;
}

// <当型循环语句> → WHILE<条件>DO<语句>
bool Syntax_Analyzer::WhileSentence(int f){
    // Insert current node to the syntax tree
    int fatherID = curIndex;
    string msg = "WHILESENTENCE";
    InsertNode(f, msg);
    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }
    // WHILE
    if(LABEL[ip] != "WHILE"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }
    int loopIdx = CODE.size();
    // <条件>
    if(!Condition(fatherID)){
        PrintErrorPos(msg);
        return false;
    }
    int jumpIdx = CODE.size();
    // insert "JPC" instruction to jump out the loop.
    GEN("JPC", 0, 0);
    // DO
    if(LABEL[ip] != "DO"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }
    // <语句>
    if(!Sentence(fatherID)){
        PrintErrorPos(msg);
        return false;
    }
    GEN("JMP", 0, loopIdx);
    CODE[jumpIdx].a = CODE.size();
    return true;
}

// <读语句> → READ(<标识符>{ ,<标识符>})
bool Syntax_Analyzer::ReadSentence(int f){
    // Insert current node to the syntax tree
    int fatherID = curIndex;
    string msg = "READSENTENCE";
    InsertNode(f, msg);

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    // READ
    if(LABEL[ip] != "READ"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }

    // (
    if(LABEL[ip] != "("){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }

    // <标识符>
    if(SYM[ip] != "IDENT"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        GEN("OPR", 0 ,12);
        Address ad = LookUp(LABEL[ip], false);
        if(ad.a == -1 && ad.l == -1){
            // Illegal access to variables
            string tmpmsg = "Error: Illegal access to variables !";
            PrintErrorPos(tmpmsg);
            return false;
        }
        if(ad.l == -1){
            // try modifying the value of the constant
            string tmpmsg = "Error: The value of the constant cannot be modified !";
            PrintErrorPos(tmpmsg);
            return false;
        }
        else{
            GEN("STO", ad.l, ad.a);
        }
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }

    // { ,<标识符>}
    while(LABEL[ip] == ","){
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
        if(SYM[ip] != "IDENT"){
            PrintErrorPos(msg);
            return false;
        }
        else{
            GEN("OPR", 0 ,12);
            Address ad = LookUp(LABEL[ip], false);
            if(ad.a == -1 && ad.l == -1){
                // Illegal access to variables
                string tmpmsg = "Error: Illegal access to variables !";
                PrintErrorPos(tmpmsg);
                return false;
            }
            if(ad.l == -1){
                // try modifying the value of the constant
                string tmpmsg = "Error: The value of the constant cannot be modified !";
                PrintErrorPos(tmpmsg);
                return false;
            }
            else{
                GEN("STO", ad.l, ad.a);
            }
            InsertNode(fatherID, LABEL[ip]);
            ip ++;
        }
    }

    // )
    if(LABEL[ip] != ")"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }

    return true;
}

// <写语句> → WRITE(<标识符>{,<标识符>})
bool Syntax_Analyzer::WriteSentence(int f){
    // Insert current node to the syntax tree
    int fatherID = curIndex;
    string msg = "WRITESENTENCE";
    InsertNode(f, msg);

    if(ip >= LABEL.size()){
        PrintErrorPos(msg);
        return false;
    }

    // WRITE
    if(LABEL[ip] != "WRITE"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }
    // (
    if(LABEL[ip] != "("){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }
    // <标识符>
    if(SYM[ip] != "IDENT"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        Address ad = LookUp(LABEL[ip], false);
        if(ad.a == -1 && ad.l == -1){
            // Illegal access to variables
            string tmpmsg = "Error: Illegal access to variables !";
            PrintErrorPos(tmpmsg);
            return false;
        }
        if(ad.l == -1){
            GEN("LIT", 0, ad.a);
        }
        else{
            GEN("LOD", ad.l, ad.a);
        }
        GEN("OPR", 0, 13);
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }
    // {,<标识符>}
    while(LABEL[ip] == ","){
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
        if(SYM[ip] != "IDENT"){
            PrintErrorPos(msg);
            return false;
        }
        else{
            Address ad = LookUp(LABEL[ip], false);
            if(ad.a == -1 && ad.l == -1){
                // Illegal access to variables
                string tmpmsg = "Error: Illegal access to variables !";
                PrintErrorPos(tmpmsg);
                return false;
            }
            if(ad.l == -1){
                GEN("LIT", 0, ad.a);
            }
            else{
                GEN("LOD", ad.l, ad.a);
            }
            GEN("OPR", 0, 13);
            InsertNode(fatherID, LABEL[ip]);
            ip ++;
        }
    }
    // )
    if(LABEL[ip] != ")"){
        PrintErrorPos(msg);
        return false;
    }
    else{
        InsertNode(fatherID, LABEL[ip]);
        ip ++;
    }

    return true;
}

// <空语句> → epsilon
bool Syntax_Analyzer::EmptySentence(int f){
    string msg2 = "EMPTY";
    InsertNode(f, msg2);
    return true;
}

//***********************************************************//
//                                                           //
//       Functions for target instruction generation         //
//                                                           //
//***********************************************************//

bool Syntax_Analyzer::IsRedefined(string name, int idx){
    for(int i = 0; i < TABLE[idx].t.size(); i ++){
        if(TABLE[idx].t[i].NAME == name){
            return true;
        }
    }
    return false;
}

void Syntax_Analyzer::GEN(string f, int l, int a){
    CODE.push_back(Instruction(f, l, a));
}

Address Syntax_Analyzer::LookUp(string name, bool flag){
    // flag = 0 ---> VARIABLE  ---> return (l, data_address)
    // flag = 0 ---> CONSTANT  ---> return (-1, val)
    // flag = 1 ---> PROCEDURE ---> return (l, code_address)
    // NO FIND  ---> return (-1, -1)
    Address tmp;
    int idx = TX, lev = 0;
    while(idx != -1){
        // PROCEDURE
        if(flag){
            for(int i = 0; i < TABLE[idx].t.size(); i ++){
                if(TABLE[idx].t[i].NAME == name && TABLE[idx].t[i].KIND == "PROCEDURE"){
                    tmp.l = lev;
                    tmp.a = TABLE[idx].t[i].ADR;
                    return tmp;
                }
            }
        }
        // VARIABLE or CONSTANT
        else{
            for(int i = 0; i < TABLE[idx].t.size(); i ++){
                if(TABLE[idx].t[i].NAME == name && TABLE[idx].t[i].KIND != "PROCEDURE"){
                    if(TABLE[idx].t[i].KIND == "VARIABLE"){
                        tmp.l = lev;
                        tmp.a = TABLE[idx].t[i].ADR;
                    }
                    else{
                        tmp.l = -1;
                        tmp.a = TABLE[idx].t[i].VAL;
                    }
                    return tmp;
                }
            }
        }
        idx = TABLE[idx].preIdx1;
        lev ++;
    }
    tmp.l = tmp.a = -1;
    return tmp;
}

//***********************************************************//
//                                                           //
//             Functions for output or debug                 //
//                                                           //
//***********************************************************//

// Output the error information
void Syntax_Analyzer::PrintErrorPos(string msg){
    if(ip >= LABEL.size())
        cerr<< "Error: Out Of Range in " << msg << "!" <<endl;
    else
        cerr<< "Error: Syntax Error at position "<< ip <<" "<<LABEL[ip]<<" in "<< msg << "!"<<endl;
}

// Output the syntax tree using dfs
// int s : the source node's id
void Syntax_Analyzer::TreeOutput(int s){
    // transfer the node name for output
    if(nodeInfo[s] == "("){
        exp += "LP";
    }
    else if(nodeInfo[s] == ")"){
        exp += "RP";
    }
    else if(nodeInfo[s] == ","){
        exp += "COMMA";
    }
    else{
        exp += nodeInfo[s];
    }
    // no child node, then return
    if(tree[s].size() == 0){
        return;
    }
    // continue dfs......
    else{
        exp += "(";
        for(int i = 0; i < tree[s].size(); i ++){
            TreeOutput(tree[s][i]);
            if(i != tree[s].size() -1)
                exp += ",";
        }
        exp += ")";
        return;
    }
}

// Depth first search
void Syntax_Analyzer::dfs(int &index, int depth){
    again:
    ans += string(depth, '\t');
    ans += tokens[index];
    index++;
    if (index < tokens.size() && tokens[index] == "(")
    {
        ans += "(\n";
        index++;
        dfs(index, depth + 1);
        if (tokens[index] != ")")throw 233;
        ans += string(depth,'\t') +")";
        index++;
    }
    if (index < tokens.size() && tokens[index] == ",")
    {
        ans += ",\n";
        index++;
        goto again;
    }
    ans += "\n";
}

// Transfer the output for debugging
// bool debug
//  = true  : output for debugging
//  = false : output for oj
void Syntax_Analyzer::TransferOutput(bool debug){
    TreeOutput(0);
    if(!debug){
        cout << exp << endl;
    }
    else{
        string tmp;
        for (int i = 0; i < exp.length(); i++)
        {
            if (exp[i] == '(' || exp[i] == ')' || exp[i] == ',')
            {
                if(!tmp.empty())tokens.push_back(tmp);
                tmp = "";
                tokens.push_back(string(1, exp[i]));
            }
            else tmp += exp[i];
        }
        if (!tmp.empty())tokens.push_back(tmp);
        try
        {
            int id = 0;
            dfs(id, 0);
        }
        catch (...)
        {
            cerr << "Error Parsing Expression" << endl;
            return;
        }
        cout << ans << endl;
    }
}

void Syntax_Analyzer::TableOutput() {
    for(int i = 0; i < TABLE.size(); i ++){
        for(int j = 0; j < TABLE[i].t.size(); j ++){
            cout<< TABLE[i].t[j].NAME <<"\t"<<TABLE[i].t[j].KIND<<"\t";
            if(TABLE[i].t[j].KIND == "CONSTANT"){
                cout<< TABLE[i].t[j].VAL << endl;
            }
            else{
                cout<< TABLE[i].t[j].LEVEL <<"\t"<<TABLE[i].t[j].ADR<<endl;
            }

        }
    }
}

void Syntax_Analyzer::CodeOutput(){
    for(int i = 0 ; i < CODE.size(); i ++){
        cout << CODE[i].f << " " << CODE[i].l << " " << CODE[i].a << endl;
    }
}