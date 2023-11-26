program principal(input, output);
    var a,b: integer;

    procedure NumberRelation(numero1: integer; numero2: integer);
    begin
        if(numero1>numero2) then
            writeln(numero1)
        else
            begin
                if(numero1 <numero2) then
                    writeln(numero2)
                else
                    writeln(numero2)
            end
    end;

begin
    a:=5;
    b:=3;
    NumberRelation(a,b)
end.