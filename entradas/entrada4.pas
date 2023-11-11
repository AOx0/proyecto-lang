program principal(input, output);
    
    var a,b,c,d: integer;

    function Add(a: integer; b: integer) : integer;
    begin
        Add := a + b
    end;

    function Mult(c: integer; a: integer) : integer;
    begin
        Mult := c * a
    end;

    procedure multadd;
    begin
        c := Add(a, b);
        d := Mult(c, a)
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
