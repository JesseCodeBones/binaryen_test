#include "binaryen-c.h"
#include "cfg/cfg-traversal.h"
#include "ir/module-utils.h"
#include "pass.h"
#include "wasm-io.h"
#include "wasm.h"
#include <iostream>
#include <vector>

struct Info {
  std::vector<BinaryenExpressionRef> exprs;
  std::vector<wasm::Function::DebugLocation> debugLocations;
  void dump(wasm::Function *currFun) {
    if (currFun) {
      std::cout << "dump for function:" << currFun->name << std::endl;
    }
    for (auto expr : exprs) {
      expr->dump();
    }
  }
};

struct TestWalker
    : public wasm::WalkerPass<wasm::CFGWalker<
          TestWalker, wasm::UnifiedExpressionVisitor<TestWalker>, Info>> {
  void visitExpression(BinaryenExpressionRef curr) {
    if (!currBasicBlock) {
      return;
    }
    currBasicBlock->contents.exprs.push_back(curr);
    auto currFun = getFunction();
    if (currFun->debugLocations.find(curr) != currFun->debugLocations.end()) {
      currBasicBlock->contents.debugLocations.push_back(
          currFun->debugLocations[curr]);
    }
  }

  void doWalkFunction(BinaryenFunctionRef func) {
    using Parent = wasm::WalkerPass<wasm::CFGWalker<
        TestWalker, wasm::UnifiedExpressionVisitor<TestWalker>, Info>>;
    if (func->imported()) { // do not handle imported function
      return;
    }
    Parent::doWalkFunction(func);
    for (auto &basicBlock : basicBlocks) {
      std::cout << "basic block ->" << basicBlock.get() << std::endl;
      for (auto &in : basicBlock->in) {
        std::cout << "block in-" << in << std::endl;
      }
      for (auto &out : basicBlock->out) {
        std::cout << "block out-" << out << std::endl;
      }
      std::cout << " block content ---\n";
      for (auto &debugLocation : basicBlock->contents.debugLocations) {
        std::cout << debugLocation.fileIndex << "-" << debugLocation.lineNumber
                  << "-" << debugLocation.columnNumber << std::endl;
      }
      std::cout << " --- block content \n";
      std::cout << "<- basic block" << std::endl;
    }
    // dumpCFG("jesse dump");
  }
};

int main(int, char **) {
  wasm::Module module;
  wasm::ModuleReader reader;
  reader.read("/home/jesse/workspace/binaryen_test/build/out.wasm", module,
              "/home/jesse/workspace/binaryen_test/build/out.wasm.map");
  TestWalker walker;
  wasm::ModuleUtils::iterDefinedFunctions(
      module, [&](BinaryenFunctionRef func) {
        walker.walkFunctionInModule(func, &module);
      });
  // walker.walkModule(&module);
  // walker.dumpCFG("dump jesse");
  // BinaryenModulePrint(&module);
}
