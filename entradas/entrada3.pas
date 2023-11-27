(*
Este programa calcula c = a+b y entonces d = c * a
Tiene tres funciones
*)
program principal(input, output, entrada);
    
    var a,b,c,d,i: integer;
    var arr: array [0..10] of integer;
    const NOMBRE = "Daniel";
    const EDAD = 21;
    const ALTURA = 1.90;

    procedure printNumbers(upTo: integer);
    begin 
        for i := 0 to upTo do
            writeln("Numero: ", i)
    end;

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

    procedure NumberRelation(numero1: integer; numero2: integer);
    begin
        if(numero1>numero2) then
            writeln(numero1)
        else
            begin
                if(numero1 <numero2) then
                    writeln("Aaaa", numero2)
                else
                    writeln("Bbb", numero2, 3)
            end
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
    b := 3 + Add(a, (1 + 3));
    readln(a);
    (*b := multadd();*)
    multadd();
    while (a > b) do
      b := b + 2 + 1;
    writeln(c);
    NumberRelation(3 * 2 + 1, 3 * 3 + b);
    writeln(d)
end.
