//
// Created by emma on 12/09/23.
//

#ifndef KODAMA_SRC_EXPRESSION_H_
#define KODAMA_SRC_EXPRESSION_H_

#include "../lexer/token.hpp"
#include "../analyzer/type/datatype.hpp"
#include <memory>
#include <llvm/IR/Value.h>

enum AstNodeKind {
  AST_PROGRAM,
  AST_STRUCT,
  AST_FUNC_DEC,
  AST_FUNC_PARAM,
  AST_BLOCK,
  AST_RETURN,
  AST_IF,
  AST_IF_ELSE,
  AST_WHILE,
  AST_DO_WHILE,
  AST_FOR,
  AST_ASSIGNMENT,
  AST_REASSIGNMENT,
  AST_BINARY_OPERATION,
  AST_FUNCTION_CALL,
  AST_FUNCTION_ARGUMENT,
  AST_VARIABLE,
  AST_INTEGER_LITERAL,
  AST_DECIMAL_LITERAL,
  AST_STRING_LITERAL,
  AST_NULL_VALUE,
  AST_BOOL_VALUE
};

class AstVisitor;

class AstNode {
 public:
  AstNode(Token token) : token{token} {};
  virtual ~AstNode() = default;

  const Token& GetToken() const { return token; };
  virtual AstNodeKind GetKind() const = 0;
  virtual void Accept(AstVisitor* visitor) = 0;

 protected:
  Token token;
};

typedef std::shared_ptr<AstNode> AstNodePtr;

class Program : public AstNode {
 private:
  std::vector<AstNodePtr> statements;

 public:
  Program(Token token, std::vector<AstNodePtr> statements);

  std::vector<AstNodePtr> GetStatements() const;

  AstNodeKind GetKind() const override;
  void Accept(AstVisitor* visitor) override;
};

class Struct : public AstNode {
 private:
  std::string identifier;
  std::vector<AstNodePtr> members;

 public:
  Struct(Token token, std::string identifier, std::vector<AstNodePtr> members);

  std::string GetIdentifier() const;
  std::vector<AstNodePtr> GetMembers() const;

  AstNodeKind GetKind() const override;
  void Accept(AstVisitor* visitor) override;
};

class FunctionDeclaration : public AstNode {
 private:
  std::string identifier;
  std::vector<AstNodePtr> parameters;
  TypePtr returnType;
  AstNodePtr body;

 public:
  FunctionDeclaration(Token token,
                      std::string identifier,
                      std::vector<AstNodePtr> parameters,
                      TypePtr returnType,
                      AstNodePtr body);

  std::string GetIdentifier() const;
  std::vector<AstNodePtr> GetParameters() const;
  TypePtr GetReturnType() const;
  AstNodePtr GetBody() const;

  AstNodeKind GetKind() const override;
  void Accept(AstVisitor* visitor) override;
};

class Parameter : public AstNode {
 private:
  std::string identifier;
  TypePtr datatype;

 public:
  Parameter(Token token, std::string identifier, TypePtr dataType);

  std::string GetIdentifier() const;
  TypePtr GetDataType() const;

  AstNodeKind GetKind() const override;
  void Accept(AstVisitor* visitor) override;
};

class Block : public AstNode {
 private:
  std::vector<AstNodePtr> statements;

 public:
  Block(Token token, std::vector<AstNodePtr> statements);

  std::vector<AstNodePtr> GetStatements() const;

  AstNodeKind GetKind() const override;
  void Accept(AstVisitor* visitor) override;
};

class ReturnStatement : public AstNode {
 private:
  AstNodePtr returnValue;

 public:
  ReturnStatement(Token token, AstNodePtr returnValue);

  AstNodePtr GetReturnValue() const;

  AstNodeKind GetKind() const override;
  void Accept(AstVisitor* visitor) override;
};

class IfStatement : public AstNode {
 private:
  AstNodePtr condition;
  AstNodePtr consequent;

 public:
  IfStatement(Token token, AstNodePtr condition, AstNodePtr consequent);

  AstNodePtr GetCondition() const;
  AstNodePtr GetConsequent() const;

  AstNodeKind GetKind() const override;
  void Accept(AstVisitor* visitor) override;
};

class IfElseStatement : public AstNode {
 private:
  AstNodePtr condition;
  AstNodePtr consequent;
  AstNodePtr alternative;

 public:
  IfElseStatement(Token token, AstNodePtr condition, AstNodePtr consequent, AstNodePtr alternative);

  AstNodePtr GetCondition() const;
  AstNodePtr GetConsequent() const;
  AstNodePtr GetAlternative() const;

  AstNodeKind GetKind() const override;
  void Accept(AstVisitor* visitor) override;
};

class WhileLoop : public AstNode {
 private:
  AstNodePtr condition;
  AstNodePtr consequent;

 public:
  WhileLoop(Token token, AstNodePtr condition, AstNodePtr consequent);

  AstNodePtr GetCondition() const;
  AstNodePtr GetConsequent() const;

  AstNodeKind GetKind() const override;
  void Accept(AstVisitor* visitor) override;
};

class DoWhileLoop : public AstNode {
 private:
  AstNodePtr condition;
  AstNodePtr consequent;

