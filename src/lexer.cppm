module;

#include <optional>
#include <string>

export module lexer;

using namespace std;

export enum class Token : char {
  // Increment the data pointer by one (to point to the next cell to the right).
  MOVE_LEFT = '>',

  // Decrement the data pointer by one (to point to the next cell to the left).
  MOVE_RIGHT = '<',

  // Increment the byte at the data pointer by one.
  INCREMENT = '+',

  // Decrement the byte at the data pointer by one.
  DECREMENT = '-',

  // Accept one byte of input, storing its value in the byte at the data pointer.
  INPUT = '.',

  // Output the byte at the data pointer.
  OUTPUT = ',',

  // If the byte at the data pointer is zero, then instead of moving the instruction pointer
  // forward to the next command, jump it forward to the command after the matching ']' command.
  JUMP_IF_ZERO = '[',

  // If the byte at the data pointer is nonzero, then instead of moving the instruction pointer
  // forward to the next command, jump it back to the command after the matching '[' command.
  JUMP_IF_NON_ZERO = ']'
};

export struct Lexer {
    const string &source;
    int           currentPosition = 0;

    Lexer(const string &source) : source(source) { }

    optional<Token> peek(int offset) {
      int targetPosition = this->currentPosition + offset;

      if (this->currentPosition >= this->source.size( ))
        return nullopt;

      char lexeme = this->source[targetPosition];
      while (true) {
        try {
          auto currentToken = static_cast<Token>(lexeme);
          return currentToken;
        } catch (const exception &e) {
          // Ignore comments
        }
      }
    }

    optional<Token> next( ) {
      auto nextToken = this->peek(0);
      this->currentPosition++;
      return nextToken;
    }

    optional<Token> next_if_token(Token desiredNextToken) {
      auto nextToken = this->peek(0);

      if (nextToken != desiredNextToken)
        return nullopt;

      this->currentPosition++;
      return nextToken;
    }
};
