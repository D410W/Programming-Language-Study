Rough compiler layout:

Reading -> Lexer -> Grammar -> Semantic -> Translate to assembly

Reading: Loads the entire file into memory (bad).
Lexer: Splits the text into tokens.
Grammar: Validates code structure and builds AST.
Semantic: Interprets the AST and gives IR.