(*
Este programa calcula c = a+b y entonces d = c * a
Tiene tres funciones
*)
program principal(input, output, entrada);
    
    var a,b,c,d: integer;
    var arr: array [0..10] of integer;
    const NOMBRE = "Daniel";
    const EDAD = 21;
    const ALTURA = 1.90;

    (* Suma numeros *)
    function Add(a: integer; b: integer) : integer;
    var zz: integer;
    begin
        writeln(a, c);
        Add := a + b
    end;

    (* Multiplica numeros *)
    function Mult(z: integer; a: integer) : integer;
    begin
        write(z);
        Mult := c * a
    end;

    (* Invoca a Multiplica y Suma numeros *)
    procedure multadd;
    begin
        c := Add(c, b);
        writeln(c);
        d := Mult(c, a)
    end;

(*
    Codigo: 
        - 20% Lexicografico
        - 20% Sintactico
        - 20% Tabla de simbolos
        - 20% Arbol sintactico
        - 20% Generacion de C++
        
*)
begin
    a := 5;
    b := arr[9];
    b := 3 + Add(a, b);
    (*b := multadd();*)
    multadd();
    while (a > b) do
      b := b + 2 + 1;
    writeln(c);
    writeln(d)
end.
