//
// Created by emma on 23/09/23.
//

#include "semanticValidator.hpp"
#include "../errors/errorFactory.hpp"
#include <cassert>

SemanticValidator::SemanticValidator(const std::vector<std::string>& code, const std::vector<Token>& tokens)
    : code{code}, tokens{tokens} {}

std::vector<Errors::Error>
SemanticValidator::Validate(const AstNodePtr& ast) {
  ast->Accept(this);

  return errors;
}

void
SemanticValidator::Visit(Program* element) {
  for (const AstNodePtr& node : element->GetStatements()) {
    switch (node->GetKind()) {
      case AST_BINARY_OPERATION:
      case AST_VARIABLE:
      case AST_INTEGER_LITERAL:
      case AST_DECIMAL_LITERAL:
      case AST_STRING_LITERAL:
      case AST_NULL_VALUE:
      case AST_BOOL_VALUE:
        errors.push_back(Errors::Generate(Errors::UNASSIGNED_VALUE,
                                          element->GetToken().GetLocation(),
                                          code));
        break;
      case AST_RETURN:
      case AST_IF:
      case AST_IF_ELSE:
      case AST_WHILE:
      case AST_DO_WHILE:
      case AST_BLOCK:
      case AST_REASSIGNMENT:
      case AST_FUNCTION_CALL:
        errors.push_back(Errors::Generate(Errors::INVALID_OUTSIDE_FUNCTION,
                                          element->GetToken().GetLocation(),
                                          code));
        break;
      default:
        node->Accept(this);
        break;
    }
  }
}

void
SemanticValidator::Visit(Struct* element) {

}


void
SemanticValidator::Visit(FunctionDeclaration* element) {
  element->GetBody()->Accept(this);
}

void
SemanticValidator::Visit(Parameter* element) {

}

void
SemanticValidator::Visit(Block* element) {
  std::unordered_map<std::string, TypePtr> oldSymbolTable;

  for (size_t i = 0; i < element->GetStatements().size(); i++) {
    switch (element->GetStatements().at(i)->GetKind()) {
      case AST_BINARY_OPERATION:
      case AST_VARIABLE:
      case AST_INTEGER_LITERAL:
      case AST_DECIMAL_LITERAL:
      case AST_STRING_LITERAL:
      case AST_NULL_VALUE:
      case AST_BOOL_VALUE:
        errors.push_back(Errors::Generate(Errors::UNASSIGNED_VALUE,
                                          element->GetToken().GetLocation(),
                                          code));
        break;
      case AST_RETURN:
        if (i < (element->GetStatements().size() - 1)) {
          errors.push_back(Errors::Generate(Errors::UNASSIGNED_VALUE,
                                            element->GetToken().GetLocation(),
                                            code));
          break;
        }
      default:
        element->GetStatements().at(i)->Accept(this);
    }
  }

  symbolTable = oldSymbolTable;
}

void
SemanticValidator::Visit(ReturnStatement* element) {

}

void
SemanticValidator::Visit(IfStatement* element) {
  element->GetConsequent()->Accept(this);
}

void
SemanticValidator::Visit(IfElseStatement* element) {
  element->GetConsequent()->Accept(this);
  element->GetAlternative()->Accept(this);
}

void
SemanticValidator::Visit(WhileLoop* element) {
  element->GetConsequent()->Accept(this);
}

void
SemanticValidator::Visit(DoWhileLoop* element) {
  element->GetConsequent()->Accept(this);
}

void
SemanticValidator::Visit(ForLoop* element) {
  element->GetConsequent()->Accept(this);
}

void
SemanticValidator::Visit(AssignmentExpression* element) {
  // Verify that the variable was not already defined
  if (symbolTable.contains(element->GetIdentifier())) {
    errors.push_back(Errors::Generate(Errors::ILLEGAL_REDEFINITION,
                                      element->GetToken().GetLocation(),
                                      code,
                                      element->GetIdentifier()));
  } else {
    symbolTable.insert({element->GetIdentifier(), element->GetDataType()});
  }

  element->GetValue()->Accept(this);
}

void
SemanticValidator::Visit(ReassignmentExpression* element) {
  // Verify that the variable being reassigned to exists
  if (!symbolTable.contains(element->GetIdentifier())) {
    errors.push_back(Errors::Generate(Errors::ID_NOT_FOUND,
                                      element->GetToken().GetLocation(),
                                      code,
                                      element->GetIdentifier()));
  }

  //Verify that the variable being reassigned to is mutable
  if (!symbolTable.at(element->GetIdentifier())->IsMutable()) {
    errors.push_back(Errors::Generate(Errors::ASSIGN_VAL,
                                      element->GetToken().GetLocation(),
                                      code,
                                      element->GetIdentifier()));
  }

  element->GetValue()->Accept(this);
}

void
SemanticValidator::Visit(BinaryOperation* element) {
  element->GetLhs()->Accept(this);
  element->GetRhs()->Accept(this);
}

void
SemanticValidator::Visit(FunctionCall* element) {
  // Verify that the function exists
  if (!symbolTable.contains(element->GetIdentifier())) {
    errors.push_back(Errors::Generate(Errors::ID_NOT_FOUND,
                                      element->GetToken().GetLocation(),
                                      code,
                                      element->GetIdentifier()));
  }

  ValidateFunctionArguments(element->GetArguments());
}

void
SemanticValidator::ValidateFunctionArguments(const std::vector<AstNodePtr>& arguments) {
  // Verify that no positional argument is found after a named argument
  bool foundNamedArgument = false;
  for (const AstNodePtr arg : arguments) {
    FunctionArgument* argument = dynamic_cast<FunctionArgument*>(arg.get());
    if (argument->GetIdentifier() != "") {
      foundNamedArgument = true;
    }
    if (argument->GetIdentifier() == "" && foundNamedArgument) {
      errors.push_back(Errors::Generate(Errors::INVALID_POSITIONAL_ARGUMENT, argument->GetToken().GetLocation(), code));
    }
  }
}

void
SemanticValidator::Visit(FunctionArgument* element) {

}

void
SemanticValidator::Visit(Variable* element) {
  // Verify that the function exists
  if (!symbolTable.contains(element->GetIdentifier())) {
    errors.push_back(Errors::Generate(Errors::ID_NOT_FOUND,
                                      element->GetToken().GetLocation(),
                                      code,
                                      element->GetIdentifier()));
  }
}

void
SemanticValidator::Visit(IntegerLiteral* element) {

}

void
SemanticValidator::Visit(DecimalLiteral* element) {

}

void
SemanticValidator::Visit(StringLiteral* element) {

}

void
SemanticValidator::Visit(BoolValue* element) {

}

void
SemanticValidator::Visit(NullValue* element) {

}