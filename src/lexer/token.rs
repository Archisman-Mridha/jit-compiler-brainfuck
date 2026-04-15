#[derive(Debug)]
pub struct Token {
  pub kind: TokenKind
}

#[derive(Debug, PartialEq, Eq)]
pub enum TokenKind {
  // Increment the data pointer by one (to point to the next cell to the right).
  MoveLeft,

  // Decrement the data pointer by one (to point to the next cell to the left).
  MoveRight,

  // Increment the byte at the data pointer by one.
  Increment,

  // Decrement the byte at the data pointer by one.
  Decrement,

  // Accept one byte of input, storing its value in the byte at the data pointer.
  Input,

  // Output the byte at the data pointer.
  Output,

  // When the byte at the data pointer is zero, then instead of moving the instruction pointer
  // forward to the next command, jump it forward to the command after the matching ']' command.
  JumpWhenZero,

  // When the byte at the data pointer is nonzero, then instead of moving the instruction pointer
  // forward to the next command, jump it back to the command after the matching '[' command.
  JumpWhenNonZero,

  Unknown
}

impl From<char> for TokenKind {
  fn from(character: char) -> Self {
    match character {
      '>' => Self::MoveLeft,
      '<' => Self::MoveRight,

      '+' => Self::Increment,
      '-' => Self::Decrement,

      '.' => Self::Input,
      ',' => Self::Output,

      '[' => Self::JumpWhenZero,
      ']' => Self::JumpWhenNonZero,

      _ => Self::Unknown
    }
  }
}
