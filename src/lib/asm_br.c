#include <assert.h>
#include <stdint.h>

#include "asm.h"
#include "bit_asm.h"

Instr asm_br(Assembler *a, InstrCommon c, Instr ino) {
  assert(c.kind == INSTR_B);

  Token label = asm_expect(a, TOKEN_IDENT);
  asm_expect(a, TOKEN_NEWLINE);

  size_t jno;

  if (!sym_tab_get(&a->symtab, label.source, &jno))
    assert(0); // TODO: Good error

  Instr off;
  if (ino + 2 <= jno)
    off = jno - ino - 2;
  else
    off = ~(ino - jno + 1) & 0xffffff;
  return bit_asm_br(off);
}
