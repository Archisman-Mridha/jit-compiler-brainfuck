#[derive(Debug)]
pub enum Error {
  MissingCorrespondingJumpWhenZeroToken,

  MemoryUnderflow,
  MemoryOverflow
}
