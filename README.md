# C-Translator
### Gramatics

Program -> [Declaration]; StatSq
declaration -> "w" {("," id)};
StatSq -> Stat { ";" Stat}
Assign -> id "=" Expr
Stat -> Assign | Read | Write | if | loop
comp -> Expr [(< | > | <= | >= | != | ==) Expr]
Expr -> Mul {("+"|"-") Mul}
Mul -> Power {("*" | "/") Power}
Power -> Term ["^" Term]
Term -> VALUE | ID | "(" Expr ")"
Read -> "woor" "(" id{"," id} ")"
Write -> "woop" "(" (Expr|String) [& String]  ")"
loop -> "woooo" "(" comp ")" "{" StatSq "}"
if -> "woi" "(" comp ")" "{" StatSq "}" ["woe" "{" StatSq "}"]
