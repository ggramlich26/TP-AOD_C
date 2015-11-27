#ifndef __PATCHTABLE_H_
#define __PATCHTABLE_H_

/**
* This enum defines the possible operations in a patch
*/
typedef enum{ADD, SUBST, DEL, COPY} operation;

#define ADDOP 0xF0000000
#define SUBSTOP 0xE0000000
#define COPYOP 0xD0000000
#define DELOP 0xC0000000
#define MULTDELOP 0xB0000000
#define LINEMASK 0x0FFFFFFF

void initPatchTable();
void setOp(int inLine, int outLine, operation op);
void setMultDelOp(int inLine, int outLine, int nLines);
operation getOp(int inLine, int outLine);
void printPatchTable(int inLine, int outLine);




#endif
