use crate::lexer::token::{Token, TokenKind};

pub mod token;

pub struct Lexer<Cs>
where
  Cs: Iterator<Item = char>
{
  characters: Cs
}

impl<Cs> Lexer<Cs>
where
  Cs: Iterator<Item = char>
{
  pub fn new(characters: impl IntoIterator<Item = char, IntoIter = Cs>) -> Self {
    Self {
      characters: characters.into_iter()
    }
  }
}

impl<Cs> Iterator for Lexer<Cs>
where
  Cs: Iterator<Item = char>
{
  type Item = Token;

  fn next(&mut self) -> Option<Self::Item> {
    match self.characters.next() {
      None => None,

      Some(character) => match TokenKind::from(character) {
        // Ignore comment.
        TokenKind::Unknown => self.next(),

        token_kind => Some(Token { kind: token_kind })
      }
    }
  }
}
