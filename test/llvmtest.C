#include <llvm/Function.h>
#include <llvm/Module.h>
#include <stdio.h>
#include <iostream>
using namespace std;
using namespace llvm;
int main() {
	Module* m;
	Function* func = cast<Function>(m->getOrInsertFunction("main",
        Type::VoidTy, (Type*)0));
	BasicBlock *bb = BasicBlock::Create("entry", func);
}
