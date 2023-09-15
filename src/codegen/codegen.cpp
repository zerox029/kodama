//
// Created by emma on 14/09/23.
//

#include "codegen.hpp"
#include <iostream>

void
Codegen::Visit(const NumberLiteral* element) {
  std::cout << "Visited number literal: " << element->GetValue() << std::endl;
}

void
Codegen::Visit(const BinaryOperation* element) {
  element->GetLhs()->Accept(this);
  std::cout << "Visited binary operator: " << element->GetToken().getStr() << std::endl;
  element->GetRhs()->Accept(this);
}