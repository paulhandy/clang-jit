#include <cstdio>
#include <stdlib.h>
#include <alloca.h>
#include "jit.h"
void setupPredefinedFunctions(ClangJitCompiler& compiler);
void setupGMPFunctions();

extern "C" int error_handler(int level, const char *filename, int line, int column, const char *message)
{
    static int count = 0;
    static char *levelString[] = {"Ignore", "Note", "Remark", "Warning", "Error", "Fatal"};
    printf("%s(%d): %s\n", levelString[level], ++count, message);
    printf("    File: %s\n", filename);
    printf("    Line: %d\n\n", line);
    return 1;
}

int main(int ac, char *const *av)
{
    ClangJitCompiler compiler;
    try {
        // Use ClangJitSourceType_C_String if you want to compile from string data.
        int nSource = atoi(av[1]);
        const char **sources = (const char **) alloca(sizeof(char *)*nSource);
        int *types = (int *) alloca(sizeof(int)*nSource);

        for (int i = 0; i < nSource && 2*i+3 < ac; i++) {
          sources[i] = av[2*i+2];
          switch(av[2*i+3][0]) {
            case 'c':
              types[i] = ClangJitSourceType_C_File; break;
            case 'x':
              types[i] = ClangJitSourceType_CXX_File; break;
            case 'X':
              types[i] = ClangJitSourceType_CXX_String; break;
            case 'C':
              types[i] = ClangJitSourceType_C_String; break;
            default:
              types[i] = 0;
          }
          //printf("type - name %s:%s\n", av[2*i+3], av[2*i+2]);
        }
        compiler.setOptimizeLevel(3);
        setupPredefinedFunctions(compiler);
        setupGMPFunctions();
        compiler.compile(sources, types, nSource, error_handler);
        compiler.finalize();

        //printf("arg 1 %s\n", av[2 + 2*nSource]);
        typedef int (*mainf_t)(int, char * const*);
        mainf_t ff = compiler.getFunctionAddress<mainf_t>("main");
        if (ff) {
            return ff(ac-2 - 2*nSource, &av[2 + 2*nSource]);
        }

        printf("The function main was not found.\n");
    } catch (std::exception& e) {
        printf("Error: %s\n", e.what());
    } catch (...) {
        printf("Unknown Error.\n");
    }

    return 1;
}
