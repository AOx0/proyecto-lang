(* Un comentario *)
program HOLA (input, output);
    var numero, edad: integer;
    const NUMERO = "ass";

    function Add(a: integer; b: integer) : integer;
    begin
        Add := a + b
    end;

    procedure multadd;
    begin
        c := Add(a, b);
        d := Mult(c, a)
    end;

    function Mult(c: integer; a: integer) : integer;
    begin
        Mult := c * a
    end;

begin
    a := 5;
    b := 3;
    multadd;
    while (a and b) do
      b := b + 1;
    writeln("c");
    writeln("d")
end.
