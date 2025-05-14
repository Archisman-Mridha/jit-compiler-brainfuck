module;

#include <iostream>
#include <optional>
#include <stack>
#include <vector>

export module ir;

import lexer;

using namespace std;

export struct IR {
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

export class IRsGenerator {
  private:
    Lexer* lexer;

  public:
    IRsGenerator(Lexer* lexer) : lexer(lexer) { }

    vector<IR> generate( ) {
      vector<IR> irs;

      stack<int> backPatchingHelper;

      auto currentTokenOptional = this->lexer->next( );
      while (currentTokenOptional.has_value( )) {
        auto currentToken = currentTokenOptional.value( );
        switch (currentToken) {
          case Token::INCREMENT:
          case Token::DECREMENT:
          case Token::MOVE_LEFT:
          case Token::MOVE_RIGHT:
          case Token::INPUT:
          case Token::OUTPUT: {
            size_t operand = 1;
            while (this->lexer->next_if_token(currentToken).has_value( ))
              operand += 1;

            auto ir = IR{.token = currentToken, .operand = operand};
            irs.push_back(ir);

            break;
          }

          case Token::JUMP_IF_ZERO: {
            auto ir         = IR{.token = currentToken, .operand = 0};
            int  irPosition = irs.size( );
            irs.push_back(ir);

            backPatchingHelper.push(irPosition);

            break;
          }

          case Token::JUMP_IF_NON_ZERO: {
            if (backPatchingHelper.size( ) == 0) {
              // TODO : Report line and column numbers.
              cerr << "No previous JUMP_IF_ZERO token for the current JUMP_IF_NOT_ZERO token"
                   << endl;
              exit(1);
            }

            int jumpedFromIRPosition = backPatchingHelper.top( );
            backPatchingHelper.pop( );
            auto jumpedFromIR = irs[jumpedFromIRPosition];

            size_t jumpToIRPosition = irs.size( );
            jumpedFromIR.operand    = jumpToIRPosition;

            break;
          }
        }

        currentTokenOptional = this->lexer->next( );
      }
      return irs;
    }
};
