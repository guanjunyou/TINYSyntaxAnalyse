/****************************************************/
/* File: parse.c                                    */
/* The parser implementation for the TINY compiler  */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/
#include "stdafx.h"
#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"
#include<iostream>
using namespace std;

static TokenTypeInProjece token; /* holds current token */

/* function prototypes for recursive calls */
static TreeNode* stmt_sequence(void);
static TreeNode* statement(void);
static TreeNode* if_stmt(void);
static TreeNode* repeat_stmt(void);
static TreeNode* assign_stmt(void);
static TreeNode* read_stmt(void);
static TreeNode* write_stmt(void);
static TreeNode* exp(void);
static TreeNode* simple_exp(void);
static TreeNode* term(void);
static TreeNode* factor(void);
static TreeNode* for_stmt(void);
static TreeNode* power(void);
static TreeNode* notterm(void);
static TreeNode* orterm(void);
static TreeNode* andterm(void);

static void syntaxError(char* message)
{
    fprintf(listing, "\n>>> ");
    fprintf(listing, "Syntax error at line %d: %s", lineno, message);
    Error = TRUE;
}

static void match(TokenTypeInProjece expected)
{
    if (token == expected) token = getToken();
    else {
        syntaxError("unexpected token -> ");
        printToken(token, tokenString);
        fprintf(listing, "      ");
    }
}

TreeNode* stmt_sequence(void)
{

    TreeNode* t = statement();
    TreeNode* p = t;

    while ((token != ENDFILE) && (token != ENDIF) &&
        (token != ELSE) && (token != UNTIL) && (token != ENDDO) )
    { // 如果还没结束则接着读入下一条语句
        TreeNode* q;

        match(SEMI); // 匹配行结束
  
        q = statement();
        if (q != NULL) {
            if (t == NULL) t = p = q;
            else /* now p cannot be NULL either */
            {
                p->sibling = q;
                p = q;
            }
        }
    }
    return t;
}


//P394 
//lineno: 961
TreeNode* statement(void)
{

    TreeNode* t = NULL;
    switch (token) {
    case IF: t = if_stmt(); break;
    case REPEAT: t = repeat_stmt(); break;
    case ID: t = assign_stmt(); break;
    case READ: t = read_stmt(); break;
    case WRITE: t = write_stmt(); break;
    case FOR: t = for_stmt(); break;
    default: syntaxError("unexpected token -> ");
        printToken(token, tokenString);
        token = getToken();
        break;
    } /* end case */
    return t;
}


//P394 
//lineno: 977
TreeNode* if_stmt(void)
{

    TreeNode* t = newStmtNode(IfK);
    match(IF);
    if (t != NULL) {
        match(LPAREN);
        t->child[0] = exp();
        match(RPAREN);
    }
    if (t != NULL) t->child[1] = stmt_sequence();
    if (token == ELSE) {
        match(ELSE);
        if (t != NULL) t->child[2] = stmt_sequence();
    }

    return t;
}

//P394 
//lineno:991
TreeNode* repeat_stmt(void)
{
    TreeNode* t = newStmtNode(RepeatK);
    match(REPEAT);
    if (t != NULL) t->child[0] = stmt_sequence();
    match(UNTIL);
    if (t != NULL) t->child[1] = exp();
    return t;
}


TreeNode* assign_stmt(void)
{
    TreeNode* t = newStmtNode(AssignK);
    if ((t != NULL) && (token == ID))
        t->attr.name = copyString(tokenString);
    match(ID);

    TreeNode* p = newExpNode(OpK);
    if (t != NULL)
        t->child[0] = p;
    if (t != NULL && p != NULL &&  token == PLUSASSIGN)
    {
        p->attr.op = token;
        match(token);
        t->child[1] = exp();
    }
    //else if (t != NULL && p != NULL) 
    //{
    //    p->attr.op = token;
    //    match(ASSIGN);
    //    t->child[1] = regex();
    //}
    else if (t != NULL)
    {
        match(ASSIGN);
        t->child[0] = exp();
    }

    return t;
}


TreeNode* read_stmt(void)
{
    TreeNode* t = newStmtNode(ReadK);
    match(READ);
    if ((t != NULL) && (token == ID))
        t->attr.name = copyString(tokenString);
    match(ID);
    return t;
}

TreeNode* write_stmt(void)
{
    TreeNode* t = newStmtNode(WriteK);
    match(WRITE);
    if (t != NULL) t->child[0] = exp();
    return t;
}

TreeNode* exp(void)
{
    TreeNode* t = simple_exp();
    if ((token == LT) || (token == EQ) || (token  == GT)  || (token == EQ)) {
        TreeNode* p = newExpNode(OpK);
        if (p != NULL) {            
            p->child[0] = t;
            p->attr.op = token;
            t = p;
        }
        match(token);
        if (t != NULL)
            t->child[1] = simple_exp();
    }
    return t;
}

