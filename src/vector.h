//! Declaración de un vector en C que
//! puede almacenar $N$ elementos de un
//! tamaño genérico de bytes.
//!
//! El vector almacena el numero de bytes
//! que ocupa el tipo de dato para poder
//! realizar operaciones como `push`, `pop`.

#ifndef LNG_VECTOR
#define LNG_VECTOR

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// `Vec`
///
/// La estructura Vec sabe la dirección de de
/// memoria de la que es dueña, y el numero de
/// elementos de tamaño `t_size` que puede
/// almacenar y que almacena.
struct Vec {
        uint8_t *ptr;
        size_t cap;
        size_t len;
        size_t t_size;
};
typedef struct Vec Vec;

/// `VecSlice`
///
/// Como en otros lenguajes de programación, se
/// trata de una estructura que tiene acceso
/// (solo de lectura) a una fracción de los
/// bytes de un arreglo o `Vec`.
struct VecSlice {
        uint8_t *ptr;
        size_t len;
        size_t t_size;
};
typedef struct VecSlice VecSlice;

/// `vec_new`
///
/// Crear un nuevo vector que almacenará valores que tienen un tamaño de
/// bytes `t_size`.
///
/// Los vectores no reservan memoria a menos que se haga `push` de un valor(es).
/// Para crear un vector que reserva una cabtidad determinada de bytes durante
/// su creación usar `vec_with_cap` o `vec_init_with_cap`, etc.
Vec vec_new(size_t t_size);

/// `vec_new_with_cap`
///
/// Crea un vector que reserva la cantidad de memoria necesaria para almacenar
/// `cap` número de elementos.
Vec vec_flip_order(Vec *v);

/// `vec_grow`
///
/// Asegura que el vector tiene espacio para almacenar `cap` elementos
/// adicionales. Reserva mas memoria con `realloc` en caso de haber agotado el
/// espacio.
void vec_grow(Vec *v, size_t cap);

/// `vec_with_cap`
///
/// Crea un vector que reserva la cantidad de memoria necesaria para almacenar
/// `cap` número de elementos
Vec vec_with_cap(size_t t_size, size_t cap);

/// `vec_shrink`
///
/// Libera la memoria extra de un vector de
/// forma que su capacidad de almacenamiento
/// sea la misma que el numero se elementos
/// que contiene.
void vec_shrink(Vec *v);

/// `vec_init`
///
/// Inicializa una instancia de vector que puede almacenar elementos de `t_size`
/// bytes de tamaño en el apuntador indicado.
void vec_init(Vec *v, size_t t_size);

/// `vec_int_with_cap`
///
/// Inicializa un vector directamente en la dirección de memoria indicada con
/// capacidad reservada con malloc para retener `cap` numero de elementos.
void vec_init_with_cap(Vec *v, size_t t_size, size_t cap);

/// `vec_get`
///
/// Devuelve un apuntador al elemento $i$ almacenado en el vector.
///
/// *Panics*
///
/// Termina la  ejecución del programa con un mensaje de  error en caso de que
/// el indice no sea valido.
void *vec_get(Vec *v, size_t idx);

/// `vec_push`
///
/// Devuelve un apuntador a la cima de la pila de forma que el usuario pueda
/// _castear_ la dirección y escribir en ella el elemento en sí.
///
/// Si el vector no tiene la capacidad de contener un nuevo elemento, entonces
/// reserva el espacio adicional para 1 elemwnto.
void *vec_push(Vec *v);

/// `vec_remove`
///
/// Elimina el elemento en la posición `idx`, y re acomoda el resto de elementos
/// para no dejar espacios sin asignar.
/// TODO: Check bounds on index.
void vec_remove(Vec *v, size_t idx);

/// `vec_pop`
///
/// Descarta el valor en la cima de la pila como elemento activo. Realizar pop
/// no elimina el valor, solo decrementa el contador de elementos almacenados.
void *vec_pop(Vec *v);

/// `vec_pop_into`
///
/// Descarta el valor en la cima de la pila como elemento activo. Realizar pop
/// no elimina el valor, solo decrementa el contador de elementos almacenados.
///
/// A diferencia de `vec_pop`, la función también copia el valor de los bits
/// del elemento en el apuntador de destino `dst`.
void *vec_pop_into(Vec *v, void *dst);

/// `vec_debug_verbose`
///
/// Imprime el vector en `stdout`, mostrando su estado y todos los bytes
/// de los elementos que almacena.
void vec_debug_verbose(Vec *v);

/// `vec_real_size`
///
/// Devuelve el número de bytes totales que puede almacenar el vector en
/// elementos.
size_t vec_real_size(Vec *v);

/// `vec_real_len`
/// Devuelve el número de bytes totales usados para almacenar elementos.
size_t vec_real_len(Vec *v);

/// `vec_clear`
///
/// Borra todos los elementos almacenados en el vector. La función también
/// inicia todos los bytes en 0, eliminando la información previamente
/// almacenada.
void vec_clear(Vec *v);

/// `vec_extend`
///
/// Copia todos los elementos de `o` en `v`. No hace una copia profunda, sino
/// que copia los bits de cada valor.
void vec_extend(Vec *v, Vec *o);

/// `vec_drop`
///
/// Libera la memoria usada por el vector.
///
/// *Seguridad*
///
/// El vector debe ser considerado como no usable luego de ejecuar `vec_drop`
/// sobre él. Principalmente porque se elimina también la información del tipo
/// de elementos que almacena (tamaño en bytes).
void vec_drop(Vec *v);

/// `vec_last`
///
/// Devuelve un apuntador al elemento en la cima de la pila.
///
/// *Panics*
///
/// El programa termina con un mensaje de error si no contiene elementos el
/// vector
void *vec_last(Vec *v);

/// `vec_first`
///
/// Devuelve un apuntador al elemento en la parte baja de la pila.
///
/// *Panics*
///
/// El programa termina con un mensaje de error si no contiene elementos el
/// vector
void *vec_first(Vec *v);

/// `vec_top`
///
/// Alias de `vec_last`
void *vec_top(Vec *v);

/// `vec_bottom`
///
/// Alias de `vec_first`
void *vec_bottom(Vec *v);

VecSlice vec_slice_from_ptr(uint8_t *c, size_t t_size, size_t len);
VecSlice vec_slice_from_v(Vec *v);

void vec_slice_self(VecSlice *sl, size_t start, size_t end);

VecSlice vec_slice_slice(VecSlice *sl, size_t start, size_t end);
VecSlice vec_slice_slice_end(VecSlice *sl, size_t start);
void *vec_slice_get(VecSlice *sl, size_t idx);
Vec vec_slice_owned(VecSlice *sl);

#endif
