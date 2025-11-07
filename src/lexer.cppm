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

export class Lexer {
  public:
    Lexer(const string& source) : source(source) { }

    optional<Token> next( ) {
      auto next_token = this->peek(0);
      this->current_position++;
      return next_token;
    }

    optional<Token> next_if_token(Token desired_next_token) {
      auto next_token = this->peek(0);

      if (next_token != desired_next_token)
        return nullopt;

      this->current_position++;
      return next_token;
    }

  private:
    const string& source;
    int           current_position = 0;

    optional<Token> peek(int offset) {
      int target_position = this->current_position + offset;

      if (this->current_position >= this->source.size( ))
        return nullopt;

      char lexeme = this->source[target_position];
      while (true) {
        try {
          auto current_token = static_cast<Token>(lexeme);
          return current_token;
        } catch (const exception& e) {
          // Ignore comments
        }
      }
    }
};
