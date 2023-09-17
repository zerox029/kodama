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
  AST_BLOCK,
  AST_RETURN,
  AST_IF,
  AST_IF_ELSE,
  AST_WHILE,
  AST_DO_WHILE,
  AST_ASSIGNMENT,
  AST_BINARY_OPERATION,
  AST_NUMBER_LITERAL,
  AST_VARIABLE
};

class AstVisitor;

class AstNode {
 public:
  virtual ~AstNode() = default;

  virtual AstNodeKind GetKind() const = 0;
  virtual llvm::Value* Accept(AstVisitor* visitor) const = 0;
};

class Block : public AstNode {
 private:
  std::vector<std::shared_ptr<AstNode>> statements;

 public:
  explicit Block(std::vector<std::shared_ptr<AstNode>> statements);

  std::vector<std::shared_ptr<AstNode>> GetStatements() const;

  AstNodeKind GetKind() const override;
  llvm::Value* Accept(AstVisitor* visitor) const override;
};

class ReturnStatement : public AstNode {
 private:
  std::shared_ptr<AstNode> returnValue;

 public:
  explicit ReturnStatement(std::shared_ptr<AstNode> returnValue);

  std::shared_ptr<AstNode> GetReturnValue() const;

  AstNodeKind GetKind() const override;
  llvm::Value* Accept(AstVisitor* visitor) const override;
};

class IfStatement : public AstNode {
 private:
  std::shared_ptr<AstNode> condition;
  std::shared_ptr<AstNode> consequent;

 public:
  IfStatement(std::shared_ptr<AstNode> condition,
              std::shared_ptr<AstNode> consequent);

  std::shared_ptr<AstNode> GetCondition() const;
  std::shared_ptr<AstNode> GetConsequent() const;

  AstNodeKind GetKind() const override;
  llvm::Value* Accept(AstVisitor* visitor) const override;
};

class IfElseStatement : public AstNode {
 private:
  std::shared_ptr<AstNode> condition;
  std::shared_ptr<AstNode> consequent;
  std::shared_ptr<AstNode> alternative;

 public:
  IfElseStatement(std::shared_ptr<AstNode> condition,
                  std::shared_ptr<AstNode> consequent,
                  std::shared_ptr<AstNode> alternative);

  std::shared_ptr<AstNode> GetCondition() const;
  std::shared_ptr<AstNode> GetConsequent() const;
  std::shared_ptr<AstNode> GetAlternative() const;

  AstNodeKind GetKind() const override;
  llvm::Value* Accept(AstVisitor* visitor) const override;
};

class WhileLoop : public AstNode {
 private:
  std::shared_ptr<AstNode> condition;
  std::shared_ptr<AstNode> consequent;

 public:
  WhileLoop(std::shared_ptr<AstNode> condition,
            std::shared_ptr<AstNode> consequent);

  std::shared_ptr<AstNode> GetCondition() const;
  std::shared_ptr<AstNode> GetConsequent() const;

  AstNodeKind GetKind() const override;
  llvm::Value* Accept(AstVisitor* visitor) const override;
};

class DoWhileLoop : public AstNode {
 private:
  std::shared_ptr<AstNode> condition;
  std::shared_ptr<AstNode> consequent;

 public:
  DoWhileLoop(std::shared_ptr<AstNode> condition,
              std::shared_ptr<AstNode> consequent);

  std::shared_ptr<AstNode> GetCondition() const;
  std::shared_ptr<AstNode> GetConsequent() const;

  AstNodeKind GetKind() const override;
  llvm::Value* Accept(AstVisitor* visitor) const override;
};

class AssignmentExpression : public AstNode {
 private:
  std::string identifier;
  DataType dataType;
  std::shared_ptr<AstNode> value;

 public:
  AssignmentExpression(std::string identifier, DataType type, std::shared_ptr<AstNode> value);

  std::string GetIdentifier() const;
  DataType GetDataType() const;
  std::shared_ptr<AstNode> GetValue() const;

  AstNodeKind GetKind() const override;
  llvm::Value* Accept(AstVisitor* visitor) const override;
};

class BinaryOperation : public AstNode {
 private:
  Token operatorToken;
  std::shared_ptr<AstNode> lhs;
  std::shared_ptr<AstNode> rhs;

 public:
  BinaryOperation(Token operatorTok, std::shared_ptr<AstNode> lhs, std::shared_ptr<AstNode> rhs);

  std::shared_ptr<AstNode> GetLhs() const;
  std::shared_ptr<AstNode> GetRhs() const;
  Token GetOperator() const;

  AstNodeKind GetKind() const override;
  llvm::Value* Accept(AstVisitor* visitor) const override;
};

class NumberLiteral : public AstNode {
 private:
  std::string value;

 public:
  explicit NumberLiteral(std::string_view val);

  std::string GetValue() const;

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

#endif //KODAMA_SRC_EXPRESSION_H_
