module;

#include <iostream>
#include <optional>
#include <stack>
#include <vector>

export module ir;

import lexer;

using namespace std;

export struct IR {
  public:
    Token token;

    /*
      Depending on the token, Represents one of the following :

        (1) how many times we're going to MOVE_LEFT / MOVE_RIGHT / INCREMENT / DECREMENT.

        (2) how many bytes we're going OUTPUT / to take as INPUT.

        (3) the address, where we want to jump if the current token is JUMP_IF_ZERO /
            JUMP_IF_NON_ZERO.
    */
    size_t operand;
};

export class IRGenerator {
  public:
    IRGenerator(Lexer* lexer) : lexer(lexer) { }

    vector<IR> generate( ) {
      vector<IR> irs;

      stack<int> back_patching_helper;

      auto current_token_optional = this->lexer->next( );
      while (current_token_optional.has_value( )) {
        auto current_token = current_token_optional.value( );
        switch (current_token) {
          case Token::INCREMENT:
          case Token::DECREMENT:
          case Token::MOVE_LEFT:
          case Token::MOVE_RIGHT:
          case Token::INPUT:
          case Token::OUTPUT: {
            size_t operand = 1;
            while (this->lexer->next_if_token(current_token).has_value( ))
              operand += 1;

            auto ir = IR{.token = current_token, .operand = operand};
            irs.push_back(ir);

            break;
          }

          case Token::JUMP_IF_ZERO: {
            auto ir          = IR{.token = current_token, .operand = 0};
            int  ir_position = irs.size( );
            irs.push_back(ir);

            back_patching_helper.push(ir_position);

            break;
          }

          case Token::JUMP_IF_NON_ZERO: {
            if (back_patching_helper.size( ) == 0) {
              // TODO : Report line and column numbers.
              cerr << "No previous JUMP_IF_ZERO token for the current JUMP_IF_NOT_ZERO token"
                   << endl;
              exit(1);
            }

            int jumped_from_ir_position = back_patching_helper.top( );
            back_patching_helper.pop( );
            auto jumped_from_ir = irs[jumped_from_ir_position];

            size_t jumpToIRPosition = irs.size( );
            jumped_from_ir.operand  = jumpToIRPosition;

            break;
          }
        }

        current_token_optional = this->lexer->next( );
      }
      return irs;
    }

  private:
    Lexer* lexer;
};
