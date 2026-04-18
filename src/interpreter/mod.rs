use crate::{error::Error, ir::IR, lexer::token::TokenKind};

struct Memory {
  buffer: Vec<usize>
}

pub struct Interpreter {
  irs:        Vec<IR>,
  ir_pointer: usize,

  memory:       Memory,
  data_pointer: usize
}

impl Interpreter {
  pub fn new(irs: Vec<IR>) -> Self {
    let mut memory = Memory { buffer: Vec::new() };

    memory.buffer.push(0);

    Self {
      irs,
      ir_pointer: 0,

      memory,
      data_pointer: 0
    }
  }

  pub fn interpret(&mut self) -> Result<(), Error> {
    while self.ir_pointer < self.irs.len() {
      let ir = &self.irs[self.ir_pointer];

      match ir.operator {
        TokenKind::Increment => {
          self.memory.buffer[self.data_pointer] += ir.operand;

          self.ir_pointer += 1;
        }

        TokenKind::Decrement => {
          self.memory.buffer[self.data_pointer] -= ir.operand;

          self.ir_pointer += 1;
        }

        TokenKind::MoveLeft => {
          if ir.operand > self.data_pointer {
            return Err(Error::MemoryUnderflow);
          }

          self.data_pointer -= ir.operand;

          self.ir_pointer += 1;
        }

        TokenKind::MoveRight => {
          self.data_pointer += ir.operand;

          // Extend the memory, if required.
          let required_min_memory_capacity = self.data_pointer + 1;
          if self.memory.buffer.len() < required_min_memory_capacity {
            self.memory.buffer.resize(required_min_memory_capacity, 0);
          }

          self.ir_pointer += 1;
        }

        TokenKind::Input => unimplemented!(),

        TokenKind::Output => {
          for _ in 0..ir.operand {
            print!("{}", self.memory.buffer[self.data_pointer] as u8 as char);
          }

          self.ir_pointer += 1;
        }

        TokenKind::JumpWhenZero =>
          if self.memory.buffer[self.data_pointer] == 0 {
            self.ir_pointer = ir.operand;
          } else {
            self.ir_pointer += 1;
          },

        TokenKind::JumpWhenNonZero =>
          if self.memory.buffer[self.data_pointer] != 0 {
            self.ir_pointer = ir.operand;
          } else {
            self.ir_pointer += 1;
          },

        TokenKind::Unknown => unreachable!()
      }
    }

    Ok(())
  }
}
