use crate::{compiler::assembler::Assembler, ir::IR, lexer::token::TokenKind};

pub mod assembler;

pub struct Compiler<A>
where
  A: Assembler
{
  irs:        Vec<IR>,
  ir_pointer: usize,

  assembler: A,

  code: Vec<usize>
}

impl<A> Compiler<A>
where
  A: Assembler
{
  pub fn new(irs: Vec<IR>, assembler: A) -> Self {
    Self {
      irs,
      ir_pointer: 0,

      assembler,

      code: Vec::new()
    }
  }

  pub fn compile(&mut self) {
    while self.ir_pointer < self.irs.len() {
      let ir = &self.irs[self.ir_pointer];

      match ir.operator {
        TokenKind::Increment => todo!(),
        TokenKind::Decrement => todo!(),

        TokenKind::MoveLeft => todo!(),
        TokenKind::MoveRight => todo!(),

        TokenKind::Input => todo!(),
        TokenKind::Output => todo!(),

        TokenKind::JumpWhenZero => todo!(),
        TokenKind::JumpWhenNonZero => todo!(),

        _ => unreachable!()
      }
    }
  }
}
