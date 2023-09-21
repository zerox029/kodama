//
// Created by emma on 12/09/23.
//

#ifndef KODAMA_SRC_EXPRESSION_H_
#define KODAMA_SRC_EXPRESSION_H_

#include "../lexer/token.hpp"
#include "datatypes.hpp"
#include <memory>
#include <llvm/IR/Value.h>

enum AstNodeKind {
  AST_FUNC_DEC,
  AST_FUNC_PARAM,
  AST_BLOCK,
  AST_RETURN,
  AST_IF,
  AST_IF_ELSE,
  AST_WHILE,
  AST_DO_WHILE,
  AST_ASSIGNMENT,
  AST_BINARY_OPERATION,
  AST_FUNCTION_CALL,
  AST_FUNCTION_ARGUMENT,
  AST_VARIABLE,
  AST_NUMBER_LITERAL,
  AST_STRING_LITERAL
};

class AstVisitor;

class AstNode {
 public:
  virtual ~AstNode() = default;

  virtual AstNodeKind GetKind() const = 0;
  virtual llvm::Value* Accept(AstVisitor* visitor) const = 0;
};

typedef std::shared_ptr<AstNode> AstNodePtr;

class FunctionDeclaration : public AstNode {
 private:
  std::string identifier;
  std::vector<AstNodePtr> parameters;
  DataType returnType;
  AstNodePtr body;

 public:
  FunctionDeclaration(std::string identifier, std::vector<AstNodePtr> parameters, DataType returnType, AstNodePtr body);

  std::string GetIdentifier() const;
  std::vector<AstNodePtr> GetParameters() const;
  DataType GetReturnType() const;
  AstNodePtr GetBody() const;

  AstNodeKind GetKind() const override;
  llvm::Value* Accept(AstVisitor* visitor) const override;
};

class FunctionParameter : public AstNode {
 private:
  std::string identifier;
  DataType datatype;

 public:
  FunctionParameter(std::string  identifier, DataType dataType);

  std::string GetIdentifier() const;
  DataType GetDataType() const;

  AstNodeKind GetKind() const override;
  llvm::Value* Accept(AstVisitor* visitor) const override;
};

class Block : public AstNode {
 private:
  std::vector<AstNodePtr> statements;

 public:
  explicit Block(std::vector<AstNodePtr> statements);

  std::vector<AstNodePtr> GetStatements() const;

  AstNodeKind GetKind() const override;
  llvm::Value* Accept(AstVisitor* visitor) const override;
};

class ReturnStatement : public AstNode {
 private:
  AstNodePtr returnValue;

 public:
  explicit ReturnStatement(AstNodePtr returnValue);

  AstNodePtr GetReturnValue() const;

  AstNodeKind GetKind() const override;
  llvm::Value* Accept(AstVisitor* visitor) const override;
};

class IfStatement : public AstNode {
 private:
  AstNodePtr condition;
  AstNodePtr consequent;

 public:
  IfStatement(AstNodePtr condition,
              AstNodePtr consequent);

  AstNodePtr GetCondition() const;
  AstNodePtr GetConsequent() const;

  AstNodeKind GetKind() const override;
  llvm::Value* Accept(AstVisitor* visitor) const override;
};

class IfElseStatement : public AstNode {
 private:
  AstNodePtr condition;
  AstNodePtr consequent;
  AstNodePtr alternative;

 public:
  IfElseStatement(AstNodePtr condition,
                  AstNodePtr consequent,
                  AstNodePtr alternative);

  AstNodePtr GetCondition() const;
  AstNodePtr GetConsequent() const;
  AstNodePtr GetAlternative() const;

  AstNodeKind GetKind() const override;
  llvm::Value* Accept(AstVisitor* visitor) const override;
};

class WhileLoop : public AstNode {
 private:
  AstNodePtr condition;
  AstNodePtr consequent;

 public:
  WhileLoop(AstNodePtr condition,
            AstNodePtr consequent);

  AstNodePtr GetCondition() const;
  AstNodePtr GetConsequent() const;

  AstNodeKind GetKind() const override;
  llvm::Value* Accept(AstVisitor* visitor) const override;
};

class DoWhileLoop : public AstNode {
 private:
  AstNodePtr condition;
  AstNodePtr consequent;

 public:
  DoWhileLoop(AstNodePtr condition,
              AstNodePtr consequent);

  AstNodePtr GetCondition() const;
  AstNodePtr GetConsequent() const;

  AstNodeKind GetKind() const override;
  llvm::Value* Accept(AstVisitor* visitor) const override;
};

class AssignmentExpression : public AstNode {
 private:
  std::string identifier;
  DataType dataType;
  AstNodePtr value;

 public:
  AssignmentExpression(std::string identifier, DataType type, AstNodePtr value);

  std::string GetIdentifier() const;
  DataType GetDataType() const;
  AstNodePtr GetValue() const;

  AstNodeKind GetKind() const override;
  llvm::Value* Accept(AstVisitor* visitor) const override;
};

class BinaryOperation : public AstNode {
 private:
  Token operatorToken;
  AstNodePtr lhs;
  AstNodePtr rhs;

 public:
  BinaryOperation(Token operatorTok, AstNodePtr lhs, AstNodePtr rhs);

  AstNodePtr GetLhs() const;
  AstNodePtr GetRhs() const;
  Token GetOperator() const;

  AstNodeKind GetKind() const override;
  llvm::Value* Accept(AstVisitor* visitor) const override;
};

class FunctionCall : public AstNode {
 private:
  std::string identifier;
  std::vector<AstNodePtr> arguments;
  bool isExtern;

 public:
  FunctionCall(std::string_view identifier, std::vector<AstNodePtr> arguments, bool isExtern);
  std::string GetIdentifier() const;
  std::vector<AstNodePtr> GetArguments() const;
  bool IsExtern() const;

  AstNodeKind GetKind() const override;
  llvm::Value* Accept(AstVisitor* visitor) const override;
};

class FunctionArgument : public AstNode {
 private:
  std::string identifier;
  AstNodePtr value;

 public:
  FunctionArgument(std::string_view identifier, AstNodePtr value);

  std::string GetIdentifier() const;
  AstNodePtr GetValue() const;

  AstNodeKind GetKind() const override;
  llvm::Value* Accept(AstVisitor* visitor) const override;
};

class Variable : public AstNode {
 private:
  std::string identifier;

 public:
  explicit Variable(std::string_view identifier);

  std::string GetIdentifier() const;

  AstNodeKind GetKind() const override;
  llvm::Value* Accept(AstVisitor* visitor) const override;
};

class NumberLiteral : public AstNode {
 private:
  std::string integerValue;
  std::string decimalValue;

 public:
  explicit NumberLiteral(std::string_view integerValue);
  NumberLiteral(std::string_view integerValue, std::string_view decimalValue);

  std::string GetIntegerValue() const;
  std::string GetDecimalValue() const;
  std::string GetValue() const;

  AstNodeKind GetKind() const override;
  llvm::Value* Accept(AstVisitor* visitor) const override;
};

class StringLiteral : public AstNode {
 private:
  std::string value;

 public:
  explicit StringLiteral(std::string_view value);

  std::string GetValue() const;

  AstNodeKind GetKind() const override;
  llvm::Value* Accept(AstVisitor* visitor) const override;
};

#endif //KODAMA_SRC_EXPRESSION_H_
