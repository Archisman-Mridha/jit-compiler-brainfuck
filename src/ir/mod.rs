use {
  crate::{
    error::Error,
    lexer::token::{Token, TokenKind}
  },
  std::iter::Peekable
};

#[derive(Debug)]
pub struct IR {
  pub operator: TokenKind,

  /**
    Depending on the token, represents one of the following :

      (1) how many times we're going to MOVE_LEFT / MOVE_RIGHT / INCREMENT / DECREMENT.

      (2) how many bytes we're going to OUTPUT / take as INPUT.

      (3) the address, where we want to jump if the current token is JUMP_IF_ZERO /
          JUMP_IF_NON_ZERO.
  */
  pub operand: usize
}

pub fn generate(mut tokens: Peekable<impl Iterator<Item = Token>>) -> Result<Vec<IR>, Error> {
  let mut irs = Vec::new();

  /*
    address(IR) = index of that IR in the irs vector.

    Now, when we encounter the JUMP_WHEN_ZERO token, we create an IR (a) out of it.
    And push the IR address to the jump_ir_addresses.

    Next, when we encounter a JUMP_WHEN_NON_ZERO token, we again create an IR (b) out of it.
    But, we also pop out the corresponding (topmost) JUMP_WHEN_ZERO IR (a) out of the
    jump_ir_addresses. And do :

                                a.operand = address(b) + 1
                                b.operand = address(a) + 1

    This technique is called backpatching, since, while processing b, we're going back and patching
    a with the correct information.
  */
  let mut jump_ir_addresses = Vec::<usize>::new();

  while let Some(token) = tokens.next() {
    match token.kind {
      TokenKind::MoveLeft
      | TokenKind::MoveRight
      | TokenKind::Increment
      | TokenKind::Decrement
      | TokenKind::Input
      | TokenKind::Output => {
        let mut ir = IR {
          operator: token.kind,
          operand:  1
        };

        // Check if there are similar kind of tokens following this token.
        // We'll compress all the consecutive similar kinded tokens into a single efficient IR.
        while tokens
          .peek()
          .take_if(|token| token.kind == ir.operator)
          .is_some()
        {
          ir.operand += 1;
        }

        irs.push(ir);
      }

      TokenKind::JumpWhenZero => {
        let ir = IR {
          operator: token.kind,
          operand:  0
        };
        let ir_address = irs.len();

        irs.push(ir);
        jump_ir_addresses.push(ir_address);
      }

      TokenKind::JumpWhenNonZero => {
        let jump_when_zero_ir_address = match jump_ir_addresses.pop() {
          None => return Err(Error::MissingCorrespondingJumpWhenZeroToken),
          Some(jump_when_zero_ir_address) => jump_when_zero_ir_address
        };

        let jump_when_non_zero_ir = IR {
          operator: token.kind,
          operand:  jump_when_zero_ir_address + 1
        };
        let jump_when_non_zero_ir_address = irs.len();

        irs.push(jump_when_non_zero_ir);

        // Backpatch the corresponding JUMP_WHEN_ZERO IR.
        irs[jump_when_zero_ir_address].operand = jump_when_non_zero_ir_address + 1;
      }

      TokenKind::Unknown => unreachable!()
    }
  }

  Ok(irs)
}
