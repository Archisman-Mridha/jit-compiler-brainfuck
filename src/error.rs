use std::io;

#[derive(Debug)]
pub enum Error {
  IO(io::Error),

  MissingCorrespondingJumpWhenZeroToken,

  MemoryUnderflow,
  MemoryOverflow
}
