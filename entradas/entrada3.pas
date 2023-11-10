(*
Este programa calcula c = a+b y entonces d = c * a
Tiene tres funciones
*)
program principal(input, output);
    
    var a,b,c,d: integer;

    (* Suma numeros *)
    function Add(a: integer; b: integer) : integer;
    begin
        writeln(c);
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
        c := Add(a, b);
        writeln(c);
        d := Mult(c, a)
    end;

begin
    a := 5;
    b := 3;
    multadd;
    while (a and b) do
      b := b + 1;
    writeln(c);
    writeln(d)
end.
