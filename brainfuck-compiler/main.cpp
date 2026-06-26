#include <cstddef>
#include <iostream>
#include <ranges>
#include <set>
#include <stack>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

// The language provides eight commands that modify the tape, move the
// pointer, perform input/output (using the ASCII character encoding) or
// control loops.
enum Commands {
  INCREMENT_DATA_POINTER = '>',
  DECREMENT_DATA_POINTER = '<',

  INCREMENT_BYTE = '+',
  DECREMENT_BYTE = '-',

  OUTPUT_BYTE = '.',
  ACCEPT_BYTE = ',',

  // When the byte at the data pointer is zero, then instead of moving the
  // instruction pointer forward to the next command, jump it forward to the
  // command after the matching ] command.
  CONDITIONAL_JUMP_FORWARD = '[',

  // When the byte at the data pointer is nonzero, then instead of moving
  // the instruction pointer forward to the next command, jump it back to
  // the command after the matching [ command.
  CONDITIONAL_JUMP_BACKWARD = ']'
};

class Interpreter {
public:
  Interpreter(std::size_t tape_size = 10'000) : tape(tape_size, 0) {}

  void run(std::string_view source_code) {
    std::string commands{source_code |
                         std::ranges::views::filter([](char character) {
                           return VALID_COMMANDS.contains(character);
                         }) |
                         std::ranges::to<std::string>()};

    auto jump_table{this->build_jump_table(commands)};

    for (size_t command_pointer{}; command_pointer < commands.size();
         command_pointer++) {
      char command{commands[command_pointer]};
      switch (command) {
      case Commands::INCREMENT_DATA_POINTER: {
        if (this->data_pointer == (this->tape.size() - 1))
          throw std::out_of_range("can't move data pointer further right");

        this->data_pointer++;

        break;
      }

      case Commands::DECREMENT_DATA_POINTER: {
        if (this->data_pointer == 0)
          throw std::out_of_range("can't move data pointer further left");

        this->data_pointer--;

        break;
      }

      case Commands::INCREMENT_BYTE:
        this->tape[this->data_pointer]++;
        break;

      case Commands::DECREMENT_BYTE:
        this->tape[this->data_pointer]--;
        break;

      case Commands::OUTPUT_BYTE:
        std::cout << static_cast<char>(this->tape[this->data_pointer]);
        break;

      case Commands::ACCEPT_BYTE:
        this->tape[this->data_pointer] =
            static_cast<unsigned char>(std::cin.get());
        break;

      case Commands::CONDITIONAL_JUMP_FORWARD:
        if (this->tape[this->data_pointer] == 0)
          command_pointer = jump_table[command_pointer];
        break;

      case Commands::CONDITIONAL_JUMP_BACKWARD:
        if (this->tape[this->data_pointer] != 0)
          command_pointer = jump_table[command_pointer];
        break;
      }
    }
  }

private:
  // In the canonical abstract machine model for Brainfuck, a program operates
  // on a one-dimensional tape of memory cells indexed from zero and unbounded
  // to the right. Each cell contains an 8-bit unsigned integer (0–255) and is
  // initialized to zero.
  std::vector<unsigned char> tape{};

  // A data pointer (initialized to point to the leftmost byte of the array)
  // indicates the current cell on the tape and can move left or right.
  std::size_t data_pointer{};

  static inline const std::set<char> VALID_COMMANDS{
      Commands::INCREMENT_DATA_POINTER,
      Commands::DECREMENT_DATA_POINTER,

      Commands::INCREMENT_BYTE,
      Commands::DECREMENT_BYTE,

      Commands::OUTPUT_BYTE,
      Commands::ACCEPT_BYTE,

      Commands::CONDITIONAL_JUMP_FORWARD,
      Commands::CONDITIONAL_JUMP_BACKWARD,
  };

  std::vector<std::size_t> build_jump_table(std::string_view commands) {
    std::vector<std::size_t> jump_table(commands.size());

    std::stack<std::size_t> stack{};

    for (std::size_t i{}; i < commands.size(); i++) {
      switch (commands[i]) {
      case Commands::CONDITIONAL_JUMP_FORWARD:
        stack.push(i);
        break;

      case Commands::CONDITIONAL_JUMP_BACKWARD:
        if (stack.empty())
          throw std::logic_error(
              "CONDITIONAL_JUMP_BACKWARD command doesn't have any jump target");

        auto jump_target{stack.top()};
        stack.pop();

        jump_table[jump_target] = i;
        jump_table[i] = jump_target;
      }
    }

    if (!stack.empty())
      throw std::logic_error(
          "CONDITIONAL_JUMP_FORWARD command doesn't have any jump target");

    return jump_table;
  }
};

int main() {
  std::string source_code =
      "+++++ +++++[>+++++ +++++<-] >++++.---.+++++ ++..+++.>>>+++[>+++++ +++++<-]>++.<<<<+++++ +++.----- ---.+++.----- -.----- ---.>>+++++ +++++.";

  Interpreter interpreter{};
  interpreter.run(source_code);

  return 0;
}
