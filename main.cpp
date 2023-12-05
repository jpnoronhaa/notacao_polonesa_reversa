#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <sstream>


bool isInt(const std::string& token);
bool isLiteral(const std::string& token);
bool priority(const std::string& token1, const std::string& token2);
std::vector<std::string> reversePolishNotation(const std::string& infix);
double calculator(const std::vector<std::string>& postfixOutput);

int main() {
    std::vector<std::string> postfixOutput;
    std::string input;
    getline(std::cin, input);

    postfixOutput = reversePolishNotation(input);
    std::cout << "Posfixo: ";
    for (auto& token : postfixOutput) {
        std::cout << token << " ";
    }
    std::cout << std::endl;
    double result = calculator(postfixOutput);
    std::cout << "Resultado do calculo: " << result << std::endl;

    return 0;
}

std::vector<std::string> reversePolishNotation(const std::string& infix) {
    std::cout << "Notacao polonesa reversa!" << std::endl;
    std::vector<std::string> postfix;
    std::vector<std::string> tokens;
    std::stack<std::string> stack;
    std::stringstream stream(infix);

    while(stream) {
        std::string temp;
        stream >> temp;
        tokens.push_back(temp);
    }

    for (int i = 0; i < tokens.size(); ++i) {
        if (tokens[i].empty()) {
            tokens.erase(tokens.begin() + i);
        }
    }

    for (int i = 0; i < tokens.size(); ++i) {
        std::cout << "Passo: " << i + 1 << " Token: " << tokens[i] << std::endl;
        if (isInt(tokens[i])) {
            std::cout << "O Token é um número e foi adicionado ao output!" << std::endl;
            postfix.push_back(tokens[i]);
        } else if (isLiteral(tokens[i])) {
            if (stack.empty()) {
                std::cout << "Token adicionado a pilha!" << std::endl;
                stack.push(tokens[i]);
            } else {
                if (priority(tokens[i], stack.top())) {
                    std::cout << "Token adicionado a pilha!" << std::endl;
                    stack.push(tokens[i]);
                } else {
                    while (!priority(tokens[i], stack.top())) {
                        std::cout << "Token: " << stack.top() << " removido da pilha e adicionado ao output!" << std::endl;
                        postfix.push_back(stack.top());
                        stack.pop();
                        if (stack.empty()) {
                            std::cout << "Token: " << tokens[i] << " adicionado a pilha!" << std::endl;
                            stack.push(tokens[i]);
                            break;
                        }
                        if (priority(tokens[i], stack.top())) {
                            std::cout << "Token: " << tokens[i] << " adicionado a pilha!" << std::endl;
                            stack.push(tokens[i]);
                            break;
                        }
                    }
                }
            }
        } else if (tokens[i] == "(") {
            std::cout << "Token adicionado a pilha!" << std::endl;
            stack.push(tokens[i]);
        }else if (tokens[i] == ")") {
            if (stack.empty()) {
                std::cout << "Erro de sintaxe!" << std::endl;
                exit(-1);
            } else {
                if (stack.top() == "(") {
                    std::cout << "Token: " << stack.top() << " removido da pilha!" << std::endl;
                    stack.pop();
                } else {
                    while (stack.top() != "(") {
                        std::cout << "Token: " << stack.top() << " removido da pilha e adicionado ao output!" << std::endl;
                        postfix.push_back(stack.top());
                        stack.pop();
                        if (stack.empty()) {
                            std::cout << "Erro de sintaxe foi digitado ) sem ( !" << std::endl;
                            exit(-1);
                        }
                        if (stack.top() == "(") {
                            std::cout << "Token: " << stack.top() << " removido da pilha!" << std::endl;
                            stack.pop();
                            break;
                        }
                    }
                }
            }
        } else {
            std::cout << "Token: " << tokens[i] << " invalido" << std::endl;
            exit(-1);
        }
    }
    while (!stack.empty()) {
        if (stack.top() == "(") {
            std::cout << "Erro de sintaxe, foi digitado um ( sem ) !" << std::endl;
            exit(-1);
        }
        std::cout << "Token: " << stack.top() << " removido da pilha e adicionado ao output!" << std::endl;
        postfix.push_back(stack.top());
        stack.pop();
    }
    return postfix;
}

double calculator(const std::vector<std::string>& postfixOutput) {
    std::cout << "Calculadora!" << std::endl;
    std::stack<double> stack;

    for (const auto & token : postfixOutput) {
        if (isInt(token)) {
            std::cout << token << " adicionado a pilha!" << std::endl;
            stack.push(std::stod(token));
        } else {
            if (stack.size() < 2) {
                std::cout << "Erro! Existe operador sem operandos suficientes" << std::endl;
                exit(-1);
            }
            double result = 0.0;
            std::cout << stack.top() << " removido da pilha!" << std::endl;
            double value2 = stack.top();
            stack.pop();
            std::cout << stack.top() << " removido da pilha!" << std::endl;
            double value1 = stack.top();
            stack.pop();

            if (token == "+") {
                result = value1 + value2;
                std::cout << value1 << " + " << value2 << " = " << result << std::endl;
            } else if (token == "-") {
                result = value1 - value2;
                std::cout << value1 << " - " << value2 << " = " << result << std::endl;
            } else if (token == "/") {
                result = value1 / value2;
                std::cout << value1 << " / " << value2 << " = " << result << std::endl;
            } else if (token == "*") {
                result = value1 * value2;
                std::cout << value1 << " * " << value2 << " = " << result << std::endl;
            } else if (token == "^") {
                result = pow(value1, value2);
                std::cout << value1 << " ^ " << value2 << " = " << result << std::endl;
            } else {
                std::cout << "Token inválido!" << std::endl;
                exit(-1);
            }

            stack.push(result);
        }
    }
    if (stack.empty()) {
        std::cout << "Pilha vazia no final!" << std::endl;
        exit(-1);
    }
    return stack.top();
}

bool priority(const std::string& token1, const std::string& token2) {
    bool priority = false;

    if (token1 == "+" || token1 == "-") {
        if (token2 == "(") {
            priority = true;
        }
    }

    if (token1 == "*" || token1 == "/") {
        if (token2 == "(" || token2 == "+" || token2 == "-") {
            priority = true;
        }
    }

    if (token1 == "^") {
        if (token2 == "(" || token2 == "+" || token2 == "-" || token1 == "*" || token1 == "/") {
            priority = true;
        }
    }

    return priority;
}

bool isInt(const std::string& token) {
    for (char i : token) {
        if (!isdigit(i)) return false;
    }
    return true;
}

bool isLiteral(const std::string& token) {
    if (token == "+" || token == "-" || token == "*" || token == "/" || token == "^") {
        return true;
    }
    return false;
}