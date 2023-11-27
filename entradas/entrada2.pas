(* Un comentario *)
program programa(input, output, other);
    var a, b: integer;

    procedure NumberRelation(numero1: integer; numero2: integer);
    begin
        if(numero1>numero2) then
            writeln("El numero mayor es: ", numero1)
        else
            begin
                if(numero1 <numero2) then
                    writeln("El numero mayor es: ", numero2)
                else
                    writeln("Los numeros son iguales")
            end
    end;


begin
    NumberRelation(5 + 3, 2)
end.
