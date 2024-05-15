#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#include "scanner.h"

static char* strtoken(Token token) {
    switch (token.type) {
        // 单字符Token
    case TOKEN_LEFT_PAREN:      return "(";
    case TOKEN_RIGHT_PAREN:     return ")";
    case TOKEN_LEFT_BRACKET:    return "[";
    case TOKEN_RIGHT_BRACKET:   return "]";
    case TOKEN_LEFT_BRACE:      return "{";
    case TOKEN_RIGHT_BRACE:     return "}";
    case TOKEN_COMMA:           return ",";
    case TOKEN_DOT:             return ".";
    case TOKEN_SEMICOLON:       return ";";
    case TOKEN_TILDE:           return "~";
    case TOKEN_HASH:            return "#";
        // 一个字符或两个字符的Token
    case TOKEN_PLUS:            return "+";
    case TOKEN_PLUS_PLUS:       return "++";
    case TOKEN_PLUS_EQUAL:      return "+=";
    case TOKEN_MINUS:           return "-";
    case TOKEN_MINUS_MINUS:     return "--";
    case TOKEN_MINUS_EQUAL:     return "-=";
    case TOKEN_MINUS_GREATER:   return "->";
    case TOKEN_STAR:            return "*";
    case TOKEN_STAR_EQUAL:      return "*=";
    case TOKEN_SLASH:           return "/";
    case TOKEN_SLASH_EQUAL:     return "/=";
    case TOKEN_PERCENT:         return "%";
    case TOKEN_PERCENT_EQUAL:   return "%=";
    case TOKEN_AMPER:           return "&";
    case TOKEN_AMPER_EQUAL:     return "&=";
    case TOKEN_AMPER_AMPER:     return "&&";
    case TOKEN_PIPE:            return "|";
    case TOKEN_PIPE_EQUAL:      return "|=";
    case TOKEN_PIPE_PIPE:       return "||";
    case TOKEN_HAT:             return "^";
    case TOKEN_HAT_EQUAL:       return "^=";
    case TOKEN_EQUAL:           return "=";
    case TOKEN_EQUAL_EQUAL:     return "==";
    case TOKEN_BANG:            return "!";
    case TOKEN_BANG_EQUAL:      return "!=";
    case TOKEN_LESS:            return "<";
    case TOKEN_LESS_EQUAL:      return "<=";
    case TOKEN_LESS_LESS:       return "<<";
    case TOKEN_GREATER:         return ">";
    case TOKEN_GREATER_EQUAL:   return ">=";
        // 字面值: 标识符, 字符, 字符串, 数字
    case TOKEN_IDENTIFIER:      return "IDENTIFIER";
    case TOKEN_CHARACTER:       return "CHARACTER";
    case TOKEN_STRING:          return "STRING";
    case TOKEN_NUMBER:          return "NUMBER";
        // 关键字
    case TOKEN_SIGNED:          return "SIGNED";
    case TOKEN_UNSIGNED:        return "UNSIGNED";
    case TOKEN_CHAR:            return "CHAR";
    case TOKEN_SHORT:           return "SHORT";
    case TOKEN_INT:             return "INT";
    case TOKEN_LONG:            return "LONG";
    case TOKEN_FLOAT:           return "FLOAT";
    case TOKEN_DOUBLE:          return "DOUBLE";
    case TOKEN_STRUCT:          return "STRUCT";
    case TOKEN_UNION:           return "UNION";
    case TOKEN_ENUM:            return "ENUM";
    case TOKEN_VOID:            return "VOID";
    case TOKEN_IF:              return "IF";
    case TOKEN_ELSE:            return "ELSE";
    case TOKEN_SWITCH:          return "SWITCH";
    case TOKEN_CASE:            return "CASE";
    case TOKEN_DEFAULT:         return "DEFAULT";
    case TOKEN_WHILE:           return "WHILE";
    case TOKEN_DO:              return "DO";
    case TOKEN_FOR:             return "FOR";
    case TOKEN_BREAK:           return "BREAK";
    case TOKEN_CONTINUE:        return "CONTINUE";
    case TOKEN_RETURN:          return "RETURN";
    case TOKEN_GOTO:            return "GOTO";
    case TOKEN_CONST:           return "CONST";
    case TOKEN_SIZEOF:          return "SIZEOF";
    case TOKEN_TYPEDEF:         return "TYPEDEF";
        // 辅助Token
    case TOKEN_ERROR:           return "ERROR";
    case TOKEN_EOF:             return "EOF";
    }
}

static void run(const char* source) {
    initScanner(source);
    int line = -1;
    for (;;) {
        Token token = scanToken();
        if (token.line != line) {
            printf("%4d:%-4d ", token.line, token.column);
            line = token.line;
        }
        else {
            printf("%4d:%-4d ", token.line, token.column);
        }
        printf("%s '%.*s'\n", strtoken(token), token.length, token.start);

        if (token.type == TOKEN_EOF) break;
    }
}


static void repl() {
    // 与用户交互，用户每输入一行代码，分析一行代码，并将结果输出
    // repl是"read evaluate print loop"的缩写
    // 读一行，run一次
    char line[1024];
    for (;;) {
        printf("> ");

        if (fgets(line, sizeof(line), stdin) == NULL) {
            printf("\n");
            break;
        }

        run(line);
    }
}

static char* readFile(const char* path) {
    // 用户输入文件名，将整个文件的内容读入内存，并在末尾添加'\0'
    // 注意: 这里应该使用动态内存分配，因此应该事先确定文件的大小。
    FILE* stream = fopen(path, "rb");
    if (!stream) {
        fprintf(stderr, "open file %s failed\n", path);
    }

    //确定文件大小
    fseek(stream, 0, SEEK_END);
    long n = ftell(stream);
    char* content = malloc(n + 1);

    //读取文件的内容，填充contentvim数组
    rewind(stream);  //回到文件开头
    int bytes = fread(content, 1, n, stream);
    content[bytes] = '\0';

    return content;
}


static void runFile(const char* path) {
    // 处理'.c'文件:用户输入文件名，分析整个文件，并将结果输出
    char* source = readFile(path);
    run(source);   //调用initScanner和scanToken做词法分析
}


int main(int argc, const char* argv[]) {
    if (argc == 1) {
        //如果没有参数，则进入交互式界面
        repl();    //交互式界面
    }
    else if (argc == 2) {
        // ./scanner后面跟入了一个参数，需要分析整个文件
        runFile(argv[1]);
    }
    else {
        fprintf(stderr, "Usage: scanner [path]\n");
        exit(1);
    }

    return 0;
}
