#ifdef INTERP
#ifdef EXT
#include "neillsdl2.h"
#define ROOMSIZE 800
#define MILLISECONDDELAY 2
#define LINESIZE 48
#endif
#endif
#define MAXWORDLEN 100
#define MAXWORDS 50
#define FLOAT_INIT_VAL 10000000000

enum ErrorCodes {StartERROR = 101, SyntaxERROR = 102, OverflowERROR = 103,
                 FunctionERROR = 104, VariableERROR = 105, FileERROR = 106,
                 CompERROR = 107, EndERROR = 108, Abort = 200};
enum CompStates {Greater = 1, Equal = 0, Less = -1, Invalid = -100};
enum check {FAIL, PASS};
typedef enum {FALSE, TRUE} bool;

#ifdef INTERP
#ifdef EXT
typedef struct SDLVars {
  int xCoord;
  int yCoord;
  SDL_Simplewin *sw;
  bool interact;
} SDLVars;
#endif
#endif

typedef struct prog {
  char **words;
  int currWord;
  int totalWords;
  mvm *vars;
  int errorState;
  #ifdef INTERP
  #ifdef EXT
  SDLVars *SDL;
  #endif
  #endif
} Program;



/* TESTED */ FILE* readFile(int argc, char** argv, int *maxWords);
/* TESTED */ void initProgram(Program *p, int maxWords);
/* TESTED */ void fillWords(Program *p, FILE *fp);

void prog(Program *p);
void instrs(Program *p);
void instruct(Program *p);
/* All possible instructions */
/* TESTED */ void file(Program *p, bool *checked);
/* TESTED */ void abortEX(Program *p, bool *checked);
/* TESTED */ void input(Program *p, bool *checked);
/* TESTED */ void ifcond(Program *p, bool *checked);
/* TESTED */ void inc(Program *p, bool *checked);
/* TESTED */ void set(Program *p, bool *checked);
/* TESTED */ void jump(Program *p, bool *checked);
void print(Program *p, bool *checked);
/* TESTED */ void rnd(Program *p, bool *checked);
/* All variables/constants */
/* TESTED */ void varcon(Program *p);
/* TESTED */ bool var(Program *p);
/* TESTED */ bool con(Program *p);
/* TESTED */ bool strvar(Program *p);
/* TESTED */ bool numvar(Program *p);
/* TESTED */ bool strcon(Program *p);
/* TESTED */ bool numcon(Program *p);

void openNewFile(Program *p);
/* TESTED */ int checkCond(Program *p);
/* TESTED */ int compCons(char *strcon1, float numcon1, char *strcon2, float numcon2);
/* TESTED */ void skipFunction(Program *p);

void printVarCon(Program *p);
/* TESTED */ char* formatStrcon(char *currentWord);
/* TESTED */ char *returnVars(Program *p);

/* TESTED */ void ROTDecode(char *ROTString);
/* TESTED */ void nextWord(Program *p);
/* TESTED */ bool isPosInteger(char *number);
void printState(Program *p);
void freeProgram(Program *p, int maxWords);

void test(void);
void basicFuncTest(void);
void testVarCon(void);
void parseTestFunc(void);
void interpTestFunc(void);

#ifdef INTERP
#ifdef EXT
void runSDL(Program *p);
void SDL_DrawRoom(SDL_Simplewin sw, Program *p);
void keyboard(SDL_Simplewin *sw, Program *p);
void printOutput(SDL_Simplewin sw, char *text);
void waitforInput(Program *p);
#endif
#endif