TreeNode* simple_exp(void) 
{
    TreeNode* t = term();
    while ((token == PLUS) || (token == MINUS))
    {
        TreeNode* p = newExpNode(OpK);
        if (p != NULL) {
            p->child[0] = t;
            p->attr.op = token;
            t = p;
            match(token);
            t->child[1] = term();
        }
    }
    return t;
}

TreeNode* term(void)
{
    TreeNode* t = NULL;

    t = power();
    if (token == OR) 
    {
        while (token == OR) 
        {
            TreeNode* p = newExpNode(OpK);
            if (p != NULL) 
            {
                p->child[0] = t;
                p->attr.op = token;
                match(token);
                p->child[1] = orterm();
                t = p;
            }
        }
        return t;
    }

    while ((token == TIMES) || (token == OVER) || (token == MOD))
    {
        TreeNode* p = newExpNode(OpK);
        if (p != NULL) {
            p->child[0] = t;
            p->attr.op = token;
            t = p;
            match(token);
            p->child[1] = power();
        }
    }
    return t;
}


TreeNode* power(void)
{
    TreeNode* t = factor();
    if (token == AND) 
    {
        while (token == AND)
        {
            TreeNode* p = newExpNode(OpK);
            if (p != NULL)
            {
                p->child[0] = t;
                p->attr.op = token;
                match(token);
                p->child[1] = andterm();
                t = p;
            }
        }
        return t;
    }
    while (token == POWER)
    {
        TreeNode* p = newExpNode(OpK);
        if (p != NULL)
        {
            p->child[0] = t; 
            p->attr.op = token;
            t = p; 
            match(token);
            p->child[1] = factor();
        }
    }
    return t;
}

TreeNode* factor(void)
{
    TreeNode* t = NULL;
    if (token == NOT) 
    {
        // match(token);
        TreeNode* t = notterm();
        return t;
    }
    switch (token) {
    case NUM:
        t = newExpNode(ConstK);
        if ((t != NULL) && (token == NUM))
            t->attr.val = atoi(tokenString);
        match(NUM);
        break;
    case ID:
        t = newExpNode(IdK);
        if ((t != NULL) && (token == ID))
            t->attr.name = copyString(tokenString);
        match(ID);
        break;
    case LPAREN:
        match(LPAREN);
        t = exp();
        match(RPAREN);
        break;
    default:
        syntaxError("unexpected token -> ");
        printToken(token, tokenString);
        token = getToken();
        break;
    }
    return t;
}

// 下面是扩充的文法
//for
TreeNode* for_stmt(void)
{
    TreeNode* t = newStmtNode(ForK); 

    match(FOR);
    if (t != NULL) t->child[0] = assign_stmt(); 
    if (token == TO)
    {
        match(TO);
        if (t != NULL) t->child[1] = simple_exp();
    }
    else
    {
        match(DOWNTO);
        if (t != NULL) t->child[1] = simple_exp();
    }
    match(DO);
    if (t != NULL) t->child[2] = stmt_sequence();
    match(ENDDO);
    return t;
}

// NOT
TreeNode* notterm(void)
{
    bool isNot = token == NOT ? true : false;
    TreeNode* t = newExpNode(OpK);

  
    if (token == NOT) {
        t->attr.op = token;
        match(token);
        if (token != LPAREN)
            t->child[0] = factor();
    }
    if (!isNot && token != LPAREN) 
    {
        return factor();
    }

    if (token == LPAREN && token != NOT)
    {
        match(LPAREN);
        t = simple_exp();
        match(RPAREN);
    }

    return t;
}

// OR
TreeNode* orterm(void)
{
    TreeNode* t = andterm();
    while (token == AND)
    {
        TreeNode* p = newExpNode(OpK);
        if (p != NULL)
        {
            p->child[0] = t;
            p->attr.op = token;
            match(token);
            p->child[1] = andterm();
            t = p;
        }
    }
    return t;
}

//AND
TreeNode* andterm(void)
{
    TreeNode* t = NULL;
    if (token == NOT)
    {
        t = newExpNode(OpK);
        t->attr.op = token;
        match(NOT);
        t->child[0] = notterm();
    }
    else
    {
        t = notterm();
    }
    return t;
}

/****************************************/
/* the primary function of the parser   */
/****************************************/
/* Function parse returns the newly
 * constructed syntax tree
 */
TreeNode* parse(void)  // 这里是分析的入口
{
    TreeNode* t;
    token = getToken();
    t = stmt_sequence();
    if (token != ENDFILE)
        syntaxError("Code ends before file\n");
    return t;
}

/* 作者：华南师范大学 关竣佑 */
