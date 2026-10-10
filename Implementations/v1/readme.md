Rough compiler layout:

Reading -> Lexer -> Parser -> Semantic -> Translate to assembly

Reading: Loads the entire file into memory (bad).
Lexer: Splits the text into tokens.
Parser: Validates token structure and builds AST.
Semantic: Interprets the AST and gives IR.
