#include "Code_Interpreter.h"
#include "utils.h"

Code_Interpreter::Code_Interpreter() {
    IP = 0;
    BP = 0;
    SP = -1;
}

Code_Interpreter::Code_Interpreter(vector<Instruction>& C) {
    IP = 0;
    BP = 0;
    SP = -1;
    CODE = C;
}

Code_Interpreter::~Code_Interpreter() {
    stack.clear();
}

void Code_Interpreter::Push(int element) {
    SP ++;
    while (SP >= stack.size()){
        stack.push_back(0);
    }
    stack[SP] = element;
}

int Code_Interpreter::Pop(){
    int element = stack[SP];
    SP --;
    return element;
}

void Code_Interpreter::AdvancePC() {
    IP ++;
}

void Code_Interpreter::OneInstruction() {
    // IR : current instruction
    IR.f = CODE[IP].f;
    IR.l = CODE[IP].l;
    IR.a = CODE[IP].a;
    if(IR.f == "LIT"){
        Push(IR.a);
    }
    else if(IR.f == "LOD"){
        int pt = BP;
        while(IR.l --){
            pt = stack[pt];
        }
        Push(stack[pt + IR.a]);
    }
    else if(IR.f == "STO"){
        int pt = BP;
        while(IR.l --){
            pt = stack[pt];
        }
        int element = Pop();
        stack[pt + IR.a] = element;
    }
    else if(IR.f == "CAL"){
        int pt = BP;
        while(IR.l --){
            pt = stack[pt];
        }
        Push(pt);
        Push(BP);
        Push(IP);
        SP -= 3;
        BP = SP + 1;
        IP = IR.a - 1;
    }
    else if(IR.f == "INT"){
        SP += IR.a;
    }
    else if(IR.f == "JMP"){
        IP = IR.a - 1;
    }
    else if(IR.f == "JPC"){
        if(Pop() == 0){
            IP = IR.a - 1;
        }
    }
    else if(IR.f == "OPR"){
        // return
        if(IR.a == 0){
            SP = BP - 1;
            IP = stack[SP + 3];
            //cout<<"RETURN"<<endl;
            //cout << IP <<endl;
            //cout << CODE[IP+1].f<<" "<< CODE[IP+1].l << " " << CODE[IP+1].a<<endl;
            BP = stack[SP + 2];
            if(IP == 0){
                IP = -1;
            }
        }
        // :=
        else if(IR.a == 1){
            // no use
        }
        // +
        else if(IR.a == 2){
            int arg1 = Pop();
            int arg2 = Pop();
            int res = arg1 + arg2;
            Push(res);
        }
        // -
        else if(IR.a == 3){
            int arg1 = Pop();
            int arg2 = Pop();
            int res = arg2 - arg1;
            Push(res);
        }
        // *
        else if(IR.a == 4){
            int arg1 = Pop();
            int arg2 = Pop();
            int res = arg2 * arg1;
            Push(res);
        }
        // /
        else if(IR.a == 5){
            int arg1 = Pop();
            int arg2 = Pop();
            int res = arg2 / arg1;
            Push(res);
        }
        // =
        else if(IR.a == 6){
            int arg1 = Pop();
            int arg2 = Pop();
            int res = (arg2 == arg1);
            Push(res);
        }
        // #
        else if(IR.a == 7){
            int arg1 = Pop();
            int arg2 = Pop();
            int res = (arg2 != arg1);
            Push(res);
        }
        // <
        else if(IR.a == 8){
            int arg1 = Pop();
            int arg2 = Pop();
            int res = (arg2 < arg1);
            Push(res);
        }
        // <=
        else if(IR.a == 9){
            int arg1 = Pop();
            int arg2 = Pop();
            int res = (arg2 <= arg1);
            Push(res);
        }
        // >
        else if(IR.a == 10){
            int arg1 = Pop();
            int arg2 = Pop();
            int res = (arg2 > arg1);
            Push(res);
        }
        // >=
        else if(IR.a == 11){
            int arg1 = Pop();
            int arg2 = Pop();
            int res = (arg2 >= arg1);
            Push(res);
        }
        // read
        else if(IR.a == 12){
            int element;
            cin >> element;
            Push(element);
        }//
        // write
        else if(IR.a == 13){
            int element = Pop();
            cout << element << endl;
        }
        // odd
        else if(IR.a == 14){
            int element = Pop();
            element = element % 2;
            Push(element);
        }
        // negative
        else if(IR.a == 15){
            int element = Pop();
            element = - element;
            Push(element);
        }
    }
}

void Code_Interpreter::Run() {
    // init for MAIN
    Push(0);    // SL
    Push(0);    // DL
    Push(0);    // RA
    SP -= 3;
    do{
        OneInstruction();
        AdvancePC();
    }while(IP != 0);
}
