#include "clang-tidy/ClangTidyModule.h"
#include "clang-tidy/ClangTidyModuleRegistry.h"
#include "AvoidCurrentTimeCheck.h"

namespace clang::tidy::snps
{
class SnpsModule : public ClangTidyModule
{
public:
    void addCheckFactories(ClangTidyCheckFactories& CheckFactories) override
    {
        CheckFactories.registerCheck<clang::tidy::snps::AvoidCurrentTimeCheck>("snps-avoid-current-time");
        volatile int snpsCheckAnchorSource = 0;
    }
};
} // namespace clang::tidy::snps

namespace clang::tidy {

// Register the module using this statically initialized variable.
static ClangTidyModuleRegistry::Add<clang::tidy::snps::SnpsModule> SnpsCheckInit("snps-module",
                                                                                       "Adds Synopsys custom checks.");

// This anchor is used to force the linker to link in the generated object file and thus register the module.
volatile int snpsCheckAnchorSource = 0;

}  // namespace clang::tidy