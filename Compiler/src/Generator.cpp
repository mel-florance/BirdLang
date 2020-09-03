#include "pch.h"
#include "Generator.h"

#include <llvm/ADT/APFloat.h>
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

Generator::Generator()
{
	llvm::LLVMContext TheContext;
	llvm::IRBuilder<> Builder(TheContext);
	std::unique_ptr<llvm::Module> TheModule;
	std::map<std::string, llvm::Value*> NamedValues;
}
