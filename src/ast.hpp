#ifndef LASERC_AST_HPP
#define LASERC_AST_HPP

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace laserc {

class ast_node {
  private:
    uint64_t line;
    uint32_t column;
  public:
    [[nodiscard]] uint64_t get_line() const noexcept;
    [[nodiscard]] uint32_t get_column() const noexcept;
    [[nodiscard]] virtual std::string_view get_node_name() const noexcept = 0;
    virtual ~ast_node() = 0;
};

template<typename T> class value_node : public ast_node {
  public:
    [[nodiscard]] virtual const T &get_value() const noexcept = 0;
    virtual ~value_node() = 0;
};

template<typename T> value_node<T>::~value_node() {}

class identifier_node : public value_node<std::string> {
  private:
    std::string value;

  public:
    identifier_node(std::string value) noexcept;
    [[nodiscard]] std::string_view get_node_name() const noexcept override;
    [[nodiscard]] const std::string &get_value() const noexcept override;
};

class pattern_node : public ast_node {
  private:
    std::unique_ptr<identifier_node> name;

  public:
    pattern_node(std::unique_ptr<identifier_node> name) noexcept;
    [[nodiscard]] std::string_view get_node_name() const noexcept override;
    [[nodiscard]] const identifier_node &get_name() const noexcept;
};

class type_node : public ast_node {
  private:
    std::unique_ptr<identifier_node> name;

  public:
    type_node(std::unique_ptr<identifier_node> name) noexcept;
    [[nodiscard]] std::string_view get_node_name() const noexcept override;
    [[nodiscard]] const identifier_node &get_name() const noexcept;
};

class parameter_node : public ast_node {
  private:
    std::unique_ptr<pattern_node> pattern;
    std::unique_ptr<type_node> type;

  public:
    parameter_node(std::unique_ptr<pattern_node> pattern,
                   std::unique_ptr<type_node> type) noexcept;
    [[nodiscard]] std::string_view get_node_name() const noexcept override;
    [[nodiscard]] const pattern_node &get_pattern() const noexcept;
    [[nodiscard]] const type_node &get_type() const noexcept;
};

class block_expression_node;

class statement_node : public ast_node { // Base interface
  public:
    virtual ~statement_node() = 0;
};

class item_node : public statement_node { // Base interface
  public:
    virtual ~item_node() = 0;
};

class fn_node : public item_node {
  private:
    std::unique_ptr<identifier_node> name;
    std::vector<std::unique_ptr<parameter_node>> parameters;
    std::unique_ptr<block_expression_node> code;

  public:
    fn_node(std::unique_ptr<identifier_node> name,
            std::vector<std::unique_ptr<parameter_node>> parameters,
            std::unique_ptr<block_expression_node> code) noexcept;
    [[nodiscard]] std::string_view get_node_name() const noexcept override;
    [[nodiscard]] const identifier_node &get_name() const noexcept;
    [[nodiscard]] const std::vector<std::unique_ptr<parameter_node>> &
    get_parameters() const noexcept;
    [[nodiscard]] const block_expression_node &get_code() const noexcept;
};

class expression_without_block_node : public statement_node { // Base interface
  public:
    virtual ~expression_without_block_node() = 0;
};

class block_expression_node : public ast_node {
  private:
    std::vector<std::unique_ptr<statement_node>> statements;
    std::unique_ptr<expression_without_block_node> ret_val; // NULLABLE

  public:
    block_expression_node(
        std::vector<std::unique_ptr<statement_node>> statements,
        std::unique_ptr<expression_without_block_node> ret_val) noexcept;
    [[nodiscard]] std::string_view get_node_name() const noexcept override;
    [[nodiscard]] const std::vector<std::unique_ptr<statement_node>> &get_statements() const noexcept;
    [[nodiscard]] expression_without_block_node *get_ret_val() const noexcept;
};

} // namespace laserc

#endif // LASERC_TOKEN_HPP