 public:
  DoWhileLoop(Token token, AstNodePtr condition, AstNodePtr consequent);

  AstNodePtr GetCondition() const;
  AstNodePtr GetConsequent() const;

  AstNodeKind GetKind() const override;
  void Accept(AstVisitor* visitor) override;
};

class ForLoop : public AstNode {
 private:
  std::string identifier;
  AstNodePtr from;
  AstNodePtr to;
  AstNodePtr consequent;

 public:
  ForLoop(Token token, std::string identifier, AstNodePtr from, AstNodePtr to, AstNodePtr consequent);

  const std::string& GetIdentifier() const;
  AstNodePtr GetFrom() const;
  AstNodePtr GetTo() const;
  const AstNodePtr& GetConsequent() const;

  AstNodeKind GetKind() const override;
  void Accept(AstVisitor* visitor) override;
};

class AssignmentExpression : public AstNode {
 private:
  std::string identifier;
  TypePtr dataType;
  AstNodePtr value;

 public:
  AssignmentExpression(Token token, std::string identifier, const TypePtr& dataType); // TODO: Move this to its own 'varDeclaration' AST node
  AssignmentExpression(Token token, std::string identifier, TypePtr type, AstNodePtr value);

  std::string GetIdentifier() const;
  TypePtr GetDataType() const;
  AstNodePtr GetValue() const;

  AstNodeKind GetKind() const override;
  void Accept(AstVisitor* visitor) override;
};

class ReassignmentExpression : public AstNode {
 private:
  std::string identifier;
  AstNodePtr value;

 public:
  ReassignmentExpression(Token token, std::string identifier, AstNodePtr value);

  std::string GetIdentifier() const;
  AstNodePtr GetValue() const;

  AstNodeKind GetKind() const override;
  void Accept(AstVisitor* visitor) override;
};


class BinaryOperation : public AstNode {
 private:
  AstNodePtr lhs;
  AstNodePtr rhs;

 public:
  BinaryOperation(Token token, AstNodePtr lhs, AstNodePtr rhs);

  AstNodePtr GetLhs() const;
  AstNodePtr GetRhs() const;
  Token GetOperator() const;

  AstNodeKind GetKind() const override;
  void Accept(AstVisitor* visitor) override;
};

class FunctionCall : public AstNode {
 private:
  std::string identifier;
  std::vector<AstNodePtr> arguments;
  bool isExtern;

 public:
  FunctionCall(Token token, std::string_view identifier, std::vector<AstNodePtr> arguments, bool isExtern);
  std::string GetIdentifier() const;
  std::vector<AstNodePtr> GetArguments() const;
  bool IsExtern() const;

  AstNodeKind GetKind() const override;
  void Accept(AstVisitor* visitor) override;
};

class FunctionArgument : public AstNode {
 private:
  std::string identifier;
  AstNodePtr value;

 public:
  FunctionArgument(Token token, AstNodePtr value);
  FunctionArgument(Token token, std::string_view identifier, AstNodePtr value);

  std::string GetIdentifier() const;
  AstNodePtr GetValue() const;

  AstNodeKind GetKind() const override;
  void Accept(AstVisitor* visitor) override;
};

class Variable : public AstNode {
 private:
  std::string identifier;

 public:
  Variable(Token token, std::string_view identifier);

  std::string GetIdentifier() const;

  AstNodeKind GetKind() const override;
  void Accept(AstVisitor* visitor) override;
};

class IntegerLiteral : public AstNode {
 private:
  std::string integerValue;
  TypePtr datatype;

 public:
  IntegerLiteral(Token token, std::string_view integerValue);

  std::string GetValue() const;
  const TypePtr& GetDatatype() const;
  void SetDatatype(TypePtr dataype);

  AstNodeKind GetKind() const override;
  void Accept(AstVisitor* visitor) override;
};

class DecimalLiteral : public AstNode {
 private:
  std::string integerValue;
  std::string decimalValue;
  TypePtr datatype;

 public:
  DecimalLiteral(Token token, std::string_view integerValue, std::string_view decimalValue);

  std::string GetIntegerValue() const;
  std::string GetDecimalValue() const;
  std::string GetValue() const;
  const TypePtr& GetDatatype() const;
  void SetDatatype(TypePtr dataype);

  AstNodeKind GetKind() const override;
  void Accept(AstVisitor* visitor) override;
};

class StringLiteral : public AstNode {
 private:
  std::string value;

 public:
  StringLiteral(Token token, std::string_view value);

  std::string GetValue() const;

  AstNodeKind GetKind() const override;
  void Accept(AstVisitor* visitor) override;
};

class BoolValue : public AstNode {
 private:
  bool value;

 public:
  BoolValue(Token token, bool value);

  bool GetValue() const;

  AstNodeKind GetKind() const override;
  void Accept(AstVisitor* visitor) override;
};

class NullValue : public AstNode {
 private:

 public:
  explicit NullValue(Token token);
  AstNodeKind GetKind() const override;
  void Accept(AstVisitor* visitor) override;
};

#endif //KODAMA_SRC_EXPRESSION_H_
